#include "Symbols.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StatusCodes.h"

void freeSymbolsList(SymbolsList** list)
{
    for (uint32_t i = 0; i < (*list)->length; i++) {
        freeSymbol((*list)->symbols + i);
    }
    free((*list)->symbols);
    free(*list);
    list = NULL;
}

void freeSymbol(Symbol* sym)
{
    free(sym->symbol);
}

/**
 * @brief Allocate and add a symbol to the given list
 *
 * @param list List to add to
 * @param symbol The symbol to add
 * @param value The value of the symbol to add
 * @return 0 if successful, otherwise an error (such as duplicates) - if error, list is unchanged
 */
uint8_t addSymbolToList(SymbolsList* list, char* symbol, uint32_t value)
{
    for (uint16_t i = 0; i < list->length; i++) {
        if (strcmp(symbol, (list->symbols + i)->symbol) == 0) {
            return ERROR_DUPLICATE_LABEL;
        }
    }
    list->length++;
    list->symbols = (Symbol*)realloc(list->symbols, list->length * sizeof(Symbol));
    (list->symbols + list->length - 1)->symbol = symbol;
    (list->symbols + list->length - 1)->value = value;
    return 0;
}

/**
 * @brief Return true if a line is empty or contains only a comment, false otherwise
 *
 * @param line Line to check
 * @return True if condition satisfied, false otherwise
 */
bool isLineEmptyOrComment(char* line)
{
    char* lineCopy = (char*)calloc(64, sizeof(char));
    strncpy(lineCopy, line, 64);
    char* firstToken = strtok(lineCopy, " \t\r\n");
    bool result = firstToken == NULL || strlen(firstToken) <= 1 || *firstToken == '#';
    free(lineCopy);
    return result;
}

/**
 * @brief Copy src[startPos:endPos] to dest (starting from position 0 in dest)
 *
 * @param dest Place to copy to
 * @param src Place to copy from
 * @param startPos Position, in src, to start copying from
 * @param endPos Position, in src, to stop copying (exclusive)
 */
void copyStringAsLower(char* dest, char* src, uint16_t startPos, uint16_t endPos)
{
    for (uint16_t i = 0; i < endPos - startPos; i++) {
        *(dest + i) = tolower(*(src + i + startPos));
    }
}

/**
 * @brief Attempt to extract a label from a line
 *
 * @param list The symbols list to add the label to
 * @param line The line to extract from
 * @param value The value that would be stored in this label, if there is one
 * @return 0 if a symbol was extracted, STATUS_LINE_CONTAINED_INSTRUCTION if it was an instruction, otherwise an error code
 */
uint8_t attemptSymbolExtraction(SymbolsList* list, char* line, uint16_t value)
{
    uint16_t lineLength = strlen(line);
    uint16_t startPos = 0;
    while (startPos < lineLength && isspace(*(line + startPos))) {  // get to potential start of label
        startPos++;
    }
    for (uint16_t i = startPos; i < lineLength; i++) {
        char c = *(line + i);
        if (c == ':') {  // found label ending point
            // make sure it's not followed by an instruction - whitespace until # or \0
            if (!isspace(*(line + i + 1)) && i + 1 < lineLength) {
                return ERROR_NO_SPACE_AFTER_LABEL;
            }
            uint16_t verifyPos = i + 2;  // + 2 since we require a space after :
            if (verifyPos < lineLength) {
                while (isspace(*(line + verifyPos))) {
                    verifyPos++;
                }
                if (*(line + verifyPos) != '#' && *(line + verifyPos) != '\0') {
                    return ERROR_INSTRUCTION_FOLLOWS_LABEL;
                }
            }
            // get the label
            char* label = (char*)calloc(i - startPos + 1, sizeof(char));
            copyStringAsLower(label, line, startPos, i);
            // add it to list
            uint8_t addStatus = addSymbolToList(list, label, value);
            if (addStatus != 0) {
                free(label);
                return addStatus;
            }
            return 0;
        } else if (isspace(c) || c == '#') {  // delimited past potential label
            return STATUS_LINE_CONTAINED_INSTRUCTION;
        }
    }
    return STATUS_LINE_CONTAINED_INSTRUCTION;  // no label, ran off end of line
}

/**
 * @brief Parse through the provided input file and make a list of symbols seen along with their line value
 *
 * @param inputFile File to parse
 * @param printErrors Print errors if any occur
 * @return List of symbols (may be empty), or NULL if an error occurred
 */
SymbolsList* extractSymbols(FILE* inputFile, bool printErrors)
{
    SymbolsList* list = (SymbolsList*)calloc(1, sizeof(SymbolsList));
    uint16_t currentOffset = 0;
    uint16_t currentLine = 0;
    char* lineBuffer = (char*)calloc(64, sizeof(char));
    while (!feof(inputFile)) {
        currentLine++;
        fgets(lineBuffer, 64, inputFile);
        if (isLineEmptyOrComment(lineBuffer)) {
            continue;  // ignore blank lines and comments
        }
        uint8_t status = attemptSymbolExtraction(list, lineBuffer, currentOffset);
        if (status != 0 && status != STATUS_LINE_CONTAINED_INSTRUCTION) {
            if (printErrors) {
                if (status == ERROR_DUPLICATE_LABEL) {
                    fprintf(stderr, "Error: Duplicate symbol on line %d.\n", currentLine);
                } else if (status == ERROR_INSTRUCTION_FOLLOWS_LABEL) {
                    fprintf(stderr, "Error: Instruction follows label on line %d (labels must be on their own line).\n", currentLine);
                } else if (status == ERROR_NO_SPACE_AFTER_LABEL) {
                    fprintf(stderr, "Error: Lacking space after label on line %d.\n", currentLine);
                } else {
                    fprintf(stderr, "Error: Unknown error on line %d.\n", currentLine);
                }
            }
            free(lineBuffer);
            freeSymbolsList(&list);
            return NULL;
        } else if (status == STATUS_LINE_CONTAINED_INSTRUCTION) {
            currentOffset++;
        }
    }
    free(lineBuffer);
    return list;
}
