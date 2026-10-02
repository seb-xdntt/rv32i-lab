#ifndef CPU_STATE_H
#define CPU_STATE_H

#include "config.h"

//Inicjalizuje stan procesora (zeruje rejestry i ustawia PC na początek pamięci programu)
void cpu_reset(CPU_STATE* cpuState);

#endif