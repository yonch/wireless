/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless") general

%include "common.i"

//////////////////
// STL
//////////////////
%include "std_string.i"
%include "std_vector.i"
%include "std_complex.i"
%include "stdint.i"

%{
#include <vector>
#include <string>
#include <complex>
#include "CodeBench.h"
#include "util/MTRand.h"
%}

%include "CodeBench.h"
%include "util/MTRand.h"

%pythoncode %{
Symbol = int
SoftSymbol = float
N0_t = float
FadingMagnitude = float
LLRValue = float
ComplexSymbol = complex
%}


%template(FadingSymbol) FadingSymbolTuple<SoftSymbol>;
%template(FadingComplexSymbol) FadingSymbolTuple<ComplexSymbol>;


// VECTOR TYPES
namespace std {
   %template(vectori) vector<int>;
   %template(vectorcf) vector< std::complex<float> >;
   %template(vectorcd) vector< std::complex<double> >;
   %template(vectorui) vector<unsigned int>;
   %template(vectorus) vector<unsigned short>;
   %template(vectors) vector<short>;
   %template(vectorul) vector<unsigned long>;
   %template(vectorull) vector<unsigned long long>;
   %template(vectorb) vector<bool>;
   %template(vectorf) vector<float>;
   %template(vectorstr) vector<string>;
   
   %template(vector_fadingsymbol) vector<FadingSymbol>;
   %template(vector_fading_csymbol) vector<FadingComplexSymbol>;
};


%pythoncode %{
vector_csymbol = vectorcd
vector_symbol = vectori
vector_softsymbol = vectorf
vector_llr = vectorf
%}