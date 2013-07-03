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
 * \brief Spinal protocol, suited for StridedPuncturingSchedule ("8-way puncturing").
 *
 * This protocol attempts to decode every time the StridedPuncturingSchedule
 *    finishes emitting a stride, and stops when reaching a maximum number
 *    of strides, or the code signals that the packet is complete (isFinished)
 */
class StridedProtocol {
public:
	/**
	 * @param numPackets: the number of packets the protocol keeps in memory in
	 * 		parallel
	 * @param spineLength: the number of spine values
	 * @param maxSymbols: the maximum number of symbols to transmit
	 * @param numLastStepSymbolsPerPass: the number of symbols to transmit from
	 * 		the last spine value
	 * @param numRepetitions: the number of symbols that are transmitted from
	 * 		the same spine value before moving on with the strided puncturing
	 * 		(ie repetition a la RepeatingPuncturingSchedule)
	 */
	StridedProtocol(unsigned int numPackets,
	                unsigned int spineLength,
	                unsigned int maxSymbols,
	                unsigned int numLastStepSymbolsPerPass,
	                unsigned int numRepetitions);

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
	const unsigned int m_maxSymbols;

	// For each sub-pass, the number of symbols to allow in that sub-pass
	vector<unsigned int> m_numSymbolsPerSubpass;

	// The number of passes modulo 8 in the last attempt
	vector<unsigned int> m_lastSubpassInd;

	// The last number of symbols returned from numSymbolsNextDecode
	vector<unsigned int> m_lastOutputNumSymbols;

	// The last number of symbols reported in setResult
	vector<unsigned int> m_lastActualNumSymbols;

};
