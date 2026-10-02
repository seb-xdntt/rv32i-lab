#include "cpu_state.h"
#include <string.h>

void cpu_reset(CPU_STATE* cpuState) 
{
    //Zerowanie wszystkich 32 rejestrów ogólnego przeznaczenia
    memset(cpuState->x, 0, sizeof(cpuState->x));

    //Ustawienie początkowego adresu pamięci procesora w pamięci RAM
    cpuState->pc = 0x80000000; 
}