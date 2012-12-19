%module(package="wireless.itpp") ldpc

///////////////////////
// Exception handling
///////////////////////
%include exception.i

%exception {
	try {
		$action
	}
	SWIG_CATCH_STDEXCEPT
	catch (...) {
     SWIG_exception(SWIG_UnknownError, "Unknown exception");
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


//////////////////
// General code-related
//////////////////
%{
#include <itpp/comm/channel_code.h>
#include <itpp/comm/ldpc.h>
using namespace itpp;
%}

%import "base_vec.i"
%import "base_sparse.i"

%include <itpp/comm/channel_code.h>


%ignore operator<<(std::ostream &os, const LDPC_Code &C);

%include <itpp/comm/ldpc.h>
 