/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <itpp/comm/llr.h>
#include "NodeUpdater.h"
#include "MultiVector.h"
#include "BipartiteBP.h"

/**
 * \ingroup bp
 * \brief Sends outgoing messages from variable nodes given incoming messages, for linear codes
 */
class LinearVariableNodeUpdater : public VariableNodeUpdater<BipartiteBP::QLLR> {
public:
	/**
	 * C'tor
	 * There is neutral prior on all variable nodes
	 */
	LinearVariableNodeUpdater(uint32_t numVariables);

	/**
	 * C'tor
	 * @param priorLLRs: the prior log likelihood ratio of variable being 0 over
	 * 		it being 1
	 */
	LinearVariableNodeUpdater(const std::vector<float>& priorLLRs);

	/**
	 * Given incoming messages, updates outgoing messages
	 */
	virtual void update(MultiVector<BipartiteBP::QLLR>& messages);

	/**
	 * Computes the log likelihood ratio of each variable node to be 0 over 1
	 */
	virtual void estimate(MultiVector<BipartiteBP::QLLR>& messages,
	                         std::vector<float>& llrs);

private:
	// The prior QLLR of each variable node
	std::vector<BipartiteBP::QLLR> m_priorQLLR;

	// IT++ calculator used to calculate the messages
	itpp::LLR_calc_unit m_llrCalc;
};
