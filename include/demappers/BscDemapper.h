/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../CodeBench.h"
#include <vector>

/**
 * \ingroup demappers
 * \brief Produces LLRs from bits distorted by the Binary Symmetric Channel
 */
class BscDemapper {
public:
	typedef float OutputType;

	/**
	 * C'tor
	 * @param flipProb: the probability (in [0,1]) of a bit flip
	 * 	   (ie 1-prob(remain same))
	 */
	BscDemapper(float flipProb);

	/**
	 * Demaps symbols to soft bit values
	 * @param symbols: the symbols to be mapped
	 * @param n0: ignored in this case.
	 * @param llrs: log-likelihood ratios
	 */
	void process(const std::vector<Symbol>& symbols,
			 	 N0_t n0,
				 std::vector<LLRValue>& llrs);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:

	// LLR of getting a 1 over getting a 0, given the bit, ie
	//   m_llr[i] = log( prob(1 given observed i) / prob(0 given obs. i) )
	float m_llr[2];
};
