#include "alu.h"

void alu_compute(const DECODED_INSTRUCTION* instruction, CPU_STATE* cpuState)
{
    // Pobranie operandów i wewnątrz ALU wykonujemy operacjach na liczbach bez znaku
    uint32_t opA = cpuState->x[instruction->rs1];
    uint32_t opB = cpuState->x[instruction->rs2];
    uint32_t imm  = (uint32_t)instruction->imm;
    uint32_t result = 0;

    switch (instruction->op)
    {
        // Format R
        case OP_ADD:
            result = opA + opB;
            break;
        case OP_SUB:
            result = opA - opB;
            break;
        case OP_XOR:
            result = opA ^ opB;
            break;
        case OP_OR:
            result = opA | opB;
            break;
        case OP_AND:
            result = opA & opB;
            break;
            
        case OP_SLL:
            // W RV32I przesunięcia uwzględniają tylko 5 najmłodszych bitów operandu drugiego
            result = opA << (opB & 0x1FU);
            break;
        case OP_SRL:
            result = opA >> (opB & 0x1FU);
            break;
        case OP_SRA:
            result = (uint32_t)((int32_t)opA >> (opB & 0x1FU));
            break;
        case OP_SLT:
            result = ((int32_t)opA < (int32_t)opB) ? 1U : 0U;
            break;
        case OP_SLTU:
            result = (opA < opB) ? 1U : 0U;
            break;

        // Format I
        case OP_ADDI:
            result = opA + imm;
            break;
        case OP_XORI:
            result = opA ^ imm;
            break;
        case OP_ORI:
            result = opA | imm;
            break;
        case OP_ANDI:
            result = opA & imm;
            break;
            
        case OP_SLLI:
            result = opA << (imm & 0x1FU);
            break;
        case OP_SRLI:
            result = opA >> (imm & 0x1FU);
            break;
        case OP_SRAI:
            result = (uint32_t)((int32_t)opA >> (imm & 0x1FU));
            break;
            
        case OP_SLTI:
            result = ((int32_t)opA < (int32_t)instruction->imm) ? 1U : 0U;
            break;
        case OP_SLTIU:
            result = (opA < imm) ? 1U : 0U;
            break;

        default:
            return; // Instrukcje pamięci lub skoków nie są tutaj obsługiwane
    }

    // Rejestr x0 musi zawsze pozostać równy 0
    if (instruction->rd != 0) cpuState->x[instruction->rd] = result;
}