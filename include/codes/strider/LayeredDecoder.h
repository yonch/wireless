/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

#include "../../CodeBench.h"
#include "../IDecoder.h"
#include "../IEncoder.h"
#include "../../mappers/IMapper.h"
#include "LayerManipulator.h"

/**
 * \ingroup strider
 * \brief Strider decoder
 */
template<typename ChannelSymbol>
class LayeredDecoder : public IDecoder<ChannelSymbol> {
public:
	/**
	 * C'tor
	 * @param innerDecoder: the decoder used to decode each fragment
	 */
	LayeredDecoder(unsigned int layerLength,
					std::complex<double>* matrixG,
					int rowsG,
					int colsG,
					unsigned int fragmentLengthBits,
					IDecoder<ComplexSymbol>::Ptr& decoder,
					IEncoderPtr& encoder,
					IMapper<ComplexSymbol>::Ptr& mapper,
					bool handleCrc);

	/**
	 * D'tor
	 */
	virtual ~LayeredDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset();

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with symbol 0.
	 * @param symbols: the symbols to add
	 *
	 * @important the decoder applies the last n0 value to all symbols received,
	 * 		not only to the new symbols
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
	typedef boost::numeric::ublas::vector<ComplexSymbol> boost_cvec;
	typedef boost::numeric::ublas::vector<float> boost_vec;

	/**
	 * Resets the LayerManipulator with a fresh copy of all add()ed symbols
	 */
	void resetManipulator();

	/**
	 * Peels layer in m_decodedLayers from m_layerManipulator
	 * @param layerInd: the layer index of the layer to peel
	 */
	void peelLayer(unsigned int frag);

	// The pass length (number of symbols generated using the same column of G)
	const unsigned int m_layerLength;

	// The number of fragments
	const unsigned int m_numFragments;

	// The length, in bits of each fragment
	const unsigned int m_fragmentLengthBits;

	// The raw received symbols
	boost::numeric::ublas::matrix<
		ComplexSymbol, boost::numeric::ublas::column_major> m_receivedSymbols;

	// The magnitudes of fading on the channel
	boost::numeric::ublas::matrix<
		ComplexBaseType, boost::numeric::ublas::column_major> m_fading;

	// The number of valid full passes in m_symbols
	unsigned int m_numFullPasses;

	// The number of symbols in the non-full pass
	unsigned int m_nonFullPassLength;

	// The number of layers already decoded
	unsigned int m_numDecodedLayers;

	// The decoded layers (including CRCs if m_handleCrc is true)
	std::vector<std::string> m_decodedLayers;

	// The decoded message
	std::string m_decodedMessage;

	// The noise power to decode with
	N0_t m_n0;

	// A LayerManipulator to do MRC and peel off layers
	LayerManipulator m_layerManipulator;

	// Decoder to recover the fragments
	IDecoder<ComplexSymbol>::Ptr m_decoder;

	// Encoder used for recreating the fragment so they can be subtracted
	IEncoderPtr m_encoder;

	// Mapper used for recreating the fragment so they can be subtracted
	IMapper<ComplexSymbol>::Ptr m_mapper;

	// Should the layered decoder handle CRC32
	const bool m_handleCrc;
};

typedef LayeredDecoder<ComplexSymbol> ComplexLayeredDecoder;
typedef LayeredDecoder<FadingComplexSymbol> FadingLayeredDecoder;

#include "LayeredDecoder.hh"
