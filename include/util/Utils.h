/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once


#include <string>
#include <vector>
#include <stdexcept>
#include <stdint.h>

/**
 * This should only contain static methods
 **/
class Utils
{
public:
	static void stringToVector(	const std::string& strData,
								std::vector<bool>& vecData);

	static void vectorToString(	const std::vector<bool>& vecData,
								std::string& strData);

	static std::string stringToHex(const std::string& s);

	/**
	 * Provides hard decision estimates of bits, given a list of Log Likelihood
	 *    Ratios.
	 * @param soft: the soft LLR values
	 * @param hard: [out] the hard estimates
	 */
	static void softToHardEstimates(const std::vector<float>& soft,
	                                	std::vector<bool>& hard);

	/**
	 * Extracts several consecutive bits from a string into a different string.
	 *
	 * @param src: the source string
	 * @param dst: the destination string
	 * @param posBits: the position, in bits, of the first bit to extract
	 * @param lengthBits: the number of bits to extract
	 */
	static void getStringBits(const std::string& src,
							  std::string& dst,
							  unsigned int posBits,
							  unsigned int lengthBits);

	/**
	 * Like getStringBits, but returns the string.
	 */
	static std::string returnStringBits(
			const std::string& src,
			unsigned int posBits,
			unsigned int lengthBits);

	template<typename BlockType>
	static void blockify(const std::string& strData,
						 uint32_t dataNumBits,
						 uint32_t blockNumBits,
						 std::vector<BlockType>& blocks);


	template<typename BlockType>
	static void unblockify(const std::vector<BlockType>& blocks,
							unsigned int blockNumBits, std::string& strData);

	static unsigned int getHammingDistance(	const std::string& a,
											const std::string& b);

	static uint16_t getCRC16(const std::string& a);

	static uint16_t getArrCRC16(const unsigned char*a, unsigned int len);

	static bool passesCRC16(const std::string& a);

	static uint32_t getCRC32(const std::string& a);

	static uint32_t getArrCRC32(const unsigned char*a, unsigned int len);

	static bool passesCRC32(const std::string& a);

	static unsigned int popcount_2(unsigned int x);

	/**
	 * Copy data from source to destination strings, with control over which
	 * 	  bits are copied
	 * @param dst: the string where data will be written
	 * @param dst_offset: the bit offset of bit copy
	 * @param src: source string
	 * @param src_offset: the offset in bits into src where to take bits
	 * @param num_bits: the number of bits to copy
	 */
	static void copyBits(std::string& dst, unsigned int dst_offset,
	                     const std::string& src, unsigned int src_offset,
	                     unsigned int num_bits);

	/**
	 * Returns dst with proper bits copied from src as return value
	 */
	static std::string returnCopiedBits(
						const std::string& dst, unsigned int dst_offset,
						const std::string& src, unsigned int src_offset,
						unsigned int num_bits);

	/**
	 * bitwise XORs the two bit strings, returning the result.
	 */
	static std::string XorStrings(const std::string& a, const std::string& b);

};

template<typename BlockType>
inline void Utils::blockify(
		const std::string & strData,
		uint32_t dataNumBits,
		uint32_t blockNumBits,
		std::vector<BlockType> & blocks)
{
	if((dataNumBits % blockNumBits) != 0) {
		throw(std::runtime_error("Data must contain an integral number of blocks"));
	}

	uint32_t requiredBytes = (dataNumBits + 7) / 8;
	if(strData.size() < requiredBytes) {
		throw(std::runtime_error("input string contains less bits than were"
								 "specified in dataNumBits argument"));
	}

	blocks.clear();
	blocks.reserve(dataNumBits / blockNumBits);

	// for each block
	BlockType data = 0;
	BlockType blockMask = (1 << blockNumBits) - 1;
	uint32_t curNumBits = 0;

	for(unsigned int i = 0; i < requiredBytes; i++) {
		data |= BlockType(uint8_t(strData[i])) << curNumBits;
		curNumBits += 8;

		while(curNumBits >= blockNumBits) {
			// get a byte out of 'data'
			blocks.push_back(data & blockMask);

			// update data
			data = data >> blockNumBits;
			curNumBits -= blockNumBits;
		}
	}
}



template<typename BlockType>
void Utils::unblockify(const std::vector<BlockType>& blocks,
						unsigned int blockNumBits, std::string& strData) {
	unsigned int sizeBytes = (((unsigned int)blocks.size() * blockNumBits) + 7) / 8;
	strData.resize( sizeBytes , 0 );

	// for each block
	BlockType data = 0;
	int curNumBits = 0;
	int curExtractedBytes = 0;
	for(unsigned int i = 0; i < blocks.size(); i++) {
		data |= (blocks[i] << curNumBits);
		curNumBits += blockNumBits;

		while(curNumBits >= 8) {
			// get a byte out of 'data'
			strData[curExtractedBytes] = data & 0xff;
			curExtractedBytes++;

			// update data
			data = data >> 8;
			curNumBits -= 8;
		}
	}

	// If we have a remainder of bits, store into string
	if (curNumBits > 0) {
		strData[curExtractedBytes] = data & 0xff;
	}
}



