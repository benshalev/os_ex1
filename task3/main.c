#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "../task2/tl_semaphore.h"
#include "cond_var.h"
#include <unistd.h>
#define THREADS 200



struct args
{
    semaphore *sem;
    condition_variable *cond;
    int *number;
};

void *thread_function(void *arg)
{
    struct args *args = (struct args*)arg;
    ticket_lock tl;
    ticketlock_init(&tl);
    ticketlock_acquire(&tl);
    condition_variable_wait(args->cond, &tl);

    semaphore_wait(args->sem);
    *(args->number) = *(args->number) + 1;
    semaphore_signal(args->sem);
    ticketlock_release(&tl);
    return NULL;
}


int main(void)
{
    int number = 0;
    semaphore sem;
    condition_variable cond;

    condition_variable_init(&cond);
    semaphore_init(&sem, 1);

    pthread_t threads[THREADS];
    struct args args[THREADS];

    for(int i = 0; i < THREADS; i++)
    {
        args[i].sem = &sem;
        args[i].number = &number;
        args[i].cond = &cond;
        pthread_create(&threads[i], NULL, thread_function, (void*)&args[i]);
    }

    printf("Ran all threads\n");
    sleep(1);

    condition_variable_broadcast(&cond);

    for(int i = 0; i < THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Total number is %d\n", number);

    return 0;
}