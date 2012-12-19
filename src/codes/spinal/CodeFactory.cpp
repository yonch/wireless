/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/spinal/CodeFactory.h"

#include "util/hashes/SalsaHash.h"
#include "util/hashes/Lookup3Hash.h"
#include "util/hashes/OneAtATimeHash.h"

// Channel transformations
#include "channels/CoherenceFading.h"
#include "mappers/LinearMapper.h"
#include "mappers/ComplexLinearMapper.h"
#include "mappers/GaussianMapper.h"
#include "mappers/SoftMapper.h"
#include "ComposedTransformation.h"
#include "TransformationAdaptor.h"

// Searches
#include "util/inference/hmm/BestK.h"
#include "util/inference/hmm/ParallelBestK.h"
#include "util/inference/hmm/BeamSearch.h"
#include "util/inference/hmm/LookaheadBeamSearch.h"

// Branch evaluators
#include "codes/spinal/SpinalBranchEvaluator.h"

#include "codes/spinal/HashEncoder.h"
#include "codes/spinal/HashDecoder.h"

/**********************************
 * Class EncoderFactory definition
 **********************************/
template<typename SpineValueType>
class EncoderFactory : public IEncoderFactory {
public:
	EncoderFactory(unsigned int k,
	            unsigned int spineLength);

	virtual IMultiStreamEncoder::Ptr encoder();

	virtual ISymbolSearchFactoryPtr linear(unsigned int premapperNumBits,
										   unsigned int precisionBits);
	virtual ISoftSearchFactoryPtr soft(unsigned int premapperNumBits);
	virtual ISymbolSearchFactoryPtr gaussian(unsigned int premapperNumBits,
											 unsigned int precisionBits,
											 float numStandardDevs);
	virtual ISymbolSearchFactoryPtr bitwise(unsigned int numBits);
	virtual IFadingSearchFactoryPtr coherence(unsigned int premapperNumBits);
private:
	unsigned int m_k;
	unsigned int m_spineLength;
};

/**********************************
 * Class SearchFactory definition
 **********************************/
template<typename BranchEvaluator>
class SearchFactory : public ISearchFactory<typename BranchEvaluator::ChannelSymbol> {
public:
	typedef typename BranchEvaluator::ChannelSymbol ChannelSymbol;
	typedef std::tr1::shared_ptr<IHashDecoder<ChannelSymbol> > IHashDecoderPtr;

	SearchFactory(unsigned int k,
				unsigned int spineLength,
				const BranchEvaluator& branchEvaluator);

	virtual IHashDecoderPtr beamDecoder(
			unsigned int numLists,
			unsigned int numBestPerList,
			unsigned int maxNumSymbolsPerValue,
			unsigned int maxNumSymbolsLastValue);
	virtual IHashDecoderPtr lookaheadBeamDecoder(
			unsigned int beamWidth,
			unsigned int lookaheadDepth,
			unsigned int maxNumSymbolsPerValue,
			unsigned int maxNumSymbolsLastValue);
private:
	const unsigned int m_k;
	const unsigned int m_spineLength;
	BranchEvaluator m_branchEvaluator;
};


/************************************************
 * CODE FACTORY IMPLEMENTATION
 ************************************************/
CodeFactory::CodeFactory(unsigned int k,
                         unsigned int spineLength)
  : m_k(k),
    m_spineLength(spineLength)
{}

IEncoderFactoryPtr CodeFactory::salsa() {
	return IEncoderFactoryPtr (
		new EncoderFactory<SalsaUnlimitedHash>(
			m_k, m_spineLength));
}

IEncoderFactoryPtr CodeFactory::lookup3()
{
	return IEncoderFactoryPtr (
		new EncoderFactory<Lookup3UnlimitedHash>(
			m_k, m_spineLength));
}

IEncoderFactoryPtr CodeFactory::oneAtATime()
{
	return IEncoderFactoryPtr (
		new EncoderFactory<OneAtATimeUnlimitedHash>(
			m_k, m_spineLength));
}


/************************************************
 * ENCODER FACTORY IMPLEMENTATION
 ************************************************/
template<typename SpineValueType>
inline EncoderFactory<SpineValueType>::EncoderFactory(
		unsigned int k,
		unsigned int spineLength)
  : m_k(k),
	m_spineLength(spineLength)
{}

template<typename SpineValueType>
inline IMultiStreamEncoder::Ptr
EncoderFactory<SpineValueType>::encoder()
{
	return IMultiStreamEncoder::Ptr (
		new HashEncoder<SpineValueType>(
			m_k,
			m_spineLength));
}

template<typename SpineValueType>
inline ISymbolSearchFactoryPtr
EncoderFactory<SpineValueType>::linear(
		unsigned int premapperNumBits,
		unsigned int precisionBits)
{
	typedef TransformationAdaptor<LinearMapper> Transformation;
	typedef SpinalBranchEvaluator<SpineValueType, Transformation, IntegerEuclidianDistance> BranchEval;
	return ISymbolSearchFactoryPtr (
		new SearchFactory<BranchEval>(
			m_k,
			m_spineLength,
			BranchEval(Transformation(LinearMapper(premapperNumBits, precisionBits)),
					   m_k)));
}


