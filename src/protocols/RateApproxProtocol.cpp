/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "protocols/RateApproxProtocol.h"

#include <algorithm>
#include <stdexcept>

RateApproxProtocol::RateApproxProtocol(
		unsigned int numPackets,
		unsigned int maxSymbols,
		float delta,
        uint32_t minSymbols)
  : m_maxSymbols(maxSymbols),
    m_oneOverDelta(1.0 / delta),
    m_minSymbols(minSymbols),
    m_nextNumSymbols(numPackets, minSymbols)
{}

unsigned int RateApproxProtocol::numSymbolsNextDecode(unsigned int packetInd)
{
	return m_nextNumSymbols[packetInd];
}

void RateApproxProtocol::setResult(
		unsigned int packetInd,
		unsigned int numSymbols,
		bool isFinished)
{
	if(numSymbols < m_nextNumSymbols[packetInd]) {
		throw(std::runtime_error("Last run used last symbols than instructed!"));
	}

	if(isFinished || (numSymbols >= m_maxSymbols)) {
		m_nextNumSymbols[packetInd] = 0;
	} else {
		// Otherwise, find the next number of symbols
		numSymbols = std::max((numSymbols + 1),
						   (unsigned int)(numSymbols * m_oneOverDelta));
		numSymbols = std::min(m_maxSymbols, numSymbols);
		m_nextNumSymbols[packetInd] = numSymbols;
	}
}

void RateApproxProtocol::resetPacket(unsigned int packetInd)
{
	m_nextNumSymbols[packetInd] = m_minSymbols;
}
