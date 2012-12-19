/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/InterleavedEncoder.h"

#include <algorithm>
#include <stdexcept>

InterleavedEncoder::InterleavedEncoder(	IEncoderPtr & encoder,
										const std::vector<uint16_t>& interleaveSequence)
  : m_encoder(encoder),
    m_interleavingSequence(interleaveSequence),
    m_maxIndex(*(std::max_element(interleaveSequence.begin(),
                                  interleaveSequence.end()))),
    m_next(0)
{
	m_encoded.reserve(m_maxIndex + 1);
}

void InterleavedEncoder::setPacket(const std::string & packet) {
	// Add packet to encoder
	m_encoder->setPacket(packet);

	// encode enough symbols for the interleaving sequence
	m_encoder->encode(m_maxIndex + 1, m_encoded);

	// reset the output to start at the beginning of interleave sequence
	m_next = 0;
}

void InterleavedEncoder::encode(unsigned int numSymbols,
								std::vector<uint16_t> & outSymbols)
{
	// Sanity check, make sure the request doesn't ask for more symbols than
	// interleaving sequence has
	if(m_next + numSymbols > m_interleavingSequence.size()) {
		throw(std::runtime_error("Requested more symbols than interleaver can handle"));
	}

	outSymbols.clear();
	outSymbols.resize(numSymbols);

	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = m_encoded[m_interleavingSequence[m_next++]];
	}
}

