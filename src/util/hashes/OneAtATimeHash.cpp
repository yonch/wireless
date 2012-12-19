/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "OneAtATimeHash.h"

#include <stdexcept>

/**************************
 * OneAtATimeHash
 **************************/

void OneAtATimeHash::init(Digest digest, State& state) {
	state = 3321836253 ^ digest;
}

#define rot(x,k) (((x) << (k)) | ((x) >> (32-(k))))


void OneAtATimeHash::update(State& state, unsigned int data) {
	for(unsigned int i = 0; i < 4; ++i) {
		state += data;
		state += (state << 10);
		state ^= (state >> 6);
		data >>= 8;
	}

	state += (state << 3);
	state ^= (state >> 11);
	state += (state << 15);
}

OneAtATimeHash::Digest OneAtATimeHash::digest(const State& state) {
	return state;
}

