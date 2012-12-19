/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") strider

%include "common.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"
%include "mappers_workaround.i"
%import "mappers.i"

/////////////////
// numpy
////////////////

%numpy_typemaps(std::complex<double>,  NPY_CDOUBLE , int);
%apply (std::complex<double>* IN_ARRAY2, int DIM1, int DIM2) {(std::complex<double>* matrixG,
					 										  int rowsG,
					 										  int colsG)}
%apply (std::complex<double>* INPLACE_ARRAY2, int DIM1, int DIM2) {(std::complex<double>* outMatrixG,
					 										  int rowsG,
					 										  int colsG)}

/////////////////
// Smart pointers
/////////////////
%shared_ptr(LayeredEncoder)
%shared_ptr(LayeredDecoder<ComplexSymbol>)


%{
#include "codes/strider/LayeredEncoder.h"
#include "codes/strider/LayerSuperposition.h"
#include "codes/strider/StriderTurboCode.h"
#include "codes/strider/LayeredDecoder.h"
#include "codes/strider/StriderGeneratorMatrix.h"
#include "codes/strider/StriderFactory.h"
%}

%include "codes/strider/LayeredEncoder.h"
%include "codes/strider/LayerSuperposition.h"
%include "codes/strider/StriderTurboCode.h"
%include "codes/strider/LayeredDecoder.h"
%include "codes/strider/StriderGeneratorMatrix.h"
%include "codes/strider/StriderFactory.h"


%template(ComplexLayeredDecoder) LayeredDecoder<ComplexSymbol>;