template<typename SpineValueType>
inline ISoftSearchFactoryPtr EncoderFactory<SpineValueType>::soft(
		unsigned int premapperNumBits)
{
	typedef TransformationAdaptor<SoftMapper> Transformation;
	typedef SpinalBranchEvaluator<SpineValueType, Transformation, SoftEuclidianDistance> BranchEval;
	return ISoftSearchFactoryPtr (
		new SearchFactory<BranchEval>(
			m_k,
			m_spineLength,
			BranchEval(Transformation(SoftMapper(premapperNumBits)),
					   m_k)));
}

template<typename SpineValueType>
inline ISymbolSearchFactoryPtr
EncoderFactory<SpineValueType>::gaussian(
		unsigned int premapperNumBits,
		unsigned int precisionBits,
		float numStandardDevs)
{
	typedef TransformationAdaptor<GaussianMapper> Transformation;
	typedef SpinalBranchEvaluator<SpineValueType, Transformation, IntegerEuclidianDistance> BranchEval;
	GaussianMapper mapper(premapperNumBits,
						  precisionBits,
						  numStandardDevs);
	return ISymbolSearchFactoryPtr (
		new SearchFactory<BranchEval>(
			m_k,
			m_spineLength,
			BranchEval(Transformation(mapper), m_k)));
}

template<typename SpineValueType>
inline ISymbolSearchFactoryPtr
EncoderFactory<SpineValueType>::bitwise(unsigned int numBits)
{
	typedef TransformationAdaptor<LinearMapper> Transformation;
	typedef SpinalBranchEvaluator<SpineValueType, Transformation, HammingDistance> BranchEval;
	return ISymbolSearchFactoryPtr (
		new SearchFactory<BranchEval>(
			m_k,
			m_spineLength,
			BranchEval(Transformation(LinearMapper(numBits, numBits)), m_k)));
}

template<typename SpineValueType>
inline IFadingSearchFactoryPtr EncoderFactory<SpineValueType>::coherence(
		unsigned int premapperNumBits)
{
	typedef ComposedTransformation2<SoftMapper, SoftCoherenceFading> Transformation;

	SoftMapper softMapper(premapperNumBits);
	SoftCoherenceFading coherenceXform(1);

	typedef SpinalBranchEvaluator<SpineValueType, Transformation, FadingEuclidianDistance> BranchEval;
	return IFadingSearchFactoryPtr (
		new SearchFactory<BranchEval>(
			m_k,
			m_spineLength,
			BranchEval(Transformation(softMapper, coherenceXform, 128), m_k)));

}

/************************************************
 * SEARCH FACTORY IMPLEMENTATION
 ************************************************/
template<typename BranchEvaluator>
inline SearchFactory<BranchEvaluator>::SearchFactory(
		unsigned int k,
		unsigned int spineLength,
		const BranchEvaluator & branchEvaluator)
  : m_k(k),
	m_spineLength(spineLength),
	m_branchEvaluator(branchEvaluator)
{}

template<typename BranchEvaluator>
inline typename SearchFactory<BranchEvaluator>::IHashDecoderPtr
SearchFactory<BranchEvaluator>::beamDecoder(
		unsigned int numLists,
		unsigned int numBestPerList,
		unsigned int maxNumSymbolsPerValue,
		unsigned int maxNumSymbolsLastValue)
{
	typedef BeamSearch<BranchEvaluator, ParallelBestK> Search;
	typedef typename BranchEvaluator::ChannelSymbol ChannelSymbol;

	typename Search::PrunerParams prunerParams(numLists, numBestPerList);

	return IHashDecoderPtr (
		new HashDecoder<Search> (
			m_k,
			m_spineLength,
			maxNumSymbolsPerValue,
			maxNumSymbolsLastValue,
			BeamSearch<BranchEvaluator, ParallelBestK>(
										prunerParams,
										m_spineLength,
										m_branchEvaluator,
										m_k)));
}

template<typename BranchEvaluator>
inline typename SearchFactory<BranchEvaluator>::IHashDecoderPtr
SearchFactory<BranchEvaluator>::lookaheadBeamDecoder(
		unsigned int beamWidth,
		unsigned int lookaheadDepth,
		unsigned int maxNumSymbolsPerValue,
		unsigned int maxNumSymbolsLastValue)
{
	typedef LookaheadBeamSearch<BranchEvaluator, BestK> Search;

	return IHashDecoderPtr (
		new HashDecoder<Search> (
			m_k,
			m_spineLength,
			maxNumSymbolsPerValue,
			maxNumSymbolsLastValue,
			LookaheadBeamSearch<BranchEvaluator, BestK>(beamWidth,
												m_spineLength,
												m_branchEvaluator,
												m_k,
												lookaheadDepth)));
}
