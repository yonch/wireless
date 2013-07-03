/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup hashes
 * \brief Python wrapper for hash functions
 */
template<typename HashFunctionT>
class SWIGHash {
public:
	typedef typename HashFunctionT::Digest Digest;
	typedef typename HashFunctionT::State State;

	/**
	 * C'tor
	 * @param digest: the digest to initialize from
	 */
	SWIGHash(Digest digest);

	/**
	 * Returns the current State
	 */
	State& getState();

	/**
	 * Updates the hash with 'data'
	 */
	void update(unsigned int data);

	/**
	 * Returns the digest
	 */
	Digest digest();

private:
	// The internal state
	State m_state;
};

template<typename HashFunctionT>
inline SWIGHash<HashFunctionT>::SWIGHash(Digest digest) {
	HashFunctionT::init(digest, m_state);
}



template<typename HashFunctionT>
inline typename SWIGHash<HashFunctionT>::State&
						SWIGHash<HashFunctionT>::getState()
{
	return m_state;
}



template<typename HashFunctionT>
inline void SWIGHash<HashFunctionT>::update(unsigned int data) {
	HashFunctionT::update(m_state, data);
}



template<typename HashFunctionT>
inline typename SWIGHash<HashFunctionT>::Digest
						SWIGHash<HashFunctionT>::digest()
{
	return HashFunctionT::digest(m_state);
}
