/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

/**
 * \ingroup hmm
 * \brief Collection of several BestK lists, insertions round-robin within the collection.
 *
 * The ParallelBestK class implements multiple BestK instances in parallel:
 *    There are multiple lists, each containing at most K elements. New items
 *    into the ParallelBestK instance arrive to a list in a round robin fashion.
 *
 * When a new element is pushed to a full list, if it is smaller than
 *    the maximal element in that list, it replaces that maximal element. If the
 *    new element is larger than the maximal element, it does not get added.
 *
 * The template argument, Datum, has to meet these requirements:
 * 1. It should define a Datum::Weight type.
 * 2. It should implement a Datum::weight member, containing the weight.
 */
template<typename Datum>
class ParallelBestK {
public:
	struct Params{
		Params(uint32_t alpha_, uint32_t beta_) : alpha(alpha_), beta(beta_) {}
		// alpha: the number of lists
		uint32_t alpha;
		// beta: the maximum number of elements to keep in each list
		uint32_t beta;
	};

	/**
	 * C'tor
	 */
	ParallelBestK(Params params);

	/**
	 * Attempts to add the given datum.
	 */
	void push(const Datum& datum);

	/**
	 * Returns the weight of the datum with maximum weight within the best k
	 *
	 * If the ParallelBestK is empty, the result is undefined (an exception might be
	 *     thrown)
	 */
	typename Datum::Weight maxWeight();

	/**
	 * Checks whether a value with weight might be pushed if push() is called
	 *
	 * @return true if value might be pushed, false otherwise
	 */
	 bool checkPush(typename Datum::Weight weight);

	/**
	 * Returns the current number of elements in the ParallelBestK.
	 */
	unsigned int size();

	/**
	 * @return the maximum possible number of elements that could be kept
	 */
	uint32_t maxSize();

	/**
	 * Moves the datums, in ascending order of weights, into the destination
	 *     vector. The ParallelBestK structure is reset at the end.
	 *
	 * @note this may use the vector::swap method, so be wary of using
	 *     iterators after the operation.
	 * @important this operation has a side effect of clearing the ParallelBestK.
	 */
	void putSorted(std::vector<Datum>& dest);

private:
	// BestK instances for the lists
	std::vector< BestK<Datum> > m_lists;

	// Temporary buffer used to get best k items from each list.
	std::vector<Datum> m_tmpDatums;

	// index into which BestK instance should be updated next
	uint32_t m_nextList;

	// The maximum number of elements in the data structure
	const uint32_t m_maxElements;

	// current number of elements stored
	uint32_t m_size;
};


#include <algorithm>

template<typename Datum>
inline ParallelBestK<Datum>::ParallelBestK(Params params)
  : m_lists(params.alpha, BestK<Datum>(params.beta)),
    m_nextList(0),
    m_maxElements(params.alpha * params.beta),
    m_size(0)
{
	m_tmpDatums.reserve(params.beta);
}

template<typename Datum>
inline void ParallelBestK<Datum>::push(const Datum & datum) {
	// Push to the next list
	m_lists[m_nextList].push(datum);

	// Update size
	if (m_size < m_maxElements) {
		m_size++;
	}

	// Advance next list
	m_nextList++;
	if(m_nextList == m_lists.size()) {
		m_nextList = 0;
	}
}

template<typename Datum>
inline bool ParallelBestK<Datum>::checkPush(typename Datum::Weight weight) {
	for(uint32_t i = 0; i < m_lists.size(); i++) {
		if(m_lists[i].checkPush(weight) == true) {
			return true;
		}
	}
	return false;
}

template<typename Datum>
inline typename Datum::Weight ParallelBestK<Datum>::maxWeight() {
	typename Datum::Weight maxWeight = m_lists[0].maxWeight();
	for(uint32_t i = 1; i < m_lists.size(); i++) {
		if(m_lists[i].size() > 0) {
			maxWeight = std::max(maxWeight, m_lists[i].maxWeight());
		}
	}
	return maxWeight;
}

template<typename Datum>
inline unsigned int ParallelBestK<Datum>::size() {
	return m_size;
}


template<typename Datum>
inline uint32_t ParallelBestK<Datum>::maxSize() {
	return m_maxElements;
}

template<typename Datum>
inline void ParallelBestK<Datum>::putSorted(std::vector<Datum> & dest)
{
	// clear destination
	dest.clear();

	// Make sure we have enough space
	dest.reserve(size());

	// Get items from all lists
	for(uint32_t i = 0; i < m_lists.size(); i++) {
		m_lists[i].putSorted(m_tmpDatums);
		dest.insert(dest.end(), m_tmpDatums.begin(), m_tmpDatums.end());
	}
	std::sort(dest.begin(), dest.end());

	// Now all BestK's are reset, since we called their putSorted methods.

	// Make sure local state (in ParallelBestK) is also reset
	m_nextList = 0;
	m_size = 0;
}


