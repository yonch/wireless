/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/inference/bp/LinearVariableNodeUpdater.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>

//#define SUPER_VERBOSE

#ifdef SUPER_VERBOSE
#include <iostream>
#endif

LinearVariableNodeUpdater::LinearVariableNodeUpdater(uint32_t numVariables)
 : m_priorQLLR(numVariables, 0)
{}


LinearVariableNodeUpdater::LinearVariableNodeUpdater(
		const std::vector<float> & priorLLRs)
{
	m_priorQLLR.reserve(priorLLRs.size());

	for(uint32_t i = 0; i < priorLLRs.size(); i++) {
		m_priorQLLR.push_back(m_llrCalc.to_qllr(priorLLRs[i]));
	}
}


void LinearVariableNodeUpdater::update(MultiVector<BipartiteBP::QLLR>& messages)
{
	uint32_t N_nodes = messages.size();

	assert(N_nodes == m_priorQLLR.size());

#ifdef SUPER_VERBOSE
	std::cout << "Variable Node NEW ITERATION ******************" << std::endl;
#endif

	// for each check node
	for (uint32_t node_ind = 0; node_ind < N_nodes; node_ind++) {
		uint32_t begin = messages.begin(node_ind);
		uint32_t end = messages.end(node_ind);

		#ifdef SUPER_VERBOSE
				std::cout << "Node " << node_ind << " \t"
					      << "Prior " << m_priorQLLR[node_ind] << "\t"
					      << end-begin << " neighbors: ";
				for (uint32_t jj = begin; jj < end; jj++) {
					std::cout << messages[jj] << ", ";
				}
				std::cout << std::endl;
		#endif

		// calculate the multiplication likelihood ratios of all messages:
		int64_t allSum = m_priorQLLR[node_ind];
		for(uint32_t edge_ind = begin; edge_ind < end; edge_ind++) {
			allSum += messages[edge_ind];
		}

		// each outgoing message is calculated using the sum of all messages
		// except the message from the outgoing edge
		for(uint32_t edge_ind = begin; edge_ind < end; edge_ind++) {
			int64_t outLikelihood = allSum - messages[edge_ind];
			if(llabs(outLikelihood) > itpp::QLLR_MAX) {
				messages[edge_ind] = (outLikelihood > 0 ? itpp::QLLR_MAX : -itpp::QLLR_MAX);
			} else {
				messages[edge_ind] = int32_t(outLikelihood);
			}
		}

#ifdef SUPER_VERBOSE
		std::cout << "Outgoing: ";
		for (uint32_t jj = begin; jj < end; jj++) {
			std::cout << messages[jj] << ", ";
		}
		std::cout << std::endl;
#endif

	}

//	std::vector<float> rightSum;
//	rightSum.reserve(100); // this is to reduce resizes
//
//
//	// for each check node
//	for (uint32_t node_ind = 0; node_ind < N_nodes; node_ind++) {
//		uint32_t begin = messages.begin(node_ind);
//		uint32_t end = messages.end(node_ind);
//		uint32_t N = end - begin;
//

//
//		if(N == 1) {
//			// if this variable is only linked to one check node, send the
//			// unbiased prior estimate
//			messages[begin] = m_priorLLR2[node_ind];
//		} else if (N > 1) {
//
//			// calculate the multiplication from the right:
//			// rightMulti[j] = messages[-1].value * messages[-2].value * ... * messages[-j].value
//			float curSum = m_priorLLR2[node_ind];
//			rightSum.push_back(curSum);
//			// go over all messages except the first one (which will never be
//			// needed anyway)
//			for(unsigned int rightInd = N - 1; rightInd > 0; rightInd--) {
//				curSum += messages[begin+rightInd];
//				rightSum.push_back(curSum);
//			}
//
//			// now we use curMulti as the cumulative multiplication from the
//			// left rather than right
//			curSum = 0.0f;
//			for (unsigned int j = 0; j < N; j++) {
//				float msgVal = messages[begin + j];
//
//				messages[begin + j] = curSum + rightSum[N - j - 1];
//
//				curSum += msgVal;
//			}
//
//			rightSum.clear();
//
//#ifdef SUPER_VERBOSE
//			std::cout << "\t\tOut: ";
//			for (uint32_t jj = 0; jj < N; jj++) {
//				std::cout << messages[begin + jj] << ", ";
//			}
//			std::cout << std::endl;
//#endif
//		}
//	}
}

void LinearVariableNodeUpdater::estimate(MultiVector<BipartiteBP::QLLR> & messages,
                                         	  std::vector<float> & llrs)
{
	uint32_t N_nodes = messages.size();

	assert(N_nodes == m_priorQLLR.size());

	// Prepare output vector
	llrs.clear();
	llrs.reserve(N_nodes);

	// for each check node
	for (uint32_t node_ind = 0; node_ind < N_nodes; node_ind++) {
		uint32_t begin = messages.begin(node_ind);
		uint32_t end = messages.end(node_ind);

		// calculate the multiplication likelihood ratios of all messages:
		int64_t allSum = m_priorQLLR[node_ind];
		for(uint32_t edge_ind = begin; edge_ind < end; edge_ind++) {
			allSum += messages[edge_ind];
		}

		if(llabs(allSum) > itpp::QLLR_MAX) {
			llrs.push_back(m_llrCalc.to_double((allSum > 0 ? itpp::QLLR_MAX : -itpp::QLLR_MAX)));
		} else {
			llrs.push_back(m_llrCalc.to_double(int32_t(allSum)));
		}
	}
}


