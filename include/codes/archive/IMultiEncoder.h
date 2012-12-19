/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <string>
#include <vector>
#include <stdint.h>

class IMultiEncoder;
typedef std::tr1::shared_ptr<IMultiEncoder> IMultiEncoderPtr;


class IMultiEncoder
{
public:
	/**
	 * d'tor
	 */
	virtual ~IMultiEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packetInd: the index of the packet (0 to numPackets-1)
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(unsigned int packetInd, const std::string& packet) = 0;

	/**
	 * Removes the packet from the cache
	 * Note that this might not actually free memory, in order to save on
	 *     new/delete overhead
	 * @param packetInd: the index of the packet to be removed
	 */
	virtual void removePacket(unsigned int packetInd) = 0;

	/**
	 * @return True if packet exists, False otherwise.
	 * @param packetInd: the index of the packet queried.
	 */
	virtual bool hasPacket(unsigned int packetInd) = 0;

	/**
	 * Copies the specified encoded symbols into outSymbols
	 * @param packetInd: the index of the packet whose symbols are requested
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int packetInd,
			unsigned int numSymbols,
			std::vector<uint16_t>& outSymbols) = 0;
};
