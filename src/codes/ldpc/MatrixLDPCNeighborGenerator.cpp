/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/MatrixLDPCNeighborGenerator.h"


MatrixLDPCNeighborGenerator::MatrixLDPCNeighborGenerator(
		const MatrixLDPCCode& code)
  : m_code(code),
    m_firstParityBitIndex(code.n * code.rateNumerator / code.rateDenominator),
    m_row(0),
    m_bitIndex(0),
    m_neighborIndex(0)
{}

void MatrixLDPCNeighborGenerator::reset() {
	m_row = 0;
	m_bitIndex = 0;
	m_neighborIndex = 0;
}

void MatrixLDPCNeighborGenerator::set(unsigned long bitNumber) {
	m_row = bitNumber / m_code.Z;
	m_bitIndex = bitNumber % m_code.Z;
	m_neighborIndex = 0;
}

void MatrixLDPCNeighborGenerator::nextNode() {
	m_bitIndex++;
	if (m_bitIndex == m_code.Z) {
		m_bitIndex = 0;
		m_row++;
	}
	m_neighborIndex = 0;
}

unsigned int MatrixLDPCNeighborGenerator::count() {
	if(m_row == (m_code.matrix.dim() >> 1)) {
		return(m_code.matrix.rowDegree(m_row) + 3);
	} else {
		return(m_code.matrix.rowDegree(m_row) + 2);
	}
}

unsigned int MatrixLDPCNeighborGenerator::next() {
	unsigned int rowDegree = m_code.matrix.rowDegree(m_row);

	if(m_neighborIndex < rowDegree) {
		unsigned int ptr = m_code.matrix.rowPtr(m_row) + m_neighborIndex;
		unsigned int col = m_code.matrix.colInd(ptr);
		unsigned int val = m_code.matrix.val(ptr);
		m_neighborIndex++;

		return((col * m_code.Z) + ((val + m_bitIndex) % m_code.Z));
	} else if(m_row == (m_code.matrix.dim() >> 1)) {
		switch(m_neighborIndex++ - rowDegree) {
		case 0: return((m_firstParityBitIndex + 0 * m_code.Z) + m_bitIndex);
		case 1: return((m_firstParityBitIndex + m_row * m_code.Z) + m_bitIndex);
		case 2: return((m_firstParityBitIndex + (m_row+1) * m_code.Z) + m_bitIndex);
		default:throw(std::runtime_error("Too many calls to next()!"));
		}
	} else if(m_row == 0) {
		switch(m_neighborIndex++ - rowDegree) {
		case 0: return(m_firstParityBitIndex + ((m_bitIndex + 1) % m_code.Z));
		case 1: return(m_firstParityBitIndex + m_code.Z + m_bitIndex);
		default:throw(std::runtime_error("Too many calls to next()!"));
		}
	} else if(m_row == m_code.matrix.dim() - 1) {
		switch(m_neighborIndex++ - rowDegree) {
		case 0: return(m_firstParityBitIndex + ((m_bitIndex + 1) % m_code.Z));
		case 1: return(m_firstParityBitIndex + m_row * m_code.Z + m_bitIndex);
		default:throw(std::runtime_error("Too many calls to next()!"));
		}
	} else {
		switch(m_neighborIndex++ - rowDegree) {
		case 0: return((m_firstParityBitIndex + m_row * m_code.Z) + m_bitIndex);
		case 1: return((m_firstParityBitIndex + (m_row+1) * m_code.Z) + m_bitIndex);
		default:throw(std::runtime_error("Too many calls to next()!"));
		}
	}
}

bool MatrixLDPCNeighborGenerator::hasMore() {
	return(count() > m_neighborIndex);
}
