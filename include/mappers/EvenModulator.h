/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>

using namespace std;

/**
 * EvenModulator
 *
 * Will modulate N bits on the I axis, and N bits on the Q axis, spaced evenly
 **/
class EvenModulator
{
public:
	/**
	 * C'tor
	 * @param numAxisBits the number of bits that will be coded into each axis
	 **/
	EvenModulator(unsigned int numAxisBits);

	/**
	 * Modulates the data in "data" into I and Q values. The last values are padded with 0's (wasteful!)
	 *
	 * @important Assumes iValues and qValues are empty (will only add values to them)
	 **/
	void modulate(const vector<bool> &data, vector<float>& iValues, vector<float>& qValues);

private:
	/**
	 * @param data the vector to extract bits from
	 * @param startIndex the first bit to extract, will be the MSB
	 * @param endIndex the index AFTER the last bit that will be the LSB
	 * @returns the bits in the vector "data" between the given locations
	 **/
	unsigned int getBits(const vector<bool>& data, unsigned int startIndex, unsigned int endIndex);

	// the number of bits encoded in each axis
	unsigned int m_numAxisBits;

	// the distance between two adjacent points on each axis (on the scale [-1,1]
	float m_delta;
};

