/*
Based on:
salsa20-merged.c version 20051118
D. J. Bernstein
Public domain.

Modified by Jonathan Perry 2012.
Public domain.
*/

#include <string.h> // for memcpy
#include <stdexcept>

#include "ecrypt-portable.h"
#include "salsa20.h"

#define SALSA_WORD_SIZE_BITS 64


#define ROTATE(v,c) (ROTL32(v,c))
#define PLUS(v,w) (U32V((v) + (w)))

#define XOR(v,w) ((v) ^ (w))
#define PLUSONE(v) (PLUS((v),1))

#define UPSAMPLE32(x,y) ((((u64)(x)) << 32) | (y))

static const char tau[17] = "expand 16-byte k";

void salsaInit(const SalsaDigest digest, SalsaState& state) {
	// data is not initialized since it is not known at init time.
	//state[6] = data;
	//state[7] = data;
	state[6] = 0;
	state[7] = 0;
	state[8] = 0;
	state[9] = 0;

	state[1] = (digest >>  0) & 0xFFFF;
	state[2] = (digest >> 16) & 0xFFFF;
	state[3] = (digest >> 32) & 0xFFFF;
	state[4] = (digest >> 48) & 0xFFFF;

	state[11] = state[1];
	state[12] = state[2];
	state[13] = state[3];
	state[14] = state[4];
	state[0] = U8TO32_LITTLE(tau + 0);
	state[5] = U8TO32_LITTLE(tau + 4);
	state[10] = U8TO32_LITTLE(tau + 8);
	state[15] = U8TO32_LITTLE(tau + 12);
}


void salsaUpdate(SalsaState& state, u32 data)
{
	state[6] ^= data;
	state[7] ^= data;

	SalsaState savedState;
	memcpy(savedState, state, sizeof(SalsaState));
	int i;

	for (i = 12;i > 0;i -= 2) {
		state[4] = XOR( state[4],ROTATE(PLUS( state[0],state[12]), 7));
		state[8] = XOR( state[8],ROTATE(PLUS( state[4], state[0]), 9));
		state[12] = XOR(state[12],ROTATE(PLUS( state[8], state[4]),13));
		state[0] = XOR( state[0],ROTATE(PLUS(state[12], state[8]),18));
		state[9] = XOR( state[9],ROTATE(PLUS( state[5], state[1]), 7));
		state[13] = XOR(state[13],ROTATE(PLUS( state[9], state[5]), 9));
		state[1] = XOR( state[1],ROTATE(PLUS(state[13], state[9]),13));
		state[5] = XOR( state[5],ROTATE(PLUS( state[1],state[13]),18));
		state[14] = XOR(state[14],ROTATE(PLUS(state[10], state[6]), 7));
		state[2] = XOR( state[2],ROTATE(PLUS(state[14],state[10]), 9));
		state[6] = XOR( state[6],ROTATE(PLUS( state[2],state[14]),13));
		state[10] = XOR(state[10],ROTATE(PLUS( state[6], state[2]),18));
		state[3] = XOR( state[3],ROTATE(PLUS(state[15],state[11]), 7));
		state[7] = XOR( state[7],ROTATE(PLUS( state[3],state[15]), 9));
		state[11] = XOR(state[11],ROTATE(PLUS( state[7], state[3]),13));
		state[15] = XOR(state[15],ROTATE(PLUS(state[11], state[7]),18));

		state[1] = XOR( state[1],ROTATE(PLUS( state[0], state[3]), 7));
		state[2] = XOR( state[2],ROTATE(PLUS( state[1], state[0]), 9));
		state[3] = XOR( state[3],ROTATE(PLUS( state[2], state[1]),13));
		state[0] = XOR( state[0],ROTATE(PLUS( state[3], state[2]),18));
		state[6] = XOR( state[6],ROTATE(PLUS( state[5], state[4]), 7));
		state[7] = XOR( state[7],ROTATE(PLUS( state[6], state[5]), 9));
		state[4] = XOR( state[4],ROTATE(PLUS( state[7], state[6]),13));
		state[5] = XOR( state[5],ROTATE(PLUS( state[4], state[7]),18));
		state[11] = XOR(state[11],ROTATE(PLUS(state[10], state[9]), 7));
		state[8] = XOR( state[8],ROTATE(PLUS(state[11],state[10]), 9));
		state[9] = XOR( state[9],ROTATE(PLUS( state[8],state[11]),13));
		state[10] = XOR(state[10],ROTATE(PLUS( state[9], state[8]),18));
		state[12] = XOR(state[12],ROTATE(PLUS(state[15],state[14]), 7));
		state[13] = XOR(state[13],ROTATE(PLUS(state[12],state[15]), 9));
		state[14] = XOR(state[14],ROTATE(PLUS(state[13],state[12]),13));
		state[15] = XOR(state[15],ROTATE(PLUS(state[14],state[13]),18));
	}
	for (i = 0; i < 16; i++) {
		state[i] = PLUS(state[i],savedState[i]);
	}

}

SalsaDigest salsaDigest(const SalsaState& state) {
	return UPSAMPLE32((state[1] ^ state[5] ^ state[9] ^ state[13] ^ state[3] ^ state[7] ^ state[11] ^ state[15]),
					  (state[0] ^ state[4] ^ state[8] ^ state[12] ^ state[2] ^ state[6] ^ state[10] ^ state[14]));
}


int salsaGetSymbol(const SalsaState& state,
					unsigned int symbolSizeBits,
					unsigned int pointIndex) {

	unsigned int symbolsPerWord = SALSA_WORD_SIZE_BITS / symbolSizeBits;
    unsigned int wordInd = pointIndex / symbolsPerWord;
    if (wordInd >= 8) {
    	throw std::runtime_error("Asked for a too high pointIndex");
    }
    unsigned long long word = UPSAMPLE32(state[2*wordInd+1], state[2*wordInd]);

    unsigned int shiftAmt = (pointIndex % symbolsPerWord) * symbolSizeBits;
    unsigned int bits = (word >> shiftAmt) & ((1 << symbolSizeBits)-1);
    return ((int)bits);
}
