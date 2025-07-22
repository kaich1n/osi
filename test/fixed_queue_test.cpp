#include <gtest/gtest.h>

#include "AllocationTestHarness.h"

extern "C" {
#include "fixed_queue_test.h"
}

class FixedQueueTest : public AllocationTestHarness {};

TEST(FixedQueueTest, test_fixed_queue_new_free) {
    ASSERT_TRUE(test_fixed_queue_new_free());
}

TEST(FixedQueueTest, test_fixed_queue_is_empty) {
    ASSERT_TRUE(test_fixed_queue_is_empty());
}

TEST(FixedQueueTest, test_fixed_queue_length) {
    ASSERT_TRUE(test_fixed_queue_length());
}

TEST(FixedQueueTest, test_fixed_queue_capacity) {
    ASSERT_TRUE(test_fixed_queue_capacity());
}

TEST(FixedQueueTest, test_fixed_queue_enqueue_dequeue) {
    ASSERT_TRUE(test_fixed_queue_enqueue_dequeue());
}

TEST(FixedQueueTest, test_fixed_queue_try_peek_first_last) {
    ASSERT_TRUE(test_fixed_queue_try_peek_first_last());
}

TEST(FixedQueueTest, test_fixed_queue_try_remove_from_queue) {
    ASSERT_TRUE(test_fixed_queue_try_remove_from_queue());
}

TEST(FixedQueueTest, test_fixed_queue_get_enqueue_dequeue_fd) {
    ASSERT_TRUE(test_fixed_queue_get_enqueue_dequeue_fd());
}

TEST(FixedQueueTest, test_fixed_queue_register_dequeue) {
    ASSERT_TRUE(test_fixed_queue_register_dequeue());
}
