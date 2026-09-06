#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <stdint.h>

typedef struct {
    uint32_t x[32];
    uint32_t pc;
} CPU_STATE;

void cpu_reset(CPU_STATE* cpuState);

#endif