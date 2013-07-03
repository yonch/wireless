/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup hmm
 * \brief Interface for a class that prunes choices using weights.
 *
 * @note smaller is better.
 */
template<typename LabelType, typename Weight>
class IPruner {
public:
	/**
	 * Attempts to add the given datum.
	 *
	 * @return: true if the node *might* be added, false if the node has been pruned.
	 *
	 * @note: the return value is a conservative estimate: false means the node WILL NOT be in the
	 *     survivor set. Return of true can mean the node might survive or might be pruned later.
	 */
	bool add(LabelType label, Weight weight);

	/**
	 * Checks whether a value with weight might be added if add() is called.
	 *
	 * This is a conservative estimate, used for optimizations; an implementation that always
	 *     returns true is still correct.
	 *
	 * @return true if value might be added, false otherwise
	 */
	 bool checkWeight(Weight weight);

	/**
	 * Moves the non-pruned labels, in ascending order of weights, into the destination vector.
	 */
	void getSurvivors(std::vector<LabelType>& dest);

	/**
	 * Resets the pruner so another set of nodes can be pruned.
	 */
	void reset();

	/**
	 * Returns the current number of elements in the BestK.
	 */
	uint32_t size();

	/**
	 * @return the maximum possible number of elements that could be kept
	 */
	uint32_t maxSize();

protected:
	/**
	 * A structure that holds information about an element to be pruned.
	 */
	struct Suggestion {
		Suggestion(LabelType _label, Weight _weight)
			: label(_label), weight(_weight) {}

		// Compares two suggestions based on weight
		bool operator< (const Suggestion& other) const {
			return weight < other.weight;
		}

		// The node's label
		LabelType label;

		// The weight of the node
		Weight weight;
	};

	/**
	 * Returns the threshold weight for the next add: a node will be added only if its
	 *   weight is smaller than the threshold.
	 *
	 * If no node had ever been added, the result is undefined (or an exception might be
	 *     thrown)
	 */
	Weight nextAddThreshold();
};


