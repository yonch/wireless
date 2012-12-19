/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/GaussianMapper.h"

#include <string.h> // for memcpy
#include <math.h>
#include <assert.h>
#include <iostream>
#include "mappers/TruncatedNormalDistribution.h"

GaussianMapper::GaussianMapper(
		unsigned int inputNumBits,
		unsigned int precisionBits,
		float numStandardDevs)
  : m_highestInput((1 << inputNumBits) - 1),
    m_mappingTable(new Symbol[ (1 << inputNumBits) ])
{
	// Precompute mapping table:
	// 1. Get the distribution instance
	TruncatedNormalDistribution dist(-numStandardDevs, numStandardDevs);

	// 2. Divide the range of the distribution to equally weighted parts
	double pIncrement = 1.0 / (1 << inputNumBits);
	double p = pIncrement / 2;

	// 3. Calculate how much the result is scaled to obtain wanted precision
	float smallestValue = dist.ppf(p);
	float range = 2 * (-smallestValue);
	unsigned int highestOutput = ((1 << precisionBits) - 1);
	float scaleFactor = highestOutput / range;

	// 4. Initialize variance variable, for computation to follow
	m_variance = 0.0;
	float mean = float(highestOutput) / 2;

	// Calculate scaled results, together with variance
	for(unsigned int sym = 0; sym < (1 << (inputNumBits - 1)); sym++) {
		float point = (dist.ppf(p) - smallestValue) * scaleFactor;
		Symbol symPoint = roundf(point);
		m_mappingTable[sym] = symPoint;
		m_mappingTable[m_highestInput - sym] = highestOutput - symPoint;
		m_variance += (symPoint - mean) * (symPoint - mean);

		p += pIncrement;
	}

	// Take variance's expected value
	m_variance /= (1 << (inputNumBits - 1));
}

GaussianMapper::GaussianMapper(const GaussianMapper & other)
  : m_highestInput(other.m_highestInput),
    m_mappingTable(new Symbol[ m_highestInput + 1 ])
{
	memcpy(m_mappingTable,
		   other.m_mappingTable,
		   (m_highestInput + 1) * sizeof(Symbol));
}


GaussianMapper::~GaussianMapper() {
	delete [] m_mappingTable;
}

void GaussianMapper::process(const std::vector<uint16_t>& inSymbols,
		 	 	 	 	 	 std::vector<Symbol>& outSymbols)
{
	unsigned int numSymbols = inSymbols.size();
	outSymbols.clear();
	outSymbols.resize(numSymbols);
	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = map(inSymbols[i]);
	}
}

Symbol GaussianMapper::map(uint16_t sym) {
	assert(sym >= 0);
	return m_mappingTable[sym & m_highestInput];
}

float GaussianMapper::getAveragePower() {
	return m_variance;
}

unsigned int GaussianMapper::forecast(unsigned int numOutputs)
{
	return numOutputs;
}



