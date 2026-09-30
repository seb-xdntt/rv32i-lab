#include "alu.h"

void alu_compute(DECODED_INSTRUCTION instruction, CPU_STATE* cpuState)
{
    uint32_t op_a = cpuState->x[instruction.rs1];
    uint32_t op_b = cpuState->x[instruction.rs2];
    uint32_t imm  = (uint32_t)instruction.imm;
    uint32_t result = 0;

    switch (instruction.op)
    {
        // Format R
        case OP_ADD:
            result = op_a + op_b;
            break;
        case OP_SUB:
            result = op_a - op_b;
            break;
        case OP_XOR:
            result = op_a ^ op_b;
            break;
        case OP_OR:
            result = op_a | op_b;
            break;
        case OP_AND:
            result = op_a & op_b;
            break;
        case OP_SLL:
            result = op_a << (op_b & 0x1FU);
            break;
        case OP_SRL:
            result = op_a >> (op_b & 0x1FU);
            break;
        case OP_SRA:
            result = (uint32_t)((int32_t)op_a >> (op_b & 0x1FU));
            break;
        case OP_SLT:
            result = ((int32_t)op_a < (int32_t)op_b) ? 1U : 0;
            break;
        case OP_SLTU:
            result = (op_a < op_b) ? 1U : 0;
            break;

        // Format I
        case OP_ADDI:
            result = op_a + imm;
            break;
        case OP_XORI:
            result = op_a ^ imm;
            break;
        case OP_ORI:
            result = op_a | imm;
            break;
        case OP_ANDI:
            result = op_a & imm;
            break;
        case OP_SLLI:
            result = op_a << (imm & 0x1FU);
            break;
        case OP_SRLI:
            result = op_a >> (imm & 0x1FU);
            break;
        case OP_SRAI:
            result = (uint32_t)((int32_t)op_a >> (imm & 0x1FU));
            break;
        case OP_SLTI:
            result = ((int32_t)op_a < (int32_t)instruction.imm) ? 1U : 0;
            break;
        case OP_SLTIU:
            result = (op_a < imm) ? 1U : 0;
            break;

        default:
            return;
    }

    if (instruction.rd != 0) cpuState->x[instruction.rd] = result;
}