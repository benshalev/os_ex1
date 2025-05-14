#include "cond_var.h"
#include <sched.h>
#include <stdatomic.h>
#include "ticket_lock.h"


/*
 * TODO: Implement condition_variable_init.
 */
void condition_variable_init(condition_variable* cv) {
    // TODO: Initialize internal fields.
    atomic_init(&cv->waiter,0);
    atomic_init(&cv->state,0);
}

/*
 * TODO: Implement condition_variable_wait.
 */
void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock) {
    // TODO: Increase waiter count, release ext_lock, wait until signaled, then reacquire ext_lock.
    int ex = atomic_fetch_add(&cv->waiter,1);
    ticketlock_release(ext_lock);
    while(atomic_load(&cv->state) < ex){
        sched_yield();
    }
    ticketlock_acquire(ext_lock);
    atomic_fetch_sub(&cv->waiter,1);
}

/*
 * TODO: Implement condition_variable_signal.
 */
void condition_variable_signal(condition_variable* cv) {
    // TODO: Signal one waiting thread.
    if(atomic_load(&cv->waiter) > 0){
        atomic_fetch_add(&cv->state,1);
    }
}


/*
 * TODO: Implement condition_variable_broadcast.
 */
void condition_variable_broadcast(condition_variable* cv) {
    // TODO: Signal all waiting threads.
    if(atomic_load(&cv->waiter) > 0){
        int waiters = atomic_load(&cv->waiter);
        atomic_fetch_add(&cv->state,waiters);
    }
    
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
