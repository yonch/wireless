/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") turbo

%include "common.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

%shared_ptr(TurboEncoder)
%shared_ptr(TurboDecoder)
%shared_ptr(InterleavedDecoder<TurboDecoder>)

%{
#include "codes/turbo/TurboEncoder.h"
#include "codes/turbo/TurboDecoder.h"
#include "codes/InterleavedDecoder.h"
%}

%include "codes/turbo/TurboEncoder.h"
%include "codes/turbo/TurboDecoder.h"
%include "codes/InterleavedDecoder.h"

%template(InterleavedTurboDecoder) InterleavedDecoder<TurboDecoder>;