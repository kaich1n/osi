#include <gtest/gtest.h>
extern "C" {
#include "array_test.h"
}

TEST(ArrayTest, test_new_free_simple) {
    ASSERT_TRUE(test_array_new_free_simple());
}

TEST(ArrayTest, test_free_null) {
    ASSERT_TRUE(test_array_free_null());
}

TEST(ArrayTest, test_invalid_ptr) {
    ASSERT_TRUE(test_array_invalid_ptr());
}

TEST(ArrayTest, test_invalid_at) {
    ASSERT_TRUE(test_array_invalid_at());
}

TEST(ArrayTest, test_append_value) {
    ASSERT_TRUE(test_array_append_value());
}

TEST(ArrayTest, test_append_ptr) {
    ASSERT_TRUE(test_array_append_ptr());
}

TEST(ArrayTest, test_large_element) {
    ASSERT_TRUE(test_array_large_element());
}
