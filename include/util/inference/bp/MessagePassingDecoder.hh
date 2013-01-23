/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include <math.h>
#include <algorithm>
#include "../../Utils.h"

#ifdef SUPER_VERBOSE
#include <iostream>
#endif

//MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::MessagePassingDecoder(vector<float> observedProbs,
//												unsigned int decodedLength,
//												unsigned int streamSeed)
//  : m_variableNodePriorLLR2(decodedLength, 0.0f),
//    m_numVariableNodes(decodedLength),
//	m_checkNodesMessages((unsigned int)observedProbs.size(),
//						 ((unsigned int)observedProbs.size() + 1) / 2),
//	m_variableNodesMessages(m_numVariableNodes, (m_numVariableNodes + 1) / 2)
//{
//	// instantiate seed generator
//	MTRand mtRand(streamSeed);
//	LTParityNeighborGenerator neighborGenerator(decodedLength);
//
//	// Load the observed bits (convert to LLR/2, and (sign, log(p-q)) form)
//	vector<float> checkNodesPriorLLR2;
//	checkNodesPriorLLR2.reserve(observedProbs.size());
//	for (unsigned int i = 0; i < observedProbs.size(); i++) {
//		unsigned int currentSeed = mtRand.randInt();
//		neighborGenerator.reset(currentSeed);
//
//		float p = observedProbs[i];
//		float llr2 = ( 0.5 * logf( p / (1-p)) );
//
//		if(neighborGenerator.count() == 1) {
//			// No need to create a new check node. Just update prior on variable
//			// node
//			m_variableNodePriorLLR2[neighborGenerator.next()] += llr2;
//		} else {
//			// Create check node
//			checkNodesPriorLLR2.push_back(llr2);
//		}
//	}
//
//	mtRand.seed(streamSeed);
//
//	// perform voting with each of the encoded bits
//	for(unsigned int encInd = 0; encInd < checkNodesPriorLLR2.size(); encInd++) {
//		// Get the next check node
//		do {
//			unsigned int currentSeed = mtRand.randInt();
//			neighborGenerator.reset(currentSeed);
//		} while(neighborGenerator.count() == 1);
//
//		BPMessage msg;
//
//		// Go through all the neighbors
//		while(neighborGenerator.hasMore()) {
//			unsigned int neigh = neighborGenerator.next();
//			// insert a message from the decoded bit, initialized with prior 0.5
//			msg.from = neigh;
//			msg.value = m_variableNodePriorLLR2[neigh];
//			m_checkNodesMessages.push(encInd, msg);
//		}
//	}
//}


template<typename CheckNodeUpdater, typename NeighborGenerator>
MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::MessagePassingDecoder(
									const std::vector<float>& variableNodeLLR,
									CheckNodeUpdater& checkNodeUpdater,
									NeighborGenerator& neighborGenerator)
  : m_numVariableNodes(variableNodeLLR.size()),
    m_numCheckNodes(checkNodeUpdater.size()),
    m_checkNodesMessages(m_numCheckNodes,  (m_numCheckNodes + 1) / 2),
    m_variableNodesMessages(m_numVariableNodes, (m_numVariableNodes + 1) / 2),
    m_checkNodeUpdater(checkNodeUpdater)
{
	// Reserve some space for the message buffer
	m_messageBuffer.reserve(100);

	// Convert LLR to LLR2 on variable nodes
	m_variableNodePriorLLR2.resize(m_numVariableNodes);
	for(unsigned int i = 0; i < m_numVariableNodes; i++) {
		m_variableNodePriorLLR2[i] = (variableNodeLLR[i] / 2);
	}

	// Bootstrap messages from variable nodes to check nodes
	for(unsigned int checkInd = 0; checkInd < m_numCheckNodes; checkInd++) {
		BPMessage msg;

		neighborGenerator.set(checkInd);

		// Go through all the neighbors
		while(neighborGenerator.hasMore()) {
			unsigned int neigh = neighborGenerator.next();
			// insert a message from the decoded bit, initialized with prior 0.5
			msg.from = neigh;
			msg.value = m_variableNodePriorLLR2[neigh];
			m_checkNodesMessages.push(checkInd, msg);
		}
	}
}

