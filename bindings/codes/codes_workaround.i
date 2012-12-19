/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

%{
#include "codes/SymbolToLLRDecoderAdaptor.h"
#include "codes/InterleavedEncoder.h"
#include "codes/EncoderMultiplexer.h"
#include "codes/puncturing/IPuncturingSchedule.h"
#include "codes/puncturing/StridedPuncturingSchedule.h"
#include "codes/puncturing/StaticPuncturingSchedule.h"
#include "codes/puncturing/RepeatingPuncturingSchedule.h"
#include "codes/puncturing/RoundRobinPuncturingSchedule.h"
#include "codes/MultiToSingleStreamEncoder.h"
#include "codes/MultiToSingleStreamDecoder.h"

// demappers:
#include <itpp/comm/modulator.h>
using namespace itpp;
#include "demappers/ItppDemapper.h"

// mappers:
#include "mappers/QPSKMapper.h"
#include "mappers/QamMapper.h"

%}
