/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

#include "Backtracker.h"
#include "BestK.h"
#include "DualPool.h"

/**
 * \ingroup hmm
 * \brief Information about a node while exploring a tree.
 */
template<typename Node>
struct SearchIntermediateResult {
	// The bits that were fed into the spine
	std::vector<unsigned short> path;

	// the node associated with the path
	Node node;
};


/**
 * \ingroup hmm
 * \brief A generic implementation of beam search exploration.
 *
 * The search starts from a root node. At each step, all nodes in the beam are
 *    branched with same number of children per node. Among the children, the
 *    ones with best score are chosen (lower is better).
 *
 * During search, backtracking information is kept. When search is over, the
 *    best path can be retrieved, along with the maximal leaf.
 *
 * The search works with weights, and is a heuristic to find the minimum weight
 *    path. As an additional requirement, all weights must be >= 0.
 *
 * Inner workings:
 *   - m_nodePool is allocated with enough Node instances for 2x the total
 *     number of children for all nodes in the beam. These Node instances are
 *     divided into two halves, (first and second halves). At every point in
 *     time, one of them contains the current beam, and the other half contains
 *     the possibilities for nodes branching from the current beam. The node
 *     pool exists in order to prevent copies or potentially heavy nodes.
 *   - The Suggestion class is a container to node weights and the indices into
 *     the m_nodePool array.
 *   - m_beam contains Suggestion instances for the current beam. It is required
 *     to keep a list of indices in the beam, since the beam is only a subset
 *     of the nodes in the relevant half of the node pool.
 *   - m_nextBeam is an intermediate data structure, used only within the
 *     advance function. Outside the advance() function, it is empty. m_nextBeam
 *     is fed with Suggestions for each branch. At the end of the step,
 *     m_nextBeam provides a list of the best m_beamWidth suggestions that were
 *     observed. This is put into the beam.
 *   - m_beamPool and m_nextBeamPool keep pointers to which half is for the
 *     children and which half is for the beam. After advance() returns, the
 *     half that contained the children, now contains the beam, and the other
 *     half can be used for the children in the next advance() call. (ie the
 *     two pointers are swapped)
 */
template<typename BranchEvaluator, template<class> class Pruner>
class BeamSearch {
private:
	// forward declaration
	class Suggestion;
public:
	typedef BranchEvaluator Evaluator;
	typedef typename BranchEvaluator::Node Node;
	typedef typename BranchEvaluator::Weight Weight;
	typedef typename BranchEvaluator::BranchData BranchData;

	typedef typename Pruner<Suggestion>::Params PrunerParams;

	/**
	 * C'tor
	 * @param prunerParams: parameters to the pruning class
	 * @param maxSearchDepth: the maximum depth of the search tree that is
	 * 		explored. This value is used in allocating backtracking structures.
	 * @param branchEvaluator: The class used to get a child node, from parent.
	 * @param logBranchFactor: The log of number of children each node can
	 * 		have (this is used in backtracking and in branch stages)
	 */
	BeamSearch(PrunerParams& prunerParams,
			   unsigned int maxSearchDepth,
			   const BranchEvaluator &branchEvaluator,
			   unsigned int logBranchFactor);

	/**
	 * Copy c'tor
	 *
	 * @important The copy constructor only copies the beam search structure,
	 *     not search state! Call initialize after copying.
	 */
	BeamSearch(const BeamSearch& other);

	/**
	 * Returns a reference to the branch evaluator.
	 *
	 * This allows external manipulation of the evaluator between branch steps.
	 */
	BranchEvaluator& branchEvaluator();

	/**
	 * Initializes a new search.
	 */
	void initialize();

	/**
	 * @return A reference to the root node. The root node should be initialized
	 *     by the caller.
	 * @note the return value of this method is only valid after
	 *     initialize() and before any calls to advance()
	 */
	Node& getRoot();

	/**
	 * Branches all the nodes in the beam, to get their children, and selects
	 *     the best children as new beam.
	 *
	 * The branch evaluator performs the branch. When using the advance()
	 *     method, the user has the responsibility to set the branch evaluator
	 *     to a correct state, so that the correct branching behavior will occur.
	 *     In other words, the BeamSearch class does not notify the branch
	 *     evaluator instance what depth we are in the search. It is left as
	 *     the responsibility of the caller.
	 */
	void advance(BranchData& branchData);

