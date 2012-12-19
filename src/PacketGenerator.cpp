/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "PacketGenerator.h"

#include <stdexcept>

#include "util/MTRand.h"

using namespace std;

PacketGenerator::PacketGenerator(unsigned int packetLengthBits)
	: m_packetLengthBits(packetLengthBits),
	  m_random(0u)
{}

void PacketGenerator::seed(unsigned int* const seed, int seedSize) {
	m_random.seed(seed, seedSize);
}

string PacketGenerator::get() {
	// initialize packet of size k to 0
	string packet((m_packetLengthBits + 7) / 8, 0);

	unsigned int numFullBytes = m_packetLengthBits / 8;
	// fill the packet with random bytes
	for (unsigned int i = 0; i < numFullBytes; i++) {
		packet[i] = m_random.randInt() & 0xFF;
	}

	if(m_packetLengthBits % 8 != 0) {
		// generate a last byte that does not have all bits
		packet[packet.size()-1] = m_random.randInt() & ((1 << (m_packetLengthBits % 8)) - 1);
	}

	return packet;
}


