#ifndef execution_h
#define execution_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fetch-decode.h"
#include "temp_pipline.h"
#include "main.h"

Instruction instructions[MAX_INSTRUCTIONS];

// int* execute(int ALU[], int shft[], int R1[], int R2[], int R3[]);

// int alu(char * instruction, int R1, int R2, int R3);

// int shifting(char * instruction, int R1, int R2, int R3);

int execute();
int alu(char* instruction,int R2, int R3);
int shifting(char* instruction, int R1, int R2, int R3);
void flush_Queues();
#endif