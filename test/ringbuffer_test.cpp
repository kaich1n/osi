#include <gtest/gtest.h>
extern "C" {
#include "ringbuffer_test.h"
}

TEST(RingbufferTest, test_new_simple) {
    ASSERT_TRUE(test_ringbuffer_new_simple());
}

TEST(RingbufferTest, test_insert_basic) {
    ASSERT_TRUE(test_ringbuffer_insert_basic());
}

TEST(RingbufferTest, test_insert_full) {
    ASSERT_TRUE(test_ringbuffer_insert_full());
}

TEST(RingbufferTest, test_multi_insert_delete) {
    ASSERT_TRUE(test_ringbuffer_multi_insert_delete());
}
