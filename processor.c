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
    int R1;
    int R2;
    int R3;
    int SHAMT;
    int IMM;
    int ADDRESS;
    char* inst;
} Data;

Data initD(){
  Data r = {-1,-1,-1,0,0,0,-1,-1,-1,"00000000000000000000000000000000"};
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
    else if (arr == R0) return 0;
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
  printf("I am currently adding content of Regs%d with content of Regs%d which will be saved later in Regs%d\n",get_register_number(R2),get_register_number(R3),get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Reg%d: %d\n",get_register_number(R3),tmp3);
  write_int_into_register(tmp1,ALU);
  printf("Now the ALU is: %d\n",tmp1);
  return (get_register_number(R1));
}

//2. SUB: (done)
int subtract(int R1[32], int R2[32], int R3[32]){
  int tmp1 = 0;
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);

  printf("I am currently subtracting content of Regs%d with content of Regs%d which will be saved later in Regs%d\n",get_register_number(R2),get_register_number(R3),get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Reg%d: %d\n",get_register_number(R3),tmp3);
  tmp1 = tmp2 - tmp3;
  write_int_into_register(tmp1,ALU);
  printf("Now the ALU is: %d\n",tmp1);
  return (get_register_number(R1));
}

//3. MUL: (done)
int multiply(int R1[32], int R2[32], int R3[32]){
  int tmp1 = 0;
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);
  printf("I am currently multiplying content of Regs%d with content of Regs%d which will be saved later in Regs%d\n",get_register_number(R2),get_register_number(R3),get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Reg%d: %d\n",get_register_number(R3),tmp3);
  tmp1 = tmp2 * tmp3;
  write_int_into_register(tmp1,ALU);
  printf("Now the ALU is: %d\n",tmp1);
  return (get_register_number(R1));
}

//4. MOVI: (done)
int move_immediate(int R1[32], int IMM){
  int tmp0 = read_from_register_and_convert_to_int(R0);
  int tmp1 = IMM;
  int res = tmp0+tmp1;
  printf("I am currently putting the content of Immediate into ALU which will be saved later in Regs%d\n",get_register_number(R1));
  printf("Immediate Value: %d\n",tmp1);
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",tmp1);
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
    res = -1;
  }
  printf("I am currently checking if the content of Regs%d is equal to the content of Regs%d, jump to immediate address\n",get_register_number(R1),get_register_number(R2));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R1),tmp1);
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Immediate Value: %d\n",IMM);
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",res);
  return res;

}

