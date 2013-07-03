/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include "IPuncturingSchedule.h"

/**
 * \ingroup puncturing
 * \brief Repeats each stream index of another puncturing schedule several (numRepetitions) times
 */
class RepeatingPuncturingSchedule : public IPuncturingSchedule
{
public:
	/**
	 * @param innerSchedule: puncturing schedule whose indices are repeated
	 * @param numRepetitions: the number of times each index in innerSchedule should be repeated
	 * 		before moving to the next symbol
	 */
	RepeatingPuncturingSchedule(IPuncturingSchedulePtr& innerSchedule,
	                          	unsigned int numRepetitions);

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
	// The schedule producing the spine values
	IPuncturingSchedulePtr m_innerSchedule;

	// number of repetitions of each spine value from the inner schedule
	const unsigned int m_numRepetitions;

	// The repeated spine value, last obtained from innerSchedule
	uint16_t m_cachedIndex;

	// Number of remaining repetitions for the cached index
	unsigned int m_remainingRepetitions;

	// A cache used by batchNext
	std::vector<uint16_t> m_tempCache;
};
