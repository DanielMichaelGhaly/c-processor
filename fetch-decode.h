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


// Initialize or reset control signals
void reset_control_signals(void);
// Initialize or reset decoded fields
void reset_decoded_fields(void);

#endif