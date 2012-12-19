/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../IEncoder.h"
#include "../ILLRDecoder.h"

class StriderTurboCode {
public:
	/**
	 * Creates a new strider turbo encoder
	 * @param lengthBits: the number of message bits the turbo code should
	 * 		handle
	 */
	static IEncoderPtr createEncoder(uint32_t lengthBits);

	/**
	 * Creates a new strider turbo decoder
	 * @param lengthBits: the number of message bits the turbo code should
	 * 		handle
	 */
	static ILLRDecoder::Ptr createDecoder(uint32_t lengthBits);

private:
	/**
	 * Returns a pseudo-random interleaver for strider
	 * @param lengthBits: the number of message bits the turbo code should
	 * 		handle
	 */
	static void getInterleaver(uint32_t lengthBits,
	                           std::vector<uint16_t>& interleavingSequence);
};
