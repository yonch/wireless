/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <algorithm>
#include <stdexcept>
#include "string.h"

template<typename ChannelSymbol>
inline FlatSymbolStorage<ChannelSymbol>::FlatSymbolStorage(
		unsigned int spineLength,
		unsigned int maxNumSymbolsPerValue,
		unsigned int maxNumSymbolsLastValue)
  : m_spineLength(spineLength),
    m_maxNumSymbolsPerValue(maxNumSymbolsPerValue),
    m_maxNumSymbolsLastValue(maxNumSymbolsLastValue),
    m_numSymbols(spineLength, 0)
{
	// Make sure the spine is non-empty
	if(spineLength < 1) {
		throw(std::runtime_error("Spine length has to be positive"));
	}

	// Calculate the total maximum number of symbols we may need to store
	unsigned int maxNumSymbols =
			(  ((m_spineLength - 1) * m_maxNumSymbolsPerValue)
			 + m_maxNumSymbolsLastValue);

	// Allocate memory for symbols
	m_symbols = new ChannelSymbol[maxNumSymbols];
	if(m_symbols == NULL) {
		throw(std::runtime_error("Could not allocate symbol array"));
	}
}

template<typename ChannelSymbol>
inline FlatSymbolStorage<ChannelSymbol>::FlatSymbolStorage(
		const FlatSymbolStorage<ChannelSymbol> & other)
  : m_spineLength(other.m_spineLength),
    m_maxNumSymbolsPerValue(other.m_maxNumSymbolsPerValue),
    m_maxNumSymbolsLastValue(other.m_maxNumSymbolsLastValue),
    m_numSymbols(other.m_numSymbols)
{
	// Calculate the total maximum number of symbols we may need to store
	unsigned int maxNumSymbols =
			(  ((m_spineLength - 1) * m_maxNumSymbolsPerValue)
			 + m_maxNumSymbolsLastValue);

	// Allocate memory for symbols
	m_symbols = new ChannelSymbol[maxNumSymbols];
	if(m_symbols == NULL) {
		throw(std::runtime_error("Could not allocate symbol array"));
	}

	// Copy the existing symbols from other storage
	for(unsigned int i = 0; i < m_spineLength; i++) {
		memcpy(&m_symbols[i * m_maxNumSymbolsPerValue],
			   &other.m_symbols[i * m_maxNumSymbolsPerValue],
			   sizeof(ChannelSymbol) * m_numSymbols[i]);
	}
}

template<typename ChannelSymbol>
inline FlatSymbolStorage<ChannelSymbol>::~FlatSymbolStorage() {
	delete [] m_symbols;
}

template<typename ChannelSymbol>
inline void FlatSymbolStorage<ChannelSymbol>::reset() {
	// We reset the number of symbols at each spine value, effectively emptying
	// the storage.
	std::fill(m_numSymbols.begin(), m_numSymbols.end(), 0);
}

template<typename ChannelSymbol>
inline void FlatSymbolStorage<ChannelSymbol>::add(
		unsigned int spineValueInd,
		ChannelSymbol sym)
{
	unsigned int spineValueBase = spineValueInd * m_maxNumSymbolsPerValue;

	m_symbols[spineValueBase + m_numSymbols[spineValueInd]] = sym;
	m_numSymbols[spineValueInd]++;
}

template<typename ChannelSymbol>
inline ChannelSymbol *FlatSymbolStorage<ChannelSymbol>::get(
		unsigned int spineValueInd)
{
	unsigned int spineValueBase = spineValueInd * m_maxNumSymbolsPerValue;
	return &m_symbols[spineValueBase];
}

template<typename ChannelSymbol>
inline unsigned int FlatSymbolStorage<ChannelSymbol>::size(
		unsigned int spineValueInd)
{
	return m_numSymbols[spineValueInd];
}