	/**
	 * Gets the best path explored by the search so far.
	 *
	 * @important: This should be called after branch() step. (never after
	 * 		bound())
	 *
	 * 	@param bestPath: [out] will contain the index of the branch taken, in
	 * 		a path from the root to the best leaf. bestPath[0] is the edge out
	 * 		of the root, bestPath[1] the edge from depth 1 to depth 2, etc.
	 * 	@return: reference to the best leaf. This allows the caller to extract
	 * 		more information out of the leaf, if the caller requires.
	 */
	Node& getBestPath(std::vector<unsigned short>& bestPath);


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
	 * A structure that holds information about a node in the search.
	 */
	struct Suggestion {
		typedef typename BranchEvaluator::Weight Weight;

		Suggestion(Weight _weight, unsigned int _poolIndex)
			: weight(_weight), poolIndex(_poolIndex) {}

		// Compares two suggestions based on weight
		bool operator< (const Suggestion& other) const;

		// The weight of the node
		Weight weight;

		// The index into m_nextBeamPool of the node.
		unsigned int poolIndex;
	};

	// The pruner params
	const PrunerParams m_prunerParams;

	// Suggestions of children.
	Pruner<Suggestion> m_nextBeam;

	// The maximum search depth
	const unsigned int m_maxSearchDepth;

	// The branch evaluator
	BranchEvaluator m_branchEvaluator;

	// The number of bits in branchFactor
	const unsigned int m_logBranchFactor;

	// The branch factor
	const unsigned int m_branchFactor;

	// Backtracker
	Backtracker<unsigned int> m_backtracker;

	// All nodes used in the search, including the beam, and the children.
	DualPool<Node> m_nodePool;

	// The indices of elements in the beam
	std::vector<Suggestion> m_beam;
};


// IMPLEMENTATION

#include <stdexcept>

template<typename BranchEvaluator, template<class> class Pruner>
inline BeamSearch<BranchEvaluator,Pruner>::BeamSearch(
		PrunerParams& prunerParams,
		unsigned int maxSearchDepth,
		const BranchEvaluator & branchEvaluator,
		unsigned int logBranchFactor)
  : m_prunerParams(prunerParams),
    m_nextBeam(prunerParams),
    m_maxSearchDepth(maxSearchDepth),
    m_branchEvaluator(branchEvaluator),
    m_logBranchFactor(logBranchFactor),
    m_branchFactor(1 << m_logBranchFactor),
    m_backtracker(m_nextBeam.maxSize(), m_maxSearchDepth, m_logBranchFactor),
    m_nodePool(m_nextBeam.maxSize() * m_branchFactor),
    m_beam()
{
	m_beam.reserve(m_nextBeam.maxSize());

	// Initialize the node pool
	for(unsigned int i = 0; i < m_nodePool.size(); i++) {
		m_branchEvaluator.initNode(m_nodePool.primary(i));
		m_branchEvaluator.initNode(m_nodePool.secondary(i));
	}
}


template<typename BranchEvaluator, template<class> class Pruner>
inline BeamSearch<BranchEvaluator,Pruner>::BeamSearch(const BeamSearch& other)
  : m_prunerParams(other.m_prunerParams),
    m_nextBeam(m_prunerParams),
	m_maxSearchDepth(other.m_maxSearchDepth),
	m_branchEvaluator(other.m_branchEvaluator),
	m_logBranchFactor(other.m_logBranchFactor),
	m_branchFactor(1 << m_logBranchFactor),
    m_backtracker(m_nextBeam.maxSize(), m_maxSearchDepth, m_logBranchFactor),
    m_nodePool(m_nextBeam.maxSize() * m_branchFactor),
    m_beam()
{
	m_beam.reserve(m_nextBeam.maxSize());

	// Initialize the node pool
	for(unsigned int i = 0; i < m_nodePool.size(); i++) {
		m_branchEvaluator.initNode(m_nodePool.primary(i));
		m_branchEvaluator.initNode(m_nodePool.secondary(i));
	}
}

