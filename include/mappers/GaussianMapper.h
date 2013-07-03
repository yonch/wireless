/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include <vector>
#include "../CodeBench.h"

/**
 * \ingroup mappers
 * \brief Maps integers to symbols distributed like a truncated Gaussian
 */
class GaussianMapper {
public:
	typedef uint16_t InputType;
	typedef Symbol OutputType;

	/**
	 * C'tor
	 * @param inputNumBits: the number of data bits each input contains
	 * @param precisionBits: the number of bits are supported by channel's
	 * 		quantization. Could be though of as the precision of the "ADC" that
	 * 		the receiver will have. This is the range in which values from the
	 * 		truncated Gaussian will be.
	 * @param numStandardDevs the number of standard deviations around the mean
	 * 		that will fit into the truncated region (the region dictated by
	 * 		precisionBits). The full truncated region will contain
	 * 		'numStandardDevs' on both sides of the mean.
	 */
	GaussianMapper(unsigned int inputNumBits,
				 unsigned int precisionBits,
				 float numStandardDevs);

	/**
	 * Copy c'tor
	 */
	GaussianMapper(const GaussianMapper& other);

	/**
	 * D'tor
	 */
	~GaussianMapper();

	/**
	 * Maps symbols to the (larger) precision, linearly
	 * @param symbols: the symbols to be mapped
	 */
	void process(const std::vector<uint16_t>& inSymbols,
				 std::vector<Symbol>& outSymbols);

	/**
	 * Maps a single symbol to the (larger) precision, linearly
	 */
	Symbol map(uint16_t sym);

	/**
	 * @return the average signal power, assuming input is uniformly distributed
	 */
	float getAveragePower();

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	/**
	 * Disallow assignment operator
	 */
	GaussianMapper& operator=(const GaussianMapper& rhs);

	// The value of the highest input symbol, used to avoid access outside
	// the table
	uint16_t m_highestInput;

	// The table used to map each input to an output symbol
	Symbol* m_mappingTable;

	// The variance of the output symbols, given uniform inputs.
	float m_variance;
};
