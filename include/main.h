#ifndef MAIN_H
#define MAIN_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "fetch-decode.h"
#include <string.h>
#include "parser.h"
#include "execution.h"
#include "logger.h"
#include "temp_pipline.h"

#define Base 1024;
extern Instruction instructions[MAX_INSTRUCTIONS];
extern int completed;

/*
    Registers, memory and methods declaration.
*/

int memory[2048][32];
extern int registers[33][32];
int total_instructions;

extern Queue fetch_queue;
extern Queue decode_queue;
extern Queue execution_queue;
extern Queue memory_queue;
extern Queue writeBack_queue;

int clock;
int res_exec;

void readFile(char * filePath);
void printArr(int* arr, int size);
void print_pipeline_stage(const char* stage, Instruction* instr);
void print_register_updates(int reg_num, int old_value, int new_value);
void print_memory_updates(int address, int old_value, int new_value);

#endif