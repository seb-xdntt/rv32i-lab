#include "cpu_state.h"
#include <string.h>

void cpu_reset(CPU_STATE* cpuState)
{
    memset(cpuState->x, 0, sizeof(cpuState->x));
    cpuState->pc = 0x80000000;
}