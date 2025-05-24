#ifndef temp_pipline_h
#define temp_pipline_h
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100

typedef struct {
    int fetch;
    int decode_start, decode_end;
    int execute_start, execute_end;
    int memory;
    int write_back;
    int completed;
} Instruction;

void initialize_instructions(Instruction instructions[], int size);

#endif