# RISC-MC8 Assembler

For ISA version 4.5

---

## Usage

1) Create assembly file.

2) Run using the following command:

    * assemble-risc-mc8 inputfile.asm output.o

3) The file output.o will be a binary file containing the assembled code. If an error occurs, a status will be printed to stderr and output.o will not be created.

---


## Syntax

### Labels

Labels must appear on their own line, followed by a : then a space.  

Examples:

* Label:
* Function: # Start of function call

Note that labels are not case sensitive, TARGET is the same as target and Target.  

### Registers

Register values may be present in either of two forms, their name or address. They are case insensitive.  

Examples:  

* ireg
* iREG
* r1
* R5
* 000
* 010
* 110

### Immediates

Immediate values may be provided either by an integer or binary string format.  

A binary string must appear as 0bXXXX where X is either 0 or 1 and the total length is the number of bits.  

Examples:  

* 0b0110
* 2
* -1

### Instructions

Instructions may start in any column of the line. They are case insensitive.  

Examples:  

* ADDi 000
* SUBi r1
* JUMP 42
* jump myLabel

### Comments

Comments are indicated by a # and may occur in any line. Anything after the # is ignored.  

Examples:  

* ADDI 001 # Add a value
* \# Blank line with comment
* Label: # This is a label
