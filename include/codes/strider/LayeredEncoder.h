/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <itpp/itbase.h>
#include "../../mappers/IMapper.h"
#include "../IEncoder.h"
#include "LayerSuperposition.h"

/**
 * \ingroup strider
 * \brief Strider encoder
 */
class LayeredEncoder {
public:
	typedef std::tr1::shared_ptr<LayeredEncoder> Ptr;

	/**
	 * C'tor
	 *
	 * @param innerEncoder: the encoder used to encode the code fragments
	 * @param innerMapper: the mapper used on each of the code fragments
	 * @param packetLengthBits: number of bits in a single full packet
	 * @param passLength: the number of symbols in every pass
	 * @param G: generator matrix for the code
	 * @param prependCrc: true if a CRC32 should be inserted before each
	 * 		fragment
	 *
	 * @note: we currently require that each fragment's length will be an
	 *     integral number of bytes
	 */
	LayeredEncoder(IEncoderPtr& innerEncoder,
			   	   IMapper<ComplexSymbol>::Ptr& innerMapper,
			   	   unsigned int packetLengthBits,
			   	   unsigned int passLength,
			   	   std::complex<double>* matrixG,
			   	   int rowsG,
			   	   int colsG,
			   	   bool prependCrc);

	/**
	 * Virtual d'tor
	 */
	virtual ~LayeredEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet);

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int numSymbols,
			std::vector<ComplexSymbol>& outSymbols);

private:
	// The encoder, used to encode each of the fragments
	IEncoderPtr m_encoder;

	// The mapper used to modulate the fragments
	IMapper<ComplexSymbol>::Ptr m_mapper;

	// The number of bits in a full packet
	const unsigned int m_packetLengthBytes;

	// The number of fragments in a packet
	const unsigned int m_batchSize;

	// The number of bits in a full packet
	const unsigned int m_fragmentLengthBits;

	// The number of symbols to transmit in each pass
	const unsigned int m_passLengthSymbols;

	// The number of encoder outputs to produce, in order to get the symbols
	const unsigned int m_passLengthEncoderOutputs;

	// Whether to prepend a CRC32
	const bool m_prependCrc;

	// The layering encoder that gets symbols from the modulated fragments
	LayerSuperposition m_layerSuperposition;
};
