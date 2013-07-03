/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../IEncoder.h"
#include <stdint.h>

/**
 * \ingroup codes
 * \brief An encoder that does not modify the input bits to the output.
 *  Each input bit is copied to an output symbol
 */
class NullEncoder: public IEncoder {
public:
	/**
	 * C'tor
	 * @param packetLength: the length of the packet
	 */
	NullEncoder(uint32_t packetLength);

	/**
	 * @see IEncoder
	 */
	virtual void setPacket(const std::string& packet);

	virtual void encode(unsigned int numSymbols,
						  std::vector<uint16_t>& outSymbols);

private:
	// The length of packets this encoder handles.
	const uint32_t m_packetLength;

	// The number of remaining symbols to decode
	uint32_t m_numRemaining;

	// The bits from the current packet
	std::vector<bool> m_packetVector;
};

