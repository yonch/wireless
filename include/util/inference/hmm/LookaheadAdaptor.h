/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

/**
 * \ingroup hmm
 * \brief A branching process that looks ahead a few layers in the tree when estimating node
 *     likelihoods.
 */
template<typename BranchEvaluator>
class LookaheadAdaptor {
public:
	typedef typename BranchEvaluator::Weight Weight;
	typedef typename BranchEvaluator::BranchData BranchData;

	/**
	 * The Node struct keeps information on one node in the explored tree
	 */
	struct Node {
		/**
		 * Returns the likelihood
		 */
		Weight getWeight() const {return minWeight;}
		typename BranchEvaluator::Node& lookaheadRoot() {return nodes[0];}

		Weight minWeight;
		std::vector<typename BranchEvaluator::Node> nodes;
	};

	/**
	 * C'tor
	 *
	 */
	LookaheadAdaptor(const BranchEvaluator& branchEvaluator,
							 unsigned int logBranchFactor,
							 unsigned int lookaheadDepth);

	/**
	 * Branches the wavefront
	 */
	void branch(Node& parent, unsigned int edge, BranchData& data, Node& child);


	/**
	 * Initializes Node objects for the first time. This is instead of using a
	 *     factory (in order to avoid pointer dereferences). initNode should
	 *     be called once on each node, after its construction.
	 *
	 * When starting a new decode, do NOT call initNode again.
	 */
	void initNode(Node& node);

	/*****
	 * Methods relating to lookahead
	 *****/
	/**
	 * Feeds information into the root node.
	 */
	void updateRoot(Node& root, unsigned int iterationIndex, BranchData& data);

	/**
	 * Adds the most likely path from the lookahead information to the vector
	 * @param node: the lookahead node to find the most likely path in
	 * @param path: a vector to add the path from the lookahead node
	 * @return a reference to the underlying node
	 */
	typename BranchEvaluator::Node& addMostLikelyPath(
								Node& node, vector<unsigned short>& path);

	/**
	 * Adds leaf nodes (of type BranchEvaluator::Node) to the intermediate
	 *     result list, given an adaptor node (of type LookaheadAdaptor::Node)
	 *
	 * @param node: the lookahead node to add leaf nodes from
	 * @param path: the path to 'node'
	 * @param interm: [out] list of intermediate results to add to
	 */
	void addIntermediate(Node& node, const vector<unsigned short>& path,
			std::vector<SearchIntermediateResult<typename BranchEvaluator::Node> > & interm);

	/**
	 * Returns the underlying branchEvaluator
	 */
	BranchEvaluator& encapsulatedBranchEvaluator();

private:
	/**
	 * Given an index into the nodes vector, adds the part of the path due to
	 *     the lookahead into the given vector.
	 *
	 * @param index: the index into the nodes array
	 * @param path: [out] where the path will be appended
	 */
	void addPathFromIndex(unsigned int index, vector<unsigned short>& path);

	// The branch evaluator used to branch nodes
	BranchEvaluator m_underlyingBranchEvaluator;

	// The log2 of the branch factor (branch factor has to be a power of 2)
	const unsigned int m_logBranchFactor;

	// Number of layers to lookahead
	const unsigned int m_lookaheadDepth;

	// The number of bits in the wavefront
	const unsigned int m_logWavefrontSize;

	// Size of wavefront
	const unsigned int m_wavefrontSize;

	// Number of branches per application
	const unsigned int m_branchFactor;

	// A mask to only get the bits up to m_logBranchFactor * m_lookaheadDepth
	const unsigned int m_wavefrontMask;

	// A mask to only get the bits up to m_logBranchFactor
	const unsigned int m_branchMask;


};


// IMPLEMENTATION
#include <assert.h>

// NODE

// LOOKAHEAD BRANCH EVALUATOR

template<typename BranchEvaluator>
inline LookaheadAdaptor<BranchEvaluator>::LookaheadAdaptor(
		const BranchEvaluator & branchEvaluator,
		unsigned int logBranchFactor,
		unsigned int lookaheadDepth)
  : m_underlyingBranchEvaluator(branchEvaluator),
    m_logBranchFactor(logBranchFactor),
    m_lookaheadDepth(lookaheadDepth),
    m_logWavefrontSize(m_logBranchFactor * m_lookaheadDepth),
    m_wavefrontSize(1 << (m_logBranchFactor * m_lookaheadDepth)),
    m_branchFactor(1 << m_logBranchFactor),
    m_wavefrontMask(m_wavefrontSize - 1),
    m_branchMask(m_branchFactor - 1)
{}

