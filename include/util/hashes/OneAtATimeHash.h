/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "SingleSymbolFunction.h"
#include "UnlimitedHash.h"

/**
 * \ingroup hashes
 * \brief Implementation of Bob Jenkin's One-At-A-Time hash function.
 *
 * 		code and discussion at:
 * 			http://www.burtleburtle.net/bob/hash/doobs.html
 */
class OneAtATimeHash {
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
 * \brief Produces symbols from the digest of a SalsaHash
 *
 * @Note: one at a time hash produces one symbol per hash application.
 */
typedef SingleSymbolFunction<OneAtATimeHash> OneAtATimeSymbolFunction;

typedef UnlimitedHash<OneAtATimeSymbolFunction> OneAtATimeUnlimitedHash;
