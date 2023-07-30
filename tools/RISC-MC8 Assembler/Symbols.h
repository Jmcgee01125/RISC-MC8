#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct _Symbol {
    char* symbol;
    uint16_t value;
} Symbol;

typedef struct _SymbolsList {
    Symbol* symbols;
    uint16_t length;
} SymbolsList;

void freeSymbol(Symbol* symbol);
void freeSymbolsList(SymbolsList** symbolsList);

/**
 * @brief Allocate and add a symbol to the given list
 *
 * @param list List to add to
 * @param symbol The symbol to add
 * @param value The value of the symbol to add
 * @return 0 if successful, otherwise an error (such as duplicates) - if error, list is unchanged
 */
uint8_t addSymbolToList(SymbolsList* list, char* symbol, uint32_t value);

/**
 * @brief Copy src[startPos:endPos] to dest (starting from position 0 in dest)
 *
 * @param dest Place to copy to
 * @param src Place to copy from
 * @param startPos Position, in src, to start copying from
 * @param endPos Position, in src, to stop copying (exclusive)
 */
void copyStringAsLower(char* dest, char* src, uint16_t startPos, uint16_t endPos);

/**
 * @brief Attempt to extract a label from a line
 *
 * @param list The symbols list to add the label to
 * @param line The line to extract from
 * @param value The value that would be stored in this label, if there is one
 * @return 0 if a symbol was extracted or there was no symbol, otherwise an error code
 */
uint8_t attemptSymbolExtraction(SymbolsList* list, char* line, uint16_t value);

/**
 * @brief Parse through the provided input file and make a list of symbols seen along with their line value
 *
 * @param inputFile File to parse
 * @param printErrors Print errors if any occur
 * @return List of symbols (may be empty), or NULL if an error occurred
 */
SymbolsList* extractSymbols(FILE* inputFile, bool printErrors);

#endif
