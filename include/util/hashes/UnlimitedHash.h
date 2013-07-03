/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

using namespace std;

/**
 * \ingroup hashes
 * \brief A hash function that produces a pseudo-random stream of 16-bit values.
 *
 * The stream of 16-bit values is produced by iteratively calling nextBits().
 *
 * The internal state (the "seed") can be retrieved using getSeed().
 *
 * Hash the internal seed with an external 32-bit word is done with hash().
 */
template<typename SymbolFunction>
class UnlimitedHash
{
public:
	typedef typename SymbolFunction::Hash Hash;
	typedef typename SymbolFunction::Hash::Digest Seed;

	/**
	 * C'tor
	 * @param spineSeed: the seed of the spine (from which a pseudo-random
	 * 		stream of bits is generated)
	 **/
	UnlimitedHash(const Seed prevSeed, uint32_t data);

	/**
	 * Evaluates $h$ on the previous spine and input data.
	 * Sets the f index to the first f (symbolInd = 0)
	 * @param prevSpine the last value of the spine.
	 * @param data the message bits to incorporate into the spine
	 */
	void hash(uint32_t data);

	/**
	 * Returns the spine's
	 */
	Seed getSeed();

	/**
	 * @return the next 16 bits from the unlimited hash
	 */
	uint16_t next();

private:

	// The previous spine
	Seed m_seed;

	// 0 if m_hashState is the output of m_prevSpine and m_data;
	// the index of the next derivation used, otherwise
	unsigned int m_stateDerivationVersion;

	// the next bits to be output from the spine value
	uint16_t m_nextSymbols[SymbolFunction::NUM_SYMBOLS_PER_STATE];

	// the number of symbols valid in m_nextSymbols
	unsigned int m_numRemainingSymbols;
};

#include "UnlimitedHash.hh"
