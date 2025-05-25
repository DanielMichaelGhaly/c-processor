#ifndef parser_h
#define parser_h
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define MEMORY_SIZE 2048
#define WORD_SIZE 32
#define MAX_LINE_LEN 256

extern const char* instruction_names[12];

int get_opcode(const char* instruction);

int parse_register(const char* reg);

void write_binary(int* binary, int addr, int start_bit, int end_bit);

void parse_instruction(const char* line, int* binary);

void print_binary(int* binary);

#endif