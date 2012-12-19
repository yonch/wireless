/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "BitwiseXor.h"

void BitwiseXor::init(Digest digest, State & state) {
	state = digest;
}

void BitwiseXor::update(State & state, unsigned int data) {
	state ^= data;
}

BitwiseXor::Digest BitwiseXor::digest(const State & state) {
	return state;
}


