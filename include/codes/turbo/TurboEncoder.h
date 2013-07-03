/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <itpp/itbase.h>
#include <itpp/comm/turbo.h>

#include "../IEncoder.h"

/**
 * \ingroup turbo
 * \brief Turbo encoder
 */
class TurboEncoder : public IEncoder
{
public:
	TurboEncoder(uint32_t messageLength);

	/**
	 * d'tor
	 */
	virtual ~TurboEncoder() {}

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
	itpp::Turbo_Codec m_codec;

	// The number of input bytes for a single message block
	unsigned int m_packetLengthBytes;
	itpp::bvec m_packetBits;
	itpp::bvec m_encodedBits;
};
