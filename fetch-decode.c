#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "fetch-decode.h"

int r1[5] = {0,0,0,0,0};
int r2[5] = {0,0,0,0,0};
int r3[5] = {0,0,0,0,0};
int shamt[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
int immediate[18] = {0};
int address[28] = {0};

int ALUsig[5] = {0,0,0,0,0};
int shift[2] = {0,0};
int memR = 0;   
int memW = 0;
int regW = 0;
int branch = 0;
//queues for the pipeline they are latter initialized in the individual functions
Queue *fetch_queue;
Queue *decode_queue;

//ControlSignals ctrl;
//DecodedFields df;

int * pc_incr(int* pc){
    return converting_dnigga(converting_bnigga((pc,32)+1));
}

int * converting_dnigga(int value, int *arr) {
    for (int i = 0; i < 32; ++i) {
        arr[i] = (value >> (31 - i)) & 1;
    }
    return arr;
}


int converting_bnigga(int *arr,int n) {
    int value = 0;
    for (int i = 0; i < n; ++i) {
        value |= (arr[i] & 1) << (n-1 - i);
    }
    return value;
}

void fetch(int* pc){
    int *IR = memory[(converting_bnigga(pc,32))]; // read instruction from memory and place in the IR
    pc_incr(pc); 
    // append instruction to fetch queue
    initQueue(fetch_queue);
    enqueue(fetch_queue, instr);
    // dequeue fetch queue in the deoced function 
    // clock cycle count ting
}


void decode(){
    // append instruction to decode queue
    int* instr = dequeue(fetch_queue);
    initQueue(decode_queue);
    enqueue(decode_queue, instr);
    // clock cycle count ting
    int opcode = (instr[0] << 3) | (instr[1] << 2) | (instr[2] << 1) | instr[3];
    int ALUsig[5] = {0,0,0,0,0};
    int shift[2] = {0,0};
    int memR = 0;   
    int memW = 0;
    int regW = 0;
    int branch = 0;
    switch (opcode) {
        case 0x0: // R-instr
        case 0x1:
        case 0x2:
        case 0x5:
        case 0x8:
        case 0x9: // R-instr
            int r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int r3 = (instr[14] << 4) | (instr[15] << 3) | (instr[16] << 2) | (instr[17] << 1) | instr[18]; 
            int shamt = 0;
            for (int i = 0; i < 13; i++) {
                shamt |= (instr[19 + i] << (12 - i));
            }


            switch (opcode) {
                case 0x0: ALUsig[0] = 1; break; // ADD
                case 0x1: ALUsig[1] = 1; break; // SUB
                case 0x2: ALUsig[2] = 1; break; // MUL
                case 0x5: ALUsig[3] = 1; break; // AND
                case 0x8: shift[0] = 1; break; // shift left
                case 0x9: shift[1] = 1; break; // shift right
                default: /* */ break;
            }
            regW = 1;
            
            break;
        case 0x3:
        case 0x4:
        case 0x6:
        case 0xA:
        case 0xB: // I-instr
            int r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int immediate = 0;
            for (int i = 0; i < 18; i++) {
                immediate |= (instr[14 + i] << (17 - i));
            }

            switch (opcode) {
                case 0x3: regW = 1; break; // MOVI
                case 0x4: branch = 1; break; // JEQ
                case 0x6: ALUsig[4] = 1; regW = 1; break; // XORI
                case 0xA: regW = 1; memR = 1; break; // MOV to reg
                case 0xB: memW = 1; break; // Write to mem
                default: /* */ break;
            }

            break;
        case 0x7: // J-instr
            int address = 0;
            for (int i = 0; i < 28; i++) {
                address |= (instr[4 + i] << (27 - i));
            }

            branch = 1;
            break;
        default:
            // Handle invalid opcode
            break;
    }
    // decode instr
    // int* instr = dequeue(fetch_queue); 
    //should be added to the excute queue
}


// void reset_control_signals(void) {
//     for(int i = 0; i < 5; ++i) ctrl.ALUsig[i] = 0;
//     for(int i = 0; i < 2; ++i) ctrl.shift[i]  = 0;
//     ctrl.memR   = 0;
//     ctrl.memW   = 0;
//     ctrl.regW   = 0;
//     ctrl.branch = 0;
// }

// void reset_decoded_fields(void) {
//     df.r1 = df.r2 = df.r3 = 0;
//     df.shamt = 0;
//     df.immediate = 0;
//     df.address = 0;
// }

// call to get the value of the register
int access_register_file(int * reg_num) {
    int * data = register_file[converting_bnigga(reg_num,5)];
    int reg = converting_bnigga(data,32); 
    return reg;
}

void Mem_nigga(int * data, int * address, int memW, int memR) {
    if (memW == 0 && memR == 0) {
        return;
    }
    if (memR) {
        int * mem_data = memory[converting_bnigga(address,32)];
        for (int i = 0; i < 32; ++i) {
            data[i] = mem_data[i];
        }
    }
    if (memW) {
        memory[converting_bnigga(address,32)] = converting_dnigga(data,32);
    }
}

void write_back(int * data, int * reg_num, int regW) {
    if (regW) {
        register_file[converting_bnigga(reg_num,5)] = converting_dnigga(data,32);
    }
}
