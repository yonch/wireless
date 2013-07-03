/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <string>

using namespace std;

/**
 * \ingroup puncturing
 * \brief Spinal protocol, transmits one pass after another
 *
 * A sequential protocol transmits pass after another, until either maxPasses
 * 		is reached, or the code signals that the packet is complete (isFinished)
 */
template<typename PuncturingSchedule>
class SequentialProtocol {
public:
	/**
	 * @param numPackets: the number of packets the protocol keeps in memory in
	 * 		parallel
	 * @param numSymbols: the number of symbols to transmit
	 */
	SequentialProtocol(unsigned int numPackets,
	                   unsigned int maxPasses,
	                   const PuncturingSchedule& punc);

	/**
	 * @param packetInd: the packet referred to
	 * @return the number of symbols that the decoder should possess before
	 * 		the protocol makes the next choice
	 */
	unsigned int numSymbolsNextDecode(unsigned int packetInd);

	/**
	 * Update the protocol with the results of a decode attempt.
	 *
	 * The protocol bases its decision on how many passes to require next
	 * 		based on this information.
	 */
	void setResult(unsigned int packetInd,
	               unsigned int numSymbols,
	               bool isFinished);

	/**
	 * The packet is done; Clean up and prepare for next packet.
	 * @param packetInd: the index of the packet that is done.
	 */
	void resetPacket(unsigned int packetInd);

private:
	// puncturing schedule, used to determine how many symbols to transmit per
	// pass
	PuncturingSchedule m_punc;

	// The maximum number of passes for protocol
	unsigned int m_maxPasses;

	vector<unsigned int> m_nextNumPasses;
	vector<unsigned int> m_nextNumSymbols;
};

#include "SequentialProtocol.hh"
