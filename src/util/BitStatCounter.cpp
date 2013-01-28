/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/BitStatCounter.h"

BitStatCounter::BitStatCounter(uint32_t num_bits)
  : m_num_bits(num_bits),
    m_stats(num_bits, 0)
{}

void BitStatCounter::process(const std::string& s) {
	for(uint32_t i = 0; i < m_num_bits; i++) {
		m_stats[i] += (uint8_t(s[i/8]) >> (i % 8)) & 0x1;
	}
}

uint32_t BitStatCounter::query(uint32_t bit_index) {
	return m_stats[bit_index];
}


