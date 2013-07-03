/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup mappers
 * \brief Helper functions to compute Normal Distribution.
 *
 * Numerical calculations of properties of the normal distribution.
 *
 * Implementation from http://home.online.no/~pjacklam/notes/invnorm/
 * The C++ source file implemented by Jeremy Lea, and supposedly freely
 * 		available to "Use them as you wish, for whatever purpose" as stated on
 * 		http://home.online.no/~pjacklam/notes/invnorm/
 *
 * This implementation is suitable for the standard normal distribution
 * 		(ie mu=0, sigma^2=1)
 */
class NormalDistribution {
public:
	/**
	 * Returns the Probability Density Function at point u
	 *
	 * Based on Jeremy Lea's stdnormal_pdf
	 */
	static double pdf(double u);

	/**
	 * Returns the Cumulative Distribution Function at point u
	 *
	 * Based on Jeremy Lea's stdnormal_cdf, which also includes the notice:
	 *
	 * The standard normal CDF, for one random variable.
	 *
	 *   Author:  W. J. Cody
	 *   URL:   http://www.netlib.org/specfun/erf
	 *
	 * This is the erfc() routine only, adapted by the
	 * transform stdnormal_cdf(u)=(erfc(-u/sqrt(2))/2;
	 */
	static double cdf(double u);

	/**
	 * Returns the Percentage Point Function for p in (0,1)
	 *
	 * Based on Jeremy Lea's stdnormal_inv
	 */
	static double ppf(double p);
};
