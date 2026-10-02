#ifndef ALU_H
#define ALU_H

#include "config.h"

// Wykonuje obliczenia w ALU i modyfikuje stan rejestrów
void alu_compute(DECODED_INSTRUCTION instruction, CPU_STATE* cpuState);

#endif