//6. AND: (done)
int and(int R1[32], int R2[32], int R3[32]){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int tmp3 = read_from_register_and_convert_to_int(R3);
  int res = 0;
  res = tmp2 & tmp3;
  printf("I am currently Anding content of Regs%d with content of Regs%d which will be saved later in Regs%d\n",get_register_number(R2),get_register_number(R3),get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Reg%d: %d\n",get_register_number(R3),tmp3);
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",res);
  return get_register_number(R1);

}

//7. XORI: (done)
int exclusive_or_immediate(int R1[32], int R2[32], int IMM){
  int tmp1 = read_from_register_and_convert_to_int(R2);
  int res = 0;
  res = tmp1 ^ IMM;
  printf("I am currently XORing the content of Regs%d with Immediate, which will be stored in Regs%d later\n",get_register_number(R2),get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp1);
  printf("Immediate Value: %d\n",IMM);
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",res);
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
  strcat(result, int_to_28bit_binary(ADDRESS));
  res = convert_from_binary_string_to_int(result);
  free(one);
  //free(two);
  printf("We will Jump the PC by some factor(Address = %d)\n",ADDRESS);
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",res);
  return get_register_number(PC);
  
}

//9. LSL: (done)
int logical_shift_left(int R1[32], int R2[32], int SHAMT){
    int tmp2 = read_from_register_and_convert_to_int(R2);
    int res = tmp2 << SHAMT;

    printf("I am currently logically shifting Left the content of Regs%d with shift amount of %d which will be saved later in Regs%d\n",get_register_number(R2),SHAMT,get_register_number(R1));
    printf("Contents of the Registers: \n");
    printf("Reg%d: %d\n",get_register_number(R2),tmp2);
    write_int_into_register(res,ALU); 
    printf("Now the ALU is: %d\n",res);
    return get_register_number(R1);
}

//10. LSR: (done)
int logical_shift_right(int R1[32], int R2[32], int SHAMT){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  unsigned int ua = (unsigned int)tmp2;
  unsigned int result = ua >> SHAMT;

  printf("I am currently logically shifting Right the content of Regs%d with shift amount of %d which will be saved later in Regs%d\n",get_register_number(R2),SHAMT,get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  write_int_into_register(result,ALU); 
  printf("Now the ALU is: %d\n",result);
  return get_register_number(R1);
}

//11. MOVR: (done)
int move_to_register(int R1[32], int R2[32], int IMM){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int res = tmp2 + IMM;
  write_int_into_register(res,ALU);
  printf("Now i am calculating the address of Memory that will be accessed: content of Regs%d + Immediate: %d which will then be stored in Regs%d\n",get_register_number(R2),IMM,get_register_number(R1));
  printf("Contents of the Registers: \n");
  printf("Reg%d: %d\n",get_register_number(R2),tmp2);
  printf("Now the ALU is: %d\n",res);
  return get_register_number(R1);

}

//12. MOVM:
int move_to_memory(int R1[32], int R2[32], int IMM){
  int tmp2 = read_from_register_and_convert_to_int(R2);
  int res = tmp2 + IMM;

  printf("I am now calculating the address of the memory (Content of Regs%d + Immediate: %d) in which the content of Regs%d will be saved \n",get_register_number(R2),IMM,get_register_number(R1));
  write_int_into_register(res,ALU);
  printf("Now the ALU is: %d\n",res);
  return read_from_register_and_convert_to_int(R1);

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


int* get_register_by_number(int reg_num) {
    switch (reg_num) {
        case 0:  return R0;
        case 1:  return R1;
        case 2:  return R2;
        case 3:  return R3;
        case 4:  return R4;
        case 5:  return R5;
        case 6:  return R6;
        case 7:  return R7;
        case 8:  return R8;
        case 9:  return R9;
        case 10: return R10;
        case 11: return R11;
        case 12: return R12;
        case 13: return R13;
        case 14: return R14;
        case 15: return R15;
        case 16: return R16;
        case 17: return R17;
        case 18: return R18;
        case 19: return R19;
        case 20: return R20;
        case 21: return R21;
        case 22: return R22;
        case 23: return R23;
        case 24: return R24;
        case 25: return R25;
        case 26: return R26;
        case 27: return R27;
        case 28: return R28;
        case 29: return R29;
        case 30: return R30;
        case 31: return R31;
        default: return NULL;  // Invalid register number
    }
}



//fetch code: 
void fetch(){
  printf("I am Currently Fetching 🔍\n");
  int line = read_from_register_and_convert_to_int(PC);
  printf("PC: %d\n",line);
  int z = read_from_ram_and_convert_to_int(ram[line]);
  char* x = read_from_ram_and_convert_to_str(ram[line]);
  printf("The instruction: %s\n",x);
  write_int_into_register(z,IR);
  line++;
  write_int_into_register(line,PC);
} 

Data decode(){
  printf("I am currently Decoding 👨🏿‍💻, (instruction: %s = %d)\n",read_from_ram_and_convert_to_str(&IR),read_from_register_and_convert_to_int(IR));
  char opcd[5];
  for(int i=0;i<4;i++){
    opcd[i]= IR[i]+'0';
  }
  opcd[4]='\0';
  int q = (unsigned int)strtoul(opcd, NULL, 2);
  printf("The OPCODE of instruction being decoded is %s = %d\n",opcd,q);
  Data d = initD();
  d.opcode = q;
  d.inst = read_from_ram_and_convert_to_str(&IR);
  if (q == 0 || q == 1 || q == 2 || q == 5 || q == 8 || q == 9) {
    char Regs1[6];
    char Regs2[6];
    char Regs3[6];
    char SHA[14];
    for(int i=0;i<5;i++){
      Regs1[i]= IR[4+i]+'0';
    }
    Regs1[5]='\0';
    for(int i=0;i<5;i++){
      Regs2[i]= IR[9+i]+'0';
    }
    Regs2[5]='\0';
    for(int i=0;i<5;i++){
      Regs3[i]= IR[14+i]+'0';
    }
    Regs3[5]='\0';
    for(int i=0;i<13;i++){
      SHA[i]= IR[19+i]+'0';
    }
    SHA[13]='\0';
    int w = (unsigned int)strtoul(Regs1, NULL, 2);
    int e = (unsigned int)strtoul(Regs2, NULL, 2);
    int r = (unsigned int)strtoul(Regs3, NULL, 2);
    int t = (unsigned int)strtoul(SHA, NULL, 2);
    d.opcode = q;
    d.SHAMT = t;
    d.R1 = w;
    d.R2 = e;
    d.R3 = r;
    printf("The instruction being decoded is R-Format:\n");
    printf("R1: reg%d\n",w);
    printf("R2: reg%d\n",e);
    printf("R3: reg%d\n",r);
    printf("SHAMT: %d\n",t);
  }
  if (q == 3 || q == 4 || q == 6 || q == 10 || q == 11 ) {
    char Regs1[6];
    char Regs2[6];
    char Im[19];
    for(int i=0;i<5;i++){
      Regs1[i]= IR[4+i]+'0';
    }
    Regs1[5]='\0';
    for(int i=0;i<5;i++){
      Regs2[i]= IR[9+i]+'0';
    }
    Regs2[5]='\0';
    for(int i=0;i<18;i++){
      Im[i]= IR[14+i]+'0';
    }
    Im[18]='\0';
    int w = (unsigned int)strtoul(Regs1, NULL, 2);
    int e = (unsigned int)strtoul(Regs2, NULL, 2);
    int r = (unsigned int)strtoul(Im, NULL, 2); //only positive :(
    d.opcode = q;
    d.R1 = w;
    d.R2 = e;
    d.IMM = r;
    printf("The instruction being decoded is I-Format:\n");
    printf("R1: reg%d\n",w);
    printf("R2: reg%d\n",e);
    printf("Immediate Value: %d\n",r);
  }
  if(q==7){
    char Ad[29];
    for(int i=0;i<28;i++){
      Ad[i]= IR[4+i]+'0';
    }
    Ad[28]='\0';
    d.opcode = q;
    int r = (unsigned int)strtoul(Ad, NULL, 2);
    d.ADDRESS = r;
    printf("The instruction being decoded is J-Format:\n");
    printf("Address: %d\n",r);
  }
  if(q>11){
    printf("Oops⚠️...This instruction is not in my Instruction Set😟\n");
  }

  return d;
}

Data execute(Data* d){
  printf("I am Currently executing ⚙️ (instruction: %s)\n",d->inst);
  int z = 0;
  switch (d->opcode) {
        case 0:  
          z = add(get_register_by_number(d->R1),get_register_by_number(d->R2),get_register_by_number(d->R3));
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break;
        case 1:  
          z = subtract(get_register_by_number(d->R1),get_register_by_number(d->R2),get_register_by_number(d->R3));
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 2: 
          z = multiply(get_register_by_number(d->R1),get_register_by_number(d->R2),get_register_by_number(d->R3));
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 3:  
          z = move_immediate(get_register_by_number(d->R1),d->IMM);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 4:
          z = jump_if_equal(get_register_by_number(d->R1),get_register_by_number(d->R2),d->IMM);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 5: 
          z = and(get_register_by_number(d->R1),get_register_by_number(d->R2),get_register_by_number(d->R3));
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 6:  
          z = exclusive_or_immediate(get_register_by_number(d->R1),get_register_by_number(d->R2),d->IMM);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 7: 
          z = jump(d->ADDRESS);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 8:  
          z = logical_shift_left(get_register_by_number(d->R1),get_register_by_number(d->R2),d->SHAMT);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 9:  
          z = logical_shift_right(get_register_by_number(d->R1),get_register_by_number(d->R2),d->SHAMT);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 10: 
          z = move_to_register(get_register_by_number(d->R1),get_register_by_number(d->R2),d->IMM);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        case 11:
          z = move_to_memory(get_register_by_number(d->R1),get_register_by_number(d->R2),d->IMM);
          d->exec = read_from_register_and_convert_to_int(ALU);
          d->sig = z;
          break; 
        default: printf("Invalid Opcode\n");// Invalid register number
    }
    return *d;

}

Data Memory(Data* d){
  printf("I am Currently in the memory phase 📝 (instruction: %s)\n",d->inst);
  if(d->opcode == 10){
     printf("I have now accessed Ram[%d]\n",d->exec);
     int z = read_from_ram_and_convert_to_int(ram[d->exec]);
     d->exec = z;
  }
  else if(d->opcode == 11){
    printf("I have now accessed Ram[%d]\n",d->exec);
    printf("I have now inserted the value of %d into that memory place\n",d->sig);
    write_int_into_register(d->sig,ram[d->exec]) ;   
    printf("We are now done with instruction %s\n",d->inst);
  }
  else{
    printf("This instruction doesn't want to access the RAM 🤷‍♂️\n");
  }
  return *d;
}


void write_back(Data* d){
  printf("I am currently in the write-back phase ⬅️ (instruction: %s)\n",d->inst);
  if(d->R1 == 0){
    printf("Sorry...I cant put smth in Reg0\n");
  }
  else{
  switch(d->opcode){
    case 0:
    case 1: 
    case 2: 
    case 3: 
    case 5:
    case 6:
    case 8:
    case 9:
    case 10: 
      printf("The register that is being modified is Regs%d \n",d->sig);
      printf("Regs%d before: %d\n",d->sig,read_from_register_and_convert_to_int(get_register_by_number(d->sig)));
      write_int_into_register(d->exec,get_register_by_number(d->sig));
      printf("Regs%d after: %d\n",d->sig,d->exec);
      break;
    case 4:
      if(d->sig == -1){
        printf("No jumping... PC will not be modified\n");
        break;
      }
      else{
        printf("The register that is being modified is the PC\n");
        printf("PC before: %d\n",read_from_register_and_convert_to_int(PC));
        write_int_into_register(d->exec,PC);
        printf("PC after: %d\n",d->exec);
        break;
      }
    case 7: 
      printf("The register that is being modified is the PC\n");
      printf("PC before: %d\n",read_from_register_and_convert_to_int(PC));
      write_int_into_register(d->exec,PC);
      printf("PC after: %d\n",d->exec);
      break;
    case 11:
      printf("This instruction was already finished from the memory phase\n");
      break;
    default: 
      printf("I dont seem to understand this instruction 🤔...Ohh its not in my ISA\n");
  }
  }
  printf("We are now done with instruction %s 🥳\n",d->inst);



}

    
int imain(){
  init_queue(&fetch_stage);
  init_queue(&decode_stage);
  init_queue(&exec_stage);
  init_queue(&mem_stage);
  init_queue(&WB_stage);
  //init();
  printf("Test: %s\n",read_from_ram_and_convert_to_str(ram[0]));
  write_int_into_register(537149441,IR);
  write_int_into_register(50,R1);
  write_int_into_register(10,R2);
  write_int_into_register(-50,R4);
  /*enqueue(&decode_stage,decode());
  Data zz;
  zz = dequeue(&decode_stage);
  Data qq;
  qq = execute(&zz);
  Data yy;
  yy = Memory(&qq);
  write_back(&yy);
*/
  


  for(int i=0;i<10;i++){
    clk_cycle ++;
    printf("Cycle Num: %d\n",clk_cycle);
    if(clk_cycle % 2 != 0){
      fetch();
      if(isEmpty(&mem_stage)==0){
        Data pp = dequeue(&mem_stage);
        write_back(&pp);
      }

    }
    else{
      enqueue(&decode_stage,decode());
      if(clk_cycle>2){
        Data ee = dequeue(&decode_stage);
        Data ff = execute(&ee);
        enqueue(&exec_stage,ff);
        if(clk_cycle>4){
          Data mm = dequeue(&exec_stage);
          Data m = Memory(&mm);
          enqueue(&mem_stage,m);
        }
      }


    }
  fflush(stdout);  
  Sleep(1000);  // Pause for 1 second

  }


  return 0;

}