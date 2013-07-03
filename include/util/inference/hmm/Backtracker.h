/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <algorithm>

/**
 * \ingroup hmm
 * \brief Keeps information about edges explored in a tree, so a path from an explored node to the
 *    root can be extracted.
 *
 * The Backtracker class saves backtracking information from a branching process
 *    and in the end retrieves the path a specific leaf in the last layer took
 *    to get to where it is.
 *
 * The backtracking structure saves a tree. We call the set of nodes in the same
 *    distance from the root a 'layer'. The backtracker saves, for each node in
 *    the layer, (i) its parent in the previous layer and (ii) the label of the
 *    edge from the parent.
 *
 * An instance is able to handle up to 'depth' layers. Each layer has up to
 * 	  'width' nodes. The edge information is up to 'edgeNumBits' bits long. The
 * 	  backtracker will encode the nodes' information in a compact structure,
 * 	  to try to minimize writes to memory.
 *
 * The template argument, ReprType, is the type used to represent the nodes'
 *    information. The lower 'edgeNumBits' bits are the label, and the upper
 *    bits are the index (in range 0..width-1) of the parent. The user has to
 *    choose the right type to contain all those bits.
 */
template<typename ReprType>
class Backtracker {
public:
	/**
	 * C'tor
	 * @param width: the maximal number of nodes in a layer
	 * @param depth: the maximal number of layers that can be stored
	 * @param edgeNumBits: the number of bits in an edge's label
	 */
	Backtracker(unsigned int width, unsigned int depth, unsigned int edgeBits);

	/**
	 * Resets the data structure to fresh state, where a new tree can be saved.
	 */
	void reset();

	/**
	 * Resets the data structure, while also writing 'null' values in all of the cells.
	 *
	 * This reset is much more expensive than regular reset; it is meant to be used
	 *    when debugging, or when rank-tracking is needed.
	 *
	 * The 'null' value is (width << edgeNumBits), which is an invalid value for
	 *    backtracking.
	 */
	void fullReset();

	/**
	 * Saves the next node in the layer
	 * @param parent: the index (in range 0..width-1) of the parent in the
	 * 	   previous layer.
	 * @param edgeLabel: a label, up to m_edgeBits bits, that describes the edge
	 *     from the parent.
	 */
	void saveNode(ReprType parent, ReprType edgeLabel);

	/**
	 * Advances to the next layer.
	 * The next calls to saveNode will relate to nodes in the new layer.
	 */
	void nextLayer();

	/**
	 * Performs backtracking in the tree from a specific node in the previous
	 * layer, computing the path from the root to the node.
	 *
	 * There has to be a call to nextLayer after finishing adding information
	 *    for the required node. No other information may be inserted after
	 *    that call to nextLayer() before reset() is called.
	 *
	 * @param nodeIndex: the index of the node in the previous layer to find
	 *     its path.
	 * @param path: [out] the path from the root to the node.
	 *
	 * @return: the parent reported by the first layer (note that this will
	 *    always be 0 when exploring a tree, but in the general case, there
	 *    could be several "roots")
	 */
	template<typename EdgeType>
	ReprType backtrack(unsigned int nodeIndex,
	               std::vector<EdgeType>& path);

	/**
	 * Tracks a path through the backtrack structure, returning the ranks of the
	 *    nodes in the data structure. Once an edge does not exist in the data
	 *    structure, no rank information is given.
	 *
	 * Note that this method tracks forward, not backward. It can be used to
	 *    find when a given path was pruned out.
	 *
	 * Unless all layers are always full (i.e., number of elements = width), the
	 *    structure should be fullReset() at the beginning of the run.
	 *
	 * @param path: the path to give ranks for
	 * @return vector: ranks of the nodes along the path, as long as the path is
	 *    within the datastructure
	 */
	std::vector<ReprType> ranks(const std::vector<ReprType>& path);

private:
	// The number of nodes in each layer
	const unsigned int m_width;

	// The number of layers
	const unsigned int m_numLayers;

	// The number of bits in each edge
	const unsigned int m_edgeBits;

	// A mask to get only the edge bits from a ReprType
	const ReprType m_edgeMask;

	// The structure that saves the backtracking information. The k'th node in
	// the l'th layer is saved in m_backtracking[(l*width)+k]
	std::vector<ReprType> m_backtracking;

	// The index in the array where next node should be written
	unsigned int m_nextNodeIndex;

	// The index in m_backtracking of the first node in the current layer
	unsigned int m_layerFirstNodeIndex;

	// The current layer in the tree
	unsigned int m_currentLayer;
};

// IMPLEMENTATION

