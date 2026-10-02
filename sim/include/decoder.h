#ifndef DECODER_H
#define DECODER_H

#include "config.h"
#include <stdint.h>

// Przetwarza 32-bitowe słowo maszynowe (instrukcję) na strukturę zdekodowanej instrukcji
void decoder_decode(DECODED_INSTRUCTION* decodedInstruction, uint32_t instruction);

#endif