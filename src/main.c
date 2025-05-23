#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "main.h"
#include <string.h>
#include "parser.h"
#include "execution.h"
#include "logger.h"

/*
Registers and Memory initialization
*/

int memory[2048][32] = {0};
int registers[35][32] = {0};
int total_instructions = 0;
int clock = 0;

//to read txt file
void readFile(char * filePath)
{
    FILE* fp = NULL;
    errno_t res = fopen_s(&fp, filePath, "r");
    if (res != 0) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LEN];
    int binary[32];
    // int line_number = 0;
    int i = 0;
    int j = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline if exists
        line[strcspn(line, "\n")] = 0;
        parse_instruction(line, binary);
        // printf("Instruction %d: ", line_number++);
        // print_binary(binary);
        if(i<1024)
        {
            for(; j<32; j++)
            {
                memory[i][j] = binary[j];
            }
            j = 0;
            i++;
        }
    }

    fclose(fp);
}

int main()
{
    init_logger("data/log.txt");
    readFile("data/test.txt");
    for(int i = 0; i < 2048; i++) {
        int_array_to_binary_string(memory[i]);
    }
    close_logger();
}