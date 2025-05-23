#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "parser.h"

// Define opcodes
const char* instruction_names[12] = {
    "ADD", "SUB", "MUL", "MOVI", "JEQ", "AND", "XORI",
    "JMP", "LSL", "LSR", "MOVR", "MOVM"
};

// Helper function to get opcode from mnemonic
int get_opcode(const char* mnemonic) {
    for (int i = 0; i < 12; i++) {
        if (strcmp(mnemonic, instruction_names[i]) == 0) return i;
    }
    return -1;
}

// Convert string register name to integer
int parse_register(const char* reg) {
    if (reg[0] == 'R' && isdigit(reg[1])) return atoi(reg + 1);
    return 0; // R0
}

// Write integer as binary into array
void write_binary(int* dest, int value, int start, int bits) {
    for (int i = 0; i < bits; i++) {
        dest[start + bits - 1 - i] = (value >> i) & 1;
    }
}

// Parse a single instruction line into binary representation
void parse_instruction(const char* line, int* binary) {
    char mnemonic[10];
    char op1[10], op2[10], op3[10];
    int opcode, reg1, reg2, reg3, imm, addr;

    sscanf(line, "%s %s %s %s", mnemonic, op1, op2, op3);
    opcode = get_opcode(mnemonic);
    if (opcode == -1) return;

    memset(binary, 0, 32 * sizeof(int));
    write_binary(binary, opcode, 0, 4);

    if (opcode <= 2 || opcode == 5 || opcode == 8 || opcode == 9) { // R-type: ADD, SUB, MUL, AND, LSL, LSR
        reg1 = parse_register(op1);
        reg2 = parse_register(op2);
        reg3 = (opcode == 8 || opcode == 9) ? 0 : parse_register(op3); // SHAMT = op3 if LSL/LSR
        int shamt = (opcode == 8 || opcode == 9) ? atoi(op3) : 0;

        write_binary(binary, reg1, 4, 5);
        write_binary(binary, reg2, 9, 5);
        write_binary(binary, reg3, 14, 5);
        write_binary(binary, shamt, 19, 13);

    } else if ((opcode >= 3 && opcode <= 6) || opcode >= 10) { // I-type: MOVI, JEQ, XORI, MOVR, MOVM
        reg1 = parse_register(op1);
        reg2 = (opcode == 3) ? 0 : parse_register(op2);
        imm = atoi(op3);

        write_binary(binary, reg1, 4, 5);
        write_binary(binary, reg2, 9, 5);
        write_binary(binary, imm, 14, 18);

    } else if (opcode == 7) { // J-type: JMP
        addr = atoi(op1);
        write_binary(binary, addr, 4, 28);
    }
}

void print_binary(int* binary) {
    for (int i = 0; i < 32; i++) printf("%d", binary[i]);
    printf("\n");
}

// int main() {

//     return 0;
// }






















// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include "parser.h"

// int getInstructionType(int *instruction) {
//     // Extract 4-bit opcode (bits 0 to 3)
//     int opcode = (instruction[0] << 3) | (instruction[1] << 2) | (instruction[2] << 1) | instruction[3];

//     switch (opcode) {
//         case 0:  // ADD
//         case 1:  // SUB
//         case 2:  // MUL
//         case 5:  // AND
//         case 8:  // LSL
//         case 9:  // LSR
//             return 0;//R

//         case 3:  // MOVI
//         case 4:  // JEQ
//         case 6:  // XORI
//         case 10: // MOVR
//         case 11: // MOVM
//             return 1; //I

//         case 7:  // JMP
//             return 2;//J

//         default:
//             return -1;
//     }
// }

// int* processLine(char line[]) {
//     static int arr[32] = {0};  // Persistent storage (not lost after return)
//     char *token = strtok(line, " ");  // Get the first token

//     if (token != NULL) {
//         printf("First token: %s\n", token);  // Example: "ADD"

//         if (strcmp(token, "ADD") == 0) {
//             arr[0] = 0; arr[1] = 0; arr[2] = 0; arr[3] = 0;
//             printf("Instruction: Add\n");

//         } else if (strcmp(token, "SUB") == 0) {
//             arr[0] = 0; arr[1] = 0; arr[2] = 0; arr[3] = 1;
//             printf("Instruction: Subtract\n");

//         } else if (strcmp(token, "MUL") == 0) {
//             arr[0] = 0; arr[1] = 0; arr[2] = 1; arr[3] = 0;
//             printf("Instruction: Multiply\n");

//         } else if (strcmp(token, "MOVI") == 0) {
//             arr[0] = 0; arr[1] = 0; arr[2] = 1; arr[3] = 1;
//             printf("Instruction: Move Immediate\n");

//         } else if (strcmp(token, "JEQ") == 0) {
//             arr[0] = 0; arr[1] = 1; arr[2] = 0; arr[3] = 0;
//             printf("Instruction: Jump if Equal\n");

