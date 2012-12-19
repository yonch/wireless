/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename Encoder>
inline CachedEncoder<Encoder>::CachedEncoder(
										Encoder encoder,
										unsigned int numPackets,
										unsigned int numCachedSymbolsPerPacket)
    : m_encoder(encoder),
      m_numPackets(numPackets),
      m_numCachedSymbolsPerPacket(numCachedSymbolsPerPacket),
      m_packets(numPackets),
      m_hasPacket(numPackets, false),
      m_cachedBlockIndex(numPackets),
      m_cachedSymbols(new Symbol[  numPackets
                                 * numCachedSymbolsPerPacket])
{}

template<typename Encoder>
inline CachedEncoder<Encoder>::~CachedEncoder() {
	delete [] m_cachedSymbols;
}


template<typename Encoder>
inline void CachedEncoder<Encoder>::setPacket(
		unsigned int packetInd,
		const string& packet)
{
	if(hasPacket(packetInd)) {
		removePacket(packetInd);
	}

	// set packet
	m_packets[packetInd] = packet;
	// we have packet
	m_hasPacket[packetInd] = true;
	// the cached symbols are invalid
	m_cachedBlockIndex[packetInd] = -1;
}


template<typename Encoder>
inline void CachedEncoder<Encoder>::removePacket(unsigned int packetInd) {
	if (!hasPacket(packetInd)) {
		throw std::runtime_error("Cannot remove packet that does not exist");
	}

	m_hasPacket[packetInd] = false;
}


template<typename Encoder>
inline bool CachedEncoder<Encoder>::hasPacket(unsigned int packetInd) {
	return m_hasPacket[packetInd];
}


template<typename Encoder>
inline void CachedEncoder<Encoder>::getSymbols(
												unsigned int packetInd,
												unsigned int firstSymbolToEmit,
												unsigned int lastSymbolToEmit,
												std::vector<Symbol>& outSymbols)
{
	int numSymbolsToEmit = (int)lastSymbolToEmit - (int)firstSymbolToEmit + 1;

	// Make sure we have the packet
	if(!hasPacket(packetInd)) {
		throw std::runtime_error("Packet does not exist, cannot get symbols.");
	}
	// Make sure output array is big enough
	outSymbols.resize(0);
	outSymbols.reserve(numSymbolsToEmit);

	int block =  firstSymbolToEmit / m_numCachedSymbolsPerPacket;
	int offset = firstSymbolToEmit % m_numCachedSymbolsPerPacket;
	Symbol* cachedPtr = &m_cachedSymbols[packetInd * m_numCachedSymbolsPerPacket];

	while(numSymbolsToEmit > 0) {
		unsigned int numSymbols = min(m_numCachedSymbolsPerPacket - offset,
									  (unsigned int)numSymbolsToEmit);

		// If we need to get a new block, get it
		if(block != m_cachedBlockIndex[packetInd]) {
			getAndCacheSymbolBlock(packetInd, block);
		}

		outSymbols.insert(outSymbols.end(),
						  &cachedPtr[offset],
						  &cachedPtr[offset+numSymbols]);

		block++;
		offset = (offset + numSymbols) % m_numCachedSymbolsPerPacket;
		numSymbolsToEmit -= numSymbols;
	}
}

template<typename Encoder>
inline void CachedEncoder<Encoder>::getAndCacheSymbolBlock(
													unsigned int packetInd,
													unsigned int blockIndex) {
	// How many passes into the cache is packetInd's first pass?
	unsigned int packetOffset = (packetInd * m_numCachedSymbolsPerPacket);

	// Pointer to the first symbol in packetInd's cache portion
	Symbol* cacheLocation = &m_cachedSymbols[packetOffset];

	// Call encoder
	m_encoder.encode(m_packets[packetInd],
					 m_numCachedSymbolsPerPacket * blockIndex,
					 m_numCachedSymbolsPerPacket * (blockIndex+1) - 1,
					 cacheLocation,
					 m_numCachedSymbolsPerPacket);

	// Remember we have the right block
	m_cachedBlockIndex[packetInd] = blockIndex;
}

