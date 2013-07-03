/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>

/**
 * \ingroup protocols
 * \brief Attempts a sequence of rates, to find a rate up to (1+delta) factor from the minimal
 *    possible rate for the channel
 *
 * A protocol that tries rates such that the spacing between rates is at most
 * 	   a given constant factor 'delta'. This means that if a system achieves
 * 	   a rate R, then the protocol will at the worst case try rate R * delta.
 */
class RateApproxProtocol {
public:
	/**
	 * @param numPackets: the number of packets the protocol keeps in memory in
	 * 		parallel
	 * @param maxSymbols: the maximum number of symbols to transmit
	 * @param delta: the approximation factor
	 * @param minSymbols: the minimum number of symbols to try
	 */
	RateApproxProtocol(unsigned int numPackets,
	                   unsigned int maxSymbols,
	                   float delta,
	                   uint32_t minSymbols);

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
	// The maximum number of symbols for protocol
	const unsigned int m_maxSymbols;

	// 1/delta, where delta is the approximation factor
	const float m_oneOverDelta;

	// The minimum number of symbols for protocol
	const unsigned int m_minSymbols;

	// for each packet, the number of symbols to attempt next time
	std::vector<unsigned int> m_nextNumSymbols;
};
