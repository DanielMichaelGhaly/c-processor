#ifndef temp_pipline_h
#define temp_pipline_h
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_INSTRUCTIONS 1024

typedef struct Instruction{
    int fetch;
    int decode_start, decode_end;
    int execute_start, execute_end;
    int memory;
    int write_back;
    int completed;
    int r1[5];
    int r2[5];
    int r3[5];
    int shamt[13];
    int immediate[18];
    int address[28];

    int ALUsig[5];
    int shift[2];
    int memR;
    int memW;
    int regW;
    int branch;
    int value; 
    int* instruction; 
    int line;
    int jump_backwards;
} Instruction;

void initialize_instructions(Instruction instructions[], int size);

void initialize_instruction(Instruction* instruction);

void print_Instruction(Instruction* instruction);

#endif