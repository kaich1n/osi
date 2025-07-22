#include "allocation_tracker_test.h"
#include "allocation_tracker.h"
#include <stdlib.h>

static const allocator_id_t allocator_id = 5;

int test_allocation_tracker_uninit_no_bad_effects(void) {
    void *dummy_allocation = malloc(4);
    allocation_tracker_uninit();
    if (allocation_tracker_resize_for_canary(4) != 4) { free(dummy_allocation); return 0; }
    allocation_tracker_notify_alloc(allocator_id, dummy_allocation, 4);
    if (allocation_tracker_expect_no_allocations() != 0) { free(dummy_allocation); return 0; }
    allocation_tracker_notify_free(allocator_id, dummy_allocation);
    if (allocation_tracker_expect_no_allocations() != 0) { free(dummy_allocation); return 0; }
    free(dummy_allocation);
    return 1;
}

int test_allocation_tracker_canaries_on(void) {
    allocation_tracker_uninit();
    allocation_tracker_init();
    size_t with_canary_size = allocation_tracker_resize_for_canary(4);
    if (with_canary_size <= 4) return 0;
    void *dummy_allocation = malloc(with_canary_size);
    void *useable_ptr = allocation_tracker_notify_alloc(allocator_id, dummy_allocation, 4);
    if (useable_ptr <= dummy_allocation) { free(dummy_allocation); return 0; }
    if (allocation_tracker_expect_no_allocations() != 4) { free(dummy_allocation); return 0; }
    void *freeable_ptr = allocation_tracker_notify_free(allocator_id, useable_ptr);
    if (freeable_ptr != dummy_allocation) { free(dummy_allocation); return 0; }
    if (allocation_tracker_expect_no_allocations() != 0) { free(dummy_allocation); return 0; }
    free(dummy_allocation);
    return 1;
} 