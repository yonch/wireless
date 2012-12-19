/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "channels/CoherenceCoeffGenerator.h"

#include <assert.h>
#include <math.h>
#include <algorithm>

CoherenceCoeffGenerator::CoherenceCoeffGenerator(uint32_t coherenceInterval)
  : m_coherenceInterval(coherenceInterval),
    m_sqrt2inv(1.0 / sqrt(2.0)),
    m_random(0u),
    m_remaining(0)
{}

void CoherenceCoeffGenerator::seed(uint32_t *const seed, int seedSize)
{
	m_random.seed(seed, seedSize);
	randomizeCoefficient();
	m_remaining = m_random.randInt(m_coherenceInterval - 1) + 1;
}

FadingMagnitude CoherenceCoeffGenerator::next()
{
	if(m_remaining == 0) {
		m_remaining = m_coherenceInterval;
		randomizeCoefficient();
	}
	m_remaining--;

	return m_fadingCoeff;
}

void CoherenceCoeffGenerator::randomizeCoefficient()
{
	FadingMagnitude a = m_random.randNorm(0.0, m_sqrt2inv);
	FadingMagnitude b = m_random.randNorm(0.0, m_sqrt2inv);

	m_fadingCoeff = sqrtf(a*a + b*b);
}
