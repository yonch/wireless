/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/ItppUtils.h"

#include <stdexcept>

void ItppUtils::stringToVector(const std::string & src, itpp::bvec & dst)
{
	uint32_t dstSizeBits = dst.size();
	uint32_t dstSizeBytes = (dstSizeBits + 7) / 8;

	// Check input size
	if(src.size() != dstSizeBytes) {
		throw(std::runtime_error("String size incompatible with destination's size"));
	}

	// convert the string to a bvec
	int bitInd = 0;
	// All but the last byte
	for(unsigned int i = 0; i + 1 < dstSizeBytes; i++) {
		unsigned char byte = static_cast<unsigned char>(src[i]);
		for(int b = 0; b < 8; b++) {
			dst[bitInd++] = (byte >> b) & 0x1;
		}
	}
	// Last byte
	unsigned char byte = static_cast<unsigned char>(src[dstSizeBytes - 1]);
	for(int b = 0; (b < 8) && (bitInd < dst.size()); b++) {
		dst[bitInd++] = (byte >> b) & 0x1;
	}
}

void ItppUtils::vectorToString(const itpp::bvec & src, std::string & dst)
{
	uint32_t srcSizeBits = src.size();
	uint32_t srcSizeBytes = (srcSizeBits + 7) / 8;

	// Resize string to the appropriate size
	dst.resize(srcSizeBytes);

	// Put the bits into the string
	uint32_t bitInd = 0;
	// All but the last byte
	for(unsigned int i = 0; i + 1 < srcSizeBytes; i++) {
		uint8_t byte = 0;
		for(int b = 0; b < 8; b++) {
			byte |= (uint8_t(bool(src[bitInd++])) & 0x1) << b;
		}
		dst[i] = byte;
	}
	// Last byte
	uint8_t byte = 0;
	for(int b = 0; (b < 8) && (bitInd < srcSizeBits); b++) {
		byte |= (uint8_t(bool(src[bitInd++])) & 0x1) << b;
	}
	dst[srcSizeBytes - 1] = byte;
}

