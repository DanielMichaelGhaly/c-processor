#ifndef logger_h
#define logger_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* log_file_memory;
extern FILE* log_file_registers;

void init_logger_memory(const char* filename);

void init_logger_registers(const char* filename);

void close_logger();

void log_print(const char* msg, char* file_name, int num);

void int_array_to_binary_string(int* arr, int signal , int num);

#endif