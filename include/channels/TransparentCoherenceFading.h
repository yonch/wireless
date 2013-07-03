/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include "../CodeBench.h"
#include "../util/MTRand.h"
#include "CoherenceFading.h"

/**
 * \ingroup channels
 * \brief Model of Rayleigh channel fading; receiver has no knowledge of fading coefficients.
 *
 * Adds noise with SNR like the CoherenceFading would do, but does not scale
 *     the symbol's magnitude, and does not include the fading strength info.
 */
template<typename ChannelSymbol>
class TransparentCoherenceFading {
public:
	typedef ChannelSymbol InputType;
	typedef ChannelSymbol OutputType;

	/**
	 * C'tor
	 * @param coherenceInterval: the number of symbols between re-randomization
	 * 		of the fading coefficient
	 * @param stddev: standard deviation of Gaussian channel noise, added after
	 *      fading
	 */
	TransparentCoherenceFading(uint32_t coherenceInterval, N0_t n0);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Fades and adds noise to given symbols.
	 */
	void process(const std::vector<ChannelSymbol>& inSymbols,
 	 	 	 	 std::vector<ChannelSymbol>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

	/**
	 * Transforms the input symbols like the observed symbols, without adding
	 *     noise. This is done for purposes of likelihood approximation
	 */
	void transform(const std::vector<InputType>& inSymbols,
				   const std::vector<OutputType>& observedSymbols,
 	 	 	 	   std::vector<OutputType>& outSymbols) const;

private:
	// Random number generator
	MTRand m_random;

	// Generator of coherence coefficients
	CoherenceCoeffGenerator m_coeffGenerator;

	// The standard deviation of the gaussian noise per I and Q
	const double m_stddevPerDimension;
};

#include "TransparentCoherenceFading.hh"
