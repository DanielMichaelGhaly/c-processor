//include libraries: 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>



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

//Clock: 
static int clk_cycle = 0;


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
int PC[32] ={0};
int IR[32] = {0};
int ALU[32] = {0};
int AR[32] = {0};
int DR[32] = {0};


//data:
typedef struct {
    int opcode;
    int sig;
    int exec;
    int* R1;
    int* R2;
    int* R3;
    int SHAMT;
    int IMM;
    int ADDRESS;
} Data;

Data initD(){
  Data r = {-1,-1,-1,R0,R0,R0,-1,-1,-1};
  return r;
}

typedef struct Node {
    Data data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} QUEUE;

void init_queue(QUEUE* q) {
    q->front = q->rear = NULL;
}

void enqueue(QUEUE* q, Data d) {
    Node* node = malloc(sizeof(Node));
    node->data = d;
    node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

Data dequeue(QUEUE* q) {
    if (q->front == NULL) {
        printf("Queue underflow!\n");
        exit(1);  // Or handle safely
    }

    Node* temp = q->front;
    Data result = temp->data;
    q->front = temp->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return result;
}

int isEmpty(QUEUE* q) {
    return (q->front == NULL);
}

QUEUE fetch_stage;
QUEUE decode_stage;
QUEUE exec_stage;
QUEUE mem_stage;
QUEUE WB_stage;





// function that will be used later as a signal for write back stage:
int get_register_number(int* arr) {
    if (arr == R1) return 1;
    else if (arr == R2) return 2;
    else if (arr == R3) return 3;
    else if (arr == R4) return 4;
    else if (arr == R5) return 5;
    else if (arr == R6) return 6;
    else if (arr == R7) return 7;
    else if (arr == R8) return 8;
    else if (arr == R9) return 9;
    else if (arr == R10) return 10;
    else if (arr == R11) return 11;
    else if (arr == R12) return 12;
    else if (arr == R13) return 13;
    else if (arr == R14) return 14;
    else if (arr == R15) return 15;
    else if (arr == R16) return 16;
    else if (arr == R17) return 17;
    else if (arr == R18) return 18;
    else if (arr == R19) return 19;
    else if (arr == R20) return 20;
    else if (arr == R21) return 21;
    else if (arr == R22) return 22;
    else if (arr == R23) return 23;
    else if (arr == R24) return 24;
    else if (arr == R25) return 25;
    else if (arr == R26) return 26;
    else if (arr == R27) return 27;
    else if (arr == R28) return 28;
    else if (arr == R29) return 29;
    else if (arr == R30) return 30;
    else if (arr == R31) return 31;
    else if (arr == PC) return 0;
    else return -1;  // Not a known register
}

// Function to convert an integer to a 28-bit binary string
char* int_to_28bit_binary(int value) {
    char* binary_str = malloc(29);  // 28 bits + 1 for null terminator
    if (!binary_str) return NULL;

    for (int i = 27; i >= 0; i--) {
        binary_str[27 - i] = ((value >> i) & 1) + '0';  // Extract bit
    }
    binary_str[28] = '\0';  // Null-terminate the string

    return binary_str;
}


//creating functions for each operation: 
//1. ADD: (done)
int add(int R1[32], int R2[32], int R3[32]){
  int tmp1 = 0;
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);

  tmp1 = tmp2 + tmp3;
  write_int_into_register(tmp1,ALU);
  return (get_register_number(R1));
}

//2. SUB: (done)
int subtract(int R1[32], int R2[32], int R3[32]){
  int tmp1 = 0;
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);

  tmp1 = tmp2 - tmp3;
  write_int_into_register(tmp1,ALU);
  return (get_register_number(R1));
}

//3. MUL: (done)
int multiply(int R1[32], int R2[32], int R3[32]){
  int tmp1 = 0;
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);

  tmp1 = tmp2 * tmp3;
  write_int_into_register(tmp1,ALU);
  return (get_register_number(R1));
}

//4. MOVI: (done)
int move_immediate(int R1[32], int IMM){
  int tmp0 = read_from_register_and_convert_to_int(R0);
  int tmp1 = IMM;
  int res = tmp0+tmp1;
  write_int_into_register(res,ALU);
  return get_register_number(R1);

  
}

//5. JEQ: (done)
int jump_if_equal(int R1[32], int R2[32], int IMM){
  int tmp1 = read_from_register_and_convert_to_int(R1);
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int pctmp = read_from_register_and_convert_to_int(PC);
  int res = 0;
  if(tmp1 == tmp2 && IMM >=0){
    res = pctmp + IMM;
  }
  else{
    res = pctmp;
  }
  write_int_into_register(res,ALU);
  return get_register_number(PC);

}

