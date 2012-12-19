/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#include "codes/puncturing/StridedPuncturingSchedule.h"


const unsigned int StridedPuncturingSchedule::m_moduluOfSubpass[8] = {4,0,6,2,7,3,5,1};

StridedPuncturingSchedule::StridedPuncturingSchedule(
										unsigned int numStreams,
										unsigned int numLastStreamSymbolsPerPass)
	: m_numStreams(numStreams),
	  m_numLastStreamSymbolsPerFullPass(numLastStreamSymbolsPerPass),
	  m_numSymbolsPerFullPass(numStreams + numLastStreamSymbolsPerPass - 1)
{
	// Calculate offsets to start from
	m_offsets.reserve(8);
	for(unsigned int i = 0; i < 8; i++) {
		if(m_moduluOfSubpass[i] < m_numSymbolsPerFullPass) {
			m_offsets.push_back(m_moduluOfSubpass[i]);
		}
	}

	reset();
}


void StridedPuncturingSchedule::reset()
{
	m_nextStreamIndex = m_offsets[0];
	m_currentSubpassIndex = 0;
}


void StridedPuncturingSchedule::batchNext(
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


uint16_t StridedPuncturingSchedule::next()
{
	unsigned int ret = m_nextStreamIndex;

	// Advance m_nextSpineValue to the next spine value
	m_nextStreamIndex += 8;
	if(m_nextStreamIndex >= m_numSymbolsPerFullPass) {
		// Advance to next modulu
		m_currentSubpassIndex++;
		if(m_currentSubpassIndex == m_offsets.size()) {
			m_currentSubpassIndex = 0;
		}

		m_nextStreamIndex = m_offsets[m_currentSubpassIndex];
	}

	// Return the right spine value
	if(ret < m_numStreams - 1) {
		// We are in a spine value in the middle, out put the spine value
		return ret;
	} else {
		// We are at the last spine value, output the last spine value
		return m_numStreams - 1;
	}
}
