/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <itpp/base/binary.h>
#include <itpp/base/vec.h>
#include <stdint.h>

/**
 * \ingroup util
 * \brief Conversion utils to/from IT++ vectors
 */
class ItppUtils {
public:
	/**
	 * Converts a string into an itpp bvec.
	 * bvec must be of the appropriate size to the string.
	 */
	static void stringToVector(const std::string& src, itpp::bvec& dst);

	/**
	 * Converts an itpp bvec to a string.
	 * The string is resized to the appropriate size.
	 */
	static void vectorToString(const itpp::bvec& src, std::string& dst);

private:
	ItppUtils() {};
};

