/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>

#include "../../CodeBench.h"

/**
 * \ingroup fountain
 * \brief Encodes graphical codes from specification of check nodes connectivity (used for LT)
 */
template<typename SymbolFunction, typename NeighborGenerator>
class ParityEncoder
{
public:
	/**
	 * C'tor
	 * @param numMessageBits the length of packets, in bits
	 **/
	ParityEncoder(
			unsigned int numMessageBits,
			const NeighborGenerator& neighborGenerator,
			const SymbolFunction& symbolFunction);

	/**
	 * d'tor
	 */
	virtual ~ParityEncoder() {}

	/**
	 * Sets the packet to be encoded
	 * @param packet: the packet to be encoded
	 */
	virtual void setPacket(const std::string& packet);

	/**
	 * Gets the next encoded symbols into outSymbols
	 * @param numSymbols: the number of symbols to emit
	 * @param outSymbols: [out] where symbols will be written
	 */
	virtual void encode(
			unsigned int numSymbols,
			std::vector<uint16_t>& outSymbols);

private:
	/**
	 * Gets the next encoded symbol
	 **/
	uint16_t next();

	// number of bits in each message
	const unsigned int m_numMessageBits;

	// a NeighborGenerator to generator what bits are incorporated to an encoded bit
	NeighborGenerator m_neighborGenerator;

	// a SymbolFunction, used to get the output symbols from SymbolFunc outputs
	SymbolFunction m_symbolFunction;

	// index of next generated symbol
	unsigned int m_nextSymbolIndex;

	// The current packet, bit by bit
	std::vector<bool> m_packetVector;

	// A temporary buffer for neighbors of the generated node
	std::vector<unsigned int> m_neighbors;
};


#include "../../util/Utils.h"
#include <algorithm>

template<typename SymbolFunction, typename NeighborGenerator>
inline ParityEncoder<SymbolFunction,NeighborGenerator>::ParityEncoder(
		unsigned int numMessageBits,
		const NeighborGenerator & neighborGenerator,
		const SymbolFunction& symbolFunction)
  : m_numMessageBits(numMessageBits),
    m_neighborGenerator(neighborGenerator),
    m_symbolFunction(symbolFunction),
    m_nextSymbolIndex(0),
    m_packetVector(m_numMessageBits, false)
{}


template<typename SymbolFunction, typename NeighborGenerator>
inline void ParityEncoder<SymbolFunction, NeighborGenerator>::setPacket(
		const std::string & packet)
{
	// Sanity check - given packet must be the correct size
	if (packet.length() != ((m_numMessageBits + 7) / 8)) {
		throw(
			std::runtime_error("input packet's size inconsistent with encoder's message size"));
	}

	// Convert the packet into a bit vector
	Utils::stringToVector(packet, m_packetVector);

	// Set internal state to correct node
	m_neighborGenerator.reset();
	m_nextSymbolIndex = 0;
}


template<typename SymbolFunction, typename NeighborGenerator>
inline void ParityEncoder<SymbolFunction,NeighborGenerator>::encode(
		unsigned int numSymbols,
		std::vector<uint16_t> & outSymbols)
{
	outSymbols.resize(numSymbols);

	for(unsigned int i = 0; i < numSymbols; i++) {
		outSymbols[i] = next();
	}
}

template<typename SymbolFunction, typename NeighborGenerator>
inline uint16_t ParityEncoder<SymbolFunction,NeighborGenerator>::next()
{
	// initialize function state
	typename SymbolFunction::Hash::State state;
	SymbolFunction::Hash::init((m_nextSymbolIndex++) << 8, state);

	// get neighbor list
	m_neighbors.clear();
	m_neighbors.reserve(m_neighborGenerator.count());
	while(m_neighborGenerator.hasMore()) {
		m_neighbors.push_back(m_neighborGenerator.next());
	}

	// sort neighbor list
	std::sort(m_neighbors.begin(), m_neighbors.end());

	// for each neighbor, incorporate the original packet's value onto the result symbol
	for(unsigned int i = 0; i < m_neighbors.size(); i++) {
		unsigned int nextNeighbor = m_neighbors[i];
		SymbolFunction::Hash::update(state, m_packetVector[nextNeighbor] & 0x1);
//		cout << "added " << m_packetVector[nextNeighbor] << " (" << nextNeighbor << ")" <<endl;
	}

	// Prepare neighbor generator for next call to next()
	m_neighborGenerator.nextNode();

	uint16_t symbolBuffer[SymbolFunction::NUM_SYMBOLS_PER_STATE];
	m_symbolFunction.getSymbols(state, symbolBuffer);
	return symbolBuffer[0];
}

