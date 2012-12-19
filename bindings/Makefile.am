# bindings Makefile

SUBDIRS = itpp util codes

# SWIG support inspired by following resources:
#  http://realmike.org/blog/2010/07/18/building-swig-python-extensions-on-gnulinux/
#  http://www.gnu.org/software/autoconf-archive/ax_pkg_swig.html
#  http://www.swig.org/Doc2.0/Introduction.html#Introduction_build_system

## Flags and commands
# use python flags; add project-wide include directory; only shared libraries are required.
AM_CPPFLAGS += $(AX_SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/include 
AM_CXXFLAGS += -shared
AM_LDFLAGS += $(PYTHON_EXTRA_LDFLAGS) -module
SWIG_CMD = $(SWIG) $(AX_SWIG_PYTHON_OPT) $(AM_CPPFLAGS) -I$(srcdir) -DHAVE_CONFIG_H

# Top level files
pkgpython_PYTHON = __init__.py

# Libraries made in this directory
pkgpyexec_LTLIBRARIES = _general.la _channels.la _mappers.la _demappers.la _misc.la 

pkgpython_PYTHON += general.py
_general_la_SOURCES = general.i
_general_la_LIBADD = -L../src -lwireless

pkgpython_PYTHON += channels.py
_channels_la_SOURCES = channels.i
_channels_la_LIBADD = -L../src -lwireless

pkgpython_PYTHON += mappers.py
_mappers_la_SOURCES = mappers.i
_mappers_la_LIBADD = -L../src -lwireless

pkgpython_PYTHON += demappers.py
_demappers_la_SOURCES = demappers.i
_demappers_la_LIBADD = -L../src -lwireless

pkgpython_PYTHON += misc.py
_misc_la_SOURCES = misc.i
_misc_la_LIBADD = -L../src -lwireless

# The protocols library is installed into the protocols directory where python classes from the
# python directory complement it, see python/protocols
protocols_execdir = ${pkgpyexecdir}/protocols
protocols_pythondir = ${pkgpythondir}/protocols
protocols_exec_LTLIBRARIES = _protocols.la
protocols_python_PYTHON = protocols.py
_protocols_la_SOURCES = protocols.i
_protocols_la_LIBADD = -L../src -lwireless

.i.cpp: general.i common.i
	$(SWIG_CMD) -o $@ $<
