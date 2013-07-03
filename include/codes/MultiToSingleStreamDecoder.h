/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <tr1/memory>
#include "../CodeBench.h"
#include "IDecoder.h"
#include "IMultiStreamDecoder.h"


/**
 * \ingroup codes
 * \brief Decodes a punctured code by rearranging received symbols according to where they were
 *   generated
 */
template<typename ChannelSymbol>
class MultiToSingleStreamDecoder : public IDecoder<ChannelSymbol> {
public:
	typedef std::tr1::shared_ptr< MultiToSingleStreamDecoder<ChannelSymbol> > Ptr;

	/**
	 * C'tor
	 */
	MultiToSingleStreamDecoder(typename IMultiStreamDecoder<ChannelSymbol>::Ptr& decoder,
	                 	 	 	   IPuncturingSchedulePtr& puncturing);

	/**
	 * @see IDecoder
	 */
	virtual void reset();
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 N0_t n0);
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 const std::vector<FadingMagnitude>& fadingMagnitude,
					 N0_t n0);
	virtual DecodeResult decode();

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 *
	 * This version returns an extended result
	 */
	//SpinalDecodeResult decodeExtended();

private:
	// The decoder
	typename IMultiStreamDecoder<ChannelSymbol>::Ptr m_decoder;

	// The puncturing schedule
	IPuncturingSchedulePtr m_puncturing;

	// Sequence of streams where symbols come from
	std::vector<uint16_t> m_streamIndicesWorkArr;
};

#include "MultiToSingleStreamDecoder.hh"
