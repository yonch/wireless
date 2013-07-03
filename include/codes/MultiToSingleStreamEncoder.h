/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include "IEncoder.h"

#include "IMultiStreamEncoder.h"
#include "puncturing/IPuncturingSchedule.h"

/**
 * \ingroup codes
 * \brief Punctures a puncture-able code, can produce a stream of symbols.
 */
class MultiToSingleStreamEncoder : public IEncoder {
public:
	MultiToSingleStreamEncoder(IMultiStreamEncoder::Ptr& encoder,
	                   IPuncturingSchedulePtr& puncturing);

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet);

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int numSymbols,
			std::vector<uint16_t>& outSymbols);

private:
	// The encoder
	IMultiStreamEncoder::Ptr m_encoder;

	// The puncturing schedule
	IPuncturingSchedulePtr m_puncturing;

	// A working array that keeps stream indices from which to take symbols, for encode()
	std::vector<uint16_t> m_streamIndicesWorkArr;
};
