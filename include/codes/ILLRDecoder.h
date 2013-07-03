/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <tr1/memory>

#include "../CodeBench.h"
#include "DecodeResult.h"

/**
 * \ingroup codes
 * \brief A decoder that uses Log Likelihood Ratio estimates of bits as its information
 */
class ILLRDecoder {
public:
	typedef DecodeResult Result;
	typedef std::tr1::shared_ptr<ILLRDecoder> Ptr;

	/**
	 * D'tor
	 */
	virtual ~ILLRDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset() = 0;

	/**
	 * Adds log likelihood information. This decoder only supports contiguous
	 * 		likelihoods, starting with bit 0.
	 * @param llrs: the log-likelihood ratios to add
	 */
	virtual void add(const std::vector<LLRValue>& llrs) = 0;

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode() = 0;
};
