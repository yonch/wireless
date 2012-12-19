/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#pragma once

#include "MatrixLDPCCode.h"

MatrixLDPCCode getWifiLDPC648(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

MatrixLDPCCode getWifiLDPC1296(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

MatrixLDPCCode getWifiLDPC1944(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

