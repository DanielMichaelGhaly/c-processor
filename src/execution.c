#include "execution.h"

int bits_to_int(const int *bits, int len) {
    int val = 0;
    for (int i = 0; i < len; ++i) {
        val = (val << 1) | bits[i];
    }
    return val;
}

int execute(Instruction* instruction) {

    if(isEmpty(&decode_queue))
    {
        printf("No instruction to execute\n");
        return 0;
    }
    Instruction* instruction_dequeued = dequeue(&decode_queue);
    printf("Executing instruction at line: %d\n", instruction_dequeued->line);
    int* instr = instruction->instruction;


    enqueue(&execution_queue, instruction_dequeued);

    if(bin_to_int(instr, 32)==0){
        return 0;
    }

    int execution_result = 0;

    // Convert bit arrays to ints
    int r1_val = bits_to_int(registers[bits_to_int(instruction->r1,5)], 32);
    int r2_val = bits_to_int(registers[bits_to_int(instruction->r2,5)], 32);
    int r3_val = bits_to_int(registers[bits_to_int(instruction->r3,5)], 32);

    for(int j = 0; j < 5; j++) {
        if(instruction->ALUsig[j] == 1) {
            char *instruction1 = "";
            switch(j) {
                case 0: instruction1 = "ADD"; break;
                case 1: instruction1 = "SUB"; break;
                case 2: instruction1 = "MUL"; break;
                case 3: instruction1 = "AND"; break;
                case 4: instruction1 = "XORI"; break;
            }
            execution_result = alu(instruction1, r2_val, r3_val);
        }
    }
    if(instruction->branch > 0) {
        if(instruction->branch ==1)
        {
            printf("jump to \n");
            printf("address: %d \n", bin_to_int(instruction->address,28));
            int address_jumped_to = bin_to_int(instruction->address,28);
            if(address_jumped_to<bin_to_int(registers[32],32)-1)
            {
                printf("entered if conidtion\n");
                instruction->jump_backwards = 1;
                instructions[instruction->line-1].jump_backwards = 1;
                instructions[instruction->line-2].jump_backwards = 1;
                // instructions[instruction->line-2].jump_backwards = 1;
                for(int i = address_jumped_to; i<instruction->line-2; i++)
                {
                    initialize_instruction(&instructions[i]);
                    completed--;
                }
            }
            int_to_bin32(bin_to_int(registers[32],32)-1, registers[32]);
            for(int i = 4; i<32; i++)
            {
                registers[32][i] = instruction->address[i-4];
            }
            flush_Queues(instr);

        }
        else{
            if(r1_val==r2_val){
                printf("they are equal\n");
                completed = completed + bin_to_int(instruction->immediate,18)-1;
                int temp_pc_val = bits_to_int(registers[32], 32);
                int_to_bin32(temp_pc_val+bin_to_int(instruction->immediate,18)-1, registers[32]);
                flush_Queues(instr);
            }
        }
    }

    if(instruction->memR == 1) {
        execution_result = r2_val + bin_to_int(instruction->immediate, 18);
    }
    if(instruction->memW == 1) {
        execution_result = r2_val + bin_to_int(instruction->immediate, 18);
    }

    for(int j = 0; j < 2; j++) {
        if(instruction->shift[j] == 1) {
            char *instruction2 = "";
            switch(j) {
                case 0: instruction2 = "LSL"; break;
                case 1: instruction2 = "LSR"; break;
            }
            execution_result = shifting(instruction2, r1_val, r2_val, bin_to_int(instruction->shamt,13));
        }
    }
    return execution_result;
}



int alu(char* instruction, int R2, int R3)
{
    if (strcmp(instruction, "ADD") == 0) {
        return R2 + R3;
    } else if (strcmp(instruction, "SUB") == 0) {
        return R2 - R3;
    } else if (strcmp(instruction, "MUL") == 0) {
        return R2 * R3;
    } else if (strcmp(instruction, "AND") == 0) {
        return R2 & R3;
    } else if (strcmp(instruction, "XORI") == 0) {
        return R2 ^ R3;
    }
    return 0;
}

int shifting(char* instruction, int R1, int R2, int R3)
{
    if(strcmp(instruction, "LSL")==0){
        R1 = R2 << R3;
    }
    else if(strcmp(instruction, "LSR")==0){
        R1 = R2 >> R3;
    }
    return R1;
}

void flush_Queues() {
    while(!isEmpty(&fetch_queue)) {
        Instruction* flushed_Instr = dequeue(&fetch_queue);
        printf("line in original array before is not zero : %d \n", instructions[14].line);
        printf("Flushing instruction at line: %d\n", flushed_Instr->line);
        initialize_instruction(flushed_Instr);
        print_Instruction(flushed_Instr);
        printf("line in original array is now zero : %d \n", instructions[14].line);
        printf("instruction is now: \n");
        print_Instruction(&instructions[15]);
    }
    while(!isEmpty(&decode_queue)) {
        Instruction* flushed_Instr_Dequeue = dequeue(&decode_queue);
        printf("Flushing instruction at line: %d\n", flushed_Instr_Dequeue->line);
        initialize_instruction(flushed_Instr_Dequeue);
    }  
}
