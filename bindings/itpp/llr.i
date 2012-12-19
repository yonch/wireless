%module(package="wireless.itpp") llr

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
#include <itpp/comm/llr.h>
using namespace itpp;
%}


%ignore operator<<(std::ostream &os, const LLR_calc_unit &l);
%include <itpp/comm/llr.h>


 