/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../CodeBench.h"
#include <stdint.h>
#include <tr1/memory>
#include <vector>

/**
 * \ingroup mappers
 * \brief Interface of a mapper
 */
template<typename ChannelSymbol>
class IMapper {
public:
	typedef uint16_t InputType;
	typedef ChannelSymbol OutputType;
	typedef std::tr1::shared_ptr< IMapper<ChannelSymbol> > Ptr;

	/**
	 * Virtual d'tor
	 */
	virtual ~IMapper() {}

	/**
	 * Maps symbols to the (larger) precision, linearly
	 * @param symbols: the symbols to be mapped
	 */
	virtual void process(const std::vector<uint16_t>& inSymbols,
						 std::vector<ChannelSymbol>& outSymbols) = 0;

	/**
	 * @return the average signal power, assuming input is uniformly distributed
	 */
	virtual float getAveragePower() = 0;

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	virtual unsigned int forecast(unsigned int numOutputs) = 0;
};
