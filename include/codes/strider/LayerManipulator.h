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
 * \brief Extracts estimates to original layers given linear combinations of layers and generator
 * matrix.
 */
class LayerManipulator {
public:
	typedef boost::numeric::ublas::matrix<ComplexSymbol,
				boost::numeric::ublas::column_major> boost_col_cmat;

	typedef boost::numeric::ublas::matrix<ComplexBaseType,
				boost::numeric::ublas::column_major> boost_col_mat;

	typedef boost::numeric::ublas::vector<ComplexSymbol> boost_cvec;

	typedef boost::numeric::ublas::vector<float> boost_vec;

	typedef boost::numeric::ublas::vector<double> boost_double_vec;

	/**
	 * C'tor
	 *
	 * @param layerLength: number of symbols in each layer
	 * @param G: generator matrix for the code
	 */
	LayerManipulator(unsigned int layerLength,
					std::complex<double>* matrixG,
					int rowsG,
					int colsG);

	/**
	 * Sets the symbols to be manipulated
	 */
	void setSymbols(unsigned int numFullPasses,
					unsigned int nonFullPassLength,
	                const boost_col_cmat& symbols);

	/**
	 * Sets the symbols to be manipulated
	 * This version also includes fading information
	 */
	void setSymbols(unsigned int numFullPasses,
					unsigned int nonFullPassLength,
	                const boost_col_cmat& symbols,
	                const boost_col_mat& fading);


	/**
	 * Performs maximal ratio combining on the symbols for given layer,
	 *    treating layers after it as interference.
	 */
	void maximalRatioCombining(unsigned int layerInd,
	                           float externalSnr,
	                           boost_cvec& combined,
	                           boost_vec& combinedScale);

	/**
	 * Subtracts the symbols corresponding to the given layer, so that other
	 *    layers can be decoded.
	 */
	void subtractLayer(unsigned int layerInd,
	                   const std::vector<ComplexSymbol>& symbols);

private:
	/**
	 * Performs maximal ratio combining on a part of the received symbols,
	 *    using given number of passes
	 * @param beginIndex: the symbol within a pass to start MRC
	 * @param endIndex: one after the last symbol to perform the MRC in a pass
	 * @param numPasses: the number of passes to combine
	 */
	void maximalRatioCombiningPartial(unsigned int layerInd,
	                                   float externalSnr,
	                                   boost_cvec& combined,
	                                   boost_vec& combinedScale,
	                                   unsigned int beginIndex,
	                                   unsigned int endIndex,
	                                   unsigned int numPasses);

	/**
	 * Performs maximal ratio combining on a part of the received symbols,
	 *    using given number of passes
	 *
	 * This version assumes all combined symbols share the same fading
	 * 	  coefficients
	 *
	 * @param beginIndex: the symbol within a pass to start MRC
	 * @param endIndex: one after the last symbol to perform the MRC in a pass
	 * @param numPasses: the number of passes to combine
	 */
	void maximalRatioCombiningSameFading(unsigned int layerInd,
	                                   float externalSnr,
	                                   const boost_double_vec& fading,
	                                   boost_cvec& combined,
	                                   boost_vec& combinedScale,
	                                   unsigned int beginIndex,
	                                   unsigned int endIndex,
	                                   unsigned int numPasses);

	/**
	 * Returns the interference power that layers below layerInd cause.
	 * 	   The interference is calculated per pass (so a vector is returned)
	 * @param layerInd: the layer below which the interference is found
	 * @param numPasses: the number of passes to compute
	 * @param interference: [out] the interference per pass
	 */
	void getInterference(unsigned int layerInd,
						 unsigned int numPasses,
						 boost_vec& interference);

	/**
	 * Calculates the weights required to do maximal ratio combining for a
	 *    signal with given noise and interference. Signal powers are calculated
	 *    from row layerInd of m_G.
	 *
	 * @return the signal power induced by the weights.
	 */
	float getWeights(unsigned int layerInd,
					 float noise,
					 const boost_vec& interference,
					 const boost_double_vec& fading,
					 boost_cvec& weights);

	/**
	 * Returns the interference after combining the signal using the given weights
	 */
	float getCoherentInterference(unsigned int layerInd,
								  const boost_cvec& weights,
								  const boost_double_vec& fading);

	// The generator matrix, G
	boost::numeric::ublas::matrix< ComplexSymbol,
						boost::numeric::ublas::column_major > m_G;

	// The channel symbols. Each pass is stored in a row.
	boost::numeric::ublas::matrix< ComplexSymbol,
						boost::numeric::ublas::column_major > m_symbols;

	// Fading information
	boost_col_mat m_fading;

	// The number of valid full passes in m_symbols
	unsigned int m_numFullPasses;

	// The number of symbols in the non-full pass
	unsigned int m_nonFullPassLength;

};
