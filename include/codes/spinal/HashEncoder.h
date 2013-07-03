/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include "../../CodeBench.h"
#include "../IMultiStreamEncoder.h"

/**
 * \ingroup spinal
 * \brief Spinal encoder
 */
template<typename SpineValueType>
class HashEncoder : public IMultiStreamEncoder
{
public:
	/**
	 * Encodes the packet "packet" into real symbols in [-1,1].
	 * @param k: the number of message bits fed into the hash when forming a new
	 * 		spine value.
	 * @param spineLength: the number of spine values in a message.
	 * @param symbolSizeBits: the number of output bits from the hash used to
	 * 		create a symbol
	 **/
	HashEncoder(unsigned int k,
				unsigned int spineLength);

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet);

	/**
	 * Copies the specified encoded symbols into outSymbols
	 * @param spineValueIndices: the sequence of spine values where values
	 * 		should be emitted from
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			const std::vector<uint16_t>& spineValueIndices,
			std::vector<uint16_t>& outSymbols);

private:
	// number of bits that are incorporated into spine in each coding step
	const unsigned int m_k;

	// The number of bits in the encoded message
	const unsigned int m_numMessageBits;

	// The length of the spine
	const unsigned int m_spineLength;

	// Evaluator object for the spine
	std::vector<SpineValueType> m_spine;
};


#include "HashEncoder.hh"
