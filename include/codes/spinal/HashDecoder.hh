/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

// for memset, memcpy:
#include <string.h>

#include <algorithm>
#include <stdexcept>
#include <cstdlib> // for rand()

#include "../../util/Utils.h"
#include <iostream>
using namespace std;



template<typename Search>
HashDecoder<Search>::HashDecoder(
		unsigned int k,
		unsigned int spineLength,
		unsigned int maxNumSymbolsPerValue,
		unsigned int maxNumSymbolsLastValue,
		const Search& search)
	: m_k(k),
	  m_spineLength(spineLength),
	  m_storage(spineLength,
			  	maxNumSymbolsPerValue,
			  	maxNumSymbolsLastValue),
	  m_search(search)
{}


template<typename Search>
inline void HashDecoder<Search>::reset()
{
	m_storage.reset();
}


template<typename Search>
inline void HashDecoder<Search>::add(
		const std::vector<uint16_t>& spineValueIndices,
		const std::vector<ChannelSymbol> & symbols,
		N0_t n0)
{
	// channel noise not used in this type of computation
	(void)n0;

	unsigned int numSymbols = spineValueIndices.size();

	// Add symbols to storage
	for(unsigned int i = 0; i < numSymbols; i++) {
		m_storage.add(spineValueIndices[i], symbols[i]);
	}
}



template<typename Search>
inline DecodeResult HashDecoder<Search>::decode()
{
	initializeSearch();

	for (unsigned int spineIndex = 0; spineIndex < m_spineLength; spineIndex++) {
		doInference(spineIndex);
	}

	DecodeResult result;
	getMostLikelyResult(result);
	return result;
}

template<typename Search>
inline SpinalDecodeResult HashDecoder<Search>::decodeExtended()
{
	initializeSearch();

	for (unsigned int spineIndex = 0; spineIndex < m_spineLength; spineIndex++) {
		doInference(spineIndex);
	}

	SpinalDecodeResult result;
	getMostLikelyResult(result);
	getExtendedResults(result);
	return result;
}


template<typename Search>
void HashDecoder<Search>::initializeSearch() {
	m_search.initialize();

	m_search.getRoot().hash = 0;
	m_search.getRoot().likelihood = 0;
}

template<typename Search>
void HashDecoder<Search>::doInference(unsigned int spineIndex)
{
	SymbolCollection<ChannelSymbol> symbols(
			m_storage.get(spineIndex),
			m_storage.size(spineIndex));

	// Advance the search using the symbols
	m_search.advance(symbols);
}

template<typename Search>
void HashDecoder<Search>::getMostLikelyResult(DecodeResult & result)
{
	// extract the bits from the edge structure into a vector of ints (by time)
	std::vector<unsigned short> resultBits;
	m_search.getBestPath(resultBits);

	// Set the packet from the path
	Utils::unblockify(resultBits, m_k, result.packet);
}



template<typename Search>
inline void HashDecoder<Search>::getExtendedResults(SpinalDecodeResult & result)
{
	// Get search results
	std::vector<SearchIntermediateResult <Node> > searchResults;
	m_search.getIntermediate(searchResults);

	if (searchResults.size() < 3) {
		throw std::runtime_error("Not enough suggestions in intermediate "
								 "results to calculate metrics");
	}

	// Sort all weights
	sort(searchResults.begin(), searchResults.end(), compareNodeWeights);

	// Prepare result vector
	result.weights.resize(6);

	// Extract best value
	result.weights[0] = searchResults[0].node.likelihood;
	result.weights[1] = searchResults[0].node.lastCodeStepLikelihood;

	// Extract second best
	result.weights[2] = searchResults[1].node.likelihood;
	result.weights[3] = searchResults[1].node.lastCodeStepLikelihood;

	// Extract random from third..last
	unsigned int range = (searchResults.size() - 2);
	unsigned int randomIndex = 2 + ((std::rand() % ((100 * range) - 1)) % range);
	result.weights[4] = searchResults[randomIndex].node.likelihood;
	result.weights[5] = searchResults[randomIndex].node.lastCodeStepLikelihood;
}


template<typename Search>
inline void HashDecoder<Search>
	::getIntermediate(std::vector<SpinalIntermediateResult> & results)
{
	// Get search results
	std::vector<SearchIntermediateResult <Node> > searchResults;
	m_search.getIntermediate(searchResults);

	// resize results
	results.resize(searchResults.size());

	for(unsigned int i = 0; i < searchResults.size(); i++) {
		results[i].path.swap(searchResults[i].path);
		results[i].spineValue = searchResults[i].node.hash;
		results[i].weight = searchResults[i].node.getWeight();
	}
}

template<typename Search>
double HashDecoder<Search>::getDetectionMetric() {

	// Get search results
	std::vector<SearchIntermediateResult <Node> > searchResults;
	m_search.getIntermediate(searchResults);

	if (searchResults.size() < 2) {
		throw std::runtime_error("Not enough suggestions in intermediate results to calculate metric");
	}

	sort(searchResults.begin(), searchResults.end(), compareNodeWeights);

	double res = (double)searchResults[0].node.getLikelihood() / searchResults[1].node.getLikelihood();
	return res;
}


template<typename Search>
inline bool HashDecoder<Search>::compareNodeWeights(
		const SearchIntermediateResult <Node>& a,
		const SearchIntermediateResult <Node>& b)
{
	return (a.node.getLikelihood() < b.node.getLikelihood());
}

