/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>

#include "../../util/Utils.h"
#include "../../CodeBench.h"

// IMPLEMENTATION OF INLINE FUNCTIONS.
// SHOULD ONLY BE INCLUDED FROM HashEncoder.h

template<typename SpineValueType>
inline HashEncoder<SpineValueType>::HashEncoder(
								unsigned int k,
								unsigned int spineLength)
	: m_k(k),
	  m_numMessageBits(k * spineLength),
	  m_spineLength(spineLength)
{
	m_spine.reserve(m_spineLength);
}

template<typename SpineValueType>
inline void HashEncoder<SpineValueType>::setPacket(const std::string & packet)
{
	// Sanity check - given packet must be the correct size
	if (packet.length() != ((m_numMessageBits + 7) / 8)) {
		throw(
			std::runtime_error("input packet's size inconsistent with encoder's message size"));
	}

	// Initialize the current spine value = 0
	typename SpineValueType::Seed spineSeed = 0;

	// Initialize the spine
	m_spine.clear();

	// Convert the packet into a bit vector
	vector<bool> packetVector;
	Utils::stringToVector(packet, packetVector);

	unsigned int remainingBits = m_numMessageBits;

	for(unsigned int spineIndex = 0; spineIndex < m_spineLength; spineIndex++) {
		// gather "transmitBits" bits from the stream
		unsigned int gatheredBits = 0;
		for(unsigned int i = 0; i < m_k; i++) {
			gatheredBits = gatheredBits | ((packetVector[m_numMessageBits-remainingBits+i] & 0x1) << i);
		}

		// hash the state with the gathered bits to get new hash state
		SpineValueType spineValue(spineSeed, gatheredBits);
		spineSeed = spineValue.getSeed();

		// Save the current spine value into the spine
		m_spine.push_back(spineValue);

		// update remaining bits
		remainingBits -= m_k;
	}
}

template<typename SpineValueType>
inline void HashEncoder<SpineValueType>::encode(
		const std::vector<uint16_t> & spineValueIndices,
		std::vector<uint16_t> & outSymbols)
{
	unsigned int numSymbols = spineValueIndices.size();
	outSymbols.clear();
	outSymbols.resize(numSymbols);

	// Encode symbols
	for(unsigned int i = 0; i < numSymbols; i++) {
		// Get what spine value to get symbol from
		uint16_t spineValueIndex = spineValueIndices[i];

		// Get the next symbol from that spine value
		outSymbols[i] = m_spine[spineValueIndex].next();
	}
}
