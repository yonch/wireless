/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/turbo/TurboEncoder.h"
#include "codes/turbo/TurboCodec.h"

#include <stdexcept>

TurboEncoder::TurboEncoder(uint32_t messageLength)
{
	itpp::ivec interleaveSequence;
	TurboCodec::getInterleaver(messageLength, interleaveSequence);
	m_codec.set_parameters("013 015 017", // gen1
						   "013 015 017", // gen2
						   4, // constraint length
						   interleaveSequence);

	m_packetLengthBytes = (m_codec.get_Nuncoded() + 7) / 8;
	m_packetBits.set_size(m_codec.get_Nuncoded());
}

void TurboEncoder::setPacket(const std::string & packet) {
	// Check input size
	if(packet.size() != m_packetLengthBytes) {
		throw(std::runtime_error("Packet size doesn't match code"));
	}

	// convert the string to a bvec
	int bitInd = 0;
	// All but the last byte
	for(unsigned int i = 0; i < m_packetLengthBytes - 1; i++) {
		unsigned char byte = static_cast<unsigned char>(packet[i]);
		for(int b = 0; b < 8; b++) {
			m_packetBits[bitInd++] = (byte >> b) & 0x1;
		}
	}

	// Last byte
	unsigned char byte = static_cast<unsigned char>(packet[m_packetLengthBytes - 1]);
	for(int b = 0; (b < 8) && (bitInd < m_packetBits.size()); b++) {
		m_packetBits[bitInd++] = (byte >> b) & 0x1;
	}
}

#include <iostream>
using namespace std;
void TurboEncoder::encode(
		unsigned int numSymbols,
		std::vector<uint16_t> & outSymbols)
{
	if((unsigned int)m_codec.get_Ncoded() != numSymbols) {
		throw(std::runtime_error("Encoder only supports outputting all coded symbols at once"));
	}

	m_codec.encode(m_packetBits, m_encodedBits);

	outSymbols.resize(m_encodedBits.size());

	for(int i = 0; i < m_encodedBits.size(); i++) {
		outSymbols[i] = bool(m_encodedBits[i]);
	}
}

