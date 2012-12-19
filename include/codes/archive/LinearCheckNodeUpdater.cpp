/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/LinearCheckNodeUpdater.h"
#include <math.h>
#include <stdexcept>
#include <assert.h>

LinearCheckNodeUpdater::LinearCheckNodeUpdater(unsigned int numCheckNodes)
  : m_checkNodesPriorLogPmQ(numCheckNodes,
          	  	  	  	  	LogPmQ(logf(+0.0f)))
{
	m_messagesPmQ.reserve(100); // this is to reduce resizes
	m_rightSum.reserve(100); // this is to reduce resizes
}

LinearCheckNodeUpdater::LinearCheckNodeUpdater(
										float *checkNodePriorsLLR,
										unsigned int checkNodePriorsLLRSize)
{
	for(unsigned int i = 0; i < checkNodePriorsLLRSize; i++) {
		m_checkNodesPriorLogPmQ.push_back(LogPmQ(checkNodePriorsLLR[i] * 0.5f));
	}
	m_messagesPmQ.reserve(100); // this is to reduce resizes
	m_rightSum.reserve(100); // this is to reduce resizes
}

unsigned int LinearCheckNodeUpdater::size()
{
	return m_checkNodesPriorLogPmQ.size();
}

void LinearCheckNodeUpdater::update(MultiVector<float> & messages)
{
	uint32_t N_nodes = messages.size();

	assert(N_nodes == m_checkNodesPriorLogPmQ.size());

	// for each check node
	for (uint32_t node_ind = 0; node_ind < N_nodes; node_ind++) {
		updateNode(node_ind, messages);
	}

}

void LinearCheckNodeUpdater::updateNode(
										unsigned int nodeInd,
										MultiVector<float>& messages)

{
	uint32_t begin = messages.begin(nodeInd);
	uint32_t end = messages.end(nodeInd);
	uint32_t N = end - begin;

	if(N == 1) {
		messages[begin] = m_checkNodesPriorLogPmQ[nodeInd].getFloat();
		return;
	}

	// Convert all message values into log(p-q) values
	for (unsigned int j = begin; j < end; j++) {
		m_messagesPmQ.push_back(LogPmQ(messages[j]));
	}

	// calculate the multiplication from the right of p-q values:
	// rightSum[j] = encodedSoftBit[i] * messages[-1].p-q * messages[-2].p-q * ... * messages[-j].p-q
	LogPmQ curSum = m_checkNodesPriorLogPmQ[nodeInd]; // get log(p-q)

	m_rightSum.push_back(curSum);
	// go over all messages except the first one (which will never be
	// needed anyway)
	for(unsigned int rightInd = N - 1; rightInd > 0; rightInd--) {
		curSum += m_messagesPmQ[rightInd];

		m_rightSum.push_back(curSum);
	}


	// special case for j = 0
	messages[begin] = m_rightSum[N - 1].getFloat();

	// now we use curSum as the cumulative multiplication from the
	// left rather than right
	curSum = m_messagesPmQ[0];

	for (unsigned int j = 1; j < N; j++) {
		messages[begin + j] = (curSum + m_rightSum[N - j - 1]).getFloat();

		//assert(isfinite(messages[begin + j]));

		curSum += m_messagesPmQ[j];
	}

	m_rightSum.clear();
	m_messagesPmQ.clear();
}


/*****************************
 * class LogPmQ
 */

LogPmQ::LogPmQ(float llr2Value)
  : m_isPos( !signbit(llr2Value) ),
    m_val( logf(tanhf(fabsf(llr2Value))) )
{}

LogPmQ & LogPmQ::operator +=(const LogPmQ & rhs) {
	m_isPos ^= rhs.m_isPos;
	m_val += rhs.m_val;
	return(*this);
}


const LogPmQ LogPmQ::operator +(const LogPmQ & other) const {
	return (LogPmQ(*this) += other);
}


float LogPmQ::getFloat() const {
	// Get back | \Pi tanh(|LLR|/2) |
	float mulOfTanh = expf(m_val);

	// Account for sign of LLR
	if (!m_isPos) {
		mulOfTanh = copysign(mulOfTanh, -1.0f);
	}

	return atanhf(mulOfTanh);
}




//ostream& operator<< (ostream& out, const LogPmQ& logPmQ) {
//	return (out << "<"
//			    << "(" << (2 * logPmQ.m_isPos - 1) << "," << logPmQ.m_val <<")"
//			    << "~" << logPmQ.getFloat() << ">");
//}
