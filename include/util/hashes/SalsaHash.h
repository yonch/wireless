/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "salsa20.h"
#include <stdint.h>
#include "UnlimitedHash.h"

class SalsaHash {
public:
	// The hash's output
	typedef SalsaDigest Digest;

	// The internal state of the hash
	typedef SalsaState State;

	static void init(Digest digest, State& state);

	static void update(State& state, unsigned int data);

	static Digest digest(const State& state);
};

/**
 * \ingroup hashes
 * \brief Produces symbols from the digest of a SalsaHash
 */
class SalsaSymbolFunction{
public:
	typedef SalsaHash Hash;
	static const unsigned int NUM_SYMBOLS_PER_STATE = 32;

	static void getSymbols(const Hash::State& state,
	                       uint16_t* symbols);
};

typedef UnlimitedHash<SalsaSymbolFunction> SalsaUnlimitedHash;
