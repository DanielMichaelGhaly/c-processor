//include libraries: 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

/* 1. reading from the ram ----> string 
   2. convert the string of binary to an int
   3. Created a function that does both; read from the ram and convert to int directly
   4. reading from register -----> int
   5. writing int to register

*/

//1. 
char* read_from_ram_and_convert_to_str(int* row){
  char* str = malloc(33*sizeof(char));
  for(int i=0;i<32;i++){
    str[i] = row[i] +'0';
  }
  str[32] = '\0';
  return str;
}

//2. 
int convert_from_binary_string_to_int(char* str) {
    int length = strlen(str);

    // Parse as unsigned to avoid sign-extension issues
    unsigned int value = (unsigned int)strtol(str, NULL, 2);

    // If the most significant bit (MSB) is 1 → negative number in two's complement
    if (str[0] == '1') {
        // Compute correct negative value based on bit-width
        unsigned int mask = 1U << (length - 1);  // Sign bit mask
        value = value - (mask << 1);             // Equivalent to value - 2^length
    }

    return (int)value;  // Return as signed 32-bit integer
}

//3.
int read_from_ram_and_convert_to_int(int* row){
  char* str = read_from_ram_and_convert_to_str(row);
  int x = convert_from_binary_string_to_int(str);
  return x;

}

//4. 
int read_from_register_and_convert_to_int(int reg[32]) {
    unsigned int value = 0;

    // Build the integer value from the register bits
    for (int i = 0; i < 32; i++) {
        value = (value << 1) | (reg[i] & 1);  // Shift left and add the current bit
    }

    // If the MSB (reg[0]) is 1, apply two's complement adjustment
    if (reg[0] == 1) {
        value -= (1UL << 32);  // Equivalent to value - 2^32
    }

    return (int)value;  // Cast to signed int
}


//5. 
void write_int_into_register(int value, int reg[32]) {
    for (int i = 31; i >= 0; i--) {
        reg[31 - i] = (value >> i) & 1;  // MSB to LSB
    }
}




int main(){

  init();
  int x = 0b100;
  printf("X: %d\n",x);

  char* res = read_from_ram_and_convert_to_str(ram[5]);
  printf("Instrunction: %s\n",res);

  char* test = "11010";
  printf("number converted: %d\n",convert_from_binary_string_to_int(test));

  int z = read_from_register_and_convert_to_int(R1);
  printf("z= %d\n",z);

  write_int_into_register(-50,R1);

  int y = read_from_register_and_convert_to_int(R1);
  printf("y= %d\n",y);

  int yy = read_from_ram_and_convert_to_int(ram[5]);
  printf("read from ram to integer: %d\n",yy);

  write_int_into_register(-6,R1);
  int yyy = read_from_register_and_convert_to_int(R1);
  printf("read from ram last: %d\n",yyy);
  

  return 0;

}