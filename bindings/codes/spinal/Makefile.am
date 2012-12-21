# bindings/codes/spinal Makefile

## Flags and commands
# use python flags; add project-wide include directory; only shared libraries are required.
AM_CPPFLAGS += $(AX_SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include 
AM_CXXFLAGS += -shared
AM_LDFLAGS += $(PYTHON_EXTRA_LDFLAGS) -module
SWIG_CMD = $(SWIG) $(AX_SWIG_PYTHON_OPT) $(AX_SWIG_PYTHON_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir) -I$(srcdir)/../.. -DHAVE_CONFIG_H

# Libraries made in this directory
spinal_execdir = ${pkgpyexecdir}/codes/spinal
spinal_pythondir = ${pkgpythondir}/codes/spinal

spinal_exec_LTLIBRARIES = _spinal.la

spinal_python_PYTHON = __init__.py

spinal_python_PYTHON += spinal.py
_spinal_la_SOURCES = spinal.i
_spinal_la_LIBADD = -L../../../src -lspinal

.i.cpp:
	$(SWIG_CMD) -o $@ $<
