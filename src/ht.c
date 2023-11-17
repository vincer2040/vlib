#include "siphash.h"
#include "vlib.h"
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#define ht_padding(size)                                                       \
    ((sizeof(void*) - ((size + 16) % sizeof(void*))) & (sizeof(void*) - 1))

#define HT_INITIAL_CAP 32

typedef struct ht_entry {
    struct ht_entry* next;
    size_t key_len;
    unsigned char data[];
} ht_entry;

static inline uint64_t ht_hash(ht* ht, void* key, size_t key_len);
static ht_entry* ht_entry_new(void* key, size_t key_len, void* value,
                              size_t value_size);
static void ht_entry_free(ht_entry* entry, FreeFn* free_key, FreeFn* free_val);

ht ht_new(size_t data_size) {
    ht ht = {0};
    ht.buckets = calloc(sizeof(ht_entry*), HT_INITIAL_CAP);
    assert(ht.buckets != NULL);
    ht.cap = HT_INITIAL_CAP;
    ht.data_size = data_size;
    get_random_bytes(ht.seed, HT_SEED_SIZE);
    return ht;
}

size_t ht_len(ht* ht) {
    return ht->len;
}

int ht_insert(ht* ht, void* key, size_t key_len, void* value, FreeFn* fn) {
    size_t data_size = ht->data_size;
    uint64_t hash = ht_hash(ht, key, key_len);
    ht_entry* cur = ht->buckets[hash];
    ht_entry* prev;
    ht_entry* new_entry;
    if (!cur) {
        new_entry = ht_entry_new(key, key_len, value, data_size);
        if (new_entry == NULL) {
            return -1;
        }
        ht->buckets[hash] = new_entry;
        ht->len++;
        return 0;
    }
    while (cur != NULL) {
        size_t cur_key_len = cur->key_len;
        if ((key_len == cur_key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            size_t offset = key_len + ht_padding(key_len);
            if (fn) {
                fn(cur->data + offset);
            }
            memcpy(cur->data + offset, value, data_size);
            goto done;
        }
        prev = cur;
        cur = cur->next;
    }

    new_entry = ht_entry_new(key, key_len, value, data_size);
    if (new_entry == NULL) {
        return -1;
    }

    cur = new_entry;
    prev->next = cur;
    ht->len++;

done:
    return 0;
}

void* ht_get(ht* ht, void* key, size_t key_len) {
    uint64_t hash = ht_hash(ht, key, key_len);
    ht_entry* cur = ht->buckets[hash];
    if (!cur) {
        return NULL;
    }

    while (cur != NULL) {
        size_t cur_key_len = cur->key_len;
        if ((key_len == cur_key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            size_t offset = key_len + ht_padding(key_len);
            return cur->data + offset;
        }
        cur = cur->next;
    }
    return NULL;
}

int ht_delete(ht* ht, void* key, size_t key_len, FreeFn* free_key,
              FreeFn* free_val) {
    uint64_t hash = ht_hash(ht, key, key_len);
    ht_entry* cur = ht->buckets[hash];
    ht_entry* prev = NULL;
    if (!cur) {
        return -1;
    }
    while (cur != NULL) {
        size_t cur_key_len = cur->key_len;
        if ((key_len == cur_key_len) &&
            (memcmp(key, cur->data, key_len) == 0)) {
            if (prev) {
                prev->next = cur->next;
            } else {
                ht->buckets[hash] = cur->next;
            }
            ht_entry_free(cur, free_key, free_val);
            ht->len--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}

void ht_free(ht* ht, FreeFn* free_key, FreeFn* free_val) {
    size_t i, len = ht->cap;
    for (i = 0; i < len; ++i) {
        ht_entry* cur = ht->buckets[i];
        if (!cur) {
            continue;
        }

        while (cur != NULL) {
            ht_entry* next = cur->next;
            ht_entry_free(cur, free_key, free_val);
            cur = next;
        }
    }
    free(ht->buckets);
}

static inline uint64_t ht_hash(ht* ht, void* key, size_t key_len) {
    return siphash(key, key_len, ht->seed) % ht->cap;
}

static ht_entry* ht_entry_new(void* key, size_t key_len, void* value,
                              size_t value_size) {
    ht_entry* entry;
    size_t offset, needed;
    offset = key_len + ht_padding(key_len);
    needed = (sizeof *entry) + offset + value_size;
    entry = malloc(needed);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, needed);
    memcpy(entry->data, key, key_len);
    memcpy(entry->data + offset, value, value_size);
    entry->key_len = key_len;
    entry->next = NULL;
    return entry;
}

static void ht_entry_free(ht_entry* entry, FreeFn* free_key, FreeFn* free_val) {
    size_t offset = entry->key_len + ht_padding(entry->key_len);
    if (free_val) {
        free_val(entry->data + offset);
    }
    if (free_key) {
        free_key(entry->data);
    }
    free(entry);
}
