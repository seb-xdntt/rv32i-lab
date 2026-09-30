#include "decoder.h"
#include <stdio.h>
#include <assert.h>

void test_sub(void)
{
    uint32_t instruction = 0x40000033;
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, instruction);

    assert(d.op == OP_SUB);
    assert(d.rs1 == 0);
    assert(d.rs2 == 0);
    assert(d.rd  == 0);
    
    printf("[PASS] SUB decoded successfully\n");
}

void test_slt(void)
{
    uint32_t instruction = 0x01FFAFB3;
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, instruction);

    assert(d.op == OP_SLT);
    assert(d.rs1 == 31);
    assert(d.rs2 == 31);
    assert(d.rd  == 31);
    
    printf("[PASS] SLT decoded successfully\n");
}

void test_addi(void)
{
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, 0xFFF08093);
    assert(d.op == OP_ADDI);
    assert(d.rs1 == 1);
    assert(d.imm == -1);
    assert(d.rd  == 1);

    decoder_decode(&d, 0x7FF00F93);
    assert(d.op == OP_ADDI);
    assert(d.rs1 == 0);
    assert(d.imm == 2047);
    assert(d.rd  == 31);

    decoder_decode(&d, 0x80000113);
    assert(d.op == OP_ADDI);
    assert(d.rs1 == 0);
    assert(d.imm == -2048);
    assert(d.rd  == 2);

    printf("[PASS] ADDI decoded successfully\n");
}

void test_sw(void)
{
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, 0xFE532FA3);
    assert(d.op == OP_SW);
    assert(d.rs1 == 6);
    assert(d.rs2 == 5);
    assert(d.imm == -1);

    decoder_decode(&d, 0x000120A3);
    assert(d.op == OP_SW);
    assert(d.rs1 == 2);
    assert(d.rs2 == 0);
    assert(d.imm == 1);

    printf("[PASS] SW decoded successfully\n");
}

void test_beq(void)
{
    uint32_t instruction = 0xFE208FE3;
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, instruction);

    assert(d.op == OP_BEQ);
    assert(d.rs1 == 1);
    assert(d.rs2 == 2);
    assert(d.imm == -2);
    
    printf("[PASS] BEQ decoded successfully\n");
}

void test_bne(void)
{
    uint32_t instruction = 0x000010E3;
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, instruction);

    assert(d.op == OP_BNE);
    assert(d.rs1 == 0);
    assert(d.rs2 == 0);
    assert(d.imm == 2048);
    
    printf("[PASS] BNE decoded successfully\n");
}

void test_lui(void)
{
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, 0xFFFFF537);
    assert(d.op == OP_LUI);
    assert(d.rd == 10);
    assert(d.imm == -4096);

    decoder_decode(&d, 0x000010B7);
    assert(d.op == OP_LUI);
    assert(d.rd == 1);
    assert(d.imm == 4096);

    printf("[PASS] LUI decoded successfully\n");
}

void test_jal(void)
{
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, 0xFFFFF06F);
    assert(d.op == OP_JAL);
    assert(d.rd == 0);
    assert(d.imm == -2);

    decoder_decode(&d, 0x001000EF);
    assert(d.op == OP_JAL);
    assert(d.rd == 1);
    assert(d.imm == 2048);

    printf("[PASS] JAL decoded successfully\n");
}

void test_unknown(void)
{
    DECODED_INSTRUCTION d;
    
    decoder_decode(&d, 0x00000000);
    assert(d.op == OP_UNKNOWN);

    decoder_decode(&d, 0xFFFFFFFF);
    assert(d.op == OP_UNKNOWN);

    printf("[PASS] UNKNOWN decoded successfully\n");
}

int main(void)
{
    test_addi();
    test_beq();
    test_bne();
    test_unknown();
    test_jal();
    test_lui();
    test_slt();
    test_sub();
    test_sw();

    printf("All decoder tests passed successfully\n");
    return 0;
}