#include "ticket_lock.h"
#include <sched.h>

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