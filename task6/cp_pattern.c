#include "cp_pattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ticket_lock.h"
#include "cond_var.h"
#include <stdbool.h>
#include "Queue.h"
#define MAX_NUMBER 1000000
bool generated[MAX_NUMBER + 1] = { false };
int numbers_produced=0;
ticket_lock global_lock;
ticket_lock print_lock;
condition_variable cond;
Queue queue;





/*
 * TODO: Implement start_consumers_producers.
 * This function should:
 *  - Print the configuration (number of consumers, producers, seed).
 *  - Seed the random number generator using srand().
 *  - Create producer and consumer threads.
 */
void start_consumers_producers(int consumers, int producers, int seed) {
    // TODO: Print configuration and start threads.
    printf("Number of Consumers: %d\n", consumers);
    printf("Number of Producers: %d\n", producers);
    printf("Seed: %d\n", seed);
    
    srand(seed);
    ticketlock_init(&global_lock);
    ticketlock_init(&print_lock);
    condition_variable_init(&cond);
    initializeQueue(&queue);

    pthread_t consumers_threads[consumers];
    pthread_t producers_threads[producers];

    for(int i=0; i<producers ; i++){
    
        pthread_create(&producers_threads[i], NULL , producer_thread_func, NULL);
        
    }

    for(int j = 0; j < consumers; j++){
         pthread_create(&consumers_threads[j], NULL , consumer_thread_func, NULL);
    }


}

void* producer_thread_func(void* arg){
   
    
    while(1){
        int generated_num = rand() % (MAX_NUMBER+1);
        ticketlock_acquire(&global_lock);

        if (numbers_produced >= MAX_NUMBER + 1) {
            ticketlock_release(&global_lock);
            condition_variable_broadcast(&cond);
            break;
        }

        if(!generated[generated_num]){
           generated[generated_num] = true;
            numbers_produced++;
            enqueue(&queue, generated_num);
        }

        ticketlock_release(&global_lock);


        char msg[100];
        sprintf(msg, "Producer %d generated number: %d\n",(int)pthread_self(), generated_num);
        print_msg(msg);

        condition_variable_signal(&cond);

        
    }
    return NULL;

}


void* consumer_thread_func(void* arg){

     while (1) {
        ticketlock_acquire(&global_lock);

       
        while (isEmpty(&queue)) {
           
            if (numbers_produced >= MAX_NUMBER + 1) {
                ticketlock_release(&global_lock);
                return NULL;
            }

            condition_variable_wait(&cond, &global_lock);
        }

      
        int num = dequeue(&queue);
        ticketlock_release(&global_lock);

       
        bool divisible = (num % 6 == 0);
        char msg[100];
        sprintf(msg, "Consumer %d checked %d. Is it divisible by 6? %s\n",
                (int)pthread_self(), num, divisible ? "True" : "False");
        print_msg(msg);

       
        condition_variable_signal(&cond);
    }


}

/*
 * TODO: Implement stop_consumers to stop all consumers threads.
 */
void stop_consumers() {
    // TODO: Stop the consumer thread with the given id.
    
}

/*
 * TODO: Implement print_msg to perform synchronized printing.
 */
void print_msg(const char* msg) {
    // TODO: Print the message ensuring output does not overlap.
    ticketlock_acquire(&print_lock);

    printf("%s", msg);

    ticketlock_release(&print_lock);
}

/*
 * TODO: Implement wait_until_producers_produced_all_numbers 
 * The function should wait until all numbers between 0 and 1,000,000 have been produced.
 */
void wait_until_producers_produced_all_numbers() {
    // TODO: Wait until production of numbers (0 to 1,000,000) is complete.
}

/*
 * TODO: Implement wait_consumers_queue_empty to wait until queue is empty, 
 * if queue is already empty - return immediately without waiting.
 */
void wait_consumers_queue_empty() {
    // TODO: Return non-zero if the consumer queue is empty.
    return 0;
}


//our implementation to a func that check if all the 3 arguments are valid positive integers
//we found out that the func atoi(a123) -> 123
int is_positive_integer(const char* str) { 
    if (str == NULL || *str == '\0') return 0;

    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0; 
        }
        str++;
    }
    return 1;
}




/*
 * TODO: Implement a main function that controls the producer-consumer process
 */
int main(int argc, char* argv[]) {
    
    // TODO: Parse arguments.
    if(argc != 4 || !is_positive_integer(argv[1]) || !is_positive_integer(argv[2]) || !is_positive_integer(argv[3])){
        printf("usage: cp pattern [consumers] [producers] [seed]\n");
        return 1;
    }
    
    int consumers = atoi(argv[1]);
    int producers = atoi(argv[2]);
    int seed = atoi(argv[3]);
    
    // TODO: Start producer-consumer process.
    start_consumers_producers(consumers, producers, seed);
    
    // TODO: Wait for threads to finish and clean up resources.


    return 0;
}

