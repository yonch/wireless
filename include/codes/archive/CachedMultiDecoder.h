/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include "../CodeBench.h"
#include "IDecoder.h"
#include "IMultiDecoder.h"

// forward declarations
template<typename ChannelSymbol> class CachedMultiDecoder;

// typedefs
typedef CachedMultiDecoder<Symbol> SymbolCachedMultiDecoder;


template<typename ChannelSymbol>
class CachedMultiDecoder : public IMultiDecoder<ChannelSymbol> {
public:
	/**
	 * Constructor
	 * @param decoder: the decoder that is used to decode one packet at a time
	 * @param numPackets: the number of packets the stateful decoder supports
	 * @param maxNumSymbolsPerPacket: the maximum number of passes this instance
	 * 		supports.
	 */
	CachedMultiDecoder(
			std::tr1::shared_ptr<IDecoder<ChannelSymbol> > decoder,
			unsigned int numPackets,
			unsigned int maxNumSymbolsPerPacket);

	/**
	 * Removes packet 'packetInd' from the decoder, so a different packet
	 * 		would be able to use the same packetInd.
	 */
	void resetPacket(unsigned int packetInd);

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with passInd == 0.
	 * @param packetInd: the packet whose pass is supplied
	 * @param firstSymbolToAdd: the first symbol's index in the symbols array
	 * @param lastSymbolToAdd: the last symbol's index in the symbols array
	 * @param symbols: the symbols of the pass
	 * @param symbolsSize: the number of symbols supplied.
	 */
	void addSymbols(
			unsigned int packetInd,
			const std::vector<ChannelSymbol>& symbols);

	/**
	 * Starts a decode for the given packetInd. The result is then read using
	 * 		getDecodeResult.
	 */
	void decode(unsigned int packetInd);

	/**
	 * @return the ML packet decoded by existing packetInd symbols.
	 * @note that a decode must be run before calling getDecodeResult.
	 */
	DecodeResult getDecodeResult(unsigned int packetInd);

private:
	/**
	 * Disallow copy c'tor & assignment operator
	 */
	CachedMultiDecoder(const CachedMultiDecoder&);
	CachedMultiDecoder& operator=(const CachedMultiDecoder&);

	// Decoder
	std::tr1::shared_ptr<IDecoder<ChannelSymbol> > m_decoder;

	// The number of packets that the instance supports
	unsigned int m_numPackets;

	// The number of cached passes per packet
	unsigned int m_maxNumSymbolsPerPacket;

	// The packets that were decoded
	std::vector<DecodeResult> m_decodedPackets;

	// Cached symbols
	std::vector < std::vector<ChannelSymbol> > m_symbols;

};

#include "CachedMultiDecoder.hh"
