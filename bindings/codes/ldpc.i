/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.codes") ldpc

%include "common.i"
%include "codes/codes_workaround.i"
%import "codes/codes.i"

/////////////////
// Smart pointers
/////////////////
%shared_ptr(MatrixLDPCEncoder)


%{
#include "codes/ldpc/WordWidthTransformer.h"
#include "codes/ldpc/SparseMatrix.h"
#include "codes/ldpc/MatrixLDPCCode.h"
#include "codes/ldpc/MatrixLDPCEncoder.h"
#include "codes/ldpc/MatrixLDPCDecoder.h"
#include "codes/ldpc/WifiLDPC.h"
%}

%include "codes/ldpc/WordWidthTransformer.h"
%include "codes/ldpc/SparseMatrix.h"
%include "codes/ldpc/MatrixLDPCCode.h"
%include "codes/ldpc/MatrixLDPCEncoder.h"
%include "codes/ldpc/MatrixLDPCDecoder.h"
%include "codes/ldpc/WifiLDPC.h"

%template(UcharSparseMatrix) SparseMatrix<unsigned char>;
