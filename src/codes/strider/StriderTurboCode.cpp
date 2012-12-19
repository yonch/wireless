/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/strider/StriderTurboCode.h"

#include <vector>
#include <stdexcept>
#include <boost/static_assert.hpp>

#include "codes/turbo/TurboEncoder.h"
#include "codes/turbo/TurboDecoder.h"
#include "codes/InterleavedEncoder.h"
#include "codes/strider/StriderInterleaver.h"
#include "codes/RandomPermutationGenerator.h"

IEncoderPtr StriderTurboCode::createEncoder(uint32_t lengthBits)
{
	// Make turbo encoder
	IEncoderPtr turbo(new TurboEncoder(lengthBits));

	// Create interleaving sequence
	std::vector<uint16_t> interleavingSequence;
	getInterleaver(lengthBits, interleavingSequence);

	// Create interleaved encoder
	IEncoderPtr strider(new InterleavedEncoder(turbo, interleavingSequence));

	return strider;
}

ILLRDecoder::Ptr StriderTurboCode::createDecoder(uint32_t lengthBits)
{
	// Create interleaving sequence
	std::vector<uint16_t> interleavingSequence;
	getInterleaver(lengthBits, interleavingSequence);

	// Create non-interleaved decoder
	TurboDecoder turbo(lengthBits);

	// Create interleaved decoder
	ILLRDecoder::Ptr strider(
			new InterleavedTurboDecoder(turbo, interleavingSequence));

	return strider;
}

void StriderTurboCode::getInterleaver(
		uint32_t lengthBits,
		std::vector<uint16_t> & interleavingSequence)
{
	if(lengthBits == 1530) {
		interleavingSequence.reserve(7680);
		interleavingSequence.assign(striderInterleaver,
									striderInterleaver + 7680);
	} else {
		BOOST_STATIC_ASSERT(RandomPermutationGenerator::SEED_SIZE_UINTS == 4);

		uint32_t numEncodedBits = (5 * lengthBits) + (6 * 3);
		uint32_t numAfterInterleave = numEncodedBits + (6 * 2);

		// Get permutation generator
		uint32_t seed[4] = {4212754317, 3259287066,  877792689, 2365549148};
		RandomPermutationGenerator permGen(numAfterInterleave, seed);

		// set destination size
		interleavingSequence.clear();
		interleavingSequence.reserve(numAfterInterleave);

		// fill the interleaver sequence
		for(uint32_t i = 0; i < numAfterInterleave; i++) {
			uint16_t permVal = permGen.next();
			if(permVal < numEncodedBits) {
				interleavingSequence.push_back(permVal);
			} else {
				uint16_t repeatedIndex = (permVal - numEncodedBits) / 2;
				uint16_t repeatLocation = (5 * lengthBits) + 3 * repeatedIndex;
				interleavingSequence.push_back(repeatLocation);
			}
		}
	}
}





