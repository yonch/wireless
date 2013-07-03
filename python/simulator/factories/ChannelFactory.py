# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless
import math

##
# \ingroup factories
# \brief Channel simulators factory 
class ChannelFactory(object):
    def make_channel(self, spec, signalAveragePower):
        if spec['type'] == 'AWGN' or spec['type'] == 'AWGN-1D':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            if spec['type'] == 'AWGN':
                noiseAveragePower = (2.0 * signalAveragePower) / SNR_ratio
            else: # spec['type'] == 'AWGN-1D':
                noiseAveragePower = signalAveragePower / SNR_ratio
            return wireless.channels.SymbolAwgnChannel(noiseAveragePower), wireless.vector_symbol, noiseAveragePower / 2.0
        elif spec['type'] == 'AWGN-soft':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = (2.0 * signalAveragePower) / SNR_ratio
            return wireless.channels.SoftAwgnChannel(noiseAveragePower), wireless.vector_softsymbol, noiseAveragePower / 2.0
        elif spec['type'] == 'AWGN_c':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = (signalAveragePower) / SNR_ratio
            return wireless.channels.ComplexAwgnChannel(noiseAveragePower), wireless.vector_csymbol, noiseAveragePower
        elif spec['type'] == 'BSC':
            flipProb = spec['flipProb']
            return wireless.channels.BscChannel(flipProb), wireless.vector_symbol, 0
        elif spec['type'] == 'MIMO-AWGN':
            nTransmitter = spec['nTransmitter']
            nReceiver = spec['nReceiver']
            channelMatrix = wireless.vector_csymbol(spec['channelMatrix'])
            
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = signalAveragePower / SNR_ratio
            
            mimo = wireless.channels.MimoChannel(nTransmitter,
                                                    nReceiver,
                                                    channelMatrix)
            awgn = wireless.channels.ComplexAwgnChannel(noiseAveragePower)
            
            channel = wireless.channels.MimoAwgnChannel(mimo, awgn, 100)
            
            return channel, wireless.vector_csymbol, noiseAveragePower
        elif spec['type'] == 'coherence-fading': # this is over soft symbols
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = (2.0 * signalAveragePower) / SNR_ratio

            coherence = wireless.channels.SoftCoherenceFading(spec['interval'])
            awgn = wireless.channels.FadingAwgnChannel(noiseAveragePower)
            channel = wireless.channels.AwgnCoherenceFadingChannel(coherence, awgn, 100)
            
            return channel, wireless.vector_fadingsymbol, noiseAveragePower / 2.0
        elif spec['type'] == 'coherence-fading_c': 
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = signalAveragePower / SNR_ratio

            coherence = wireless.channels.ComplexCoherenceFading(spec['interval'])
            awgn = wireless.channels.FadingComplexAwgnChannel(noiseAveragePower)
            channel = wireless.channels.AwgnCoherenceComplexFadingChannel(coherence, awgn, 100)
            
            return channel, wireless.vector_fading_csymbol, noiseAveragePower
        elif spec['type'] == 'transparent-coherence_c':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = signalAveragePower / SNR_ratio

            channel = wireless.channels.ComplexTransparentFadingChannel(spec['interval'], noiseAveragePower)
            
            return channel, wireless.vector_csymbol, noiseAveragePower
        elif spec['type'] == 'transparent-coherence':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = (2.0 * signalAveragePower) / SNR_ratio

            channel = wireless.channels.SoftTransparentFadingChannel(spec['interval'], noiseAveragePower)
            
            return channel, wireless.vector_softsymbol, noiseAveragePower / 2.0
        elif spec['type'] == 'transparent-coherence-symbol':
            SNR_dB = spec['SNR_dB']
            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
            noiseAveragePower = (2.0 * signalAveragePower) / SNR_ratio

            channel = wireless.channels.SymbolTransparentFadingChannel(spec['interval'], noiseAveragePower)
            
            return channel, wireless.vector_symbol, noiseAveragePower / 2.0

        else:
            return None
