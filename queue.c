#include "queue.h"
#include <stdio.h>


void initQueue(Queue *q){
    q ->front = -1;
    q ->back = -1;
}

int isEmpty(Queue *q){
    if(q ->front == -1 && q ->back == -1){
        return 1;
    }
    return 0;
}

int isFull(Queue *q){
    if(q ->back == MAX_SIZE - 1){
        return 1;
    }
    return 0;
}

void enqueue(Queue *q, int value){
    if(isFull(q)){
        printf("Queue is full\n");
        return;
    }
    if(isEmpty(q)){
        q ->front = 0;
    }
    q ->back++;
    q ->items[q ->back] = value;
}

int dequeue(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return -1;
    }
    int item = q ->items[q ->front];
    if(q ->front == q ->back){
        q ->front = -1;
        q ->back = -1;
    }else{
        q ->front++;
    }
    return item;
}

int peek(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return -1;
    }
    return q ->items[q ->front];
}