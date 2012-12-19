/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") fountain

%include "common.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

// Fix import problem in hashes
%include "carrays.i" 
%array_class(unsigned int, uint32_Array)
%import "util/hashes.i"


%shared_ptr(LTDecoder)
%shared_ptr(RaptorDecoder)

%{
#include "util/hashes/BitwiseXor.h"

#include "codes/fountain/LTParityNeighborGenerator.h"
#include "codes/fountain/ParityEncoder.h"
#include "codes/fountain/LTDecoder.h"
#include "codes/fountain/RaptorEncoder.h"
#include "codes/fountain/RaptorDecoder.h"

%}

%include "codes/fountain/LTParityNeighborGenerator.h"
%include "codes/fountain/ParityEncoder.h"
%include "codes/fountain/LTDecoder.h"
%include "codes/fountain/RaptorEncoder.h"
%include "codes/fountain/RaptorDecoder.h"

%template(LTEncoder) ParityEncoder<BitwiseXorSymbolFunction, LTParityNeighborGenerator>;
