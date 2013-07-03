/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include "BeamSearch.h"
#include "LookaheadAdaptor.h"

/**
 * \ingroup hmm
 * \brief A beam search, with lookahead
 */
template<typename BranchEvaluator, template<class> class Pruner>
class LookaheadBeamSearch {
public:
	typedef BranchEvaluator Evaluator;
	typedef typename BranchEvaluator::Node Node;
	typedef typename BranchEvaluator::Weight Weight;
	typedef typename BranchEvaluator::BranchData BranchData;

	typedef typename BeamSearch<BranchEvaluator,Pruner>::PrunerParams PrunerParams;

	/**
	 * C'tor
	 * @param beamWidth: the number of nodes to be expanded.
	 * @param maxSearchDepth: the maximum depth of the search tree that is
	 * 		explored. This value is used in allocating backtracking structures.
	 * @param branchEvaluator: The class used to get a child node, from parent.
	 * @param logBranchFactor: The log of number of children each node can
	 * 		have (this is used in backtracking and in branch stages)
	 * @param lookaheadDepth: the number of branches the search should look
	 * 		ahead to figure the weights for nodes.
	 */
	LookaheadBeamSearch(PrunerParams& prunerParams,
						unsigned int maxSearchDepth,
						const BranchEvaluator &branchEvaluator,
						unsigned int logBranchFactor,
						unsigned int lookaheadDepth);

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
	typedef LookaheadAdaptor<BranchEvaluator> Adaptor;
	typedef typename Adaptor::Node AdaptorNode;

	/**
	 * @returns the adaptor branch evaluator
	 */
	Adaptor& adaptor();

	// the lookahead depth
	const unsigned int m_lookaheadDepth;

	// maximum search depth
	const unsigned int m_maxSearchDepth;

	// Beam search over the lookahead adaptor's branching semantics
	BeamSearch<Adaptor, Pruner> m_lookaheadBeamSearch;

	// the number of completed layers
	unsigned int m_layer;
};


// IMPLEMENTATION

#include <stdexcept>

template<typename BranchEvaluator, template<class> class Pruner>
inline LookaheadBeamSearch<BranchEvaluator,Pruner>::LookaheadBeamSearch(
										PrunerParams& prunerParams,
										unsigned int maxSearchDepth,
										const BranchEvaluator & branchEvaluator,
										unsigned int logBranchFactor,
										unsigned int lookaheadDepth)
  : m_lookaheadDepth(lookaheadDepth),
    m_maxSearchDepth(maxSearchDepth),
    m_lookaheadBeamSearch(prunerParams,
		  	  	  	  	  maxSearchDepth - lookaheadDepth,
		  	  	  	  	  Adaptor(branchEvaluator,
		  	  	  	  			    logBranchFactor,
		  	  	  	  			    lookaheadDepth),
		  	  	  	  	  logBranchFactor),
    m_layer(0)
{}

template<typename BranchEvaluator, template<class> class Pruner>
inline BranchEvaluator & LookaheadBeamSearch<BranchEvaluator,Pruner>::branchEvaluator()
{
	// Get the original branch evaluator. (the one that is encapsulated by the
	// lookahead adaptor
	return adaptor().encapsulatedBranchEvaluator();
}

template<typename BranchEvaluator, template<class> class Pruner>
inline void LookaheadBeamSearch<BranchEvaluator,Pruner>::initialize()
{
	m_layer = 0;
	m_lookaheadBeamSearch.initialize();
}

template<typename BranchEvaluator, template<class> class Pruner>
inline typename LookaheadBeamSearch<BranchEvaluator,Pruner>::Node &
LookaheadBeamSearch<BranchEvaluator,Pruner>::getRoot()
{
	// get the adaptor's node
	AdaptorNode& adaptorRoot(m_lookaheadBeamSearch.getRoot());

	// return the underlying root node
	return adaptorRoot.lookaheadRoot();
}

template<typename BranchEvaluator, template<class> class Pruner>
inline void LookaheadBeamSearch<BranchEvaluator,Pruner>::advance(BranchData& branchData)
{
	if(m_layer < m_lookaheadDepth) {
		// We need to update the root
		AdaptorNode& adaptorRoot(m_lookaheadBeamSearch.getRoot());

		adaptor().updateRoot(adaptorRoot, m_layer, branchData);
	} else {
		// The lookahead's root had already been initialized. Now we call the
		// regular branching process of the beam search
		m_lookaheadBeamSearch.advance(branchData);
	}

	m_layer++;
}

template<typename BranchEvaluator, template<class> class Pruner>
inline typename LookaheadBeamSearch<BranchEvaluator,Pruner>::Node &
LookaheadBeamSearch<BranchEvaluator,Pruner>::getBestPath(
		std::vector<unsigned short> & bestPath)
{
	bestPath.clear();
	bestPath.reserve(m_maxSearchDepth);
	AdaptorNode& bestAdaptorNode(m_lookaheadBeamSearch.getBestPath(bestPath));

	return adaptor().addMostLikelyPath(bestAdaptorNode, bestPath);
}

template<typename BranchEvaluator, template<class> class Pruner>
inline void LookaheadBeamSearch<BranchEvaluator,Pruner>::getIntermediate(
		std::vector<SearchIntermediateResult<Node> > & interm)
{
	// Clear output list
	interm.clear();

	// Get the intermediate lookahead nodes
	std::vector<SearchIntermediateResult<AdaptorNode> > adaptorInterm;
	m_lookaheadBeamSearch.getIntermediate(adaptorInterm);

	// Extract the leaf nodes from the lookahead nodes
	for(unsigned int i = 0 ; i < adaptorInterm.size(); i++) {
		adaptor().addIntermediate(adaptorInterm[i].node,
								  adaptorInterm[i].path,
								  interm);
	}
}

template<typename BranchEvaluator, template<class> class Pruner>
inline typename LookaheadBeamSearch<BranchEvaluator,Pruner>::Adaptor &
LookaheadBeamSearch<BranchEvaluator,Pruner>::adaptor()
{
	return m_lookaheadBeamSearch.branchEvaluator();
}

