/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include <boost/numeric/ublas/matrix.hpp>
#include "../CodeBench.h"
#include "../util/MTRand.h"

using namespace std;

class MimoChannel
{
public:
	typedef ComplexSymbol InputType;
	typedef ComplexSymbol OutputType;

	/**
	 * C'tor
	 * @param nTransmitter the number of antennas at the transmitter
	 * @param nReceiver the number of antennas at the receiver
	 * @param channelMatrix the channel matrix. It is nTransmitter x nReceiver
	 * 		components. It is given row by row.
	 */
	MimoChannel(unsigned int nTransmitter,
				unsigned int nReceiver,
				const std::vector<ComplexNumber>& channelMatrix);

	/**
	 * Seed the pseudo-random number generator
	 * 	Null operation on this class
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Adds noise to given symbols.
	 * @note Noise is rounded to the nearest integer.
	 * @note Handles under/overflows by saturating the value
	 */
	void process(const std::vector<ComplexSymbol>& inSymbols,
				 std::vector<ComplexSymbol>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	// number of antennas at the transmitter
	const unsigned int m_nTransmitter;

	// number of antennas at the receiver
	const unsigned int m_nReceiver;

	// The channel matrix, row by row
	//const vector<ComplexNumber>& m_channelMatrix;
	boost::numeric::ublas::matrix< ComplexSymbol > m_channelMatrix;
};



