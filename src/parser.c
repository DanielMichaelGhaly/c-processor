#include "parser.h"

const char* instruction_names[12] = {
    "ADD", "SUB", "MUL", "MOVI", "JEQ", "AND", "XORI",
    "JMP", "LSL", "LSR", "MOVR", "MOVM"
};

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
int parse_instruction(const char* line, int* binary) {
    if (line == NULL || strlen(line) == 0 || strspn(line, " \t\n") == strlen(line) || line[0]=='#') {
        return 1;
    }
    char mnemonic[10] = {0};
    char op1[10] = {0}, op2[10] = {0}, op3[10] = {0};
    int opcode, reg1, reg2, reg3, imm, addr;

    sscanf(line, "%9s %9[^, \t\n] %9[^, \t\n] %9[^, \t\n]", mnemonic, op1, op2, op3);
    opcode = get_opcode(mnemonic);
    if (opcode == -1) return 1;

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
        if(strcmp(mnemonic, "MOVI")==0)
        {
            imm = atoi(op2);
        }
        else{
            imm = atoi(op3);
        }

        write_binary(binary, reg1, 4, 5);
        write_binary(binary, reg2, 9, 5);
        write_binary(binary, imm, 14, 18);

    } else if (opcode == 7) { // J-type: JMP
        addr = atoi(op1);
        write_binary(binary, addr, 4, 28);
    }
    return 0;
}

void print_binary(int* binary) {
    for (int i = 0; i < 32; i++) printf("%d", binary[i]);
    printf("\n");
}
