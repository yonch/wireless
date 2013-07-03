/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdint.h>
#include "../../CodeBench.h"
#include "../../channels/CoherenceFading.h"


/**
 * \ingroup spinal
 * \brief Computes the euclidian distance between two integer symbols
 */
class IntegerEuclidianDistance {
public:
	typedef uint64_t Weight;
	static Weight dist(Symbol x, Symbol y);
};

/**
 * \ingroup spinal
 * \brief Computes the euclidian distance between two soft numbers
 */
class SoftEuclidianDistance {
public:
	typedef double Weight;
	static Weight dist(SoftSymbol x, SoftSymbol y);
};


/**
 * \ingroup spinal
 * \brief Computes the hamming distance between two symbols (that are integers)
 */
class HammingDistance {
public:
	typedef uint32_t Weight;
	static Weight dist(Symbol x, Symbol y);
};

/**
 * \ingroup spinal
 * \brief Computes the euclidian distance between two faded soft symbols
 */
class FadingEuclidianDistance {
public:
	typedef double Weight;
	static Weight dist(FadingSymbol x, FadingSymbol y);
};

/**
 * \ingroup spinal
 * \brief The Node struct keeps information on one node in the explored tree
 */
template<typename SpineValueSeed, typename WeightType>
struct SpinalNode {
	/**
	 * Comparison of node weights
	 */
	bool operator<(const SpinalNode& other) const;

	/**
	 * Returns the likelihood
	 */
	WeightType getWeight() const {return likelihood;}

	WeightType getLikelihood() const {return likelihood;}

	WeightType likelihood;
	WeightType lastCodeStepLikelihood;
	SpineValueSeed hash;
};

/**
 * \ingroup spinal
 * \brief A container for a group of symbols
 */
template<typename ChannelSymbol>
struct SymbolCollection {
	// C'tor
	SymbolCollection(const ChannelSymbol* _data, unsigned int _size)
	   : data(_data), size(_size) {}

	// The data the collection holds.
	const ChannelSymbol* data;

	// The number of elements in the collection
	unsigned int size;
};


/**
 * \ingroup spinal
 * \brief Evaluates likelihoods of branches in the decoding tree
 */
template<typename SpineValueType, typename ChannelTransformation, typename Distance>
class SpinalBranchEvaluator {
public:
	typedef typename Distance::Weight Weight;
	typedef typename ChannelTransformation::OutputType ChannelSymbol;

	typedef SpinalNode<typename SpineValueType::Seed, Weight> Node;
	typedef SymbolCollection< ChannelSymbol > BranchData;

	/**
	 * C'tor
	 *
	 * @param xform: the transform that maps encoder output to symbols
	 * @param k: the size of k for the code
	 */
	SpinalBranchEvaluator(const ChannelTransformation& xform, uint32_t k);

	/**
	 * Advances the spine from 'parent', using 'edge' as input message bits. The
	 *    resulting decode information is stored in 'child'
	 *
	 * @note implementation MUST behave correctly when parent and child are
	 *    the same object.
	 */
	void branch(Node& parent,
				unsigned int edge,
				BranchData& syms,
				Node& child);

	/**
	 * Initializes Node objects for the first time. This is instead of using a
	 *     factory (in order to avoid pointer dereferences). initNode should
	 *     be called once on each node, after its construction.
	 *
	 * When starting a new decode, do NOT call initNode again.
	 */
	void initNode(Node& node);

private:
	// K for each of the decoders
	const uint32_t m_k;

	// Mask to extract k LSB bits
	const uint32_t m_mask;

	// ChannelTransformation from bits to symbols
	ChannelTransformation m_xform;

	// Symbols that would have been encoded
	typename std::vector<uint16_t> m_encodedSymbols;

	// Channel symbols that would have been produced
	std::vector<typename ChannelTransformation::OutputType> m_candidateSymbols;

	// Symbols that had been observed, according to BranchData
	std::vector<typename ChannelTransformation::OutputType> m_observedSymbols;
};


// IMPLEMENTATION


// SPINAL NODE
template<typename SpineValue, typename WeightType>
inline bool SpinalNode<SpineValue, WeightType>::operator <(const SpinalNode & other) const
{
	return likelihood < other.likelihood;
}

// LOOKAHEAD BRANCH EVALUATOR

template<typename SpineValueType, typename ChannelTransformation, typename Distance>
inline SpinalBranchEvaluator<SpineValueType,ChannelTransformation,Distance>
	::SpinalBranchEvaluator(const ChannelTransformation & xform, uint32_t k)
	 : m_k(k),
	   m_mask((1 << m_k) - 1),
	   m_xform(xform)
{
	m_encodedSymbols.reserve(100);
	m_candidateSymbols.reserve(100);
	m_observedSymbols.reserve(100);
}

template<typename SpineValueType, typename ChannelTransformation, typename Distance>
inline void SpinalBranchEvaluator<SpineValueType,ChannelTransformation,Distance>
	::branch(Node & parent, unsigned int edge,  BranchData& syms, Node & child)
{

	// The likelihood due to this step
	Weight stepLikelihood = 0;

	// Prepare encoded symbols buffer
	unsigned int numEncodedSymbols = m_xform.forecast(syms.size);
	m_encodedSymbols.resize(numEncodedSymbols);

	// generate the next spine value from the bits
	SpineValueType spineValue(parent.hash, edge);
	child.hash = spineValue.getSeed();

	// Calculate encoded symbols for the encoded
	for(unsigned int i = 0; i < numEncodedSymbols; i++) {
		m_encodedSymbols[i] = spineValue.next();
	}

	// Get observed symbols into a vector
	m_observedSymbols.assign(syms.data, syms.data + syms.size);

	// Transform symbols using the channel transformation
	m_xform.transform(m_encodedSymbols, m_observedSymbols, m_candidateSymbols);

	// Encode symbols, updating the likelihood
	for(unsigned int i = 0; i < syms.size; i++) {
		// calculate the log likelihood of the proposed symbol, given
		// the actual symbol received
		stepLikelihood += Distance::dist(syms.data[i], m_candidateSymbols[i]);
	}

	child.lastCodeStepLikelihood = stepLikelihood;
	child.likelihood = parent.likelihood + stepLikelihood;
}

template<typename SpineValueType, typename ChannelTransformation, typename Distance>
inline void SpinalBranchEvaluator<SpineValueType,ChannelTransformation,Distance>::initNode(Node & node)
{// We don't have to do anything in this case.
}
