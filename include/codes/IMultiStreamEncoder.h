/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <tr1/memory>
#include "../CodeBench.h"

/**
 * \ingroup codes
 * \brief Interface for puncture-able encoders
 *
 * The IMultiStreamEncoder interface is suitable for codes where values can be generated from
 *     different "locations" or "streams". These kind of encoders can be punctured easily, by
 *     requesting outputs from specific streams.
 *
 * A IMultiStreamEncoder instance allows explicit access to the different streams. If combined with
 *     a puncturing strategy, the result is an encoder that produces a single stream (an IEncoder).
 */
class IMultiStreamEncoder {
public:
	typedef std::tr1::shared_ptr<IMultiStreamEncoder> Ptr;

	/**
	 * Virtual d'tor
	 */
	virtual ~IMultiStreamEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet) = 0;

	/**
	 * Encodes symbols from requested spine values
	 * @param streamIndices: the sequence of streams from which values should be emitted
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			const std::vector<uint16_t>& streamIndices,
			std::vector<uint16_t>& outSymbols) = 0;
};
