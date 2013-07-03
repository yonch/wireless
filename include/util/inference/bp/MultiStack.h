/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string.h>
#include <stdexcept>
#include "ElementHeap.h"

/**
 * \ingroup bp
 * \brief A low-overhead implementation of multiple stacks. Uses ElementHeap for cheap memory
 *     allocation.
 */
template<class T>
class MultiStack
{
public:
	/**
	 * C'tor
	 *
	 * @param numStacks the number of different stacks needed in the MultiStack
	 * @param allocationQuantom the number of Ts to allocate at once, when more space is needed
	 */
	MultiStack(unsigned int numStacks, unsigned int allocationQuantum);

	/**
	 * D'tor
	 * Frees resources
	 */
	~MultiStack(void);

	/**
	 * Pushes val into the specified stack
	 */
	void push(unsigned int stackIndex, const T& val);

	/**
	 * Pops value from the specified stack
	 * Assumes stack is not empty
	 */
	T pop(unsigned int stackIndex);

	/**
	 * @return true if specific stack is empty. false otherwise.
	 */
	bool isEmpty(unsigned int stackIndex);

	/**
	 * Resets the MultiStack to empty stacks.
	 */
	void reset();

private:
	struct StackElement {
	public:
		T value;
		StackElement* next;
	};

	/**
	 * Private copy c'tor and assignment operator, to avoid copying complex
	 * 		data structure
	 */
	MultiStack(const MultiStack& other);
	MultiStack& operator=(const MultiStack& other);


	/**
	 * The numeber of stacks
	 */
	unsigned int m_numStacks;

	/**
	 * The stack heads
	 */
	StackElement** m_heads;

	/**
	 * Heap to allocate the element memory from
	 */
	ElementHeap<StackElement> m_elementHeap;

};

template<class T>
MultiStack<T>::MultiStack(unsigned int numStacks, unsigned int allocationQuantum) 
	: m_numStacks(numStacks), m_heads(new StackElement*[numStacks]), m_elementHeap(allocationQuantum)
{
	if(numStacks == 0) {
		throw(std::runtime_error("numStacks must be larger than 0"));
	}
	memset(m_heads, 0, sizeof(StackElement*) * numStacks);
}

template<class T>
MultiStack<T>::~MultiStack(void) {
	delete[] m_heads;
}
	
template<class T>
void MultiStack<T>::push(unsigned int stackIndex, const T& val) {
	if(stackIndex >= m_numStacks) {
		throw;
	}
	
	StackElement* newElement = m_elementHeap.allocate();
	newElement->value = val;
	newElement->next = m_heads[stackIndex];
	m_heads[stackIndex] = newElement;
}

template<class T>
T MultiStack<T>::pop(unsigned int stackIndex) {
	if(isEmpty(stackIndex)) {
		throw;
	}

	StackElement* stackTop = m_heads[stackIndex];
	m_heads[stackIndex] = stackTop->next;
	// Note: we do not free stackTop because it is taken from ElementHeap and will be released when ElementHeap is reset().
	return stackTop->value; 
}

template<class T>
bool MultiStack<T>::isEmpty(unsigned int stackIndex) {
	if (stackIndex >= m_numStacks) {
		throw;
	}
	return (m_heads[stackIndex] == NULL);
}

template<class T>
void MultiStack<T>::reset() {
	memset(m_heads, 0, sizeof(StackElement*) * m_numStacks);
	m_elementHeap.reset();
}
