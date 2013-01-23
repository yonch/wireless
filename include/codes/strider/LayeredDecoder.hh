/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#include "../../util/Utils.h"


template<typename ChannelSymbol>
LayeredDecoder<ChannelSymbol>::LayeredDecoder(	unsigned int layerLength,
								std::complex<double> *matrixG,
								int rowsG,
								int colsG,
								unsigned int fragmentLengthBits,
								IDecoder<ComplexSymbol>::Ptr & decoder,
								IEncoderPtr & encoder,
								IMapper<ComplexSymbol>::Ptr & mapper,
								bool handleCrc)
  : m_layerLength(layerLength),
    m_numFragments(colsG),
    m_fragmentLengthBits(fragmentLengthBits),
    m_receivedSymbols(rowsG, m_layerLength),
    m_fading(rowsG, m_layerLength),
    m_numFullPasses(0),
    m_nonFullPassLength(0),
    m_numDecodedLayers(0),
    m_n0(0),
    m_layerManipulator(m_layerLength, matrixG, rowsG, colsG),
    m_decoder(decoder),
    m_encoder(encoder),
    m_mapper(mapper),
    m_handleCrc(handleCrc)
{
	unsigned int packetLengthBits = m_fragmentLengthBits * m_numFragments;
	m_decodedMessage.resize((packetLengthBits + 7) / 8);

	m_decodedLayers.resize(m_numFragments);
}


template<typename ChannelSymbol>
void LayeredDecoder<ChannelSymbol>::reset() {
	m_numFullPasses = 0;
	m_nonFullPassLength = 0;
	m_numDecodedLayers = 0;
}

template<>
inline void LayeredDecoder<ComplexSymbol>::add(
		const std::vector<ComplexSymbol> & symbols,
		N0_t n0)
{
	m_n0 = n0;

	for(unsigned int i = 0; i < symbols.size(); i++) {
		m_receivedSymbols(m_numFullPasses, m_nonFullPassLength++) = symbols[i];
		if(m_nonFullPassLength == m_layerLength) {
			m_nonFullPassLength = 0;
			m_numFullPasses++;
		}
	}
}

template<>
inline void LayeredDecoder<FadingComplexSymbol>::add(
		const std::vector<FadingComplexSymbol> & symbols,
		N0_t n0)
{
	m_n0 = n0;

	for(unsigned int i = 0; i < symbols.size(); i++) {
		m_receivedSymbols(m_numFullPasses, m_nonFullPassLength) = symbols[i].symbol;
		m_fading(m_numFullPasses, m_nonFullPassLength) = symbols[i].fading;
		m_nonFullPassLength++;
		if(m_nonFullPassLength == m_layerLength) {
			m_nonFullPassLength = 0;
			m_numFullPasses++;
		}
	}
}

template<typename ChannelSymbol>
void LayeredDecoder<ChannelSymbol>::add(const std::vector<ChannelSymbol>& symbols,
		 	 	 	 	 const std::vector<FadingMagnitude>& fadingMagnitude,
		 	 	 	 	 N0_t n0)
{
	throw(std::runtime_error("Not supported"));
}

template<typename ChannelSymbol>
DecodeResult LayeredDecoder<ChannelSymbol>::decode() {
	// First, set received symbols into m_layerManipulator
    resetManipulator();

	// Peel off all already decoded layers
	for(unsigned int frag = 0; frag < m_numDecodedLayers; frag++) {
		peelLayer(frag);
	}

	// Maximal Ratio Combined signal
	boost_cvec mrc(m_layerLength);
	std::vector<ComplexSymbol> combined;

	// SNRs for each element in the combined signal
	boost_vec combinedScale(m_layerLength);
	std::vector<float> fadingMagnitude;


	// For each fragment
	for(unsigned int frag = m_numDecodedLayers; frag < m_numFragments; frag++) {
		// Get the Maximum Ratio Combining
		m_layerManipulator.maximalRatioCombining(frag,
												 m_n0,
												 mrc,
												 combinedScale);

		// Decode with demapped information
		combined.resize(mrc.size());
		fadingMagnitude.resize(combinedScale.size());
		for(unsigned int i = 0; i < combinedScale.size(); i++) {
			fadingMagnitude[i] = combinedScale[i];
			combined[i] = mrc[i];
		}
		m_decoder->reset();
		m_decoder->add(combined, fadingMagnitude, m_n0);
		DecodeResult res = m_decoder->decode();
		m_decodedLayers[frag] = res.packet;

		// Decide whether to proceed
		if(m_handleCrc) {
			// Update decoded result into record
			Utils::copyBits(m_decodedMessage, frag*m_fragmentLengthBits,
			                res.packet, 32,
			                m_fragmentLengthBits);

			if(Utils::passesCRC32(res.packet) == false) {
				// CRC doesn't check. return now with what we already have.
				return DecodeResult(m_decodedMessage, 0.0f);
			} else {
				// We can use the decoded layers next time
				m_numDecodedLayers++;
			}
		} else {
			// No CRC, we update the result anyway
			Utils::copyBits(m_decodedMessage, frag*m_fragmentLengthBits,
			                res.packet, 0,
			                m_fragmentLengthBits);
		}

		peelLayer(frag);
	}

	return DecodeResult(m_decodedMessage, 0.0f);
}

template<>
inline void LayeredDecoder<ComplexSymbol>::resetManipulator()
{
    // First, set received symbols into m_layerManipulator
    m_layerManipulator.setSymbols(m_numFullPasses,
    							  m_nonFullPassLength,
    							  m_receivedSymbols);
}

template<>
inline void LayeredDecoder<FadingComplexSymbol>::resetManipulator()
{
    // First, set received symbols into m_layerManipulator
    m_layerManipulator.setSymbols(m_numFullPasses,
    							  m_nonFullPassLength,
    							  m_receivedSymbols,
    							  m_fading);
}

template<typename ChannelSymbol>
void LayeredDecoder<ChannelSymbol>::peelLayer(unsigned int frag)
{
	// Re-encoded fragment
	std::vector<uint16_t> encoded;

	// Re-mapped fragment
	std::vector<ComplexSymbol> mapped;

	unsigned int numEncodedBits = m_mapper->forecast(m_layerLength);

	// Encode the decoded fragment
	m_encoder->setPacket(m_decodedLayers[frag]);
	m_encoder->encode(numEncodedBits,
					  encoded);

	// Modulate the fragment
	m_mapper->process(encoded, mapped);

	// Subtract the fragment's contribution from the remaining signal
	m_layerManipulator.subtractLayer(frag, mapped);
}

