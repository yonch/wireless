/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include "UnlimitedHash.h"

/**
 * \ingroup hashes
 * \brief Implementation of Bob Jenkin's lookup3 hash function.
 *
 * 		Source code partially derived from:
 * 			http://www.burtleburtle.net/bob/c/lookup3.c
 */
class Lookup3Hash {
public:
	// The hash's output
	typedef uint32_t Digest;

	// The internal state of the hash
	typedef uint32_t State[3];

	static void init(Digest digest, State& state);

	static void update(State& state, unsigned int data);

	static Digest digest(const State& state);

	static uint64_t digest_extended(const State& state);
};

/**
 * \ingroup hashes
 * \brief Produces symbols from the digest of a Lookup3Hash
 */
class Lookup3SymbolFunction{
public:
	typedef Lookup3Hash Hash;
	static const unsigned int NUM_SYMBOLS_PER_STATE = 2;

	static void getSymbols(const Hash::State& state,
	                       uint16_t* symbols);

};

typedef UnlimitedHash<Lookup3SymbolFunction> Lookup3UnlimitedHash;
