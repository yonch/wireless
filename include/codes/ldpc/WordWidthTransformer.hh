/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <stdexcept>
#include <algorithm>

template<typename SrcType, typename DstType>
inline WordWidthTransformer<SrcType,DstType>::WordWidthTransformer(
		unsigned int srcBitsPerWord, unsigned int dstBitsPerWord)
  : m_srcBitsPerWord(srcBitsPerWord),
    m_dstBitsPerWord(dstBitsPerWord)
{}


template<typename SrcType, typename SrcIter, typename DstType, typename DstIter>
inline void transformWidthUnsafe(SrcIter source,
                                 DstIter dest,
                                 unsigned int srcBitsPerWord,
                                 unsigned int dstBitsPerWord,
                                 unsigned int numBits)
{
	SrcType srcbuf = 0;
	DstType dstbuf = 0;

	// number of bits remaining in srcbuf
	unsigned int srcN = 0;
	// number of bits already in dstbuf
	unsigned int dstN = 0;

	while(numBits > 0) {
		// reload another source word if necessary
		if(srcN == 0) {
			srcbuf = *(source++);
			srcN = srcBitsPerWord;
		}
		// advance dstN if necessary
		 if (dstN == dstBitsPerWord) {
			*(dest++) = dstbuf;
			dstbuf = 0;
			dstN = 0;
		 }

		// how many bits should we take
		unsigned int amt = std::min(srcN, dstBitsPerWord - dstN);

		// take 'amt' bits from source to destination, preserving endianity
		dstbuf |= ((srcbuf >> (srcBitsPerWord - srcN)) & ((1 << amt) - 1))
						<< dstN;
		dstN += amt;
		srcN -= amt;
		numBits-= amt;
	}

	// if dstbuf has not been written and needs to be written, write it.
	if(dstN > 0) {
		*(dest) = dstbuf;
	}
}

template<typename SrcType, typename DstType>
inline void WordWidthTransformer<SrcType,DstType>::processArray(
		unsigned int numBits,
		const SrcType *source,
		int sourceSize,
		DstType *dest,
		int destSize)
{
	if(sourceSize * m_srcBitsPerWord < numBits) {
		throw std::runtime_error("Source buffer too small for requested"
				"number of bits");
	}
	if (destSize * m_dstBitsPerWord < numBits) {
		throw std::runtime_error("Destination buffer too small for requested"
				"number of bits");
	}
	if(sourceSize == 0) {
		return;
	}

	transformWidthUnsafe<SrcType, const SrcType*, DstType, DstType*>(
			source,
			dest,
			m_srcBitsPerWord,
			m_dstBitsPerWord,
			numBits);
}

template<typename SrcType, typename DstType>
inline void WordWidthTransformer<SrcType,DstType>::processVector(
		unsigned int numBits,
		const std::vector<SrcType> & source,
		std::vector<DstType> & dest)
{
	if(source.size() * m_srcBitsPerWord < numBits) {
		throw std::runtime_error("Source vector too small for requested"
				"number of bits");
	}

	dest.clear();
	dest.resize((numBits + m_dstBitsPerWord - 1) / m_dstBitsPerWord);

	if(source.size() == 0) {
		return;
	}

	transformWidthUnsafe<SrcType,
						 typename std::vector<SrcType>::const_iterator,
						 DstType,
						 typename std::vector<DstType>::iterator>(
			source.begin(),
			dest.begin(),
			m_srcBitsPerWord,
			m_dstBitsPerWord,
			numBits);
}
