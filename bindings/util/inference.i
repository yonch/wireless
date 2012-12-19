/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.util") inference

%include "std_vector.i"
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

/////////////////
// Smart pointers
/////////////////
#define SWIG_SHARED_PTR_SUBNAMESPACE tr1
%include <std_shared_ptr.i>
%{
#include <tr1/memory>
%}


%{
#include "util/inference/bp/NodeUpdater.h"
#include "util/inference/bp/MultiVector.h"
#include "util/inference/bp/BipartiteGraph.h"
%}

// NodeUpdater
%include "util/inference/bp/NodeUpdater.h"
%template(template_NodeUpdater_int) NodeUpdater<signed int>;

// MultiVector
%ignore MultiVector::operator[](uint32_t);
%include "util/inference/bp/MultiVector.h"
%extend MultiVector {
	T __getitem__(uint32_t index) {
		return (*$self)[index];
	}
	void __setitem__(uint32_t index, const T& val) {
		(*$self)[index] = val;
	}
}
%template(MultiVectorInt) MultiVector<signed int>;
%template(MultiVectorUint32) MultiVector<uint32_t>;
%template(MultiVectorUint16) MultiVector<uint16_t>;

// BipartiteGraph
#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif
#if defined(WITH_ITPP)
%import "itpp/ldpc.i"
#endif
%include "util/inference/bp/BipartiteGraph.h"
%shared_ptr(BipartiteGraph<int32_t>);
%template(BipartiteGraphInt32) BipartiteGraph<int32_t>;
