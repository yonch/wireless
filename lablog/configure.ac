# lablog configure.ac

# Initialize
AC_INIT([lablog], [1.0], [yonch@yonch.com])

# Require a minimum version of autoconf
AC_PREREQ([2.68])

# Sanity check
AC_CONFIG_SRCDIR([python/__init__.py])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Get Python paths and targets
AM_PATH_PYTHON

# Files to be generated
AC_CONFIG_FILES([Makefile])

# Do output
AC_OUTPUT
