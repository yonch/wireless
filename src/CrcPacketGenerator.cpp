/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "CrcPacketGenerator.h"

#include <stdexcept>

#include "util/MTRand.h"
#include "util/Utils.h"

using namespace std;

CrcPacketGenerator::CrcPacketGenerator(unsigned int packetLengthBits)
	: m_packetLengthBits(packetLengthBits),
	  m_random(0u)
{
	if(packetLengthBits < 40) {
		throw(std::runtime_error("Packet length should be no less than 40"));
	}
}

void CrcPacketGenerator::seed(unsigned int* const seed, int seedSize) {
	m_random.seed(seed, seedSize);
}

string CrcPacketGenerator::get() {
	// initialize packet of size k to 0
	string packet((m_packetLengthBits + 7) / 8, 0);

	unsigned int numFullBytes = m_packetLengthBits / 8;
	// fill the packet with random bytes, except the CRC bits
	for (unsigned int i = 2; i < numFullBytes; i++) {
		packet[i] = m_random.randInt() & 0xFF;
	}

	if(m_packetLengthBits % 8 != 0) {
		// generate a last byte that does not have all bits
		packet[packet.size()-1] = m_random.randInt() & ((1 << (m_packetLengthBits % 8)) - 1);
	}

	*(uint16_t*)packet.data() =
			Utils::getArrCRC16((const uint8_t*)packet.data() + 2, packet.size() - 2);

	return packet;
}


