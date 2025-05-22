#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "main.h"
#include <string.h>
#include "parser.h"
/*
Registers and Memory initialization
*/

int memory[2048][32] = {0};

const int R0[32] = {0};
int R1[32] = {0};
int R2[32] = {0};
int R3[32] = {0};
int R4[32] = {0};
int R5[32] = {0};
int R6[32] = {0};
int R7[32] = {0};
int R8[32] = {0};
int R9[32] = {0};
int R10[32] = {0};
int R11[32] = {0};
int R12[32] = {0};
int R13[32] = {0};
int R14[32] = {0};
int R15[32] = {0};
int R16[32] = {0};
int R17[32] = {0};
int R18[32] = {0};
int R19[32] = {0};
int R20[32] = {0};
int R21[32] = {0};
int R22[32] = {0};
int R23[32] = {0};
int R24[32] = {0};
int R25[32] = {0};
int R26[32] = {0};
int R27[32] = {0};
int R28[32] = {0};
int R29[32] = {0};
int R30[32] = {0};
int R31[32] = {0};

int pc[32] = {0};
int IR[32] = {0};

//int MDR[32] = {0};

int MAR[32] = {0}; 

int clock = 0;

//ALU[5], branch, memory read, memory write, register write, memory to register, shift[2]
//if for example ALU[0] = 1 then ADD for example, if branch is 1 then will branch, if shift[1] == 1 then shift right
//branch = 0 means no jump, branch = 1 means jump, branch = 2 means JEQ
int* execute(int ALU[], int branch, int memoryRead, int memoryWrite, int registerWrite, 
    int shft[] , int R1, int R2, int R3){

/* 
the return value will be an array of size 3 
where the answer of ALU or shift or the displaced pointer is in index 0,
if memory read or write then index 1 if value is 1 means will write Mem[answer] to R1 (R1 = Mem[answer])
if value is 2 means will write R1 into Mem[answer], Mem[answer] = R1
if zero means nth will be done in the memory stage. nothing done
index 2 will be set to 1 if we will write to register R1(first passed register) 
and zero means that value is already updated in PC so it is a jump instruction.  
*/
    int execution_Result[3] = {0};
    int answer = 0;
    for(int i = 0; i<5; i++){
        if(ALU[i] == 1){
            char * instruction = "";
            switch(i)
            {
                case 0: instruction = "ADD";break;
                case 1: instruction = "SUB";break;
                case 2: instruction = "MUL";break;
                case 3: instruction = "AND";break;
                case 4: instruction = "XORI";break;
            }
            answer = alu(instruction, R1, R2, R3);
            execution_Result[0] = answer;
        }
    }

    if(branch > 0){
        // switch(branch)
        // {
        //     case 1: pc = R1;break;
        //     case 2: if(R1==R2){pc = pc + 1 + R3;};break;
        // }
        execution_Result[2] = 0; 
    }

    //in memory stage
    if(memoryRead == 1){  
        answer = R2 + R3;
        execution_Result[0] = answer;
    }
    //in memory stage
    if(memoryWrite == 1){
        answer = R2 + R3;
        execution_Result[0] = answer;
    }
    //in write back stage
    if(registerWrite == 1){
        execution_Result[2] = 1;
    }

    for(int i = 0; i<2; i++){
        if(shft[i] == 1){
            char * instruction = "";
            switch(i)
            {
                case 0: instruction = "LSL";break;
                case 1: instruction = "LSR";break;
            }
            answer = shift(instruction, R1, R2, R3);
            execution_Result[0] = answer;
        }
    }
    return execution_Result;
}

int alu(char * instruction, int R1, int R2, int R3)
{
    if (strcmp(instruction, "ADD") == 0) {
        R1 = R2 + R3;
    } else if (strcmp(instruction, "SUB") == 0) {
        R1 = R2 - R3;
    } else if (strcmp(instruction, "MUL") == 0) {
        R1 = R2 * R3;
    } else if (strcmp(instruction, "AND") == 0) {
        R1 = R2 & R3;
    } else if (strcmp(instruction, "XORI") == 0) {
        R1 = R2 ^ R3;
    }
    return R1;
}

int shift(char * instruction, int R1, int R2, int R3)
{
    if(strcmp(instruction, "LSL")==0){
        R1 = R2 << R3;
    }
    else if(strcmp(instruction, "LSR")==0){
        R1 = R2 >> R3;
    }
    return R1;
}

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