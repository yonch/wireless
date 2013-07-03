/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "SingleSymbolFunction.h"

/**
 * \ingroup hashes
 * \brief Implementation of a simple bitwise XOR of uint32
 */
class BitwiseXor {
public:
	// The hash's output
	typedef unsigned int Digest;

	// The internal state of the hash
	typedef unsigned int State;

	static void init(Digest digest, State& state);

	static void update(State& state, unsigned int data);

	static Digest digest(const State& state);
};

/**
 * \ingroup hashes
 * \brief Produces symbols from the digest of a BitwiseXor
 */
typedef SingleSymbolFunction<BitwiseXor> BitwiseXorSymbolFunction;

