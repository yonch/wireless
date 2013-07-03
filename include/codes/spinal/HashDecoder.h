/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include "../../util/inference/hmm/BeamSearch.h" // For SearchIntermediateResult

#include "IHashDecoder.h"
#include "SpinalBranchEvaluator.h"
#include "FlatSymbolStorage.h"

/**
 * \ingroup spinal
 * \brief Holder for intermediate state, retrievable using HashDecoder::getIntermediate()
 */
struct SpinalIntermediateResult {
	// The bits that were fed into the spine
	std::vector<unsigned short> path;

	// the value of the spine at the current layer
	unsigned long long spineValue;

	// the weight associated with the spine value
	unsigned long long weight;
};

/**
 * \ingroup spinal
 * \brief Decoder for spinal codes
 */
template<typename Search>
class HashDecoder : public IHashDecoder<typename Search::Evaluator::ChannelSymbol>
{
public:
	typedef DecodeResult Result;
	typedef typename Search::Node Node;
	typedef typename Search::Weight Weight;
	typedef typename Search::Evaluator::ChannelSymbol ChannelSymbol;

	/**
	 * C'tor
	 *
	 * @param k: the number of message bits incorporated
	 *     into each hash application
	 * @param spineLength: number of spine values in the spine
	 * @param maxNumSymbolsPerValue: the maximum number of symbols we'll have
	 * 		to store for all spine values except the last one
	 * @param maxNumSymbolsLastValue: the maximum number of symbols we'll have
	 * 		to store for the last spine value.
	 * @param search: the implementation that searches a subset of all possible
	 * 		spines, to find the spine meant by the decoder.
	 */
	HashDecoder(unsigned int k,
				unsigned int spineLength,
				unsigned int maxNumSymbolsPerValue,
				unsigned int maxNumSymbolsLastValue,
				const Search& search);

	/**
	 * Virtual d'tor
	 */
	virtual ~HashDecoder() {}

	/**
	 * Resets the decoder, so a different packet can be decoded
	 */
	virtual void reset();

	/**
	 * Sets the symbols for the packet. This decoder only supports contiguous
	 * 		symbols, starting with symbol 0.
	 * @param spineValueIndices: the sequence of spine values where symbols
	 * 		were emitted from
	 * @param symbols: the symbols to add
	 */
	virtual void add(
			const std::vector<uint16_t>& spineValueIndices,
			const std::vector<ChannelSymbol>& symbols,
			N0_t n0);

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 */
	virtual DecodeResult decode();

	/**
	 * Performs a full decode of the given symbols, returning the most likely
	 *   candidate for the packet.
	 *
	 * This version returns an extended result
	 */
	virtual SpinalDecodeResult decodeExtended();

	///////////////////////////////////////////////////////////////////
	//// Fine-grained control of the decode process
	///////////////////////////////////////////////////////////////////
	/**
	 * Initializes the decoder for a new packet
	 */
	virtual void initializeSearch();

	/**
	 * Derives the suggestions to the next spine value.
	 */
	void doInference(unsigned int codeStep);

	/**
	 * @return the most likely packet up until this coding step
	 *
	 * @assumes doInference was called at least once
	 *
	 * @param result: [out] where the result is written
	 */
	virtual void getMostLikelyResult(DecodeResult& result);


	/**
	 * @return extended results
	 *
	 * @assumes doInference was called at least once
	 *
	 * @param result: [out] where the result is written
	 */
	void getExtendedResults(SpinalDecodeResult& result);


	/**
	 * goal: examination of internal state for debugging
	 * @return the internal set of suggestions
	 */
	void getIntermediate(std::vector<SpinalIntermediateResult>& results);

	/**
	 * Gets the detection metric for this round, i.e. the ratio between
	 *   Lowest distance metric and second lowest.
	 */
	double getDetectionMetric();

private:
	/**
	 * Compares the two Nodes. Helper function to allow sorting of the result
	 *     of getBeamNodes by weight.
	 *
	 * @return true if a's weight is smaller than b's.
	 */
	static bool compareNodeWeights(	const SearchIntermediateResult <Node>& a,
	                            	const SearchIntermediateResult <Node>& b);

	// The number of message bits in each hash application
	const unsigned int m_k;

	// The number of coding steps
	const unsigned int m_spineLength;

	// Symbol organizer
	FlatSymbolStorage<ChannelSymbol> m_storage;

	// Instance to perform beam search
	Search m_search;
};

// include implementation
#include "HashDecoder.hh"
