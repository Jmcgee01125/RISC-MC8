#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <inttypes.h>
#include <stdbool.h>

#include "Symbols.h"

/**
 * @brief Parse the given instruction
 *
 * @param instructionDest The place to write the finished instruction
 * @param instructionLine The source instruction line, case insensitive
 * @param currentOffset The offset of the instruction being parsed
 * @param symbols List of symbols to use for translating
 * @return 0 if successful, otherwise returns the error that occurred
 */
uint8_t parseInstruction(uint8_t* const instructionDest, const char* const instructionLine, uint16_t currentOffset, SymbolsList* symbols);

/**
 * @brief Parse an assembly file and output the results to outputFile (overwrite)
 *
 * @param inputFile File to read instructions from
 * @param outputFile File to write assembled code to
 * @param symbols List of symbols to use for translating
 * @param printErrors If true, print errors, noting the line that they occurred on
 * @return 0 if successful, otherwise returns the error that occurred (output file may be partially written to)
 */
uint8_t parseInstructionsFile(FILE* inputFile, FILE* outputFile, SymbolsList* symbols, bool printErrors);

#endif
