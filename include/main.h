#ifndef MAIN_H
#define MAIN_H

#define Base 1024;

/*
    Registers, memory and methods declaration.
*/

int memory[2048][32];
int registers[35][32];
int total_instructions;

Queue fetch_queue;
Queue decode_queue;
Queue execution_queue;
Queue memory_queue;
Queue writeBack_queue;

int clock;
void readFile(char * filePath);

#endif