/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/strider/LayerManipulator.h"

#include <stdexcept>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>

using namespace boost::numeric::ublas;

LayerManipulator::LayerManipulator(
		unsigned int layerLength,
		std::complex<double> *matrixG,
		int rowsG,
		int colsG)
:	m_G(rowsG, colsG),
 	m_symbols(rowsG, layerLength),
 	m_fading(scalar_matrix<FadingMagnitude>(rowsG, layerLength, 1.0)),
 	m_numFullPasses(0),
 	m_nonFullPassLength(0)
{
	for(int row = 0; row < rowsG; row++) {
		for(int col = 0; col < colsG; col++) {
			m_G(row, col) = *(matrixG++);
		}
	}
}


void LayerManipulator::setSymbols(
		unsigned int numFullPasses,
		unsigned int nonFullPassLength,
		const boost_col_cmat & symbols)
{
	const unsigned int passLength = m_symbols.size2();
	m_numFullPasses = numFullPasses;
	m_nonFullPassLength = nonFullPassLength;

	// Copy short columns
	subrange(m_symbols, 0, m_numFullPasses,
						m_nonFullPassLength, passLength) =
		subrange(symbols, 0, m_numFullPasses,
						  m_nonFullPassLength, passLength);

	if(m_nonFullPassLength > 0) {
		// Copy long columns
		subrange(m_symbols, 0, m_numFullPasses+1, 0, m_nonFullPassLength) =
			subrange(symbols, 0, m_numFullPasses+1, 0, m_nonFullPassLength);
	}
}


void LayerManipulator::setSymbols(
		unsigned int numFullPasses,
		unsigned int nonFullPassLength,
		const boost_col_cmat & symbols,
		const boost_col_mat & fading)
{
	// Set the symbols
	setSymbols(numFullPasses, nonFullPassLength, symbols);

	// Set the fading coefficients
	// Copy short columns
	const unsigned int passLength = m_symbols.size2();
	subrange(m_fading, 0, m_numFullPasses,
						m_nonFullPassLength, passLength) =
		subrange(fading, 0, m_numFullPasses,
						  m_nonFullPassLength, passLength);

	if(m_nonFullPassLength > 0) {
		// Copy long columns
		subrange(m_fading, 0, m_numFullPasses+1, 0, m_nonFullPassLength) =
			subrange(fading, 0, m_numFullPasses+1, 0, m_nonFullPassLength);
	}
}



void LayerManipulator::maximalRatioCombining(
		unsigned int layerInd,
		float externalSnr,
		boost_cvec & combined,
		boost_vec & combinedScale)
{
	const unsigned int passLength = m_symbols.size2();

	maximalRatioCombiningPartial(layerInd,
	                             externalSnr,
	                             combined,
	                             combinedScale,
	                             m_nonFullPassLength,
	                             passLength,
	                             m_numFullPasses);

	if(m_nonFullPassLength > 0) {
		// We have symbols in a non-full pass, need to combine them as well
		maximalRatioCombiningPartial(layerInd,
		                             externalSnr,
		                             combined,
		                             combinedScale,
		                             0,
		                             m_nonFullPassLength,
		                             m_numFullPasses + 1);
	}
}

void LayerManipulator::subtractLayer(
		unsigned int layerInd,
		const std::vector<ComplexSymbol> & symbols)
{
	if(symbols.size() != m_symbols.size2()) {
		throw(std::runtime_error("Number of symbols to subtract should be equal to a layer's length"));
	}

	// full passes
	for(unsigned int i = m_nonFullPassLength; i < m_symbols.size2(); i++) {
		ComplexNumber sym = symbols[i];
		for(unsigned int pass = 0; pass < m_numFullPasses; pass++) {
			m_symbols(pass,i) -= sym * m_G(pass, layerInd) * m_fading(pass,i);
		}
	}

	// non full passes
	for(unsigned int i = 0; i < m_nonFullPassLength; i++) {
		ComplexNumber sym = symbols[i];
		for(unsigned int pass = 0; pass < m_numFullPasses + 1; pass++) {
			m_symbols(pass,i) -= sym * m_G(pass, layerInd) * m_fading(pass,i);
		}
	}
}

void LayerManipulator::maximalRatioCombiningPartial(unsigned int layerInd,
													float externalSnr,
													boost_cvec & combined,
													boost_vec & combinedScale,
													unsigned int beginIndex,
													unsigned int endIndex,
													unsigned int numPasses)
{
	boost_double_vec fading(
			matrix_vector_slice<boost_col_mat> (m_fading,
												slice(0,1,numPasses),
												slice(beginIndex,0,numPasses)));
	uint32_t fadingIndex = beginIndex;

	for(uint32_t i = beginIndex + 1; i < endIndex; i++) {
		matrix_vector_slice<boost_col_mat> thisFading(
												m_fading,
												slice(0,1,numPasses),
												slice(i,0,numPasses));
		for(uint32_t j = 0; j < numPasses; j++) {
			if(thisFading[j] != fading[j]) {
				// Run maximal ratio combining on previous sequence of indices,
				//  fadingIndex, fadingIndex+1, .., i-1
				maximalRatioCombiningSameFading(layerInd,
												externalSnr,
												fading,
												combined,
												combinedScale,
												fadingIndex,
												i,
												numPasses);
				// Update state so the fading information for index i will be cached
				fading = thisFading;
				fadingIndex = i;

				// no need to check more indices
				break;
			}
		}
	}

	// We finished the for loop, there is still another combining to do
	maximalRatioCombiningSameFading(layerInd,
								    externalSnr,
								    fading,
								    combined,
								    combinedScale,
								    fadingIndex,
								    endIndex,
								    numPasses);
}

