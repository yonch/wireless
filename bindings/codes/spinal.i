/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") spinal

//%feature("autodoc")

%include exception.i
%include <std_string.i>
%include <std_vector.i>
%include <std_complex.i>
%{
// numpy initialization
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"


%include "common.i"
%import "general.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"


//////////////////
// STL
//////////////////
%include <std_vector.i>

%{
#include <vector>
%}
/////////////////
// numpy
////////////////
%init %{
	import_array();
%}


%shared_ptr(IEncoderFactory)
%shared_ptr(IDecoderFactory)
%shared_ptr(ISearchFactory<Symbol>)
%shared_ptr(IHashDecoder<Symbol>)
%shared_ptr(ISearchFactory<SoftSymbol>)
%shared_ptr(IHashDecoder<SoftSymbol>)
%shared_ptr(ISearchFactory<ComplexSymbol>)
%shared_ptr(IHashDecoder<ComplexSymbol>)
%shared_ptr(ISearchFactory<FadingSymbol>)
%shared_ptr(IHashDecoder<FadingSymbol>)
%shared_ptr(StubHashDecoder)





/////////////////
// Spinal Codes
/////////////////
%apply (int* INPLACE_ARRAY1, int DIM1) {(Symbol* outSymbols,
					 					int outSymbolsSize)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(const Symbol* symbols,
					 					int symbolsSize)}
%apply (unsigned long long* ARGOUT_ARRAY1, int DIM1) {(unsigned long long* outLikelihoods,
					 					int likelihoodsSize)}
%apply (unsigned long long* ARGOUT_ARRAY1, int DIM1) {(unsigned long long* outHashes,
					 					int hashesSize)}


%{
#include "codes/spinal/SpinalBranchEvaluator.h"

#include "codes/spinal/FlatSymbolStorage.h"

#include "codes/spinal/HashEncoder.h"
#include "codes/spinal/HashDecoder.h"
#include "codes/spinal/IHashDecoder.h"

#include "codes/spinal/protocols/StridedProtocol.h"

#include "codes/spinal/Composites.h"
#include "codes/spinal/StubHashDecoder.h"
#include "codes/spinal/CodeFactory.h"
%}

%include "codes/spinal/SpinalBranchEvaluator.h"
%include "codes/spinal/FlatSymbolStorage.h"
%include "codes/spinal/HashEncoder.h"
%include "codes/spinal/HashDecoder.h"
%include "codes/spinal/IHashDecoder.h"
%include "codes/spinal/protocols/StridedProtocol.h"
%include "codes/spinal/Composites.h"
%include "codes/spinal/StubHashDecoder.h"
%include "codes/spinal/CodeFactory.h"

// Decoder intermediate results
namespace std {
   %template(IntermediateVector) vector<SpinalIntermediateResult>;
};

// Decoders with different channel symbol types
%template(template_IHashDecoder_Symbol) IHashDecoder<Symbol>;
%template(template_ISearchFactory_Symbol) ISearchFactory<Symbol>;
%template(template_IHashDecoder_SoftSymbol) IHashDecoder<SoftSymbol>;
%template(template_ISearchFactory_SoftSymbol) ISearchFactory<SoftSymbol>;
%template(template_IHashDecoder_Fading) IHashDecoder<FadingSymbol>;
%template(template_ISearchFactory_Fading) ISearchFactory<FadingSymbol>; 


// Symbol storage
%template(SymbolFlatSymbolStorage) FlatSymbolStorage<Symbol>;
