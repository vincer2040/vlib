#include "siphash.h"
#include "vlib.h"
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#define ht_padding(size)                                                       \
    ((sizeof(void*) - ((size + 8) % sizeof(void*))) & (sizeof(void*) - 1))

#define HT_INITIAL_CAP 32
#define HT_BUCKET_INITIAL_CAP 2

typedef struct ht_entry {
    size_t key_len;
    unsigned char data[];
} ht_entry;

static uint64_t ht_hash(ht* ht, void* key, size_t key_len);
static int ht_resize(ht* ht);
static int ht_init_bucket(ht_bucket* bucket);
static int ht_realloc_bucket(ht_bucket* bucket);
static void ht_bucket_remove(ht_bucket* bucket, size_t idx, FreeFn* free_key,
                             FreeFn* free_val);
static void ht_bucket_free(ht_bucket* bucket, FreeFn* free_key,
                           FreeFn* free_val);
static ht_entry* ht_entry_new(void* key, size_t key_len, void* data,
                              size_t data_size);
static void ht_entry_free(ht_entry* entry, FreeFn* free_key, FreeFn* free_val);

ht ht_new(size_t data_size) {
    ht ht = {0};
    ht.buckets = calloc(HT_INITIAL_CAP, sizeof(ht_bucket));
    assert(ht.buckets != NULL);
    ht.cap = HT_INITIAL_CAP;
    ht.len = 0;
    ht.data_size = data_size;
    get_random_bytes(ht.seed, HT_SEED_SIZE);
    return ht;
}

size_t ht_len(ht* ht) { return ht->len; }

