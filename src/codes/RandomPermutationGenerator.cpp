/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/RandomPermutationGenerator.h"
#include <string.h> // memcpy

RandomPermutationGenerator::RandomPermutationGenerator(
									unsigned int groupSize,
									unsigned int *const seed)
  : m_rand(seed, SEED_SIZE_UINTS),
	m_groupSize(groupSize),
	m_numRemaining(groupSize),
	m_arr(new unsigned int[groupSize])
{
	for(unsigned int i = 0; i < groupSize; i++) {
		m_arr[i] = i;
	}
}

RandomPermutationGenerator::RandomPermutationGenerator(
										const RandomPermutationGenerator& other)
  : m_rand(other.m_rand),
    m_groupSize(other.m_groupSize),
    m_numRemaining(other.m_numRemaining),
    m_arr(new unsigned int[m_groupSize])
{
	memcpy(m_arr, other.m_arr, m_groupSize * sizeof(unsigned int));
}


RandomPermutationGenerator::~RandomPermutationGenerator() {
	delete[] m_arr;
}

void RandomPermutationGenerator::reset(unsigned int *const seed) {
	m_rand.seed(seed, SEED_SIZE_UINTS);
	for(unsigned int i = 0; i < m_groupSize; i++) {
		m_arr[i] = i;
	}
	m_numRemaining = m_groupSize;
}

unsigned int RandomPermutationGenerator::next() {
	if(m_numRemaining == 1) {
		m_numRemaining = m_groupSize;
		return m_arr[0];
	} else {
		m_numRemaining--;
		unsigned int swapDest = m_rand.randInt(m_numRemaining);
		unsigned int tmp = m_arr[swapDest];
		m_arr[swapDest] = m_arr[m_numRemaining];
		m_arr[m_numRemaining] = tmp;

		return tmp;
	}
}
