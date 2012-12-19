/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>

template<typename PuncturingSchedule>
inline SequentialProtocol<PuncturingSchedule>::SequentialProtocol(
											unsigned int numPackets,
											unsigned int maxPasses,
											const PuncturingSchedule & punc)
  : m_punc(punc),
    m_maxPasses(maxPasses),
    m_nextNumPasses(numPackets, 1),
    m_nextNumSymbols(numPackets, m_punc.numSymbolsAfterPasses(1))
{
    if (maxPasses < 1) {
        throw std::runtime_error("this protocol requires at least 1 pass");
    }
}

template<typename PuncturingSchedule>
inline unsigned int SequentialProtocol<PuncturingSchedule>::numSymbolsNextDecode(
		unsigned int packetInd)
{
	return m_nextNumSymbols[packetInd];
}

template<typename PuncturingSchedule>
void SequentialProtocol<PuncturingSchedule>::setResult(
									unsigned int packetInd,
									unsigned int numSymbols,
									bool isFinished)
{
	if(numSymbols != m_nextNumSymbols[packetInd]) {
		throw (std::runtime_error("unexpected result from packet"));
	}

    if(isFinished || (m_nextNumPasses[packetInd] == m_maxPasses)) {
        m_nextNumSymbols[packetInd] = 0;
        m_nextNumPasses[packetInd] = 0;
    } else {
    	do {
			m_nextNumPasses[packetInd] += 1;
			m_nextNumSymbols[packetInd] =
					m_punc.numSymbolsAfterPasses(m_nextNumPasses[packetInd]);
    	} while(   (m_nextNumPasses[packetInd] <= m_maxPasses)
    			&& (m_nextNumSymbols[packetInd] == numSymbols));

        if(m_nextNumPasses[packetInd] > m_maxPasses) {
            m_nextNumSymbols[packetInd] = 0;
            m_nextNumPasses[packetInd] = 0;
        }
    }
}




template<typename PuncturingSchedule>
void SequentialProtocol<PuncturingSchedule>::resetPacket(unsigned int packetInd) {
	m_nextNumPasses[packetInd] = 1;
	m_nextNumSymbols[packetInd] =
			m_punc.numSymbolsAfterPasses(m_nextNumPasses[packetInd]);

}

