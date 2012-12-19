/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/puncturing/StaticPuncturingSchedule.h"

#include <stdexcept>

StaticPuncturingSchedule::StaticPuncturingSchedule()
  : m_nextIndex(0)
{}

void StaticPuncturingSchedule::reset() {
	m_nextIndex = 0;
}

void StaticPuncturingSchedule::batchNext(
		unsigned int numSymbols,
		std::vector<uint16_t> & streamIndices)
{
	// Clear before resize, so if reallocation is needed, copies are avoided
	streamIndices.clear();
	streamIndices.resize(numSymbols);

	for(unsigned int i = 0; i < numSymbols; i++) {
		streamIndices[i] = next();
	}
}

uint16_t StaticPuncturingSchedule::next() {
	if(m_nextIndex >= m_schedule.size()) {
		throw(std::runtime_error("Exceeded schedule size."));
	}

	return m_schedule[m_nextIndex++];
}

void StaticPuncturingSchedule::set(const std::vector<uint16_t> & schedule) {
	m_schedule = schedule;
}

