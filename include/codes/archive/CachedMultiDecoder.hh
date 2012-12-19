/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>
#include <string.h> // memcpy


template<typename ChannelSymbol>
inline CachedMultiDecoder<ChannelSymbol>::CachedMultiDecoder(
		std::tr1::shared_ptr<IDecoder<ChannelSymbol> > decoder,
		unsigned int numPackets,
		unsigned int maxNumSymbolsPerPacket)
  : m_decoder(decoder),
    m_numPackets(numPackets),
    m_maxNumSymbolsPerPacket(maxNumSymbolsPerPacket),
    m_decodedPackets(m_numPackets)
{
	m_symbols.resize(m_numPackets);
	for(unsigned int i = 0; i < numPackets; i++) {
		m_symbols[i].reserve(m_maxNumSymbolsPerPacket);
	}
}



template<typename ChannelSymbol>
inline void CachedMultiDecoder<ChannelSymbol>::resetPacket(unsigned int packetInd) {
	m_symbols[packetInd].clear();
}



template<typename ChannelSymbol>
inline void CachedMultiDecoder<ChannelSymbol>::addSymbols(
											unsigned int packetInd,
											const std::vector<ChannelSymbol>& symbols)
{
	if(!(packetInd < m_numPackets)) {
		throw std::runtime_error("Invalid packetInd");
	}
	if(m_symbols[packetInd].size() + symbols.size() > m_maxNumSymbolsPerPacket) {
		throw std::runtime_error("Too many symbols for this instance");
	}

	m_symbols[packetInd].insert(m_symbols[packetInd].end(),
								symbols.begin(),
								symbols.end());
}



template<typename ChannelSymbol>
inline void CachedMultiDecoder<ChannelSymbol>::decode(unsigned int packetInd)
{
	m_decodedPackets[packetInd] = m_decoder->decode(
			m_symbols[packetInd].size(),
			m_symbols[packetInd]);
}



template<typename ChannelSymbol>
inline DecodeResult CachedMultiDecoder<ChannelSymbol>::getDecodeResult(
												unsigned int packetInd)
{
	if (m_symbols[packetInd].size() == 0) {
		throw(runtime_error("Must call decode with numSymbols>0 to have result"));
	}
	return m_decodedPackets[packetInd];
}
