/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include <tr1/memory>

#include "../ILLRDecoder.h"
#include "LTParityNeighborGenerator.h"

/**
 * \ingroup fountain
 * \brief Decoder for LT codes
 */
class LTDecoder : public ILLRDecoder {
public:
	/**
	 * C'tor
	 * @param numVariables: the number of message bits transmitted using the
	 * 		LT code
	 * @param llrBufferSize: The initial size of memory allocation for the llrs
	 * 		(given in #llr values)
	 * @param numIterations: number of belief propagation iterations to perform
	 */
	LTDecoder(uint32_t numVariables, uint32_t llrBufferSize,
	          uint32_t numIterations);

	/**
	 * D'tor
	 */
	virtual ~LTDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset();

	/**
	 * Adds log likelihood information. This decoder only supports contiguous
	 * 		likelihoods, starting with bit 0.
	 * @param llrs: the log-likelihood ratios to add
	 */
	virtual void add(const std::vector<LLRValue>& llrs);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode();

	/**
	 * Performs a soft decode, returning the LLR of variables
	 */
	virtual void softDecode(std::vector<LLRValue>& llrs);

private:
	// number of variable nodes
	const uint32_t m_numVariables;

	// number of iterations
	const uint32_t m_numIterations;

	// llrs
	std::vector<LLRValue> m_llrs;

	// Neighbor generator
	LTParityNeighborGenerator m_neighborGenerator;
};
