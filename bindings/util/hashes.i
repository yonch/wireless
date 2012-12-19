/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.util") hashes

%include exception.i
%include stdint.i

///////////////////////
// Exception handling
///////////////////////

%{
#include <exception>
%}

%exception {
	try {
		$function
	} catch(const std::exception& err) {
		SWIG_exception(SWIG_RuntimeError, err.what());
	} catch(...) {
		SWIG_exception(SWIG_RuntimeError, "Unknown exception");
	}
}


%{
#include "util/hashes/salsa20.h"
#include "util/hashes/SalsaHash.h"
#include "util/hashes/Lookup3Hash.h"
#include "util/hashes/OneAtATimeHash.h"
#include "util/hashes/UnlimitedHash.h"
#include "util/hashes/BitwiseXor.h"
#include "util/hashes/SingleSymbolFunction.h"
#include "util/hashes/SWIGHash.h"

#include "util/hashes/ShiftRegisterAdaptorHash.h"
%}

%include "util/hashes/salsa20.h"
%include "util/hashes/SalsaHash.h"
%include "util/hashes/Lookup3Hash.h"
%include "util/hashes/OneAtATimeHash.h"
%include "util/hashes/UnlimitedHash.h"
%include "util/hashes/BitwiseXor.h"
%include "util/hashes/SingleSymbolFunction.h"
%include "util/hashes/SWIGHash.h"

//%include "util/hashes/ShiftRegisterAdaptorHash.h"

%template(BitwiseXorSymbolFunction) SingleSymbolFunction<BitwiseXor>;

%template(Lookup3UnlimitedHash) UnlimitedHash<Lookup3SymbolFunction>;
%template(SalsaUnlimitedHash) UnlimitedHash<SalsaSymbolFunction>;

%template(Lookup3) SWIGHash<Lookup3Hash>;

// Make types available
%include "carrays.i"

%array_class(unsigned int, uint32_Array)

%pythoncode %{
# SALSA TYPES
def SalsaState():
	return uint32_Array(16)
%}
