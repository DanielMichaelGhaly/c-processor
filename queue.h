#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 10

typedef struct Queue{
    int items[MAX_SIZE];
    int front;
    int back;
}Queue;

void initQueue(Queue *q);

// if return value is 1, the queue is empty, else 0
int isEmpty(Queue *q);

//if return value is 1, the queue is full, else 0
int isFull(Queue *q);

void enqueue(Queue *q, int value);

int dequeue(Queue *q);

int peek(Queue *q);

#endif