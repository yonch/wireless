/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#include <math.h>
#include <stdexcept>

template<typename Mapper>
inline BitwiseDemapper<Mapper>::BitwiseDemapper(
											const Mapper& mapper,
											unsigned int numBitsPerSymbol)
  : m_mapper(mapper),
    m_numBitsPerSymbol(numBitsPerSymbol),
    m_numConstellationPoints(1 << numBitsPerSymbol),
    m_expConst(0)
{}



template<typename Mapper>
inline void BitwiseDemapper<Mapper>::process(const std::vector<ChannelSymbol>& symbols,
											 N0_t n0,
		 	 	 	 	 	 	 	 	 	 std::vector<float>& llrs)
{
	unsigned int symbolsSize = symbols.size();
	llrs.clear();
	llrs.resize(symbolsSize * m_numBitsPerSymbol);

	m_expConst = -0.5f / n0;

	// Note: we go from end to start, to enable in-place processing (although
	// in this case that would be a bit strange)
	for(int destInd = (symbolsSize * m_numBitsPerSymbol) - 1; destInd >= 0; destInd--) {
		llrs[destInd] = demap(symbols[destInd / m_numBitsPerSymbol],
								(destInd % m_numBitsPerSymbol));
	}
}

template<typename Mapper>
inline float BitwiseDemapper<Mapper>::demap(ChannelSymbol sym,
											unsigned int bitNum)
{
	// probability of bit being 0 or 1
	float prob[2];
	prob[0] = prob[1] = 0;

	for (uint16_t x = 0; x < (uint16_t)m_numConstellationPoints; x++) {
		long long delta = (sym - m_mapper.map(x));
		prob[(x >> bitNum) & 0x1] += expf(m_expConst * (float)(delta * delta));
	}

	return logf(prob[0] / prob[1]);
}

template<typename Mapper>
inline unsigned int BitwiseDemapper<Mapper>::forecast(unsigned int numOutputs)
{
	return (numOutputs + m_numBitsPerSymbol - 1) / m_numBitsPerSymbol;
}
