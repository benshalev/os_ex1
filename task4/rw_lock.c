#include "rw_lock.h"
#include "ticket_lock.h"
#include "cond_var.h"
/*
 * TODO: Implement rwlock_init.
 */
void rwlock_init(rwlock* lock) {
    // TODO: Initialize the lock structure.
    ticketlock_init(&lock-> lock);
    condition_variable_init(&lock->readers_cv);
    condition_variable_init(&lock->writers_cv);
    atomic_init(&lock->readers,0);
    atomic_init(&lock->writer_active,0);
    atomic_init(&lock->writers_waiting,0);
}

/*
 * TODO: Implement rwlock_acquire_read.
 */
void rwlock_acquire_read(rwlock* lock) {
    // TODO: Allow multiple readers while ensuring no writer is active.
    ticketlock_acquire(&lock->lock);

    while (atomic_load(&lock->writer_active) || atomic_load(&lock->writers_waiting) > 0) {
        condition_variable_wait(&lock->readers_cv, &lock->lock);
    }
    
    atomic_fetch_add(&lock->readers, 1);
    
    ticketlock_release(&lock->lock);
}

/*
 * TODO: Implement rwlock_release_read.
 */
void rwlock_release_read(rwlock* lock) {
    // TODO: Decrement the reader count.
    ticketlock_acquire(&lock->lock);

    if (atomic_fetch_sub(&lock->readers, 1) == 1) {
        if (atomic_load(&lock->writers_waiting) > 0) {
            condition_variable_signal(&lock->writers_cv);
        }
    }
    ticketlock_release(&lock->lock);
}

/*
 * TODO: Implement rwlock_acquire_write.
 */
void rwlock_acquire_write(rwlock* lock) {
    // TODO: Ensure exclusive access for writing.
    ticketlock_acquire(&lock->lock);
    atomic_fetch_add(&lock->writers_waiting,1);
    while(atomic_load(&lock->readers)>0 || atomic_load(&lock->writer_active)){
        condition_variable_wait(&lock->writers_cv,&lock->lock);
    }
    atomic_store(&lock->writer_active,1);
    atomic_fetch_sub(&lock->writers_waiting,1);
    ticketlock_release(&lock->lock);
}

/*
 * TODO: Implement rwlock_release_write.
 */
void rwlock_release_write(rwlock* lock) {
    // TODO: Release the write lock.
    ticketlock_acquire(&lock->lock);
    atomic_store(&lock->writer_active,0);
    if(atomic_load(&lock->writers_waiting) > 0){
        condition_variable_signal(&lock->writers_cv);
    }else{
        condition_variable_broadcast(&lock->readers_cv);
        

    }
    ticketlock_release(&lock->lock);
}
