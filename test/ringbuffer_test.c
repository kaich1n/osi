#include "ringbuffer_test.h"
#include "ringbuffer.h"
#include "osi.h"
#include <string.h>
#include <stdint.h>

int test_ringbuffer_new_simple(void) {
    ringbuffer_t *rb = ringbuffer_init(4096);
    if (!rb) return 0;
    if (ringbuffer_available(rb) != 4096) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 0) { ringbuffer_free(rb); return 0; }
    ringbuffer_free(rb);
    return 1;
}

int test_ringbuffer_insert_basic(void) {
    ringbuffer_t *rb = ringbuffer_init(16);
    if (!rb) return 0;
    uint8_t buffer[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    ringbuffer_insert(rb, buffer, 10);
    if (ringbuffer_size(rb) != 10) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 6) { ringbuffer_free(rb); return 0; }
    uint8_t peek[10] = {0};
    size_t peeked = ringbuffer_peek(rb, peek, 10);
    if (ringbuffer_size(rb) != 10) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 6) { ringbuffer_free(rb); return 0; }
    if (peeked != 10) { ringbuffer_free(rb); return 0; }
    if (memcmp(buffer, peek, peeked) != 0) { ringbuffer_free(rb); return 0; }
    ringbuffer_free(rb);
    return 1;
}

int test_ringbuffer_insert_full(void) {
    ringbuffer_t *rb = ringbuffer_init(5);
    if (!rb) return 0;
    uint8_t aa[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    uint8_t bb[] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
    uint8_t peek[5] = {0};
    size_t added = ringbuffer_insert(rb, aa, 7);
    if (added != 5) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 0) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 5) { ringbuffer_free(rb); return 0; }
    added = ringbuffer_insert(rb, bb, 5);
    if (added != 0) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 0) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 5) { ringbuffer_free(rb); return 0; }
    size_t peeked = ringbuffer_peek(rb, peek, 5);
    if (peeked != 5) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 0) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 5) { ringbuffer_free(rb); return 0; }
    if (memcmp(aa, peek, peeked) != 0) { ringbuffer_free(rb); return 0; }
    ringbuffer_free(rb);
    return 1;
}

int test_ringbuffer_multi_insert_delete(void) {
    ringbuffer_t *rb = ringbuffer_init(16);
    if (!rb) return 0;
    if (ringbuffer_available(rb) != 16) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 0) { ringbuffer_free(rb); return 0; }
    uint8_t aa[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    size_t added = ringbuffer_insert(rb, aa, sizeof(aa));
    if (added != 8) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 8) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 8) { ringbuffer_free(rb); return 0; }
    uint8_t bb[] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
    ringbuffer_insert(rb, bb, sizeof(bb));
    if (ringbuffer_available(rb) != 3) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 13) { ringbuffer_free(rb); return 0; }
    uint8_t content[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
    uint8_t peek[16] = {0};
    size_t peeked = ringbuffer_peek(rb, peek, 16);
    if (peeked != 13) { ringbuffer_free(rb); return 0; }
    if (memcmp(content, peek, peeked) != 0) { ringbuffer_free(rb); return 0; }
    ringbuffer_delete(rb, sizeof(aa));
    if (ringbuffer_available(rb) != 11) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 5) { ringbuffer_free(rb); return 0; }
    uint8_t cc[] = {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};
    ringbuffer_insert(rb, cc, sizeof(cc));
    if (ringbuffer_available(rb) != 2) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 14) { ringbuffer_free(rb); return 0; }
    uint8_t content2[] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xCC, 0xCC};
    peeked = ringbuffer_peek(rb, peek, 7);
    if (peeked != 7) { ringbuffer_free(rb); return 0; }
    if (memcmp(content2, peek, peeked) != 0) { ringbuffer_free(rb); return 0; }
    memset(peek, 0, 16);
    size_t popped = ringbuffer_pop(rb, peek, 7);
    if (popped != 7) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 9) { ringbuffer_free(rb); return 0; }
    if (memcmp(content2, peek, peeked) != 0) { ringbuffer_free(rb); return 0; }
    uint8_t dd[] = { 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD };
    added = ringbuffer_insert(rb, dd, sizeof(dd));
    if (added != 8) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 1) { ringbuffer_free(rb); return 0; }
    ringbuffer_delete(rb, 16);
    if (ringbuffer_available(rb) != 16) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 0) { ringbuffer_free(rb); return 0; }
    uint8_t ae[] = { 0xAE, 0xAE, 0xAE };
    added = ringbuffer_insert(rb, ae, sizeof(ae));
    if (ringbuffer_available(rb) != 13) { ringbuffer_free(rb); return 0; }
    popped = ringbuffer_pop(rb, peek, 16);
    if (added != popped) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_available(rb) != 16) { ringbuffer_free(rb); return 0; }
    if (ringbuffer_size(rb) != 0) { ringbuffer_free(rb); return 0; }
    if (memcmp(ae, peek, popped) != 0) { ringbuffer_free(rb); return 0; }
    ringbuffer_free(rb);
    return 1;
} 