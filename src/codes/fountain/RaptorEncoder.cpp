/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/fountain/RaptorEncoder.h"
#include "util/ItppUtils.h"

RaptorEncoder::RaptorEncoder(const std::string & ldpcFilename)
  : m_G(),
    m_ldpc(ldpcFilename, &m_G),
    m_lt(m_ldpc.get_nvar(),
         getNeighborGenerator(m_ldpc.get_nvar()),
         getSymbolFunction()),
    m_packetBits(m_ldpc.get_ninfo()),
    m_encodedBits(m_ldpc.get_nvar()),
    m_encodedVec(m_ldpc.get_nvar(), 0)
{}

void RaptorEncoder::setPacket(const std::string & packet)
{
	// Convert the packet into bvec
	ItppUtils::stringToVector(packet, m_packetBits);

	// LDPC encode the packet
	m_ldpc.encode(m_packetBits, m_encodedBits);

	// Convert the encoded bvec into a string
	std::string ldpcCodeword;
	ItppUtils::vectorToString(m_encodedBits, ldpcCodeword);

	// Set the ldpc codeword to the LT code
	m_lt.setPacket(ldpcCodeword);
}

void RaptorEncoder::encode(unsigned int numSymbols, std::vector<uint16_t> & outSymbols)
{
	m_lt.encode(numSymbols, outSymbols);
}

BitwiseXorSymbolFunction RaptorEncoder::getSymbolFunction()
{
	return BitwiseXorSymbolFunction();
}

LTParityNeighborGenerator RaptorEncoder::getNeighborGenerator(uint32_t codewordSize)
{
	return LTParityNeighborGenerator(codewordSize, 0xdeadbeef);
}

