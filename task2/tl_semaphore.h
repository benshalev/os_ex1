#ifndef TL_SEMAPHORE_H
#define TL_SEMAPHORE_H

#include <stdatomic.h>


typedef struct
 {
 atomic_int ticket ;
 atomic_int cur_ticket ;
} ticket_lock ;

void ticketlock_init(ticket_lock * lock );
void ticketlock_acquire( ticket_lock * lock );
void ticketlock_release( ticket_lock * lock );

/*
 * Define the semaphore type for the Ticket Lock implementation.
 * Write your struct details in this file..
 */
typedef struct {
    // write your implementation here
    atomic_int resources;
    ticket_lock ticket;
} semaphore;

/*
 * Initializes the semaphore pointed to by 'sem' with the specified initial value.
 */
void semaphore_init(semaphore* sem, int initial_value);

/*
 * Decrements the semaphore (wait operation).
 */
void semaphore_wait(semaphore* sem);

/*
 * Increments the semaphore (signal operation).
 */
void semaphore_signal(semaphore* sem);

#endif // TL_SEMAPHORE_H
