/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdexcept>
#include <tr1/memory>

#include "../CodeBench.h"
#include "DecodeResult.h"
#include "IDecoder.h"
#include "ILLRDecoder.h"
#include "../demappers/IDemapper.h"

/**
 * \ingroup codes
 * \brief Composes a demapper with an LLR decoder.
 *
 * Implements a symbol decoder that handles symbols by demapping them using a
 *     given demapper and then passes them on to an ILLRDecoder
 */
template<typename ChannelSymbol>
class SymbolToLLRDecoderAdaptor : public IDecoder<ChannelSymbol> {
public:
	typedef DecodeResult Result;
	typedef std::tr1::shared_ptr<SymbolToLLRDecoderAdaptor<ChannelSymbol> > Ptr;

	/**
	 * C'tor
	 * @param demapper: demapper used to demap incoming symbols
	 * @param decoder: decoder used to decode the resulting LLRs
	 */
	SymbolToLLRDecoderAdaptor(typename IDemapper<ChannelSymbol>::Ptr& demapper,
							  ILLRDecoder::Ptr& decoder);

	/**
	 * D'tor
	 */
	virtual ~SymbolToLLRDecoderAdaptor() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset();

	/**
	 * Adds more symbol information to the packet.
	 * 		Assumes the noise power is constant accross all symbols
	 *
	 * @param symbols: the symbols to add
	 */
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 N0_t n0);

	/**
	 * Adds more symbol information to the packet.
	 * 		Assumes a fading channel (where noise power is constant but
	 * 		received power changes).
	 *
	 * @param symbols: the symbols to add
	 * @param fadingMagnitude: the scaling factor that the symbols underwent
	 * 		when transmitted on the channel
	 * @param n0: channel noise power
	 */
	virtual void add(const std::vector<ChannelSymbol>& symbols,
					 const std::vector<FadingMagnitude>& fadingMagnitude,
					 N0_t n0);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode();

private:
	// Demapper
	typename IDemapper<ChannelSymbol>::Ptr m_demapper;

	// Decoder
 	ILLRDecoder::Ptr m_decoder;

 	// Buffer to hold demapped values
 	std::vector<LLRValue> m_llrs;
};

#include "SymbolToLLRDecoderAdaptor.hh"
