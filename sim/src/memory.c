#include "memory.h"

void memory_store(uint8_t* memory, uint32_t address, uint32_t rs2Value, INSTRUCTION_TYPE instructionType)
{
    // Ignorujemy próby dostępu poza przydzielony obszar
    if (address < MEM_BASE) return;
    
    // Przeliczenie adresu architektonicznego na fizyczny indeks w tablicy
    uint32_t offset = address - MEM_BASE;

    // Architektura RISC-V jest w formacie Little endian.
    if (instructionType == OP_SB)
    {
        if (offset >= MEM_SIZE) return;
        memory[offset] = (uint8_t)(rs2Value & 0xFFU);
    }
    else if (instructionType == OP_SH)
    {
        if (offset + 1 >= MEM_SIZE) return;
        memory[offset]     = (uint8_t)(rs2Value & 0xFFU);             // LSB
        memory[offset + 1] = (uint8_t)((rs2Value >> 8) & 0xFFU);      // MSB
    }
    else if (instructionType == OP_SW)
    {
        if (offset + 3 >= MEM_SIZE) return;
        memory[offset]     = (uint8_t)(rs2Value & 0xFFU);             // LSB
        memory[offset + 1] = (uint8_t)((rs2Value >> 8) & 0xFFU);
        memory[offset + 2] = (uint8_t)((rs2Value >> 16) & 0xFFU);
        memory[offset + 3] = (uint8_t)((rs2Value >> 24) & 0xFFU);     // MSB
    }
}

void memory_load(uint8_t* memory, CPU_STATE* cpuState, uint32_t address, uint8_t rd, INSTRUCTION_TYPE instructionType)
{
    if (address < MEM_BASE) return;
    uint32_t offset = address - MEM_BASE;
    uint32_t value = 0;

    if (instructionType == OP_LB)
    {
        if (offset >= MEM_SIZE) return;
        value = (uint32_t)(int32_t)(int8_t)memory[offset];
    }
    else if (instructionType == OP_LBU)
    {
        if (offset >= MEM_SIZE) return;
        value = (uint32_t)memory[offset];
    }
    else if (instructionType == OP_LH)
    {
        if (offset + 1 >= MEM_SIZE) return;
        uint16_t rawHalfWord = (uint16_t)memory[offset] | ((uint16_t)memory[offset + 1] << 8);
        value = (uint32_t)(int32_t)(int16_t)rawHalfWord; 
    }
    else if (instructionType == OP_LHU)
    {
        if (offset + 1 >= MEM_SIZE) return;
        value = (uint32_t)memory[offset] | ((uint32_t)memory[offset + 1] << 8); 
    }
    else if (instructionType == OP_LW)
    {
        if (offset + 3 >= MEM_SIZE) return;
        value = (uint32_t)memory[offset]
              | ((uint32_t)memory[offset + 1] << 8)
              | ((uint32_t)memory[offset + 2] << 16)
              | ((uint32_t)memory[offset + 3] << 24);
    }
    else
    {
        return;
    }

    // Nie można nadpisać rejestru x0 (x0 jest zawsze równe 0)
    if (rd != 0) 
    {
        cpuState->x[rd] = value;
    }
}