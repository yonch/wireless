/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

/**
 * \ingroup util
 * \brief Composes two transformations into one transform.
 */
template<typename Transform1, typename Transform2>
class ComposedTransformation2 {
public:
	typedef typename Transform2::OutputType OutputType;

	/**
	 * C'tor
	 *
	 * @param transform1: the first transform applied to data
	 * @param transform2: the second transform applied
	 * @param numOutputSymbolsToReserve: the number of output symbols that
	 * 	   the temporary buffer should be able to hold. This parameter is
	 * 	   required so that there won't be reallocation of the temporary buffer.
	 * 	   Giving a wrong number will not hurt correctness, but may impact
	 * 	   performance.
	 */
	ComposedTransformation2(const Transform1& transform1,
	                        const Transform2& transform2,
	                        uint32_t numOutputSymbolsToReserve);

	/**
	 * Transforms the input symbols into the output vector, using the observed
	 * 		symbols to make a suitable transformation
	 */
	void transform(const std::vector<uint16_t>& inSymbols,
				   const std::vector<OutputType>& observedSymbols,
				   std::vector<OutputType>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	// Local copy of first transformation
	Transform1 m_transform1;

	// Local copy of second transformation
	Transform2 m_transform2;

	// buffer to hold the outputs of transform1 before feeding into transform2
	std::vector<typename Transform1::OutputType> m_intermediateBuffer;
};

template<typename Transform1, typename Transform2>
inline ComposedTransformation2<Transform1,Transform2>::ComposedTransformation2(
		const Transform1 & transform1,
		const Transform2 & transform2,
		uint32_t numOutputSymbolsToReserve)
  : m_transform1(transform1),
    m_transform2(transform2)
{
	m_intermediateBuffer.reserve(
			m_transform2.forecast(numOutputSymbolsToReserve));
}

template<typename Transform1, typename Transform2>
inline void ComposedTransformation2<Transform1,Transform2>::transform(
		const std::vector<uint16_t> & inSymbols,
		const std::vector<OutputType>& observedSymbols,
		std::vector<OutputType> & outSymbols)
{
	m_transform1.process(inSymbols, m_intermediateBuffer);
	m_transform2.transform(m_intermediateBuffer, observedSymbols, outSymbols);
}



template<typename Transform1, typename Transform2>
inline unsigned int ComposedTransformation2<Transform1, Transform2>::forecast(
		unsigned int numOutputs)
{
	unsigned int numIntermediate = m_transform2.forecast(numOutputs);
	return m_transform1.forecast(numIntermediate);
}
