#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "local_storage.h" // ודא שזה כולל את ההגדרות והפונקציות

#define NUM_THREADS 5

void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    free(arg); // שחרור הזיכרון שהוקצה עבור מספר התהליכון

    tls_thread_alloc();

    // הקצאת זיכרון חדש למידע ושמירה ב־TLS
    int* data = malloc(sizeof(int));
    *data = thread_num * 100; // ערך ייחודי לכל תהליכון
    set_tls_data((void*)data);

    // קבלת מידע ובדיקת תקינות
    int* retrieved = (int*)get_tls_data();
    if (retrieved == NULL) {
        printf("Thread %ld: Failed to retrieve TLS data\n", (long)pthread_self());
    } else {
        printf("Thread %ld: Stored %d, Retrieved %d\n",
               (long)pthread_self(), *data, *retrieved);
    }

    tls_thread_free();
    free(data); // שחרור המידע שהוקצה

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    printf("Initializing TLS storage...\n");
    init_storage();

    printf("Creating threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        int* thread_num = malloc(sizeof(int));
        *thread_num = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, thread_num) != 0) {
            perror("Failed to create thread");
            free(thread_num);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed.\n");
    return 0;
}
