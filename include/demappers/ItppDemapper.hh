/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */

#include "ItppDemapper.h"

#include <math.h>
#include <assert.h>
#include <itpp/base/vec.h>

template<typename ChannelSymbol>
inline ItppDemapper<ChannelSymbol>::ItppDemapper(	ModulatorPtr& modulator,
													unsigned int forecastFactor,
													bool useApprox)
  : m_modulator(modulator),
    m_forecastFactor(forecastFactor)
{
	if(useApprox == true) {
		m_softMethod = itpp::APPROX;
	} else {
		m_softMethod = itpp::LOGMAP;
	}
}

template<typename ChannelSymbol>
inline void ItppDemapper<ChannelSymbol>::process(	const std::vector<
															ChannelSymbol> & symbols,
													N0_t n0,
													std::vector<LLRValue> & llrs)
{
	itpp::Vec<ChannelSymbol> itppSymbols(symbols.size());
	for(unsigned int i = 0; i < symbols.size(); i++) {
		itppSymbols[i] = symbols[i];
	}

	itpp::vec itppLlrs;
	m_modulator->demodulate_soft_bits(itppSymbols, n0, itppLlrs, m_softMethod);

	llrs.clear();
	llrs.resize(itppLlrs.size());
	for(unsigned int i = 0; i < llrs.size(); i++) {
		llrs[i] = itppLlrs[i];
	}
}

template<typename ChannelSymbol>
inline void ItppDemapper<ChannelSymbol>::process(
		const std::vector<ChannelSymbol> & symbols,
		const std::vector<FadingMagnitude>& fadingMagnitude,
		N0_t n0,
		std::vector<LLRValue> & llrs)
{
	if(symbols.size() != fadingMagnitude.size()) {
		throw(std::runtime_error("must have same amount of symbols and of fading information"));
	}

	itpp::Vec<ChannelSymbol> itppSymbols(symbols.data(), symbols.size());

	itpp::Vec<ChannelSymbol> itppChannels(symbols.size());
	for(unsigned int i = 0; i < symbols.size(); i++) {
		itppChannels[i] = fadingMagnitude[i];
	}

	itpp::vec itppLlrs;
	m_modulator->demodulate_soft_bits(itppSymbols, itppChannels, n0, itppLlrs, m_softMethod);

	llrs.clear();
	llrs.resize(itppLlrs.size());
	for(unsigned int i = 0; i < llrs.size(); i++) {
		assert(isfinite(itppLlrs[i]));
		llrs[i] = itppLlrs[i];
	}

}

template<typename ChannelSymbol>
inline unsigned int ItppDemapper<ChannelSymbol>::forecast(unsigned int numOutputs)
{
	return (numOutputs + m_forecastFactor - 1) / m_forecastFactor;
}

