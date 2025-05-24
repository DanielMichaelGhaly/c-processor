#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define strcasecmp _stricmp
#include "processor.c"


#define MAX_LINES 100
#define MAX_LINE_LENGTH 100


char* get_opcode_binary(const char* mnemonic) {
    // Use static buffer to return string
    static char* binary = NULL;

    if (strcasecmp(mnemonic, "ADD") == 0) binary = "0000";
    else if (strcasecmp(mnemonic, "SUB") == 0) binary = "0001";
    else if (strcasecmp(mnemonic, "MUL") == 0) binary = "0010";
    else if (strcasecmp(mnemonic, "MOVI") == 0) binary = "0011";
    else if (strcasecmp(mnemonic, "JEQ") == 0) binary = "0100";
    else if (strcasecmp(mnemonic, "AND") == 0) binary = "0101";
    else if (strcasecmp(mnemonic, "XORI") == 0) binary = "0110";
    else if (strcasecmp(mnemonic, "JMP") == 0) binary = "0111";
    else if (strcasecmp(mnemonic, "LSL") == 0) binary = "1000";
    else if (strcasecmp(mnemonic, "LSR") == 0) binary = "1001";
    else if (strcasecmp(mnemonic, "MOVR") == 0) binary = "1010";
    else if (strcasecmp(mnemonic, "MOVM") == 0) binary = "1011";
    else binary = NULL;

    return binary;
}

char* get_register_binary(const char* reg) {
    if (reg[0] != 'R') return NULL;

    int num = atoi(&reg[1]);
    if (num < 0 || num > 31) return NULL;

    char* binary = malloc(6);  // 5 bits + '\0'
    for (int i = 4; i >= 0; i--) {
        binary[4 - i] = ((num >> i) & 1) + '0';
    }
    binary[5] = '\0';
    return binary;
}


char* get_13bit_binary(const char* str) {
    static char binary[14];  // 13 bits + null terminator

    int num = atoi(str);  // convert string to int

    // handle negative values as 2's complement (optional)
    if (num < 0) {
        num = (1 << 13) + num;  // 2's complement for 13 bits
    }

    for (int i = 12; i >= 0; i--) {
        binary[12 - i] = ((num >> i) & 1) + '0';
    }

    binary[13] = '\0';  // null-terminate
    return binary;
}


char* get_18bit_binary(const char* str) {
    static char binary[19];  // 18 bits + null terminator

    int num = atoi(str);  // convert string to int

    // Handle negative values using 2's complement (optional)
    if (num < 0) {
        num = (1 << 18) + num;  // Apply 2's complement for 18 bits
    }

    for (int i = 17; i >= 0; i--) {
        binary[17 - i] = ((num >> i) & 1) + '0';
    }

    binary[18] = '\0';  // Null-terminate
    return binary;
}


char* get_28bit_binary(const char* str) {
    static char binary[29];  // 28 bits + null terminator

    int num = atoi(str);  // Convert string to int

    // Handle negative values using 2's complement for 28 bits
    if (num < 0) {
        num = (1 << 28) + num;
    }

    for (int i = 27; i >= 0; i--) {
        binary[27 - i] = ((num >> i) & 1) + '0';
    }

    binary[28] = '\0';  // Null-terminate
    return binary;
}



int main() {
    init();
    FILE* file = fopen("instructions.txt", "r");
    if (file == NULL) {
        printf("Error: could not open file.\n");
        return 1;
    }

    char* instructions[MAX_LINES];
    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL && count < MAX_LINES) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';

        // Allocate memory and copy the line
        instructions[count] = malloc(strlen(line) + 1);
        strcpy(instructions[count], line);
        count++;
    }

    fclose(file);

    // ✅ Parse each instruction line into words
for (int i = 0; i < count; i++) {
    printf("\nInstruction %d: %s\n", i + 1, instructions[i]);

    // Create a copy of the line (because strtok modifies the string)
    char line_copy[MAX_LINE_LENGTH];
    strcpy(line_copy, instructions[i]);

    // Array of word pointers
    char* words[10];  // Assume max 10 words per instruction
    int word_count = 0;

    // Tokenize the line by spaces
    char* token = strtok(line_copy, " ");
    while (token != NULL && word_count < 10) {
        words[word_count++] = token;
        token = strtok(NULL, " ");
    }

    // Print the words
    printf("Words: [");
    for (int j = 0; j < word_count; j++) {
        printf("%s", words[j]);
        if (j < word_count - 1) printf(", ");
    }
    printf("]\n");
    char res[32];
    if(strcmp(words[0], "ADD") == 0 || strcmp(words[0], "SUB") == 0 || strcmp(words[0], "MUL") == 0 || strcmp(words[0], "AND") == 0){
      char* a = get_opcode_binary(words[0]);
      char* b = get_register_binary(words[1]);
      char* c = get_register_binary(words[2]);
      char* d = get_register_binary(words[3]);
      printf("Testa %s\n",a);
      printf("Testb %s\n",b);
      printf("Testc %s\n",c);
      printf("Testd %s\n",d);
      strcpy(res, a); 
      strcat(res,b);
      strcat(res,c);
      strcat(res,d);
      strcat(res,"0000000000000");
    }
    if(strcmp(words[0], "MOVI") == 0){
      char* e = get_opcode_binary(words[0]);
      char* f = get_register_binary(words[1]);
      char* g = get_18bit_binary(words[2]);
      strcpy(res, e); 
      strcat(res,f);
      strcat(res,"00000");
      strcat(res,g);

    }
    if(strcmp(words[0], "JEQ") == 0 || strcmp(words[0], "XORI") == 0 || strcmp(words[0], "MOVR") == 0 || strcmp(words[0], "MOVM") == 0){
      char* h = get_opcode_binary(words[0]);
      char* i = get_register_binary(words[1]);
      char* j = get_register_binary(words[2]);
      char* k = get_18bit_binary(words[3]);

      strcpy(res, h); 
      strcat(res,i);
      strcat(res,j);
      strcat(res,k);
    }
    if(strcmp(words[0], "JMP") == 0){
      char* l = get_opcode_binary(words[0]);
      char* m = get_28bit_binary(words[1]);
      strcpy(res, l); 
      strcat(res,m);
    }
    if(strcmp(words[0], "LSL") == 0 || strcmp(words[0], "LSR") == 0){
      char* n = get_opcode_binary(words[0]);
      char* o = get_register_binary(words[1]);
      char* p = get_register_binary(words[2]);
      char* q = get_13bit_binary(words[3]);
      strcpy(res, n); 
      strcat(res,o);
      strcat(res,p);
      strcat(res,"00000");
      strcat(res,q);
      
    }
    printf("Result: %s\n",res);
    int zz = convert_from_binary_string_to_int(res);

    write_int_into_register(zz,ram[i]);

}

    // ✅ Free allocated memory
    for (int i = 0; i < count; i++) {
        free(instructions[i]);
    }
    imain(count);

    return 0;
}
