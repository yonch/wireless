# Initialize
AC_INIT([wireless], [1.0], [yonch@yonch.com])

# Require a minimum version of autoconf
AC_PREREQ([2.68])

# Sanity check
AC_CONFIG_SRCDIR([include/codes/spinal/IHashDecoder.h])

# Directories for output of build files
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_AUX_DIR([build-aux])

# Should build the lablog subpackage
AC_CONFIG_SUBDIRS([lablog])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# bootstrap C and C++ compilers 
AC_PROG_CC
AC_PROG_CXX
# Default compiler is C++
AC_LANG([C++])


# Link time optimization feature in newer gcc/g++
#    based on http://svn.r-project.org/R/trunk/configure.ac
AC_ARG_ENABLE([lto],
	[AS_HELP_STRING([--enable-lto],[enable link-time optimization @<:@no@:>@])],
	[if test "x${enableval}" = xyes -o "x${enableval}" = x; then
  		want_lto=yes
	 elif test "x${enableval}" = xno; then
  		want_lto=no
	 else
	    AC_MSG_ERROR([Invalid response to --enable-lto (got ${enableval})])
	 fi], 
	[want_lto=no])
if test "x${want_lto}" = xyes; then
  AX_CHECK_COMPILE_FLAG([-flto], 
  						[], 
  						[AC_MSG_ERROR([Compiler doesn't support -flto, requested by link-time optimization (--enable-lto)])]) 
  LTO=-flto
fi
AC_SUBST(LTO)
AM_CONDITIONAL(BUILD_LTO, [test "x${want_lto}" != xno])

# Create defaults for AM_CPPFLAGS, AM_CXXFLAGS, AM_LDFLAGS
AM_CPPFLAGS=${CPPFLAGS}
AM_CXXFLAGS=${CXXFLAGS} ${LTO}
AM_LDFLAGS=${LDFLAGS} ${LTO}
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS]) 
#, [$(LTO)])

# Libtool, for compiling libraries
LT_INIT

# Get Python paths and targets
AM_PATH_PYTHON

# SWIG configuration
AX_PKG_SWIG(2.0.7, [], [ AC_MSG_ERROR([SWIG 2.0.7 or higher is required to build..]) ])
AX_SWIG_ENABLE_CXX
# deprecated in SWIG 2.0.7: AX_SWIG_MULTI_MODULE_SUPPORT
AX_SWIG_PYTHON

# Output config header
AC_CONFIG_HEADERS([config.h])

# Files to be generated
AC_CONFIG_FILES([Makefile src/Makefile \
	bindings/Makefile bindings/itpp/Makefile bindings/codes/Makefile 
	bindings/codes/spinal/Makefile bindings/util/Makefile \
	python/Makefile data/Makefile include/Makefile])

# IT++, using pkg-config as recommended by IT++ documentation
PKG_CHECK_MODULES([ITPP], [itpp], [HAVE_LIBITPP=1])
AC_SUBST([ITPP_LIBS])
AC_SUBST([ITPP_CFLAGS])
AC_CHECK_HEADERS([itpp/base/binary.h])
AC_DEFINE([WITH_ITPP],[], [Use IT++ library])

# Protocol buffers compiler, for simulator serialization
AC_CHECK_PROG(PROTOC_CHECK,protoc,yes)
if test x"$PROTOC_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Could not find protoc in the path, please install it or add to the path.])
fi

# Do output
AC_OUTPUT
