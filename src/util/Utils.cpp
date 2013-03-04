/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#include "util/Utils.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdexcept>
#include <stdint.h>
#include <strings.h> // for ffs()
#include "util/crc.h"

void Utils::stringToVector(	const std::string& strData,
							std::vector<bool>& vecData)
{
	vecData.resize(8 * strData.size());
	// convert the string to a vector<bool>
	for(unsigned int i = 0; i < strData.size(); i++) {
		unsigned char byte = static_cast<unsigned char>(strData[i]);
		for(int b = 0; b < 8; b++) {
			vecData[8*i+b] = (byte >> b) & 0x1;
		}
	}
}

void Utils::vectorToString(	const std::vector<bool>& vecData,
							std::string& strData)
{
	unsigned int sizeBytes = ((unsigned int)vecData.size() + 7) / 8;
	unsigned int fullBytes = (unsigned int)vecData.size() / 8;
	strData.resize( sizeBytes , 0 );

	// all but last byte
	for(unsigned int i = 0; i < fullBytes; i++) {
		unsigned char ch = 0;
		for (int b = 0; b < 8; b++) {
			ch |= ( ((unsigned char)vecData[(8*i)+b]) << b);
		}
		strData[i] = ch;
	}

	// last byte
	int numRemaining = vecData.size() % 8;
	unsigned char ch = 0;
	for (int b = 0; b < numRemaining; b++) {
		ch |= ( ((unsigned char)vecData[(8*fullBytes)+b]) << b);
	}
	strData[fullBytes] = ch;
}

void Utils::getStringBits(
		const std::string & src,
		std::string & dst,
		unsigned int posBits,
		unsigned int lengthBits)
{
	// Sanity check: src should contain enough bits
	if(src.size() < ((posBits + lengthBits + 7) / 8)) {
		throw(std::runtime_error("Source string doesn't contain enough bits"));
	}

	// Clear before resizing, so there won't be a copy
	dst.clear();
	// Resize destination
	dst.resize((lengthBits + 7) / 8);

	unsigned int spos = posBits / 8;
	unsigned int rshift = posBits % 8;
	unsigned int dpos = 0;

	// Copy full destination bytes
	while(lengthBits >= 8) {
		dst[dpos++] = (  ((unsigned char)src[spos] >> rshift)
					   | ((unsigned char)src[spos+1] << (8-rshift)));
		spos++;
		lengthBits -= 8;
	}

	// Copy partially full byte, if exists.
	if (lengthBits > 0) {
		// Get bits that haven't been used from byte pointed by spos
		unsigned char ch = ((unsigned char)src[spos] >> rshift);
		// If still more bits are necessary, get them from next byte
		if(lengthBits > 8 - rshift) {
			ch |= ((unsigned char)src[spos+1] << (8-rshift));
		}
		// Mask out only wanted bits
		ch &= ((1 << lengthBits) - 1);
		// Set destination byte
		dst[dpos] = ch;
	}
}

std::string Utils::returnStringBits(
									const std::string & src,
									unsigned int posBits,
									unsigned int lengthBits)
{
	std::string dst;
	getStringBits(src,dst,posBits,lengthBits);
	return dst;
}

std::string Utils::stringToHex(const std::string & s)
{
	std::string res(s.size() * 2, 0);
	char buf[3];
	for(unsigned int i = 0; i < s.size(); i++) {
		snprintf(buf, 3, "%02x", (unsigned char)s[i]);
		res[2*i] = buf[0];
		res[2*i + 1] = buf[1];
	}
	return res;
}

void Utils::softToHardEstimates(const std::vector<float> & soft,
                                	std::vector<bool> & hard)
{
	hard.clear();
	hard.reserve(soft.size());

	for(uint32_t i = 0; i < soft.size(); i++) {
		hard.push_back(soft[i] < 0);
	}
}

unsigned int Utils::getHammingDistance(	const std::string& a,
										const std::string& b)
{
	const char* ap = a.data();
	const char* bp = b.data();
	unsigned int remaining = a.size();
	unsigned int dist = 0;

	if (a.size() != b.size()) {
		throw(std::runtime_error("Compared strings are not of equal size!"));
	}

	while(remaining >= 4) {
		dist += popcount_2((*((const unsigned int*)ap)) ^ (*((const unsigned int*)bp)));
		ap += 4;
		bp += 4;
		remaining -= 4;
	}

	unsigned int xa = 0;
	unsigned int xb = 0;
	for (unsigned int i = 0; i < remaining; i++) {
		xa = (xa << 8) | (ap[i]);
		xb = (xb << 8) | (bp[i]);
	}
	dist += popcount_2(xa^xb);

	return dist;
}

uint16_t Utils::getCRC16(const std::string & a) {
	return getArrCRC16((const uint8_t*)a.data(), a.size());
}