//         } else if (strcmp(token, "AND") == 0) {
//             arr[0] = 0; arr[1] = 1; arr[2] = 0; arr[3] = 1;
//             printf("Instruction: And\n");

//         } else if (strcmp(token, "XORI") == 0) {
//             arr[0] = 0; arr[1] = 1; arr[2] = 1; arr[3] = 0;
//             printf("Instruction: Exclusive Or Immediate\n");

//         } else if (strcmp(token, "JMP") == 0) {
//             arr[0] = 0; arr[1] = 1; arr[2] = 1; arr[3] = 1;
//             printf("Instruction: Jump\n");

//         } else if (strcmp(token, "LSL") == 0) {
//             arr[0] = 1; arr[1] = 0; arr[2] = 0; arr[3] = 0;
//             printf("Instruction: Logical Shift Left\n");

//         } else if (strcmp(token, "LSR") == 0) {
//             arr[0] = 1; arr[1] = 0; arr[2] = 0; arr[3] = 1;
//             printf("Instruction: Logical Shift Right\n");

//         } else if (strcmp(token, "MOVR") == 0) {
//             arr[0] = 1; arr[1] = 0; arr[2] = 1; arr[3] = 0;
//             printf("Instruction: Move to Register\n");

//         } else if (strcmp(token, "MOVM") == 0) {
//             arr[0] = 1; arr[1] = 0; arr[2] = 1; arr[3] = 1;
//             printf("Instruction: Move to Memory\n");

//         } else {
//             printf("Unknown instruction: %s\n", token);
//         }
//     }

//     int type = getInstructionType(arr);
//     printf("Instruction type is: %d\n", type);
//     if (type == 0 ) {
//         char *op1 = strtok(NULL, " ");  // e.g., R1// R-type  R1,R2
//         char *op2 = strtok(NULL, " ");  // e.g., R2

//     if (strcmp(op1, "R0") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R1") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R2") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R3") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R4") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R5") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R6") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R7") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R8") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R9") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R10") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R11") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R12") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R13") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R14") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R15") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R16") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R17") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R18") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R19") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R20") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R21") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R22") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R23") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R24") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R25") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R26") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R27") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R28") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R29") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R30") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R31") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else {
//         printf("Invalid register3: %s\n", op1);
//     }
//         if (strcmp(op2, "R0") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R1") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R2") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R3") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R4") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R5") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R6") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R7") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R8") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R9") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R10") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R11") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R12") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R13") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R14") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R15") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R16") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R17") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R18") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R19") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R20") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R21") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R22") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R23") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R24") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R25") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R26") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R27") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R28") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R29") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R30") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R31") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else {
//         printf("Invalid register1: %s\n", op2);
//     }

// }
//     if (type==0) {
//         char* op3 = strtok(NULL, " ");  // e.g., R3
//         printf("The character is: %c\n", op3[0]);
        
       
//         if (op3[0] == 'R') {
            
//             if (strcmp(op3, "R0") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R1") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R2") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R3") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R4") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R5") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R6") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R7") == 0) {
//         arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R8") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R9") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R10") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R11") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R12") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R13") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R14") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R15") == 0) {
//         arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R16") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R17") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R18") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R19") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R20") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R21") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R22") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R23") == 0) {
//         arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R24") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R25") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R26") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R27") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=1;
//     } else if (strcmp(op3, "R28") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=0;
//     } else if (strcmp(op3, "R29") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=1;
//     } else if (strcmp(op3, "R30") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=0;
//     } else if (strcmp(op3, "R31") == 0) {
//         arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=1;
//     } else {
//         printf("Invalid register2: %s\n", op3);
//     }

//         }else {
//            int value = atoi(op3);
//             printf("%d\n", value);
//             int bits = 13;        // SHAMT is 13 bits wide
//             int start = 19;       // MSB goes here

//             unsigned int uval = value & ((1 << bits) - 1);  // ensure we only keep 13 bits

//             for (int i = 0; i < bits; i++) {
//                 arr[start + i] = (uval >> (bits - 1 - i)) & 1;
//             }


//        }


//     }
//     if (type==1) {
//         char *op1 = strtok(NULL, " ");  // e.g., R1
//         //I
//         if (strcmp(op1, "R0") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R1") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R2") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R3") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R4") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R5") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R6") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R7") == 0) {
//         arr[4]=0; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R8") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R9") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R10") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R11") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R12") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R13") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R14") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R15") == 0) {
//         arr[4]=0; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R16") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R17") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R18") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R19") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R20") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R21") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R22") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R23") == 0) {
//         arr[4]=1; arr[5]=0; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R24") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R25") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R26") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R27") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=0; arr[7]=1; arr[8]=1;
//     } else if (strcmp(op1, "R28") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=0;
//     } else if (strcmp(op1, "R29") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=0; arr[8]=1;
//     } else if (strcmp(op1, "R30") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=0;
//     } else if (strcmp(op1, "R31") == 0) {
//         arr[4]=1; arr[5]=1; arr[6]=1; arr[7]=1; arr[8]=1;
//     } else {
//         printf("Invalid register4: %s\n", op1);
//     }
//         char *op2 = strtok(NULL, " ");  // e.g., R2
       