template<typename BranchEvaluator>
inline void LookaheadAdaptor<BranchEvaluator>::branch(
		Node & parent,
		unsigned int edge,
		BranchData& data,
		Node & child)
{
	assert(parent.nodes.size() == m_wavefrontSize);
	assert(child.nodes.size() == m_wavefrontSize);
	assert(edge < m_branchFactor);

	// the variable ind contains, in the upper bits, the index of the parent
	// node to branched from. In the lower bits, it contains the child bits
	// to be branched.
	unsigned int ind = edge << (m_logWavefrontSize);

	m_underlyingBranchEvaluator.branch(parent.nodes[ind >> m_logBranchFactor],
									   ind & m_branchMask,
									   data,
									   child.nodes[0]);
	ind++;

	Weight minWeight = child.nodes[0].getWeight();

	for (; (ind & m_wavefrontMask) != 0; ind++) {
		m_underlyingBranchEvaluator.branch(parent.nodes[ind >> m_logBranchFactor],
										   ind & m_branchMask,
										   data,
										   child.nodes[ind & m_wavefrontMask]);

		minWeight = min(minWeight, child.nodes[ind & m_wavefrontMask].getWeight());
	}

	child.minWeight = minWeight;
}

template<typename BranchEvaluator>
inline void LookaheadAdaptor<BranchEvaluator>::initNode(Node & node) {

	node.nodes.resize(m_wavefrontSize);

	for (unsigned int i = 0; i < m_wavefrontSize; i++) {
		m_underlyingBranchEvaluator.initNode(node.nodes[i]);
	}

}

template<typename BranchEvaluator>
inline void LookaheadAdaptor<BranchEvaluator>::updateRoot(
													Node & root,
													unsigned int iterationIndex,
													BranchData& data)
{
	// Note that going from last node to first, will mean that we do not
	// overwrite values we will need later. The fact that
	// m_underlyingBranchEvaluator.branch must work correctly when parent==child
	// guarantees the overwriting works correctly when i == j == 0.

	// Enumerate over active wavefront
	for (int i = (int)(1 << (m_logBranchFactor * iterationIndex)) - 1; i >= 0; i--) {
		// Enumerate over all possible branches
		for (int j = (int)(1 << m_logBranchFactor) - 1; j >= 0; j--) {
			m_underlyingBranchEvaluator.branch(
					root.nodes[i],
					j,
					data,
					root.nodes[(i << m_logBranchFactor) + j]);
		}
	}
}

template<typename BranchEvaluator>
inline typename BranchEvaluator::Node &
LookaheadAdaptor<BranchEvaluator>::addMostLikelyPath(
		Node & node,
		vector<unsigned short> & path)
{
	unsigned int minIndex =
			min_element(node.nodes.begin(), node.nodes.end()) - node.nodes.begin();

	addPathFromIndex(minIndex, path);

	return node.nodes[minIndex];
}

template<typename BranchEvaluator>
inline void LookaheadAdaptor<BranchEvaluator>::addIntermediate(
		Node & node,
		const vector<unsigned short> & path,
		std::vector<SearchIntermediateResult<typename BranchEvaluator::Node> > & interm)
{
	for(unsigned int i = 0; i < m_wavefrontSize; i++) {
		interm.push_back(SearchIntermediateResult<typename BranchEvaluator::Node>());

		// Add path
		interm.back().path = path;
		addPathFromIndex(i, interm.back().path);

		// Set node
		interm.back().node = node.nodes[i];
	}
}

template<typename BranchEvaluator>
inline BranchEvaluator &
LookaheadAdaptor<BranchEvaluator>::encapsulatedBranchEvaluator()
{
	return m_underlyingBranchEvaluator;
}

template<typename BranchEvaluator>
inline void LookaheadAdaptor<BranchEvaluator>::addPathFromIndex(
		unsigned int index, vector<unsigned short > & path)
{
	for (int depth = (int)m_lookaheadDepth - 1; depth >= 0; depth--) {
		path.push_back((index >> (m_logBranchFactor * depth)) & m_branchMask);
	}
}


