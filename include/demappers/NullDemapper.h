/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../CodeBench.h"
#include <vector>
using namespace std;

/**
 * \ingroup demappers
 * \brief Performs no transformation on the symbols
 */
class NullDemapper {
public:
	typedef Symbol OutputType;

	/**
	 * maps a group of symbols. NOP
	 */
	void process(const std::vector<Symbol>& inSymbols,
	             std::vector<Symbol>& outSymbols);

	/**
	 * Maps a single symbol. NOP
	 */
	Symbol map(Symbol sym);

	vector<int> createOutputVector() {
		std::vector<int> a;
		a.push_back(4);
		a.push_back(12412);
		return a;}
};
