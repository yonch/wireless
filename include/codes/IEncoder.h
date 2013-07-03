/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <string>
#include <vector>
#include <stdint.h>

class IEncoder;
typedef std::tr1::shared_ptr<IEncoder> IEncoderPtr;

/**
 * \ingroup codes
 * \brief Interface of an encoder
 */
class IEncoder
{
public:
	/**
	 * d'tor
	 */
	virtual ~IEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet) = 0;

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int numSymbols,
			std::vector<uint16_t>& outSymbols) = 0;
};
