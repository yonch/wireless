/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include "../CodeBench.h"
#include "../util/MTRand.h"

using namespace std;

/**
 * \ingroup channels
 * \brief Simulates the Additive White Gaussian Channel
 */
template<typename ChannelSymbol>
class AwgnChannel
{
public:
	typedef ChannelSymbol InputType;
	typedef ChannelSymbol OutputType;

	/**
	 * C'tor
	 * @param n0 the noise power per *complex* (2D) symbol.
	 */
	AwgnChannel(N0_t n0);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Adds noise to given symbols.
	 * @note Noise is rounded to the nearest integer.
	 * @note Handles under/overflows by saturating the value
	 */
	void process(const std::vector<ChannelSymbol>& inSymbols,
 	 	 	 	 std::vector<ChannelSymbol>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

	/**
	 * @return the AWGN standard deviation per dimension
	 */
	double getStddevPerDimension();

	/**
	 * @return the channel symbol, after added noise
	 */
	ChannelSymbol noisify(ChannelSymbol x, double stddevPerDimension);

private:
	// The standard deviation of the gaussian noise per I and Q
	double m_stddevPerDimension;

	// The random number generator
	MTRand m_random;
};

typedef AwgnChannel<Symbol> SymbolAwgnChannel;
typedef AwgnChannel<ComplexSymbol> ComplexAwgnChannel;
typedef AwgnChannel<SoftSymbol> SoftAwgnChannel;
typedef AwgnChannel<FadingSymbol> FadingAwgnChannel;
typedef AwgnChannel<FadingComplexSymbol> FadingComplexAwgnChannel;

#include "AwgnChannel.hh"
