/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "demappers/NullDemapper.h"


void NullDemapper::process(const std::vector<Symbol>& inSymbols,
        				 std::vector<Symbol>& outSymbols)
{
	outSymbols = inSymbols;
}

Symbol NullDemapper::map(Symbol sym) {
	return sym;
}


