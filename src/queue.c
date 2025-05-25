#include "queue.h"

void initQueue(Queue *q){
    q ->front = -1;
    q ->back = -1;
    q -> clk = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < 32; j++) {
            q->items[i][j] = 0;
        }
    }
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

void enqueue(Queue *q, int* value){
    if(isFull(q)){
        printf("Queue is full\n");
        return;
    }
    if(isEmpty(q)){
        q ->front = 0;
    }
    q ->back++;
    for(int i = 0; i<32; i++)
    {
        (q -> items[q->back])[i] = *value;
        value++;
    }
}

int* dequeue(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return NULL;
    }
    int* item = q ->items[q ->front];
    if(q ->front == q ->back){
        q ->front = -1;
        q ->back = -1;
    }else{
        q ->front++;
    }
    return item;
}

int* peek(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return NULL;
    }
    return q ->items[q ->front];
}