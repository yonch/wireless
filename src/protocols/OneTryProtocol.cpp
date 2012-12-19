/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "protocols/OneTryProtocol.h"

#include <stdexcept>

OneTryProtocol::OneTryProtocol(	unsigned int numPackets,
								unsigned int numSymbols)
  : m_numSymbols(numSymbols),
    m_nextDecode(numPackets, numSymbols)
{
    if (numSymbols < 1) {
        throw std::runtime_error("this protocol requires at least 1 symbol");
    }
}

unsigned int OneTryProtocol::numSymbolsNextDecode(unsigned int packetInd) {
	return m_nextDecode[packetInd];
}

void OneTryProtocol::setResult(	unsigned int packetInd,
									unsigned int numSymbols,
									bool isFinished)
{
	if(numSymbols != m_nextDecode[packetInd]) {
		throw (std::runtime_error("unexpected result from packet"));
	}

	// After one try, we're done
	m_nextDecode[packetInd] = 0;
}

void OneTryProtocol::resetPacket(unsigned int packetInd) {
	m_nextDecode[packetInd] = m_numSymbols;
}