uint16_t Utils::getArrCRC16(const unsigned char *a, unsigned int len) {
	return crc16r(a, len, 0);
}

bool Utils::passesCRC16(const std::string & a) {
	uint16_t computedCRC =
			Utils::getArrCRC16((const uint8_t*)a.data() + 2, a.size() - 2);

	uint16_t packetCRC = *(const uint16_t*)a.data();

	return (packetCRC == computedCRC);
}

uint32_t Utils::getCRC32(const std::string& a)
{
	return getArrCRC32((const uint8_t*)a.data(), a.size());
}

uint32_t Utils::getArrCRC32(const unsigned char *a, unsigned int len)
{
	return crc32r(a, len, 0);
}

bool Utils::passesCRC32(const std::string & a)
{
	uint32_t packetCRC = *(const uint32_t*)a.data();

	return (packetCRC == getArrCRC32((const uint8_t*)a.data() + 4,
									 a.size() - 4));
}

/* Classic binary divide-and-conquer popcount.
   This is popcount_2() from
   http://en.wikipedia.org/wiki/Hamming_weight */
/* 15 ops, 3 long immediates, 14 stages */
unsigned int
Utils::popcount_2(unsigned int x)
{
    unsigned int m1 = 0x55555555;
    unsigned int m2 = 0x33333333;
    unsigned int m4 = 0x0f0f0f0f;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    return (x + (x >> 16)) & 0x3f;
}

void Utils::copyBits(	std::string & dst,
						unsigned int dst_offset,
						const std::string & src,
						unsigned int src_offset,
						unsigned int num_bits)
{
	unsigned int dst_pos = dst_offset / 8;
	unsigned int dst_bit = dst_offset % 8;
	unsigned int src_pos = src_offset / 8;
	unsigned int src_bit = src_offset % 8;

	// Get the bits that should not be overwritten from dst[dst_pos]
	uint8_t dst_mask = (1 << dst_bit) - 1;
	uint16_t data = ((uint8_t)dst[dst_pos]) & dst_mask;
	// We keep invariant num_bits = data_size + (#bits we still need to extract)
	unsigned int data_size = dst_bit;
	num_bits += dst_bit;
	// Now we don't need to think about bit offsets into dst (dst_bit = 0)

	// Add the bits from the src[src_pos]
	data |= ((uint16_t)(uint8_t)src[src_pos] >> src_bit) << dst_bit;
	data_size += (8 - src_bit);
	src_pos++;
	// Now we don't need to think about bit offsets into src (src_bit = 0)

	// Make sure data_size is < 8 by writing a byte if needed
	// (note that data size <= 15 before call)
	if ((num_bits >= 8) && (data_size >= 8)) {
		dst[dst_pos] = (int8_t)(data & 0xFF);
		data >>= 8;
		num_bits -= 8;
		data_size -= 8;
		dst_pos++;
	}

	while(num_bits >= 8) {
		data |= ((uint16_t)(uint8_t)src[src_pos++]) << data_size;
		dst[dst_pos++] = (int8_t)(data & 0xFF);
		data >>= 8;
		num_bits-= 8;
	}

	if(num_bits > 0) {
		if (num_bits > data_size) {
			data |= ((uint16_t)(uint8_t)src[src_pos]) << data_size;
		}
		// We want to put the bits into the destination, but careful not to
		// overwrite bits that shouldn't be touched
		dst_mask = (1 << num_bits) - 1;
		dst[dst_pos] = (int8_t)(((uint8_t)dst[dst_pos] & (~dst_mask)) | (data & dst_mask));
	}
}

std::string Utils::returnCopiedBits(const std::string & dst,
									unsigned int dst_offset,
									const std::string & src,
									unsigned int src_offset,
									unsigned int num_bits)
{
	std::string res(dst);
	copyBits(res, dst_offset, src, src_offset, num_bits);
	return res;
}



std::string Utils::XorStrings(const std::string& a, const std::string& b) {
	if (a.size() != b.size()) {
		throw std::runtime_error("String lengths do not match");
	}

	const uint32_t num_bytes = a.size();

	// Allocate result string
	std::string res(num_bytes, 0);

	// Perform XOR
	for (uint32_t i = 0; i < num_bytes; i++) {
		res[i] = a[i] ^ b[i];
	}

	return res;
}


int Utils::find_first_difference(const std::string& a, const std::string& b) {
	if (a.size() != b.size()) {
		throw std::runtime_error("String lengths do not match");
	}

	const uint32_t num_bytes = a.size();
	int result = 0;
	for(uint32_t i = 0; i < num_bytes; i++, result+=8) {
		if(a[i] != b[i]) {
			// Found a differing byte, find differing bit
			uint8_t c = uint8_t(a[i]) ^ uint8_t(b[i]);
			return result + ffs(c) - 1;
		}
	}

	// string match, result should now contain the index after the last bit.
	return result;
}
