/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include <vector>
#include <itpp/comm/modulator.h>

#include "../CodeBench.h"
#include "IMapper.h"

/**
 * \ingroup mappers
 * \brief Maps bits to arbitrary QAM-2^k (using ITPP)
 */
class QamMapper : public IMapper<ComplexSymbol> {
public:
	/**
	 * C'tor
	 * @param k: this will be QAM-2^k
	 * @note k must be even
	 */
	QamMapper(uint32_t k);

	/**
	 * Virtual d'tor
	 */
	virtual ~QamMapper() {}

	/**
	 * Maps symbols to the (larger) precision, linearly
	 * @param symbols: the symbols to be mapped
	 */
	virtual void process(const std::vector<uint16_t>& inSymbols,
						 std::vector<ComplexSymbol>& outSymbols);

	/**
	 * @return the average signal power, assuming input is uniformly distributed
	 */
	virtual float getAveragePower();

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	virtual unsigned int forecast(unsigned int numOutputs);

private:
	// The k such that we are doing QAM-2^k
	const uint32_t m_k;

	// Qam modulator from ITPP
	itpp::QAM m_qam;
};
