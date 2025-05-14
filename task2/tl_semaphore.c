#include "tl_semaphore.h"
#include <sched.h>


/*
 * TODO: Implement semaphore_init for the Ticket Lock semaphore.
 */
void semaphore_init(semaphore* sem, int initial_value) {
    // TODO: Define the structure and initialize the semaphore.
    atomic_init(&sem->resources,initial_value);
    ticketlock_init(&sem->ticket);
}

/*
 * TODO: Implement semaphore_wait using the Ticket Lock mechanism.
 */
void semaphore_wait(semaphore* sem) {
    // TODO: Obtain a ticket and wait until it is your turn; then decrement the semaphore value.
    while(1){
        ticketlock_acquire(&sem -> ticket );
        if(atomic_load(&sem->resources)==0){
            ticketlock_release(&sem->ticket);
            sched_yield();
        }else{
            atomic_fetch_sub(&sem->resources, 1);
            ticketlock_release(&sem->ticket);
            break;
        }        
    }


}

/*
 * TODO: Implement semaphore_signal using the Ticket Lock mechanism.
 */
void semaphore_signal(semaphore* sem) {
    // TODO: Increment the semaphore value and allow the next ticket holder to proceed.
    ticketlock_acquire(&sem -> ticket );
    atomic_fetch_add(&sem->resources,1);
    ticketlock_release(&sem->ticket);
}

void ticketlock_init (ticket_lock* lock ){
    atomic_init(&lock -> ticket , 0);
    atomic_init(&lock -> cur_ticket , 0);

}

void ticketlock_acquire( ticket_lock * lock ){
    int my_ticket = atomic_fetch_add (& lock -> ticket , 1) ;
    while ( atomic_load (& lock -> cur_ticket ) != my_ticket )
 {
 sched_yield () ;
 }
}

void ticketlock_release ( ticket_lock * lock )
 {
 atomic_fetch_add (& lock -> cur_ticket , 1) ;
 }

