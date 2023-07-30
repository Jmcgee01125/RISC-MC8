#ifndef REGISTERS_H
#define REGISTERS_H

#include <inttypes.h>

#define NUM_REGISTERS 8

static const struct _RegisterDefinition {
    char* name;
    char* altName;
    uint8_t value;
} RegisterDefinitionLUT[] = {
    {"000", "ireg", 0b000},
    {"001", "r1", 0b001},
    {"010", "r2", 0b010},
    {"011", "r3", 0b011},
    {"100", "r4", 0b100},
    {"101", "r5", 0b101},
    {"110", "r6", 0b110},
    {"111", "r7", 0b111},
};
typedef struct _RegisterDefinition RegisterDefinition;

/**
 * @brief Get a register with the given name
 *
 * @param name The register to get information about
 * @return The definition of the register, or NULL if the name was invalid
 */
const RegisterDefinition* getRegisterDefinition(const char* const name);

#endif
