/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include "../CodeBench.h"
#include "../util/MTRand.h"

/**
 * \ingroup channels
 * \brief Generates fading coefficients for coherence-time based fading
 */
class CoherenceCoeffGenerator {
public:
	/**
	 * C'tor
	 * @param coherenceInterval: the number of symbols between re-randomization
	 * 		of the fading coefficient
	 */
	CoherenceCoeffGenerator(uint32_t coherenceInterval);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(uint32_t* const seed, int seedSize);

	/**
	 * @return the next fading coefficient
	 */
	FadingMagnitude next();

private:
	/**
	 * Re-randomizes the fading coefficient
	 */
	void randomizeCoefficient();

	// The coherence interval, in symbols
	const uint32_t m_coherenceInterval;

	// 1/sqrt(2) for randomization of fading coefficient
	const double m_sqrt2inv;

	// The random number generator
	MTRand m_random;

	// The current channel coefficient
	FadingMagnitude m_fadingCoeff;

	// Number of remaining symbols at the current coefficient before
	// re-randomization
	uint32_t m_remaining;
};
