/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <string>

/**
 * \ingroup detectors
 * \brief Determines whether packet was decoded successfully by comparing to ground truth
 */
template<typename DecodeResult>
class OracleDetector {
public:
	OracleDetector(unsigned int numPackets);

	/**
	 * @param packetInd the index of the packet
	 * @param packet the packet data
	 */
	void setPacket(unsigned int packetInd, const std::string& packet);

	/**
	 * @return True if the decoded result is equal to the set packet
	 */
	bool isFinished(unsigned int packetInd, const DecodeResult& result);
private:
	// The packets
	std::vector<std::string> m_packets;
};

template<typename DecodeResult>
inline OracleDetector<DecodeResult>::OracleDetector(unsigned int numPackets)
	: m_packets(numPackets)
{}

template<typename DecodeResult>
inline void OracleDetector<DecodeResult>::setPacket(
		unsigned int packetInd,
		const std::string & packet)
{
	m_packets[packetInd] = packet;
}



template<typename DecodeResult>
inline bool OracleDetector<DecodeResult>::isFinished(
		unsigned int packetInd,
		const DecodeResult & result)
{
	return result.packet == m_packets[packetInd];
}


