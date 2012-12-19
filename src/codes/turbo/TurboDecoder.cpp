/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/turbo/TurboDecoder.h"
#include "codes/turbo/TurboCodec.h"
#include <math.h>
#include <assert.h>

TurboDecoder::TurboDecoder(uint32_t messageLength)
  : m_next(0)
{
	itpp::ivec interleaveSequence;
	TurboCodec::getInterleaver(messageLength, interleaveSequence);
	m_codec.set_parameters("013 015 017", // gen1
						   "013 015 017", // gen2
						   4, // constraint length
						   interleaveSequence);

	m_codec.set_metric("TABLE");

	m_llr.set_size(m_codec.get_Ncoded(), false);
}

void TurboDecoder::reset() {
	m_llr.zeros();
	m_next = 0;
}

void TurboDecoder::add(const std::vector<float> & symbols) {
	for(unsigned int i = 0; i < symbols.size(); i++) {
		assert(m_next < m_llr.size());
		addLLR(m_next++, symbols[i]);
	}
}

void TurboDecoder::addLLR(unsigned int location, float llr) {
	assert(isfinite(llr));
	m_llr[location] += llr;
}

DecodeResult TurboDecoder::decode() {
	itpp::bvec decoded;

	m_codec.decode(m_llr, decoded);

	DecodeResult res;
	int numBytes = ((int)decoded.size() + 7) / 8;
	res.packet.resize(numBytes);

	// all but last byte
	int dec_i = 0;
	for(int i = 0; i < numBytes - 1; i++) {
		unsigned char ch = 0;
		for (int b = 0; b < 8; b++) {
			ch |= ( ((unsigned char)decoded[dec_i++].value()) << b);
		}
		res.packet[i] = ch;
	}

	unsigned char ch = 0;
	for(int b = 0; (b < 8) && (dec_i < decoded.size()); b++) {
		ch |= ( ((unsigned char)decoded[dec_i++].value()) << b);
	}
	res.packet[numBytes - 1] = ch;

	return res;
}




