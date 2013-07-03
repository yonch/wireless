/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../../util/MTRand.h"

#define DIST_TABLE_SIZE 7

/**
 * \ingroup fountain
 * \brief Generates LT node distribution and selects which bits are XOR'd
 */
class LTParityNeighborGenerator
{
public:
	/**
	 * C'tor
	 * @param numSymbols the total number of possible neighbors
	 * @param streamSeed the seed used to generate the neighbors
	 **/
	 LTParityNeighborGenerator(
			 unsigned int numSymbols,
			 unsigned int streamSeed);

	/**
	 * Resets the generator state to beginning of neighbor list
	 **/
	void reset();

	/**
	 * Advances to next check node
	 */
	void nextNode();

	/**
	 * @return the number of neighbors in the neighbor list
	 **/
	unsigned int count();

	/**
	 * @return the next neighbor in the list
	 * @note neighbors are guaranteed to be unique
	 **/
	unsigned long next();

	/**
	 * @return true if there are more neighbors, false otherwise
	 * 
	 * It will return true before count() calls to next() had been performed.
	 **/
	bool hasMore();

private:
	/**
	 * Resets state to the given node seed
	 */
	void reset(unsigned int nodeSeed);


	// The total number of possible neighbors, given in the c'tor
	const unsigned int m_numSymbols;

	// The stream seed, used in reset()
	const unsigned int m_streamSeed;

	// Random generator, to generator node seeds from stream seed
	MTRand m_rand;

	// a mask to only capture the bits that are needed to represent the numbers in the range 0..numSymbols-1
	unsigned int m_symbolsBitMask;

	// The seed. This will not be modified through calls to count(), next() and hasMore()
	unsigned long m_nodeSeed;

	// A count of how many neighbors remain
	unsigned int m_remainingNeighbors;

	// The current numeric state of the generator, used to produce the next() neighbor
	unsigned long m_state;

	// adding constant
	unsigned long m_Cadd;

	// xorin constant
	unsigned long m_Cxor;

	static const unsigned long DEGREE_TABLE[DIST_TABLE_SIZE];
	static const unsigned long PROBABILITY_TABLE[DIST_TABLE_SIZE];

};

