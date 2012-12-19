/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

#include "Backtracker.h"
#include "BestK.h"
#include "DualPool.h"

template<typename Node>
struct SearchIntermediateResult {
	// The bits that were fed into the spine
	std::vector<uint16_t> path;

	// the node associated with the path
	Node node;
};


/**
 * A generic implementation of breadth first search exploration with pruning.
 *
 * The search starts from a root node. At each step, all nodes in the beam are
 *    branched with same number of children per node. Among the children, the
 *    ones with best score are chosen (lower is better).
 *
 * During search, backtracking information is kept. When the search is over, the
 *    best path can be retrieved, along with the maximal leaf.
 *
 * The search works with weights, and is a heuristic to find the minimum weight
 *    path. As an additional requirement, all weights must be >= 0.
 *
 * Inner workings:
 *   - m_nodes is an std::vector of nodes at the current layer. The indices of the nodes that have
 *     not been pruned is kept in m_workSet.
 *   - While processing the next layer, m_pruner is fed with node weights. After going through the
 *     nodes at the layer, the pruner returns the locations of non-pruned nodes, to be put in
 *     m_workSet.
 *   - m_intermediateNodes is a buffer used for intermediate results when calculating the next
 *     layer's nodes. When layer processing is complete, the contents of m_intermediateNodes is
 *     swapped with m_nodes, so the newly computed layer becomes the current layer.
 */
template<typename BranchEvaluator, typename Pruner>
class PrunedBFS {
public:
	typedef BranchEvaluator Evaluator;
	typedef typename BranchEvaluator::Node Node;
	typedef typename BranchEvaluator::Weight Weight;
	typedef typename BranchEvaluator::BranchData BranchData;

	typedef typename Pruner::Params PrunerParams;

	/**
	 * C'tor
	 * @param prunerParams: parameters to the pruning class
	 * @param maxSearchDepth: the maximum depth of the search tree that is
	 * 		explored. This value is used in allocating backtracking structures.
	 * @param branchEvaluator: The class used to get a child node, from parent.
	 * @param logBranchFactor: The log of number of children each node can
	 * 		have (this is used in backtracking and in branch stages)
	 */
	PrunedBFS(PrunerParams& prunerParams,
			   uint32_t maxSearchDepth,
			   const BranchEvaluator &branchEvaluator,
			   uint32_t logBranchFactor);


	/**
	 * Returns a reference to the branch evaluator.
	 *
	 * This allows external manipulation of the evaluator between branch steps.
	 */
	BranchEvaluator& branchEvaluator();

	/**
	 * Initializes a new search.
	 *
	 * @param rootNode: the root of the explored tree
	 */
	void initialize(const Node& rootNode);

	/**
	 * Branches all the nodes in the beam, to get their children, and prunes the
	 *     nodes to a smaller working set.
	 *
	 * The branch evaluator performs the branch. When using the advance()
	 *     method, the user has the responsibility to set the branch evaluator
	 *     to a correct state, so that the correct branching behavior will occur.
	 *     In other words, the PrunedBFS class does not notify the branch
	 *     evaluator instance of the current search depth. It is left as
	 *     the responsibility of the caller.
	 */
	void advance(BranchData& branchData);

	/**
	 * Gets the best path explored by the search so far.
	 *
	 * 	@param bestPath: [out] will contain the index of the branch taken, in
	 * 		a path from the root to the best leaf. bestPath[0] is the edge out
	 * 		of the root, bestPath[1] the edge from depth 1 to depth 2, etc.
	 * 	@return: reference to the best leaf. This allows the caller to extract
	 * 		more information out of the leaf, if the caller requires.
	 */
	Node& getBestPath(std::vector<uint16_t>& bestPath);


	/**
	 * Returns the intermediate state of the search. This includes, for each
	 *     node in the intermediate state, its path from the root and a pointer
	 *     to the node itself. (See SearchIntermediateResult)
	 *
	 * This can be used to inspect the intermediate state of the search.
	 *
	 * @param interm: [out] where the intermediate results are written.
	 */
	void getIntermediate(
			std::vector<SearchIntermediateResult<Node> >& interm);

private:
	/**
	 * Copy c'tor
	 *
	 * @important The copy constructor only copies the search parameters,
	 *     not search state! Call initialize after copying.
	 */
	PrunedBFS(const PrunedBFS& other);


	// The maximum search depth
	const uint32_t m_maxSearchDepth;

	// The branch evaluator
	BranchEvaluator m_branchEvaluator;

	// The branch factor
	const uint32_t m_branchFactor;

	// The pruner instance
	Pruner m_pruner;

