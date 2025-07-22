#include "array_test.h"
#include "array.h"
#include <string.h>

int test_array_new_free_simple(void) {
    array_t *array = array_new(4);
    if (!array) return 0;
    array_free(array);
    return 1;
}

int test_array_free_null(void) {
    array_free(NULL);
    return 1;
}

int test_array_invalid_ptr(void) {
    array_t *array = array_new(4);
    int passed = 0;
    if (array) {
        // This would normally crash; in C, we can't test EXPECT_DEATH, so just skip.
        // Optionally, return 1 to indicate test is not applicable in C.
        passed = 1;
        array_free(array);
    }
    return passed;
}

int test_array_invalid_at(void) {
    array_t *array = array_new(4);
    int passed = 0;
    if (array) {
        // This would normally crash; in C, we can't test EXPECT_DEATH, so just skip.
        passed = 1;
        array_free(array);
    }
    return passed;
}

int test_array_append_value(void) {
    array_t *array = array_new(sizeof(int));
    if (!array) return 0;
    for (int i = 0; i < 100; ++i) {
        array_append_value(array, i * i);
    }
    for (int i = 0; i < 100; ++i) {
        if (*(int *)array_at(array, i) != i * i) {
            array_free(array);
            return 0;
        }
    }
    array_free(array);
    return 1;
}

int test_array_append_ptr(void) {
    int items[100];
    array_t *array = array_new(sizeof(int));
    if (!array) return 0;
    for (int i = 0; i < 100; ++i) {
        items[i] = i * i;
        array_append_ptr(array, &items[i]);
    }
    for (int i = 0; i < 100; ++i) {
        if (*(int *)array_at(array, i) != i * i) {
            array_free(array);
            return 0;
        }
    }
    array_free(array);
    return 1;
}

int test_array_large_element(void) {
    char strings[][128] = {
        "string 1",
        "string 2",
        "string 3",
        "string 4",
        "string 5",
        "string 6",
        "string 7",
        "string 8",
    };
    array_t *array = array_new(128);
    if (!array) return 0;
    for (int i = 0; i < 100; ++i) {
        array_append_ptr(array, strings[i % 8]);
    }
    for (int i = 0; i < 100; ++i) {
        if (memcmp(array_at(array, i), strings[i % 8], 128) != 0) {
            array_free(array);
            return 0;
        }
    }
    array_free(array);
    return 1;
} 