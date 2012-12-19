/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/puncturing/RoundRobinPuncturingSchedule.h"

RoundRobinPuncturingSchedule::RoundRobinPuncturingSchedule(
		uint32_t numStreams)
  : m_numStreams(numStreams),
    m_nextStream(0)
{}

void RoundRobinPuncturingSchedule::reset() {
	m_nextStream = 0;
}

void RoundRobinPuncturingSchedule::batchNext(
		unsigned int numSymbols,
		std::vector<uint16_t>& streamIndices) {
	streamIndices.clear();
	streamIndices.reserve(numSymbols);
	for(uint32_t i = 0; i < numSymbols; i++) {
		// Add next stream to list
		streamIndices.push_back(m_nextStream);

		// Advance the next stream, wrap around.
		m_nextStream++;
		if(m_nextStream == m_numStreams) {
			m_nextStream = 0;
		}
	}
}