//6. AND: (done)
int and(int R1[32], int R2[32], int R3[32]){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);
  int res = 0;
  res = tmp2 & tmp3;
  write_int_into_register(res,ALU);
  return get_register_number(R1);


}

//7. XORI: (done)
int exclusive_or_immediate(int R1[32], int R2[32], int IMM){
  int tmp1 = read_from_register_and_convert_to_int(R2);
  int res = 0;
  res = tmp1 ^ IMM;
  write_int_into_register(res,ALU);
  return get_register_number(R1);
}

//8. JMP: (done)
int jump(int ADDRESS){
  int res = 0;
  //char *two = malloc(sizeof(char)*29);
  //two = int_to_28bit_binary(ADDRESS);
  char *one = malloc(sizeof(char)*25);
  for(int i=0;i<4;i++){
    one[i] = PC[i] +'0';
  }
  one[4] = '\0';
  char result[32];
  strcpy(result, one);  
  printf("result: %s\n",result);
  strcat(result, int_to_28bit_binary(ADDRESS));
  res = convert_from_binary_string_to_int(result);
  free(one);
  //free(two);
  write_int_into_register(res,ALU);
  return get_register_number(PC);
  
}

//9. LSL: (done)
int logical_shift_left(int R1[32], int R2[32], int SHAMT){
    int tmp2 = read_from_register_and_convert_to_int(R2);
    int res = tmp2 << SHAMT;

    write_int_into_register(res,ALU); 
    return get_register_number(R1);
}

//10. LSR: (done)
int logical_shift_right(int R1[32], int R2[32], int SHAMT){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  unsigned int ua = (unsigned int)tmp2;
  unsigned int result = ua >> SHAMT;

  write_int_into_register(result,ALU); 
  return get_register_number(R1);
}

//11. MOVR: (done)
void move_to_register(int R1[32], int R2[32], int IMM){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int res = tmp2 + IMM;
  write_int_into_register(res,AR);

  return get_register_number(R1);

}


//12. MOVM:
void move_to_memory(int R1[32], int R2[32], int IMM){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int res = tmp2 + IMM;

  write_int_into_register(res,AR);
  return get_register_number(R1);

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
int convert_from_binary_string_to_int(const char* str) {
    int length = strlen(str);

    // Convert binary string to unsigned integer
    uint32_t value = (uint32_t)strtoul(str, NULL, 2);

    // If MSB is 1 (i.e., str[0] == '1'), perform manual sign-extension
    if (str[0] == '1') {
        // Two's complement: subtract 2^length
        return (int)(value - (1U << length)+1);
    }

    return (int)value;
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
  init_queue(&fetch_stage);
  init_queue(&decode_stage);
  init_queue(&exec_stage);
  init_queue(&mem_stage);
  init_queue(&WB_stage);

  int x = 0b100;
  Data a1 = initD();
  Data a2 = initD();
  a2.opcode = 2;
  enqueue(&fetch_stage,a1);
  enqueue(&fetch_stage,a2);
  Data a3 = dequeue(&fetch_stage);
  printf("the first data's opcode: %d\n",a3.opcode);
  a3 = dequeue(&fetch_stage);
  printf("the second data's opcode: %d\n",a3.opcode);



  printf("X: %d\n",x);
  write_int_into_register(50,R1);
  write_int_into_register(16,R2);
  write_int_into_register(-50,R4);
  

  int sig = logical_shift_right(R30,R2,2);
  int tmp1 = read_from_register_and_convert_to_int(R1);
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);
  int tmp4 = read_from_register_and_convert_to_int(R4);
  int aluc = read_from_register_and_convert_to_int(ALU);
  char* alucc = read_from_ram_and_convert_to_str(ALU);
  int pccc = read_from_register_and_convert_to_int(PC);
  char* pcc = read_from_ram_and_convert_to_str(PC);


  printf("Content of register1: %d\n",tmp1);
  printf("Content of register2: %d\n",tmp2);
  printf("Content of register3: %d\n",tmp3);
  printf("Content of register4: %d\n",tmp4);
  printf("Content of ALU: %d\n",aluc);
  printf("Content of ALU: %s\n",alucc);
  printf("Content of Sig: %d\n",sig);
  printf("Content of PC: %s\n",pcc);
  printf("Content of PC: %d\n",pccc);



  return 0;

}