int ht_insert(ht* ht, void* key, size_t key_len, void* value, FreeFn* fn) {
    uint64_t hash;
    ht_bucket* bucket;
    ht_entry* entry;
    size_t i, len, cap, data_size = ht->data_size;
    if (ht->len >= ht->cap) {
        if (ht_resize(ht) == -1) {
            return -1;
        }
    }
    hash = ht_hash(ht, key, key_len);
    bucket = &(ht->buckets[hash]);
    len = bucket->len;
    cap = bucket->cap;
    if (cap == 0) {
        int init_res = ht_init_bucket(bucket);
        if (init_res == -1) {
            return -1;
        }
        entry = ht_entry_new(key, key_len, value, data_size);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        ht->len++;
        return 0;
    }

    if (len == 0) {
        entry = ht_entry_new(key, key_len, value, data_size);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        ht->len++;
        return 0;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket->entries[i];
        size_t cur_key_len = cur->key_len;
        if ((key_len == cur_key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            size_t offset = key_len + ht_padding(key_len);
            void* ptr = cur->data + offset;
            if (fn) {
                fn(ptr);
            }
            memcpy(cur->data + offset, value, data_size);
            return 0;
        }
    }

    if (len == cap) {
        int realloc_bucket_res = ht_realloc_bucket(bucket);
        if (realloc_bucket_res == -1) {
            return -1;
        }
    }

    entry = ht_entry_new(key, key_len, value, data_size);
    if (entry == NULL) {
        return -1;
    }

    bucket->entries[len] = entry;
    bucket->len++;
    ht->len++;
    return 0;
}

void* ht_get(ht* ht, void* key, size_t key_len) {
    uint64_t hash = ht_hash(ht, key, key_len);
    ht_bucket bucket = ht->buckets[hash];
    size_t i, len = bucket.len, cap = bucket.cap;
    if (cap == 0) {
        return NULL;
    }
    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket.entries[i];
        size_t cur_key_len = cur->key_len;
        if ((cur_key_len == key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            size_t offset = key_len + ht_padding(key_len);
            void* ptr = cur->data + offset;
            return ptr;
        }
    }
    return NULL;
}

int ht_delete(ht* ht, void* key, size_t key_len, FreeFn* free_key,
              FreeFn* free_val) {
    uint64_t hash = ht_hash(ht, key, key_len);
    ht_bucket* bucket = &(ht->buckets[hash]);
    size_t i, len = bucket->len;
    if (len == 0) {
        return -1;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket->entries[i];
        size_t cur_key_len = cur->key_len;
        if ((cur_key_len == key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            ht_bucket_remove(bucket, i, free_key, free_val);
            ht->len--;
            return 0;
        }
    }
    return -1;
}

void ht_free(ht* ht, FreeFn* free_key, FreeFn* free_val) {
    size_t i, len = ht->cap;
    for (i = 0; i < len; ++i) {
        ht_bucket bucket = ht->buckets[i];
        ht_bucket_free(&bucket, free_key, free_val);
    }
    free(ht->buckets);
}

static uint64_t ht_hash(ht* ht, void* key, size_t key_len) {
    return siphash(key, key_len, ht->seed) % ht->cap;
}

static int ht_resize(ht* ht) {
    size_t i, old_cap = ht->cap;
    size_t new_cap = old_cap << 1;
    void* tmp = realloc(ht->buckets, new_cap * sizeof(ht_bucket));
    if (tmp == NULL) {
        return -1;
    }
    ht->buckets = tmp;
    memset(&(ht->buckets[old_cap]), 0, (new_cap - old_cap) * sizeof(ht_bucket));
    for (i = 0; i < old_cap; ++i) {
        ht_bucket* bucket = &(ht->buckets[i]);
        size_t j, len = bucket->len;
        for (j = 0; j < len; ++j) {
            ht_entry* entry = bucket->entries[j];
            size_t key_len = entry->key_len;
            uint64_t hash = ht_hash(ht, entry->data, key_len);
            if (hash == i) {
                size_t k;
                for (k = 0; k < j; ++k) {
                    ht_entry* cur = bucket->entries[k];
                    if (!cur) {
                        memmove(&(bucket->entries[k]), &(bucket->entries[j]),
                                sizeof(ht_entry*));
                        memset(&(bucket->entries[j]), 0, sizeof(ht_entry*));
                        goto next;
                    }
                }
                goto next;
            } else {
                ht_bucket* new_bucket = &(ht->buckets[hash]);
                size_t new_bucket_len = new_bucket->len,
                       new_bucket_cap = new_bucket->cap;
                if (new_bucket_cap == 0) {
                    if (ht_init_bucket(new_bucket) == -1) {
                        return -1;
                    }
                    new_bucket_cap = new_bucket->cap;
                }
                if (new_bucket->len == 0) {
                    new_bucket->entries[0] = entry;
                    memset(&(bucket->entries[j]), 0, sizeof(ht_entry*));
                    new_bucket->len++;
                    bucket->len--;
                    goto next;
                }
                if (new_bucket_len == new_bucket_cap) {
                    if (ht_realloc_bucket(new_bucket) == -1) {
                        return -1;
                    }
                }
                memmove(&(new_bucket->entries[new_bucket_len]),
                        &(bucket->entries[j]), sizeof(ht_entry*));
                memset(&(bucket->entries[j]), 0, sizeof(ht_entry*));
                new_bucket->len++;
                bucket->len--;
            }
        next:
            continue;
        }
    }
    return 0;
}

static int ht_init_bucket(ht_bucket* bucket) {
    bucket->entries = calloc(HT_BUCKET_INITIAL_CAP, sizeof(ht_entry*));
    if (bucket->entries == NULL) {
        return -1;
    }
    bucket->cap = HT_BUCKET_INITIAL_CAP;
    return 0;
}

static int ht_realloc_bucket(ht_bucket* bucket) {
    size_t len = bucket->len, new_cap = bucket->cap << 1;
    void* tmp = realloc(bucket->entries, new_cap * sizeof(ht_entry*));
    if (tmp == NULL) {
        return -1;
    }
    bucket->entries = tmp;
    memset(&(bucket->entries[len]), 0, (new_cap - len) * sizeof(ht_entry*));
    bucket->cap = new_cap;
    return 0;
}

static void ht_bucket_remove(ht_bucket* bucket, size_t idx, FreeFn* free_key,
                             FreeFn* free_val) {
    size_t len;
    ht_entry* entry = bucket->entries[idx];
    ht_entry_free(entry, free_key, free_val);
    bucket->len--;
    len = bucket->len;
    if (len == 0) {
        memset(bucket->entries, 0, sizeof(ht_entry*));
        return;
    }
    memmove(&(bucket->entries[idx]), &(bucket->entries[idx + 1]),
            (len - idx) * sizeof(ht_entry*));
    memset(&(bucket->entries[len]), 0, sizeof(ht_entry*));
}

static void ht_bucket_free(ht_bucket* bucket, FreeFn* free_key,
                           FreeFn* free_val) {
    size_t i, len = bucket->len, cap = bucket->cap;
    if (cap == 0) {
        return;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* entry = bucket->entries[i];
        ht_entry_free(entry, free_key, free_val);
    }
    if (bucket->entries) {
        free(bucket->entries);
    }
}

static ht_entry* ht_entry_new(void* key, size_t key_len, void* data,
                              size_t data_size) {
    ht_entry* entry;
    size_t offset = key_len + ht_padding(key_len);
    size_t needed = (sizeof *entry) + offset + data_size;
    entry = malloc(needed);
    if (entry == NULL) {
        return NULL;
    }
    memcpy(entry->data, key, key_len);
    memcpy(entry->data + offset, data, data_size);
    entry->key_len = key_len;
    return entry;
}

static void ht_entry_free(ht_entry* entry, FreeFn* free_key, FreeFn* free_val) {
    if (free_val) {
        size_t key_len = entry->key_len;
        size_t offset = key_len + ht_padding(key_len);
        free_val(entry->data + offset);
    }
    if (free_key) {
        free_key(entry->data);
    }
    free(entry);
}
