/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdexcept>
#include <stdint.h>
#include <tr1/memory>
#include "../IMultiStreamDecoder.h"

/**
 * \ingroup spinal
 * \brief Result of a spinal decode attempt
 */
struct SpinalDecodeResult : public DecodeResult {
	SpinalDecodeResult(const std::string& _packet, float _logProbError)
		: DecodeResult(_packet,_logProbError) {}
	SpinalDecodeResult() {};

	// The weights associated with the decode attempt.
	// There should be exactly 6 weights:
	// 	index 0: the total path weight of the most likely result
	// 	index 1: the edge       weight of the most likely result
	// 	index 2: the total path weight of the second most likely result
	// 	index 3: the edge       weight of the second most likely result
	// 	index 4: the total path weight of a random result from third..last
	// 	index 5: the edge       weight of a random result from third..last
	std::vector<float> weights;
};

/**
 * \ingroup spinal
 * \brief Decoder for spinal codes.
 */
template<typename ChannelSymbol>
class IHashDecoder : public IMultiStreamDecoder<ChannelSymbol> {
public:
	typedef std::tr1::shared_ptr< IHashDecoder<ChannelSymbol> > Ptr;

	/**
	 * @see IMultiStreamDecoder
	 * functions reset, add, decode
	 *****/

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 *
	 * This version returns an extended result
	 */
	virtual SpinalDecodeResult decodeExtended() = 0;
};
