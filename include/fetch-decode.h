#ifndef fetch_decode_h
#define fetch_decode_h
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "main.h"
#include "temp_pipline.h"

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

void decode();

int access_register_file(int * reg_num);

void memory_access();

void write_back();

int signed_immediate_to_int(int* bits, int len);

#endif