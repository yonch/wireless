/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../ILLRDecoder.h"
#include <stdint.h>

/**
 * \ingroup codes
 * \brief A decoder that simply hard-decodes its given LLRs
 */
class NullDecoder: public ILLRDecoder {
public:
	/**
	 * C'tor
	 * @param packetLength: the length of the packet
	 */
	NullDecoder(uint32_t packetLength);

	/**
	 * @see ILLRDecoder
	 */
	virtual void reset();
	virtual void add(const std::vector<LLRValue>& llrs);
	virtual DecodeResult decode();

private:
	// The packet length this decoder is built for
	uint32_t m_packetLength;

	// The packet estimates
	std::vector<bool> m_packetVector;
};

