/*
    - config.h
    - Główny plik konfiguracyjny symulatora, definiuje architektoniczny stan procesora w każdym cyklu
    - oraz struktury danych wykorzystywane do dekodowania instrukcji
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// Stan architektoniczny procesora RV32I (odczytywany co każdy cykl zegara)
typedef struct {
    uint32_t x[32]; // 32 rejestry ogólnego przeznaczenia (x0-x31)
    uint32_t pc;    // Licznik rozkazów, czyli Program Counter
} CPU_STATE;

// Wszystkie obsługiwane typy instrukcji w formie zdekodowanej
typedef enum {
    OP_UNKNOWN = 0,
    // Format R (operacje rejestr-rejestr)
    OP_ADD, OP_SUB, OP_XOR, OP_OR, OP_AND,
    OP_SLL, OP_SRL, OP_SRA, OP_SLT, OP_SLTU,
    // Format I (operacje rejestr-natychmiastowa)
    OP_ADDI, OP_XORI, OP_ORI, OP_ANDI,
    OP_SLLI, OP_SRLI, OP_SRAI, OP_SLTI, OP_SLTIU,
    OP_LB, OP_LH, OP_LW, OP_LBU, OP_LHU, OP_JALR,
    // Format S (operacje zapisu do pamięci)
    OP_SB, OP_SH, OP_SW,
    // Format B (skoki warunkowe)
    OP_BEQ, OP_BNE, OP_BLT, OP_BGE, OP_BLTU, OP_BGEU,
    // Format U & J (skoki bezwarunkowe i operacje natychmiastowych stałych górnych)
    OP_LUI, OP_AUIPC, OP_JAL
} INSTRUCTION_TYPE;

// Struktura zdekodowanej instrukcji
typedef struct {
    INSTRUCTION_TYPE op; // Zidentyfikowana operacja
    uint8_t rs1;         // Indeks rejestru źródłowego 1 (x0-x31)
    uint8_t rs2;         // Indeks rejestru źródłowego 2 (x0-x31)
    uint8_t rd;          // Indeks rejestru docelowego (x0-x31)
    int32_t imm;         // 32-bitowy stała natyhmiastowa z rozszerzonym znakiem
} DECODED_INSTRUCTION;

#endif