#ifndef execution_h
#define execution_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fetch-decode.h"
#include "temp_pipline.h"

// int* execute(int ALU[], int shft[], int R1[], int R2[], int R3[]);

// int alu(char * instruction, int R1, int R2, int R3);

// int shifting(char * instruction, int R1, int R2, int R3);

int execute(Instruction* instruction);
int alu(char* instruction3,int R2, int R3);
int shifting(char* instruction4, int R1, int R2, int R3);

#endif