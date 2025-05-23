#ifndef fetch_decode_h
#define fetch_decode_h

typedef struct {
    int ALUsig[5];
    int shift[2];
    int memR;
    int memW;
    int regW;
    int branch;
} ControlSignals;

typedef struct {
    int r1;
    int r2;
    int r3;
    int shamt;
    int immediate;
    int address;
} DecodedFields;


void initialize_with_zeros(int *arr, int size);

int * int_to_bin32(int value, int *arr);

int bin_to_int(int *arr, int n);

int * pc_incr(int* pc);

void fetch(int* pc);

void decode();

int access_register_file(int * reg_num);

void memory_access(int * address, int * data, int memR, int memW);

void write_back(int * data, int * address, int regW);

#endif