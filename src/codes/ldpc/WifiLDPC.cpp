/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "codes/ldpc/WifiLDPC.h"
#include <stdexcept>

#define CODE648_Z 27
#define CODE648_RATE_1_2_NUM_ROWS 12
#define CODE648_RATE_1_2_NUM_VALUES 63

const unsigned int code648_rate_1_2_rowPtrs[CODE648_RATE_1_2_NUM_ROWS] = { 0,
		5, 11, 16, 21, 26, 32, 36, 41, 47, 52, 58 };

const unsigned int code648_rate_1_2_cols[CODE648_RATE_1_2_NUM_VALUES] = { 0, 4,
		5, 8, 11, 0, 1, 4, 6, 7, 8, 0, 2, 4, 8, 10, 0, 3, 4, 8, 9, 0, 4, 8, 10,
		11, 0, 2, 3, 4, 6, 8, 0, 4, 8, 9, 0, 1, 4, 6, 8, 0, 1, 3, 4, 5, 8, 0,
		4, 8, 10, 11, 0, 2, 4, 5, 7, 8, 0, 4, 7, 8, 9 };

const unsigned char code648_rate_1_2_vals[CODE648_RATE_1_2_NUM_VALUES] = { 0,
		0, 0, 0, 0, 22, 0, 17, 0, 0, 12, 6, 0, 10, 24, 0, 2, 0, 20, 25, 0, 23,
		3, 0, 9, 11, 24, 23, 1, 17, 3, 10, 25, 8, 7, 18, 13, 24, 0, 8, 6, 7,
		20, 16, 22, 10, 23, 11, 19, 13, 3, 17, 25, 8, 23, 18, 14, 9, 3, 16, 2,
		25, 5 };

#define CODE648_RATE_2_3_NUM_ROWS 8
#define CODE648_RATE_2_3_NUM_VALUES 71
const unsigned int code648_rate_2_3_rowPtrs[CODE648_RATE_2_3_NUM_ROWS] = { 0,
		9, 18, 27, 36, 44, 53, 62 };
const unsigned int code648_rate_2_3_cols[CODE648_RATE_2_3_NUM_VALUES] = { 0, 1,
		2, 4, 6, 8, 11, 13, 15, 0, 1, 2, 3, 5, 7, 10, 12, 14, 0, 1, 2, 3, 4, 6,
		8, 9, 11, 0, 1, 2, 3, 5, 7, 10, 13, 15, 0, 1, 2, 4, 6, 8, 12, 14, 0, 1,
		2, 3, 5, 7, 9, 11, 13, 0, 1, 2, 3, 4, 6, 8, 10, 15, 0, 1, 2, 3, 5, 7,
		9, 12, 14 };
const unsigned char code648_rate_2_3_vals[CODE648_RATE_2_3_NUM_VALUES] = { 25,
		26, 14, 20, 2, 4, 8, 16, 18, 10, 9, 15, 11, 0, 1, 18, 8, 10, 16, 2, 20,
		26, 21, 6, 1, 26, 7, 10, 13, 5, 0, 3, 7, 26, 13, 16, 23, 14, 24, 12,
		19, 17, 20, 21, 6, 22, 9, 20, 25, 17, 8, 14, 18, 14, 23, 21, 11, 20,
		24, 18, 19, 22, 17, 11, 11, 20, 21, 26, 3, 18, 26 };

#define CODE648_RATE_3_4_NUM_ROWS 6
#define CODE648_RATE_3_4_NUM_VALUES 75
const unsigned int code648_rate_3_4_rowPtrs[CODE648_RATE_3_4_NUM_ROWS] = { 0,
		13, 26, 39, 50, 62 };
