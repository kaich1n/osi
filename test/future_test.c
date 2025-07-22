#include "future_test.h"
#include "future.h"
#include "osi.h"
#include "thread.h"

static const char *pass_back_data0 = "fancy a sandwich? it's a fancy sandwich";
static const char *pass_back_data1 = "what kind of ice cream truck plays the worst christmas song of all time?";

static void post_to_future(void *context) {
    future_ready((future_t *)context, (void *)pass_back_data0);
}

int test_future_non_immediate(void) {
    future_t *future = future_new();
    if (!future) return 0;
    thread_t *worker_thread = thread_new("worker thread");
    thread_post(worker_thread, post_to_future, future);
    if (future_await(future) != pass_back_data0) {
        thread_free(worker_thread);
        return 0;
    }
    thread_free(worker_thread);
    return 1;
}

int test_future_immediate(void) {
    future_t *future = future_new_immediate((void *)pass_back_data1);
    if (!future) return 0;
    if (future_await(future) != pass_back_data1) return 0;
    return 1;
} 