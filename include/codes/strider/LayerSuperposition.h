/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

#include "../../CodeBench.h"

/**
 * \ingroup strider
 * \brief Composes linear combinations of layers using a given matrix, to produce symbols
 */
class LayerSuperposition {
public:
	/**
	 * C'tor
	 *
	 * @param layerLength: number of symbols in each layer
	 * @param G: generator matrix for the code
	 */
	LayerSuperposition(unsigned int layerLength,
					std::complex<double>* matrixG,
					int rowsG,
					int colsG);

	/**
	 * Sets the packet to be encoded
	 */
	void setLayer(unsigned int layerInd,
				  const std::vector< ComplexSymbol >& layer);

	/**
	 * Resets the encoder to start from the first symbol of first pass
	 */
	void reset();

	/**
	 * Gets the next encoded symbol
	 */
	ComplexSymbol next();

private:
	// The generator matrix, G
	boost::numeric::ublas::matrix< ComplexSymbol,
						boost::numeric::ublas::row_major > m_G;

	// The layer symbols. Each layer is stored in a row.
	boost::numeric::ublas::matrix< ComplexSymbol,
						boost::numeric::ublas::column_major > m_layerSymbols;

	// The next symbol to be output within current pass
	unsigned int m_nextSymbol;

	// The current pass that is output (which row of m_G)
	unsigned int m_currentPass;
};
