#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>

typedef enum {
    OP_UNKNOWN = 0,
    // Format R
    OP_ADD, OP_SUB, OP_XOR, OP_OR, OP_AND,
    OP_SLL, OP_SRL, OP_SRA, OP_SLT, OP_SLTU,
    // Format I
    OP_ADDI, OP_XORI, OP_ORI, OP_ANDI,
    OP_SLLI, OP_SRLI, OP_SRAI, OP_SLTI, OP_SLTIU,
    OP_LB, OP_LH, OP_LW, OP_LBU, OP_LHU, OP_JALR,
    // Format S
    OP_SB, OP_SH, OP_SW,
    // Format B
    OP_BEQ, OP_BNE, OP_BLT, OP_BGE, OP_BLTU, OP_BGEU,
    // Format U & J
    OP_LUI, OP_AUIPC, OP_JAL
} INSTRUCTION_TYPE;

typedef struct {
    INSTRUCTION_TYPE op;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;
    int32_t imm;
} DECODED_INSTRUCTION;

void decoder_init(DECODED_INSTRUCTION* decodedInstruction);
void decoder_decode(DECODED_INSTRUCTION* decodedInstruction, uint32_t instruction);

#endif