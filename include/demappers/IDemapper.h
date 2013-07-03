/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../CodeBench.h"
#include <vector>
#include <tr1/memory>

/**
 * \ingroup demappers
 * \brief Interface for a demapper
 */
template<typename ChannelSymbol>
class IDemapper {
public:
	typedef LLRValue OutputType;
	typedef std::tr1::shared_ptr<IDemapper<ChannelSymbol> > Ptr;

	/**
	 * virtual d'tor
	 */
	virtual ~IDemapper() {}

	/**
	 * Demaps symbols to soft bit values
	 * @param symbols: the symbols to be mapped
	 * @param n0: channel noise power
	 * @param llrs: [out] the soft values
	 */
	virtual void process(const std::vector<ChannelSymbol>& symbols,
						 N0_t n0,
						 std::vector<LLRValue>& llrs) = 0;

	/**
	 * Demaps symbols to soft bit values, given different noise for each symbol
	 * @param symbols: the symbols to be mapped
	 * @param fadingMagnitude: the scaling factor that the symbols underwent
	 * 		when transmitted on the channel
	 * @param n0: channel noise power
	 * @param llrs: [out] the soft values
	 */
	virtual void process(const std::vector<ChannelSymbol>& symbols,
				 	   	 const std::vector<FadingMagnitude>& fadingMagnitude,
						 N0_t n0,
						 std::vector<LLRValue>& llrs) = 0;

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	virtual unsigned int forecast(unsigned int numOutputs) = 0;
};
