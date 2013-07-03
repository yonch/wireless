/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include <vector>
#include "IPuncturingSchedule.h"

/**
 * \ingroup puncturing
 * \brief Punctures according to a given vector
 *
 * Instances are given a vector with the puncturing schedule they provide.
 *
 * There is support for changing the schedule while running.
 */
class StaticPuncturingSchedule	: public IPuncturingSchedule
{
public:
	/**
	 * C'tor.
	 *
	 * Starts with an empty schedule.
	 */
	StaticPuncturingSchedule();

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

	/**
	 * Sets the puncturing schedule
	 */
	void set(const std::vector<uint16_t>& schedule);

private:
	// List of locations to return as a puncturing schedule
	std::vector<uint16_t> m_schedule;

	// The index into m_schedule where the next puncturing location is.
	unsigned int m_nextIndex;
};
