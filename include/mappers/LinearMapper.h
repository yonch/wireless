/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include "../CodeBench.h"
#include <stdint.h>
#include <vector>

/**
 * \ingroup mappers
 * \brief Maps integers to fixed-precision symbols
 */
class LinearMapper {
public:
	typedef std::tr1::shared_ptr<LinearMapper> Ptr;
	typedef uint16_t InputType;
	typedef Symbol OutputType;

	/**
	 * C'tor
	 * @param symbolSizeBits: the number of data bits each element contains
	 * @param precisionBits: the number of bits are supported by channel's
	 * 		quantization. Could be though of as the precision of the "ADC" that
	 * 		the receiver will have.
	 */
	LinearMapper(unsigned int symbolSizeBits,
				 unsigned int precisionBits);

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
	// Number of bits from each input to take for symbol
	const unsigned int m_symbolSizeBits;

	// A mask to apply to the input to extract only relevant bits
	const uint16_t m_symbolMask;

	// The total number of bits to represent a symbol
	const unsigned int m_precisionBits;

	// The size of left shift needed to get m_symbolSizeBits to wanted precision
	const unsigned int m_leftShiftSize;
};
