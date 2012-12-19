# bindings/util Makefile

## Flags and commands
# use python flags; add project-wide include directory; only shared libraries are required.
AM_CPPFLAGS += $(AX_SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include
AM_CXXFLAGS += -shared -DDATA_PATH='"$(datadir)"'
AM_LDFLAGS += $(PYTHON_EXTRA_LDFLAGS) -module
SWIG_CMD = $(SWIG) $(AX_SWIG_PYTHON_OPT) $(AM_CPPFLAGS) -I$(srcdir) -I$(srcdir)/.. -I${top_builddir} -DHAVE_CONFIG_H

# Directories
util_execdir = ${pkgpyexecdir}/util
util_pythondir = ${pkgpythondir}/util

# Libraries made in this directory
util_exec_LTLIBRARIES = _hashes.la _inference.la _config.la
util_python_PYTHON = __init__.py

util_python_PYTHON += hashes.py
_hashes_la_SOURCES = hashes.i
_hashes_la_LIBADD = -L../../src -lwireless

util_python_PYTHON += inference.py
_inference_la_SOURCES = inference.i
_inference_la_LIBADD = -L../../src -lwireless

util_python_PYTHON += config.py
_config_la_SOURCES = config.i
_config_la_LIBADD = -L../../src -lwireless

.i.cpp:
	$(SWIG_CMD) -o $@ $<
