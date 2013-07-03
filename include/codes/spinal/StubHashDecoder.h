/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include "../../CodeBench.h"
#include "../IDecoder.h"
using namespace std;


/**
 * \ingroup spinal
 * \brief A stub of a decoder, for testing purposes. The inputs to the decode function
 * are saved and can be accessed.
 */
class StubHashDecoder
{
public:
	typedef DecodeResult Result;

	StubHashDecoder(unsigned int numSymbolsPerPass);

	/**
	 * Stub for the decoder.
	 *  - numPasses is saved into this->lastCallNumPasses
	 *  - symbols are copied into this->lastCallSymbols
	 *
	 *  @return empty string
	 */
	Result decode(unsigned int numPasses,
				  const Symbol* symbols,
				  int symbolsSize);

	/**
	 * @return the number of symbols per pass.
	 * @note The construction of the puncturing means all passes have the same
	 * 		number of symbols.
	 */
	unsigned int numSymbolsPerPass();

	// member where numPasses will be saved
	unsigned int lastCallNumPasses;

	// member that will contain the symbols that are passed.
	vector<Symbol> lastCallSymbols;

private:
	unsigned int m_numSymbolsPerPass;

};
