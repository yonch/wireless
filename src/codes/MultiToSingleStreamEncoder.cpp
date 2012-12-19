/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/MultiToSingleStreamEncoder.h"

#include <assert.h>
#include <stdexcept>

MultiToSingleStreamEncoder::MultiToSingleStreamEncoder(	IMultiStreamEncoder::Ptr & encoder,
																IPuncturingSchedulePtr & puncturing)
  : m_encoder(encoder),
    m_puncturing(puncturing)
{}

void MultiToSingleStreamEncoder::setPacket(const std::string & packet)
{
	m_encoder->setPacket(packet);
	m_puncturing->reset();
}

void MultiToSingleStreamEncoder::encode(unsigned int numSymbols,
											  std::vector<uint16_t> & outSymbols)
{
	// Get which spine values need to be computed
	m_puncturing->batchNext(numSymbols, m_streamIndicesWorkArr);

	// Encode all requested spine values
	m_encoder->encode(m_streamIndicesWorkArr, outSymbols);
}
