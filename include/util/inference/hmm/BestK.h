/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup hmm
 * \brief A list with element weights where at most K minimal weight elements are maintained.
 *
 * The BestK class is a datastructure, that keeps at most K elements, that have
 *    minimal weight of the ones that were added.
 *
 * When a new element is pushed to a full datastructure, if it is smaller than
 *    the maximal element, it replaces that maximal element. If the new element
 *    is larger than the maximal element, it does not get added.
 *
 * The template argument, Datum, has to meet these requirements:
 * 1. It should define a Datum::Weight type.
 * 2. It should implement a Datum::weight member, containing the weight.
 */
template<typename Datum>
class BestK {
public:
	typedef unsigned int Params;

	/**
	 * C'tor
	 * @param k: the maximum number of elements to keep
	 */
	BestK(unsigned int k);

	/**
	 * Attempts to add the given datum.
	 */
	void push(const Datum& datum);

	/**
	 * Returns the weight of the datum with maximum weight within the best k
	 *
	 * If the BestK is empty, the result is undefined (an exception might be
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
	 * Returns the current number of elements in the BestK.
	 */
	unsigned int size();

	/**
	 * @return the maximum possible number of elements that could be kept
	 */
	uint32_t maxSize();

	/**
	 * Moves the datums, in ascending order of weights, into the destination
	 *     vector. The BestK structure is reset at the end.
	 *
	 * @note this may use the vector::swap method, so be wary of using
	 *     iterators after the operation.
	 * @important this operation has a side effect of clearing the BestK.
	 */
	void putSorted(std::vector<Datum>& dest);

private:
	// Number of items to keep
	unsigned int m_k;

	// Holder for the best k items.
	std::vector<Datum> m_heap;
};


#include <algorithm>

template<typename Datum>
inline BestK<Datum>::BestK(unsigned int k)
  : m_k(k)
{
	m_heap.reserve(m_k);
}

template<typename Datum>
inline void BestK<Datum>::push(const Datum & datum) {
	// Note m_heap is always kept as a heap, never with more than m_k elements.
	if (m_heap.size() < m_k) {
		// There are less than K elements in the heap, add datum.
		m_heap.push_back(datum);
		std::push_heap(m_heap.begin(), m_heap.end());
	} else if (datum.weight < maxWeight()) {
		// There are exactly K elements in the heap, but datum is smaller than
		// maximum, so datum should replace maximum element.
		std::pop_heap(m_heap.begin(), m_heap.end());
		m_heap.pop_back();
		m_heap.push_back(datum);
		std::push_heap(m_heap.begin(), m_heap.end());
	}
}

template<typename Datum>
inline typename Datum::Weight BestK<Datum>::maxWeight() {
	return m_heap.front().weight;
}

template<typename Datum>
inline bool BestK<Datum>::checkPush(typename Datum::Weight weight) {
	return ((size() < m_k) || (weight < maxWeight()));
}

template<typename Datum>
inline unsigned int BestK<Datum>::size() {
	return m_heap.size();
}

template<typename Datum>
inline uint32_t BestK<Datum>::maxSize() {
	return m_k;
}

template<typename Datum>
inline void BestK<Datum>::putSorted(std::vector<Datum> & dest)
{
	// Sort heap before transferring to dest.
	std::sort_heap(m_heap.begin(), m_heap.end());

	// Transfer to dest in constant time. Note dest is cleared, so after swap,
	//    m_heap will be empty.
	dest.clear();
	dest.swap(m_heap);
}
