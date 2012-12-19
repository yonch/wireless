/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <stdint.h>

// See crc.c file for more information

uint16_t crc16r(const uint8_t *data, int len, uint16_t crc);
uint32_t crc32r(const uint8_t *data, int len, uint32_t crc);
