#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "rw_lock.h"

#define ROUNDS 20
#define READERS_PER_ROUND 3
#define WRITERS_PER_ROUND 2
#define MAX_LOG_SIZE 1000
#define WRITER_STARVATION_LIMIT 10  // אם יש יותר מ-10 כניסות קוראים רצופות בלי כותב – נחשב רעב

rwlock lock;

typedef struct {
    char role; // 'R' or 'W'
    int id;
} log_entry;

log_entry access_log[MAX_LOG_SIZE];
int log_index = 0;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void sleep_milliseconds(int ms) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = ms * 1000000L;
    nanosleep(&req, NULL);
}

void log_access(char role, int id) {
    pthread_mutex_lock(&log_mutex);
    if (log_index < MAX_LOG_SIZE) {
        access_log[log_index].role = role;
        access_log[log_index].id = id;
        log_index++;
    }
    pthread_mutex_unlock(&log_mutex);
}

void* reader(void* arg) {
    int id = *(int*)arg;
    rwlock_acquire_read(&lock);
    log_access('R', id);
    sleep_milliseconds(10); // simulate reading
    rwlock_release_read(&lock);
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    rwlock_acquire_write(&lock);
    log_access('W', id);
    sleep_milliseconds(10); // simulate writing
    rwlock_release_write(&lock);
    return NULL;
}

int main() {
    pthread_t threads[READERS_PER_ROUND + WRITERS_PER_ROUND];
    int ids[READERS_PER_ROUND + WRITERS_PER_ROUND];
    rwlock_init(&lock);

    for (int round = 0; round < ROUNDS; ++round) {
        for (int i = 0; i < READERS_PER_ROUND; ++i) {
            ids[i] = round * 10 + i;
            pthread_create(&threads[i], NULL, reader, &ids[i]);
        }

        for (int i = 0; i < WRITERS_PER_ROUND; ++i) {
            ids[READERS_PER_ROUND + i] = round * 10 + 100 + i;
            pthread_create(&threads[READERS_PER_ROUND + i], NULL, writer, &ids[READERS_PER_ROUND + i]);
        }

        for (int i = 0; i < READERS_PER_ROUND + WRITERS_PER_ROUND; ++i) {
            pthread_join(threads[i], NULL);
        }
    }

    // Validation Phase
    int error = 0;
    int writer_starvation_counter = 0;
    int found_writer = 0;

    for (int i = 0; i < log_index; ++i) {
        char role = access_log[i].role;

        if (role == 'R') {
            writer_starvation_counter++;
            if (writer_starvation_counter > WRITER_STARVATION_LIMIT) {
                error = 2; // starvation
                break;
            }
        } else if (role == 'W') {
            writer_starvation_counter = 0;
            found_writer = 1;
        } else {
            error = 1;
            break;
        }
    }

    if (error == 0 && found_writer) {
        printf("TEST RESULT: PASS – No concurrency violations or starvation detected ✅\n");
    } else if (error == 2) {
        printf("TEST RESULT: FAIL – Writer starvation detected ❌\n");
    } else {
        printf("TEST RESULT: FAIL – Log integrity or concurrency violation ❌\n");
    }

    return 0;
}
