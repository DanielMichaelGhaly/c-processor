//include libraries: 
#include <stdio.h>
#include <stdlib.h>


//Creating ram: 
int** ram;

void ram_initialize(){
ram = malloc(2048 * sizeof(int*));
for (int i=0;i<2048;i++){
  ram[i]=malloc(32*sizeof(int));
}
for (int j=0; j<2048;j++){
  for(int k=0;k<32;k++){
    ram[j][k]=0;
  }
}
}


//Creating Registers: 
int R1[32] = {0};
int R2[32]= {0};
int R3[32]= {0};
int R4[32]= {0};
int R5[32]= {0};
int R6[32]= {0};
int R7[32]= {0};
int R8[32]= {0};
int R9[32]= {0};
int R10[32]= {0};
int R11[32]= {0};
int R12[32]= {0};
int R13[32]= {0};
int R14[32]= {0};
int R15[32]= {0};
int R16[32]= {0};
int R17[32]= {0};
int R18[32]= {0};
int R19[32]= {0};
int R20[32]= {0};
int R21[32]= {0};
int R22[32]= {0};
int R23[32]= {0};
int R24[32]= {0};
int R25[32]= {0};
int R26[32]= {0};
int R27[32]= {0};
int R28[32]= {0};
int R29[32]= {0};
int R30[32]= {0};
int R31[32]= {0};
const int R0[32] = {0};
int PC[11] ={0};


//creating functions for each operation: 
//1. ADD:
void add(int* R1, int* R2, int* R3){

}
//2. SUB: 
void subtract(int* R1, int* R2, int* R3){

}
//3. MUL:
void multiply(int* R1, int* R2, int* R3){

}
//4. MOVI:
void move_immediate(int* R1, int IMM){
  
}
//5. JEQ:
void jump_if_equal(int* R1, int* R2, int IMM){
  
}
//6. AND:
void and(int* R1, int* R2, int* R3){
  
}
//7. XORI:
void exclusive_or_immediate(int* R1, int* R2, int IMM){
  
}
//8. JMP:
void jump(int ADDRESS){
  
}
//9. LSL:
void logical_shift_left(int* R1, int* R2, int SHAMT){
  
}
//10. LSR:
void logical_shift_right(int* R1, int* R2, int SHAMT){
  
}
//11. MOVR:
void move_to_register(int* R1, int* R2, int IMM){
  
}
//12. MOVM:
void move_to_memory(int* R1, int* R2, int IMM){
  
}



void init(){
  ram_initialize();
}


int main(){

  init();
  printf("Memory: %d",PC[5]);


  return 0;

}