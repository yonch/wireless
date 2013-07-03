/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include "MultiVector.h"

/**
 * \ingroup bp
 * \brief Interface for classes that send out messages given incoming messages
 *
 * The node updater receives messages in a MultiVector, and replies in that
 *    same MultiVector. Each virtual vector represents messages incoming to a
 *    node, and each element in that vector is an edge incident to that node.
 */
template<typename MessageType>
class NodeUpdater {
public:
	/**
	 * Virtual d'tor
	 */
	virtual ~NodeUpdater() {};

	/**
	 * Given incoming messages, updates outgoing messages in place.
	 */
	virtual void update(MultiVector<MessageType>& messages) = 0;
};


/**
 * Variable node updater
 * In addition to deriving outgoing messages from incoming messages, the
 * 		variable node updater also estimates the log likelihood ratio for each
 * 		variable.
 */
template<typename MessageType>
class VariableNodeUpdater : public NodeUpdater<MessageType> {
public:
	/**
	 * Computes the log likelihood ratio of each variable node to be 0 over 1
	 */
	virtual void estimate(MultiVector<MessageType>& messages,
	                         std::vector<float>& llrs) = 0;
};
