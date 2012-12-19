/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/EvenModulator.h"

#define MIN(x,y) ((x < y) ? (x) : (y))

EvenModulator::EvenModulator(unsigned int numAxisBits) 
	: m_numAxisBits(numAxisBits),
	  m_delta(2.0f / (1 << numAxisBits))
{}

void EvenModulator::modulate(const vector<bool> &data, vector<float>& iValues, vector<float>& qValues) {
	unsigned int dataLen = (unsigned int)data.size();
	unsigned int numSymbols = (dataLen + 2*m_numAxisBits - 1) / (2 * m_numAxisBits);
	iValues.reserve(numSymbols);
	qValues.reserve(numSymbols);

	for (unsigned int sym = 0; sym < numSymbols - 1; sym++) {
		iValues.push_back(-1.0f + (m_delta * getBits(data, 
													MIN((2*sym + 0) * m_numAxisBits, dataLen), 
													MIN((2*sym + 1) * m_numAxisBits, dataLen))));
		qValues.push_back(-1.0f + (m_delta * getBits(data, 
													MIN((2*sym + 1) * m_numAxisBits, dataLen), 
													MIN((2*sym + 2) * m_numAxisBits, dataLen))));
	}
}

unsigned int EvenModulator::getBits(const vector<bool>& data, unsigned int startIndex, unsigned int endIndex) {
	unsigned int res = 0;
	for(unsigned int i = startIndex; i < endIndex; i++) {
		res = (res << 1) | ((bool)data[i] & 0x1);
	}
	return res;
}

