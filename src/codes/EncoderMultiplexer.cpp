/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/EncoderMultiplexer.h"

#include <stdexcept>
#include "util/Utils.h"

EncoderMultiplexer::EncoderMultiplexer(
		const std::vector<IEncoderPtr> & encoders,
		const std::vector<uint32_t> & messageLengthsBits)
  : m_encoders(encoders),
    m_messageLengthsBits(messageLengthsBits),
    m_next(0)
{
	if(m_encoders.size() != m_messageLengthsBits.size()) {
		throw(std::runtime_error("Mismatch between number of encoders and lengths of messages"));
	}

	m_symbolBuffer.reserve(1024);
}

void EncoderMultiplexer::setPacket(const std::string & packet) {
	std::string subPacket;
	uint32_t currentOffset = 0;

	for(uint32_t i = 0; i < m_messageLengthsBits.size(); i++) {
		// extract the sub-packet
		Utils::getStringBits(packet,
							 subPacket,
							 currentOffset,
							 m_messageLengthsBits[i]);

		// set the appropriate encoder
		m_encoders[i]->setPacket(subPacket);

		// advance the offset where the next sub-packet is taken
		currentOffset += m_messageLengthsBits[i];
	}

	// set the first symbol to be taken from first encoder
	m_next = 0;
}

void EncoderMultiplexer::encode(
		unsigned int numSymbols,
		std::vector<uint16_t> & outSymbols)
{
	// resize the output buffer
	outSymbols.clear();
	outSymbols.resize(numSymbols);

	uint32_t N = m_encoders.size();

	uint32_t shortEncodersLen = numSymbols / N;
	uint32_t numLongerEncoders = numSymbols % N;

	for(uint32_t i = 0; i < N; i++) {
		// Calculate number of symbols to transmit
		uint32_t encLen = shortEncodersLen;
		if(numLongerEncoders > 0) {
			encLen++;
			numLongerEncoders--;
		}

		if(encLen > 0) {
			// Encode that many symbols
			m_encoders[(m_next + i) % N]->encode(encLen, m_symbolBuffer);

			// Put in right locations in output buffer
			uint32_t srcLoc = 0;
			for(uint32_t dstLoc = i; dstLoc < numSymbols; dstLoc += N) {
				outSymbols[dstLoc] = m_symbolBuffer[srcLoc++];
			}
		}
	}

	// Update m_next
	m_next = (m_next + numSymbols) % N;
}
