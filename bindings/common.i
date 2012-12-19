/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

///////////////////////
// Exception handling
///////////////////////
%include exception.i

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

/////////////////
// numpy
////////////////
%{
// numpy initialization
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"

%init %{
	import_array();
%}

/////////////////
// Smart pointers
/////////////////
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <std_shared_ptr.i>
%{
#include <tr1/memory>
%}

