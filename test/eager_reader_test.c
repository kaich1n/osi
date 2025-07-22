#include "eager_reader_test.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "allocator.h"
#include "eager_reader.h"
#include "osi.h"
#include "semaphore.h"
#include "thread.h"

#define BUFFER_SIZE 32

static const char *small_data = "white chocolate lindor truffles";
static const char *large_data =
  "Let him make him examine and thoroughly sift everything he reads, and "
  "lodge nothing in his fancy upon simple authority and upon trust. "
  "Aristotle's principles will then be no more principles to him, than those "
  "of Epicurus and the Stoics: let this diversity of opinions be propounded "
  "to, and laid before him; he will himself choose, if he be able; if not, "
  "he will remain in doubt. "
  "   \"Che non men the saver, dubbiar m' aggrata.\" "
  "   [\"I love to doubt, as well as to know.\"--Dante, Inferno, xi. 93] "
  "for, if he embrace the opinions of Xenophon and Plato, by his own reason, "
  "they will no more be theirs, but become his own.  Who follows another, "
  "follows nothing, finds nothing, nay, is inquisitive after nothing. "
  "   \"Non sumus sub rege; sibi quisque se vindicet.\" "
  "   [\"We are under no king; let each vindicate himself.\" --Seneca, Ep.,33] "
  "let him, at least, know that he knows.  it will be necessary that he "
  "imbibe their knowledge, not that he be corrupted with their precepts; "
  "and no matter if he forget where he had his learning, provided he know "
  "how to apply it to his own use.  truth and reason are common to every "
  "one, and are no more his who spake them first, than his who speaks them "
  "after: 'tis no more according to plato, than according to me, since both "
  "he and i equally see and understand them.  bees cull their several sweets "
  "from this flower and that blossom, here and there where they find them, "
  "but themselves afterwards make the honey, which is all and purely their "
  "own, and no more thyme and marjoram: so the several fragments he borrows "
  "from others, he will transform and shuffle together to compile a work "
  "that shall be absolutely his own; that is to say, his judgment: "
  "his instruction, labour and study, tend to nothing else but to form that. ";

static semaphore_t *done;
static int pipefd[2];

static int expect_data_result = 1;
static void expect_data(eager_reader_t *reader, void *context) {
    char *data = (char *)context;
    int length = strlen(data);
    for (int i = 0; i < length; i++) {
        uint8_t byte;
        if (eager_reader_read(reader, &byte, 1) != 1) { expect_data_result = 0; break; }
        if (data[i] != byte) { expect_data_result = 0; break; }
    }
    semaphore_post(done);
}

static int expect_data_multibyte_result = 1;
static void expect_data_multibyte(eager_reader_t *reader, void *context) {
    char *data = (char *)context;
    size_t length = strlen(data);
    for (size_t i = 0; i < length;) {
        uint8_t buffer[28];
        size_t bytes_to_read = (length - i) > 28 ? 28 : (length - i);
        size_t bytes_read = eager_reader_read(reader, buffer, bytes_to_read);
        if (bytes_read > bytes_to_read) { expect_data_multibyte_result = 0; break; }
        for (size_t j = 0; j < bytes_read && i < length; j++, i++) {
            if (data[i] != buffer[j]) { expect_data_multibyte_result = 0; break; }
        }
    }
    semaphore_post(done);
}

int test_eager_reader_new_free_simple(void) {
    if (pipe(pipefd) != 0) return 0;
    done = semaphore_new(0);
    eager_reader_t *reader = eager_reader_new(pipefd[0], &allocator_malloc, BUFFER_SIZE, SIZE_MAX, "test_thread");
    if (!reader) { semaphore_free(done); return 0; }
    eager_reader_free(reader);
    semaphore_free(done);
    return 1;
}

int test_eager_reader_small_data(void) {
    if (pipe(pipefd) != 0) return 0;
    done = semaphore_new(0);
    eager_reader_t *reader = eager_reader_new(pipefd[0], &allocator_malloc, BUFFER_SIZE, SIZE_MAX, "test_thread");
    if (!reader) { semaphore_free(done); return 0; }
    thread_t *read_thread = thread_new("read_thread");
    expect_data_result = 1;
    eager_reader_register(reader, thread_get_reactor(read_thread), expect_data, (void *)small_data);
    ssize_t written = write(pipefd[1], small_data, strlen(small_data));
    if (written != (ssize_t)strlen(small_data)) { eager_reader_free(reader); thread_free(read_thread); semaphore_free(done); return 0; }
    semaphore_wait(done);
    eager_reader_free(reader);
    thread_free(read_thread);
    semaphore_free(done);
    return expect_data_result;
}

int test_eager_reader_large_data_multibyte(void) {
    if (pipe(pipefd) != 0) return 0;
    done = semaphore_new(0);
    eager_reader_t *reader = eager_reader_new(pipefd[0], &allocator_malloc, BUFFER_SIZE, SIZE_MAX, "test_thread");
    if (!reader) { semaphore_free(done); return 0; }
    thread_t *read_thread = thread_new("read_thread");
    expect_data_multibyte_result = 1;
    eager_reader_register(reader, thread_get_reactor(read_thread), expect_data_multibyte, (void *)large_data);
    ssize_t written = write(pipefd[1], large_data, strlen(large_data));
    if (written != (ssize_t)strlen(large_data)) { eager_reader_free(reader); thread_free(read_thread); semaphore_free(done); return 0; }
    semaphore_wait(done);
    eager_reader_free(reader);
    thread_free(read_thread);
    semaphore_free(done);
    return expect_data_multibyte_result;
} 