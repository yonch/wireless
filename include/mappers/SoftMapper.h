/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../CodeBench.h"
#include <stdint.h>
#include <vector>

/**
 * \ingroup mappers
 * \brief Maps several bits of an integer to a float
 */
class SoftMapper {
public:
	typedef uint16_t InputType;
	typedef SoftSymbol OutputType;

	/**
	 * C'tor
	 * @param symbolSizeBits: the number of data bits each element contains
	 */
	SoftMapper(unsigned int symbolSizeBits);

	/**
	 * Maps symbols to the (larger) precision, linearly
	 * @param symbols: the symbols to be mapped
	 */
	void process(const std::vector<uint16_t>& inSymbols,
		 	 	 std::vector<SoftSymbol>& outSymbols);

	/**
	 * Maps a single symbol to the (larger) precision, linearly
	 */
	SoftSymbol map(uint16_t sym);

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
	const unsigned int m_symbolSizeBits;

	// A mask to apply to the input to extract only relevant bits
	const uint16_t m_symbolMask;
};
