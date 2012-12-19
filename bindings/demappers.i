/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") demappers

%include "common.i"
%import "general.i"
%import "mappers.i"
%include "mappers_workaround.i"
%import "itpp/modulator.i"

/////////////////
// numpy
////////////////

%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int* const seed, 
										int seedSize)}

 
%{
#include "demappers/IDemapper.h"
#include "demappers/BitwiseDemapper.h"
#include "demappers/BscDemapper.h"
#include "demappers/NullDemapper.h"
#include "demappers/ItppDemapper.h"

// For GrayDemapper typedef
#include "mappers/GrayMapper.h"

// Workaround for %import problem
using namespace itpp;
%}

%include "demappers/IDemapper.h"
%include "demappers/BitwiseDemapper.h"
%include "demappers/BscDemapper.h"
%include "demappers/NullDemapper.h"
%include "demappers/ItppDemapper.h"

%shared_ptr(IDemapper<Symbol>)
%template(template_IDemapper_Symbol) IDemapper<Symbol>;

%shared_ptr(IDemapper<ComplexSymbol>)
%template(template_IDemapper_ComplexSymbol) IDemapper<ComplexSymbol>;
%template(GrayDemapper) BitwiseDemapper<GrayMapper>;

%shared_ptr(ItppDemapper<ComplexSymbol>)
%template(ItppComplexDemapper) ItppDemapper<ComplexSymbol>;
