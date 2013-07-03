/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <string>

using namespace std;

/**
 * \ingroup protocols
 * \brief Transmits exactly a single burst of symbols, numSymbols symbols long.
 */
class OneTryProtocol {
public:
	/**
	 * @param numPackets: the number of packets the protocol keeps in memory in
	 * 		parallel
	 * @param numSymbols: the number of symbols to transmit
	 */
	OneTryProtocol(unsigned int numPackets, unsigned int numSymbols);

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
	// The maximum number of passes for protocol
	unsigned int m_numSymbols;

	vector<unsigned int> m_nextDecode;
};
