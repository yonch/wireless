/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

template<typename ChannelSymbol>
inline SymbolToLLRDecoderAdaptor<ChannelSymbol>::SymbolToLLRDecoderAdaptor(
		typename IDemapper<ChannelSymbol>::Ptr & demapper,
		ILLRDecoder::Ptr & decoder)
  : m_demapper(demapper),
    m_decoder(decoder)
{}

template<typename ChannelSymbol>
inline void SymbolToLLRDecoderAdaptor<ChannelSymbol>::reset() {
	m_decoder->reset();
}

template<typename ChannelSymbol>
inline void SymbolToLLRDecoderAdaptor<ChannelSymbol>::add(
		const std::vector<ChannelSymbol> & symbols,
		N0_t n0)
{
	m_demapper->process(symbols, n0, m_llrs);
	m_decoder->add(m_llrs);
}

template<typename ChannelSymbol>
inline void SymbolToLLRDecoderAdaptor<ChannelSymbol>::add(
		const std::vector<ChannelSymbol> & symbols,
		const std::vector<FadingMagnitude>& fadingMagnitude,
		N0_t n0)
{
	m_demapper->process(symbols, fadingMagnitude, n0, m_llrs);
	m_decoder->add(m_llrs);
}

template<typename ChannelSymbol>
inline DecodeResult SymbolToLLRDecoderAdaptor<ChannelSymbol>::decode() {
	return m_decoder->decode();
}

