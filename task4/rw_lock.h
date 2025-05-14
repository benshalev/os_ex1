#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <stdatomic.h>



/*
 * Define the read-write lock type.
 * Write your struct details in this file..
 */
typedef struct {
    // write your implementation here
    atomic_int writer_active;
    ticket_lock lock;
    atomic_int readers;
    atomic_int writers_waiting;
    condition_variable readers_cv;
    condition_variable writers_cv;
} rwlock;

/*
 * Initializes the read-write lock.
 */
void rwlock_init(rwlock* lock);

/*
 * Acquires the lock for reading.
 */
void rwlock_acquire_read(rwlock* lock);

/*
 * Releases the lock after reading.
 */
void rwlock_release_read(rwlock* lock);

/*
 * Acquires the lock for writing. This operation should ensure exclusive access.
 */
void rwlock_acquire_write(rwlock* lock);

/*
 * Releases the lock after writing.
 */
void rwlock_release_write(rwlock* lock);

#endif // RW_LOCK_H
