/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/QamMapper.h"

#include <itpp/base/vec.h>
#include <stdexcept>

QamMapper::QamMapper(uint32_t k)
  : m_k(k),
    m_qam(1 << m_k)
{}

void QamMapper::process(	const std::vector<uint16_t> & inSymbols,
							std::vector<ComplexSymbol> & outSymbols)
{
	if (inSymbols.size() % m_k != 0) {
		throw(std::runtime_error("number of input bits must be a multiple of k to modulate in QAM-2^k"));
	}

	// Convert inputs into a bvec
	itpp::bvec bits(inSymbols.size());
	for(unsigned int i = 0; i < inSymbols.size(); i++) {
		bits[i] = ((inSymbols[i] & 0x1) != 0);
	}

	// Modulate using ITPP
	itpp::cvec modulated = m_qam.modulate_bits(bits);

	// Fill into the output
	outSymbols.clear();
	outSymbols.resize(modulated.size());
	for(int i = 0; i < modulated.size(); i++) {
		outSymbols[i] = modulated[i];
	}
}

float QamMapper::getAveragePower() {
	return 1.0f;
}


unsigned int QamMapper::forecast(unsigned int numOutputs) {
	return (numOutputs * m_k);
}

