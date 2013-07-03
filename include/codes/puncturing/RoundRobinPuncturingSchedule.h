/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "IPuncturingSchedule.h"
#include <vector>
#include <stdint.h>

/**
 * \ingroup puncturing
 * \brief Gets symbols from index 0 to last index and wraps around
 *
 * Chooses streams in order 0,1,2.. and wraps around when reaching the highest stream
 */
class RoundRobinPuncturingSchedule : public IPuncturingSchedule {
public:
	/**
	 * c'tor
	 * @param numStreams: the number of streams to puncture. class will produce indices
	 *     0, 1, ... numStreams-1, 0, 1, ...
	 */
	RoundRobinPuncturingSchedule(uint32_t numStreams);

	/**
	 * @see IPuncturingSchedule
	 */
	virtual void reset();

	/**
	 * @see IPuncturingSchedule
	 */
	virtual void batchNext(unsigned int numSymbols,
	                       std::vector<uint16_t>& streamIndices);

private:
	// The total number of streams to puncture from
	const uint32_t m_numStreams;

	// The next stream where symbols should be taken from
	uint32_t m_nextStream;
};
