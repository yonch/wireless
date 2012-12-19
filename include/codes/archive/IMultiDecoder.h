/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <tr1/memory>
#include "IDecoder.h"

template<typename ChannelSymbol>
class IMultiDecoder {
public:
	// DecodeResult is defined in IDecoder
	typedef DecodeResult Result;

	/**
	 * D'tor
	 */
	virtual ~IMultiDecoder() {}

	/**
	 * Removes packet 'packetInd' from the decoder, so a different packet
	 * 		would be able to use the same packetInd.
	 */
	virtual void resetPacket(unsigned int packetInd) = 0;

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with passInd == 0.
	 * @param packetInd: the packet whose pass is supplied
	 * @param symbols: the symbols of the pass
	 */
	virtual void addSymbols(
			unsigned int packetInd,
			const std::vector<ChannelSymbol>& symbols) = 0;

	/**
	 * Starts a decode for the given packetInd. The result is then read using
	 * 		getDecodeResult.
	 */
	virtual void decode(unsigned int packetInd) = 0;

	/**
	 * @return the ML packet decoded by existing packetInd symbols.
	 * @note that a decode must be run before calling getDecodeResult.
	 */
	virtual DecodeResult getDecodeResult(unsigned int packetInd) = 0;
};
