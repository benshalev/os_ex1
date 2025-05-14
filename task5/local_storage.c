#include "local_storage.h"
#include "ticket_lock.h"
#include <stdio.h>
#include <stdlib.h>    
#include <pthread.h> 

ticket_lock tls_lock;
/*
 * TODO: Define the global TLS array.
 */
tls_data_t g_tls[MAX_THREADS];


/*
 * TODO: Implement init_storage to initialize g_tls.
 */
void init_storage(void) {
    // TODO: Set all thread_id fields to -1 and data pointers to NULL.
    ticketlock_init(&tls_lock);
    ticketlock_acquire(&tls_lock);
    for(int i = 0 ; i < MAX_THREADS ; i++){
        g_tls[i].thread_id = -1;
        g_tls[i].data = NULL;
    }
    ticketlock_release(&tls_lock);
}

/*
 * TODO: Implement tls_thread_alloc to allocate a TLS entry for the calling thread.
 */
void tls_thread_alloc(void) {
    // TODO: Use your synchronization mechanism to safely allocate an entry.
    ticketlock_acquire(&tls_lock);
    int first_clear_cell = -1;
    int64_t tid = (int64_t)(uintptr_t)pthread_self();
    for(int cell = 0 ; cell<MAX_THREADS ; cell ++){
        if(tid == g_tls[cell].thread_id){
            ticketlock_release(&tls_lock);
            return;
        }
        else if(g_tls[cell].thread_id == -1 && first_clear_cell == -1){
            first_clear_cell = cell;
        }
    }
    if(first_clear_cell == -1){
        printf("thread [%lu] failed to initialize, not enough space\n", (unsigned long)tid);
        ticketlock_release(&tls_lock);
        exit (1);
    }
    else{
        g_tls[first_clear_cell].thread_id = tid;
        ticketlock_release(&tls_lock); 
    }
}

/*
 * TODO: Implement get_tls_data to retrieve the TLS data for the calling thread.
 */
void* get_tls_data(void) {
    // TODO: Search for the calling thread's entry and return its data.
    ticketlock_acquire(&tls_lock);
    int64_t tid = (int64_t)(uintptr_t)pthread_self();
    for(int cell = 0 ; cell<MAX_THREADS ; cell ++){
        if(tid == g_tls[cell].thread_id){ 
            ticketlock_release(&tls_lock);
            return g_tls[cell].data;
        }
    }
    printf("thread [%lu] hasn't been initialized in the TLS\n", (unsigned long)tid);
    ticketlock_release(&tls_lock);
    exit (2);
}

/*
 * TODO: Implement set_tls_data to update the TLS data for the calling thread.
 */
void set_tls_data(void* data) {
    // TODO: Search for the calling thread's entry and set its data.
    ticketlock_acquire(&tls_lock);
    int64_t tid = (int64_t)(uintptr_t)pthread_self();
    for(int cell = 0 ; cell<MAX_THREADS ; cell ++){
        if(tid == g_tls[cell].thread_id){ 
            g_tls[cell].data = data;
            ticketlock_release(&tls_lock);
            return;
        }
    }
    printf("thread [%lu] hasn't been initialized in the TLS\n", (unsigned long)tid);
    ticketlock_release(&tls_lock);
    exit (2);
}

/*
 * TODO: Implement tls_thread_free to free the TLS entry for the calling thread.
 */
void tls_thread_free(void) {
    // TODO: Reset the thread_id and data in the corresponding TLS entry.
    ticketlock_acquire(&tls_lock);
    int64_t tid = (int64_t)(uintptr_t)pthread_self();
    for(int cell = 0 ; cell<MAX_THREADS ; cell ++){
        if(tid == g_tls[cell].thread_id){ 
            g_tls[cell].thread_id = -1;
            g_tls[cell].data = NULL;
            ticketlock_release(&tls_lock);
            return;
        }
    }
    ticketlock_release(&tls_lock);
    
}
