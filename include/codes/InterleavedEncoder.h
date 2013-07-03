/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include "IEncoder.h"

/**
 * \ingroup codes
 * \brief An encoder that encodes using a specified encoder, and then shuffles the
 * 	   output symbols according to the given permutation
 */
class InterleavedEncoder : public IEncoder
{
public:
	/**
	 * C'tor
	 * @param encoder: the inner encoder
	 * @param interleaveSequence: a vector of indices. the i'th element is the
	 * 		index from the encoder's output to take the i'th interleaved output.
	 * 		Note that repeating the same index and omitting indices is okay.
	 */
	InterleavedEncoder(IEncoderPtr& encoder,
	                   const std::vector<uint16_t>& interleaveSequence);

	/**
	 * d'tor
	 */
	virtual ~InterleavedEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet);

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int numSymbols,
			std::vector<uint16_t>& outSymbols);

private:
	// Inner encoder
	IEncoderPtr m_encoder;

	// Interleaving sequence
	const std::vector<uint16_t> m_interleavingSequence;

	// Highest index in the interleaving sequence
	const uint16_t m_maxIndex;

	// Encoded packet
	std::vector<uint16_t> m_encoded;

	// The next symbol to output
	unsigned int m_next;
};
