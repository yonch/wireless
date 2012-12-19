/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/MatrixLDPCDecoder.h"
#include "codes/ldpc/MatrixLDPCNeighborGenerator.h"
#include "codes/ldpc/LinearCheckNodeUpdater.h"

#include "util/inference/bp/BipartiteBP.h"
#include "util/inference/bp/BipartiteGraph.h"
#include "util/inference/bp/LinearVariableNodeUpdater.h"


/******************************
 * MatrixLDPCDecoder
 */

MatrixLDPCDecoder::MatrixLDPCDecoder(	const MatrixLDPCCode & code,
										unsigned int numIter)
  : m_code(code),
    m_numIter(numIter)
{
	m_LLRs.reserve(m_code.n);
}


void MatrixLDPCDecoder::reset() {
	m_LLRs.clear();
}

void MatrixLDPCDecoder::add(const std::vector<float>& floats)
{
	if (m_LLRs.size() + floats.size() > m_code.n) {
		throw(std::runtime_error("cannot add more information than code size"));
	}

	m_LLRs.insert(m_LLRs.end(), floats.begin(), floats.end());
}

DecodeResult MatrixLDPCDecoder::decode() {
	if (m_LLRs.size() != m_code.n) {
		throw(std::runtime_error("decoder currently only supports providing all "
								 "soft values for the code, less were given"));
	}

	unsigned int numVariables = m_code.n * m_code.rateNumerator / m_code.rateDenominator;
	unsigned int numChecks = m_code.n - numVariables;

	MatrixLDPCNeighborGenerator neighborGenerator(m_code);

	// Construct a BipartiteGraph
	BipartiteGraph<BipartiteBP::QLLR>* graph = BipartiteGraph<BipartiteBP::QLLR>::createFromGenerator(
					numVariables + numChecks, numChecks, neighborGenerator);
	LinearVariableNodeUpdater variableUpdater(m_LLRs);
	LinearCheckNodeUpdater checkUpdater(numChecks, 12, 300, 7);
	std::vector<float> estimates;
	{
		BipartiteBP decoder(*graph, variableUpdater, checkUpdater);
		decoder.advance(m_numIter);
		decoder.get_soft_values(estimates);
	}
	delete graph;

	// perform hard decision
	std::vector<bool> hard_estimates;
	Utils::softToHardEstimates(estimates, hard_estimates);

	// get the string from hard decisions
	string codeword;
	Utils::vectorToString(hard_estimates, codeword);

	// get a string with all relevant bytes
	string message = codeword.substr(0, (numVariables + 7) / 8);

	// mask out bits that are not in the message
	if(numVariables % 8 != 0) {
		message[message.size() - 1] &= ((1 << (numVariables % 8)) - 1);
	}

	return DecodeResult(message, 0.0f);
}
