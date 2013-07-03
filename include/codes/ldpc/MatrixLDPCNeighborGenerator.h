/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "SparseMatrix.h"
#include "MatrixLDPCCode.h"

/**
 * \ingroup ldpc
 * \brief Outputs graphical structure of LDPC code, for belief propagation.
 */
class MatrixLDPCNeighborGenerator {
public:
	MatrixLDPCNeighborGenerator(const MatrixLDPCCode& code);

	/**
	 * restart at the beginning
	 */
	void reset();

	/**
	 * Sets the observed node we're getting neighbors for to 'observedBitIndex'
	 */
	void set(unsigned long observedBitIndex);

	/**
	 * Advances to next check node
	 */
	void nextNode();

	/**
	 * @return the number of neighbors for the specific message bit
	 */
	unsigned int count();

	/**
	 * @return the next neighbor
	 */
	unsigned int next();

	/**
	 * @return true if there are more neighbors, false otherwise
	 *
	 * It will return true before count() calls to next() had been performed.
	 **/
	bool hasMore();

private:
	MatrixLDPCCode m_code;

	// the index of the first parity bit
	unsigned int m_firstParityBitIndex;

	// current matrix row
	unsigned int m_row;

	// current bitIndex in 0..Z-1
	unsigned int m_bitIndex;

	// current index into neighbors
	unsigned int m_neighborIndex;
};
