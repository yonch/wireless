/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/LinearMapper.h"

#include <stdexcept>


LinearMapper::LinearMapper(unsigned int symbolSizeBits,
						   unsigned int precisionBits)
  : m_symbolSizeBits(symbolSizeBits),
    m_symbolMask(((1 << m_symbolSizeBits)-1)),
    m_precisionBits(precisionBits),
    m_leftShiftSize(precisionBits - symbolSizeBits)
{
	if (symbolSizeBits > 16) {
		throw(std::runtime_error("Symbol size to big, max 16"));
	}

	if(m_symbolSizeBits > m_precisionBits) {
		throw(std::runtime_error("Precision insufficient for non-lossy linear mapping"));
	}
}


void LinearMapper::process(const std::vector<uint16_t>& inSymbols,
	 	 	 	 	 	   std::vector<Symbol>& outSymbols)
{
	unsigned int numSymbols = inSymbols.size();
	outSymbols.clear();
	outSymbols.resize(numSymbols);
	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = map(inSymbols[i]);
	}
}

Symbol LinearMapper::map(uint16_t sym) {
	return ((sym & m_symbolMask) << m_leftShiftSize);
}

float LinearMapper::getAveragePower() {
	// N = number of constellation points
	unsigned int N = (1 << m_symbolSizeBits);

	// encoder power is the second moment of uniform discrete distribution
	// around its mean (see http://mathworld.wolfram.com/DiscreteUniformDistribution.html)
	float encoderPowerWithoutAddedPrecision = ((float)(N - 1)) * ((float)(N + 1)) / 12.0;

	// need to add precision. Since the average is over the signal squared,
	// we need to add square the precision
	unsigned long long precisionFactor = ((unsigned long long)1 << (2 * m_leftShiftSize));
	float encoderPower = encoderPowerWithoutAddedPrecision * precisionFactor;

	return encoderPower;
}

unsigned int LinearMapper::forecast(unsigned int numOutputs)
{
	return numOutputs;
}
