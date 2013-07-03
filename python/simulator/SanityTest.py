#!/usr/bin/python

# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

##
# \addtogroup simulator
# @{
from Simulator import Simulator
import numpy


def runTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 16,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runParallelKTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'parallel',
                    'alpha': 4,
                    'beta': 64,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               10)
    print str(res)


def runCoherenceFadingTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'soft',
                'bitsPerSymbol': 10},
        'channel': {'type': 'coherence-fading',
                    'interval': 10,
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 16,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runRateApproxProtocolTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 24},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 4},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 16,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'rate-approx',
                     'maxSymbols': 48*7,
                     'delta': 0.99},
        'statistics': {'type': 'errors'}
        }
    if 1==0:
        experimentSpec['protocol'] = {'type': 'sequential', 'maxPasses':48*8}
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               500)
    print str(res)


def runGaussianTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'trunc-norm-v2',
                'bitsPerSymbol': 10,
                'precisionBits': 14,
                'numStandardDevs': 0.1},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 16,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runLDPCTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 648*5/6},
            'code': {'type': 'ldpc',
                     'n': 648,
                     'rate': (5,6),
                     'bitsPerSymbol': 3},
            'map': {'type': 'gray',
                    'bitsPerSymbol': 3,
                    'precisionBits': 14},
            'channel': {'type': 'AWGN',
                        'SNR_dB': 17.8},
            'demap': {'type': 'ml-gray',
                      'SNR_dB': 17.8},
            'decoder': {'type': 'ldpc-float-bp',
                        'numIter': 40},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'one-try',
                         'numSymbols': 648/3},
            'statistics': {'type': 'errors'}
            }
    runner = Simulator(0)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)

def runItppQamLDPCTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 648*5/6},
            'code': {'type': 'ldpc',
                     'n': 648,
                     'rate': (5,6),
                     'bitsPerSymbol': 1},
            'map': {'type': 'QAM',
                    'bitsPerSymbol': 6},
            'channel': {'type': 'AWGN_c',
                        'SNR_dB': 17.8},
            'demap': {'type': 'QAM'},
            'decoder': {'type': 'ldpc-float-bp',
                        'numIter': 40},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'one-try',
                         'numSymbols': 648/6},
            'statistics': {'type': 'errors'}
            }
    runner = Simulator(0)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runItppQamTurboTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 1530},
            'code': {'type': 'turbo'},
            'map': {'type': 'QAM',
                    'bitsPerSymbol': 6},
            'channel': {'type': 'AWGN_c',
                        'SNR_dB': 3.2},
            'demap': {'type': 'QAM'},
            'decoder': {'type': 'regular'},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'one-try',
                         'numSymbols': (1530*5+18)/6},
            'statistics': {'type': 'errors'}
            }
    runner = Simulator(0)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)

def runLTTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 1000},
            'code': {'type': 'LT'},
            'map': {'type': 'QAM',
                    'bitsPerSymbol': 6},
            'channel': {'type': 'AWGN_c',
                        'SNR_dB': 17.8},
            'demap': {'type': 'QAM'},
            'decoder': {'type': 'LT',
                        'numIter': 6},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'rate-approx',
                         'maxSymbols': 1000,
                         'delta': 0.97,
                         'minSymbols':1000/6},
            'statistics': {'type': 'errors'}
            }
    runner = Simulator(0)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               10)
    print str(res)

def runRaptorTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 9500},
            'code': {'type': 'raptor'},
            'map': {'type': 'QAM',
                    'bitsPerSymbol': 6},
            'channel': {'type': 'AWGN_c',
                        'SNR_dB': 20},
            'demap': {'type': 'QAM',
                      'useApprox': True},
            'decoder': {'type': 'raptor',
                        'numIter': 40},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'rate-approx',
                         'maxSymbols': 11500,
                         'delta': 0.97,
                         'minSymbols':1280},
            'statistics': {'type': 'errors'}
            }
    runner = Simulator(2)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               3)
    print str(res)

def runBscLDPCTest():
    experimentSpec = {
           'packetGen': {'type': 'random',
                          'length': 648*2/3},
            'code': {'type': 'ldpc',
                     'n': 648,
                     'rate': (2,3),
                     'bitsPerSymbol': 1},
            'map': {'type': 'linear',
                    'bitsPerSymbol': 1,
                    'precisionBits': 1},
            'channel': {'type': 'BSC',
                        'flipProb': 0.04},
            'demap': {'type': 'BSC',
                      'flipProb': 0.04},
            'decoder': {'type': 'ldpc-float-bp',
                        'numIter': 40},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'one-try',
                         'numSymbols': 648},
            'statistics': {'type': 'errors'}
             }
    runner = Simulator(0)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runLookaheadTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'lookahead',
                   'beamWidth': 4,
                   'lookaheadDepth': 1,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)


def runStriderTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 1498*33},
        'code': {'type': 'strider'},
        'map': {'type': 'null',
                'avgPower': 1.0},
        'channel': {'type': 'AWGN_c',
                    'SNR_dB': 30},
        'demap': {'type': 'null'},
        'decoder': {'type': 'strider'},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'multiple-try',
                     'numSymbolsList': [(3840/4) * i for i in xrange(1,4*27+1)]},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               10)
    print str(res)

def runShortStriderTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 480*33},
        'code': {'type': 'strider',
                 'fragmentLength': 512},
        'map': {'type': 'null',
                'avgPower': 1.0},
        'channel': {'type': 'AWGN_c',
                    'SNR_dB': 30},
        'demap': {'type': 'null'},
        'decoder': {'type': 'strider'},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'multiple-try',
                     'numSymbolsList': [(1215/4) * i for i in xrange(1,4*27+1)]},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               10)
    print str(res)

def runStriderTransparentFadingTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 1498*33},
        'code': {'type': 'strider'},
        'map': {'type': 'null',
                'avgPower': 1.0},
        'channel': {'type': 'transparent-coherence_c',
                    'SNR_dB': 30,
                    'interval': 100},
        'demap': {'type': 'null'},
        'decoder': {'type': 'strider'},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'multiple-try',
                     'numSymbolsList': [(3840/4) * i for i in xrange(1,4*27+1)]},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               5)
    print str(res)

def runStriderFadingTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 1498*33},
        'code': {'type': 'strider'},
        'map': {'type': 'null',
                'avgPower': 1.0},
        'channel': {'type': 'coherence-fading_c',
                    'SNR_dB': 30,
                    'interval': 100},
        'demap': {'type': 'null'},
        'decoder': {'type': 'strider-fading'},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'multiple-try',
                     'numSymbolsList': [(3840/4) * i for i in xrange(1,4*27+1)]},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               10)
    print str(res)

def runNullDecoderTest():
    experimentSpec = {
            'packetGen': {'type': 'random',
                          'length': 8000},
            'code': {'type': 'null'},
            'map': {'type': 'QAM',
                    'bitsPerSymbol': 4},
            'channel': {'type': 'AWGN_c',
                        'SNR_dB': 17.8},
            'demap': {'type': 'QAM',
                      'useApprox': True},
            'decoder': {'type': 'null'},
            'detector': {'type': 'oracle'},
            'protocol': {'type': 'one-try',
                         'numSymbols': 8000/4},
            'statistics': {'type': 'errors'}
            }


def runCrcTest():
    experimentSpec = {
        'packetGen': {'type': 'crc16',
                      'length': 300},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 3},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': 10},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 16,
                   'maxPasses': 48*8},
        'detector': {'type': 'crc16'},
        'protocol': {'type': 'sequential',
                     'maxPasses': 48*8},
        'statistics': {'type': 'errors'}
        }
    runner = Simulator(1)
    res = runner.runExperiment(experimentSpec, 
                               numpy.random.RandomState().tomaxint(4), 
                               100)
    print str(res)

def runFirstErrorStatisticsTest():
    experimentSpec = {
        'packetGen': {'type': 'random',
                      'length': 256},
        'code': {'type': 'spinal',
                 'puncturing': {'type': '8-way-v2',
                                'numLastCodeStep': 2},
                 'hash': 'one-at-a-time',
                 'bitsPerSymbol': 10,
                 'k': 4},
        'map': {'type': 'linear',
                'bitsPerSymbol': 10,
                'precisionBits': 14},
        'channel': {'type': 'AWGN',
                    'SNR_dB': -2},
        'demap': {'type': 'null'},
        'decoder': {'type': 'regular',
                   'beamWidth': 100,
                   'maxPasses': 48*8},
        'detector': {'type': 'oracle'},
        'protocol': {'type': 'one-try',
                     'numSymbols': 256*2},
        'statistics': {'type': 'first-error'}
        }
    runner = Simulator(1)
    for i in xrange(10):
        res = runner.runExperiment(experimentSpec, 
                                   numpy.random.RandomState().tomaxint(4), 
                                   1)
        print str(res),
    print 

runTest()
runCrcTest()
runRateApproxProtocolTest()
runParallelKTest()
runGaussianTest()
runCoherenceFadingTest()
runLookaheadTest()

runLDPCTest()
runBscLDPCTest()
runItppQamLDPCTest()

runLTTest()
runRaptorTest()
runItppQamTurboTest()

runStriderTest()
runShortStriderTest()
runStriderFadingTest()
runStriderTransparentFadingTest()

runNullDecoderTest()

runFirstErrorStatisticsTest()


## @}