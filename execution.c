#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execution.h"

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