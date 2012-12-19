/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "MultiToSingleStreamDecoder.h"


template<typename ChannelSymbol>
inline MultiToSingleStreamDecoder<ChannelSymbol>::MultiToSingleStreamDecoder(
		typename IMultiStreamDecoder<ChannelSymbol>::Ptr & decoder,
		IPuncturingSchedulePtr & puncturing)
  : m_decoder(decoder),
    m_puncturing(puncturing)
{}



template<typename ChannelSymbol>
inline void MultiToSingleStreamDecoder<ChannelSymbol>::reset()
{
	m_decoder->reset();
	m_puncturing->reset();
}


template<typename ChannelSymbol>
inline void MultiToSingleStreamDecoder<ChannelSymbol>::add(
		const std::vector<ChannelSymbol> & symbols,
		N0_t n0) {
	// Get which spine values need to be computed
	m_puncturing->batchNext(symbols.size(), m_streamIndicesWorkArr);

	// Encode all requested spine values
	m_decoder->add(m_streamIndicesWorkArr, symbols, n0);
}

template<typename ChannelSymbol>
inline void MultiToSingleStreamDecoder<ChannelSymbol>::add(
		const std::vector<ChannelSymbol> & symbols,
		 const std::vector<FadingMagnitude>& fadingMagnitude,
		 N0_t n0)
{
	throw(std::runtime_error("Not currently supported"));
}

template<typename ChannelSymbol>
inline DecodeResult MultiToSingleStreamDecoder<ChannelSymbol>::decode()
{
	return m_decoder->decode();
}

//template<typename ChannelSymbol>
//inline SpinalDecodeResult MultiToSingleStreamDecoder<ChannelSymbol>::decodeExtended()
//{
//	return m_decoder->decodeExtended();
//}