void LayerManipulator::maximalRatioCombiningSameFading(
		unsigned int layerInd,
		float externalSnr,
		const boost_double_vec & fading,
		boost_cvec & combined,
		boost_vec & combinedScale,
		unsigned int beginIndex,
		unsigned int endIndex,
		unsigned int numPasses)
{
	// debug print
//	cout << "MRC indices [" << beginIndex << ", " << endIndex << ")"
//			<< " num passes=" << numPasses << endl;

	// Stores the interference from other layers
	boost_vec interference;

	// Weights to combine the passes coherently
	boost_cvec weights;

	// Get the internal interference power for each of the layers
	getInterference(layerInd, numPasses, interference);

	// Calculate normalized weights
	float signalPower = getWeights(layerInd,
									externalSnr,
									interference,
									fading,
									weights);

	if (signalPower == 0) {
		subrange(combined, beginIndex, endIndex) =
				scalar_vector<float>(endIndex - beginIndex, 0);
		subrange(combinedScale, beginIndex, endIndex) =
				scalar_vector<float>(endIndex - beginIndex, 0);
	} else {
		// Calculate SNRs of combined signal after coherent combining of
		// interference
		float interferencePower = getCoherentInterference(layerInd, weights, fading);

		// Calculate normalized noise power
		float noisePower = externalSnr * real(inner_prod(weights, conj(weights)));

		// Scaling factor makes the noise+interference be of power externalSnr
		float noiseNormalizationFactor =
				sqrt(externalSnr / (noisePower + interferencePower));
		assert(isfinite(noiseNormalizationFactor));

		// combinedScale is the total scaling from combinedScalingFactor and
		// the weights
		float scale = sqrt(signalPower) * noiseNormalizationFactor;
		assert(isfinite(scale));
		subrange(combinedScale, beginIndex, endIndex) =
				scalar_vector<float>(endIndex - beginIndex, scale);


		// Combine the different passes into one coherent reception
		subrange(combined, beginIndex, endIndex) =
				   double(noiseNormalizationFactor) *
				   prod(weights,
						subrange(m_symbols, 0, numPasses,
											beginIndex, endIndex));
		// debug print
//		cout << "combined_scale=" << scale
//			 << " signal_power=" << signalPower
//			 << " interference_power=" << interferencePower
//			 << " noise_norm=" << noiseNormalizationFactor
//				<< endl;

	}
}

void LayerManipulator::getInterference(	unsigned int layerInd,
										unsigned int numPasses,
										boost_vec & interference)
{
	matrix_range<boost_col_cmat> coeffs(
			subrange(m_G, 0, numPasses, layerInd+1, m_G.size2()));

	if(layerInd != m_G.size2() - 1) {
		interference =
			real(
				prod(element_prod(coeffs, conj(coeffs)),
					 scalar_vector<ComplexBaseType>(m_G.size2() - (layerInd + 1), 1.0f)));
	} else {
		interference = scalar_vector<float>(numPasses, 0);
	}
}


float LayerManipulator::getWeights(
		unsigned int layerInd,
		float noise,
		const boost_vec & interference,
		const boost_double_vec & fading,
		boost_cvec & weights)
{
	const unsigned int num_passes = interference.size();

	matrix_vector_slice<boost_col_cmat> layerCoeff(m_G,
												   slice(0,1,num_passes),
												   slice(layerInd,0,num_passes));
	// Calculate un-normalized weights
	weights = element_div(element_prod(conj(layerCoeff), fading),
						  ( element_prod(interference, element_prod(fading,fading))
						   + scalar_vector<ComplexBaseType>(num_passes,noise)));

	float scale = real(inner_prod(weights, element_prod(layerCoeff, fading)));
	return scale*scale;
}

float LayerManipulator::getCoherentInterference(
		unsigned int layerInd,
		const boost_cvec & weights,
		const boost_double_vec & fading)
{
	const unsigned int num_passes = weights.size();

	// Get interference power if not on the last layer
	float interferencePower = 0;
	if(layerInd != m_G.size2() - 1) {
		boost_cvec layerInterference =
			prod(element_prod(weights,fading),
				 subrange(m_G, 0, num_passes, layerInd+1, m_G.size2()));

		interferencePower = real(inner_prod(layerInterference,
											conj(layerInterference)));
	}

	return interferencePower;
}



