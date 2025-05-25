# Custom Processor Design & Simulation Project

## Project Overview
This project implements a custom processor architecture with a 5-stage pipeline simulation in C. The processor features a custom Instruction Set Architecture (ISA) with 12 distinct instructions, supporting various addressing modes and instruction formats. The implementation demonstrates deep understanding of computer architecture concepts including pipelining, instruction execution, and memory management.

## Architecture Design

### Instruction Set Architecture (ISA)
The processor implements a RISC-like architecture with three instruction formats:
- **R-Format**: For register-to-register operations (ADD, SUB, MUL, AND, LSL, LSR)
- **I-Format**: For immediate and memory operations (MOVI, JEQ, XORI, MOVR, MOVM)
- **J-Format**: For unconditional jumps (JMP)

### Pipeline Stages
The processor implements a classic 5-stage pipeline:
1. **Fetch** (🔍): Retrieves instructions from memory using PC
2. **Decode** (👨🏿‍💻): Parses instruction formats and extracts operands
3. **Execute** (⚙️): Performs ALU operations based on instruction type
4. **Memory** (📝): Handles memory access for load/store operations
5. **Write Back** (⬅️): Updates register file with results

### Key Components
- **Register File**: 32 general-purpose registers (R0-R31)
- **Special Registers**: PC (Program Counter), IR (Instruction Register), ALU
- **Memory**: 2048-word RAM implementation
- **Pipeline Registers**: Dedicated queues for each pipeline stage

## Implementation Details

### Memory Management
- **RAM Implementation**: 2048x32 bit memory array
- **Memory Access**: Implemented read/write functions with binary-to-integer conversion
- **Memory Addressing**: Supports both direct and indirect addressing modes

### Pipeline Handling
- **Pipeline Stages**: Implemented using queue data structures
- **Hazard Detection**: Handles control hazards during branch/jump instructions
- **Pipeline Flushing**: Implemented for branch instructions using jflag mechanism

### Instruction Set Implementation
```
1. ADD  (0000): R1 = R2 + R3
2. SUB  (0001): R1 = R2 - R3
3. MUL  (0010): R1 = R2 * R3
4. MOVI (0011): R1 = IMM
5. JEQ  (0100): if(R1 == R2) PC = PC + IMM
6. AND  (0101): R1 = R2 & R3
7. XORI (0110): R1 = R2 ^ IMM
8. JMP  (0111): PC = ADDRESS
9. LSL  (1000): R1 = R2 << SHAMT
10. LSR (1001): R1 = R2 >> SHAMT
11. MOVR (1010): R1 = MEM[R2 + IMM]
12. MOVM (1011): MEM[R2 + IMM] = R1
```

### Binary Translation
- Implemented in `translate.c`
- Converts assembly instructions to 32-bit binary format
- Handles different instruction formats automatically
- Supports immediate value sign extension

### Data Structures
- **Pipeline Queues**: Custom queue implementation for pipeline stages
- **Data Structure**: Comprehensive struct for instruction data tracking
- **Binary Conversion**: Robust functions for binary-decimal conversions

## Technical Implementation Highlights

### Instruction Processing
- Custom `Data` structure to track instruction information:
  ```c
  typedef struct {
      int opcode;      // Operation code
      int sig;         // Signal for write-back
      int exec;        // Execution result
      int R1, R2, R3;  // Register operands
      int SHAMT;       // Shift amount
      int IMM;         // Immediate value
      int ADDRESS;     // Jump address
      char* inst;      // Instruction string
  } Data;
  ```

### Binary-Decimal Conversion
- Sophisticated binary-to-decimal conversion handling signed values
- Two's complement implementation for negative numbers
- Custom functions for different bit widths (13-bit, 18-bit, 28-bit)

### Pipeline Implementation
- Clock cycle-based execution alternating between odd and even cycles
- Fetch and Write-back occur in odd cycles
- Decode, Execute, and Memory access occur in even cycles
- Pipeline stalls handled through queue emptying during jumps (jflag mechanism)
- Clear stage separation using custom QUEUE data structure with front/rear pointers

### Memory Hierarchy
- Efficient memory access patterns
- Register file implementation with zero register (R0) handling
- Memory-register data movement optimization

### Control Flow
- Branch prediction implementation for JEQ instructions
- Jump handling with pipeline flushing
- Proper PC updates with instruction alignment

## Error Handling & Debug Features
- Comprehensive error checking for invalid operations
- Detailed execution logging for each pipeline stage
- Register state tracking and display
- Memory state visualization
- Cycle-accurate execution monitoring

## Performance Features
- Pipeline stage utilization tracking
- Instruction count monitoring
- Clock cycle accounting
- Pipeline efficiency metrics

## Testing & Validation
The processor has been tested with various assembly programs including:
- Arithmetic operations
- Logical operations
- Control flow instructions
- Memory access patterns
- Edge cases and error conditions

## Future Enhancements
1. Cache implementation
2. Branch prediction improvements
3. Superscalar execution support
4. Additional instruction support
5. Performance optimization tools

## Usage

1. Write your assembly code in the `instructions.txt` file. The supported instructions are:
   - R-Format: ADD, SUB, MUL, AND, LSL, LSR
   - I-Format: MOVI, JEQ, XORI, MOVR, MOVM
   - J-Format: JMP

2. Compile and run `translate.c`:
   ```powershell
   gcc translate.c -o translate.exe
   ./translate.exe
   ```
   This will:
   - Read the assembly instructions from `instructions.txt`
   - Convert them to 32-bit binary format
   - Load them into the simulated RAM
   - Start the processor simulation

3. Watch the detailed pipeline execution:
   - Each pipeline stage shows emojis to indicate its current operation:
     - Fetch (🔍)
     - Decode (👨🏿‍💻)
     - Execute (⚙️)
     - Memory (📝)
     - Write Back (⬅️)
   - The output shows detailed register contents and ALU operations
   - Memory state changes are displayed in real-time

4. Execution Speed Control:
   - You can modify the cycle time in `processor.c` (line 876)
   - Current default is `Sleep(500)` (500ms per cycle)
   - Adjust this value to speed up or slow down the simulation

## Project Structure
```
├── processor.c       # Main processor implementation
├── translate.c       # Assembly to binary translator
├── translate.h       # Header file for translator
├── instructions.txt  # Input assembly code
└── README.md        # Project documentation
```

This project demonstrates a deep understanding of computer architecture principles, showcasing the implementation of a complete processor pipeline with proper handling of instructions, hazards, and memory management.
