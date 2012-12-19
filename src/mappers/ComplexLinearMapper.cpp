/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/ComplexLinearMapper.h"

#include <stdexcept>

ComplexLinearMapper::ComplexLinearMapper(unsigned int numBitsPerDim)
  : m_numBitsPerDim(numBitsPerDim),
    m_symbolMask(((1 << m_numBitsPerDim)-1))
{
	if (m_numBitsPerDim > 8) {
		throw(std::runtime_error("Number of bits per dimension to large, max 8"));
	}
}

void ComplexLinearMapper::process(const std::vector<uint16_t>& inSymbols,
	 	 	 	 	 	   std::vector<ComplexSymbol>& outSymbols)
{
	unsigned int numSymbols = inSymbols.size();
	outSymbols.clear();
	outSymbols.resize(numSymbols);
	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = map(inSymbols[i]);
	}
}

ComplexSymbol ComplexLinearMapper::map(uint16_t sym)
{
	return ComplexSymbol((sym >> 0              ) & m_symbolMask,
						 (sym >> m_numBitsPerDim) & m_symbolMask);
}

float ComplexLinearMapper::getAveragePower() {
	// N = number of constellation points in single dimension
	unsigned int N = (1 << m_numBitsPerDim);

	// signal power is the second moment of uniform discrete distribution
	// around its mean (see http://mathworld.wolfram.com/DiscreteUniformDistribution.html)
	float oneDimSignalPower = ((float)(N - 1)) * ((float)(N + 1)) / 12.0;

	return (2.0 * oneDimSignalPower);
}

unsigned int ComplexLinearMapper::forecast(unsigned int numOutputs)
{
	return numOutputs;
}



