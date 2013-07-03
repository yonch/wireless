/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#pragma once

#include <stdint.h>
#include "IPuncturingSchedule.h"

// Forward declaration
class StridedProtocol;

/**
 * \ingroup puncturing
 * \brief Splits the symbols stream to 8 sub-groups, then sends symbols from each sub-group
 *
 * We try to puncture evenly, so that there isn't a stream that is left
 * 		behind. The last stream is a special case because in spinal decoding, the
 * 		errors concentrate in the last few bits. In order to make sure the
 * 		decoder	chooses the	correct suggestion from the list, we need to supply
 * 		more symbols for the last stream. This has the effect of making
 * 		the true suggestion's likelihood "stand out" from the others.
 *
 * This implementation splits all symbols from a single coding pass into 8
 * 		sub-passes. Each sub-pass emits symbols from streams that are 8
 * 		apart, with some offset 0 to 7. In addition, "numLastStepSymbolsPerPass"
 * 		symbols are	emitted for the last spine value. If the number of symbols
 * 		from a full pass (one symbol from each ordinary stream, and
 * 		'numLastStepSymbolsPerPass' from the last stream) is not a multiple
 * 		of 8, then some sub-passes that contain less symbols than other
 * 		sub-passes.
 */
class StridedPuncturingSchedule	: public IPuncturingSchedule
{
public:
	/**
	 * @param numStreams: total number of code steps
	 * @param numLastStreamSymbolsPerPass: the number of symbols to emit from the
	 * 		last stream in every pass, in addition to all other symbols
	 */
	StridedPuncturingSchedule(unsigned int numStreams,
	                          	  unsigned int numLastStreamSymbolsPerPass);

	/**
	 * @see IPuncturingSchedule
	 */
	virtual void reset();

	/**
	 * @see IPuncturingSchedule
	 */
	virtual void batchNext(unsigned int numSymbols,
	                       	  std::vector<uint16_t>& streamIndices);

	/**
	 * Returns next spine value from which a symbol is derived
	 */
	uint16_t next();

private:
	friend class StridedProtocol;

	const unsigned int m_numStreams;
	const unsigned int m_numLastStreamSymbolsPerFullPass;
	const unsigned int m_numSymbolsPerFullPass;

	static const unsigned int m_moduluOfSubpass[8];

	// Contains the offsets that this puncturing schedule should use in sequence
	std::vector<uint16_t> m_offsets;

	unsigned int m_nextStreamIndex;
	unsigned int m_currentSubpassIndex;
};
