/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/spinal/StubHashDecoder.h"

StubHashDecoder::StubHashDecoder(unsigned int numSymbolsPerPass)
  : m_numSymbolsPerPass(numSymbolsPerPass)
{}

DecodeResult StubHashDecoder::decode(
		unsigned int numPasses, const Symbol *symbols, int symbolsSize)
{
	lastCallNumPasses = numPasses;

	lastCallSymbols.clear();
	lastCallSymbols.reserve(symbolsSize);
	for (int i = 0; i < symbolsSize; i++) {
		lastCallSymbols.push_back(symbols[i]);
	}

	return DecodeResult();
}


unsigned int StubHashDecoder::numSymbolsPerPass() {
	return m_numSymbolsPerPass;
}

