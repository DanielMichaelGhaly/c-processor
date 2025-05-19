#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define MAX_SIZE 10

typedef struct Queue{
    int items[MAX_SIZE];
    int front;
    int back;
};

void initQueue(Queue *q);

bool isEmpty(Queue *q);

bool isFull(Queue *q);

void enqueue(Queue *q, int value);

int dequeue(Queue *q);

int peek(Queue *q);

#endif