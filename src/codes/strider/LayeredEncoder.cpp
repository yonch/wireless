/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/strider/LayeredEncoder.h"

#include <stdexcept>

#include "util/Utils.h"

LayeredEncoder::LayeredEncoder(
		IEncoderPtr & innerEncoder,
		IMapper<ComplexSymbol>::Ptr & innerMapper,
		unsigned int packetLengthBits,
        unsigned int passLength,
        std::complex<double>* matrixG,
		int rowsG,
		int colsG,
	   	bool prependCrc)
  : m_encoder(innerEncoder),
    m_mapper(innerMapper),
    m_packetLengthBytes((packetLengthBits + 7) / 8),
    m_batchSize((unsigned int)colsG),
    m_fragmentLengthBits(packetLengthBits / m_batchSize),
    m_passLengthSymbols(passLength),
    m_passLengthEncoderOutputs(m_mapper->forecast(passLength)),
    m_prependCrc(prependCrc),
    m_layerSuperposition(m_passLengthSymbols, matrixG, rowsG, colsG)
{
	if(m_fragmentLengthBits * m_batchSize != packetLengthBits) {
		throw std::runtime_error("Each fragment must be an integral number of bits");
	}
}

void LayeredEncoder::setPacket(const std::string & packet) {
	// Check we got packet of the correct size
	if(packet.size() != m_packetLengthBytes) {
		throw(std::runtime_error("Got packet of wrong size"));
	}

	// Buffer for encoder outputs
	std::vector<uint16_t> encoderOutputs;

	// Buffer for mapped symbols
	std::vector<ComplexSymbol> mapperOutputs;

	// Buffer to hold each fragments' bits
	unsigned int fragmentLengthBytes = (m_fragmentLengthBits + 7) / 8;
	std::string subpacket;

	// Reset the LayerSuperposition instance
	m_layerSuperposition.reset();

	// For each fragment:
	for(unsigned int i = 0; i < m_batchSize; i++) {
		if (m_prependCrc) {
			subpacket.resize(fragmentLengthBytes + 4);
			// Get the subpacket
			Utils::copyBits(subpacket,
							32,
							packet,
							m_fragmentLengthBits * i,
							m_fragmentLengthBits);
			// Get CRC32
			uint32_t crc = Utils::getArrCRC32((const uint8_t*)subpacket.data() + 4,
											  fragmentLengthBytes);
			*(uint32_t*)subpacket.data() = crc;
		} else {
			subpacket.resize(fragmentLengthBytes);
			// Get the subpacket
			Utils::copyBits(subpacket,
							0,
							packet,
							m_fragmentLengthBits * i,
							m_fragmentLengthBits);
		}

		// Encode the subpacket
		m_encoder->setPacket(subpacket);
		m_encoder->encode(m_passLengthEncoderOutputs, encoderOutputs);

		// Map the symbols
		m_mapper->process(encoderOutputs, mapperOutputs);

		// Set the encoded symbols in the LayerSuperposition
		m_layerSuperposition.setLayer(i, mapperOutputs);
	}
}

void LayeredEncoder::encode(
		unsigned int numSymbols,
		std::vector<ComplexSymbol> & outSymbols)
{
	outSymbols.clear();
	outSymbols.resize(numSymbols);

	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = m_layerSuperposition.next();
	}
}
