#include <stdio.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100

typedef struct {
    int fetch;
    int decode_start, decode_end;
    int execute_start, execute_end;
    int memory;
    int write_back;
    int completed;
} Instruction;

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

int main() {
    Instruction instructions[MAX_INSTRUCTIONS];
    int num_instructions;
    printf("Enter number of instructions: ");
    scanf("%d", &num_instructions);

    initialize_instructions(instructions, num_instructions);

    int cycle = 1;
    int completed = 0;
    int last_fetch_cycle = -2;
    int memory_busy = 0;

    while (completed < num_instructions) {
        memory_busy = 0;

        for (int i = 0; i < num_instructions; i++) {
            if (instructions[i].completed) continue;

            // Write Back
            if (instructions[i].memory != -1 && instructions[i].write_back == -1 && cycle > instructions[i].memory) {
                instructions[i].write_back = cycle;
                instructions[i].completed = 1;
                completed++;
                continue;
            }

            // Memory (only one instruction per cycle)
            if (instructions[i].execute_end != -1 && instructions[i].memory == -1 && cycle > instructions[i].execute_end && !memory_busy) {
                instructions[i].memory = cycle;
                memory_busy = 1;
                continue;
            }

            // Execute (2 cycles)
            if (instructions[i].decode_end != -1 && instructions[i].execute_start == -1 && cycle > instructions[i].decode_end) {
                instructions[i].execute_start = cycle;
                instructions[i].execute_end = cycle + 1;
                continue;
            }

            // Decode (2 cycles)
            if (instructions[i].fetch != -1 && instructions[i].decode_start == -1 && cycle > instructions[i].fetch) {
                instructions[i].decode_start = cycle;
                instructions[i].decode_end = cycle + 1;
                continue;
            }

            // Fetch (only if last fetch wasn’t the previous cycle and no MEM this cycle)
            if (instructions[i].fetch == -1 && cycle - last_fetch_cycle > 1 && !memory_busy) {
                instructions[i].fetch = cycle;
                last_fetch_cycle = cycle;
                break;
            }
        }

        cycle++;
    }

    printf("\nCycle-by-cycle simulation:\n");
    for (int i = 0; i < num_instructions; i++) {
        printf("Instruction %d: ", i + 1);
        printf("IF=%d, ID=[%d-%d], EX=[%d-%d], MEM=%d, WB=%d\n",
               instructions[i].fetch,
               instructions[i].decode_start, instructions[i].decode_end,
               instructions[i].execute_start, instructions[i].execute_end,
               instructions[i].memory,
               instructions[i].write_back);
    }

    return 0;
}
