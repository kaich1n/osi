#include "hash_map_test.h"
#include "hash_map.h"
#include "osi.h"
#include <string.h>
#include <stdbool.h>

static hash_index_t hash_map_fn00(const void *key) {
    hash_index_t hash_key = (hash_index_t)key;
    return hash_key;
}

static size_t g_key_free;
static void key_free_fn00(void *data) {
    g_key_free++;
}

static size_t g_data_free;
static void data_free_fn00(void *data) {
    g_data_free++;
}

int test_hash_map_new_free_simple(void) {
    hash_map_t *hash_map = hash_map_new(5, hash_map_fn00, NULL, NULL, NULL);
    if (!hash_map) return 0;
    hash_map_free(hash_map);
    return 1;
}

int test_hash_map_insert_simple(void) {
    hash_map_t *hash_map = hash_map_new(5, hash_map_fn00, NULL, NULL, NULL);
    if (!hash_map) return 0;
    struct { const char *key; const char *data; } data[] = {
        { "0", "zero" }, { "1", "one" }, { "2", "two" }, { "3", "three" },
    };
    size_t data_sz = sizeof(data)/sizeof(data[0]);
    for (size_t i = 0; i < data_sz; i++) {
        if (hash_map_size(hash_map) != i) { hash_map_free(hash_map); return 0; }
        hash_map_set(hash_map, data[i].key, (void*)data[i].data);
        if (hash_map_size(hash_map) != i + 1) { hash_map_free(hash_map); return 0; }
    }
    if (hash_map_size(hash_map) != data_sz) { hash_map_free(hash_map); return 0; }
    for (size_t i = 0; i < data_sz; i++) {
        char *val = (char *)hash_map_get(hash_map, data[i].key);
        if (!val || strcmp(data[i].data, val) != 0) { hash_map_free(hash_map); return 0; }
    }
    if (hash_map_size(hash_map) != data_sz) { hash_map_free(hash_map); return 0; }
    hash_map_free(hash_map);
    return 1;
}

int test_hash_map_insert_same(void) {
    hash_map_t *hash_map = hash_map_new(5, hash_map_fn00, NULL, NULL, NULL);
    if (!hash_map) return 0;
    struct { const char *key; const char *data; } data[] = {
        { "0", "zero" }, { "0", "one" }, { "0", "two" }, { "0", "three" },
    };
    size_t data_sz = sizeof(data)/sizeof(data[0]);
    for (size_t i = 0; i < data_sz; i++) {
        hash_map_set(hash_map, data[i].key, (void*)data[i].data);
        if (hash_map_size(hash_map) != 1) { hash_map_free(hash_map); return 0; }
    }
    if (hash_map_size(hash_map) != 1) { hash_map_free(hash_map); return 0; }
    for (size_t i = 0; i < data_sz; i++) {
        char *val = (char *)hash_map_get(hash_map, data[i].key);
        if (!val || strcmp(data[data_sz - 1].data, val) != 0) { hash_map_free(hash_map); return 0; }
    }
    hash_map_free(hash_map);
    return 1;
}

int test_hash_map_functions(void) {
    hash_map_t *hash_map = hash_map_new(5, hash_map_fn00, key_free_fn00, data_free_fn00, NULL);
    if (!hash_map) return 0;
    struct { const char *key; const char *data; } data[] = {
        { "0", "zero" }, { "1", "one" }, { "2", "two" }, { "3", "three" },
    };
    g_data_free = 0;
    g_key_free = 0;
    size_t data_sz = sizeof(data)/sizeof(data[0]);
    for (size_t i = 0; i < data_sz; i++) {
        if (hash_map_size(hash_map) != i) { hash_map_free(hash_map); return 0; }
        hash_map_set(hash_map, data[i].key, (void*)data[i].data);
    }
    if (hash_map_size(hash_map) != data_sz) { hash_map_free(hash_map); return 0; }
    if (g_data_free != 0 || g_key_free != 0) { hash_map_free(hash_map); return 0; }
    for (size_t i = 0; i < data_sz; i++) {
        char *val = (char *)hash_map_get(hash_map, data[i].key);
        if (!val || strcmp(data[i].data, val) != 0) { hash_map_free(hash_map); return 0; }
        hash_map_erase(hash_map, (void*)data[i].key);
        if (g_data_free != i + 1 || g_key_free != i + 1) { hash_map_free(hash_map); return 0; }
    }
    hash_map_free(hash_map);
    return 1;
}

struct hash_test_iter_data_s {
    const char *key;
    const char *data;
};
static struct hash_test_iter_data_s hash_test_iter_data[] = {
    { "0", "zero" }, { "1", "one" }, { "2", "two" }, { "3", "three" },
    { "elephant", "big" }, { "fox", "medium" }, { "gerbil", "small" },
};

static int hash_test_iter_found = 0;
static bool hash_test_iter_cb(hash_map_entry_t *hash_map_entry, void *context) {
    const char *key = (const char *)hash_map_entry->key;
    char *data = (char *)hash_map_entry->data;
    if (!data) return false;
    size_t hash_test_iter_data_sz = sizeof(hash_test_iter_data)/sizeof(hash_test_iter_data[0]);
    size_t i;
    for (i = 0; i < hash_test_iter_data_sz; i++) {
        if (strcmp(hash_test_iter_data[i].key, key) == 0)
            break;
    }
    if (i == hash_test_iter_data_sz) return false;
    if (context != NULL) return false;
    if (strcmp(hash_test_iter_data[i].data, data) != 0) return false;
    hash_test_iter_found++;
    return true;
}

int test_hash_map_iter(void) {
    hash_map_t *hash_map = hash_map_new(5, hash_map_fn00, key_free_fn00, data_free_fn00, NULL);
    if (!hash_map) return 0;
    g_data_free = 0;
    g_key_free = 0;
    size_t hash_test_iter_data_sz = sizeof(hash_test_iter_data)/sizeof(hash_test_iter_data[0]);
    for (size_t i = 0; i < hash_test_iter_data_sz; i++) {
        if (hash_map_size(hash_map) != i) { hash_map_free(hash_map); return 0; }
        hash_map_set(hash_map, hash_test_iter_data[i].key, (void*)hash_test_iter_data[i].data);
    }
    void *context = NULL;
    hash_test_iter_found = 0;
    hash_map_foreach(hash_map, hash_test_iter_cb, context);
    if (hash_test_iter_found != (int)hash_test_iter_data_sz) { hash_map_free(hash_map); return 0; }
    hash_map_free(hash_map);
    return 1;
} 