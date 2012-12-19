/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/strider/StriderFactory.h"

#include <tr1/memory>
#include <itpp/comm/modulator.h>
#include "codes/IEncoder.h"
#include "codes/SymbolToLLRDecoderAdaptor.h"
#include "codes/strider/StriderTurboCode.h"
#include "codes/strider/LayeredEncoder.h"
#include "codes/strider/LayeredDecoder.h"
#include "codes/strider/StriderGeneratorMatrix.h"
#include "mappers/QPSKMapper.h"
#include "demappers/ItppDemapper.h"

LayeredEncoder::Ptr StriderFactory::createEncoder(uint32_t fragmentLengthBits)
{
	// Get turbo encoder
	IEncoderPtr turbo(StriderTurboCode::createEncoder(fragmentLengthBits));

	// Get QPSK mapper
	IMapper<ComplexSymbol>::Ptr mapper(new QPSKMapper());

	// Get G
	std::complex<double> G[27*33];
	getStriderGeneratorMatrix(&G[0], 27, 33);

	// Get strider encoder
	LayeredEncoder::Ptr strider(
			new LayeredEncoder(turbo, mapper, (fragmentLengthBits-32)*33,
					(fragmentLengthBits * 5 / 2) + 15, G, 27, 33, true));

	return strider;
}


template<typename T>
typename IDecoder<T>::Ptr decoderCreatorHelper(uint32_t fragmentLengthBits)
{
	// Get turbo encoder
	IEncoderPtr turbo_enc(StriderTurboCode::createEncoder(fragmentLengthBits));

	// Get QPSK mapper
	IMapper<ComplexSymbol>::Ptr mapper(new QPSKMapper());

	// Get G
	std::complex<double> G[27*33];
	getStriderGeneratorMatrix(&G[0], 27, 33);

	// Get turbo encoder
	ILLRDecoder::Ptr turbo_dec(StriderTurboCode::createDecoder(fragmentLengthBits));

	// Get demapper
	ItppDemapper<ComplexSymbol>::ModulatorPtr qpsk(new itpp::QPSK());
	IDemapper<ComplexSymbol>::Ptr demapper(new ItppDemapper<ComplexSymbol>(qpsk, 2, false));

	// Get composite decoder
	IDecoder<ComplexSymbol>::Ptr composite_dec(
			new SymbolToLLRDecoderAdaptor<ComplexSymbol>(demapper, turbo_dec));

	// Get strider decoder
	typename LayeredDecoder<T>::Ptr strider(
			new LayeredDecoder<T>((fragmentLengthBits * 5 / 2) + 15, &G[0],
								  27, 33, fragmentLengthBits - 32,
								  composite_dec, turbo_enc, mapper, true));


	return strider;
}

IDecoder<ComplexSymbol>::Ptr StriderFactory::createDecoder(uint32_t fragmentLengthBits)
{
	return decoderCreatorHelper<ComplexSymbol>(fragmentLengthBits);
}

IDecoder<FadingComplexSymbol>::Ptr StriderFactory::createFadingDecoder(uint32_t fragmentLengthBits)
{
	return decoderCreatorHelper<FadingComplexSymbol>(fragmentLengthBits);
}


