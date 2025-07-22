#include <gtest/gtest.h>

#include "AllocationTestHarness.h"

extern "C" {
#include "data_dispatcher_test.h"
#include "fixed_queue.h"
#include "osi.h"
}

#define DUMMY_TYPE_0 34
#define DUMMY_TYPE_1 42
#define TYPE_EDGE_CASE_ZERO 0
#define TYPE_EDGE_CASE_MAX INT_MAX

#define DUMMY_QUEUE_SIZE 10

class DataDispatcherTest : public AllocationTestHarness {};

TEST(DataDispatcherTest, test_new_free_simple) {
    ASSERT_TRUE(test_data_dispatcher_new_free_simple());
}

TEST(DataDispatcherTest, test_dispatch_single_to_nowhere) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_nowhere());
}

TEST(DataDispatcherTest, test_dispatch_single_to_single) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_single());
}

TEST(DataDispatcherTest, test_dispatch_single_to_multiple) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_multiple());
}

TEST(DataDispatcherTest, test_dispatch_single_to_default) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_default());
}

TEST(DataDispatcherTest, test_dispatch_multiple_to_single) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_multiple_to_single());
}

TEST(DataDispatcherTest, test_dispatch_multiple_to_multiple) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_multiple_to_multiple());
}

TEST(DataDispatcherTest, test_dispatch_single_to_single_reregistered) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_single_reregistered());
}

TEST(DataDispatcherTest, test_dispatch_single_to_reregistered_null) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_reregistered_null());
}

TEST(DataDispatcherTest, test_dispatch_single_to_default_reregistered_null) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_single_to_default_reregistered_null());
}

TEST(DataDispatcherTest, test_dispatch_edge_zero) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_edge_zero());
}

TEST(DataDispatcherTest, test_dispatch_edge_max) {
    ASSERT_TRUE(test_data_dispatcher_dispatch_edge_max());
}
