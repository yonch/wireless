/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.protocols") protocols

%include "common.i"
%import "general.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

/////////////////
// numpy
////////////////

%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int* const seed, 
										int seedSize)}

//////////////////
// General code-related
//////////////////
%{
#include "protocols/RateApproxProtocol.h"
#include "protocols/OneTryProtocol.h"
%}

%include "protocols/RateApproxProtocol.h"
%include "protocols/OneTryProtocol.h"
