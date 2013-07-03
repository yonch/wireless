/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once


#include <vector>
#include "../CodeBench.h"
#include "../util/MTRand.h"

using std::vector;

/**
 * \ingroup channels
 * \brief Simulates the Binary Symmetric Channel
 */
class BscChannel
{
public:
	typedef Symbol InputType;
	typedef Symbol OutputType;

	/**
	 * C'tor
	 * @param p: probability of a flip (ie 1-prob(remain same))
	 */
	BscChannel(float p);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Adds noise to given symbols.
	 * bits are flipped with probability p.
	 */
	void process(const std::vector<Symbol>& inSymbols,
 	 	 	 	 std::vector<Symbol>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	// probability of error
	float m_p;

	// random generator
	MTRand m_random;
};
