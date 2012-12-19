/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#include <math.h>
#include <limits.h>
#include <stdexcept>

#include "../CodeBench.h"
#include "AwgnNoiseGenerator.h"

template<typename ChannelSymbol>
inline AwgnChannel<ChannelSymbol>::AwgnChannel(N0_t n0)
  : m_stddevPerDimension(sqrt(n0 / 2.0)),
    m_random(0u)
{}

template<typename ChannelSymbol>
inline void AwgnChannel<ChannelSymbol>::seed(
		unsigned int *const seed,
		int seedSize)
{
	m_random.seed(seed, seedSize);
}

template<typename ChannelSymbol>
inline void AwgnChannel<ChannelSymbol>::process(
		const std::vector<ChannelSymbol> & inSymbols,
		std::vector<ChannelSymbol> & outSymbols)
{
	unsigned int symbolsSize = inSymbols.size();
	outSymbols.clear();
	outSymbols.resize(symbolsSize);
	for (unsigned int i = 0; i < symbolsSize; i++) {
		outSymbols[i] = noisify(inSymbols[i], m_stddevPerDimension);
	}
}

template<typename ChannelSymbol>
inline unsigned int AwgnChannel<ChannelSymbol>::forecast(unsigned int numOutputs)
{
	return numOutputs;
}

template<typename ChannelSymbol>
inline double AwgnChannel<ChannelSymbol>::getStddevPerDimension()
{
	return m_stddevPerDimension;
}

template<typename ChannelSymbol>
inline ChannelSymbol AwgnChannel<ChannelSymbol>::noisify(
		ChannelSymbol x,
		double stddevPerDimension)
{
	return AwgnNoiseGenerator<ChannelSymbol>::noisify(m_random, x, stddevPerDimension);
}