//             if (strcmp(op2, "R0") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R1") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R2") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R3") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R4") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R5") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R6") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R7") == 0) {
//         arr[9]=0; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R8") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R9") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R10") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R11") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R12") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R13") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R14") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R15") == 0) {
//         arr[9]=0; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R16") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R17") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R18") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R19") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R20") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R21") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R22") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R23") == 0) {
//         arr[9]=1; arr[10]=0; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R24") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R25") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R26") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R27") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=0; arr[12]=1; arr[13]=1;
//     } else if (strcmp(op2, "R28") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=0;
//     } else if (strcmp(op2, "R29") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=0; arr[13]=1;
//     } else if (strcmp(op2, "R30") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=0;
//     } else if (strcmp(op2, "R31") == 0) {
//         arr[9]=1; arr[10]=1; arr[11]=1; arr[12]=1; arr[13]=1;
//     } else {
//         printf("Invalid register: %s\n", op2);
    

//         } 
//         char *op3 = strtok(NULL, " "); 
//         if(op3[0]=='R'){
//             if (strcmp(op3, "R0") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R1") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R2") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R3") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R4") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R5") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R6") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R7") == 0) {
//                 arr[14]=0; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R8") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R9") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R10") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R11") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R12") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R13") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R14") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R15") == 0) {
//                 arr[14]=0; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R16") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R17") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R18") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R19") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=0; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R20") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R21") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R22") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R23") == 0) {
//                 arr[14]=1; arr[15]=0; arr[16]=1; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R24") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R25") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R26") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R27") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=0; arr[17]=1; arr[18]=1;
//             } else if (strcmp(op3, "R28") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=0;
//             } else if (strcmp(op3, "R29") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=0; arr[18]=1;
//             } else if (strcmp(op3, "R30") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=0;
//             } else if (strcmp(op3, "R31") == 0) {
//                 arr[14]=1; arr[15]=1; arr[16]=1; arr[17]=1; arr[18]=1;
//             } else {
//                 printf("Invalid register: %s\n", op3);
//             }



//     }
//     else{
//         int value = atoi(op3);
//         printf("%d\n", value);
//         int bits = 18;          // Number of bits to store
//         int end = 31;           // LSB at arr[31]
//         int start = 14;         // MSB at arr[14]

//         // Two's complement for negative numbers
//         if (value < 0) {
//             value = (1 << bits) + value;
//         }

//         // Store from arr[31] to arr[14] (LSB → MSB)
//         for (int i = 0; i < bits; i++) {
//             arr[end - i] = value % 2;
//             value = value / 2;
//         }




//     }
//     }




























//     if (type==2) {
//         char *op1 = strtok(NULL, " ");  // e.g., R1
//         int value = atoi(op1);
//         printf("%d\n", value);
//         int bits = 28;        // SHAMT is 13 bits wide
//         int start = 4;       // MSB goes here

//         unsigned int uval = value & ((1 << bits) - 1);  // ensure we only keep 13 bits

//         for (int i = 0; i < bits; i++) {
//             arr[start + i] = (uval >> (bits - 1 - i)) & 1;
//         }


//     }












//     return arr;
// }

// // int main() {
// //     char line1[] = "JEQ R1 R2 5";
// //     // char line2[] = "MOVI R3 5";
// //     // char line3[] = "ADD R1 R2 R3";
// //     // char line4[] = "MUL R4 R1 R5";
// //      int *arr1 = processLine(line1);
// //     // int *arr2 = processLine(line2);
// //     // int *arr3 = processLine(line3);
// //     // int *arr4 = processLine(line4);

// //     // Print first 4 bits of the instruction
// //     printf("Opcode: ");
// //     for (int i = 0; i < 32; i++) {
// //         printf("%d", arr1[i]);
// //          }
// //          printf("\n");
// //         // printf("\n");
// //     //     // char str[] = "-45";
// //     //     // int value = atoi(str);
// //     //     // printf("%d\n", value);
// //     //     printf("Opcode: ");
// //     //     for (int i = 0; i < 32; i++) {
// //     //         printf("%d", arr2[i]);
// //     //          }
// //     //          printf("\n");
// //     // printf("Opcode: ");
// //     // for (int i = 0; i < 32; i++) {
// //     //     printf("%d", arr3[i]);
// //     //      }
// //     //      printf("\n");
// //     //     // printf("\n");
// //     //     // char str[] = "-45";
// //     //     // int value = atoi(str);
// //     //     // printf("%d\n", value);
// //     //     printf("Opcode: ");
// //     //     for (int i = 0; i < 32; i++) {
// //     //         printf("%d", arr4[i]);
// //     //          }

// //         return 0;
// //     }

