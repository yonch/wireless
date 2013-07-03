/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup hashes
 * \brief An adaptor class that allows updates bit-by-bit, then hashes the entire state.
 *
 * Initialization keeps the initialized state until digestion, and
 * 	 inits internal state to 0.
 * Updates on the hash only shift left the state by one bit, and xor the data
 * Digest causes an init of the underlying hash with value saved, update with
 *   state of the shift register, and then digest.
 */
template<typename Hash>
class ShiftRegisterAdaptorHash {
public:
	// The hash's output
	typedef typename Hash::Digest Digest;

	// The internal state of the hash
	struct State {
		typename Hash::Digest digest;
		unsigned int shiftRegister;
	};

	static void init(Digest digest, State& state);

	static void update(State& state, unsigned int data);

	static Digest digest(const State& state);
};

/**
 * \ingroup hashes
 * \brief Produces symbols from the digest of a ShiftRegisterAdaptorHash
 */
template<typename HashFunctionT>
class ShiftRegisterAdaptorSymbolFunction {
public:
	typedef ShiftRegisterAdaptorHash<HashFunctionT> Hash;

	ShiftRegisterAdaptorSymbolFunction(unsigned int symbolSizeBits);

	unsigned int numSymbolsPerState();

	int getSymbol(const typename Hash::State& state,
				  unsigned int symbolIndex);

protected:
	int m_symbolSizeBits;
	// the mask ANDed with a word to get a single symbol
	unsigned int m_symbolMask;
};



/***************************************
 * ShiftRegisterAdaptorHash
 ***************************************/

template<typename Hash>
inline void ShiftRegisterAdaptorHash<Hash>::init(Digest digest, State & state)
{
	state.digest = digest;
	state.shiftRegister = 0;
}


template<typename Hash>
inline void ShiftRegisterAdaptorHash<Hash>::update(State & state, unsigned int data)
{
	state.shiftRegister = (state.shiftRegister << 1) ^ data;
}


template<typename Hash>
inline typename ShiftRegisterAdaptorHash<Hash>::Digest ShiftRegisterAdaptorHash<Hash>::digest(const State & state)
{
	typename Hash::State hashState;
	// Init hash with digest given in our ::init
	Hash::init(state.digest, hashState);
	// Update with the shift register
	Hash::update(hashState, state.shiftRegister);
	// Return the digest
	return Hash::digest(hashState);
}



/***************************************
 * ShiftRegisterAdaptorSymbolFunction
 ***************************************/

template<typename HashFunctionT>
inline ShiftRegisterAdaptorSymbolFunction<HashFunctionT>::ShiftRegisterAdaptorSymbolFunction(
		unsigned int symbolSizeBits)
	: m_symbolSizeBits(symbolSizeBits),
	  m_symbolMask(((1 << m_symbolSizeBits)-1))
{
	if (symbolSizeBits > 32) {
		throw(std::runtime_error("Symbol size to big, max 32"));
	}
}

template<typename HashFunctionT>
inline unsigned int ShiftRegisterAdaptorSymbolFunction<HashFunctionT>::numSymbolsPerState()
{
	return 1;
}

template<typename HashFunctionT>
inline int ShiftRegisterAdaptorSymbolFunction<HashFunctionT>::getSymbol(
		const typename Hash::State & state,
		unsigned int symbolIndex)
{
    if (symbolIndex >= 1) {
    	throw std::runtime_error("Asked for a too high pointIndex");
    }
    unsigned int bits = Hash::digest(state) & m_symbolMask;
    return ((int)bits);
}
