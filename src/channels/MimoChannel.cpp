/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "channels/MimoChannel.h"

#include <boost/numeric/ublas/operation.hpp>

MimoChannel::MimoChannel(
		unsigned int nTransmitter,
		unsigned int nReceiver,
		const std::vector<ComplexNumber> & channelMatrix)
  : m_nTransmitter(nTransmitter),
    m_nReceiver(nReceiver),
    m_channelMatrix(nReceiver, nTransmitter)
{
	for(unsigned int row = 0; row < nReceiver; row++) {
		for(unsigned int col = 0; col < nTransmitter; col++) {
			m_channelMatrix(row, col) = channelMatrix[row * nTransmitter + col];
		}
	}
}

void MimoChannel::seed(unsigned int *const , int )
{
	// no RNG in this class
	return;
}



void MimoChannel::process(
		const std::vector<ComplexSymbol> & inSymbols,
		std::vector<ComplexSymbol> & outSymbols)
{
	unsigned int numIQSymbols = inSymbols.size();

	unsigned int numChannelUsages = numIQSymbols / m_nTransmitter;

	if (inSymbols.size() != m_nTransmitter * numChannelUsages) {
		throw std::runtime_error("input symbols size should contain symbols for all transmit antenna");
	}

	unsigned int numOutputSymbols = numChannelUsages * m_nReceiver;
	outSymbols.clear();
	outSymbols.resize(numOutputSymbols);

	// Initialize a matrix with all symbols
	boost::numeric::ublas::matrix<ComplexSymbol> symbolMatrix(m_nTransmitter,
															  numChannelUsages);
	unsigned int inSymbolIter = 0;
	for(unsigned int col = 0; col < numChannelUsages; col++) {
		for(unsigned int row = 0; row < m_nTransmitter; row++) {
			symbolMatrix(row, col) = inSymbols[inSymbolIter++];
		}
	}

	// Multiply channel matrix by symbol matrix
	boost::numeric::ublas::matrix<ComplexSymbol> outputMatrix(m_nReceiver,
															  numChannelUsages);
	axpy_prod(m_channelMatrix, symbolMatrix, outputMatrix, true);

	// Read the output symbols from outputMatrix
	unsigned int outSymbolIter = 0;
	for(unsigned int col = 0; col < numChannelUsages; col++) {
		for(unsigned int row = 0; row < m_nReceiver; row++) {
			outSymbols[outSymbolIter++] = outputMatrix(row, col);
		}
	}

}

unsigned int MimoChannel::forecast(unsigned int numOutputs)
{
	unsigned int numChannelUsages = (numOutputs + m_nReceiver - 1) / m_nReceiver;
	return numChannelUsages * m_nTransmitter;
}

