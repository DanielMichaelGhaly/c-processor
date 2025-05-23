#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 10
#define ENTRY_SIZE 32 // Each entry is a 32-bit array

typedef struct Queue {
    int items[MAX_SIZE][ENTRY_SIZE]; // 2D array for 32-bit entries
    int front;
    int back;
    int clk;
} Queue;

void initQueue(Queue *q);

// if return value is 1, the queue is empty, else 0
int isEmpty(Queue *q);

// if return value is 1, the queue is full, else 0
int isFull(Queue *q);

void enqueue(Queue *q, int* value);

int* dequeue(Queue *q);

int* peek(Queue *q);

#endif