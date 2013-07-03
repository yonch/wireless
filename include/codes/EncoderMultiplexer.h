/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <string>
#include <vector>
#include <stdint.h>

#include "IEncoder.h"

/**
 * \ingroup codes
 * \brief Divides message to different encoders, then round-robins for symbols from these encoders.
 *
 * The EncoderMultiplexer class is given several different encoders. A given
 * 		message is divided into sub-messages and these sub-messages are passed
 * 		on to individual encoders. When symbols are produced, the different
 * 		encoders are consulted in a round-robin fashion.
 */
class EncoderMultiplexer : public IEncoder
{
public:
	typedef std::tr1::shared_ptr<EncoderMultiplexer> Ptr;

	EncoderMultiplexer(const std::vector<IEncoderPtr>& encoders,
					   const std::vector<uint32_t>& messageLengthsBits);

	/**
	 * d'tor
	 */
	virtual ~EncoderMultiplexer() {}

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
	// The encoders used
	const std::vector<IEncoderPtr> m_encoders;

	// The lengths of messages for the different encoders
	const std::vector<uint32_t> m_messageLengthsBits;

	// The next encoder that should be consulted
	uint32_t m_next;

	// A temporary buffer to hold symbols from individual encoders
	std::vector<uint16_t> m_symbolBuffer;
};
