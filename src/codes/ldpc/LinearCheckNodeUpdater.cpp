/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/LinearCheckNodeUpdater.h"
#include <math.h>
#include <stdexcept>
#include <assert.h>

//#undef SUPER_VERBOSE
//#define SUPER_VERBOSE

#ifdef SUPER_VERBOSE
#include <iostream>
#endif

LinearCheckNodeUpdater::LinearCheckNodeUpdater(
		unsigned int numCheckNodes,
		short int Dint1,
		short int Dint2,
		short int Dint3)
  : m_llrCalc(Dint1, Dint2, Dint3),
	m_checkNodesPriorQLLR(numCheckNodes,
          	  	  	  	  itpp::QLLR_MAX)
{
	m_messagesWorkArr.reserve(100); // this is to reduce resizes
	m_rightSum.reserve(100); // this is to reduce resizes
}

LinearCheckNodeUpdater::LinearCheckNodeUpdater(
		const std::vector<float>& priorLLRs,
		short int Dint1,
		short int Dint2,
		short int Dint3)
  : m_llrCalc(Dint1, Dint2, Dint3)
{
	for(unsigned int i = 0; i < priorLLRs.size(); i++) {
		m_checkNodesPriorQLLR.push_back(m_llrCalc.to_qllr(priorLLRs[i]));
	}
	m_messagesWorkArr.reserve(100); // this is to reduce resizes
	m_rightSum.reserve(100); // this is to reduce resizes
}


void LinearCheckNodeUpdater::update(MultiVector<BipartiteBP::QLLR> & messages)
{
	uint32_t N_nodes = messages.size();

	assert(N_nodes == m_checkNodesPriorQLLR.size());

#ifdef SUPER_VERBOSE
	std::cout << "Check Node NEW ITERATION *******" << std::endl;
#endif

	// for each check node
	for (uint32_t node_ind = 0; node_ind < N_nodes; node_ind++) {
		updateNode(node_ind, messages);
	}
}

void LinearCheckNodeUpdater::updateNode(
										unsigned int nodeInd,
										MultiVector<BipartiteBP::QLLR>& messages)

{
	uint32_t begin = messages.begin(nodeInd);
	uint32_t end = messages.end(nodeInd);
	uint32_t N = end - begin;

#ifdef SUPER_VERBOSE
		std::cout << "CheckNode " << nodeInd << " \t" << N << " neighbors: ";
		for (uint32_t jj = 0; jj < N; jj++) {
			std::cout << messages[begin + jj] << ", ";
		}
		std::cout << std::endl;
#endif


	if(N == 1) {
		messages[begin] = m_checkNodesPriorQLLR[nodeInd];
		return;
	}

	// Convert all message values into log(p-q) values
	for (unsigned int j = begin; j < end; j++) {
		m_messagesWorkArr.push_back(messages[j]);
	}

	// calculate the multiplication from the right of p-q values:
	// rightSum[j] = encodedSoftBit[i] * messages[-1].p-q * messages[-2].p-q * ... * messages[-j].p-q
	itpp::QLLR curSum = m_checkNodesPriorQLLR[nodeInd]; // get log(p-q)

	m_rightSum.push_back(curSum);
	// go over all messages except the first one (which will never be
	// needed anyway)
	for(unsigned int rightInd = N - 1; rightInd > 0; rightInd--) {
#ifdef SUPER_VERBOSE
		std::cout << "\t\tcurSum=" << curSum << " qllr=" << m_messagesWorkArr[rightInd] <<
				" boxPlus=" << m_llrCalc.Boxplus(curSum, m_messagesWorkArr[rightInd]) << std::endl;
#endif

		curSum = m_llrCalc.Boxplus(curSum, m_messagesWorkArr[rightInd]);

		m_rightSum.push_back(curSum);
	}

#ifdef SUPER_VERBOSE
			std::cout << "\t\tRightSum: ";
			for (uint32_t jj = 0; jj < N; jj++) {
				std::cout << m_rightSum[jj] << ", ";
			}
			std::cout << std::endl;
#endif

	// special case for j = 0
	messages[begin] = m_rightSum[N - 1];

	// now we use curSum as the cumulative multiplication from the
	// left rather than right
	curSum = m_messagesWorkArr[0];

	for (unsigned int j = 1; j < N; j++) {
		messages[begin + j] = m_llrCalc.Boxplus(curSum, m_rightSum[N - j - 1]);

		//assert(isfinite(messages[begin + j]));

		curSum = m_llrCalc.Boxplus(curSum, m_messagesWorkArr[j]);
	}

	m_rightSum.clear();
	m_messagesWorkArr.clear();

#ifdef SUPER_VERBOSE
			std::cout << "\t\tOut: ";
			for (uint32_t jj = 0; jj < N; jj++) {
				std::cout << messages[begin + jj] << ", ";
			}
			std::cout << std::endl;
#endif

}


unsigned int LinearCheckNodeUpdater::size()
{
	return m_checkNodesPriorQLLR.size();
}
