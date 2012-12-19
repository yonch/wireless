/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") mappers

%include "common.i"
%import "general.i"

/////////////////
// numpy
////////////////

/////////////////
// Smart pointers
/////////////////
%shared_ptr(IMapper<ComplexSymbol>)
%shared_ptr(QPSKMapper)
%shared_ptr(QamMapper)
%shared_ptr(LinearMapper)


%{
#include "mappers/IMapper.h"
#include "mappers/LinearMapper.h"
#include "mappers/GrayMapper.h"
#include "mappers/GaussianMapper.h"
#include "mappers/NormalDistribution.h"
#include "mappers/TruncatedNormalDistribution.h"
#include "mappers/QPSKMapper.h"
#include "mappers/SoftMapper.h"
#include "mappers/ComplexLinearMapper.h"
#include "mappers/QamMapper.h"
%}

%include "mappers/IMapper.h"
%template(IComplexMapper) IMapper<ComplexSymbol>;

%include "mappers/LinearMapper.h"
%include "mappers/GrayMapper.h"
%include "mappers/GaussianMapper.h"
%include "mappers/TruncatedNormalDistribution.h"
%include "mappers/NormalDistribution.h"
%include "mappers/QPSKMapper.h"
%include "mappers/SoftMapper.h"
%include "mappers/ComplexLinearMapper.h"
%include "mappers/QamMapper.h"

