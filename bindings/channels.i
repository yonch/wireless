/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") channels

%include "common.i"
%import "general.i"

/////////////////
// numpy
////////////////
%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int* const seed, 
										int seedSize)}
%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(uint32_t* const seed, 
												  int seedSize)}


%{
#include "channels/AwgnChannel.h"
#include "channels/BscChannel.h"
#include "channels/MimoChannel.h"
#include "channels/CoherenceFading.h"
#include "channels/TransparentCoherenceFading.h"
#include "channels/CompositeChannel.h"
%}


%include "channels/AwgnChannel.h"
%include "channels/BscChannel.h"
%include "channels/MimoChannel.h"
%include "channels/CoherenceFading.h"
%include "channels/TransparentCoherenceFading.h"
%include "channels/CompositeChannel.h"

%template(SymbolAwgnChannel) AwgnChannel<Symbol>;
%template(ComplexAwgnChannel) AwgnChannel<ComplexSymbol>;
%template(SoftAwgnChannel) AwgnChannel<SoftSymbol>;
%template(FadingAwgnChannel) AwgnChannel<FadingSymbol>;
%template(FadingComplexAwgnChannel) AwgnChannel<FadingComplexSymbol>;

%template(SoftCoherenceFading) CoherenceFading<SoftSymbol>;
%template(ComplexCoherenceFading) CoherenceFading<ComplexSymbol>;

%template(MimoAwgnChannel) CompositeChannel<MimoChannel,ComplexAwgnChannel>;
%template(AwgnCoherenceFadingChannel) CompositeChannel<SoftCoherenceFading, FadingAwgnChannel>; 
%template(AwgnCoherenceComplexFadingChannel) CompositeChannel<ComplexCoherenceFading, FadingComplexAwgnChannel>; 

%template(ComplexTransparentFadingChannel) TransparentCoherenceFading<ComplexSymbol>;
%template(SoftTransparentFadingChannel) TransparentCoherenceFading<SoftSymbol>;
%template(SymbolTransparentFadingChannel) TransparentCoherenceFading<Symbol>;