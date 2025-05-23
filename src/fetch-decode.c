#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "fetch-decode.h"
#include "main.h"

int r1[5];
int r2[5];
int r3[5];
int shamt[13] ;
int immediate[18];
int address[28];

int ALUsig[5];
int shift[2];
int memR = 0;
int memW = 0;
int regW = 0;
int branch = 0;

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

void fetch(int* pc) {
    int *IR = memory[bin_to_int(pc, 32)];
    pc_incr(pc);
    initQueue(&fetch_queue);
    enqueue(&fetch_queue, *IR);
}

void decode() {
    int* instr = dequeue(&fetch_queue);
    initQueue(&decode_queue);
    enqueue(&decode_queue, *instr);

    int opcode = (instr[0] << 3) | (instr[1] << 2) | (instr[2] << 1) | instr[3];
    initialize_with_zeros(shift, 2);
    initialize_with_zeros(ALUsig, 5);
    initialize_with_zeros(shamt, 13);
    initialize_with_zeros(immediate, 18);
    initialize_with_zeros(address, 28);
    memR = 0;
    memW = 0;
    regW = 0;
    branch = 0;

    switch (opcode) {
        case 0x0: case 0x1: case 0x2: case 0x5: case 0x8: case 0x9: {
            int local_r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int local_r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int local_r3 = (instr[14] << 4) | (instr[15] << 3) | (instr[16] << 2) | (instr[17] << 1) | instr[18];
            int local_shamt = 0;
            for (int i = 0; i < 13; i++) {
                local_shamt |= (instr[19 + i] << (12 - i));
            }

            switch (opcode) {
                case 0x0: ALUsig[0] = 1; break;
                case 0x1: ALUsig[1] = 1; break;
                case 0x2: ALUsig[2] = 1; break;
                case 0x5: ALUsig[3] = 1; break;
                case 0x8: shift[0] = 1; break;
                case 0x9: shift[1] = 1; break;
            }
            regW = 1;
            break;
        }
        case 0x3: case 0x4: case 0x6: case 0xA: case 0xB: {
            int local_r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int local_r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int local_immediate = 0;
            for (int i = 0; i < 18; i++) {
                local_immediate |= (instr[14 + i] << (17 - i));
            }

            switch (opcode) {
                case 0x3: regW = 1; break;
                case 0x4: branch = 1; break;
                case 0x6: ALUsig[4] = 1; regW = 1; break;
                case 0xA: regW = 1; memR = 1; break;
                case 0xB: memW = 1; break;
            }
            break;
        }
        case 0x7: {
            int local_address = 0;
            for (int i = 0; i < 28; i++) {
                local_address |= (instr[4 + i] << (27 - i));
            }
            branch = 1;
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

void memory_access(int * data, int * address, int memW, int memR) {
    if (memW == 0 && memR == 0) return;

    if (memR) {
        int * mem_data = memory[bin_to_int(address, 32)];
        for (int i = 0; i < 32; ++i) {
            data[i] = mem_data[i];
        }
    }
    if (memW) {
        int mem_index = bin_to_int(address, 32);
        int *src = data;
        int *dest = memory[mem_index];
        for (int i = 0; i < 32; ++i) {
            dest[i] = src[i];
        }
    }
}

void write_back(int * data, int * reg_num, int regW) {
    if (regW) {
        int reg_index = bin_to_int(reg_num, 5);
        int *reg_ptr = registers[reg_index];
        int *src = int_to_bin32(bin_to_int(data, 32), data);
        for (int i = 0; i < 32; ++i) {
            reg_ptr[i] = src[i];
        }
    }
}
