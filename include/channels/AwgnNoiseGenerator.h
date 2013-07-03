/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once


/**
 * \ingroup channels
 * \brief Generic helper to add AWGN noise to symbols
 */
template<typename ChannelSymbol>
class AwgnNoiseGenerator {
public:
	/**
	 * @param random: a random number generator to use to generate noise
	 * @param x: channel symbol to add noise to
	 * @return symbol with AWGN noise with given standard deviation per
	 * 		dimension
	 */
	static ChannelSymbol noisify(MTRand& random,
									 ChannelSymbol x,
									 double stddevPerDimension);
};


// Default implementation throws exception
template<typename ChannelSymbol>
inline ChannelSymbol AwgnNoiseGenerator<ChannelSymbol>::noisify(
		MTRand & random, ChannelSymbol x, double stddevPerDimension)
{
	throw(std::runtime_error("unsupported type"));
}

// Specialization for Symbol
template<>
inline Symbol AwgnNoiseGenerator<Symbol>::noisify(
		MTRand& random,
		Symbol x,
		double stddevPerDimension)
{
	// make noise
	Symbol noise = lrint(random.randNorm(0, stddevPerDimension));

	// get sum
	Symbol sum = x + noise;

	// check for under/overflow and assign back
	if ((sum < x) && (noise > 0)) {
		// underflow
		return SYMBOL_MIN;
	} else if ((sum > x) && (noise < 0)) {
		// overflow
		return SYMBOL_MAX;
	} else {
		return sum;
	}
}

// Specialization for ComplexSymbol
template<>
inline ComplexSymbol AwgnNoiseGenerator<ComplexSymbol>::noisify(
		MTRand& random,
		ComplexSymbol x,
		double stddevPerDimension)
{
	// make noise
	ComplexSymbol noise(random.randNorm(0, stddevPerDimension),
						random.randNorm(0, stddevPerDimension));

	// get sum
	return x + noise;
}

// Specialization for SoftSymbol
template<>
inline SoftSymbol AwgnNoiseGenerator<SoftSymbol>::noisify(
		MTRand& random,
		SoftSymbol x,
		double stddevPerDimension)
{
	// make noise
	SoftSymbol noise(random.randNorm(0, stddevPerDimension));

	// get sum
	return x + noise;
}

// Partial specialization FadingSymbolTuples
template<typename T>
class AwgnNoiseGenerator<FadingSymbolTuple<T> > {
public:
	static FadingSymbolTuple<T> noisify(
			MTRand& random,
			FadingSymbolTuple<T> x,
			double stddevPerDimension)
	{
		// Use other template specializations to add noise to the symbol, leaving
		// the fading intact
		x.symbol = AwgnNoiseGenerator<T>::noisify(random, x.symbol, stddevPerDimension);

		return x;
	}
};
