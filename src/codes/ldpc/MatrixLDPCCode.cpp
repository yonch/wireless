/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/MatrixLDPCCode.h"
#include <stdexcept>

MatrixLDPCCode::MatrixLDPCCode(	unsigned int _n,
								unsigned int _Z,
								unsigned int _rateNumerator,
								unsigned int _rateDenominator,
								const unsigned int *rowPtrs,
								unsigned int numValues,
								const unsigned char *vals,
								const unsigned int *cols)
  : n(_n),
    Z(_Z),
    rateNumerator(_rateNumerator),
    rateDenominator(_rateDenominator),
    matrix(n * (rateDenominator - rateNumerator) / (rateDenominator * Z),
           rowPtrs,
           numValues,
           vals,
           cols)
{
	// sanity checks
	if(n % Z != 0) {
		throw(std::runtime_error("n should be a multiple of Z"));
	}
	if((n * rateNumerator) % (rateDenominator * Z) != 0) {
		throw(std::runtime_error("there should be a whole number of submatrices"
								 " for message part and for parity part."));
	}
}


