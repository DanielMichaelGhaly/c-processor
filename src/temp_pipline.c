
#include "temp_pipline.h"

#define MAX_INSTRUCTIONS 100

void initialize_instructions(Instruction instructions[], int size) {
    for (int i = 0; i < size; i++) {
        instructions[i].fetch = -1;
        instructions[i].decode_start = instructions[i].decode_end = -1;
        instructions[i].execute_start = instructions[i].execute_end = -1;
        instructions[i].memory = -1;
        instructions[i].write_back = -1;
        instructions[i].completed = 0;
        instructions[i].memR = 0;
        instructions[i].memW = 0;
        instructions[i].regW = 0;
        instructions[i].branch = 0;
        for (int j = 0; j < 5; j++) {
            instructions[i].r1[j] = 0;
            instructions[i].r2[j] = 0;
            instructions[i].r3[j] = 0;
            instructions[i].ALUsig[j] = 0;
        }
        for (int j = 0; j < 13; j++) {
            instructions[i].shamt[j] = 0;
        }
        for (int j = 0; j < 18; j++) {
            instructions[i].immediate[j] = 0;
        }
        for (int j = 0; j < 28; j++) {
            instructions[i].address[j] = 0;
        }
        for (int j = 0; j < 2; j++) {
            instructions[i].shift[j] = 0;
        }
        
    }
}

// int main() {
   
//     return 0;
// }
