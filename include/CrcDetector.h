/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <string>
#include "util/Utils.h"

/**
 * \ingroup detectors
 * \brief Detects whether the packet's CRC checks out
 */
template<typename DecodeResult>
class CrcDetector {
public:
	CrcDetector(unsigned int numPackets);

	/**
	 * @param packetInd the index of the packet
	 * @param packet the packet data
	 */
	void setPacket(unsigned int packetInd, const std::string& packet);

	/**
	 * @return True if the decoded result is equal to the set packet
	 */
	bool isFinished(unsigned int packetInd, const DecodeResult& result);
};

template<typename DecodeResult>
inline CrcDetector<DecodeResult>::CrcDetector(unsigned int numPackets)
{}

template<typename DecodeResult>
inline void CrcDetector<DecodeResult>::setPacket(
		unsigned int packetInd,
		const std::string & packet)
{
	// nothing to be done; this class only relies on received CRC, thus doesn't need the real packet
	// to decide whether transmission is complete.
}

template<typename DecodeResult>
inline bool CrcDetector<DecodeResult>::isFinished(
		unsigned int packetInd,
		const DecodeResult & result)
{
	uint16_t crc = Utils::getArrCRC16((const uint8_t*)result.packet.data() + 2,
									  result.packet.size() - 2);
	return (crc == *(uint16_t*)result.packet.data());
}
