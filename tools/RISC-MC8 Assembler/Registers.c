#include "Registers.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Get a register with the given name
 *
 * @param name The register to get information about
 * @return The definition of the register, or NULL if the name was invalid
 */
const RegisterDefinition* getRegisterDefinition(const char* const name)
{
    for (uint8_t i = 0; i < NUM_REGISTERS; i++) {
        if (strncmp(name, RegisterDefinitionLUT[i].name, 4) == 0) {
            return &RegisterDefinitionLUT[i];
        } else if (strncmp(name, RegisterDefinitionLUT[i].altName, 5) == 0) {
            return &RegisterDefinitionLUT[i];
        }
    }
    return NULL;
}
