
int pc = 0;

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

extern ControlSignals ctrl;
extern DecodedFields df;


// Initialize or reset control signals
void reset_control_signals(void);
// Initialize or reset decoded fields
void reset_decoded_fields(void);

ControlSignals ctrl;
DecodedFields df;

int pc_incr(int* pc){
    return pc += 1 ;
}



void fetch(int* pc){
    int instr = read_from_memory(read_from_register(pc)); // read instruction from memory
    pc_incr(pc); 
    // append instr to fetch queue assume clk
    // dequeu instr from fetch queue 
}


void decode(int* instr){
    int opcode = (instr[0] << 3) | (instr[1] << 2) | (instr[2] << 1) | instr[3];
    int ALUsig[5] = {0,0,0,0,0};
    int shift[2] = {0,0};
    int memR = 0;   
    int memW = 0;
    int regW = 0;
    int branch = 0;
    switch (opcode) {
        case 0x0: // R-instr
        case 0x1:
        case 0x2:
        case 0x5:
        case 0x8:
        case 0x9: // R-instr
            int r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int r3 = (instr[14] << 4) | (instr[15] << 3) | (instr[16] << 2) | (instr[17] << 1) | instr[18]; 
            int shamt = 0;
            for (int i = 0; i < 13; i++) {
                shamt |= (instr[19 + i] << (12 - i));
            }


            switch (opcode) {
                case 0x0: ALUsig[0] = 1; break; // ADD
                case 0x1: ALUsig[1] = 1; break; // SUB
                case 0x2: ALUsig[2] = 1; break; // MUL
                case 0x5: ALUsig[3] = 1; break; // AND
                case 0x8: shift[0] = 1; break; // shift left
                case 0x9: shift[1] = 1; break; // shift right
                default: /* */ break;
            }
            regW = 1;
            
            break;
        case 0x3:
        case 0x4:
        case 0x6:
        case 0xA:
        case 0xB: // I-instr
            int r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int immediate = 0;
            for (int i = 0; i < 18; i++) {
                immediate |= (instr[14 + i] << (17 - i));
            }

            switch (opcode) {
                case 0x3: regW = 1; break; // MOVI
                case 0x4: branch = 1; break; // JEQ
                case 0x6: ALUsig[4] = 1; regW = 1; break; // XORI
                case 0xA: regW = 1; memR = 1; break; // MOV to reg
                case 0xB: memW = 1; break; // Write to mem
                default: /* */ break;
            }

            break;
        case 0x7: // J-instr
            int address = 0;
            for (int i = 0; i < 28; i++) {
                address |= (instr[4 + i] << (27 - i));
            }

            branch = 1;
            break;
        default:
            // Handle invalid opcode
            break;
    }
    // decode instr
    // append decoded instr to decode queue
}


void reset_control_signals(void) {
    for(int i = 0; i < 5; ++i) ctrl.ALUsig[i] = 0;
    for(int i = 0; i < 2; ++i) ctrl.shift[i]  = 0;
    ctrl.memR   = 0;
    ctrl.memW   = 0;
    ctrl.regW   = 0;
    ctrl.branch = 0;
}

void reset_decoded_fields(void) {
    df.r1 = df.r2 = df.r3 = 0;
    df.shamt = 0;
    df.immediate = 0;
    df.address = 0;
}
