#include <stdbool.h>
#include <stdio.h>
#define MAX_NUMBER 1000000
typedef struct {
    int items[MAX_NUMBER];
    int front;
    int rear;
} Queue;

void initializeQueue(Queue* q);
bool isEmpty(Queue* q);
bool isFull(Queue* q);
void enqueue(Queue* q, int value);
int dequeue(Queue* q);
