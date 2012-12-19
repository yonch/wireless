/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "SalsaHash.h"
#include "salsa20.h"

/**************************
 * SalsaHash
 **************************/

void SalsaHash::init(Digest digest, State& state) {
	salsaInit(digest, state);
}

void SalsaHash::update(State& state, u32 data) {
	salsaUpdate(state, data);
}

SalsaHash::Digest SalsaHash::digest(const State& state) {
	return salsaDigest(state);
}


/**************************
 * SalsaSymbolFunction
 **************************/
void SalsaSymbolFunction::getSymbols(
		const Hash::State & state,
		uint16_t *symbols)
{
	for(unsigned int i = 0; i < 16; i++) {
		symbols[(2 * i) + 0] = (state[15 - i] >> 16) & 0xFFFF;
		symbols[(2 * i) + 1] = (state[15 - i] >>  0) & 0xFFFF;
	}
}
