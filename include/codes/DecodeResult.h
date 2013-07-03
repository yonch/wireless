/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>

/**
 * \ingroup codes
 * \brief The result of a decode attempt
 */
struct DecodeResult {
	DecodeResult(const std::string& _packet, float _logProbError)
		: packet(_packet), logProbError(_logProbError) {}
	DecodeResult() {};

	// The decoded packet
	std::string packet;

	// The log of the probability of error as appears from decoding process
	float logProbError;
};

