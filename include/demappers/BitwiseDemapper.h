/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

#include "../CodeBench.h"

// forward declarations
template<typename Mapper> class BitwiseDemapper;
class GrayMapper;

// typedefs
typedef BitwiseDemapper<GrayMapper> GrayDemapper;

/**
 * \ingroup demappers
 * \brief Demaps constellation maps by enumerating on all constellation points, computing each
 *    one's likelihood.
 */
template<typename Mapper>
class BitwiseDemapper {
public:
	typedef float OutputType;
	typedef typename Mapper::OutputType ChannelSymbol;

	BitwiseDemapper(const Mapper& mapper,
					unsigned int numBitsPerSymbol);

	/**
	 * Demaps symbols to soft bit values
	 * @param symbols: the symbols to be mapped
	 * @param floats: the soft values
	 */
	void process(const std::vector<ChannelSymbol>& symbols,
				 N0_t n0,
				 std::vector<float>& llrs);


	/**
	 * Soft demaps a specific bit from the symbol
	 * @param sym symbol to be demapped
	 * @param bitNum the bit to extract soft information for
	 * @return the LLR (log likelihood ratio) of the wanted bit
	 */
	float demap(ChannelSymbol sym, unsigned int bitNum);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	// Mapper, used to map bits to get constellation points
	Mapper m_mapper;

	// the number of bits encoded in each symbol
	unsigned int m_numBitsPerSymbol;

	// Number of constellation points to find score from
	unsigned int m_numConstellationPoints;

	// -1 / (2 * sigma^2), which is the term used when calculating the
	// likelihood of a the symbol given the constellation point
	float m_expConst;
};

#include "BitwiseDemapper.hh"
