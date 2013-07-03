/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include <itpp/comm/ldpc.h>
#include "../../util/hashes/BitwiseXor.h"
#include "ParityEncoder.h"
#include "LTParityNeighborGenerator.h"
#include "../../CodeBench.h"


/**
 * \ingroup fountain
 * \brief Encoder for Raptor codes
 */
class RaptorEncoder
{
public:
	/**
	 * C'tor
	 * @param ldpcFilename the filename of the ITPP LDPC codec specification
	 **/
	RaptorEncoder(const std::string& ldpcFilename);

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
	/**
	 * Helper function for constructor
	 */
	static BitwiseXorSymbolFunction getSymbolFunction();

	/**
	 * Helper function for constructor
	 */
	static LTParityNeighborGenerator getNeighborGenerator(uint32_t codewordSize);

	// The generator object for the LDPC code
	itpp::LDPC_Generator_Systematic m_G;

	// The LDPC codec
	itpp::LDPC_Code m_ldpc;

	// The LT code
	ParityEncoder<BitwiseXorSymbolFunction, LTParityNeighborGenerator> m_lt;

	// The current packet, bit by bit
	itpp::bvec m_packetBits;

	// The LDPC codeword
	itpp::bvec m_encodedBits;

	// A temporary vector for the LDPC codeword
	std::vector<uint16_t> m_encodedVec;
};

