/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include "IDecoder.h"

/**
 * \ingroup codes
 * \brief Decodes an interleaved code: de-interleaves, then decodes.
 */
template<typename Decoder>
class InterleavedDecoder : public ILLRDecoder {
public:
	/**
	 * C'tor
	 * @param decoder: the internal decoder
	 * @param interleaveSequence: the interleaving sequence
	 */
	InterleavedDecoder(const Decoder& decoder,
					   const std::vector<uint16_t>& interleaveSequence);

	/**
	 * D'tor
	 */
	virtual ~InterleavedDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset();

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with symbol 0.
	 * @param symbols: the symbols to add
	 */
	virtual void add(const std::vector<float>& symbols);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode();

private:
	// The decoder we're interleaving into
	Decoder m_decoder;

	// The interleaving sequence
	std::vector<uint16_t> m_interleavingSequence;

	// The next symbol we're expecting
	unsigned int m_next;
};

#include "InterleavedDecoder.hh"
