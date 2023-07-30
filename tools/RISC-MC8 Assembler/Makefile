CC = gcc
CFLAGS = -std=c11
CFLAGS_GDB = -ggdb3 -Wall
TARGET = assemble-risc-mc8
MAINFILE = Main.c
LIBS = InstructionParser.c Instructions.c Registers.c Symbols.c

assemble:
	$(CC) $(MAINFILE) -o $(TARGET) $(LIBS) $(CFLAGS)

debug:
	$(CC) $(MAINFILE) -o $(TARGET) $(LIBS) $(CFLAGS) $(CFLAGS_GDB)

