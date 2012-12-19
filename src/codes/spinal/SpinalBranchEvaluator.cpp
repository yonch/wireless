/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/spinal/SpinalBranchEvaluator.h"

#include <complex>
#include "util/Utils.h"

// DISTANCE FUNCTIONS

uint64_t IntegerEuclidianDistance::dist(Symbol x, Symbol y)
{
	int64_t delta = ((int64_t)(x) - (int64_t)(y));
	return delta * delta;
}

uint32_t HammingDistance::dist(Symbol x, Symbol y)
{
	return Utils::popcount_2(x ^ y);
}

double FadingEuclidianDistance::dist(FadingSymbol x,
									FadingSymbol y)
{
	SoftSymbol delta = (x.symbol - y.symbol);
	return double(delta * delta);
}

double SoftEuclidianDistance::dist(SoftSymbol x, SoftSymbol y)
{
	SoftSymbol delta = x - y;
	return double(delta * delta);
}






