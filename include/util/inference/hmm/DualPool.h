/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup hmm
 * \brief Dual-buffered memory staging area (supports access to memory and 'flip').
 *
 * A DualPool keeps two pools of 'size' instances. It supports accessing these
 *     two pools, 'primary' and 'secondary' by index, and a special operation
 *     'flip', causes the access to switch: primary becomes secondary and vice
 *     versa.
 */
template<typename Element>
class DualPool {
public:
	/**
	 * c'tor
	 * @param poolSize: the size of each pool.
	 *
	 */
	DualPool(unsigned int poolSize);

	/**
	 * Copy c'tor
	 *
	 * @important The copy constructor only copies the pool structure, not
	 *    the individual elements!
	 */
	DualPool(const DualPool& other);

	/**
	 * D'tor
	 */
	~DualPool();

	/**
	 * @return the size of each pool
	 */
	unsigned int size();

	/**
	 * Changes primary pool to be secondary, and secondary to primary
	 */
	void flip();

	/**
	 * Accessors to primary pool by reference
	 */
	Element& primary(unsigned int i);
	Element& secondary(unsigned int i);

	/**
	 * Accessors to primary pool by pointer
	 */
	Element* primaryPtr(unsigned int i);
	Element* secondaryPtr(unsigned int i);

private:
	unsigned int m_size;

	// The whole pool, 2*m_size elements
	Element* m_pool;

	// Pointer to primary pool
	Element* m_primary;

	// Pointer to secondary pool
	Element* m_secondary;
};


#include <stddef.h>
#include <algorithm>
#include <stdexcept>

template<typename Element>
inline DualPool<Element>::DualPool(unsigned int poolSize)
	:m_size(poolSize),
	 m_pool(new Element[2 * m_size]),
	 m_primary(m_pool),
	 m_secondary(m_pool + m_size)
{
	if(m_pool == NULL) {
		throw(std::runtime_error("Unable to allocate pool"));
	}
}

template<typename Element>
inline DualPool<Element>::DualPool(const DualPool & other)
:m_size(other.m_size),
 m_pool(new Element[2 * m_size]),
 m_primary(m_pool),
 m_secondary(m_pool + m_size)
{
	if(m_pool == NULL) {
		throw(std::runtime_error("Unable to allocate pool"));
	}
}

template<typename Element>
inline DualPool<Element>::~DualPool() {
	delete [] m_pool;
}

template<typename Element>
inline unsigned int DualPool<Element>::size() {
	return m_size;
}

template<typename Element>
inline void DualPool<Element>::flip() {
	std::swap(m_primary, m_secondary);
}

template<typename Element>
inline Element & DualPool<Element>::primary(unsigned int i) {
	return m_primary[i];
}

template<typename Element>
inline Element & DualPool<Element>::secondary(unsigned int i) {
	return m_secondary[i];
}

template<typename Element>
inline Element *DualPool<Element>::primaryPtr(unsigned int i) {
	return &m_primary[i];
}

template<typename Element>
inline Element *DualPool<Element>::secondaryPtr(unsigned int i) {
	return &m_secondary[i];
}

