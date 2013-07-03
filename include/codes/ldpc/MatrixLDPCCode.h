/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "SparseMatrix.h"

/**
 * \ingroup ldpc
 * \brief Specification of a Quasi-Cyclic LDPC code
 */
struct MatrixLDPCCode {
	MatrixLDPCCode(unsigned int _n,
	               unsigned int _Z,
	               unsigned int _rateNumerator,
	               unsigned int _rateDenominator,
	               const unsigned int* rowPtrs,
	               unsigned int numValues,
	               const unsigned char* vals,
	               const unsigned int* cols);

	// Size of codeword
	unsigned int n;

	// Size of cyclic-permutation submatrices
	unsigned int Z;

	// the numerator of the rate
	unsigned int rateNumerator;
	// the denominator of the rate
	unsigned int rateDenominator;

	// matrix of the message bits each check node xors (this does not include
	// how parity bits are xored in check nodes - we assume the almost-diagonal
	// structure in 802.11n-2009
	SparseMatrix<unsigned char> matrix;
};

