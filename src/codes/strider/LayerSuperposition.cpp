/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/strider/LayerSuperposition.h"

#include <assert.h>
#include <boost/numeric/ublas/operation.hpp>

using namespace boost::numeric::ublas;

LayerSuperposition::LayerSuperposition(unsigned int layerLength,
								 std::complex<double>* matrixG,
								 int rowsG,
								 int colsG)
  :	m_G(rowsG, colsG),
   	m_layerSymbols(colsG, layerLength),
   	m_nextSymbol(0),
   	m_currentPass(0)
{
	for(int row = 0; row < rowsG; row++) {
		for(int col = 0; col < colsG; col++) {
			m_G(row, col) = *(matrixG++);
		}
	}
}

void LayerSuperposition::setLayer(
		unsigned int layerInd,
		const std::vector<ComplexSymbol> & layer)
{
	assert(layerInd < m_layerSymbols.size1());
	assert(layer.size() == m_layerSymbols.size2());

	for(unsigned int i = 0; i < m_layerSymbols.size2(); i++) {
		m_layerSymbols(layerInd,i) = layer[i];
	}
}

void LayerSuperposition::reset() {
	m_nextSymbol = 0;
	m_currentPass = 0;
}

ComplexSymbol LayerSuperposition::next() {
	ComplexSymbol res = inner_prod(row(m_G, m_currentPass),
								   column(m_layerSymbols, m_nextSymbol));

	m_nextSymbol++;
	if(m_nextSymbol == m_layerSymbols.size2()) {
		m_nextSymbol = 0;
		m_currentPass++;
	}

	return res;
}

