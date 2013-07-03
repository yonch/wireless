/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdexcept>
#include <stdint.h>

/**
 * \ingroup hashes
 * \brief Extracts a single symbol from the underlying hash digest
 */
template<typename HashFunctionT>
class SingleSymbolFunction{
public:
	typedef HashFunctionT Hash;
	static const unsigned int NUM_SYMBOLS_PER_STATE = 1;

	static void getSymbols(const typename Hash::State& state,
	                       uint16_t* symbols);
};


template<typename HashFunctionT>
inline void SingleSymbolFunction<HashFunctionT>::getSymbols(
		const typename Hash::State & state,
		uint16_t* symbols)
{
	*symbols = state & 0xFFFF;
}
