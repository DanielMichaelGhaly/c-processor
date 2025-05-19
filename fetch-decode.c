
int pc = 0;

int pc_incr(int* pc){
    return pc += 1 ;
}

int clk = 1;

void fetch(int* pc){
    int instr = mem[*pc];
    pc_incr(pc); 
    // append instr to fetch queue assume clk
    // dequeu instr from fetch queue 
    decode(&instr);

}

void decode(int* instr){
    int opcode = (instr[0] << 3) | (instr[1] << 2) | (instr[2] << 1) | instr[3];
    int ALUsig = {0,0,0,0,0}
    switch (opcode) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x5:
        case 0x8:
        case 0x9: // R-instr
            int r1 = (instr[4] << 4) | (instr[5] << 3) | (instr[6] << 2) | (instr[7] << 1) | instr[8];
            int r2 = (instr[9] << 4) | (instr[10] << 3) | (instr[11] << 2) | (instr[12] << 1) | instr[13];
            int r3 = (instr[14] << 4) | (instr[15] << 3) | (instr[16] << 2) | (instr[17] << 1) | instr[18]; 
            int shamt = 0;
            int shamt = 0;
            for (int i = 0; i < 13; i++) {
                shamt |= (instr[19 + i] << (12 - i));
            }
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
            break;
        case 0x7: // J-instr
            int address = 0;
            for (int i = 0; i < 28; i++) {
                address |= (instr[4 + i] << (27 - i));
            }
            break;
        default:
            // Handle invalid opcode
            break;
    }
    // decode instr
    // append decoded instr to decode queue
}


