#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"
#include <stdint.h>

// Adres bazowy pamięci RAM dla architektury RISC-V
#define MEM_BASE 0x80000000U
#define MEM_SIZE 128 * 1024 // Domyślny rozmiar pamięci, czyli 128 KiB

void memory_store(uint8_t* memory, uint32_t address, uint32_t rs2Value, INSTRUCTION_TYPE instructionType);
void memory_load(uint8_t* memory, CPU_STATE* cpuState, uint32_t address, uint8_t rd, INSTRUCTION_TYPE instructionType);

#endif