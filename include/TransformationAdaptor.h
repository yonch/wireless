/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

/**
 * \ingroup util
 * \brief Adapts classes with process() method to transform() method
 *
 * Adapts a Processor instance (that has method process(in,out)) to a
 * 	   transformation instance (with method transform(in, observed, out))
 *
 * Transformations are used in estimation in likeihoods, where a proposed symbol
 * 	   needs to be transformed like the received symbol, so the distance between
 * 	   the two can be calculated.
 */
template<typename Processor>
class TransformationAdaptor {
public:
	typedef typename Processor::InputType InputType;
	typedef typename Processor::OutputType OutputType;

	/**
	 * C'tor
	 *
	 * @param processor: the processor to adapt
	 */
	TransformationAdaptor(const Processor& processor);

	/**
	 * Transforms the input symbols using the processor, ignoring the
	 * 		observed symbols.
	 */
	void transform(const std::vector<InputType>& inSymbols,
				   const std::vector<OutputType>& observedSymbols,
				   std::vector<OutputType>& outSymbols);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	unsigned int forecast(unsigned int numOutputs);

private:
	// Local copy of processor
	Processor m_processor;
};

template<typename Processor>
inline TransformationAdaptor<Processor>::TransformationAdaptor(
		const Processor & processor)
  : m_processor(processor)
{}

template<typename Processor>
inline void TransformationAdaptor<Processor>::transform(
		const std::vector<InputType> & inSymbols,
		const std::vector<OutputType> &, // unused
		std::vector<OutputType> & outSymbols)
{
	m_processor.process(inSymbols, outSymbols);
}

template<typename Processor>
inline unsigned int TransformationAdaptor<Processor>::forecast(
		unsigned int numOutputs)
{
	return m_processor.forecast(numOutputs);
}
