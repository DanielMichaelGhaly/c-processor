
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
    }
}

// int main() {
   
//     return 0;
// }
