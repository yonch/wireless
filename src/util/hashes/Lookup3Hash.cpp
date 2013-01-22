/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "Lookup3Hash.h"

#include <stdexcept>

/**************************
 * Lookup3Hash
 **************************/

void Lookup3Hash::init(Digest digest, State& state) {
	state[0] = state[1] = state[2] = 0xdeadbeef + digest;
}

#define rot(x,k) (((x) << (k)) | ((x) >> (32-(k))))


void Lookup3Hash::update(State& state, unsigned int data) {
	state[1] += data;
	state[2] ^= state[1]; state[2] -= rot(state[1],14); \
	state[0] ^= state[2]; state[0] -= rot(state[2],11); \
	state[1] ^= state[0]; state[1] -= rot(state[0],25); \
	state[2] ^= state[1]; state[2] -= rot(state[1],16); \
	state[0] ^= state[2]; state[0] -= rot(state[2],4);  \
	state[1] ^= state[0]; state[1] -= rot(state[0],14); \
	state[2] ^= state[1]; state[2] -= rot(state[1],24); \
}

Lookup3Hash::Digest Lookup3Hash::digest(const State& state) {
	return state[2];
}

uint64_t Lookup3Hash::digest_extended(const State& state) {
	return state[2] | (uint64_t(state[1]) << 32);
}


/**************************
 * Lookup3SymbolFunction
 **************************/


void Lookup3SymbolFunction::getSymbols(const Hash::State & state,
                                       uint16_t* symbols)
{
	symbols[0] = state[2] & 0xffff;
	symbols[1] = state[2] >> 16;
}


