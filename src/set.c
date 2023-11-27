#include "siphash.h"
#include "vlib.h"
#include <assert.h>

static uint64_t set_hash(set* ht, void* key, size_t key_len);
static int set_resize(set* set);
static void set_bucket_remove(ht_bucket* bucket, size_t idx, FreeFn* free_fn);
static int set_realloc_bucket(ht_bucket* bucket);
static int set_init_bucket(ht_bucket* bucket);
static void set_bucket_free(ht_bucket* bucket, FreeFn* free_val);

set set_new(size_t data_size, CmpFn* cmp_key) {
    set set = {0};
    set.buckets = calloc(HT_INITIAL_CAP, sizeof(ht_bucket));
    assert(set.buckets != NULL);
    set.cap = HT_INITIAL_CAP;
    set.len = 0;
    set.data_size = data_size;
    set.cmp_key = cmp_key;
    get_random_bytes(set.seed, HT_SEED_SIZE);
    return set;
}

size_t set_len(set* set) { return set->len; }

bool set_has(set* set, void* key) {
    size_t key_len = set->data_size;
    uint64_t hash = set_hash(set, key, key_len);
    ht_bucket bucket = set->buckets[hash];
    size_t i, len = bucket.len, cap = bucket.cap;
    if (cap == 0) {
        return false;
    }
    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket.entries[i];
        if (set->cmp_key) {
            int cmp = set->cmp_key(key, cur->data);
            if (cmp == 0) {
                return true;
            }
        } else {
            size_t cur_key_len = cur->key_len;
            if ((cur_key_len == key_len) &&
                (memcmp(key, cur->data, key_len) == 0)) {
                return true;
            }
        }
    }
    return false;
}

int set_insert(set* set, void* key) {
    uint64_t hash;
    ht_bucket* bucket;
    ht_entry* entry;
    size_t i, len, cap, data_size = set->data_size;
    if (set->len == set->cap) {
        if (set_resize(set) == -1) {
            return -1;
        }
    }
    hash = set_hash(set, key, data_size);
    bucket = &(set->buckets[hash]);
    len = bucket->len;
    cap = bucket->cap;
    if (cap == 0) {
        int init_res = set_init_bucket(bucket);
        if (init_res == -1) {
            return -1;
        }
        entry = ht_entry_new(key, data_size, NULL, 0);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        set->len++;
        return 0;
    }

    if (len == 0) {
        entry = ht_entry_new(key, data_size, NULL, 0);
        if (entry == NULL) {
            return -1;
        }
        bucket->entries[0] = entry;
        bucket->len++;
        set->len++;
        return 0;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket->entries[i];
        if (set->cmp_key) {
            int cmp = set->cmp_key(cur->data, key);
            if (cmp == 0) {
                return -1;
            }
        } else {
            size_t cur_key_len = cur->key_len;
            if ((data_size == cur_key_len) &&
                (memcmp(key, cur->data, data_size) == 0)) {
                return -1;
            }
        }
    }

    if (len == cap) {
        int realloc_bucket_res = set_realloc_bucket(bucket);
        if (realloc_bucket_res == -1) {
            return -1;
        }
    }

    entry = ht_entry_new(key, data_size, NULL, 0);
    if (entry == NULL) {
        return -1;
    }

    bucket->entries[len] = entry;
    bucket->len++;
    set->len++;
    return 0;
}

int set_delete(set* set, void* key, FreeFn* free_fn) {
    size_t key_len = set->data_size;
    uint64_t hash = set_hash(set, key, key_len);
    ht_bucket* bucket = &(set->buckets[hash]);
    size_t i, len = bucket->len;
    if (len == 0) {
        return -1;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* cur = bucket->entries[i];
        if (set->cmp_key) {
            int cmp = set->cmp_key(key, cur->data);
            if (cmp == 0) {
                set_bucket_remove(bucket, i, free_fn);
                set->len--;
                return 0;
            }
        } else {
            size_t cur_key_len = cur->key_len;
            if ((cur_key_len == key_len) &&
                (memcmp(key, cur->data, key_len) == 0)) {
                set_bucket_remove(bucket, i, free_fn);
                set->len--;
                return 0;
            }
        }
    }
    return -1;
}

void set_free(set* set, FreeFn* free_fn) {
    size_t i, len = set->cap;
    for (i = 0; i < len; ++i) {
        ht_bucket bucket = set->buckets[i];
        set_bucket_free(&bucket, free_fn);
    }
    free(set->buckets);
}

static uint64_t set_hash(set* set, void* key, size_t key_len) {
    return siphash(key, key_len, set->seed) % set->cap;
}

static int set_resize(set* set) {
    size_t i, old_cap = set->cap;
    size_t new_cap = old_cap << 1;
    void* tmp = realloc(set->buckets, new_cap * sizeof(ht_bucket));
    if (tmp == NULL) {
        return -1;
    }
    set->buckets = tmp;
    memset(&(set->buckets[old_cap]), 0,
           (new_cap - old_cap) * sizeof(ht_bucket));
    for (i = 0; i < old_cap; ++i) {
        ht_bucket* bucket = &(set->buckets[i]);
        size_t j, len = bucket->len;
        for (j = 0; j < len; ++j) {
            ht_entry* entry = bucket->entries[j];
            size_t key_len = entry->key_len;
            uint64_t hash = set_hash(set, entry->data, key_len);
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
                ht_bucket* new_bucket = &(set->buckets[hash]);
                size_t new_bucket_len = new_bucket->len,
                       new_bucket_cap = new_bucket->cap;
                if (new_bucket_cap == 0) {
                    if (set_init_bucket(new_bucket) == -1) {
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
                    if (set_realloc_bucket(new_bucket) == -1) {
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

static int set_init_bucket(ht_bucket* bucket) {
    bucket->entries = calloc(HT_BUCKET_INITIAL_CAP, sizeof(ht_entry*));
    if (bucket->entries == NULL) {
        return -1;
    }
    bucket->cap = HT_BUCKET_INITIAL_CAP;
    return 0;
}

static int set_realloc_bucket(ht_bucket* bucket) {
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

static void set_bucket_remove(ht_bucket* bucket, size_t idx, FreeFn* free_fn) {
    size_t len;
    ht_entry* entry = bucket->entries[idx];
    ht_entry_free(entry, free_fn, NULL);
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

static void set_bucket_free(ht_bucket* bucket, FreeFn* free_fn) {
    size_t i, len = bucket->len, cap = bucket->cap;
    if (cap == 0) {
        return;
    }

    for (i = 0; i < len; ++i) {
        ht_entry* entry = bucket->entries[i];
        ht_entry_free(entry, free_fn, NULL);
    }
    if (bucket->entries) {
        free(bucket->entries);
    }
}
