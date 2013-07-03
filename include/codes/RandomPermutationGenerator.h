/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../util/MTRand.h"

/**
 * \ingroup codes
 * \brief Generates pseudo-random permutation (given a seed)
 */
class RandomPermutationGenerator {
public:
	enum {SEED_SIZE_UINTS = 4};

	/**
	 * C'tor
	 * @param groupSize: the number of elements in the permutation
	 * @param seed: the seed for the random permutation generator. 
     *              seed MUST be SEED_SIZE_UINTS long.
	 */
	RandomPermutationGenerator(unsigned int groupSize,
							   unsigned int *const seed);

	/**
	 * Copy c'tor
	 */
	RandomPermutationGenerator(const RandomPermutationGenerator& other);

	/**
	 * D'tor
	 */
	~RandomPermutationGenerator();

	/**
	 * Resets the random permutation with a new seed
	 * @param seed: the seed for the random permutation generator. 
     *              seed MUST be SEED_SIZE_UINTS long.
	 */
	void reset(unsigned int *const seed);

	/**
	 * Retrieves the next value in the permutation. If the permutation has
	 * 	 already been fully explored, comes up with a new random permutation
	 * 	 and returns the first value.
	 */
	unsigned int next();

private:
	/**
	 * Disallow assignment operator
	 */
	RandomPermutationGenerator& operator=(const RandomPermutationGenerator&);

	// Random generator
	MTRand m_rand;

	// The number of elements in the permutation
	unsigned int m_groupSize;

	// The number of remaining values to derive from this permutation
	unsigned int m_numRemaining;

	// Shift values
	unsigned int* m_arr;
};
