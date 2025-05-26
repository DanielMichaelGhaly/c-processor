#ifndef fetch_decode_h
#define fetch_decode_h
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "main.h"
#include "temp_pipline.h"


// typedef struct {
//     int ALUsig[5];
//     int shift[2];
//     int memR;
//     int memW;
//     int regW;
//     int branch;
// } ControlSignals;

// typedef struct {
//     int r1;
//     int r2;
//     int r3;
//     int shamt;
//     int immediate;
//     int address;
// } DecodedFields;

Queue fetch_queue;
Queue decode_queue;
Queue execution_queue;
Queue memory_queue;
Queue writeBack_queue;

void initialize_with_zeros(int *arr, int size);

int * int_to_bin32(int value, int *arr);

int bin_to_int(int *arr, int n);

int * pc_incr(int* pc);

void fetch(int* pc, Instruction* instruction);

void decode(Instruction* instruction);

int access_register_file(int * reg_num);

void memory_access(Instruction* instruction,int d);

void write_back(Instruction* instruction, int d);

int signed_immediate_to_int(int* bits, int len);

#endif