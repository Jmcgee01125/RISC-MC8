#include "InstructionParser.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Instructions.h"
#include "StatusCodes.h"

/**
 * @brief Parse the given instruction
 *
 * @param instructionDest The place to write the finished instruction
 * @param instructionLine The source instruction line, case insensitive
 * @param currentOffset The offset of the instruction being parsed
 * @param symbols List of symbols to use for translating
 * @return 0 if successful, otherwise returns the error that occurred
 */
uint8_t parseInstruction(uint8_t* const instructionDest, const char* const instructionLine, uint16_t currentOffset, SymbolsList* symbols)
{
    // get a local copy of instruction to use in strtok and other functions
    uint16_t length = strlen(instructionLine);
    char* instInParse = (char*)calloc(sizeof(char), length + 1);
    for (uint16_t i = 0; i < length && *(instructionLine + i) != '#'; i++) {
        *(instInParse + i) = tolower(*(instructionLine + i));
    }

    // create our token breaker and get the mnemonic
    char* token;
    token = strtok(instInParse, " \t\r\n");
    if (token == NULL || *(token + strlen(token) - 1) == ':') {
        free(instInParse);
        return STATUS_LINE_NOT_INSTRUCTION;
    }

    // using the name, get the instruction information
    const InstructionLoaderDefinition* iDef = getInstructionLoaderDefinition(token);
    if (iDef == NULL) {
        free(instInParse);
        return ERROR_UNKNOWN_MNEMONIC;
    }

    // load the parts of the instruction
    *instructionDest = iDef->instructionBase;
    token = strtok(NULL, " \t\r\n");
    if (token == NULL) {
        free(instInParse);
        return ERROR_MISSING_INSTRUCTION_PARAMETER;
    }
    uint8_t loaderStatus = iDef->tokenLoader(instructionDest, currentOffset, token, symbols);

    // error on `addi 000 000`
    token = strtok(NULL, " \t\r\n");
    if (token != NULL) {
        free(instInParse);
        return ERROR_TOO_MANY_TOKENS;
    }

    // return and say if we got an error
    free(instInParse);
    return loaderStatus;
}

/**
 * @brief Parse an assembly file and output the results to outputFile (overwrite)
 *
 * @param inputFile File to read instructions from
 * @param outputFile File to write assembled code to
 * @param symbols List of symbols to use for translating
 * @param printErrors If true, print errors, noting the line that they occurred on
 * @return 0 if successful, otherwise returns the error that occurred (output file may be partially written to)
 */
uint8_t parseInstructionsFile(FILE* inputFile, FILE* outputFile, SymbolsList* symbols, bool printErrors)
{
    char* lineBuffer = (char*)calloc(64, sizeof(char));
    uint8_t* instruction = (uint8_t*)calloc(1, sizeof(uint8_t));
    uint32_t line = 1;
    uint16_t currentOffset = 0;
    while (fgets(lineBuffer, 64, inputFile) != NULL) {
        uint8_t status = parseInstruction(instruction, lineBuffer, currentOffset, symbols);
        if (status == 0) {
            fputc(*instruction, outputFile);
            currentOffset++;
        } else if (status != STATUS_LINE_NOT_INSTRUCTION) {
            if (printErrors) {
                if (status == ERROR_UNKNOWN_REGISTER) {
                    fprintf(stderr, "Error: Invalid register name on line %d.\n", line);
                } else if (status == ERROR_VALUE_OUT_OF_RANGE) {
                    fprintf(stderr, "Error: Value out of range on line %d.\n", line);
                } else if (status == ERROR_INVALID_NUMBER) {
                    fprintf(stderr, "Error: Invalid number on line %d.\n", line);
                } else if (status == ERROR_INVALID_BINARY_STRING_CHARACTER) {
                    fprintf(stderr, "Error: Invalid binary string character on line %d.\n", line);
                } else if (status == ERROR_INVALID_BINARY_STRING_LENGTH) {
                    fprintf(stderr, "Error: Invalid binary string length on line %d.\n", line);
                } else if (status == ERROR_UNKNOWN_MNEMONIC) {
                    fprintf(stderr, "Error: Unknown mnemonic on line %d.\n", line);
                } else if (status == ERROR_MISSING_INSTRUCTION_PARAMETER) {
                    fprintf(stderr, "Error: Malformed instruction on line %d.\n", line);
                } else if (status == ERROR_TOO_MANY_TOKENS) {
                    fprintf(stderr, "Error: Too many tokens on line %d.\n", line);
                } else {
                    fprintf(stderr, "Error: Unknown error on line %d.\n", line);
                }
            }
            free(lineBuffer);
            free(instruction);
            return status;
        }
        *instruction = 0;
        line++;
    }
    free(lineBuffer);
    free(instruction);
    return 0;
}
