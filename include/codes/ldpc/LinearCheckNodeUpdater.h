/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <itpp/comm/llr.h>
#include "../../util/inference/bp/NodeUpdater.h"
#include "../../util/inference/bp/BipartiteBP.h"


/**
 * \ingroup ldpc
 * \brief Computes messages from check nodes in belief propagation of linear codes
 * A check node updater that uses a table lookup on quantized LLR values, to
 *    compute messages from check nodes to variable nodes.
 * @see itpp::LLR_calc_unit for further details
 */
class LinearCheckNodeUpdater : public NodeUpdater<BipartiteBP::QLLR> {
public:
	/**
	 * C'tor
	 * Initializes check node priors to be neutral
	 * @param numCheckNodes the number of check nodes
	 */
	LinearCheckNodeUpdater(unsigned int numCheckNodes,
			short int Dint1, short int Dint2, short int Dint3);

	/**
	 * C'tor
	 * Initializes check node priors to given array
	 * @param checkNodePriorsLLR the LLRs of each check node
	 */
	LinearCheckNodeUpdater(const std::vector<float>& checkNodePriorsLLR,
	                                short int Dint1, short int Dint2, short int Dint3);

	/**
	 * @return the number of check nodes
	 */
	unsigned int size();

	/**
	 * Given incoming messages, updates outgoing messages in place.
	 */
	virtual void update(MultiVector<BipartiteBP::QLLR>& messages);

private:
	/**
	 * Calculates outgoing messages from checknodes, given incoming messages
	 * @param nodeInd the index of the node that is calculated
	 * @param inMsgs the incoming messages
	 * @param outValues the LLR of the outgoing messages
	 */
	void updateNode(unsigned int nodeInd,
	                MultiVector<BipartiteBP::QLLR>& messages);

	// IT++ calculator used to calculate the messages
	itpp::LLR_calc_unit m_llrCalc;

	// the vector of check nodes' likelihood metric
	std::vector<itpp::QLLR> m_checkNodesPriorQLLR;

	// Temporary array to store incoming messages
	std::vector<itpp::QLLR> m_messagesWorkArr;
	// Temporary array when computing forward-backward
	std::vector<itpp::QLLR> m_rightSum;
};
