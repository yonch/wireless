/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

/**
 * \ingroup bp
 * \brief A belief propagation message
 */
struct BPMessage {
public:
	unsigned int from;
	float value;
};

