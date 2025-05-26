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
    int i = 0;
    int j = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline if exists
        line[strcspn(line, "\n")] = 0;
        // r == 0 means success else r==1 means empty line or comment
        int r = parse_instruction(line, binary);

        if(r==0&&i<1024)
        {
            total_instructions++;
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

void print_pipeline_stage(const char* stage, Instruction* instr) {
    if (instr == NULL) {
        printf("%s Stage: Empty\n", stage);
        return;
    }
    printf("%s Stage: Instruction at line %d\n", stage, instr->line);
    
    // Print input parameters based on stage
    if (strcmp(stage, "Fetch") == 0) {
        printf("PC: %d\n", bin_to_int(registers[32], 32));
    }
    else if (strcmp(stage, "Decode") == 0) {
        printf("Instruction bits: ");
        for (int i = 0; i < 32; i++) {
            printf("%d", instr->instruction[i]);
        }
        printf("\n");
    }
    else if (strcmp(stage, "Execute") == 0) {
        printf("R1: %d, R2: %d, R3: %d\n", 
            bin_to_int(instr->r1, 5),
            bin_to_int(instr->r2, 5),
            bin_to_int(instr->r3, 5));
    }
    else if (strcmp(stage, "Memory") == 0) {
        if (instr->memW) 
            printf("Memory Write at address: %d\n", instr->value + 1023);
        if (instr->memR)
            printf("Memory Read from address: %d\n", instr->value + 1023);
    }
    else if (strcmp(stage, "WriteBack") == 0) {
        if (instr->regW)
            printf("Writing to R%d\n", bin_to_int(instr->r1, 5));
    }
}

void print_register_updates(int reg_num, int old_value, int new_value) {
    if (old_value != new_value) {
        printf("Register Update - R%d: %d -> %d\n", 
               reg_num, old_value, new_value);
    }
}

void print_memory_updates(int address, int old_value, int new_value) {
    if (old_value != new_value) {
        printf("Memory Update - Address %d: %d -> %d\n", 
               address, old_value, new_value);
    }
}

int main()
{

    initQueue(&fetch_queue);
    initQueue(&decode_queue);
    initQueue(&execution_queue);
    initQueue(&memory_queue);
    initQueue(&writeBack_queue);
    init_logger_memory("data/memory_log.txt");
    init_logger_registers("data/registers_log.txt");
    readFile("data/test.txt");


    initialize_instructions(instructions, total_instructions);

    int cycle = 1;
    int last_fetch_cycle = -2;
    int memory_busy = 0;

    printf("Total Instructions: %d\n", total_instructions);

    while (completed<total_instructions) {
        printf("\n=== Clock Cycle %d ===\n", cycle);
        memory_busy = 0;

        // Store old values for comparison
        int old_reg_values[33];
        int old_mem_values[2048];
        for(int i = 0; i < 33; i++) {
            old_reg_values[i] = bin_to_int(registers[i], 32);
        }
        for(int i = 0; i < 2048; i++) {
            old_mem_values[i] = bin_to_int(memory[i], 32);
        }

        for (int i = 0; i < total_instructions; i++) {
            if (instructions[i].completed) 
            {
                continue;
            }
            
            // Write Back
            if (instructions[i].memory != -1 && instructions[i].write_back == -1 && cycle > instructions[i].memory) {
                instructions[i].write_back = cycle;
                write_back();
                print_pipeline_stage("WriteBack", &instructions[i]);
                if(instructions[i].jump_backwards==0)
                {
                    instructions[i].completed = 1;
                    completed++;
                }
                else{
                    instructions[i].jump_backwards = 0;
                    initialize_instruction(&instructions[i]);
                }
                continue;
            }

            // Memory (only one instruction per cycle)
            if (instructions[i].execute_end != -1 && instructions[i].memory == -1 && cycle > instructions[i].execute_end && !memory_busy) {
                instructions[i].memory = cycle;
                memory_busy = 1;
                memory_access();
                print_pipeline_stage("Memory", &instructions[i]);
                continue;
            }
            // Execute (2 cycles)
            if (instructions[i].decode_end != -1 && instructions[i].execute_start == -1 && cycle > instructions[i].decode_end) {
                instructions[i].execute_start = cycle;
                instructions[i].execute_end = cycle + 1;
                res_exec = execute();
                instructions[i].value = res_exec;
                print_pipeline_stage("Execute", &instructions[i]);
                continue;
            }

            // Decode (2 cycles)
            if (instructions[i].fetch != -1 && instructions[i].decode_start == -1 && cycle > instructions[i].fetch) {
                instructions[i].decode_start = cycle;
                instructions[i].decode_end = cycle + 1;
                decode();
                print_pipeline_stage("Decode", &instructions[i]);
                continue;
            }

            // Fetch (only if last fetch wasn’t the previous cycle and no MEM this cycle)
            if (instructions[i].fetch == -1 && cycle - last_fetch_cycle > 1 && !memory_busy) {
                initialize_instruction(&instructions[i]);
                instructions[i].fetch = cycle;
                last_fetch_cycle = cycle;
                fetch(registers[32], &(instructions[i]));
                print_pipeline_stage("Fetch", &instructions[i]);
                continue;
            }
        }
            // Print register and memory updates
        for(int i = 0; i < 33; i++) {
            int new_value = bin_to_int(registers[i], 32);
            print_register_updates(i, old_reg_values[i], new_value);
        }
        
        for(int i = 0; i < 2048; i++) {
            int new_value = bin_to_int(memory[i], 32);
            print_memory_updates(i, old_mem_values[i], new_value);
        }
        cycle++;

    }
    for(int i = 0; i < 2048; i++) {
        int_array_to_binary_string(memory[i],-1,i+1);
    }
    for(int i = 0; i<33 ;i++)
    {
        int_array_to_binary_string(registers[i],0,i);
    }
    
    close_logger();
    return 0;
}