	// Backtracker
	Backtracker<uint32_t> m_backtracker;

	// All nodes at current layer.
	std::vector<Node> m_nodes;

	// Intermediate buffer for nodes
	std::vector<Node> m_intermediateNodes;

	// The indices of elements in the beam
	std::vector<uint16_t> m_workSet;
};


// IMPLEMENTATION

#include <stdexcept>

template<typename BranchEvaluator, class Pruner>
inline PrunedBFS<BranchEvaluator,Pruner>::PrunedBFS(
		PrunerParams& prunerParams,
		uint32_t maxSearchDepth,
		const BranchEvaluator & branchEvaluator,
		uint32_t logBranchFactor)
  : m_maxSearchDepth(maxSearchDepth),
    m_branchEvaluator(branchEvaluator),
    m_branchFactor(1 << logBranchFactor),
    m_pruner(prunerParams),
    m_backtracker(m_pruner.maxSize(), m_maxSearchDepth, logBranchFactor)
{
	m_nodes.reserve(m_pruner.maxSize() * m_branchFactor);
	m_intermediateNodes.reserve(m_pruner.maxSize() * m_branchFactor);
	m_workSet.reserve(m_pruner.maxSize());
}


template<typename BranchEvaluator, class Pruner>
inline BranchEvaluator & PrunedBFS<BranchEvaluator,Pruner>::branchEvaluator() {
	return m_branchEvaluator;
}

template<typename BranchEvaluator, typename Pruner>
inline void PrunedBFS<BranchEvaluator, Pruner>::initialize(const Node& rootNode) {
	// Reset pruner
	m_pruner.reset();

	// Reset backtracking
	m_backtracker.reset();

	// Clear nodes and work set
	m_nodes.clear();
	m_workSet.clear();

	// Add the root node
	m_nodes.push_back(rootNode);
	m_workSet.push_back(0);
}

template<typename BranchEvaluator, class Pruner>
inline void PrunedBFS<BranchEvaluator,Pruner>::advance(BranchData& branchData)
{
	// Sanity check: m_intermediateNodes is always empty before and after advance()
	assert(m_intermediateNodes.size() == 0);

	// A child node
	Node childNode;

	// For every element in the work set:
	for(uint32_t workSetInd = 0; workSetInd < m_workSet.size(); workSetInd++) {

		uint32_t parentIndex = m_workSet[workSetInd];
		Node& curParent(m_nodes[parentIndex]);

		// only bother to enumerate if there is a chance to be included in next round
		if(m_pruner.checkWeight(curParent.weight)) {

			// enumerate over all possible bit combinations
			for (uint32_t msgBits = 0; msgBits < m_branchFactor; msgBits++) {

				// Evaluate the branch
				m_branchEvaluator.branch(curParent,
										 msgBits,
										 branchData,
										 childNode);

				bool mightSurvive = m_pruner.add(m_intermediateNodes.size(), childNode.weight);

				if(mightSurvive) {
					// Add to m_intermediateNodes
					m_intermediateNodes.push_back(childNode);

					// Add backtracking information
					m_backtracker.saveNode(parentIndex, msgBits);
				}
			}
		}
	}

	// Close this layer in the backtracker
	m_backtracker.nextLayer();

	// Get the new work set and reset the pruner
	m_pruner.getSurvivors(m_workSet);
	m_pruner.reset();

	// Flip the intermediate nodes with the current layer nodes - making the newly computed layer
	// the current layer. The old layer is cleared.
	m_nodes.swap(m_intermediateNodes);
	m_intermediateNodes.clear();

	// Sanity check: there should be at most m_beamWidth elements in beam.
	assert(m_workSet.size() <= m_pruner.maxSize());
}

template<typename BranchEvaluator, class Pruner>
inline typename PrunedBFS<BranchEvaluator,Pruner>::Node &
	PrunedBFS<BranchEvaluator,Pruner>::getBestPath(
									std::vector<uint16_t> & bestPath)
{
	m_backtracker.backtrack<uint16_t>(0, bestPath);
	return m_nodes[m_workSet[0]];
}


template<typename BranchEvaluator, class Pruner>
inline void PrunedBFS<BranchEvaluator,Pruner>::getIntermediate(
		std::vector<SearchIntermediateResult<Node> > & interm)
{
	interm.clear();
	interm.resize(m_workSet.size());

	for(uint32_t i = 0; i < m_workSet.size(); i++) {
		interm[i].node = m_nodes(m_workSet[i]);
		m_backtracker.backtrack(m_workSet[i],interm[i].path);
	}
}
