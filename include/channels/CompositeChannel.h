/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <complex>
#include <stdint.h>
#include "../CodeBench.h"
#include "../util/MTRand.h"

/**
 * \ingroup channels
 * \brief Concatenates two channels, to make a new channel
 */
template<typename Channel1, typename Channel2>
class CompositeChannel
{
public:
	typedef typename Channel1::InputType InputType;
	typedef typename Channel2::OutputType OutputType;

	/**
	 * C'tor
	 */
	CompositeChannel(const Channel1& channel1,
					 const Channel2& channel2,
					 uint32_t numOutputSymbolsToReserve);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Transforms the given symbols through both channels
	 */
	void process(const std::vector<InputType>& inSymbols,
				 std::vector<OutputType>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:

	// The channels
	Channel1 m_channel1;
	Channel2 m_channel2;

	// Random number generator, to seed both channels
	MTRand m_random;

	// buffer to hold the outputs of channel1 before feeding into channel2
	std::vector<typename Channel1::OutputType> m_intermediateBuffer;
};

#include "CompositeChannel.hh"
