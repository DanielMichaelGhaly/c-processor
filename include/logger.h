#ifndef logger_h
#define logger_h
#include <stdio.h>

extern FILE* log_file;

void init_logger(const char* filename);

void close_logger();

void log_print(const char* msg);

void int_array_to_binary_string(int* arr);

#endif