#include <assert.h>

template<typename ReprType>
inline Backtracker<ReprType>::Backtracker(	unsigned int width,
											unsigned int depth,
											unsigned int edgeBits)
  : m_width(width),
    m_numLayers(depth),
    m_edgeBits(edgeBits),
    m_edgeMask((1 << m_edgeBits) - 1),
	m_backtracking(width*depth)
{
	reset();
}



template<typename ReprType>
inline void Backtracker<ReprType>::reset() {
	m_layerFirstNodeIndex = 0;
	m_nextNodeIndex = 0;
	m_currentLayer = 0;
}

template<typename ReprType>
inline void Backtracker<ReprType>::fullReset() {
	// Perform a regular reset
	reset();

	// Fill m_backtracking with 'null' value
	std::fill(m_backtracking.begin(), m_backtracking.end(), m_width << m_edgeBits);
}


template<typename ReprType>
inline void Backtracker<ReprType>::saveNode(ReprType parent,
											ReprType edgeLabel) {
	// Make sure we did not advance layers without calling nextLayer()
	assert(m_nextNodeIndex < m_layerFirstNodeIndex + m_width);
	// Make sure user did not call saveNode after calling nextLayer on the last
	// layer
	assert(m_nextNodeIndex < m_backtracking.size());

	// Make sure edgeLabel has right number of bits
	assert((edgeLabel & (~((1 << m_edgeBits) - 1))) == 0);

	// Update backtracking structure
	m_backtracking[m_nextNodeIndex] = ((parent << m_edgeBits) | edgeLabel);

	// Advance index of next node
	m_nextNodeIndex++;
}

template<typename ReprType>
inline void Backtracker<ReprType>::nextLayer() {
	// Advance the layer index
	m_layerFirstNodeIndex += m_width;

	// Set the next node to be the new layer's first node
	m_nextNodeIndex = m_layerFirstNodeIndex;

	// Update the current depth
	m_currentLayer++;

	// Make sure we haven't gone too deep in layers
	assert(m_currentLayer <= m_numLayers);
}

template<typename ReprType>
template<typename EdgeType>
inline ReprType Backtracker<ReprType>::backtrack(
										unsigned int nodeIndex,
										std::vector<EdgeType> & path)
{
	// Sanity check: there hasn't been any saveNode()s since last nextLayer
	assert(m_nextNodeIndex == m_layerFirstNodeIndex);

	// Sanity check: there has been at least one layer that has been input
	assert(m_layerFirstNodeIndex > 0);

	// Resize the output vector to contain enough layers to backtrack
	path.resize(m_currentLayer);

	// extract the bits from the edge structure into a vector of ints (by time)
	unsigned int layerFirstNode = m_layerFirstNodeIndex - m_width;

	for (int layer = (int)m_currentLayer - 1; layer >= 0; layer--) {
		// Get the information from the saved node
		ReprType edgeInfo = m_backtracking[layerFirstNode + nodeIndex];

		// Extract the edge bits, and save into output vector
		path[layer] = (edgeInfo & m_edgeMask);

		// Get the bits that represent the parent
		nodeIndex = edgeInfo >> m_edgeBits;
		// Move to the parents' layer
		layerFirstNode -= m_width;
	}

	return nodeIndex;
}


template<typename ReprType>
inline std::vector<ReprType> Backtracker<ReprType>::ranks(
		const std::vector<ReprType>& path)
{
	// Sanity check: there hasn't been any saveNode()s since last nextLayer
	assert(m_nextNodeIndex == m_layerFirstNodeIndex);

	// Sanity check: there has been at least one layer that has been input
	assert(m_currentLayer > 0);

	// Check that the given path is of the right size for current input
	if(path.size() != m_currentLayer) {
		throw std::runtime_error("Given path is not the same length as backtracking data");
	}

	// Result vector, holding ranks
	std::vector<ReprType> result;

	// Node at previous layer (parent of the node we're searching for)
	ReprType parent = 0;

	// Go through each layer
	for(uint32_t layerInd = 0; layerInd < m_currentLayer; layerInd++) {
		// Search for a mention of the edge from the parent in the current layer
		ReprType wantedNode = (parent << m_edgeBits) | path[layerInd];
		for(uint32_t nodeInd = 0; nodeInd < m_width; nodeInd++) {
			if(m_backtracking[(layerInd * m_width) + nodeInd] == wantedNode) {
				result.push_back(nodeInd);
				parent = nodeInd;
				break; // break inner loop - go to next layer
			}
		}

		// Couldn't find node - return partial ranks
		return result;
	}

	// Found the full path - return all ranks
	return result;
}
