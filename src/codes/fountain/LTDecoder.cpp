/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/fountain/LTDecoder.h"

#include "codes/ldpc/LinearCheckNodeUpdater.h"
#include "util/inference/bp/BipartiteGraph.h"
#include "util/inference/bp/BipartiteBP.h"
#include "util/inference/bp/LinearVariableNodeUpdater.h"
#include "util/Utils.h"


LTDecoder::LTDecoder(uint32_t numVariables, uint32_t llrBufferSize,
                     uint32_t numIterations)
  : m_numVariables(numVariables),
    m_numIterations(numIterations),
    m_neighborGenerator(numVariables, 0xdeadbeef)
{
	m_llrs.reserve(llrBufferSize);
}

void LTDecoder::reset()
{
	m_llrs.clear();
}

void LTDecoder::add(const std::vector<LLRValue> & llrs)
{
	// Append the given LLRs
	m_llrs.insert(m_llrs.end(), llrs.begin(), llrs.end());
}

DecodeResult LTDecoder::decode()
{
	std::vector<float> estimates;
	softDecode(estimates);

	// perform hard decision
	std::vector<bool> hard_estimates;
	Utils::softToHardEstimates(estimates, hard_estimates);

	// get the string from hard decisions
	string codeword;
	Utils::vectorToString(hard_estimates, codeword);

	// get a string with all relevant bytes
	string message = codeword.substr(0, (m_numVariables + 7) / 8);

	// mask out bits that are not in the message
	if(m_numVariables % 8 != 0) {
		message[message.size() - 1] &= ((1 << (m_numVariables % 8)) - 1);
	}

	return DecodeResult(message, 0.0f);
}

void LTDecoder::softDecode(std::vector<LLRValue> & llrs)
{
	m_neighborGenerator.reset();

	// Construct a BipartiteGraph
	BipartiteGraph<BipartiteBP::QLLR>* graph =
			BipartiteGraph<BipartiteBP::QLLR>::createFromGenerator(
						m_numVariables, m_llrs.size(), m_neighborGenerator);
	LinearVariableNodeUpdater variableUpdater(m_numVariables);
	LinearCheckNodeUpdater checkUpdater(m_llrs,12,300,7);
	{
		BipartiteBP decoder(*graph, variableUpdater, checkUpdater);
		decoder.advance(m_numIterations);
		decoder.get_soft_values(llrs);
	}
	delete graph;
}

