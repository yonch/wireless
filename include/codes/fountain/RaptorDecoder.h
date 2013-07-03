/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include <tr1/memory>

#include <itpp/comm/ldpc.h>
#include "../../CodeBench.h"
#include "../ILLRDecoder.h"
#include "LTDecoder.h"

/**
 * \ingroup fountain
 * \brief Decoder for Raptor codes
 */
class RaptorDecoder : public ILLRDecoder {
public:
	/**
	 * C'tor
	 * @param ldpcFilename the filename of the ITPP LDPC codec specification
	 * @param numVariables: the number of message bits transmitted using the
	 * 		LT code
	 * @param numLtIterations: number of belief propagation iterations in LT
	 * 		decoder
	 */
	RaptorDecoder(const std::string& ldpcFilename,
	              uint32_t numLtIterations);

	/**
	 * D'tor
	 */
	virtual ~RaptorDecoder() {}

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

private:
	// The generator object for the LDPC code
	itpp::LDPC_Generator_Systematic m_G;

	// The LDPC codec
	itpp::LDPC_Code m_ldpc;

	// The LT decoder
	LTDecoder m_lt;

	// a buffer for LLR outputs from the LT code
	std::vector<LLRValue> m_llrs;

	// a buffer for LLR input to the LDPC code
	itpp::vec m_itppLlrs;

	// a buffer for the decoded LDPC message
	itpp::bvec m_decodedBits;
};
