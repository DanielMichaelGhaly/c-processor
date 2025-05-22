#ifndef MAIN_H
#define MAIN_H

#define Base 1024;

/*
    Registers, memory and methods declaration.
*/

int memory[2048][32];
const int R0[32];
int R1[32];
int R2[32];
int R3[32];
int R4[32];
int R5[32];
int R6[32];
int R7[32];
int R8[32];
int R9[32];
int R10[32];
int R11[32];
int R12[32];
int R13[32];
int R14[32];
int R15[32];
int R16[32];
int R17[32];
int R18[32];
int R19[32];
int R20[32];
int R21[32];
int R22[32];
int R23[32];
int R24[32];
int R25[32];
int R26[32];
int R27[32];
int R28[32];
int R29[32];
int R30[32];
int R31[32];

int pc[32];
int IR[32];
int MAR[32];

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

void printArr(int * arr);

#endif