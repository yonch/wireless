/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include "../CodeBench.h"
#include "IEncoder.h"

using namespace std;

template<typename Encoder>
class CachedEncoder : public IEncoder
{
public:

	/**
	 * Uses encoder as a black box to produce passes in batches, while providing
	 * 	  an interface that allows getting passes one-by-one, thereby reducing
	 *    the overhead of coding.
	 * @param encoder: the encoder to be used
	 * @param numPackets: the number of different packets that the encoder can
	 *     cache concurrently
	 * @param numCachedSymbolsPerPacket: the number of symbols to be produced in
	 * 	   batch. The symbols generated will always be aligned to blocks of
	 * 	   numCachedPassesPerPacket passes.
	 **/
	CachedEncoder(
			Encoder encoder,
			unsigned int numPackets,
			unsigned int numCachedSymbolsPerPacket);

	/**
	 * d'tor
	 */
	~CachedEncoder();

	/**
	 * Sets the packet to be encoded
	 * @param packetInd: the index of the packet (0 to numPackets-1)
	 * @param packet: the packet to be encoded
	 */
	void setPacket(unsigned int packetInd, const string& packet);

	/**
	 * Removes the packet from the cache
	 * Note that this might not actually free memory, in order to save on
	 *     new/delete overhead
	 * @param packetInd: the index of the packet to be removed
	 */
	void removePacket(unsigned int packetInd);

	/**
	 * @return True if packet exists, False otherwise.
	 * @param packetInd: the index of the packet queried.
	 */
	bool hasPacket(unsigned int packetInd);

	/**
	 * Copies the specified pass into outSymbols
	 * @param packetInd: the index of the packet whose symbols are requested
	 * @param firstSymbolToEmit: the index of the first symbol wanted
	 * @param lastSymbolToEmit: the index of the last symbol wanted
	 * @param outSymbols: [out] where symbols will be written
	 * @param outSymbolsSize: size of outSymbols array
	 */
	void getSymbols(
			unsigned int packetInd,
			unsigned int firstSymbolToEmit,
			unsigned int lastSymbolToEmit,
			std::vector<Symbol>& outSymbols);

private:
	/**
	 * Disallow copy c'tor & assignment operator
	 */
	CachedEncoder(const CachedEncoder&);
	CachedEncoder& operator=(const CachedEncoder&);

	/**
	 * Computes the symbol block for the given packet, and puts it into the
	 *  right location in the cache.
	 */
	void getAndCacheSymbolBlock(unsigned int packetInd, unsigned int blockIndex);

	// Encoder
	Encoder m_encoder;

	// The number of packets that the instance supports
	unsigned int m_numPackets;

	// The number of cached passes per packet
	unsigned int m_numCachedSymbolsPerPacket;

	// The packets we are encoding
	vector<string> m_packets;

	// Whether we have a specific packet
	vector<bool> m_hasPacket;
	// The offset of the pass block we are currently caching
	vector<int> m_cachedBlockIndex;

	// Cached symbols
	Symbol* m_cachedSymbols;
};

#include "CachedEncoder.hh"
