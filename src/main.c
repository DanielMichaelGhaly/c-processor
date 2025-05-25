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
int completed = 0;

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
        // r == 0 means success else r==1 means empty line or comment
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

    initQueue(&fetch_queue);
    initQueue(&decode_queue);
    initQueue(&execution_queue);
    initQueue(&memory_queue);
    initQueue(&writeBack_queue);
    init_logger("data/log.txt");
    readFile("data/test.txt");


    initialize_instructions(instructions, total_instructions);

    int cycle = 1;
    int last_fetch_cycle = -2;
    int memory_busy = 0;

    printf("Total Instructions: %d\n", total_instructions);
    while (completed<total_instructions) {
        memory_busy = 0;
        printf("Completed till now %d\n",completed);
        for (int i = 0; i < total_instructions; i++) {
            if (instructions[i].completed) continue;
            
            // Write Back
            if (instructions[i].memory != -1 && instructions[i].write_back == -1 && cycle > instructions[i].memory) {
                instructions[i].write_back = cycle;
                write_back(&instructions[i],instructions[i].value);
                if(instructions[i].jump_backward==0)
                {
                    instructions[i].completed = 1;
                    completed++;
                }
                else{
                    instructions[i].jump_backward = 0;
                }
                printf("entered write back stage");

                // printf("Finished write back. ");
                continue;
            }

            // Memory (only one instruction per cycle)
            if (instructions[i].execute_end != -1 && instructions[i].memory == -1 && cycle > instructions[i].execute_end && !memory_busy) {
                instructions[i].memory = cycle;
                memory_busy = 1;
                memory_access(&instructions[i],instructions[i].value);
                printf("entered memory stage");
                // printf("Finished memory access. ");
                continue;
            }
            // Execute (2 cycles)
            if (instructions[i].decode_end != -1 && instructions[i].execute_start == -1 && cycle > instructions[i].decode_end) {
                instructions[i].execute_start = cycle;
                instructions[i].execute_end = cycle + 1;
                res_exec = execute(&instructions[i]);
                instructions[i].value = res_exec;
                // printf("Finished execution. \n");
                // printf("After executing instruction:\n");
                // print_Instruction(&(instructions[i]));
                // printf("Instruction %d : %d", i, instructions[i].value);
                // printf("result : %d", res_exec);
                // printf("\n");
                printf("entered execute stage");
                continue;
            }

            // Decode (2 cycles)
            if (instructions[i].fetch != -1 && instructions[i].decode_start == -1 && cycle > instructions[i].fetch) {
                instructions[i].decode_start = cycle;
                instructions[i].decode_end = cycle + 1;
                decode(&instructions[i]);
                // printf("Finished decode. ");
                // printf("After decoding instruction:\n");
                // print_Instruction(&(instructions[i]));
                // printf("r1: ");
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
                // printf("regsgdgs %d", instructions[i].regW);
                printf("entered decode stage");
                continue;
            }

            // Fetch (only if last fetch wasn’t the previous cycle and no MEM this cycle)
            if (instructions[i].fetch == -1 && cycle - last_fetch_cycle > 1 && !memory_busy) {
                initialize_instruction(&instructions[i]);
                instructions[i].fetch = cycle;
                last_fetch_cycle = cycle;
                fetch(registers[32], &(instructions[i]));
                instructions[i].line = bin_to_int(registers[32],32);
                printf("line: %d", instructions[i].line);
                printf("entered fetch stage");
                // printf("Outside fetch function\n");
                // print_Instruction(&(instructions[i]));
                // printf("Finished fetch. ");
                // printf("After fetching instruction:\n");
                // print_Instruction(&(instructions[i]));
                // int* firstInstruction = peek(&fetch_queue);
                // printArr(firstInstruction);
                continue;
            }
        }
        printf("PC: ");
        printArr(registers[32], 32);
        printf("Cycles : %d", cycle);
        printf("\n");
        printf("Instruction: ");
        printArr((int *) memory[bin_to_int(registers[32], 32)],32);
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
        for(int i = 0; i<33 ;i++)
        {
            printf("%d: \n", bin_to_int(registers[i], 32));
        }
        // printf("R1: ");
        // printArr(registers[1], 32);
        // printf("R2: ");
        // printArr(registers[2], 32);

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
    // for(int i = 0; i<2048; i++)
    // {
    //     int current_pc = bin_to_int(registers[32],32);
    //     int current_inst = (int *) memory[bin_to_int(registers[32], 32)];
    //     Instruction* instruction;
    //     initialize_instruction(&instruction);
    //     if(isEmpty(&fetch_queue)==1&&isEmpty(&decode_queue)==1&&isEmpty(&execution_queue)==1&&isEmpty(&memory_queue)==1&&isEmpty(&writeBack_queue)==1&&current_inst==0)
    //     {
    //         break;
    //     }
    //     cycle++;
    //     printf("Cycle Num: %d\n", cycle);
    //     if(cycle % 2 != 0)
    //     {
    //         if(current_inst!=0)
    //         {
    //             fetch(&registers[32],instruction);
    //         }
    //         if(isEmpty(&memory_queue)==0)
    //         {
    //             Instruction* pp = dequeue(&memory_queue);
    //             write_back(pp,pp->value);
    //         }
    //     }
    //     else{
    //         enqueue(&decode_queue)
    //     }
    for(int i = 0; i < 2048; i++) {
        int_array_to_binary_string(memory[i],-1);
    }

    log_print("Registers:");
    for(int i = 0; i<33 ;i++)
    {
        int_array_to_binary_string(registers[i],i);
        printf("%d: \n", bin_to_int(registers[i], 32));
    }
    
    close_logger();
    return 0;
}
