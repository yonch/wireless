/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/MatrixLDPCEncoder.h"

#include <stdexcept>

MatrixLDPCEncoder::MatrixLDPCEncoder(const MatrixLDPCCode& code,
                                     unsigned int numBitsPerSymbol)
  : m_code(code),
    m_numBitsPerSymbol(numBitsPerSymbol),
    m_numMessageBits(m_code.n * m_code.rateNumerator / m_code.rateDenominator),
    m_symbolXformer(code.Z, numBitsPerSymbol)
{
	if (code.Z > 64) {
		throw(std::runtime_error("Current implementation's word size too small"
								 " for requested matrix size Z"));
	}

	if (m_numMessageBits % m_code.Z != 0) {
		throw(std::runtime_error("Number of message bits should be a multiple "
								 "of Z"));
	}

	if(m_numMessageBits + (m_code.matrix.dim() * m_code.Z) != m_code.n) {
		throw(std::runtime_error("Dimension of code matrix does not match  rate"
								 "and codeword size"));
	}

	if(m_code.n % m_numBitsPerSymbol != 0) {
		throw(std::runtime_error("Total number of coded bits doesn't match "
								 "requested number of bits per symbol"));
	}

}

void MatrixLDPCEncoder::setPacket(const std::string & packet)
{
	m_packet = packet;
}

void MatrixLDPCEncoder::encode(unsigned int numSymbols,
                               std::vector<uint16_t>& outSymbols)
{
	unsigned int numOutputSymbols =
			(m_code.n + m_numBitsPerSymbol - 1) / m_numBitsPerSymbol;
	if(numSymbols != numOutputSymbols) {
		throw(std::runtime_error("Encoder only support output of ALL codewords"));
	}

	unsigned int numMessageWords = (m_numMessageBits / m_code.Z);
	unsigned int codewordNumWords = numMessageWords + m_code.matrix.dim();

	// Reserve space for intermediate words
	m_codeword.resize(codewordNumWords);

	// First, divide the packet into words
	transformWidthUnsafe<unsigned char,
						 const unsigned char*,
						 uint64_t,
						 std::vector<uint64_t>::iterator>(
								 (unsigned char*)m_packet.data(),
								 m_codeword.begin(),
								 8,
								 m_code.Z,
								 m_numMessageBits);

	// Now, go through the matrix, xoring into the check bits.
	unsigned long long firstParityWord = 0;
	for(unsigned int rind = 0; rind < m_code.matrix.dim(); rind++) {
		// Initialize the check bits
		unsigned long long parityWord = 0;

		// Go through the line, xoring the values in
		unsigned int rowDegree = m_code.matrix.rowDegree(rind);
		unsigned int rowPtr = m_code.matrix.rowPtr(rind);
		for(unsigned int i = 0; i < rowDegree; i++, rowPtr++) {
			unsigned int colInd = m_code.matrix.colInd(rowPtr);
			unsigned char shiftVal = m_code.matrix.val(rowPtr);

			parityWord ^= rotr( m_codeword[colInd], shiftVal );
		}

		// save parity word
		m_codeword[numMessageWords + rind] = parityWord;

		// add to calculation of the first parity word
		firstParityWord ^= parityWord;
	}

	// peel away all references to first parity word
	m_codeword[numMessageWords] ^= rotr(firstParityWord, 1);
	m_codeword[codewordNumWords - 1] ^= rotr(firstParityWord, 1);
	m_codeword[numMessageWords + (m_code.matrix.dim() >> 1)] ^= firstParityWord;

	// Extract the other parity words (except first parity word)
	for(unsigned int rind = m_code.matrix.dim() - 1; rind > 1; rind--) {
		m_codeword[numMessageWords + rind - 1] ^= m_codeword[numMessageWords + rind];
	}

	// write first parity word
	m_codeword[numMessageWords] = firstParityWord;

	// Divide the codeword into symbols
	m_symbolXformer.processVector(m_code.n,
								  m_codeword,
								  outSymbols);

}

uint64_t MatrixLDPCEncoder::rotr(uint64_t x,
                                 unsigned int amount)
{
	amount %= m_code.Z;
	return (((x >> amount) | (x << (m_code.Z - amount))) & ((1 << m_code.Z) - 1));
}
