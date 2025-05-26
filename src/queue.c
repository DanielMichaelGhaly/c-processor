#include "queue.h"

void initQueue(Queue *q) {
    q->front = -1;
    q->back = -1;
    q->clk = 0;
    // Initialize array of pointers to NULL
    for (int i = 0; i < MAX_SIZE; i++) {
        q->items[i] = NULL;
    }
}

int isEmpty(Queue *q) {
    return (q->front == -1 && q->back == -1);
}

int isFull(Queue *q) {
    return (q->back == MAX_SIZE - 1);
}

void enqueue(Queue *q, Instruction* value) {
    if(isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    if(isEmpty(q)) {
        q->front = 0;
    }
    q->back++;
    q->items[q->back] = value;  // Store pointer directly
}

Instruction* dequeue(Queue *q) {
    if(isEmpty(q)) {
        printf("Queue is empty\n");
        return NULL;
    }
    Instruction* item = q->items[q->front];
    if(q->front == q->back) {
        q->front = -1;
        q->back = -1;
    } else {
        q->front++;
    }
    return item;
}

Instruction* peek(Queue *q) {
    if(isEmpty(q)) {
        printf("Queue is empty\n");
        return NULL;
    }
    return q->items[q->front];
}

// Keep deepCopy for when you need to create actual copies
void deepCopy(Instruction* dest, Instruction* src) {
    if (dest == NULL || src == NULL) {
        return;
    }
    *dest = *src;  // Copy the basic structure

    // Copy arrays manually to ensure deep copy
    memcpy(dest->r1, src->r1, sizeof(src->r1));
    memcpy(dest->r2, src->r2, sizeof(src->r2));
    memcpy(dest->r3, src->r3, sizeof(src->r3));
    memcpy(dest->shamt, src->shamt, sizeof(src->shamt));
    memcpy(dest->immediate, src->immediate, sizeof(src->immediate));
    memcpy(dest->address, src->address, sizeof(src->address));
    memcpy(dest->ALUsig, src->ALUsig, sizeof(src->ALUsig));
    memcpy(dest->shift, src->shift, sizeof(src->shift));
}