const unsigned int code648_rate_3_4_cols[CODE648_RATE_3_4_NUM_VALUES] = { 0, 1,
		2, 3, 4, 5, 6, 8, 9, 10, 12, 14, 16, 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 12,
		14, 17, 0, 1, 2, 3, 4, 5, 6, 8, 10, 12, 14, 15, 16, 0, 1, 2, 3, 4, 7,
		8, 10, 11, 13, 16, 0, 1, 2, 3, 4, 7, 8, 9, 11, 13, 15, 17, 0, 1, 2, 3,
		4, 5, 6, 7, 9, 11, 13, 15, 17 };
const unsigned char code648_rate_3_4_vals[CODE648_RATE_3_4_NUM_VALUES] = { 16,
		17, 22, 24, 9, 3, 14, 4, 2, 7, 26, 2, 21, 25, 12, 12, 3, 3, 26, 6, 21,
		15, 22, 15, 4, 16, 25, 18, 26, 16, 22, 23, 9, 0, 4, 4, 8, 23, 11, 9, 7,
		0, 1, 17, 7, 3, 3, 23, 16, 21, 24, 5, 26, 7, 1, 15, 24, 15, 8, 13, 13,
		11, 2, 2, 19, 14, 24, 1, 15, 19, 21, 2, 24, 3, 2 };

#define CODE648_RATE_5_6_NUM_ROWS 4
#define CODE648_RATE_5_6_NUM_VALUES 79
const unsigned int code648_rate_5_6_rowPtrs[CODE648_RATE_5_6_NUM_ROWS] = { 0,
		20, 40, 59 };
