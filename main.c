#include <stdio.h>
#include <stdlib.h>

//ALU[5], branch, memory read, memory write, register write, memory to register, shift[2]
//if for example ALU[0] = 1 then ADD for example, if branch is 1 then will branch, if shift[1] == 1 then shift right
void execute(int ALU[], int branch, int memoryRead, int memoryWrite, int registerWrite, int memoryToRegister, int shft[] , int R1, int R2, int R3){
    for(int i = 0; i<5; i++){
        if(ALU[i] == 1){
            char * instruction = "";
            switch(i)
            {
                case 0: instruction = "ADD";break;
                case 1: instruction = "SUB";break;
                case 2: instruction = "MUL";break;
                case 3: instruction = "AND";break;
                case 4: instruction = "XORI";break;
            }
            alu(instruction, R1, R2, R3);
            return;
        }
    }
    if(branch == 1){

        return;
    }
    if(memoryRead == 1){
            
        return;
    }
    if(memoryWrite == 1){
        return;
    }
    if(registerWrite == 1){
        return;
    }
    if(memoryToRegister == 1){
        return;
    }
    for(int i = 0; i<2; i++){
        if(shft[i] == 1){
            char * instruction = "";
            switch(i)
            {
                case 0: instruction = "LSL";break;
                case 1: instruction = "LSR";break;
            }
            shift(instruction, R1, R2, R3);
            return;
        }
    }
}

void alu(char * instruction, int R1, int R2, int R3)
{

}

void shift(char * instruction, int R1, int R2, int R3)
{

}

int main()
{
    return 0;
}