#include "fixed_queue_test.h"
#include "allocator.h"
#include "fixed_queue.h"
#include "future.h"
#include "osi.h"
#include "thread.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

static const size_t TEST_QUEUE_SIZE = 10;
static const char *DUMMY_DATA_STRING = "Dummy data string";
static const char *DUMMY_DATA_STRING1 = "Dummy data string1";
static const char *DUMMY_DATA_STRING2 = "Dummy data string2";
static const char *DUMMY_DATA_STRING3 = "Dummy data string3";
static future_t *received_message_future = NULL;

static int is_fd_readable(int fd) {
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_SET(fd, &rfds);
    int result = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
    if (result < 0) return 0;
    return FD_ISSET(fd, &rfds);
}

static void fixed_queue_ready(fixed_queue_t *queue, void *context) {
    void *msg = fixed_queue_try_dequeue(queue);
    if (!msg) return;
    future_ready(received_message_future, msg);
}

int test_fixed_queue_new_free(void) {
    fixed_queue_t *queue;
    queue = fixed_queue_new(0);
    if (!queue) return 0;
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new(1);
    if (!queue) return 0;
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new((size_t)-1);
    if (!queue) return 0;
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    fixed_queue_free(queue, NULL);
    fixed_queue_free(NULL, NULL);
    fixed_queue_free(NULL, osi_free);
    return 1;
}

int test_fixed_queue_is_empty(void) {
    fixed_queue_t *queue;
    if (!fixed_queue_is_empty(NULL)) return 0;
    queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    if (!fixed_queue_is_empty(queue)) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING);
    if (fixed_queue_is_empty(queue)) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_dequeue(queue) != DUMMY_DATA_STRING) { fixed_queue_free(queue, NULL); return 0; }
    if (!fixed_queue_is_empty(queue)) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_length(void) {
    fixed_queue_t *queue;
    if (fixed_queue_length(NULL) != 0) return 0;
    queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    if (fixed_queue_length(queue) != 0) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING);
    if (fixed_queue_length(queue) != 1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_dequeue(queue) != DUMMY_DATA_STRING) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_length(queue) != 0) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_capacity(void) {
    fixed_queue_t *queue;
    queue = fixed_queue_new(0);
    if (!queue) return 0;
    if (fixed_queue_capacity(queue) != 0) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new(1);
    if (!queue) return 0;
    if (fixed_queue_capacity(queue) != 1) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new((size_t)-1);
    if (!queue) return 0;
    if (fixed_queue_capacity(queue) != (size_t)-1) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    if (fixed_queue_capacity(queue) != TEST_QUEUE_SIZE) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_enqueue_dequeue(void) {
    fixed_queue_t *queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING);
    if (fixed_queue_length(queue) != 1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_dequeue(queue) != DUMMY_DATA_STRING) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_length(queue) != 0) { fixed_queue_free(queue, NULL); return 0; }
    if (!fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING)) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_length(queue) != 1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_dequeue(queue) != DUMMY_DATA_STRING) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_length(queue) != 0) { fixed_queue_free(queue, NULL); return 0; }
    for (size_t i = 0; i < TEST_QUEUE_SIZE; i++) {
        if (!fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING)) { fixed_queue_free(queue, NULL); return 0; }
    }
    if (fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING)) { fixed_queue_free(queue, NULL); return 0; }
    for (size_t i = 0; i < TEST_QUEUE_SIZE; i++) {
        if (fixed_queue_try_dequeue(queue) != DUMMY_DATA_STRING) { fixed_queue_free(queue, NULL); return 0; }
    }
    if (fixed_queue_try_dequeue(queue) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_dequeue(NULL) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_try_peek_first_last(void) {
    fixed_queue_t *queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    if (fixed_queue_try_peek_first(NULL) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_last(NULL) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_first(queue) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_last(queue) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING1);
    if (fixed_queue_try_peek_first(queue) != DUMMY_DATA_STRING1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_last(queue) != DUMMY_DATA_STRING1) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING2);
    if (fixed_queue_try_peek_first(queue) != DUMMY_DATA_STRING1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_last(queue) != DUMMY_DATA_STRING2) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING3);
    if (fixed_queue_try_peek_first(queue) != DUMMY_DATA_STRING1) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_peek_last(queue) != DUMMY_DATA_STRING3) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_try_remove_from_queue(void) {
    fixed_queue_t *queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    if (fixed_queue_try_remove_from_queue(NULL, (void *)DUMMY_DATA_STRING) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_remove_from_queue(queue, (void *)DUMMY_DATA_STRING) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING1);
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING2);
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING3);
    if (fixed_queue_length(queue) != 3) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_remove_from_queue(queue, (void *)DUMMY_DATA_STRING2) != DUMMY_DATA_STRING2) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_length(queue) != 2) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_remove_from_queue(queue, (void *)DUMMY_DATA_STRING2) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    if (fixed_queue_try_remove_from_queue(queue, (void *)DUMMY_DATA_STRING) != NULL) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_get_enqueue_dequeue_fd(void) {
    fixed_queue_t *queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    int enqueue_fd = fixed_queue_get_enqueue_fd(queue);
    int dequeue_fd = fixed_queue_get_dequeue_fd(queue);
    if (enqueue_fd < 0 || dequeue_fd < 0) { fixed_queue_free(queue, NULL); return 0; }
    if (enqueue_fd >= FD_SETSIZE || dequeue_fd >= FD_SETSIZE) { fixed_queue_free(queue, NULL); return 0; }
    if (!is_fd_readable(enqueue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    if (is_fd_readable(dequeue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING);
    if (!is_fd_readable(enqueue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    if (!is_fd_readable(dequeue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_dequeue(queue);
    for (size_t i = 0; i < TEST_QUEUE_SIZE; i++) {
        if (!fixed_queue_try_enqueue(queue, (void *)DUMMY_DATA_STRING)) { fixed_queue_free(queue, NULL); return 0; }
    }
    if (is_fd_readable(enqueue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    if (!is_fd_readable(dequeue_fd)) { fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_free(queue, NULL);
    return 1;
}

int test_fixed_queue_register_dequeue(void) {
    fixed_queue_t *queue = fixed_queue_new(TEST_QUEUE_SIZE);
    if (!queue) return 0;
    received_message_future = future_new();
    if (!received_message_future) { fixed_queue_free(queue, NULL); return 0; }
    thread_t *worker_thread = thread_new("test_fixed_queue_worker_thread");
    if (!worker_thread) { future_free(received_message_future); fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_register_dequeue(queue, thread_get_reactor(worker_thread), fixed_queue_ready, NULL);
    fixed_queue_enqueue(queue, (void *)DUMMY_DATA_STRING);
    const char *msg = (const char *)future_await(received_message_future);
    if (msg != DUMMY_DATA_STRING) { fixed_queue_unregister_dequeue(queue); thread_free(worker_thread); future_free(received_message_future); fixed_queue_free(queue, NULL); return 0; }
    fixed_queue_unregister_dequeue(queue);
    thread_free(worker_thread);
    fixed_queue_free(queue, NULL);
    return 1;
} 