/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include "../CodeBench.h"
#include "../util/MTRand.h"
#include "CoherenceCoeffGenerator.h"

/**
 * \ingroup channels
 * \brief Model of Rayleigh channel fading; receiver knows exact fading coefficients.
 */
template<typename ChannelSymbol>
class CoherenceFading {
public:
	typedef ChannelSymbol InputType;
	typedef FadingSymbolTuple<ChannelSymbol> OutputType;

	/**
	 * C'tor
	 * @param coherenceInterval: the number of symbols between re-randomization
	 * 		of the fading coefficient
	 */
	CoherenceFading(uint32_t coherenceInterval);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(uint32_t* const seed, int seedSize);

	/**
	 * Fades given symbols.
	 */
	void process(const std::vector<ChannelSymbol>& inSymbols,
 	 	 	 	 std::vector<OutputType>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

	/**
	 * Transforms the input symbols like the observed symbols, without adding
	 *     noise. This is done for purposes of likelihood approximation
	 */
	void transform(const std::vector<ChannelSymbol>& inSymbols,
				   const std::vector<OutputType>& observedSymbols,
 	 	 	 	   std::vector<OutputType>& outSymbols) const;

private:
	// A fading coefficient generator
	CoherenceCoeffGenerator m_coeffGenerator;
};

typedef CoherenceFading<SoftSymbol> SoftCoherenceFading;
typedef CoherenceFading<ComplexSymbol> ComplexCoherenceFading;

#include "CoherenceFading.hh"
