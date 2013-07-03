/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdexcept>
#include <stdint.h>
#include <tr1/memory>
#include "IDecoder.h"

/**
 * \ingroup codes
 * \brief Interface for decoders for punctured codes
 */
template<typename ChannelSymbol>
class IMultiStreamDecoder {
public:
	typedef std::tr1::shared_ptr< IMultiStreamDecoder<ChannelSymbol> > Ptr;

	/**
	 * Virtual D'tor
	 */
	virtual ~IMultiStreamDecoder() {};

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset() = 0;

	/**
	 * Sets the symbols for the packet.
	 * @param streamIndices: for each symbol, which stream it came from
	 * @param symbols: the symbols to add
	 */
	virtual void add(const std::vector<uint16_t>& streamIndices,
	                   const std::vector<ChannelSymbol>& symbols,
	                   N0_t n0) = 0;

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode() = 0;
};
