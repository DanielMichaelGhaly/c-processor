#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "main.h"
#include <string.h>
#include "parser.h"
#include "execution.h"

/*
Registers and Memory initialization
*/

int memory[2048][32] = {0};
int registers[35][32] = {0};

int clock = 0;

//to read txt file
int readFile(char * filePath)
{
    FILE * file = fopen(filePath, "r");
    if(file == NULL){
        perror("Error opening file");
        return 1;
    }

    char line[256];
    while(fgets(line, sizeof(line), file)){
        int * processedInstruction = processLine(line);
        printArr(processedInstruction);
    }
    fclose(file);
}

void printArr(int * processInstruction)
{
    for(int i = 0; i<32; i++)
    {
        printf("%d", *processInstruction);
        processInstruction++;
    }
    printf("\n");
}

int main()
{
    readFile("test.txt");
}