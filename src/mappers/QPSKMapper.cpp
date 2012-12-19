/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/QPSKMapper.h"

#include <itpp/base/vec.h>

void QPSKMapper::process(	const std::vector<uint16_t> & inSymbols,
							std::vector<ComplexSymbol> & outSymbols)
{
	// Convert inputs into a bvec
	itpp::bvec bits(inSymbols.size());
	for(unsigned int i = 0; i < inSymbols.size(); i++) {
		bits[i] = ((inSymbols[i] & 0x1) != 0);
	}

	// Modulate using ITPP
	itpp::cvec modulated = m_qpsk.modulate_bits(bits);

	// Fill into the output
	outSymbols.clear();
	outSymbols.resize(modulated.size());
	for(int i = 0; i < modulated.size(); i++) {
		outSymbols[i] = modulated[i];
	}
}

float QPSKMapper::getAveragePower() {
	return 1.0f;
}

unsigned int QPSKMapper::forecast(unsigned int numOutputs) {
	return (numOutputs * 2);
}

