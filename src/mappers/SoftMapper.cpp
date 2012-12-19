/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/SoftMapper.h"

#include <stdexcept>


SoftMapper::SoftMapper(unsigned int symbolSizeBits)
  : m_symbolSizeBits(symbolSizeBits),
    m_symbolMask(((1 << m_symbolSizeBits)-1))
{
	if (symbolSizeBits > 16) {
		throw(std::runtime_error("Symbol size to big, max 16"));
	}
}


void SoftMapper::process(const std::vector<uint16_t>& inSymbols,
	 	 	 	 	 	   std::vector<SoftSymbol>& outSymbols)
{
	unsigned int numSymbols = inSymbols.size();
	outSymbols.resize(numSymbols);
	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = map(inSymbols[i]);
	}
}

SoftSymbol SoftMapper::map(uint16_t sym) {
	return SoftSymbol(sym & m_symbolMask);
}

float SoftMapper::getAveragePower() {
	// N = number of constellation points
	unsigned int N = (1 << m_symbolSizeBits);

	// encoder power is the second moment of uniform discrete distribution
	// around its mean (see http://mathworld.wolfram.com/DiscreteUniformDistribution.html)
	float encoderPowerWithoutAddedPrecision = ((float)(N - 1)) * ((float)(N + 1)) / 12.0;

	return encoderPowerWithoutAddedPrecision;
}

unsigned int SoftMapper::forecast(unsigned int numOutputs)
{
	return numOutputs;
}
