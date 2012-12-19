/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <itpp/itbase.h>

class TurboCodec {
public:
	/**
	 * Returns an interleaver for the given message length
	 */
	static void getInterleaver(uint32_t messageLength, itpp::ivec& interleaver);

private:
	TurboCodec() {};
};

