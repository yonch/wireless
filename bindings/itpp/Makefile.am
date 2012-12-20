# bindings/itpp Makefile

## Flags and commands
# use python flags; add project-wide include directory; only shared libraries are required.
AM_CPPFLAGS += $(AX_SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include 
AM_CXXFLAGS += -shared
AM_LDFLAGS += $(PYTHON_EXTRA_LDFLAGS) -module
SWIG_CMD = $(SWIG) $(AX_SWIG_PYTHON_OPT) $(AM_CPPFLAGS) -I$(srcdir) -I$(srcdir)/.. -DHAVE_CONFIG_H

# Directories
itpp_execdir = ${pkgpyexecdir}/itpp
itpp_pythondir = ${pkgpythondir}/itpp

# Libraries made in this directory
itpp_exec_LTLIBRARIES = _base_sparse.la _base_vec.la _ldpc.la _llr.la _modulator.la
itpp_python_PYTHON = __init__.py

itpp_python_PYTHON += base_sparse.py
_base_sparse_la_SOURCES = base_sparse.i
_base_sparse_la_LIBADD = $(ITPP_LIBS)

itpp_python_PYTHON += base_vec.py
_base_vec_la_SOURCES = base_vec.i
_base_vec_la_LIBADD = $(ITPP_LIBS)

itpp_python_PYTHON += ldpc.py
_ldpc_la_SOURCES = ldpc.i
_ldpc_la_LIBADD = $(ITPP_LIBS)

itpp_python_PYTHON += llr.py
_llr_la_SOURCES = llr.i
_llr_la_LIBADD = $(ITPP_LIBS)

itpp_python_PYTHON += modulator.py
_modulator_la_SOURCES = modulator.i
_modulator_la_LIBADD = $(ITPP_LIBS)

.i.cpp:
	$(SWIG_CMD) -o $@ $<
