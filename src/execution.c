#include "execution.h"

int bits_to_int(const int *bits, int len) {
    int val = 0;
    for (int i = 0; i < len; ++i) {
        val = (val << 1) | bits[i];
    }
    return val;
}

int execute(Instruction* instruction) {
    int* instr = dequeue(&decode_queue);
    enqueue(&execution_queue, instr);

    int execution_result = 0;

    // Convert bit arrays to ints
    int r1_val = bits_to_int(registers[bits_to_int(instruction->r1,5)], 32);
    int r2_val = bits_to_int(registers[bits_to_int(instruction->r2,5)], 32);
    int r3_val = bits_to_int(registers[bits_to_int(instruction->r3,5)], 32);

    for(int j = 0; j < 5; j++) {
        if(instruction->ALUsig[j] == 1) {
            char *instruction1 = "";
            switch(j) {
                case 0: instruction1 = "ADD"; break;
                case 1: instruction1 = "SUB"; break;
                case 2: instruction1 = "MUL"; break;
                case 3: instruction1 = "AND"; break;
                case 4: instruction1 = "XORI"; break;
            }
            execution_result = alu(instruction1, r2_val, r3_val);
        }
    }
    if(instruction->branch > 0) {
        switch(instruction->branch) {
            case 1: int_to_bin32(r1_val, registers[32]); break;
            case 2: if(r1_val == r2_val) { 
                pc_incr(registers[32]);
                int temp_pc_val = bits_to_int(registers[32], 32);
                int_to_bin32(temp_pc_val+r3_val, registers[32]);
            } break;
        }
    }

    if(instruction->memR == 1) {
        execution_result = r2_val + bin_to_int(instruction->immediate, 18);
    }
    if(instruction->memW == 1) {
        execution_result = r2_val + bin_to_int(instruction->immediate, 18);
    }
    // if(instruction->regW == 1) {
    //     execution_result = bin_to_int(instruction->immediate, 18);
    // }

    for(int j = 0; j < 2; j++) {
        if(instruction->shift[j] == 1) {
            char *instruction2 = "";
            switch(j) {
                case 0: instruction2 = "LSL"; break;
                case 1: instruction2 = "LSR"; break;
            }
            execution_result = shifting(instruction2, r1_val, r2_val, r3_val);
        }
    }
    return execution_result;
}



int alu(char* instruction3, int R2, int R3)
{
    if (strcmp(instruction3, "ADD") == 0) {
        return R2 + R3;
    } else if (strcmp(instruction3, "SUB") == 0) {
        return R2 - R3;
    } else if (strcmp(instruction3, "MUL") == 0) {
        return R2 * R3;
    } else if (strcmp(instruction3, "AND") == 0) {
        return R2 & R3;
    } else if (strcmp(instruction3, "XORI") == 0) {
        return R2 ^ R3;
    }
    return 0;
}

int shifting(char* instruction4, int R1, int R2, int R3)
{
    if(strcmp(instruction4, "LSL")==0){
        R1 = R2 << R3;
    }
    else if(strcmp(instruction4, "LSR")==0){
        R1 = R2 >> R3;
    }
    return R1;
}
