/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <stdexcept>

using namespace std;

/**
 * \ingroup bp
 * \brief Efficient memory heap for constant-sized memory if fine-grained deallocation is not
 *     required.
 *
 * A class that implements a special heap:
 *   - Only allocates one type of variable
 *   - Allocates a lot of elements at once from system heap to save on overhead
 *   - Only supports freeing all of the memory at once, not element by element.
 */
template<class T>
class ElementHeap
{
public:
	/**
	 * C'tor
	 * @param allocationQuantum the number of elements to allocate at once from the system 
	 *   heap when more elements are needed
	 */
	ElementHeap(unsigned int allocationQuantum);
	
	/**
	 * D'tor
	 */
	~ElementHeap(void);

	/**
	 * Allocates another element
	 */
	T* allocate();

	/**
	 * Allocates a few contiguous elements.
	 * If there is enough space remaining in the quantum, that space is used.
	 * Otherwise, the remaining quantum is thrown away and a new quantum is allocated.
	 */
	T* allocate(unsigned int numElements);

	/**
	 * Frees all of the elements at once.
	 */
	void reset();

private:
	/**
	 * The allocation quantum
	 */
	unsigned int m_allocationQuantum;

	/**
	 * The memory pools
	 */
	vector<T*> m_elementPools;

	/**
	 * Remaining elements in current pool
	 */
	unsigned int m_poolRemainingElements;

	/**
	 * Next element to allocate
	 */
	T* m_poolNextElement;
};


template<class T>
ElementHeap<T>::ElementHeap(unsigned int allocationQuantum) 
	: m_allocationQuantum(allocationQuantum),
	  m_poolRemainingElements(0),
	  m_poolNextElement(NULL)
{
	if(m_allocationQuantum == 0) {
		throw(std::runtime_error("Allocation quantum must be strictly greater "
								 "than 0"));
	}
}


template<class T>
ElementHeap<T>::~ElementHeap(void) {
	reset();
}

template<class T>
T* ElementHeap<T>::allocate() {
	return allocate(1);
}

template<class T>
T* ElementHeap<T>::allocate(unsigned int numElements) {
	if(m_poolRemainingElements < numElements) {
		if (numElements > m_allocationQuantum) {
			throw(std::runtime_error("Required number of elements too large for quantum"));
		}

		// allocate another quantum elements
		T* newPool = new T[m_allocationQuantum];

		m_elementPools.push_back(newPool);
		m_poolNextElement = newPool;
		m_poolRemainingElements = m_allocationQuantum;
	}
	T* ret = m_poolNextElement;
		
	// update remeining elements state
	m_poolNextElement += numElements;
	m_poolRemainingElements -= numElements;

	return ret;
}

template<class T>
void ElementHeap<T>::reset() {
	for(unsigned int i = 0; i < m_elementPools.size(); i++) {
		delete[] m_elementPools[i];
	}

	m_elementPools.clear();
	m_poolNextElement = NULL;
	m_poolRemainingElements = 0;
}
