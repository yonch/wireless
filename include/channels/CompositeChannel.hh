/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#define COMPOSITE_CHANNEL_SUBSEED_SIZE 4

template<typename Channel1, typename Channel2>
inline CompositeChannel<Channel1,Channel2>::CompositeChannel(
		const Channel1 & channel1,
		const Channel2 & channel2,
		uint32_t numOutputSymbolsToReserve)
: m_channel1(channel1),
  m_channel2(channel2)
{
	m_intermediateBuffer.reserve(
			m_channel2.forecast(numOutputSymbolsToReserve));
}

template<typename Channel1, typename Channel2>
inline void CompositeChannel<Channel1,Channel2>::seed(
		unsigned int * const seed,
		int seedSize)
{
	m_random.seed(seed, seedSize);

	// Buffer for seeding of internal channels
	uint32_t subSeed[COMPOSITE_CHANNEL_SUBSEED_SIZE];

	// Seed channel1
	for(uint32_t i = 0; i < COMPOSITE_CHANNEL_SUBSEED_SIZE; i++) {
		subSeed[i] = m_random.randInt();
	}
	m_channel1.seed(subSeed, COMPOSITE_CHANNEL_SUBSEED_SIZE);

	// Seed channel2
	for(uint32_t i = 0; i < COMPOSITE_CHANNEL_SUBSEED_SIZE; i++) {
		subSeed[i] = m_random.randInt();
	}
	m_channel2.seed(subSeed, COMPOSITE_CHANNEL_SUBSEED_SIZE);
}



template<typename Channel1, typename Channel2>
inline void CompositeChannel<Channel1,Channel2>::process(
		const std::vector<InputType> & inSymbols,
		std::vector<OutputType> & outSymbols)
{
	m_channel1.process(inSymbols, m_intermediateBuffer);
	m_channel2.process(m_intermediateBuffer, outSymbols);
}



template<typename Channel1, typename Channel2>
inline unsigned int CompositeChannel<Channel1,Channel2>::forecast(
		unsigned int numOutputs)
{
	return m_channel1.forecast(m_channel2.forecast(numOutputs));
}

