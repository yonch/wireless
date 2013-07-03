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
 * \brief Maps a bit sequence to a constellation point using a Gray code.
 */
class GrayMapper {
public:
	typedef uint16_t InputType;
	typedef Symbol OutputType;

	/**
	 * C'tor
	 * @param symbolSizeBits: the number of data bits each element contains
	 * @param precisionBits: the number of bits are supported by channel's
	 * 		quantization. Could be though of as the precision of the "ADC" that
	 * 		the receiver will have.
	 */
	GrayMapper(unsigned int symbolSizeBits,
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
	Symbol map(Symbol sym);

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
	// number of bits to take from inputs
	const unsigned int m_symbolSizeBits;

	// the number of bits the result should occupy
	const unsigned int m_precisionBits;

	// the amount of left shift necessary to achieve m_precisionBits
	const unsigned int m_leftShiftSize;

	// mask to only extract m_symbolSizeBits bits
	const uint16_t m_mask;
};

inline Symbol GrayMapper::map(Symbol sym) {
	sym = sym & m_mask;
	return ((sym ^ (sym >> 1)) << m_leftShiftSize);
}

