#include "tas_semaphore.h"
#include <sched.h>

/*
 * TODO: Implement semaphore_init using a TAS spinlock.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    // TODO: Define the structure and initialize the semaphore.
    atomic_init(&sem->resources,initial_value);
    atomic_flag_clear(&sem->lock);
}

/*
 * TODO: Implement semaphore_wait using the TAS spinlock mechanism.
 */
void semaphore_wait(semaphore* sem) {
    // TODO: Acquire the spinlock, decrement the semaphore value, then release the spinlock.
    while(1){
        while (atomic_flag_test_and_set(&sem->lock)){ 
            sched_yield();
        }
        if(atomic_load(&sem->resources) > 0){
            atomic_fetch_sub(&sem->resources, 1);
            atomic_flag_clear(&sem->lock);
            break;
        }else{
            atomic_flag_clear(&sem->lock);
            sched_yield();
        }
    }
}

/*
 * TODO: Implement semaphore_signal using the TAS spinlock mechanism.
 */
void semaphore_signal(semaphore* sem) {
    // TODO: Acquire the spinlock, increment the semaphore value, then release the spinlock.
    while (atomic_flag_test_and_set(&sem->lock)){ 
        sched_yield();
    }
    atomic_fetch_add(&sem->resources,1);
    atomic_flag_clear(&sem->lock);
}
