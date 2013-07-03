/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdexcept>
#include <tr1/memory>

#include "../CodeBench.h"
#include "DecodeResult.h"

/**
 * \ingroup codes
 * \brief Interface for a decoder
 */
template<typename ChannelSymbol>
class IDecoder {
public:
	typedef DecodeResult Result;
	typedef std::tr1::shared_ptr<IDecoder<ChannelSymbol> > Ptr;

	/**
	 * D'tor
	 */
	virtual ~IDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset() = 0;

	/**
	 * Adds more symbol information to the packet.
	 * 		Assumes the noise power is constant accross all symbols
	 *
	 * @param symbols: the symbols to add
	 */
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 N0_t n0) = 0;

	/**
	 * Adds more symbol information to the packet.
	 * 		Assumes a fading channel (where noise power is constant but
	 * 		received power changes).
	 *
	 * @param symbols: the symbols to add
	 * @param fadingMagnitude: the scaling factor that the symbols underwent
	 * 		when transmitted on the channel
	 * @param n0: channel noise power
	 */
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 const std::vector<FadingMagnitude>& fadingMagnitude,
					 N0_t n0) = 0;

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode() = 0;
};
