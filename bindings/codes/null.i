/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") null

%include "common.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

// Fix import problem in hashes
%include "carrays.i" 
%array_class(unsigned int, uint32_Array)
%import "util/hashes.i"


%shared_ptr(NullEncoder)
%shared_ptr(NullDecoder)

%{
#include "codes/null/NullEncoder.h"
#include "codes/null/NullDecoder.h"
%}

%include "codes/null/NullEncoder.h"
%include "codes/null/NullDecoder.h"
