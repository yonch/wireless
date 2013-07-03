/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <vector>
#include <tr1/memory>

#include <itpp/comm/modulator.h>

#include "../CodeBench.h"
#include "IDemapper.h"


/**
 * \ingroup demappers
 * \brief An adaptor between IT++ Modulator and our IDemapper interface
 */
template<typename ChannelSymbol>
class ItppDemapper : public IDemapper<ChannelSymbol> {
public:
	typedef std::tr1::shared_ptr<itpp::Modulator_2D > ModulatorPtr;

	/**
	 * C'tor
	 * @param modulator: the IT++ modulator to use
	 * @param forecastFactor: the number of LLRs produced by one symbol
	 * @param useApprox: True if the APPROX soft demapping method should be used
	 */
	ItppDemapper(ModulatorPtr& modulator,
	             unsigned int forecastFactor,
	             bool useApprox);

	/**
	 * virtual d'tor
	 */
	virtual ~ItppDemapper() {}

	/**
	 * Demaps symbols to soft bit values
	 * @param symbols: the symbols to be mapped
	 * @param n0: channel noise power
	 * @param llrs: [out] the soft values
	 */
	virtual void process(const std::vector<ChannelSymbol>& symbols,
						 N0_t n0,
						 std::vector<LLRValue>& llrs);

	/**
	 * Demaps symbols to soft bit values, given different noise for each symbol
	 * @param symbols: the symbols to be mapped
	 * @param fadingMagnitude: the scaling factor that the symbols underwent
	 * 		when transmitted on the channel
	 * @param n0: channel noise power
	 * @param llrs: [out] the soft values
	 */
	virtual void process(const std::vector<ChannelSymbol>& symbols,
						 const std::vector<FadingMagnitude>& fadingMagnitude,
						 N0_t n0,
						 std::vector<LLRValue>& llrs);

	/**
	 * @return the number of input items needed to generate numOutputs output
	 *     items
	 */
	virtual unsigned int forecast(unsigned int numOutputs);

private:
	// IT++ modulator
	ModulatorPtr m_modulator;

	// The forecast factor
	const unsigned int m_forecastFactor;

	// True if the soft demapping should use the APPROX Soft_Method
	itpp::Soft_Method m_softMethod;
};


#include "ItppDemapper.hh"
