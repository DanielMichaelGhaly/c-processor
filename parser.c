#include <stdio.h>
#include <string.h>

int* processLine(char line[]) {
    static int arr[32] = {0};  // Persistent storage (not lost after return)
    char *token = strtok(line, " ");  // Get the first token

    if (token != NULL) {
        printf("First token: %s\n", token);  // Example: "ADD"

        if (strcmp(token, "ADD") == 0) {
            arr[0] = 0; arr[1] = 0; arr[2] = 0; arr[3] = 0;
            printf("Instruction: Add\n");

        } else if (strcmp(token, "SUB") == 0) {
            arr[0] = 0; arr[1] = 0; arr[2] = 0; arr[3] = 1;
            printf("Instruction: Subtract\n");

        } else if (strcmp(token, "MUL") == 0) {
            arr[0] = 0; arr[1] = 0; arr[2] = 1; arr[3] = 0;
            printf("Instruction: Multiply\n");

        } else if (strcmp(token, "MOVI") == 0) {
            arr[0] = 0; arr[1] = 0; arr[2] = 1; arr[3] = 1;
            printf("Instruction: Move Immediate\n");

        } else if (strcmp(token, "JEQ") == 0) {
            arr[0] = 0; arr[1] = 1; arr[2] = 0; arr[3] = 0;
            printf("Instruction: Jump if Equal\n");

        } else if (strcmp(token, "AND") == 0) {
            arr[0] = 0; arr[1] = 1; arr[2] = 0; arr[3] = 1;
            printf("Instruction: And\n");

        } else if (strcmp(token, "XORI") == 0) {
            arr[0] = 0; arr[1] = 1; arr[2] = 1; arr[3] = 0;
            printf("Instruction: Exclusive Or Immediate\n");

        } else if (strcmp(token, "JMP") == 0) {
            arr[0] = 0; arr[1] = 1; arr[2] = 1; arr[3] = 1;
            printf("Instruction: Jump\n");

        } else if (strcmp(token, "LSL") == 0) {
            arr[0] = 1; arr[1] = 0; arr[2] = 0; arr[3] = 0;
            printf("Instruction: Logical Shift Left\n");

        } else if (strcmp(token, "LSR") == 0) {
            arr[0] = 1; arr[1] = 0; arr[2] = 0; arr[3] = 1;
            printf("Instruction: Logical Shift Right\n");

        } else if (strcmp(token, "MOVR") == 0) {
            arr[0] = 1; arr[1] = 0; arr[2] = 1; arr[3] = 0;
            printf("Instruction: Move to Register\n");

        } else if (strcmp(token, "MOVM") == 0) {
            arr[0] = 1; arr[1] = 0; arr[2] = 1; arr[3] = 1;
            printf("Instruction: Move to Memory\n");

        } else {
            printf("Unknown instruction: %s\n", token);
        }
    }

    return arr;
}

int main() {
    char line[] = "MOVM R1 R2 R3";
    int *arr = processLine(line);

    // Print first 4 bits of the instruction
    printf("Opcode: ");
    for (int i = 0; i < 4; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");

    return 0;
}
