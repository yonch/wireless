# bindings/codes Makefile

## Flags and commands
# use python flags; add project-wide include directory; only shared libraries are required.
AM_CPPFLAGS += $(AX_SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include 
AM_CXXFLAGS += -shared
AM_LDFLAGS += $(PYTHON_EXTRA_LDFLAGS) -module
SWIG_CMD = $(SWIG) $(AX_SWIG_PYTHON_OPT) $(AX_SWIG_PYTHON_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir) -I$(srcdir)/.. -DHAVE_CONFIG_H

# Libraries made in this directory
codes_execdir = ${pkgpyexecdir}/codes
codes_pythondir = ${pkgpythondir}/codes

codes_exec_LTLIBRARIES = _spinal.la _codes.la _fountain.la _strider.la _turbo.la _ldpc.la \
	_null.la

codes_python_PYTHON = __init__.py

codes_python_PYTHON += spinal.py
_spinal_la_SOURCES = spinal.i
_spinal_la_LIBADD = -L../../src -lspinal

codes_python_PYTHON += codes.py
_codes_la_SOURCES = codes.i
_codes_la_LIBADD = -L../../src -lwireless

codes_python_PYTHON += fountain.py
_fountain_la_SOURCES = fountain.i
_fountain_la_LIBADD = -L../../src -lwireless

codes_python_PYTHON += strider.py
_strider_la_SOURCES = strider.i
_strider_la_LIBADD = -L../../src -lwireless

codes_python_PYTHON += turbo.py
_turbo_la_SOURCES = turbo.i
_turbo_la_LIBADD = -L../../src -lwireless

codes_python_PYTHON += ldpc.py
_ldpc_la_SOURCES = ldpc.i
_ldpc_la_LIBADD = -L../../src -lwireless

codes_python_PYTHON += null.py
_null_la_SOURCES = null.i
_null_la_LIBADD = -L../../src -lwireless

.i.cpp:
	$(SWIG_CMD) -o $@ $<