template<typename BranchEvaluator, template<class> class Pruner>
inline BranchEvaluator & BeamSearch<BranchEvaluator,Pruner>::branchEvaluator() {
	return m_branchEvaluator;
}

template<typename BranchEvaluator, template<class> class Pruner>
inline void	BeamSearch<BranchEvaluator,Pruner>::initialize()
{
	// Reset backtracking
	m_backtracker.reset();

	m_beam.clear();
	m_beam.push_back(Suggestion(0,0));

	m_backtracker.reset();
}

template<typename BranchEvaluator, template<class> class Pruner>
inline typename BeamSearch<BranchEvaluator,Pruner>::Node &
BeamSearch<BranchEvaluator,Pruner>::getRoot()
{
	return m_nodePool.primary(0);
}


template<typename BranchEvaluator, template<class> class Pruner>
inline void BeamSearch<BranchEvaluator,Pruner>::advance(BranchData& branchData)
{
	// Sanity check: m_nextBeam is always empty before and after advance()
	assert(m_nextBeam.size() == 0);

	// The index into m_childPool of the suggestion being computed
	unsigned int poolIndex = 0;

	// For every element in the beam:
	typename std::vector<Suggestion>::iterator beamIter;
	for(beamIter = m_beam.begin(); beamIter != m_beam.end(); beamIter++) {

		// only bother to enumerate if there is a chance to be included in next round
		if(m_nextBeam.checkPush(beamIter->weight)) {

			Node& beamNode(m_nodePool.primary(beamIter->poolIndex));

			// enumerate over all possible bit combinations
			for (unsigned int msgBits = 0; msgBits < m_branchFactor; msgBits++) {

				Node& childNode(m_nodePool.secondary(poolIndex));

				// Evaluate the branch
				m_branchEvaluator.branch(beamNode,
										 msgBits,
										 branchData,
										 childNode);

				// update m_nextBeam with the new value.
				m_nextBeam.push(Suggestion(
									childNode.getWeight(),
									poolIndex));

				poolIndex++;
			}
		} else {
			// We will not branch these nodes, advance poolIndex accordingly
			poolIndex += m_branchFactor;
		}
	}

	// Update the beam using the new computed nodes
	m_nextBeam.putSorted(m_beam);

	// The nodes for the current beam now reside where 'next layer' nodes
	// used to reside
	m_nodePool.flip();

	// Sanity check: there should be at most m_beamWidth elements in beam.
	assert(m_beam.size() <= m_nextBeam.maxSize());

	// save backtracking information
	typename std::vector<Suggestion>::iterator beamNode;
	for(beamNode = m_beam.begin(); beamNode != m_beam.end(); beamNode++) {
		unsigned int poolIndex = beamNode->poolIndex;
		m_backtracker.saveNode(poolIndex >> m_logBranchFactor,
							   poolIndex & (m_branchFactor - 1));
	}

	// Close this layer in the backtracker
	m_backtracker.nextLayer();
}

template<typename BranchEvaluator, template<class> class Pruner>
inline typename BeamSearch<BranchEvaluator,Pruner>::Node &
	BeamSearch<BranchEvaluator,Pruner>::getBestPath(
									std::vector<unsigned short> & bestPath)
{
	m_backtracker.backtrack<unsigned short>(0, bestPath);
	return m_nodePool.primary(m_beam[0].poolIndex);
}


template<typename BranchEvaluator, template<class> class Pruner>
inline void BeamSearch<BranchEvaluator,Pruner>::getIntermediate(
		std::vector<SearchIntermediateResult<Node> > & interm)
{
	interm.clear();
	interm.resize(m_beam.size());

	for(unsigned int i = 0; i < m_beam.size(); i++) {
		interm[i].node = m_nodePool.primary(m_beam[i].poolIndex);
		m_backtracker.backtrack(i,interm[i].path);
	}
}

// BEAMSEARCH::SUGGESTION
template<typename BranchEvaluator, template<class> class Pruner>
inline bool BeamSearch<BranchEvaluator,Pruner>::Suggestion::operator <(
		const Suggestion & other) const
{
	return weight < other.weight;
}
