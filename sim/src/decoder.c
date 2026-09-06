#include "decoder.h"

// Stałe kodów operacji zgodne z RV32I
#define OPCODE_R_TYPE    0x33
#define OPCODE_I_IMM     0x13
#define OPCODE_I_LOAD    0x03
#define OPCODE_I_JALR    0x67
#define OPCODE_S_TYPE    0x23
#define OPCODE_B_TYPE    0x63
#define OPCODE_U_LUI     0x37
#define OPCODE_U_AUIPC   0x17
#define OPCODE_J_JAL     0x6F

void decoder_init(DECODED_INSTRUCTION* decodedInstruction)
{
    decodedInstruction->rs1 = 0;
    decodedInstruction->rs2 = 0;
    decodedInstruction->rd  = 0;
    decodedInstruction->imm = 0;
    decodedInstruction->op  = OP_UNKNOWN;
}

void decoder_decode(DECODED_INSTRUCTION* decodedInstruction, uint32_t instruction)
{
    decoder_init(decodedInstruction);

    uint8_t opcode = instruction & 0x7F;
    uint8_t funct3 = (instruction >> 12) & 0x07;
    uint8_t funct7 = (instruction >> 25) & 0x7F;

    switch (opcode)
    {
        case OPCODE_R_TYPE:
            decodedInstruction->rs1 = (instruction >> 15) & 0x1F;
            decodedInstruction->rs2 = (instruction >> 20) & 0x1F;
            decodedInstruction->rd  = (instruction >> 7)  & 0x1F;

            switch (funct3)
            {
                case 0x00:
                    if (funct7 == 0x00)      decodedInstruction->op = OP_ADD;
                    else if (funct7 == 0x20) decodedInstruction->op = OP_SUB;
                    break;
                case 0x01: decodedInstruction->op = OP_SLL;  break;
                case 0x02: decodedInstruction->op = OP_SLT;  break;
                case 0x03: decodedInstruction->op = OP_SLTU; break;
                case 0x04: decodedInstruction->op = OP_XOR;  break;
                case 0x05:
                    if (funct7 == 0x00)      decodedInstruction->op = OP_SRL;
                    else if (funct7 == 0x20) decodedInstruction->op = OP_SRA;
                    break;
                case 0x06: decodedInstruction->op = OP_OR;   break;
                case 0x07: decodedInstruction->op = OP_AND;  break;
                default:   decodedInstruction->op = OP_UNKNOWN; break;
            }
            break;

        case OPCODE_I_IMM:
        case OPCODE_I_LOAD:
        case OPCODE_I_JALR:
            decodedInstruction->rs1 = (instruction >> 15) & 0x1F;
            decodedInstruction->rd  = (instruction >> 7)  & 0x1F;
            
            // Format I: imm[11:0] = inst[31:20] (automatycznie rozszerza znak do 32 bitów)
            decodedInstruction->imm = (int32_t)(instruction) >> 20;

            if (opcode == OPCODE_I_IMM)
            {
                switch (funct3)
                {
                    case 0x00: decodedInstruction->op = OP_ADDI;  break;
                    case 0x01: decodedInstruction->op = OP_SLLI;  break;
                    case 0x02: decodedInstruction->op = OP_SLTI;  break;
                    case 0x03: decodedInstruction->op = OP_SLTIU; break;
                    case 0x04: decodedInstruction->op = OP_XORI;  break;
                    case 0x05:
                        if (funct7 == 0x00)      decodedInstruction->op = OP_SRLI;
                        else if (funct7 == 0x20) decodedInstruction->op = OP_SRAI;
                        break;
                    case 0x06: decodedInstruction->op = OP_ORI;   break;
                    case 0x07: decodedInstruction->op = OP_ANDI;  break;
                    default:   decodedInstruction->op = OP_UNKNOWN; break;
                }
            }
            else if (opcode == OPCODE_I_LOAD)
            {
                switch (funct3)
                {
                    case 0x00: decodedInstruction->op = OP_LB;  break;
                    case 0x01: decodedInstruction->op = OP_LH;  break;
                    case 0x02: decodedInstruction->op = OP_LW;  break;
                    case 0x04: decodedInstruction->op = OP_LBU; break;
                    case 0x05: decodedInstruction->op = OP_LHU; break;
                    default:   decodedInstruction->op = OP_UNKNOWN; break;
                }
            }
            else if (opcode == OPCODE_I_JALR)
            {
                if (funct3 == 0x00) decodedInstruction->op = OP_JALR;
            }
            break;

        case OPCODE_S_TYPE:
            decodedInstruction->rs1 = (instruction >> 15) & 0x1F;
            decodedInstruction->rs2 = (instruction >> 20) & 0x1F;

            // Format S: imm[11:5] = inst[31:25], imm[4:0] = inst[11:7]
            decodedInstruction->imm = ((instruction >> 7)  & 0x0000001F)
                                    | ((instruction >> 20) & 0x00000FE0);
            
            // Rozszerzenie znaku z 12 bitów
            if (decodedInstruction->imm & 0x800) {
                decodedInstruction->imm |= 0xFFFFF000;
            }

            switch (funct3)
            {
                case 0x00: decodedInstruction->op = OP_SB; break;
                case 0x01: decodedInstruction->op = OP_SH; break;
                case 0x02: decodedInstruction->op = OP_SW; break;
                default:   decodedInstruction->op = OP_UNKNOWN; break;
            }
            break;

        case OPCODE_B_TYPE:
            decodedInstruction->rs1 = (instruction >> 15) & 0x1F;
            decodedInstruction->rs2 = (instruction >> 20) & 0x1F;

            // Format B: imm[12|10:5] = inst[31:25], imm[4:1|11] = inst[11:7], imm[0] = 0
            decodedInstruction->imm = ((instruction >> 7)  & 0x0000001E)  // imm[4:1]
                                    | ((instruction >> 20) & 0x000007E0)  // imm[10:5]
                                    | ((instruction << 4)  & 0x00000800)  // imm[11]
                                    | ((instruction >> 19) & 0x00001000); // imm[12]

            // Rozszerzenie znaku z 13 bitów
            if (decodedInstruction->imm & 0x1000) {
                decodedInstruction->imm |= 0xFFFFE000;
            }

            switch (funct3)
            {
                case 0x00: decodedInstruction->op = OP_BEQ;  break;
                case 0x01: decodedInstruction->op = OP_BNE;  break;
                case 0x04: decodedInstruction->op = OP_BLT;  break;
                case 0x05: decodedInstruction->op = OP_BGE;  break;
                case 0x06: decodedInstruction->op = OP_BLTU; break;
                case 0x07: decodedInstruction->op = OP_BGEU; break;
                default:   decodedInstruction->op = OP_UNKNOWN; break;
            }
            break;

        case OPCODE_U_LUI:
        case OPCODE_U_AUIPC:
            decodedInstruction->rd  = (instruction >> 7) & 0x1F;
            decodedInstruction->imm = (int32_t)(instruction & 0xFFFFF000);

            if (opcode == OPCODE_U_LUI) decodedInstruction->op = OP_LUI;
            else                        decodedInstruction->op = OP_AUIPC;
            break;

        case OPCODE_J_JAL:
            decodedInstruction->rd = (instruction >> 7) & 0x1F;

            // Format J: imm[20|10:1|11|19:12] = inst[31:12], imm[0] = 0
            decodedInstruction->imm = ((instruction >> 20) & 0x000007FE)  // imm[10:1]
                                    | ((instruction >> 9)  & 0x00000800)  // imm[11]
                                    | (instruction         & 0x000FF000)  // imm[19:12]
                                    | ((instruction >> 11) & 0x00100000); // imm[20]

            // Rozszerzenie znaku z 21 bitów
            if (decodedInstruction->imm & 0x100000) {
                decodedInstruction->imm |= 0xFFF00000;
            }

            decodedInstruction->op = OP_JAL;
            break;

        default:
            decodedInstruction->op = OP_UNKNOWN;
            break;
    }
}