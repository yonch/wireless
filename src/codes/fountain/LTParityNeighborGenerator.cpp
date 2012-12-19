/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/fountain/LTParityNeighborGenerator.h"

#include <stdexcept>

// Numerical recipe's LCG (see http://en.wikipedia.org/wiki/Linear_congruential_generator )
#define CONST_A 1664525
#define CONST_C 1013904223

// This is the RAPTOR table from RFC5053 section 5.4.4.2
const unsigned long LTParityNeighborGenerator::PROBABILITY_TABLE[DIST_TABLE_SIZE] = {10241, 491582, 712794, 831695, 948446, 1032189, 1048576};
const unsigned long LTParityNeighborGenerator::DEGREE_TABLE[DIST_TABLE_SIZE] = {1,2,3,4,10,11,40};

LTParityNeighborGenerator::LTParityNeighborGenerator(
			unsigned int numSymbols,
			unsigned int streamSeed)
  : m_numSymbols(numSymbols),
    m_streamSeed(streamSeed),
    m_rand(streamSeed)
{
	// calculate the number of bits
	unsigned int symsDivided = numSymbols;
	unsigned int numBits;
	for (numBits = 0; numBits <= 32; numBits++) {
		if(symsDivided == 0) {
			break;
		} else {
			symsDivided = symsDivided >> 1;
		}
	}
	
	if (numBits >= 32) {
		throw; // we don't want numerical problems with the next statement
	}
	// set the symbols bitmask
	m_symbolsBitMask = (1 << numBits) - 1;

	// Get state ready to generate the first node
	nextNode();
}

void LTParityNeighborGenerator::reset() {
	m_rand.seed(m_streamSeed);
	nextNode();
}

void LTParityNeighborGenerator::nextNode() {
	reset(m_rand.randInt());
}

void LTParityNeighborGenerator::reset(unsigned int nodeSeed) {
	m_nodeSeed = nodeSeed;
	m_remainingNeighbors = count();
	m_state = (m_nodeSeed * CONST_A) + CONST_C;
	m_Cadd = (m_state >> 16) | (m_state << 16);
	m_state = (m_state * CONST_A) + CONST_C;
	m_Cxor = (m_state >> 10 ^ m_state << 10);
	m_state = (m_state * CONST_A) + CONST_C;
}

unsigned int LTParityNeighborGenerator::count() {
	// we use the raptor method in RFC5053 to generate degrees. This only uses 20 bits of random.
	unsigned int raptorRand = (m_nodeSeed >> 10) & ((1 << 20) - 1);

	// search for raptorRand in the PROBABILITY_TABLE (RFC5053's f[j])
	for (int i = 0; i < DIST_TABLE_SIZE; i++) {
		if(raptorRand < PROBABILITY_TABLE[i]) {
			return DEGREE_TABLE[i];
		}
	}

	// we never reach this point since the table is well formed, but we add this to remove the compiler warning.
	return DEGREE_TABLE[DIST_TABLE_SIZE-1];
}

unsigned long LTParityNeighborGenerator::next() {
	// make sure the generator is in a valid state
	if(m_remainingNeighbors == 0) {
		throw(std::runtime_error("No more neighbors available"));
	}

	// the LCG mod a power of two gives a permutation of values up until that power of two.
	// we use that trait to generate neighbours such that we don't have repetitions. We have
	// to dilute out the numbers outside the allowed range, until we reach a valid neighbor.
	unsigned long result = (((m_state + m_Cadd) ^ m_Cxor) & m_symbolsBitMask);
	while (result >= m_numSymbols) {
		m_state = (m_state * CONST_A) + CONST_C;
		result = (((m_state + m_Cadd) ^ m_Cxor) & m_symbolsBitMask);
	}

	// the state is always ready for derivation of a new neighbor. We keep it that way.
	m_state = (m_state * CONST_A) + CONST_C;

	// decrement the number of remaining neighbors
	m_remainingNeighbors -= 1;

	return result;
}


bool LTParityNeighborGenerator::hasMore() {
	return (m_remainingNeighbors > 0);
}
