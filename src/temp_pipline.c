#include "temp_pipline.h"

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
        instructions[i].value = 0;
        instructions[i].line = 0;
        instructions[i].jump_backwards = 0;
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
        instructions[i].instruction = (int*)malloc(32 * sizeof(int));
        
    }
}

void initialize_instruction(Instruction* instruction) {
    instruction->fetch = -1;
    instruction->decode_start = instruction->decode_end = -1;
    instruction->execute_start = instruction->execute_end = -1;
    instruction->memory = -1;
    instruction->write_back = -1;
    instruction->completed = 0;
    instruction->memR = 0;
    instruction->memW = 0;
    instruction->regW = 0;
    instruction->branch = 0;
    instruction->value = 0;
    instruction->line = 0;
    instruction->jump_backwards = 0;

    for (int j = 0; j < 5; j++) {
        instruction->r1[j] = 0;
        instruction->r2[j] = 0;
        instruction->r3[j] = 0;
        instruction->ALUsig[j] = 0;
    }
    for (int j = 0; j < 13; j++) {
        instruction->shamt[j] = 0;
    }
    for (int j = 0; j < 18; j++) {
        instruction->immediate[j] = 0;
    }
    for (int j = 0; j < 28; j++) {
        instruction->address[j] = 0;
    }
    for (int j = 0; j < 2; j++) {
        instruction->shift[j] = 0;
    }
    instruction->instruction = (int*)malloc(32 * sizeof(int));
}

void print_Instruction(Instruction* instruction) {
    printf("Fetch: %d\n", instruction->fetch);
    printf("Decode Start: %d, Decode End: %d\n", instruction->decode_start, instruction->decode_end);
    printf("Execute Start: %d, Execute End: %d\n", instruction->execute_start, instruction->execute_end);
    printf("Memory: %d\n", instruction->memory);
    printf("Write Back: %d\n", instruction->write_back);
    printf("Completed: %d\n", instruction->completed);

    printf("Line: %d\n", instruction->line);

    printf("Jump Backwards: %d\n", instruction->jump_backwards);

    printf("R1: ");
    for (int i = 0; i < 5; i++) printf("%d ", instruction->r1[i]);
    printf("\n");

    printf("R2: ");
    for (int i = 0; i < 5; i++) printf("%d ", instruction->r2[i]);
    printf("\n");

    printf("R3: ");
    for (int i = 0; i < 5; i++) printf("%d ", instruction->r3[i]);
    printf("\n");

    printf("SHAMT: ");
    for (int i = 0; i < 13; i++) printf("%d ", instruction->shamt[i]);
    printf("\n");
    printf("Immediate: ");
    for (int i = 0; i < 18; i++) printf("%d ", instruction->immediate[i]);
    printf("\n");
    printf("Address: ");
    for (int i = 0; i < 28; i++) printf("%d ", instruction->address[i]);
    printf("\n");
    printf("ALUsig: ");
    for (int i = 0; i < 5; i++) printf("%d ", instruction->ALUsig[i]);
    printf("\n");
    printf("Shift: ");
    for (int i = 0; i < 2; i++) printf("%d ", instruction->shift[i]);
    printf("\n");
    printf("MemR: %d, MemW: %d, RegW: %d, Branch: %d\n", instruction->memR, instruction->memW, instruction->regW, instruction->branch);
    printf("Value: %d\n", instruction->value);
    printf("Instruction: ");
    if (instruction->instruction != NULL) {
        for (int i = 0; i < 32; i++) {
            printf("%d", instruction->instruction[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");
    printf("--------------------------------------------------\n");
}
// int main() {
   
//     return 0;
// }
