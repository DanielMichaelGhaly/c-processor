#include "queue.h"

void initQueue(Queue *q){
    q ->front = -1;
    q ->back = -1;
    q -> clk = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < 32; j++) {
            initialize_instructions(&(q->items[i]), 1);
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

void enqueue(Queue *q, Instruction* value){
    if(isFull(q)){
        printf("Queue is full\n");
        return;
    }
    if(isEmpty(q)){
        q ->front = 0;
    }
    q ->back++;
    deepCopy(&(q->items[q->back]), value);
}

Instruction* dequeue(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return NULL;
    }
    Instruction* item = &(q ->items[q ->front]);
    if(q ->front == q ->back){
        q ->front = -1;
        q ->back = -1;
    }else{
        q ->front++;
    }
    return item;
}

Instruction* peek(Queue *q){
    if(isEmpty(q)){
        printf("Queue is empty\n");
        return NULL;
    }
    return &(q ->items[q ->front]);
}

void deepCopy(Instruction* dest, Instruction* src) {
    dest->fetch = src->fetch;
    dest->decode_start = src->decode_start;
    dest->decode_end = src->decode_end;
    dest->execute_start = src->execute_start;
    dest->execute_end = src->execute_end;
    dest->memory = src->memory;
    dest->write_back = src->write_back;
    dest->completed = src->completed;

    for (int i = 0; i < 5; i++) {
        dest->r1[i] = src->r1[i];
        dest->r2[i] = src->r2[i];
        dest->r3[i] = src->r3[i];
        dest->ALUsig[i] = src->ALUsig[i];
    }

    for (int i = 0; i < 13; i++) {
        dest->shamt[i] = src->shamt[i];
    }

    for (int i = 0; i < 18; i++) {
        dest->immediate[i] = src->immediate[i];
    }

    for (int i = 0; i < 28; i++) {
        dest->address[i] = src->address[i];
    }

    for (int i = 0; i < 2; i++) {
        dest->shift[i] = src->shift[i];
    }

    dest->memR = src->memR;
    dest->memW = src->memW;
    dest->regW = src->regW;
    dest->branch = src->branch;
    dest->value = src->value;
    dest->instruction = src->instruction;
}