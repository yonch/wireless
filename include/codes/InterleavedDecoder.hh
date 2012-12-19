/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <assert.h>

template<typename Decoder>
inline InterleavedDecoder<Decoder>::InterleavedDecoder(
		const Decoder& decoder,
		const std::vector<uint16_t> & interleaveSequence)
  : m_decoder(decoder),
    m_interleavingSequence(interleaveSequence),
    m_next(0)
{}

template<typename Decoder>
inline void InterleavedDecoder<Decoder>::reset() {
	m_decoder.reset();
	m_next = 0;
}

template<typename Decoder>
inline void InterleavedDecoder<Decoder>::add(
		const std::vector<float> & symbols)
{
	assert(m_next + symbols.size() <= m_interleavingSequence.size());

	for(unsigned int i = 0; i < symbols.size(); i++) {
		m_decoder.addLLR(m_interleavingSequence[m_next++], symbols[i]);
	}
}

template<typename Decoder>
inline DecodeResult InterleavedDecoder<Decoder>::decode() {
	return m_decoder.decode();
}

