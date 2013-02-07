/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/BlockStatCounter.h"

#include <stdexcept>
#include "util/Utils.h"

BlockStatCounter::BlockStatCounter(uint32_t num_bits, uint32_t k)
  : m_num_bits(num_bits),
    m_k(k),
    m_num_blocks(num_bits / m_k),
    m_stats(m_num_blocks, 0)
{
	if((m_num_bits % m_k) != 0) {
		throw std::runtime_error("String length must be divisible by block length");
	}

	if(m_k > 32) {
		throw std::runtime_error("Currently only <= 32 bit blocks are supported.");
	}
}

void BlockStatCounter::process(const std::string& s) {
	// Divide the string bits into blocks of k bits
	Utils::blockify(s, m_num_bits, m_k, m_tmp_blocks);

	for(uint32_t i = 0; i < m_num_blocks; i++) {
		if(m_tmp_blocks[i] != 0) {
			m_stats[i]++;
		}
	}
}

uint32_t BlockStatCounter::query(uint32_t block_index) {
	return m_stats[block_index];
}


