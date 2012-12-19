/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/inference/bp/BipartiteBP.h"

#include "util/inference/bp/MultiVector.h"

BipartiteBP::BipartiteBP(BipartiteGraph<QLLR>& graph,
                            VariableNodeUpdater<QLLR>& variableNodeUpdater,
                            NodeUpdater<QLLR>& checkNodeUpdater)
: m_graph(graph),
  m_variableNodeUpdater(variableNodeUpdater),
  m_checkNodeUpdater(checkNodeUpdater)
{
	MultiVector<QLLR>& left(m_graph.left());
	for(uint32_t i = 0; i < left.total_num_elements(); i++) {
		left[i] = 0;
	}
}

void BipartiteBP::get_soft_values(std::vector<float> & llrs)
{
	m_variableNodeUpdater.estimate(m_graph.left(), llrs);
}

void BipartiteBP::advance(uint32_t numIterations)
{
	for (uint32_t i = 0; i < numIterations; i++) {
		// advance variable nodes
		m_variableNodeUpdater.update(m_graph.left());

		// transmit messages to right
		m_graph.leftToRight();

		// advance check nodes
		m_checkNodeUpdater.update(m_graph.right());

		// transmit messages to left
		m_graph.rightToLeft();
	}
}


