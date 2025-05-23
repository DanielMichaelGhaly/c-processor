#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execution.h"
#include "main.h"
#include "fetch-decode.h"
int bits_to_int(const int *bits, int len) {
    int val = 0;
    for (int i = 0; i < len; ++i) {
        val = (val << 1) | bits[i];
    }
    return val;
}

int* execute(int ALU[], int branch,int shft[], int R1[], int R2[], int R3[]) {

    int execution_Result[3] = {0};
    int answer = 0;

    // Convert bit arrays to ints
    int r1_val = bits_to_int(R1, 32);
    int r2_val = bits_to_int(R2, 32);
    int r3_val = bits_to_int(R3, 32);

    for(int i = 0; i < 5; i++) {
        if(ALU[i] == 1) {
            char *instruction = "";
            switch(i) {
                case 0: instruction = "ADD"; break;
                case 1: instruction = "SUB"; break;
                case 2: instruction = "MUL"; break;
                case 3: instruction = "AND"; break;
                case 4: instruction = "XORI"; break;
            }
            answer = alu(instruction, r1_val, r2_val, r3_val);
            execution_Result[0] = answer;
        }
    }
    if(branch > 0) {
        switch(branch) {
            case 1: int_to_bin32(r1_val, &registers[32]); break;
            case 2: if(r1_val == r2_val) { 
                pc_incr(&registers[32]);
                int temp_pc_val = bits_to_int(&registers[32], 32);
                int_to_bin32(temp_pc_val+r3_val, &registers[32]);
            } break;
        }
        execution_Result[2] = 0;
    }

    // if(memoryRead == 1) {
    //     answer = r2_val + r3_val;
    //     execution_Result[0] = answer;
    // }
    // if(memoryWrite == 1) {
    //     answer = r2_val + r3_val;
    //     execution_Result[0] = answer;
    // }
    // if(registerWrite == 1) {
    //     execution_Result[2] = 1;
    // }

    for(int i = 0; i < 2; i++) {
        if(shft[i] == 1) {
            char *instruction = "";
            switch(i) {
                case 0: instruction = "LSL"; break;
                case 1: instruction = "LSR"; break;
            }
            answer = shift(instruction, r1_val, r2_val, r3_val);
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
