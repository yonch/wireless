/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "LayeredEncoder.h"
#include "../IDecoder.h"

/**
 * \ingroup strider
 * \brief Factory for strider encoders and decoders
 */
class StriderFactory {
public:
	/**
	 * Creates the standard strider encoder
	 * @param fragmentLengthBits: the length of each fragment, in bits,
	 * 		including the CRC bits.
	 *
	 * @note: fragmentLengthBits must be an even number
	 */
	static LayeredEncoder::Ptr createEncoder(uint32_t fragmentLengthBits);

	/**
	 * Creates the standard strider decoder
	 * @param fragmentLengthBits: the length of each fragment, in bits,
	 * 		including the CRC bits.
	 */
	static IDecoder<ComplexSymbol>::Ptr createDecoder(uint32_t fragmentLengthBits);

	/**
	 * Creates the strider decoder for a fading channel
	 */
	static IDecoder<FadingComplexSymbol>::Ptr createFadingDecoder(uint32_t fragmentLengthBits);

};

