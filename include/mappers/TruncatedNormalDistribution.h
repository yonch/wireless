/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup mappers
 * \brief Produces a elements from gaussian distribution, limited to a range (a,b).
 *
 * Calculates values associated with the Truncated Normal Distribution,
 * 		see http://en.wikipedia.org/wiki/Truncated_normal_distribution
 *
 * This implementation only deals with truncations of the standard normal
 * 		mu = 0, sigma^2 = 1.
 */
class TruncatedNormalDistribution {
public:
	/**
	 * C'tor
	 * @param a the lower truncation point
	 * @param b the upper truncation point
	 */
	TruncatedNormalDistribution(double a, double b);

	/**
	 * @return the Percentage Point Function for p in (0,1)
	 */
	double ppf(double p);

	/**
	 * @return the variance of the truncated normal
	 */
	double variance();

private:
	// The lower truncation point
	double m_a;

	// The upper truncation point
	double m_b;

	// The CDF of point a on a standard normal variable
	double m_cdfOfA;

	// The probability of a standard normal to fall between a and b
	double m_weightGivenAB;
};
