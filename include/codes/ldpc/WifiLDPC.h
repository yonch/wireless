/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#pragma once

#include "MatrixLDPCCode.h"

/**
 * \ingroup ldpc
 * \brief factory for 802.11n LDPC codes with n=648
 */
MatrixLDPCCode getWifiLDPC648(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

/**
 * \ingroup ldpc
 * \brief factory for 802.11n LDPC codes with n=1296
 */
MatrixLDPCCode getWifiLDPC1296(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

/**
 * \ingroup ldpc
 * \brief factory for 802.11n LDPC codes with n=1944
 */
MatrixLDPCCode getWifiLDPC1944(unsigned int rateNumerator,
				 			  unsigned int rateDenominator);