const unsigned int code648_rate_5_6_cols[CODE648_RATE_5_6_NUM_VALUES] = { 0, 1,
		2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1,
		2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1,
		2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 0, 1, 2, 3,
		4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
const unsigned char code648_rate_5_6_vals[CODE648_RATE_5_6_NUM_VALUES] = { 17,
		13, 8, 21, 9, 3, 18, 12, 10, 0, 4, 15, 19, 2, 5, 10, 26, 19, 13, 13, 3,
		12, 11, 14, 11, 25, 5, 18, 0, 9, 2, 26, 26, 10, 24, 7, 14, 20, 4, 2,
		22, 16, 4, 3, 10, 21, 12, 5, 21, 14, 19, 5, 8, 5, 18, 11, 5, 5, 15, 7,
		7, 14, 14, 4, 16, 16, 24, 24, 10, 1, 7, 15, 6, 10, 26, 8, 18, 21, 14 };

#define CODE1296_Z 54
#define CODE1296_RATE_1_2_NUM_ROWS 12
#define CODE1296_RATE_1_2_NUM_VALUES 61
const unsigned int code1296_rate_1_2_rowPtrs[CODE1296_RATE_1_2_NUM_ROWS] = { 0,
		5, 11, 16, 21, 26, 31, 35, 41, 46, 51, 56 };
const unsigned int code1296_rate_1_2_cols[CODE1296_RATE_1_2_NUM_VALUES] = { 0,
		4, 6, 7, 8, 0, 1, 4, 5, 8, 10, 0, 1, 4, 6, 11, 0, 3, 4, 7, 8, 0, 4, 5,
		8, 9, 0, 3, 4, 8, 10, 0, 1, 5, 8, 0, 2, 4, 6, 8, 9, 0, 3, 4, 8, 11, 0,
		2, 4, 8, 10, 1, 4, 7, 8, 9, 0, 2, 4, 8, 11 };
const unsigned char code1296_rate_1_2_vals[CODE1296_RATE_1_2_NUM_VALUES] = { 40,
		22, 49, 23, 43, 50, 1, 48, 35, 13, 30, 39, 50, 4, 2, 49, 33, 38, 37, 4,
		1, 45, 0, 22, 20, 42, 51, 48, 35, 44, 18, 47, 11, 17, 51, 5, 25, 6, 45,
		13, 40, 33, 34, 24, 23, 46, 1, 27, 1, 38, 44, 18, 23, 8, 0, 35, 49, 17,
		30, 34, 19 };

#define CODE1296_RATE_2_3_NUM_ROWS 8
#define CODE1296_RATE_2_3_NUM_VALUES 71
const unsigned int code1296_rate_2_3_rowPtrs[CODE1296_RATE_2_3_NUM_ROWS] = { 0,
		9, 18, 27, 36, 44, 53, 62 };
const unsigned int code1296_rate_2_3_cols[CODE1296_RATE_2_3_NUM_VALUES] = { 0,
		1, 2, 3, 5, 6, 8, 11, 15, 0, 1, 2, 3, 4, 6, 8, 12, 14, 0, 1, 2, 3, 4,
		6, 9, 12, 14, 0, 1, 2, 4, 5, 7, 10, 13, 14, 0, 1, 2, 3, 4, 5, 9, 12, 0,
		1, 2, 3, 4, 7, 10, 11, 13, 0, 1, 2, 3, 4, 7, 10, 13, 15, 0, 1, 2, 3, 4,
		8, 9, 11, 15 };
const unsigned char code1296_rate_2_3_vals[CODE1296_RATE_2_3_NUM_VALUES] = { 39,
		31, 22, 43, 40, 4, 11, 50, 6, 25, 52, 41, 2, 6, 14, 34, 24, 37, 43, 31,
		29, 0, 21, 28, 2, 7, 17, 20, 33, 48, 4, 13, 26, 22, 46, 42, 45, 7, 18,
		51, 12, 25, 50, 5, 35, 40, 32, 16, 5, 18, 43, 51, 32, 9, 24, 13, 22,
		28, 37, 25, 52, 13, 32, 22, 4, 21, 16, 27, 28, 38, 8 };

#define CODE1296_RATE_3_4_NUM_ROWS 6
#define CODE1296_RATE_3_4_NUM_VALUES 75
const unsigned int code1296_rate_3_4_rowPtrs[CODE1296_RATE_3_4_NUM_ROWS] = { 0,
		13, 25, 38, 50, 63 };
const unsigned int code1296_rate_3_4_cols[CODE1296_RATE_3_4_NUM_VALUES] = { 0,
		1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 17, 0, 1, 2, 3, 4, 5, 6, 8, 10, 12,
		14, 16, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 17, 0, 1, 2, 3, 4, 5, 6,
		8, 10, 12, 14, 16, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 17, 0, 1, 2,
		3, 4, 5, 6, 8, 10, 12, 14, 16 };
const unsigned char code1296_rate_3_4_vals[CODE1296_RATE_3_4_NUM_VALUES] = { 39,
		40, 51, 41, 3, 29, 8, 36, 14, 6, 33, 11, 4, 48, 21, 47, 9, 48, 35, 51,
		38, 28, 34, 50, 50, 30, 39, 28, 42, 50, 39, 5, 17, 6, 18, 20, 15, 40,
		29, 0, 1, 43, 36, 30, 47, 49, 47, 3, 35, 34, 1, 32, 11, 23, 10, 44, 12,
		7, 48, 4, 9, 17, 16, 13, 7, 15, 47, 23, 16, 47, 43, 29, 52, 2, 53 };

#define CODE1296_RATE_5_6_NUM_ROWS 4
#define CODE1296_RATE_5_6_NUM_VALUES 76
const unsigned int code1296_rate_5_6_rowPtrs[CODE1296_RATE_5_6_NUM_ROWS] = { 0,
		19, 38, 57 };
const unsigned int code1296_rate_5_6_cols[CODE1296_RATE_5_6_NUM_VALUES] = { 0,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 19, 0, 1, 2,
		3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 1, 2, 3, 4,
		5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 17, 18, 19, 0, 1, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19 };
const unsigned char code1296_rate_5_6_vals[CODE1296_RATE_5_6_NUM_VALUES] = { 48,
		29, 37, 52, 2, 16, 6, 14, 53, 31, 34, 5, 18, 42, 53, 31, 45, 46, 52,
		17, 4, 30, 7, 43, 11, 24, 6, 14, 21, 6, 39, 17, 40, 47, 7, 15, 41, 19,
		7, 2, 51, 31, 46, 23, 16, 11, 53, 40, 10, 7, 46, 53, 33, 35, 25, 35,
		38, 19, 48, 41, 1, 10, 7, 36, 47, 5, 29, 52, 52, 31, 10, 26, 6, 3, 2,
		51 };

#define CODE1944_Z 81
#define CODE1944_RATE_1_2_NUM_ROWS 12
#define CODE1944_RATE_1_2_NUM_VALUES 61
const unsigned int code1944_rate_1_2_rowPtrs[CODE1944_RATE_1_2_NUM_ROWS] = { 0,
		5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
const unsigned int code1944_rate_1_2_cols[CODE1944_RATE_1_2_NUM_VALUES] = { 0,
		4, 6, 8, 10, 0, 2, 4, 8, 9, 0, 4, 5, 8, 9, 0, 1, 4, 7, 8, 0, 3, 4, 7,
		8, 0, 4, 6, 8, 11, 0, 1, 2, 6, 8, 0, 4, 5, 8, 10, 0, 4, 5, 8, 11, 1, 3,
		4, 8, 9, 0, 1, 3, 4, 10, 0, 2, 4, 7, 8, 11 };
const unsigned char code1944_rate_1_2_vals[CODE1944_RATE_1_2_NUM_VALUES] = {
		57, 50, 11, 50, 79, 3, 28, 0, 55, 7, 30, 24, 37, 56, 14, 62, 53, 53, 3,
		35, 40, 20, 66, 22, 28, 0, 8, 42, 50, 8, 69, 79, 79, 56, 52, 65, 38,
		57, 72, 27, 64, 14, 52, 30, 32, 45, 70, 0, 77, 9, 2, 56, 57, 35, 12,
		24, 61, 60, 27, 51, 16 };

#define CODE1944_RATE_2_3_NUM_ROWS 8
#define CODE1944_RATE_2_3_NUM_VALUES 71
const unsigned int code1944_rate_2_3_rowPtrs[CODE1944_RATE_2_3_NUM_ROWS] = { 0,
		9, 18, 27, 36, 44, 53, 62 };
const unsigned int code1944_rate_2_3_cols[CODE1944_RATE_2_3_NUM_VALUES] = { 0,
		1, 2, 3, 4, 11, 13, 14, 15, 0, 1, 2, 3, 7, 8, 9, 10, 12, 0, 1, 2, 3, 4,
		5, 6, 10, 14, 0, 1, 2, 3, 4, 9, 10, 12, 13, 0, 1, 2, 3, 5, 6, 8, 11, 0,
		1, 2, 3, 4, 6, 12, 13, 14, 0, 1, 2, 3, 4, 5, 7, 11, 15, 0, 1, 2, 3, 4,
		7, 8, 9, 15 };
const unsigned char code1944_rate_2_3_vals[CODE1944_RATE_2_3_NUM_VALUES] = {
		61, 75, 4, 63, 56, 8, 2, 17, 25, 56, 74, 77, 20, 64, 24, 4, 67, 7, 28,
		21, 68, 10, 7, 14, 65, 23, 75, 48, 38, 43, 78, 76, 5, 36, 15, 72, 40,
		2, 53, 25, 52, 62, 20, 44, 69, 23, 64, 10, 22, 21, 68, 23, 29, 12, 0,
		68, 20, 55, 61, 40, 52, 44, 58, 8, 34, 64, 78, 11, 78, 24, 58 };

#define CODE1944_RATE_3_4_NUM_ROWS 6
#define CODE1944_RATE_3_4_NUM_VALUES 72
const unsigned int code1944_rate_3_4_rowPtrs[CODE1944_RATE_3_4_NUM_ROWS] = { 0,
		12, 24, 36, 48, 60 };
const unsigned int code1944_rate_3_4_cols[CODE1944_RATE_3_4_NUM_VALUES] = { 0,
		1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17, 0, 1, 2, 3, 4, 5, 9, 10, 11, 12,
		13, 15, 0, 1, 2, 3, 4, 5, 6, 8, 9, 13, 14, 17, 0, 1, 2, 3, 4, 5, 6, 7,
		8, 12, 16, 17, 0, 1, 2, 3, 4, 5, 7, 8, 10, 12, 14, 16, 0, 1, 2, 3, 4,
		5, 6, 7, 11, 13, 14, 15 };
const unsigned char code1944_rate_3_4_vals[CODE1944_RATE_3_4_NUM_VALUES] = {
		48, 29, 28, 39, 9, 61, 63, 45, 80, 37, 32, 22, 4, 49, 42, 48, 11, 30,
		49, 17, 41, 37, 15, 54, 35, 76, 78, 51, 37, 35, 21, 17, 64, 59, 7, 32,
		9, 65, 44, 9, 54, 56, 73, 34, 42, 35, 46, 39, 3, 62, 7, 80, 68, 26, 80,
		55, 36, 26, 9, 72, 26, 75, 33, 21, 69, 59, 3, 38, 35, 62, 36, 26 };

#define CODE1944_RATE_5_6_NUM_ROWS 4
#define CODE1944_RATE_5_6_NUM_VALUES 70
const unsigned int code1944_rate_5_6_rowPtrs[CODE1944_RATE_5_6_NUM_ROWS] = { 0,
		18, 36, 53 };
const unsigned int code1944_rate_5_6_cols[CODE1944_RATE_5_6_NUM_VALUES] = { 0,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 0, 1, 2, 3,
		4, 5, 6, 7, 8, 9, 10, 12, 14, 15, 16, 17, 18, 19, 0, 1, 2, 3, 4, 5, 6,
		7, 8, 9, 10, 11, 12, 13, 15, 17, 19, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11,
		12, 13, 14, 16, 18, 19 };
const unsigned char code1944_rate_5_6_vals[CODE1944_RATE_5_6_NUM_VALUES] = {
		13, 48, 80, 66, 4, 74, 7, 30, 76, 52, 37, 60, 49, 73, 31, 74, 73, 23,
		69, 63, 74, 56, 64, 77, 57, 65, 6, 16, 51, 64, 68, 9, 48, 62, 54, 27,
		51, 15, 0, 80, 24, 25, 42, 54, 44, 71, 71, 9, 67, 35, 58, 29, 53, 16,
		29, 36, 41, 44, 56, 59, 37, 50, 24, 65, 4, 65, 52, 4, 73, 52 };



MatrixLDPCCode getWifiLDPC648(	unsigned int rateNumerator,
								unsigned int rateDenominator)
{
	if((rateNumerator == 1) && (rateDenominator == 2)) {
		// rate 1/2
		return MatrixLDPCCode(
				648,
				CODE648_Z,
				rateNumerator,
				rateDenominator,
				code648_rate_1_2_rowPtrs,
				CODE648_RATE_1_2_NUM_VALUES,
				code648_rate_1_2_vals,
				code648_rate_1_2_cols);
	} else if((rateNumerator == 2) && (rateDenominator == 3)) {
		// rate 2/3
		return MatrixLDPCCode(
				648,
				CODE648_Z,
				rateNumerator,
				rateDenominator,
				code648_rate_2_3_rowPtrs,
				CODE648_RATE_2_3_NUM_VALUES,
				code648_rate_2_3_vals,
				code648_rate_2_3_cols);
	} else if((rateNumerator == 3) && (rateDenominator == 4)) {
		// rate 3/4
		return MatrixLDPCCode(
				648,
				CODE648_Z,
				rateNumerator,
				rateDenominator,
				code648_rate_3_4_rowPtrs,
				CODE648_RATE_3_4_NUM_VALUES,
				code648_rate_3_4_vals,
				code648_rate_3_4_cols);
	} else if((rateNumerator == 5) && (rateDenominator == 6)) {
		// rate 5/6
		return MatrixLDPCCode(
				648,
				CODE648_Z,
				rateNumerator,
				rateDenominator,
				code648_rate_5_6_rowPtrs,
				CODE648_RATE_5_6_NUM_VALUES,
				code648_rate_5_6_vals,
				code648_rate_5_6_cols);
	} else {
		throw(std::runtime_error("Unknown code rate"));
	}
}


MatrixLDPCCode getWifiLDPC1296(	unsigned int rateNumerator,
								unsigned int rateDenominator)
{
	if((rateNumerator == 1) && (rateDenominator == 2)) {
		// rate 1/2
		return MatrixLDPCCode(
				1296,
				CODE1296_Z,
				rateNumerator,
				rateDenominator,
				code1296_rate_1_2_rowPtrs,
				CODE1296_RATE_1_2_NUM_VALUES,
				code1296_rate_1_2_vals,
				code1296_rate_1_2_cols);
	} else if((rateNumerator == 2) && (rateDenominator == 3)) {
		// rate 2/3
		return MatrixLDPCCode(
				1296,
				CODE1296_Z,
				rateNumerator,
				rateDenominator,
				code1296_rate_2_3_rowPtrs,
				CODE1296_RATE_2_3_NUM_VALUES,
				code1296_rate_2_3_vals,
				code1296_rate_2_3_cols);
	} else if((rateNumerator == 3) && (rateDenominator == 4)) {
		// rate 3/4
		return MatrixLDPCCode(
				1296,
				CODE1296_Z,
				rateNumerator,
				rateDenominator,
				code1296_rate_3_4_rowPtrs,
				CODE1296_RATE_3_4_NUM_VALUES,
				code1296_rate_3_4_vals,
				code1296_rate_3_4_cols);
	} else if((rateNumerator == 5) && (rateDenominator == 6)) {
		// rate 5/6
		return MatrixLDPCCode(
				1296,
				CODE1296_Z,
				rateNumerator,
				rateDenominator,
				code1296_rate_5_6_rowPtrs,
				CODE1296_RATE_5_6_NUM_VALUES,
				code1296_rate_5_6_vals,
				code1296_rate_5_6_cols);
	} else {
		throw(std::runtime_error("Unknown code rate"));
	}
}


MatrixLDPCCode getWifiLDPC1944(	unsigned int rateNumerator,
								unsigned int rateDenominator)
{
	if((rateNumerator == 1) && (rateDenominator == 2)) {
		// rate 1/2
		return MatrixLDPCCode(
				1944,
				CODE1944_Z,
				rateNumerator,
				rateDenominator,
				code1944_rate_1_2_rowPtrs,
				CODE1944_RATE_1_2_NUM_VALUES,
				code1944_rate_1_2_vals,
				code1944_rate_1_2_cols);
	} else if((rateNumerator == 2) && (rateDenominator == 3)) {
		// rate 2/3
		return MatrixLDPCCode(
				1944,
				CODE1944_Z,
				rateNumerator,
				rateDenominator,
				code1944_rate_2_3_rowPtrs,
				CODE1944_RATE_2_3_NUM_VALUES,
				code1944_rate_2_3_vals,
				code1944_rate_2_3_cols);
	} else if((rateNumerator == 3) && (rateDenominator == 4)) {
		// rate 3/4
		return MatrixLDPCCode(
				1944,
				CODE1944_Z,
				rateNumerator,
				rateDenominator,
				code1944_rate_3_4_rowPtrs,
				CODE1944_RATE_3_4_NUM_VALUES,
				code1944_rate_3_4_vals,
				code1944_rate_3_4_cols);
	} else if((rateNumerator == 5) && (rateDenominator == 6)) {
		// rate 5/6
		return MatrixLDPCCode(
				1944,
				CODE1944_Z,
				rateNumerator,
				rateDenominator,
				code1944_rate_5_6_rowPtrs,
				CODE1944_RATE_5_6_NUM_VALUES,
				code1944_rate_5_6_vals,
				code1944_rate_5_6_cols);
	} else {
		throw(std::runtime_error("Unknown code rate"));
	}
}
