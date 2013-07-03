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
 * \brief Counts statistics of k-bit blocks in a series of strings.
 *
 * The user processes multiple strings, then queries for different blocks i how many times block i
 *     was != 0.
 */
class BlockStatCounter {
public:
	/**
	 * C'tor
	 * @param num_bits: The length of strings whose statistics are gathered.
	 * @param k: The block length.
	 * @note num_bits must be divisible by k
	 */
	BlockStatCounter(uint32_t num_bits, uint32_t k);

	/**
	 * Processes the string, keeping statistics
	 * @param s: the string whose bits should be processed.
	 */
	void process(const std::string& s);

	/**
	 * Queries the statistics of a block
	 * @param block_index: statistics of which block to get (must be
	 * 		0 <= block_index < m_num_bits / m_k)
	 * @returns: the number of processed string where block block_index was != 0.
	 */
	uint32_t query(uint32_t block_index);

private:
	// The number of bits to get statistics for
	const uint32_t m_num_bits;

	// The block size
	const uint32_t m_k;

	// The number of blocks in each string
	const uint32_t m_num_blocks;

	// The number of 1 occurences for each bit
	std::vector<uint32_t> m_stats;

	// A temp vector to blockify strings into.
	std::vector<uint32_t> m_tmp_blocks;
};
