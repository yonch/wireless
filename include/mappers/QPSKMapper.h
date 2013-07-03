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
 * \brief Maps bits to QPSK (using ITPP)
 */
class QPSKMapper : public IMapper<ComplexSymbol> {
public:
	/**
	 * Virtual d'tor
	 */
	virtual ~QPSKMapper() {}

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
	// QPSK modulator from ITPP
	itpp::QPSK m_qpsk;
};
