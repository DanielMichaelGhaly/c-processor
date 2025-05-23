#ifndef execution_h
#define execution_h

int* execute(int ALU[], int branch, int memoryRead, int memoryWrite, int registerWrite, 
    int shft[] , int R1, int R2, int R3);

int alu(char * instruction, int R1, int R2, int R3);

int shift(char * instruction, int R1, int R2, int R3);

#endif