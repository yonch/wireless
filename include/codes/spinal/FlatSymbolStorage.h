/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

/**
 * \ingroup spinal
 * \brief Stores symbols indexed by spine value
 */
template<typename ChannelSymbol>
class FlatSymbolStorage {
public:
	/**
	 * C'tor
	 * @param spineLength: the number of spine values to store symbols for
	 * @param maxNumSymbolsPerValue: the maximum number of symbols we'll have
	 * 		to store for all spine values except the last one
	 * @param maxNumSymbolsLastValue: the maximum number of symbols we'll have
	 * 		to store for the last spine value.
	 */
	FlatSymbolStorage(unsigned int spineLength,
	                  unsigned int maxNumSymbolsPerValue,
	                  unsigned int maxNumSymbolsLastValue);

	/**
	 * Copy c'tor
	 */
	FlatSymbolStorage(const FlatSymbolStorage<ChannelSymbol>& other);

	/**
	 * D'tor
	 */
	~FlatSymbolStorage();

	/**
	 * Erases all symbols from the storage, preparing it for a new message
	 */
	void reset();

	/**
	 * Adds the symbol to the spine value. The order of the symbols stored in
	 * 		the structure is the same as the order passed to add().
	 */
	void add(unsigned int spineValueInd,
	         ChannelSymbol sym);

	/**
	 * @return a pointer to the symbols for a specific spine value
	 * @param spineValueInd: the spine value to return symbols for
	 */
	ChannelSymbol* get(unsigned int spineValueInd);

	/**
	 * @return the number of symbols in storage, for a specific spine value
	 * @param spineValueInd: the spine value to return symbols for
	 */
	unsigned int size(unsigned int spineValueInd);

private:
	// number of spine values
	const unsigned int m_spineLength;

	// the maximum number of symbols for spine values except the last one
	const unsigned int m_maxNumSymbolsPerValue;

	// the maximum number of symbols for the last spine value
	const unsigned int m_maxNumSymbolsLastValue;

	// The channel symbols
	ChannelSymbol* m_symbols;

	// The number of symbols currently in storage, for each spine value
	std::vector<uint16_t> m_numSymbols;
};


#include "FlatSymbolStorage.hh"
