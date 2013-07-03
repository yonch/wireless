/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <stdint.h>
#include <vector>
#include "MultiVector.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WITH_ITPP
#include <itpp/comm/ldpc.h>
#endif

/**
 * \ingroup bp
 * \brief Representation of a bipartite graph, with support for message passing on edges.
 */
template<typename T>
class BipartiteGraph {
public:
	typedef std::tr1::shared_ptr<BipartiteGraph<T> > Ptr;

	struct Edge{
		uint32_t left;
		uint32_t right;
	};

	/**
	 * Factory
	 * @param num_left: number of nodes on the left part of the graph
	 * @param num_right: number of nodes on the right part of the graph
	 * @param edges: the edges of the graph
	 */
	static BipartiteGraph* create(uint32_t num_left,
	                                uint32_t num_right,
	                                const std::vector<Edge>& edges);

	template<typename Generator>
	static inline BipartiteGraph* createFromGenerator(
									uint32_t num_left,
	                                uint32_t num_right,
	                                Generator& generator);

#ifdef WITH_ITPP
	/**
	 * Factory from IT++ LDPC parity
	 */
	static BipartiteGraph* createFromLdpcParity(const itpp::LDPC_Parity& parity);
#endif

	/**
	 * @return the left nodes and their edge data
	 */
	MultiVector<T>& left();
	/**
	 * @return the right nodes and their edge data
	 */
	MultiVector<T>& right();

	/**
	 * Shuffles edge information from left data structure to right data structure
	 */
	void leftToRight();

	/**
	 * Shuffles edge information from right data structure to left data structure
	 */
	void rightToLeft();

private:
	/**
	 * C'tor
	 * @param num_left: number of nodes on the left part of the graph
	 * @param num_right: number of nodes on the right part of the graph
	 * @param edges: the edges of the graph
	 */
	BipartiteGraph(const std::vector<Edge>& edges,
	   	   	   	   const std::vector<uint32_t>& left_degrees,
	   	   	   	   const std::vector<uint32_t>& right_degrees);

	// the edges sorted by the left nodes
	MultiVector<T> m_left;

	// the edges sorted by the right nodes
	MultiVector<T> m_right;

	// for each edge from a left node, the index of the corresponding back
	// edge in the right node data structure
	std::vector<uint32_t> m_leftBackEdges;
};






template<typename T>
inline BipartiteGraph<T>* BipartiteGraph<T>::create(
		uint32_t num_left,
		uint32_t num_right,
		const std::vector<Edge>& edges)
{
	// Make a pass and count left and right degrees
	std::vector<uint32_t> left_degrees(num_left, 0);
	std::vector<uint32_t> right_degrees(num_right, 0);

	for(uint32_t i = 0; i < edges.size(); i++) {
		uint32_t l = edges[i].left;
		uint32_t r = edges[i].right;
		left_degrees[l]++;
		right_degrees[r]++;
	}

	return new BipartiteGraph<T>(edges, left_degrees, right_degrees);
}

template<typename T>
template<typename Generator>
inline BipartiteGraph<T>* BipartiteGraph<T>::createFromGenerator(
		uint32_t num_left,
		uint32_t num_right,
		Generator& generator)
{
	// Get all edges in the graph
	std::vector<Edge> edges;
	for(uint32_t i = 0; i < num_right; i++) {
		while(generator.hasMore()) {
			Edge e;
			e.left = generator.next();
			e.right = i;
			edges.push_back(e);
		}
		generator.nextNode();
	}

	// Return the BipartiteGraph from these edges
	return create(num_left, num_right, edges);
}

#ifdef WITH_ITPP
template<typename T>
inline BipartiteGraph<T> *BipartiteGraph<T>::createFromLdpcParity(
		const itpp::LDPC_Parity & parity) {
	// Get a sparse matrix, where each column specifies a check node
	itpp::GF2mat_sparse parity_matrix(parity.get_H(true));

	// Get all edges in the graph
	std::vector<Edge> edges;
	itpp::GF2vec_sparse sparse_vec;

	for(int32_t i = 0; i < parity_matrix.cols(); i++) {
		parity_matrix.get_col(i, sparse_vec);
		for (int32_t j = 0; j < sparse_vec.nnz(); j++) {
			Edge e;
			e.left = sparse_vec.get_nz_index(j);
			e.right = i;
			edges.push_back(e);
		}
	}

	// Return the BipartiteGraph from these edges
	return create(parity.get_nvar(), parity.get_ncheck(), edges);
}
#endif


template<typename T>
inline MultiVector<T>& BipartiteGraph<T>::left() {
	return m_left;
}

template<typename T>
inline MultiVector<T>& BipartiteGraph<T>::right() {
	return m_right;
}

template<typename T>
inline void BipartiteGraph<T>::leftToRight() {
	for(uint32_t i = 0; i < m_leftBackEdges.size(); i++) {
		m_right[m_leftBackEdges[i]] = m_left[i];
	}
}

template<typename T>
inline void BipartiteGraph<T>::rightToLeft() {
	for(uint32_t i = 0; i < m_leftBackEdges.size(); i++) {
		m_left[i] = m_right[m_leftBackEdges[i]];
	}
}

template<typename T>
inline BipartiteGraph<T>::BipartiteGraph(
		const std::vector<Edge>& edges,
		const std::vector<uint32_t>& left_degrees,
		const std::vector<uint32_t>& right_degrees)
  : m_left(left_degrees),
	m_right(right_degrees),
	m_leftBackEdges(m_left.total_num_elements())
{
	std::vector<uint32_t> left_index(left_degrees.size(), 0);
	std::vector<uint32_t> right_index(right_degrees.size(), 0);

	for(uint32_t i = 0; i < edges.size(); i++) {
		uint32_t l = edges[i].left;
		uint32_t r = edges[i].right;

		m_leftBackEdges[m_left.begin(l) + left_index[l]] =
			m_right.begin(r) + right_index[r];

		left_index[l]++;
		right_index[r]++;
	}
}

