/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

#include "../IEncoder.h"
#include "MatrixLDPCCode.h"
#include "WordWidthTransformer.h"

/**
 * \ingroup ldpc
 * \brief Encoder for Quasi-Cyclic LDPC codes
 * Matrix LDPC Encoder, like the one specified in 802.11n-2009 standard (section
 * 		20.3.11.6.4	and Annex R)
 */
class MatrixLDPCEncoder : public IEncoder {
public:
	/**
	 * @param Z: The dimension of the matrices
	 * @param matrix: The specification of parity checks, only the links on the
	 * 		code bits (the check bits all have the same pattern)
	 */
	MatrixLDPCEncoder(const MatrixLDPCCode& code,
					  unsigned int numBitsPerSymbol);

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	void setPacket(const std::string& packet);

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	void encode(unsigned int numSymbols,
	            std::vector<uint16_t>& outSymbols);

private:
	/**
	 * Rotates the given word to the right, given that it only has m_Z bits.
	 * @param x: word to be rotated
	 * @param amount: number of places to rotate right
	 */
	uint64_t rotr(uint64_t x, unsigned int amount);

	// Code specification
	MatrixLDPCCode m_code;

	// the number of coded bits per symbol
	const unsigned int m_numBitsPerSymbol;

	// the number of bits in a message
	const unsigned int m_numMessageBits;

	// A temporary buffer for a codeword, used in encode()
	std::vector<uint64_t> m_codeword;

	// transformer to transform the codeword to symbols
	WordWidthTransformer<uint64_t, uint16_t> m_symbolXformer;

	// The packets we are encoding
	std::string m_packet;
};
