/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "MultiStack.h"
#include "BPMessage.h"

/**
 * \ingroup bp
 * \brief Decoder using belief propagation on bipartite graphs
 */
template<typename CheckNodeUpdater, typename NeighborGenerator>
class MessagePassingDecoder
{
public:
	/**
	 * C'tor for LT code decoder
	 * 
	 * @param observedProbs the bits encoded from the original packet, using a
	 * 		sequence of seeds obtained from an MTRand seeded with streamSeed.
	 * 		These are soft values, trustiness for each bit in encodedBits
	 * 	    -> what is the probability of it being true (values in (0,1))
	 * @param decodedLength the original size of the packet
	 * @param streamSeed the seed used to generate the seeds to the neighbor
	 * 		generator when encodedBits was generated
	 **/
//	MessagePassingDecoder(
//				vector<float> observedProbs,
//				unsigned int decodedLength,
//				unsigned int streamSeed);

	/**
	 * C'tor for QC-LDPC decoder
	 *
	 * @param variableNodeLLR: the log likelihoods of the received message bits
	 * @param checkNodeUpdater: the implementation used to update check nodes
	 * @param neighborGenerator: supplies what variable nodes are connected to
	 * 		each check node
	 */
	MessagePassingDecoder(
			const std::vector<float>& variableNodeLLR,
			CheckNodeUpdater& checkNodeUpdater,
			NeighborGenerator& neighborGenerator);

	/**
	 * C'tor for "ParityEncoder" decoder
	 *
	 * @param numVariables: number of message bits
	 * @param checkNodeUpdater: the implementation used to update check nodes
	 * @param neighborGenerator: supplies what variable nodes are connected to
	 * 		each check node
	 */
	MessagePassingDecoder(
			unsigned int numVariables,
			CheckNodeUpdater& checkNodeUpdater,
			NeighborGenerator& neighborGenerator);

	/**
	 * @return the decoded packet
	 * assumes that all votes are conclusive, otherwise the bit result is the
	 * 		most probable
	 **/
	std::string getDecoded();

	/**
	 * @return the decoded packet
	 * assumes that all votes are conclusive, otherwise the bit result is the
	 * 		most probable
	 **/
	void getLLRs(std::vector<float>& llrs);

	/**
	 * Performs a round of voting
	 *
	 * @return the number of conclusive bits (the bits whose values are above a
	 * 		threshold)
	 * @param threshold that a bit is conclusive
	 **/
	unsigned int advance(float threshold);

private:

	/**
	 * Gets the value in the stack into the vector
	 */
	void stackToVector(
			MultiStack<BPMessage>& multiStack,
			unsigned int stackIndex,
			std::vector<BPMessage>& dest);

	/**
	 * Performs an iteration on check nodes - receives messages from variable
	 * 		nodes and sends out messages.
	 */
	void advanceCheckNodes();

	/**
	 * Performs an iteration on variable nodes - receives messages from check
	 * 		nodes and sends out messages.
	 *
	 * @return the number of conclusive bits (the bits whose values are above a
	 * 		threshold)
	 * @param threshold that a bit is conclusive
	 */
	unsigned int advanceVariableNodes(float threshold);

	/**
	 * Returns the LLR/2 estimate for a specific variable
	 *
	 * @important: this is destructive towards the variable: can only be called
	 *   once for each variable and after the call further calls to advance()
	 *   have undefined behavior.
	 */
	float estimateVariable(uint32_t variableInd);

	/**
	 * Returns the max-APP estimate for a specific variable
	 *
	 * @important: this is destructive towards the variable: can only be called
	 *   once for each variable and after the call further calls to advance()
	 *   have undefined behavior.
	 */
	bool estimateBit(uint32_t variableInd);


	// the vector of variable nodes' likelihood metric. These are kept as LLR/2:
	// ie 0.5 * Log-likelihood-ratio (log with natural base)
	 std::vector<float> m_variableNodePriorLLR2;

	// the length of the original (decoded) packet
	unsigned int m_numVariableNodes;

	// the number of check nodes
	unsigned int m_numCheckNodes;

	// NOTE: messages are always LLR/2 values, see m_observedLLR2
	// Messages from variable nodes to check nodes
	MultiStack<BPMessage> m_checkNodesMessages;

	// Messages from check nodes to variable nodes
	MultiStack<BPMessage> m_variableNodesMessages;

	CheckNodeUpdater& m_checkNodeUpdater;

	// Temporary buffer to hold BP messages
	std::vector<BPMessage> m_messageBuffer;
};

#include "MessagePassingDecoder.hh"
