#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <inttypes.h>

#include "Symbols.h"

/**
 * @brief Insert the given register into (according to token) into the instruction, using 000 if token is malformed
 *
 * @param instruction The 8-bit instruction to load the register into
 * @param offset Current offset of this instruction
 * @param token The token name of the register, either "000"-style, "r1"-style, or "ireg"
 * @param symbols List of symbols
 * @return error code, 0 if successful
 */
uint8_t loadReg(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols);

/**
 * @brief Insert the given 4-bit unsigned immediate into the instruction
 *
 * @param instruction The 8-bit instruction to load the immediate into
 * @param offset Current offset of this instruction
 * @param token The token to load, either "0b1010"-style or an integer in the range 0-15
 * @param symbols List of symbols
 * @return error code, 0 if successful
 */
uint8_t load4BitImm(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols);

/**
 * @brief Insert the given 7-bit signed immediate into the instruction
 *
 * @param instruction The 8-bit instruction to load the immediate into
 * @param offset Current offset of this instruction
 * @param token The token to load, either "0b1010101"-style or an integer in the range -64 to 63
 * @param symbols List of symbols, if token matches a symbol then its value is used instead
 * @return error code, 0 if successful
 */
uint8_t load7BitSImm(uint8_t* const instruction, uint16_t offset, const char* const token, SymbolsList* symbols);

#define NUM_INSTRUCTIONS 15

static const struct _InstructionLoaderDefinition {
    char* mnemonic;
    uint8_t instructionBase;
    uint8_t (*tokenLoader)(uint8_t* const, uint16_t, const char* const, SymbolsList*);
} InstructionLoaderLUT[] = {
    {"andi", 0b00000000, &loadReg},
    {"nand", 0b00001000, &loadReg},
    {"addi", 0b00010000, &loadReg},
    {"subi", 0b00011000, &loadReg},
    {"iori", 0b00100000, &loadReg},
    {"xori", 0b00101000, &loadReg},
    {"dupi", 0b00110000, &loadReg},
    {"dupr", 0b00111000, &loadReg},
    {"load", 0b01000000, &loadReg},
    {"stor", 0b01001000, &loadReg},
    {"shif", 0b01010000, &loadReg},
    {"skip", 0b01011000, &loadReg},
    {"stlo", 0b01100000, &load4BitImm},
    {"sthi", 0b01110000, &load4BitImm},
    {"jump", 0b10000000, &load7BitSImm},
};
typedef struct _InstructionLoaderDefinition InstructionLoaderDefinition;

/**
 * @brief Get the instruction loader information for a given mnemonic
 *
 * @param mnemonic The instruction mnemonic to get
 * @return The corresponding InstructionLoaderDefinition, or NULL if the mnemonic was invalid
 */
const InstructionLoaderDefinition* getInstructionLoaderDefinition(const char* const mnemonic);

#endif
