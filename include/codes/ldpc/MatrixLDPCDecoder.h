/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include "MatrixLDPCCode.h"
#include "../IDecoder.h"
#include "../../OracleDetector.h"
#include "../../util/inference/bp/MessagePassingDecoder.h"

using namespace std;

/**
 * \ingroup ldpc
 * \brief Decoder for Quasi-Cyclic LDPC codes
 */
class MatrixLDPCDecoder
{
public:
	MatrixLDPCDecoder(const MatrixLDPCCode& code,
	                  unsigned int numIters);

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	 void reset();

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with symbol 0.
	 * @param symbols: the symbols to add
	 */
	 void add(const std::vector<float>& floats);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	 DecodeResult decode();

private:
	// Decoder
	MatrixLDPCCode m_code;

	// The number of iterations of message passing to perform
	unsigned int m_numIter;

	// The received codeword's log likelihoods (one per bit)
	std::vector<float> m_LLRs;
};
