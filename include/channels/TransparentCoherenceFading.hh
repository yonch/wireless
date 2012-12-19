/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdint.h>

#define TRANSP_COH_FADING_CHANNEL_SUBSEED_SIZE 4

template<typename ChannelSymbol>
inline TransparentCoherenceFading<ChannelSymbol>::TransparentCoherenceFading(
		uint32_t coherenceInterval,
		N0_t n0)
  : m_random(0u),
    m_coeffGenerator(coherenceInterval),
    m_stddevPerDimension(sqrt(n0 / 2.0))
{}

template<typename ChannelSymbol>
inline void TransparentCoherenceFading<ChannelSymbol>::seed(
		unsigned int * const seed,
		int seedSize)
{
	// seed own random
	m_random.seed(seed, seedSize);

	// seed the coefficient generator
	uint32_t subSeed[TRANSP_COH_FADING_CHANNEL_SUBSEED_SIZE];
	for(uint32_t i = 0; i < TRANSP_COH_FADING_CHANNEL_SUBSEED_SIZE; i++) {
		subSeed[i] = m_random.randInt();
	}
	m_coeffGenerator.seed(subSeed, TRANSP_COH_FADING_CHANNEL_SUBSEED_SIZE);
}

template<typename ChannelSymbol>
inline void TransparentCoherenceFading<ChannelSymbol>::process(
		const std::vector<ChannelSymbol> & inSymbols,
		std::vector<ChannelSymbol> & outSymbols)
{
	uint32_t numSymbols = inSymbols.size();

	outSymbols.resize(numSymbols);

	for(uint32_t i = 0; i < numSymbols; i++) {
		FadingMagnitude fading = m_coeffGenerator.next();
		outSymbols[i] =	AwgnNoiseGenerator<ChannelSymbol>::noisify(
				m_random, inSymbols[i], m_stddevPerDimension / fading);
	}
}

template<typename ChannelSymbol>
inline unsigned int TransparentCoherenceFading<ChannelSymbol>::forecast(
		unsigned int numOutputs)
{
	return numOutputs;
}

template<typename ChannelSymbol>
inline void TransparentCoherenceFading<ChannelSymbol>::transform(
		const std::vector<InputType> & inSymbols,
		const std::vector<OutputType> & observedSymbols,
		std::vector<OutputType> & outSymbols) const
{
	// There is no actual transformation to do in this case, just copy the input
	// symbols to the output symbols
	outSymbols = inSymbols;
}

