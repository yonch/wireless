/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include <string>
#include <vector>

/**
 * \ingroup util
 * \brief Counts statistics of individual bits in a series of strings.
 *
 * The user processes multiple strings, then queries for different bits i how many times bit i was 1.
 */
class BitStatCounter {
public:
	/**
	 * C'tor
	 * @param num_bits: The length of strings whose statistics are gathered.
	 */
	BitStatCounter(uint32_t num_bits);

	/**
	 * Processes the string, keeping statistics
	 * @param s: the string whose bits should be processed.
	 */
	void process(const std::string& s);

	/**
	 * Queries the statistics of a bit
	 * @param bit_index: statistics of which bit to get (must be 0 <= bit_index < m_num_bits)
	 * @returns: the number of processed string where bit bit_index was 1.
	 */
	uint32_t query(uint32_t bit_index);

private:
	// The number of bits to get statistics for
	const uint32_t m_num_bits;

	// The number of 1 occurences for each bit
	std::vector<uint32_t> m_stats;
};
