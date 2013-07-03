/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>

#include "BipartiteGraph.h"
#include "NodeUpdater.h"

/**
 * \ingroup bp
 * \brief Belief propagation implementation on Bipartite Graphs
 */
class BipartiteBP
{
public:
	// Type for quantized LLR
	typedef int32_t QLLR;

	/**
	 * C'tor, bipartite belief propagation
	 *
	 * @param graph: the graph data structure on which to perform belief
	 * 		propagation
	 * @param variableNodeUpdater: an instance that updates variable nodes
	 * @param checkNodeUpdater: the implementation used to update check nodes
	 */
	BipartiteBP(
			BipartiteGraph<QLLR>& graph,
			VariableNodeUpdater<QLLR>& variableNodeUpdater,
			NodeUpdater<QLLR>& checkNodeUpdater);

	/**
	 * @return the Log Likelihood Ratios of variable nodes
	 **/
	void get_soft_values(std::vector<float>& llrs);

	/**
	 * Performs a round of belief propagation
	 **/
	void advance(uint32_t numIterations = 1);

private:
	// Graph with edges that transmit floats
	BipartiteGraph<QLLR>& m_graph;

	// An updater for variable nodes
	VariableNodeUpdater<QLLR>& m_variableNodeUpdater;

	// An updater for check nodes
	NodeUpdater<QLLR>& m_checkNodeUpdater;
};
