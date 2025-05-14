#include <stdbool.h>
#include <stdio.h>
#include "Queue.h"
#define MAX_NUMBER 1000000

/*Queue Implimintaion*/
void initializeQueue(Queue* q)
{
    q->front = -1;
    q->rear = 0;
}


bool isEmpty(Queue* q) { return (q->front == q->rear - 1); }


bool isFull(Queue* q) { return (q->rear == MAX_NUMBER); }


void enqueue(Queue* q, int value)
{
    if (isFull(q)) {
        return;
    }
    q->items[q->rear] = value;
    q->rear++;
}


int dequeue(Queue* q)
{
    if (isEmpty(q)) {
        return -1;
    }
    q->front++;
    return q->items[q->front];
}

