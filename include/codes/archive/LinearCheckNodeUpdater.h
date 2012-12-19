/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include "util/inference/bp/NodeUpdater.h"

using namespace std;

/**
 * Class to manipulate the (sign(x), ln(tanh(|x|/2)) representation (where x
 * 		is in LLR format)
 */
class LogPmQ {
public:
	/**
	 * C'tor
	 * @param llr2value the log likelihood ratio / 2
	 * @note that the value is already divided by 2.
	 */
	explicit LogPmQ(float llr2Value);

	/**
	 * Compound assignment operator +=
	 */
	LogPmQ& operator+=(const LogPmQ& rhs);

	/**
	 * Addition operator
	 */
	const LogPmQ operator+(const LogPmQ& other) const;

	/**
	 * @return LLR/2 value that the
	 */
	float getFloat() const;

	// stream operator<< for debugging, as friend
	//friend ostream& ::operator<< (ostream& out, const LogPmQ& logPmQ);

private:
	// is the original LLR value positive (1 if positive, 0 o/w)
	bool m_isPos;
	// value of ln(tanh(|LLR|/2)
	float m_val;
};

//ostream& operator<< (ostream& out, const LogPmQ& logPmQ);


/**
 * Check node updater that uses trigonometric functions to update messages. This is extremely accurate
 *    when compared with table-based optimizations, but is also very slow.
 */
class LinearCheckNodeUpdater : public NodeUpdater<float> {
public:
	/**
	 * C'tor
	 * Initializes check node priors to be neutral
	 * @param numCheckNodes the number of check nodes
	 */
	LinearCheckNodeUpdater(unsigned int numCheckNodes);

	/**
	 * C'tor
	 * Initializes check node priors to given array
	 * @param checkNodePriorsLLR the LLRs of each check node
	 * @param checkNodePriorsLLRSize the number of check nodes
	 */
	LinearCheckNodeUpdater(float* checkNodePriorsLLR,
						   unsigned int checkNodePriorsLLRSize);

	/**
	 * @return the number of check nodes
	 */
	unsigned int size();

	/**
	 * Given incoming messages, updates outgoing messages in place.
	 */
	virtual void update(MultiVector<float>& messages);

private:
	/**
	 * Calculates outgoing messages from checknodes, given incoming messages
	 * @param nodeInd the index of the node that is calculated
	 * @param inMsgs the incoming messages
	 * @param outValues the LLR of the outgoing messages
	 */
	void updateNode(unsigned int nodeInd,
	                  MultiVector<float>& messages);

	// the vector of check nodes' likelihood metric, kept in
	// isPositive(LLR(p)), log(p-q) form (ie in LogPmQ representation)
	vector<LogPmQ> m_checkNodesPriorLogPmQ;

	// Temporary array to store log(p-q) values of incoming messages
	vector<LogPmQ> m_messagesPmQ;
	// Temporary array when computing forward-backward
	vector<LogPmQ> m_rightSum;
};
