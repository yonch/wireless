/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/null/NullEncoder.h"
#include "util/Utils.h"

NullEncoder::NullEncoder(uint32_t packetLength)
  : m_packetLength(packetLength),
    m_numRemaining(0)
{
	m_packetVector.reserve(packetLength);
}

void NullEncoder::setPacket(const std::string & packet)
{
	// make sure packet is of the right size
	if (packet.size() != (m_packetLength + 7) / 8) {
		throw std::runtime_error("Packet size doesn't match this encoder");
	}

	// Convert the packet into a bit vector
	Utils::stringToVector(packet, m_packetVector);
	m_numRemaining = m_packetLength;
}

void NullEncoder::encode(unsigned int numSymbols, std::vector<uint16_t> & outSymbols)
{
	if (numSymbols > m_numRemaining) {
		throw std::runtime_error("Requested more symbols than are available");
	}

	outSymbols.clear();
	outSymbols.reserve(numSymbols);
	for(uint32_t i = m_packetLength - m_numRemaining; outSymbols.size() < numSymbols; i++) {
		outSymbols.push_back(uint16_t(m_packetVector[i]) & 0x1);
	}
}



