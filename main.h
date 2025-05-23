#ifndef MAIN_H
#define MAIN_H

#define Base 1024;

/*
    Registers, memory and methods declaration.
*/

int memory[2048][32];
int registers[35][32];
int total_instructions;

Queue fetch_queue;
Queue decode_queue;
Queue execution_queue;
Queue memory_queue;
Queue writeBack_queue;

int clock;


int* execute(int ALU[], int branch, int memoryRead, int memoryWrite, int registerWrite, 
    int shft[] , int R1, int R2, int R3);

int alu(char * instruction, int R1, int R2, int R3);

int shift(char * instruction, int R1, int R2, int R3);

int readFile(char * filePath);

#endif