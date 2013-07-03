/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <tr1/memory>
#include <vector>
#include "../../CodeBench.h"
#include "../IDecoder.h"
#include "../IMultiStreamEncoder.h"
#include "IHashDecoder.h"

// Forward declarations
class IEncoderFactory;
template<typename ChannelSymbol> class ISearchFactory;

/// Smart pointer typedefs
typedef std::tr1::shared_ptr<IEncoderFactory> IEncoderFactoryPtr;
typedef std::tr1::shared_ptr<ISearchFactory<Symbol> > ISymbolSearchFactoryPtr;
typedef std::tr1::shared_ptr<ISearchFactory<SoftSymbol > > ISoftSearchFactoryPtr;
typedef std::tr1::shared_ptr<ISearchFactory<FadingSymbol > > IFadingSearchFactoryPtr;

/**
 * \ingroup spinal
 * \brief Main factory for spinal encoders and decoders
 */
class CodeFactory {
public:
	CodeFactory(unsigned int k,
	            unsigned int spineLength);

	IEncoderFactoryPtr salsa();
	IEncoderFactoryPtr lookup3();
	IEncoderFactoryPtr oneAtATime();

private:
	unsigned int m_k;
	unsigned int m_spineLength;
};

/**
 * \ingroup spinal
 * \brief Intermediate spinal factory: returned from CodeFactory
 */
class IEncoderFactory {
public:
	virtual ~IEncoderFactory() {}
	virtual IMultiStreamEncoder::Ptr encoder() = 0;

	virtual ISymbolSearchFactoryPtr linear(unsigned int premapperNumBits,
										   unsigned int precisionBits) = 0;
	virtual ISoftSearchFactoryPtr soft(unsigned int premapperNumBits) = 0;
	virtual ISymbolSearchFactoryPtr gaussian(unsigned int premapperNumBits,
											 unsigned int precisionBits,
											 float numStandardDevs) = 0;
	virtual ISymbolSearchFactoryPtr bitwise(unsigned int numBits) = 0;
	virtual IFadingSearchFactoryPtr coherence(unsigned int premapperNumBits) = 0;
};

/**
 * \ingroup spinal
 * \brief Intermediate spinal factory: returned from EncoderFactory
 */
template<typename ChannelSymbol>
class ISearchFactory {
public:
	typedef std::tr1::shared_ptr<ISearchFactory<ChannelSymbol> > Ptr;
	typedef std::tr1::shared_ptr<IHashDecoder<ChannelSymbol> > IHashDecoderPtr;

	virtual ~ISearchFactory() {};
	virtual IHashDecoderPtr beamDecoder(
			unsigned int numLists,
			unsigned int numBestPerList,
			unsigned int maxNumSymbolsPerValue,
			unsigned int maxNumSymbolsLastValue) = 0;
	virtual IHashDecoderPtr lookaheadBeamDecoder(
			unsigned int beamWidth,
			unsigned int lookaheadDepth,
			unsigned int maxNumSymbolsPerValue,
			unsigned int maxNumSymbolsLastValue) = 0;
};

