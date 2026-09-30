#include "alu.h"
#include "cpu_state.h"
#include <stdio.h>
#include <assert.h>

void test_x0(void)
{
    CPU_STATE cpu;
    cpu_reset(&cpu);

    cpu.x[1] = 42;
    cpu.x[2] = 58;

    DECODED_INSTRUCTION instr = {
        .op = OP_ADD,
        .rs1 = 1,
        .rs2 = 2,
        .rd = 0,
        .imm = 0
    };

    alu_compute(instr, &cpu);
    assert(cpu.x[0] == 0);

    instr.op = OP_ADDI;
    instr.rs1 = 0;
    instr.rd = 0;
    instr.imm = 100;

    alu_compute(instr, &cpu);
    assert(cpu.x[0] == 0);

    printf("[PASS] x0\n");
}

void test_overflow_underflow(void)
{
    CPU_STATE cpu;
    cpu_reset(&cpu);

    cpu.x[1] = 0xFFFFFFFF;
    cpu.x[2] = 1;

    DECODED_INSTRUCTION instr = {
        .op  = OP_ADD,
        .rs1 = 1,
        .rs2 = 2,
        .rd  = 3,
        .imm = 0
    };

    alu_compute(instr, &cpu);
    assert(cpu.x[3] == 0x00000000);

    instr.op  = OP_SUB;
    instr.rs1 = 0; 
    instr.rs2 = 2; 
    instr.rd  = 4;

    alu_compute(instr, &cpu);
    assert(cpu.x[4] == 0xFFFFFFFF);

    printf("[PASS] Overflow and Underflow\n");
}

void test_shifts(void)
{
    CPU_STATE cpu;
    cpu_reset(&cpu);

    cpu.x[1] = 0x00000001;
    cpu.x[2] = 33;

    DECODED_INSTRUCTION instr = {
        .op  = OP_SLL,
        .rs1 = 1,
        .rs2 = 2,
        .rd  = 3,
        .imm = 0
    };

    alu_compute(instr, &cpu);
    assert(cpu.x[3] == 0x00000002);

    cpu.x[4] = 0x80000000U;
    instr.op  = OP_SRLI;
    instr.rs1 = 4;
    instr.rd  = 5;
    instr.imm = 4;

    alu_compute(instr, &cpu);
    assert(cpu.x[5] == 0x08000000);

    instr.op  = OP_SRAI;
    instr.rs1 = 4;
    instr.rd  = 6;
    instr.imm = 4;
    alu_compute(instr, &cpu);
    assert(cpu.x[6] == 0xF8000000);

    printf("[PASS] Shifts\n");
}

void test_comparisons(void)
{
    CPU_STATE cpu;
    cpu_reset(&cpu);

    cpu.x[1] = 0xFFFFFFFF;
    cpu.x[2] = 0x00000001;

    DECODED_INSTRUCTION instr = {
        .op  = OP_SLT,
        .rs1 = 1,
        .rs2 = 2,
        .rd  = 3,
        .imm = 0
    };

    alu_compute(instr, &cpu);
    assert(cpu.x[3] == 1);

    instr.op = OP_SLTU;
    instr.rd = 4;
    alu_compute(instr, &cpu);
    assert(cpu.x[4] == 0);

    instr.op  = OP_SLTIU;
    instr.rs1 = 2;
    instr.rd  = 5;
    instr.imm = 2;
    alu_compute(instr, &cpu);
    assert(cpu.x[5] == 1);

    printf("[PASS] Comparisons\n");
}

void test_logical(void)
{
    CPU_STATE cpu;
    cpu_reset(&cpu);

    cpu.x[1] = 0xA;
    cpu.x[2] = 0xC;

    DECODED_INSTRUCTION instr = {
        .op  = OP_AND,
        .rs1 = 1,
        .rs2 = 2,
        .rd  = 3,
        .imm = 0
    };

    alu_compute(instr, &cpu);
    assert(cpu.x[3] == 0x8);

    instr.op = OP_OR;
    instr.rd = 4;
    alu_compute(instr, &cpu);
    assert(cpu.x[4] == 0xE);

    instr.op = OP_XOR;
    instr.rd = 5;
    alu_compute(instr, &cpu);
    assert(cpu.x[5] == 0x6);

    printf("[PASS] Logical operations\n");
}

int main(void)
{
    test_x0();
    test_overflow_underflow();
    test_shifts();
    test_comparisons();
    test_logical();

    printf("All alu tests passed successfully\n");
    return 0;
}