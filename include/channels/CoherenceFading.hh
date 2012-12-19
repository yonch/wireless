/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <assert.h>
#include <math.h>
#include <algorithm>

template<typename ChannelSymbol>
inline CoherenceFading<ChannelSymbol>::CoherenceFading(
		uint32_t coherenceInterval)
  : m_coeffGenerator(coherenceInterval)
{}

template<typename ChannelSymbol>
void CoherenceFading<ChannelSymbol>::seed(uint32_t *const seed, int seedSize)
{
	m_coeffGenerator.seed(seed, seedSize);
}

template<typename ChannelSymbol>
inline void CoherenceFading<ChannelSymbol>::process(
		const std::vector<ChannelSymbol> & inSymbols,
		std::vector<OutputType> & outSymbols)
{
	uint32_t numSymbols = inSymbols.size();

	outSymbols.resize(numSymbols);

	for(uint32_t i = 0; i < numSymbols; i++) {
		FadingMagnitude fading = m_coeffGenerator.next();
		outSymbols[i].fading = fading;
		outSymbols[i].symbol = inSymbols[i] * ComplexBaseType(fading);
	}
}


template<typename ChannelSymbol>
unsigned int CoherenceFading<ChannelSymbol>::forecast(unsigned int numOutputs) {
	return numOutputs;
}

template<typename ChannelSymbol>
inline void CoherenceFading<ChannelSymbol>::transform(
								const std::vector<ChannelSymbol> & inSymbols,
								const std::vector<OutputType> & observedSymbols,
								std::vector<OutputType> & outSymbols) const
{
	uint32_t numSymbols = std::min(inSymbols.size(), observedSymbols.size());

	outSymbols.resize(numSymbols);

	for(uint32_t i = 0; i < numSymbols; i++) {
		FadingMagnitude fading = observedSymbols[i].fading;
		outSymbols[i].fading = fading;
		outSymbols[i].symbol = inSymbols[i] * ComplexBaseType(fading);
	}
}
