#include "main.h"
/*
Registers and Memory initialization
*/

int memory[2048][32] = {0};
//registers[32] -> PC
int registers[33][32] = {0};

int res_exec = 0;
    
int total_instructions = 0;
int clock = 0;

void printArr(int* arr, int size)
{
    for(int i = 0; i < size; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

//to read txt file
void readFile(char * filePath)
{
    FILE* fp = NULL;
    errno_t res = fopen_s(&fp, filePath, "r");
    if (res != 0) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LEN];
    int binary[32];
    // int line_number = 0;
    int i = 0;
    int j = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline if exists
        line[strcspn(line, "\n")] = 0;
        parse_instruction(line, binary);
        total_instructions++;
        // printf("Instruction %d: ", line_number++);
        // print_binary(binary);
        if(i<1024)
        {
            for(; j<32; j++)
            {
                memory[i][j] = binary[j];
            }
            j = 0;
            i++;
        }
    }

    fclose(fp);
}

int main()
{
    registers[2][31] = 1;
    registers[2][30] = 1;
    registers[3][31] = 1;
    registers[3][29] = 1;

    initQueue(&fetch_queue);
    initQueue(&decode_queue);
    initQueue(&execution_queue);
    initQueue(&memory_queue);
    initQueue(&writeBack_queue);
    init_logger("data/log.txt");
    readFile("data/test.txt");

    Instruction instructions[MAX_INSTRUCTIONS];

    initialize_instructions(instructions, total_instructions);

    int cycle = 1;
    int completed = 0;
    int last_fetch_cycle = -2;
    int memory_busy = 0;

    while (completed < total_instructions) {
        memory_busy = 0;

        for (int i = 0; i < total_instructions; i++) {
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
                res_exec = execute();
                // printf("%d", res);
                // printf("\n");
                continue;
            }

            // Decode (2 cycles)
            if (instructions[i].fetch != -1 && instructions[i].decode_start == -1 && cycle > instructions[i].fetch) {
                instructions[i].decode_start = cycle;
                instructions[i].decode_end = cycle + 1;
                decode();
                // printf("cycle: %d", cycle);
                // printf("\n");
                // printArr(r1,5);
                // printArr(r2,5);
                // printArr(r3,5);
                // printArr(shamt,13);
                // printArr(immediate, 18);
                // printArr(address, 28);
                // printArr(ALUsig, 5);
                // printArr(shift, 2);
                // printf("%d, %d, %d, %d",memR,memW,regW,branch);
                // printf("\n");
                continue;
            }

            // Fetch (only if last fetch wasn’t the previous cycle and no MEM this cycle)
            if (instructions[i].fetch == -1 && cycle - last_fetch_cycle > 1 && !memory_busy) {
                instructions[i].fetch = cycle;
                last_fetch_cycle = cycle;
                fetch(registers[32]);
                // int* firstInstruction = peek(&fetch_queue);
                // printArr(firstInstruction);
                break;
            }
        }
        printf("PC: ");
        printArr(registers[32], 32);
        printf("Cycles : %d", cycle);
        printf("\n");

        cycle++;
    }

    printf("\nCycle-by-cycle simulation:\n");
    for (int i = 0; i < total_instructions; i++) {
        printf("Instruction %d: ", i + 1);
        printf("IF=%d, ID=[%d-%d], EX=[%d-%d], MEM=%d, WB=%d\n",
               instructions[i].fetch,
               instructions[i].decode_start, instructions[i].decode_end,
               instructions[i].execute_start, instructions[i].execute_end,
               instructions[i].memory,
               instructions[i].write_back);
    }

    for(int i = 0; i < 2048; i++) {
        int_array_to_binary_string(memory[i]);
    }
    
    close_logger();

}