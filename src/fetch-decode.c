#include "fetch-decode.h"

// int r1[5]= {0};
// int r2[5] = {0};
// int r3[5] = {0};
// int shamt[13] = {0};
// int immediate[18] = {0};
// int address[28] = {0};

// int ALUsig[5] = {0};
// int shift[2] = {0};
// int memR = 0;
// int memW = 0;
// int regW = 0;
// int branch = 0;

void initialize_with_zeros(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}


int * int_to_bin32(int value, int *arr) {
    for (int i = 0; i < 32; ++i) {
        arr[i] = (value >> (31 - i)) & 1;
    }
    return arr;
}

int bin_to_int(int *arr, int n) {
    int value = 0;
    for (int i = 0; i < n; ++i) {
        value |= (arr[i] & 1) << (n - 1 - i);
    }
    return value;
}

int * pc_incr(int* pc) {
    return int_to_bin32(bin_to_int(pc, 32) + 1, pc);
}

void fetch(int* pc, Instruction* instruction) {
    if(bin_to_int(pc, 32) == total_instructions) {
        printf("PC at end\n");
        return;
    }
    int *IR = (int *) memory[bin_to_int(registers[32], 32)];
    int pc_val = bin_to_int(registers[32], 32);
    pc_val++;
    int_to_bin32(pc_val, registers[32]);
    instruction->instruction = IR;
    instruction->line = pc_val;
    enqueue(&fetch_queue, instruction);
}

void decode() {
    if(isEmpty(&fetch_queue))
    {
        printf("No instruction to decode\n");
        return;
    }
    Instruction* instruction = dequeue(&fetch_queue);
    int* instr = instruction->instruction;
    enqueue(&decode_queue, instruction);

    instruction->instruction = instr;

    int opcode = (instr[0] << 3) | (instr[1] << 2) | (instr[2] << 1) | instr[3];
    initialize_with_zeros(instruction->shift, 2);
    initialize_with_zeros(instruction->ALUsig, 5);
    initialize_with_zeros(instruction->shamt, 13);
    initialize_with_zeros(instruction->immediate, 18);
    initialize_with_zeros(instruction->address, 28);
    instruction->memR = 0;
    instruction->memW = 0;
    instruction->regW = 0;
    instruction->branch = 0;


    switch (opcode) {
        case 0x0: case 0x1: case 0x2: case 0x5: case 0x8: case 0x9: {
            // Extract r1, r2, r3 as 5-bit register numbers
            instruction->r1[0] = instr[4] ; instruction->r1[1] = instr[5]; instruction->r1[2] = instr[6]; instruction->r1[3] = instr[7]; instruction->r1[4] = instr[8];
            instruction->r2[0] = instr[9]; instruction->r2[1] = instr[10]; instruction->r2[2] = instr[11]; instruction->r2[3] = instr[12]; instruction->r2[4] = instr[13];
            instruction->r3[0] = instr[14]; instruction->r3[1] = instr[15]; instruction->r3[2] = instr[16]; instruction->r3[3] = instr[17]; instruction->r3[4] = instr[18];
            for (int j = 0; j < 13; j++) {
                instruction->shamt[j] = (instr[19 +j]);
            }

            switch (opcode) {
                case 0x0: instruction->ALUsig[0] = 1; break;
                case 0x1: instruction->ALUsig[1] = 1; break;
                case 0x2: instruction->ALUsig[2] = 1; break;
                case 0x5: instruction->ALUsig[3] = 1; break;
                case 0x8: instruction->shift[0] = 1; break;
                case 0x9: instruction->shift[1] = 1; break;
            }
            instruction->regW = 1;
            break;
        }
        case 0x3: case 0x4: case 0x6: case 0xA: case 0xB: {
            instruction->r1[0] = instr[4] ; instruction->r1[1] = instr[5]; instruction->r1[2] = instr[6]; instruction->r1[3] = instr[7]; instruction->r1[4] = instr[8];
            instruction->r2[0] = instr[9]; instruction->r2[1] = instr[10]; instruction->r2[2] = instr[11]; instruction->r2[3] = instr[12]; instruction->r2[4] = instr[13];
            for (int j = 0; j < 18; j++) {
                instruction->immediate[j]= (instr[14 + j]);
            }
            if (instruction->immediate[0] == 1) {
                instruction->value = signed_immediate_to_int(instruction->immediate, 18);
            } else {
                instruction->value = bin_to_int(instruction->immediate, 18);
            }
            switch (opcode) {
                case 0x3: instruction->regW = 1; break;
                case 0x4: instruction->branch = 2; break;
                case 0x6: instruction->ALUsig[4] = 1; instruction->regW = 1; break;
                case 0xA: instruction->regW = 1; instruction->memR = 1; break;
                case 0xB: instruction->memW = 1; break;
            }
            break;
        }
        case 0x7: {
            for (int j = 0; j < 28; j++) {
                instruction->address[j]= (instr[4 + j]);
            }
            instruction->branch = 1;
            break;
        }
        default:
            break;
    }
}

int access_register_file(int * reg_num) {
    int * data = registers[bin_to_int(reg_num, 5)];
    return bin_to_int(data, 32);
}

void memory_access() {
    if(isEmpty(&execution_queue))
    {
        printf("No instruction to memory access\n");
        return;
    }
    Instruction* instruction = dequeue(&execution_queue);
    enqueue(&memory_queue, instruction);

    int * data = malloc(32 * sizeof(int));
    int_to_bin32(instruction->value, data);
    if (instruction->memW == 0 && instruction->memR == 0) return;

    if (instruction->memR) {
        // printf("entered memory read\n");
        int * mem_data = memory[instruction->value+1023];
        for (int i = 0; i < 32; ++i) {
            data[i] = mem_data[i];
        }
    }
    if (instruction->memW) {
        int mem_index = 1023 + instruction->value;
        int *dest = memory[mem_index];
        for (int j = 0; j < 32; j++) {
            dest[j] = registers[bin_to_int(instruction->r1, 5)][j];
        }
    }
}

void write_back() {
    if(isEmpty(&memory_queue))
    {
        printf("No instruction to write back\n");
        return;
    }
    Instruction* instruction = dequeue(&memory_queue);
    enqueue(&writeBack_queue, instruction);

    //int* data = malloc(32 * sizeof(int));
    //int_to_bin32(bin_to_int(instruction->immediate,18), data);
    if (instruction->regW) {
        int reg_index = bin_to_int(instruction->r1, 5);
        if(reg_index==0)
        {
            printf("Attempted to write to R0");
            return;
        }
        if(bin_to_int(instruction->immediate,18)!=0){
            int_to_bin32(signed_immediate_to_int(instruction->immediate,18), registers[reg_index]);
        }
        else{
            int_to_bin32(instruction->value, registers[reg_index]);
        }
    }
}

int signed_immediate_to_int(int* bits, int len) {
    int value = 0;
    for (int i = 0; i < len; i++) {
        value = (value << 1) | bits[i];
    }
    if (bits[0] == 1) { 
        int sign_mask = ~((1 << len) - 1);
        value |= sign_mask;
    }
   return value;
}
