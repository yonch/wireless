/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/fountain/RaptorDecoder.h"

#include "util/ItppUtils.h"

RaptorDecoder::RaptorDecoder(const std::string & ldpcFilename,
                             uint32_t numLtIterations)
: m_G(),
  m_ldpc(ldpcFilename, &m_G),
  m_lt(m_ldpc.get_nvar(),
       8 * m_ldpc.get_nvar(),
       numLtIterations),
  m_llrs(m_ldpc.get_nvar(), 0),
  m_itppLlrs(m_ldpc.get_nvar()),
  m_decodedBits(m_ldpc.get_ninfo())
{}

void RaptorDecoder::reset()
{
	m_lt.reset();
}

void RaptorDecoder::add(const std::vector<LLRValue> & llrs)
{
	m_lt.add(llrs);
}

DecodeResult RaptorDecoder::decode()
{
	// Perform decode of LT code
	m_lt.softDecode(m_llrs);

	// Copy result into the itpp LLR vec
	for(uint32_t i = 0; i < m_llrs.size(); i++) {
		m_itppLlrs[i] = m_llrs[i];
	}

	// Perform LDPC decode
	m_ldpc.decode(m_itppLlrs, m_decodedBits);

	DecodeResult res;
	ItppUtils::vectorToString(m_decodedBits, res.packet);

	return res;
}

