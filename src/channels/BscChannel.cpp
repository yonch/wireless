/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "channels/BscChannel.h"

BscChannel::BscChannel(float p)
  : m_p(p)
{}

void BscChannel::seed(unsigned int *const seed, int seedSize) {
	m_random.seed(seed, seedSize);
}

void BscChannel::process(const std::vector<Symbol>& inSymbols,
	 	 	 	 	 	 std::vector<Symbol>& outSymbols)
{
	// flip bits with probability p
	unsigned int symbolsSize = inSymbols.size();
	outSymbols.clear();
	outSymbols.resize(symbolsSize);
	for (unsigned int i = 0; i < symbolsSize; i++) {
		if(m_random.rand() < m_p) {
			outSymbols[i] = inSymbols[i] ^ 0x1;
		} else {
			outSymbols[i] = inSymbols[i];
		}
	}
}

unsigned int BscChannel::forecast(unsigned int numOutputs)
{
	return numOutputs;
}
