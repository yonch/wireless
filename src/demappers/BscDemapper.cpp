/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "demappers/BscDemapper.h"

#include <math.h>
#include <stdexcept>

BscDemapper::BscDemapper(float flipProb)
{
	m_llr[1] = (logf(flipProb) - logf(1-flipProb));
	m_llr[0] = -m_llr[1];
}



void BscDemapper::process(const std::vector<Symbol>& symbols,
						  N0_t n0,
		 	 	 	 	  std::vector<float>& llrs)
{
	unsigned int symbolsSize = symbols.size();
	llrs.clear();
	llrs.resize(symbolsSize);
	for(unsigned int i = 0; i < symbolsSize; i++) {
		llrs[i] = m_llr[symbols[i] & 0x1];
	}
}

unsigned int BscDemapper::forecast(unsigned int numOutputs)
{
	return numOutputs;
}
