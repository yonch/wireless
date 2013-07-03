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
 * \brief Maps integers to complex numbers
 */
class ComplexLinearMapper {
public:
	typedef uint16_t InputType;
	typedef ComplexSymbol OutputType;

	/**
	 * C'tor
	 * @param numBitsPerDim: the number of data bits to take for each of the
	 * 		I and Q parts
	 */
	ComplexLinearMapper(unsigned int numBitsPerDim);

	/**
	 * Maps symbols to the complex domain.
	 * 	  LSB's taken for real part, next bits for imaginary
	 *
	 * @param inSymbols: the inputs to be mapped
	 * @param outSymbols: the mapped outputs
	 */
	void process(const std::vector<uint16_t>& inSymbols,
		 	 	 std::vector<ComplexSymbol>& outSymbols);

	/**
	 * Maps a single symbol to the complex domain
	 */
	ComplexSymbol map(uint16_t sym);

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
	// Number of bits from each input to take for each dimension
	const unsigned int m_numBitsPerDim;

	// A mask to apply to the input to extract only LSB bits for one dimension
	const uint16_t m_symbolMask;
};
