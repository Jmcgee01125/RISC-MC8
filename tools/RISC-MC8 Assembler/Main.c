#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "InstructionParser.h"
#include "StatusCodes.h"
#include "Symbols.h"

/**
 * @brief Assemble the indicated file to the indicated output file, using the RISC-MC8 instruction set
 *
 * @param argc Argument count
 * @param argv Arguments, should be `source.asm, output.o`
 * @return 0 if successful, otherwise a non-zero error code accompanied with a message on stout
 */
int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong number of arguments.\n");
        fprintf(stderr, "Expected arguments: source.asm output.o\n");
        return ERROR_INVALID_ARGUMENTS;
    }
    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Input file does not exist.\n");
        return ERROR_INVALID_ARGUMENTS;
    }

    printf("Extracting symbols...\n");

    SymbolsList* symbols = extractSymbols(inputFile, true);
    if (symbols == NULL) {
        fclose(inputFile);
        return ERROR_SYMBOLS_LIST_NULL;
    }
    rewind(inputFile);

    printf("Assembling instructions...\n");

    FILE* outputFile = fopen(argv[2], "wb");
    uint8_t parseStatus = parseInstructionsFile(inputFile, outputFile, symbols, true);
    fclose(inputFile);
    fclose(outputFile);
    freeSymbolsList(&symbols);
    if (parseStatus != 0) {
        remove(argv[2]);  // nuke output file if there was an error
    } else {
        printf("Finished successfully.\n");
    }
    return parseStatus;
}
