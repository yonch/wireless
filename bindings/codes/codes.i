/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") codes

%include "std_vector.i"
%include "common.i"
%import "general.i"
%include "codes/codes_workaround.i"

/////////////////
// numpy
////////////////

%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int* const seed, 
										int seedSize)}

/////////////////
// Smart pointers
/////////////////
%shared_ptr(IEncoder)
%shared_ptr(ILLRDecoder)
%shared_ptr(InterleavedEncoder)
%shared_ptr(EncoderMultiplexer)
%shared_ptr(IMultiStreamEncoder)


%shared_ptr(IPuncturingSchedule)
%shared_ptr(StridedPuncturingSchedule)
%shared_ptr(RepeatingPuncturingSchedule)
%shared_ptr(StaticPuncturingSchedule)
%shared_ptr(RoundRobinPuncturingSchedule)

%shared_ptr(MultiToSingleStreamEncoder)

%{
#include "codes/IDecoder.h"
#include "codes/ILLRDecoder.h"
#include "codes/DecodeResult.h"
#include "codes/IEncoder.h"
#include "codes/SymbolToLLRDecoderAdaptor.h"
#include "codes/InterleavedEncoder.h"
#include "codes/EncoderMultiplexer.h"
#include "codes/RandomPermutationGenerator.h"
#include "codes/IMultiStreamEncoder.h"
#include "codes/IMultiStreamDecoder.h"
#include "codes/MultiToSingleStreamEncoder.h"
#include "codes/MultiToSingleStreamDecoder.h"

#include "codes/puncturing/IPuncturingSchedule.h"
#include "codes/puncturing/StridedPuncturingSchedule.h"
#include "codes/puncturing/StaticPuncturingSchedule.h"
#include "codes/puncturing/RepeatingPuncturingSchedule.h"
#include "codes/puncturing/RoundRobinPuncturingSchedule.h"
%}

%include "codes/IDecoder.h"
%include "codes/ILLRDecoder.h"
%include "codes/DecodeResult.h"
%include "codes/IEncoder.h"
%include "codes/InterleavedEncoder.h"
%include "codes/EncoderMultiplexer.h"
%include "codes/RandomPermutationGenerator.h"
%include "codes/IMultiStreamEncoder.h"
%include "codes/IMultiStreamDecoder.h"
%include "codes/MultiToSingleStreamEncoder.h"
%include "codes/MultiToSingleStreamDecoder.h"



%include "demappers_workaround.i"
%import "demappers.i"
%include "codes/SymbolToLLRDecoderAdaptor.h"

%include "codes/puncturing/IPuncturingSchedule.h"
%include "codes/puncturing/StridedPuncturingSchedule.h"
%include "codes/puncturing/StaticPuncturingSchedule.h"
%include "codes/puncturing/RepeatingPuncturingSchedule.h"
%include "codes/puncturing/RoundRobinPuncturingSchedule.h"

// IDecoder shared_ptrs
%shared_ptr(IDecoder<Symbol>)
%shared_ptr(IDecoder<SoftSymbol>)
%shared_ptr(IDecoder<ComplexSymbol>)
%shared_ptr(IDecoder<FadingSymbol>)
%shared_ptr(IDecoder<FadingComplexSymbol>)

// IDecoder templates
%template(template_IDecoder_Symbol) IDecoder<Symbol>;
%template(template_IDecoder_SoftSymbol) IDecoder<SoftSymbol>;
%template(template_IDecoder_ComplexSymbol) IDecoder<ComplexSymbol>;
%template(template_IDecoder_FadingSymbol) IDecoder<FadingSymbol>;
%template(template_IDecoder_FadingComplexSymbol) IDecoder<FadingComplexSymbol>;

// IMultiStreamDecoder shared_ptrs
%shared_ptr(IMultiStreamDecoder<Symbol>)
%shared_ptr(IMultiStreamDecoder<SoftSymbol>)
%shared_ptr(IMultiStreamDecoder<ComplexSymbol>)
%shared_ptr(IMultiStreamDecoder<FadingSymbol>)
%shared_ptr(IMultiStreamDecoder<FadingComplexSymbol>)

// IMultiStreamDecoder shared_ptrs
%template(template_IMultiStreamDecoder_Symbol) IMultiStreamDecoder<Symbol>;
%template(template_IMultiStreamDecoder_SoftSymbol) IMultiStreamDecoder<SoftSymbol>;
%template(template_IMultiStreamDecoder_ComplexSymbol) IMultiStreamDecoder<ComplexSymbol>;
%template(template_IMultiStreamDecoder_FadingSymbol) IMultiStreamDecoder<FadingSymbol>;
%template(template_IMultiStreamDecoder_FadingComplexSymbol) IMultiStreamDecoder<FadingComplexSymbol>;

%shared_ptr(SymbolToLLRDecoderAdaptor<ComplexSymbol>)
%template(ComplexSymbolToLLRDecoderAdaptor) SymbolToLLRDecoderAdaptor<ComplexSymbol>; 

// MultiToSingleStreamDecoder shared_ptrs
%shared_ptr(MultiToSingleStreamDecoder<Symbol>)
%shared_ptr(MultiToSingleStreamDecoder<SoftSymbol>)
%shared_ptr(MultiToSingleStreamDecoder<ComplexSymbol>)
%shared_ptr(MultiToSingleStreamDecoder<FadingSymbol>)
%shared_ptr(MultiToSingleStreamDecoder<FadingComplexSymbol>)

// MultiToSingleStreamDecoder shared_ptrs
%template(MultiToSingleStreamDecoder_Symbol) MultiToSingleStreamDecoder<Symbol>;
%template(MultiToSingleStreamDecoder_SoftSymbol) MultiToSingleStreamDecoder<SoftSymbol>;
%template(MultiToSingleStreamDecoder_ComplexSymbol) MultiToSingleStreamDecoder<ComplexSymbol>;
%template(MultiToSingleStreamDecoder_FadingSymbol) MultiToSingleStreamDecoder<FadingSymbol>;
%template(MultiToSingleStreamDecoder_FadingComplexSymbol) MultiToSingleStreamDecoder<FadingComplexSymbol>;


namespace std {
   %template(vector_encoder) vector<std::tr1::shared_ptr<IEncoder> >;
};
