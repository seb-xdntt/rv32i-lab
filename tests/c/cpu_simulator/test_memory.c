#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include "cpu_state.h"
#include "memory.h"

void test_endianess_word_access(void)
{
    uint8_t memory[MEM_SIZE];
    
    CPU_STATE cpu;
    cpu_reset(&cpu);

    uint32_t address = MEM_BASE + 0x100;
    uint32_t value = 0x12345678;

    memory_store(memory, address, value, OP_SW);

    uint32_t offset = address - MEM_BASE;

    assert(memory[offset] == 0x78);
    assert(memory[offset + 1] == 0x56);
    assert(memory[offset + 2] == 0x34);
    assert(memory[offset + 3] == 0x12);

    memory_load(memory, &cpu, address, 1, OP_LW);
    assert(cpu.x[1] == 0x12345678);

    printf("[PASS] Little endian and word access\n");
}

void test_stores_loads(void)
{
    uint8_t memory[MEM_SIZE];
    
    CPU_STATE cpu;
    cpu_reset(&cpu);

    uint32_t address = MEM_BASE + 0x100;

    memory_store(memory, address, 0x3E, OP_SB);
    memory_store(memory, address + 1, 0x47, OP_SB);
    memory_store(memory, address + 2, 0xC7FFU, OP_SH);

    memory_load(memory, &cpu, address, 2, OP_LW);
    assert(cpu.x[2] == 0xC7FF473EU);

    printf("[PASS] Stores and loads\n");
}

void test_sign_extension(void)
{
    uint8_t memory[MEM_SIZE];
    
    CPU_STATE cpu;
    cpu_reset(&cpu);

    uint32_t address1 = MEM_BASE + 0x300;
    uint32_t address2 = MEM_BASE + 0x304;
    uint32_t address3 = MEM_BASE + 0x308;
    uint32_t address4 = MEM_BASE + 0x30C;

    memory_store(memory, address1, 0x80U, OP_SB);
    memory_store(memory, address2, 0x7F, OP_SB);
    memory_store(memory, address3, 0x8000U, OP_SH);
    memory_store(memory, address4, 0x7FFF, OP_SH);

    memory_load(memory, &cpu, address1, 3, OP_LB);
    assert(cpu.x[3] == 0xFFFFFF80U);

    memory_load(memory, &cpu, address1, 4, OP_LBU);
    assert(cpu.x[4] == 0x00000080U);

    memory_load(memory, &cpu, address2, 5, OP_LB);
    assert(cpu.x[5] == 0x0000007F);

    memory_load(memory, &cpu, address3, 6, OP_LH);
    assert(cpu.x[6] == 0xFFFF8000U);

    memory_load(memory, &cpu, address3, 7, OP_LHU);
    assert(cpu.x[7] == 0x00008000U);

    memory_load(memory, &cpu, address4, 8, OP_LH);
    assert(cpu.x[8] == 0x00007FFF);

    printf("[PASS] Sign extension\n");
}

void test_x0(void)
{
    uint8_t memory[MEM_SIZE];
    
    CPU_STATE cpu;
    cpu_reset(&cpu);

    uint32_t address = MEM_BASE + 0x400;
    memory_store(memory, address, 0xFFFFFFFFU, OP_SW);

    memory_load(memory, &cpu, address, 0, OP_LW);
    assert(cpu.x[0] == 0);

    memory_load(memory, &cpu, address, 0, OP_LB);
    assert(cpu.x[0] == 0);

    memory_load(memory, &cpu, address, 0, OP_LH);
    assert(cpu.x[0] == 0);

    printf("[PASS] x0\n");
}

void test_safety_bounds(void)
{
    uint8_t memory[MEM_SIZE];
    
    CPU_STATE cpu;
    cpu_reset(&cpu);

    uint32_t address1 = MEM_BASE - 4;
    memory_store(memory, address1, 0x1236FEA4, OP_SW);
    memory_load(memory, &cpu, address1, 1, OP_LW);
    assert(cpu.x[1] == 0);

    uint32_t address2 = MEM_BASE + (uint32_t)MEM_BASE;
    memory_store(memory, address2, 0x1236FEA4, OP_SW);
    memory_load(memory, &cpu, address2, 2, OP_LW);
    assert(cpu.x[2] == 0);

    uint32_t address3 = MEM_BASE + (uint32_t)MEM_BASE - 2;
    memory_store(memory, address3, 0x1236FEA4, OP_SW);
    memory_load(memory, &cpu, address3, 3, OP_LW);
    assert(cpu.x[3] == 0);

    printf("[PASS] Safety bounds\n");
}

int main(void)
{
    test_endianess_word_access();
    test_stores_loads();
    test_sign_extension();
    test_x0();
    test_safety_bounds();

    printf("All memory tests passed successfully\n");
    return 0;
}