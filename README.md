# C-Processor Simulator

A pipelined processor simulator implemented in C with a React-based GUI interface.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Pipeline Stages](#pipeline-stages)
- [File Formats](#file-formats)
- [GUI Interface](#gui-interface)

## Overview

This project implements a pipelined processor simulator that executes assembly instructions through multiple pipeline stages: Fetch, Decode, Execute, Memory, and Write Back. It includes both a command-line interface and a graphical user interface.

## Features

- 5-stage pipeline implementation
- Support for various instruction types (arithmetic, memory, branch)
- Register file simulation (R0-R31 + PC)
- Memory simulation (2048 locations)
- Pipeline hazard handling
- Graphical user interface
- Instruction logging and debugging

## Project Structure

```
c-processor/
├── src/                    # Source files
│   ├── main.c             # Main program
│   ├── fetch-decode.c     # Fetch and decode stages
│   ├── execution.c        # Execute stage
│   ├── logger.c           # Logging functionality
│   └── queue.c            # Queue implementation
├── include/               # Header files
├── data/                  # Input/output files
├── c-processor-gui/       # React GUI application
└── README.md             # Documentation
```

## Installation

### Prerequisites

- C compiler (GCC recommended)
- Node.js and npm (for GUI)
- Visual Studio Code (recommended)

### Building the Project

1. Clone the repository:

```bash
git clone https://github.com/yourusername/c-processor.git
cd c-processor
```

2. Build the C program:

```bash
gcc src/*.c -I include/ -o processor
```

3. Install and build the GUI:

```bash
cd c-processor-gui
npm install
npm start
```

## Usage

### Command Line Interface

1. Create an input file with assembly instructions in `data/test.txt`
2. Run the processor:

```bash
./processor
```

### GUI Interface

1. Start the React application:

```bash
cd c-processor-gui
npm start
```

2. Access the GUI at `http://localhost:3000`
3. Use the interface to input instructions and run the simulation

## Pipeline Stages

### Fetch

- Retrieves instruction from memory
- Updates PC
- Outputs: IR (Instruction Register)

### Decode

- Decodes instruction format
- Reads register values
- Outputs: Control signals, register values

### Execute

- Performs ALU operations
- Handles branch decisions
- Outputs: ALU result, branch decision

### Memory

- Performs memory access (read/write)
- Handles data memory operations
- Outputs: Memory data

### Write Back

- Writes results back to registers
- Updates processor state
- Outputs: Final register values

## File Formats

### Input File (test.txt)

```
ADD R1, R2, R3
SUB R4, R5, R6
...
```

### Output Files

- `memory_log.txt`: Memory state logs
- `registers_log.txt`: Register state logs

## GUI Interface

The GUI provides:

- Instruction input interface
- Pipeline stage visualization
- Register and memory state display
- Real-time execution monitoring

## Output Format

### Register Log Format

```
R0: 00000000000000000000000000000000
R1: 00000000000000000000000000000001
...
R31: 00000000000000000000000000000000
PC = 00000000000000000000000000000100
```

### Memory Log Format

```
Address 1: 00000000000000000000000000000000
Address 2: 00000000000000000000000000000001
...
```
