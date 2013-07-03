/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>
#include <string.h>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <iterator>
#include <assert.h>

/**
 * \ingroup bp
 * \brief A container for multiple fixed-size vectors in a memory-efficient block.
 *
 * The MultiVector class keeps many virtual vectors in a contiguous memory block.
 *  * virtual vector lengths are given at construction time, and remain constant
 *    throughout the life of the MultiVector.
 *  * there is a large std::vector of elements which stores the elements for
 *    virtual vectors in sequence. The elements are accessed through get/put
 *    methods. The beginning and end of a specific virtual vector can be
 *    obtained using the begin/end methods.
 */
template<class T>
class MultiVector
{
public:
	/**
	 * C'tor
	 *
	 * @param lengths the lengths of each of the vectors
	 */
	explicit MultiVector(const std::vector<uint32_t>& lengths);

	/**
	 * @param vec_index: what virtual vector to access
	 * @return the offset of the vector beginning into the get/set structure
	 */
	inline uint32_t begin(uint32_t vec_index);

	/**
	 * @param vec_index: what virtual vector to access
	 * @return the offset of the element after the vector's last element
	 */
	inline uint32_t end(uint32_t vec_index);

	/**
	 * @param elem_index: what element to get/set
	 */
	inline T& operator[] (uint32_t elem_index);

	/**
	 * @return the number of virtual vectors
	 */
	inline uint32_t size();

	/**
	 * @return the total number of elements in the MultiVector
	 */
	uint32_t total_num_elements();

 private:
	// A vector of offsets into the first elements of each virtual vector.
	// There is also an offset for the element after the last virtual vector.
	std::vector<uint32_t> m_heads;

	// The element data
	std::vector<T> m_data;
};

template<class T> inline MultiVector<T>::MultiVector(const std::vector<uint32_t>& lengths)
{
	if(lengths.size() == 0) {
		throw(std::runtime_error("must have at least one vector"));
	}

	// Calculate first elements of each virtual vector
	m_heads.reserve(lengths.size() + 1);
	m_heads.push_back(0);
	std::partial_sum(lengths.begin(),
					 lengths.end(),
					 std::back_insert_iterator<std::vector<uint32_t> >(m_heads));

	// Allocate the data vector
	m_data.resize(m_heads.back());
}

template<class T> inline uint32_t MultiVector<T>::begin(uint32_t vec_index)
{
	assert(vec_index < (m_heads.size() - 1));
	return m_heads[vec_index];
}

template<class T> inline uint32_t MultiVector<T>::end(uint32_t vec_index)
{
	assert(vec_index < (m_heads.size() - 1));
	return m_heads[vec_index + 1];
}

template<class T> inline T & MultiVector<T>::operator [](uint32_t elem_index)
{
	return m_data[elem_index];
}

template<class T>
inline uint32_t MultiVector<T>::size() {
	return (m_heads.size() - 1);
}

template<class T>
inline uint32_t MultiVector<T>::total_num_elements() {
	return m_data.size();
}

