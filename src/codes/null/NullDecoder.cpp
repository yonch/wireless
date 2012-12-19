/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/null/NullDecoder.h"
#include <stdexcept>
#include "util/Utils.h"

NullDecoder::NullDecoder(uint32_t packetLength)
  : m_packetLength(packetLength)
{
	m_packetVector.reserve(packetLength);
}

void NullDecoder::reset() {
	m_packetVector.clear();
}

void NullDecoder::add(const std::vector<LLRValue> & llrs) {
	if (llrs.size() + m_packetVector.size() > m_packetLength) {
		throw std::runtime_error("Tried to add more LLR values than packet length");
	}

	for(uint32_t i = 0; i < llrs.size(); i++) {
		m_packetVector.push_back(llrs[i] < 0);
	}
}

DecodeResult NullDecoder::decode() {
	if(m_packetVector.size() != m_packetLength) {
		throw std::runtime_error("Not enough symbols to decode");
	}

	DecodeResult res;
	Utils::vectorToString(m_packetVector, res.packet);
	return res;
}



