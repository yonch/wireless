/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/spinal/protocols/StridedProtocol.h"

#include <algorithm>

#include "codes/puncturing/StridedPuncturingSchedule.h"

StridedProtocol::StridedProtocol(unsigned int numPackets,
             	                 unsigned int spineLength,
             	                 unsigned int maxSymbols,
             	                 unsigned int numLastStepSymbolsPerPass,
             	                 unsigned int numRepetitions)
  : m_maxSymbols(maxSymbols),
    m_numSymbolsPerSubpass(8,
         ((spineLength - 1 + numLastStepSymbolsPerPass) / 8) * numRepetitions),
    m_lastSubpassInd(numPackets, 0),
    m_lastOutputNumSymbols(numPackets, 0),
	m_lastActualNumSymbols(numPackets, 0)
{
	// Correct m_numSymbolsPerSubpass if the number of symbols per full pass
	// is not a multiple of 8
	unsigned int remainder = (spineLength - 1 + numLastStepSymbolsPerPass) % 8;
	for(unsigned int i = 0; i < 8; i++) {
		if(StridedPuncturingSchedule::m_moduluOfSubpass[i] < remainder) {
			m_numSymbolsPerSubpass[i] += numRepetitions;
		}
	}
}

unsigned int StridedProtocol::numSymbolsNextDecode(unsigned int packetInd)
{
	if(m_lastActualNumSymbols[packetInd] >= m_maxSymbols) {
		// Last attempt tried the maximum, should quit now
		return 0;
	}

	unsigned int numSyms = m_lastOutputNumSymbols[packetInd];
	unsigned int subpass = m_lastSubpassInd[packetInd];

	// Advance the number of symbols until surpassing the last attempt
	while(numSyms <= m_lastActualNumSymbols[packetInd]) {
		numSyms += m_numSymbolsPerSubpass[subpass++];
		if(subpass == 8) {
			subpass = 0;
		}
	}

	numSyms = std::min(m_maxSymbols, numSyms);

	m_lastOutputNumSymbols[packetInd] = numSyms;
	m_lastSubpassInd[packetInd] = subpass;

	return numSyms;
}

void StridedProtocol::setResult(
		unsigned int packetInd,
		unsigned int numSymbols,
		bool isFinished)
{
	if(isFinished) {
		m_lastActualNumSymbols[packetInd] = m_maxSymbols;
	} else {
		m_lastActualNumSymbols[packetInd] = numSymbols;
	}
}

void StridedProtocol::resetPacket(unsigned int packetInd)
{
	m_lastSubpassInd[packetInd] = 0;
	m_lastOutputNumSymbols[packetInd] = 0;
	m_lastActualNumSymbols[packetInd] = 0;
}
