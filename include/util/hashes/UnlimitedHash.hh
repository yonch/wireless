/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

// IMPLEMENTATION OF INLINE FUNCTIONS.
// SHOULD ONLY BE INCLUDED FROM UnlimitedHash.h

#define INITIAL_STATE_VERSION (3610617884)
#define STATE_VERSION_INCREMENT (3243335647)

template<typename SymbolFunction>
inline UnlimitedHash<SymbolFunction>::UnlimitedHash(
		const Seed prevSeed,
		uint32_t data)
  : m_seed(prevSeed)
{
	hash(data);
}

template<typename SymbolFunction>
inline void UnlimitedHash<SymbolFunction>::hash(unsigned int data)
{
	typename Hash::State hashState;

	Hash::init(m_seed, hashState);
	Hash::update(hashState, data);

	// Update internal seed with new seed
	m_seed = Hash::digest(hashState);

	// Use the state to get the first symbols
	SymbolFunction::getSymbols(hashState, m_nextSymbols);
	m_numRemainingSymbols = SymbolFunction::NUM_SYMBOLS_PER_STATE;

	// Reset the pseudo-random symbol generation
	m_stateDerivationVersion = INITIAL_STATE_VERSION;
}

template<typename SymbolFunction>
inline typename SymbolFunction::Hash::Digest
	UnlimitedHash<SymbolFunction>::getSeed()
{
	return m_seed;
}

template<typename SymbolFunction>
inline uint16_t UnlimitedHash<SymbolFunction>::next()
{
	// Do we need to apply hash function to get different state?
	if (m_numRemainingSymbols == 0) {
		// We need to re-derive

		// need to derive from m_seed with m_stateDerivationVersion
		typename Hash::State hashState;
		Hash::init(m_seed, hashState);
		Hash::update(hashState, m_stateDerivationVersion);

		// Get symbols from the state to array, update number available symbols
		SymbolFunction::getSymbols(hashState, m_nextSymbols);
		m_numRemainingSymbols = SymbolFunction::NUM_SYMBOLS_PER_STATE;

		// Update the state derivation version, so next time we'll get more
		// pseudo-random symbols.
		m_stateDerivationVersion += STATE_VERSION_INCREMENT;
	}

	return m_nextSymbols[--m_numRemainingSymbols];
}