template<typename CheckNodeUpdater, typename NeighborGenerator>
inline MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::MessagePassingDecoder(
		unsigned int numVariables,
		CheckNodeUpdater & checkNodeUpdater,
		NeighborGenerator & neighborGenerator)
  : m_variableNodePriorLLR2(numVariables, 0),
    m_numVariableNodes(numVariables),
	m_numCheckNodes(checkNodeUpdater.size()),
	m_checkNodesMessages(m_numCheckNodes,  (m_numCheckNodes + 1) / 2),
	m_variableNodesMessages(m_numVariableNodes, (m_numVariableNodes + 1) / 2),
	m_checkNodeUpdater(checkNodeUpdater)
{
	// Reserve some space for the message buffer
	m_messageBuffer.reserve(100);

	neighborGenerator.reset();

	// Bootstrap messages from variable nodes to check nodes
	for(unsigned int checkInd = 0; checkInd < m_numCheckNodes; checkInd++) {
		BPMessage msg;

		// get neighbor list
		vector<unsigned int> neighbors;
		neighbors.reserve(neighborGenerator.count());
		while(neighborGenerator.hasMore()) {
			neighbors.push_back(neighborGenerator.next());
		}

		// sort neighbor list
		std::sort(neighbors.begin(), neighbors.end());

		// Go through all the neighbors
		for(unsigned int i = 0; i < neighbors.size(); i++) {
			unsigned int neigh = neighbors[i];
			// insert a message from the decoded bit, initialized with prior 0.5
			msg.from = neigh;
			msg.value = 0;
			m_checkNodesMessages.push(checkInd, msg);
		}

		// advance to next neighbor for next look iteration
		neighborGenerator.nextNode();
	}
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
string MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::getDecoded() {
	advanceCheckNodes();

	vector<bool> estimate(m_numVariableNodes, false);

	// for each check node
	for (unsigned int i = 0; i < m_numVariableNodes; i++) {
		estimate[i] = estimateBit(i);
	}

	string result;
	Utils::vectorToString(estimate, result);

	return result;
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
inline void MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::getLLRs(
		std::vector<float> & llrs)
{
	advanceCheckNodes();

	llrs.clear();
	llrs.reserve(m_numVariableNodes);

	// for each check node
	for (unsigned int i = 0; i < m_numVariableNodes; i++) {
		float val = estimateVariable(i);
		llrs.push_back(val * 2);
	}
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
unsigned int MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::advance(float threshold) {
	advanceCheckNodes();
	return advanceVariableNodes(threshold);
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
void MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::stackToVector(MultiStack<BPMessage>& multiStack,
										  unsigned int stackIndex,
										  vector<BPMessage>& dest) {
	while(multiStack.isEmpty(stackIndex) == false) {
		dest.push_back(multiStack.pop(stackIndex));
	}
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
void MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::advanceCheckNodes() {
	vector<BPMessage> inMsgs;
	vector<float> outValues;

	inMsgs.reserve(100); // this is to reduce resizes
	outValues.reserve(100); // this is to reduce resizes


	BPMessage msg;

	// for each check node
	for (unsigned int i = 0; i < m_numCheckNodes; i++) {
		// Extract messages from stack
		stackToVector(m_checkNodesMessages, i, inMsgs);

		// let N be the number of messages
		unsigned int N = inMsgs.size();

		// resize output vector
		outValues.resize(N);

		// call message derivation kernel
		m_checkNodeUpdater.updateNode(i, inMsgs, outValues);

		// Post the return messages
		msg.from = i;
		for (unsigned int j = 0; j < N; j++) {
			msg.value = outValues[j];
			m_variableNodesMessages.push(inMsgs[j].from, msg);
		}

		// Discard the incoming messages - not needed anymore
		inMsgs.clear();
	}

	// Clear multi-stack to prepare for propagation back from message nodes
	m_checkNodesMessages.reset();
}


template<typename CheckNodeUpdater, typename NeighborGenerator>
unsigned int MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::advanceVariableNodes(float threshold) {
	vector<BPMessage> messages;
	vector<float> rightSum;
	BPMessage msg;
	messages.reserve(100); // this is to reduce resizes
	rightSum.reserve(100); // this is to reduce resizes
	unsigned int numConclusive = 0;

#ifdef SUPER_VERBOSE
	std::cout << "OLD-IMPL, NEW ITERATION ******************" << std::endl;
#endif

	// for each check node
	for (unsigned int i = 0; i < m_numVariableNodes; i++) {
		stackToVector(m_variableNodesMessages, i, messages);
		msg.from = i;
		// let N be the number of messages
		unsigned int N = messages.size();

#ifdef SUPER_VERBOSE
		std::cout << "Node " << i << " \t" << N << " neighbors: ";
		for (uint32_t jj = 0; jj < N; jj++) {
			std::cout << messages[jj].value << ", ";
		}
		std::cout << std::endl;
		std::cout << "\t\tOut: ";
#endif

		if(N == 1) {
			// if this variable is only linked to one check node, send the
			// unbiased prior estimate
			msg.value = m_variableNodePriorLLR2[i];
			m_checkNodesMessages.push(messages[0].from, msg);
			if((fabsf(messages[0].value + msg.value) > threshold)) {
				numConclusive++;
			}
		} else if (N > 1) {
			// calculate the multiplication from the right:
			// rightMulti[j] = messages[-1].value * messages[-2].value * ... * messages[-j].value
			float curSum = m_variableNodePriorLLR2[i];
			rightSum.push_back(curSum);
			// go over all messages except the first one (which will never be
			// needed anyway)
			for(unsigned int rightInd = N - 1; rightInd > 0; rightInd--) {
				curSum += messages[rightInd].value;
				rightSum.push_back(curSum);
			}

			// now we use curMulti as the cumulative multiplication from the
			// left rather than right
			curSum = 0.0f;
			for (unsigned int j = 0; j < N; j++) {
				msg.value = curSum + rightSum[messages.size() - j - 1];
				m_checkNodesMessages.push(messages[j].from, msg);

#ifdef SUPER_VERBOSE
				std::cout << msg.value << ", ";
#endif

				curSum += messages[j].value;
			}
			
			rightSum.clear();
			
			// curSum now contains the multiplication over all messages ->
			// it estimates probability of 1 similarly for curMultiNeg
			if(fabsf(curSum) > threshold) {
				numConclusive++;
			}

		}

#ifdef SUPER_VERBOSE
		std::cout << std::endl;
#endif

		messages.clear();
	}

	m_variableNodesMessages.reset();
	return numConclusive;
}

template<typename CheckNodeUpdater, typename NeighborGenerator>
inline float MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::estimateVariable(
		uint32_t variableInd)
{
	stackToVector(m_variableNodesMessages, variableInd, m_messageBuffer);

	float LLRsum = m_variableNodePriorLLR2[variableInd];

	for (unsigned int j = 0; j < m_messageBuffer.size(); j++) {
		LLRsum += m_messageBuffer[j].value;
	}
	m_messageBuffer.clear();

	return LLRsum;
}

template<typename CheckNodeUpdater, typename NeighborGenerator>
inline bool MessagePassingDecoder<CheckNodeUpdater,NeighborGenerator>::estimateBit(
		uint32_t variableInd)
{
	return (estimateVariable(variableInd) > 0);
}
