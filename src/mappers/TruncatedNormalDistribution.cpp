/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "mappers/TruncatedNormalDistribution.h"

#include <math.h>
#include "mappers/NormalDistribution.h"

TruncatedNormalDistribution::TruncatedNormalDistribution(double a, double b)
  : m_a(a),
    m_b(b),
    m_cdfOfA(NormalDistribution::cdf(a)),
    m_weightGivenAB(NormalDistribution::cdf(b) - m_cdfOfA)
{}



double TruncatedNormalDistribution::ppf(double p) {
	return NormalDistribution::ppf(m_cdfOfA + (m_weightGivenAB * p));
}



double TruncatedNormalDistribution::variance() {
	// Formula at http://en.wikipedia.org/wiki/Truncated_normal_distribution
	double secondTerm =
			(  (m_a * NormalDistribution::pdf(m_a))
			 - (m_b * NormalDistribution::pdf(m_b))) / m_weightGivenAB;
	double thirdTermBeforeSqrt =
			(  NormalDistribution::pdf(m_a)
			 - NormalDistribution::pdf(m_b)) / m_weightGivenAB;

	return (  1
			+ secondTerm
			- sqrt(thirdTermBeforeSqrt));
}



