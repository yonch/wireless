/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include "util/MTRand.h"

using std::string;
using std::vector;

/**
 * \ingroup packetgen
 * \brief Generates random packets
 */
class PacketGenerator
{
public:
	/**
	 * C'tor
	 * @param packetLengthBits the length of the packet, in bits. Must be a
	 * 		multiple of 8.
	 */
	PacketGenerator(unsigned int packetLengthBits);

	/**
	 * Seed the pseudo-random number generator
	 * @param seedValue the seed
	 */
	void seed(unsigned int* const seed, int seedSize);

	/**
	 * Generates a packet.
	 */
	string get();

private:
	// packet length, in bytes, of the desired packet
	unsigned int m_packetLengthBits;

	// random number generator
	MTRand m_random;
};

