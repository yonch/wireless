/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#pragma once

#include <tr1/memory>
#include <vector>
#include <stdint.h>

// forward declaration
class IPuncturingSchedule;

typedef std::tr1::shared_ptr<IPuncturingSchedule> IPuncturingSchedulePtr;

/**
 * \ingroup puncturing
 * \brief Interface for classes that specify puncturing schedules
 *
 * The IPuncturingSchedule specifies what sequence of streams of an IMultiStreamEncoder is used
 *     to produce a codeword.
 */
class IPuncturingSchedule {
public:
	/**
	 * Virtual D'tor
	 */
	virtual ~IPuncturingSchedule() {}

	/**
	 * Resets the instance to the beginning of the puncturing schedule
	 */
	virtual void reset() = 0;

	/**
	 * Returns next stream indices from which a symbol is derived
	 */
	virtual void batchNext(unsigned int numSymbols,
	                       std::vector<uint16_t>& streamIndices) = 0;
};
