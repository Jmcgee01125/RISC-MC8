#include "Instructions.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "Registers.h"
#include "StatusCodes.h"
#include "Symbols.h"

/**
 * @param str String to test
 * @return true if the given string is entirely composed of digits, false otherwise
 */
bool isInteger(const char* const str)
{
    uint32_t length = strlen(str);
    uint32_t i = 0;
    // first character might be - for negative number
    if (*str == '-') {
        i++;
    }
    // rest should be digits
    while (i < length) {
        if (!isdigit(*(str + i))) {
            return false;
        }
        i++;
    }
    return true;
}

/**
 * @brief Insert the given register into (according to token) into the instruction, using 000 if token is malformed
 *
 * @param instruction The 8-bit instruction to load the register into
 * @param offset Current offset of this instruction
 * @param token The token name of the register, either "000"-style, "r1"-style, or "ireg"
 * @param symbols List of symbols
 * @return error code, 0 if successful
 */
uint8_t loadReg(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols)
{
    const RegisterDefinition* rDef = getRegisterDefinition(token);
    if (rDef == NULL) {
        return ERROR_UNKNOWN_REGISTER;
    }
    *instruction |= rDef->value;
    return 0;
}

/**
 * @brief Insert the given 4-bit unsigned immediate into the instruction
 *
 * @param instruction The 8-bit instruction to load the immediate into
 * @param offset Current offset of this instruction
 * @param token The token to load, either "0b1010"-style or an integer in the range 0-15
 * @param symbols List of symbols
 * @return error code, 0 if successful
 */
uint8_t load4BitImm(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols)
{
    int8_t insertValue = 0;
    // loading 0b1010 value
    if (*token == '0' && *(token + 1) == 'b') {
        if (strlen(token) != 6) {
            return ERROR_INVALID_BINARY_STRING_LENGTH;
        }
        for (uint8_t i = 0; i < 4; i++) {
            char nextVal = *(token + 2 + i);
            // if invalid character
            if (nextVal != '0' && nextVal != '1') {
                return ERROR_INVALID_BINARY_STRING_CHARACTER;
            }
            // get bit 1 or 0 and shift it into where it belongs
            insertValue |= (nextVal - 48) << (3 - i);
        }
    }
    // loading direct "int"
    else {
        if (!isInteger(token)) {
            return ERROR_INVALID_NUMBER;
        }
        insertValue = atoi(token);
        if (insertValue < 0 || insertValue > 15) {
            return ERROR_VALUE_OUT_OF_RANGE;
        }
        insertValue &= 0b1111;  // drop high bits
    }
    *instruction |= insertValue;
    return 0;
}

/**
 * @brief Insert the given 7-bit signed immediate into the instruction
 *
 * @param instruction The 8-bit instruction to load the immediate into
 * @param offset Current offset of this instruction
 * @param token The token to load, either "0b1010101"-style or an integer in the range -64 to 63
 * @param symbols List of symbols, if token matches a symbol then its value is used instead
 * @return error code, 0 if successful
 */
uint8_t load7BitSImm(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols)
{
    int8_t insertValue = 0;
    // try to load a symbol first
    for (uint16_t i = 0; i < symbols->length; i++) {
        if (strcmp(token, (symbols->symbols + i)->symbol) == 0) {
            insertValue = (symbols->symbols + i)->value - offset;
            if (insertValue < -64 || insertValue > 63) {
                return ERROR_VALUE_OUT_OF_RANGE;
            }
            insertValue &= 0b1111111;  // drop high bits
            *instruction |= insertValue;
            return 0;
        }
    }
    // loading 0b101010 value
    if (*token == '0' && *(token + 1) == 'b') {
        if (strlen(token) != 9) {
            return ERROR_INVALID_BINARY_STRING_LENGTH;
        }
        for (uint8_t i = 0; i < 7; i++) {
            char nextVal = *(token + 2 + i);
            // if invalid character
            if (nextVal != '0' && nextVal != '1') {
                return ERROR_INVALID_BINARY_STRING_CHARACTER;
            }
            // get bit 1 or 0 and shift it into where it belongs
            insertValue |= (nextVal - 48) << (6 - i);
        }
    }
    // loading direct "int"
    else {
        if (!isInteger(token)) {
            return ERROR_INVALID_NUMBER;
        }
        insertValue = atoi(token);
        if (insertValue < -64 || insertValue > 63) {
            return ERROR_VALUE_OUT_OF_RANGE;
        }
        insertValue &= 0b1111111;  // drop high bits
    }
    *instruction |= insertValue;
    return 0;
}

/**
 * @brief Get the instruction loader information for a given mnemonic
 *
 * @param mnemonic The instruction mnemonic to get, not NULL
 * @return The corresponding InstructionLoaderDefinition, or NULL if the mnemonic was invalid
 */
const InstructionLoaderDefinition* getInstructionLoaderDefinition(const char* const mnemonic)
{
    for (uint8_t i = 0; i < NUM_INSTRUCTIONS; i++) {
        if (strncmp(mnemonic, InstructionLoaderLUT[i].mnemonic, 5) == 0) {
            return &InstructionLoaderLUT[i];
        }
    }
    return NULL;
}
