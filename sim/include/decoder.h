#ifndef DECODER_H
#define DECODER_H

#include "config.h"
#include <stdint.h>

void decoder_init(DECODED_INSTRUCTION* decodedInstruction);
void decoder_decode(DECODED_INSTRUCTION* decodedInstruction, uint32_t instruction);

#endif