/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
%module(package="wireless.util") config

%include "std_string.i"

%{
#include "config.h" 

std::string get_data_dir() {
	return DATA_PATH;
}
%}

%include "config.h"

std::string get_data_dir();