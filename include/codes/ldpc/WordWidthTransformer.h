/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

#include "../../CodeBench.h"

/**
 * \ingroup ldpc
 * \brief Changes memory representation from k_1-bit blocks to k_2-bit blocks
 * Converts a stream of bits from one grouping to another (ie 6-bit words stored
 * 		in uchar to 61-bit words stored in uint64)
 *
 * Bits are assumed to be in little endian, ie first word contains least
 * 		significant bits.
 */
template<typename SrcType, typename DstType>
class WordWidthTransformer {
public:
	/**
	 * @param srcBitsPerWord: number of bits per source word
	 * @param dstBitsPerWord: number of bits per destination word
	 */
	WordWidthTransformer(unsigned int srcBitsPerWord,
						 unsigned int dstBitsPerWord);

	/**
	 * Copy bits to different grouping.
	 * @param numBits: number of bits to transfer from source to destination
	 * @param source: The source data
	 * @param dest: [out] where newly grouped data is written.
	 */
	void processArray(unsigned int numBits,
	                  const SrcType* source,
	                  int sourceSize,
	                  DstType* dest,
	                  int destSize);

	/**
	 * Copy bits to different grouping.
	 * @param numBits: number of bits to transfer from source to destination
	 * @param source: The source data
	 * @param dest: [out] where newly grouped data is written.
	 */
	void processVector(unsigned int numBits,
	                   const std::vector<SrcType>& source,
	                   std::vector<DstType>& dest);


private:
	// Number of bits per source word
	unsigned int m_srcBitsPerWord;

	// Number of bits per destination word
	unsigned int m_dstBitsPerWord;
};

template<typename SrcType, typename SrcIter, typename DstType, typename DstIter>
void transformWidthUnsafe(SrcIter source,
						  DstIter dest,
						  unsigned int srcBitsPerWord,
						  unsigned int dstBitsPerWord,
						  unsigned int numBits);


#include "WordWidthTransformer.hh"
