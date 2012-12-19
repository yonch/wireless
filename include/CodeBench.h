/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <string>
#include <complex>
#include <limits.h>

using namespace std;


// IMPORTANT: Remember to change all reference in pyCoding.i if changing
// the type of Symbol.
typedef int Symbol;
typedef float SoftSymbol;
typedef double ComplexBaseType;
typedef std::complex<ComplexBaseType> ComplexSymbol;
typedef std::complex<ComplexBaseType> ComplexNumber;

typedef float N0_t;
typedef float FadingMagnitude;
typedef float LLRValue;

#define SYMBOL_MAX INT_MAX
#define SYMBOL_MIN INT_MIN

/**
 * Container for a symbol together with fading information
 */
template <typename SymbolType>
struct FadingSymbolTuple {
	// Symbol
	SymbolType symbol;

	// the fading magnitude
	FadingMagnitude fading;
};

typedef FadingSymbolTuple<SoftSymbol> FadingSymbol;
typedef FadingSymbolTuple<ComplexSymbol> FadingComplexSymbol;
