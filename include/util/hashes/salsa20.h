/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "ecrypt-portable.h"

typedef unsigned int SalsaState[16];
typedef unsigned long long SalsaDigest;


void salsaInit(const SalsaDigest digest, SalsaState& state);

void salsaUpdate(SalsaState& state, u32 data);

SalsaDigest salsaDigest(const SalsaState& state);

int salsaGetSymbol(const SalsaState& hashState,
					unsigned int symbolSizeBits,
					unsigned int pointIndex);
