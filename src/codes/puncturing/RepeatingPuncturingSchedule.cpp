/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/puncturing/RepeatingPuncturingSchedule.h"

#include <iostream>

RepeatingPuncturingSchedule::RepeatingPuncturingSchedule(
		IPuncturingSchedulePtr & innerSchedule,
		unsigned int numRepetitions)
  : m_innerSchedule(innerSchedule),
    m_numRepetitions(numRepetitions),
    m_remainingRepetitions(0),
    m_tempCache()
{}

void RepeatingPuncturingSchedule::reset() {
	m_innerSchedule->reset();
	m_remainingRepetitions = 0;
}

void RepeatingPuncturingSchedule::batchNext(
		unsigned int numSymbols,
		std::vector<uint16_t> & streamIndices)
{
	streamIndices.clear();
	streamIndices.resize(numSymbols);

	// the location into streamIndices next symbol is written to
	unsigned int nextLocation = 0;

	// First, use the cached index
	while((nextLocation < numSymbols) && (m_remainingRepetitions > 0)) {
		streamIndices[nextLocation++] = m_cachedIndex;
		m_remainingRepetitions--;
	}

	// Get more indices from inner schedule
	unsigned int numInner =	((numSymbols - nextLocation) + m_numRepetitions - 1) / m_numRepetitions;
	m_innerSchedule->batchNext(numInner, m_tempCache);

	// Put all indices except the last one
	for(int i = 0; i < ((int)numInner) - 1; i++) {
		for(unsigned int j = 0; j < m_numRepetitions; j++) {
			streamIndices[nextLocation++] = m_tempCache[i];
		}
	}

	// Put the last index
	m_cachedIndex = m_tempCache[numInner - 1];
	m_remainingRepetitions = m_numRepetitions;
	while(nextLocation < numSymbols) {
		streamIndices[nextLocation++] = m_cachedIndex;
		m_remainingRepetitions--;
	}
}
