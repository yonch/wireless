/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <itpp/itbase.h>
#include <itpp/comm/turbo.h>

#include "../ILLRDecoder.h"
#include "../InterleavedDecoder.h"

/**
 * \ingroup turbo
 * \brief Turbo decoder
 */
class TurboDecoder : public ILLRDecoder {
public:
	TurboDecoder(uint32_t messageLength);

	/**
	 * D'tor
	 */
	virtual ~TurboDecoder() {}

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
	 * Adds the LLR to the specific bit.
	 *    Can be used to implement decoding for punctured or interleaved
	 *    sequences
	 * @param location: the bit of the original turbo encoder that the LLR
	 * 		corresponds to
	 * @param llr: the log-likelihood-ratio value
	 */
	void addLLR(unsigned int location, float llr);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode();

private:
	itpp::Turbo_Codec m_codec;

	// Vector where log likelihood estimates are saved
	itpp::vec m_llr;

	// Next symbol to be added using add()
	unsigned int m_next;
};

typedef InterleavedDecoder<TurboDecoder> InterleavedTurboDecoder;
