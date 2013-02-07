/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") misc

%include "common.i"
%import "general.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

/////////////////
// numpy
////////////////

%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int* const seed, 
										int seedSize)}


%{
#include "PacketGenerator.h"
#include "CrcPacketGenerator.h"

#include "OracleDetector.h"
#include "CrcDetector.h"
#include "ComposedTransformation.h"

// For DefaultOracleDetector
#include "codes/IDecoder.h"
%}

%include "PacketGenerator.h"
%include "CrcPacketGenerator.h"

%include "OracleDetector.h"
%include "CrcDetector.h"
%include "ComposedTransformation.h"

%template(DefaultOracleDetector) OracleDetector<DecodeResult>;
%template(DefaultCrcDetector) CrcDetector<DecodeResult>;


// Utils class
%{
#include "util/Utils.h"
%}
%include "util/Utils.h"
%template(BlockifyUint32) Utils::blockify<uint32_t>;

// Extra utils:
%{
#include "util/BitStatCounter.h"
#include "util/BlockStatCounter.h"
%}
%include "util/BitStatCounter.h"
%include "util/BlockStatCounter.h"
