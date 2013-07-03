#!/usr/bin/python

# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import math
from pprint import pprint
import numpy
import copy

import wireless
from FactoryCollection import FactoryCollection  

##
# \ingroup simulator
# \brief Simulates a complete packet transmission (packet generator, encoder, mapper, channel, etc..) 
class Simulator(object):
    def __init__(self, verbosity):
        
        self.factories = FactoryCollection(['make_pre_packet_manipulator',
                                            'make_packet_generator',
                                            'make_encoder',
                                            'make_mapper',
                                            'make_channel',
                                            'make_demapper',
                                            'make_decoder',
                                            'make_detector',
                                            'make_protocol',
                                            'make_statistics'])
        
        try:
            from alt_configuration import get_factory_list
        except:
            from default_configuration import get_factory_list
            
        for fobj in get_factory_list():
            self.factories.add_factory(fobj)
        
        self.verbosity = verbosity
        
        # Random generator
        self.random = numpy.random.mtrand.RandomState()


    def getComponents(self, experiment):
        """
        Makes the components needed for the experiment specified by 'experiment'
        """
        try:
            self.prePacketManipulator = self.factories.make('make_pre_packet_manipulator', experiment)
        except:
            self.prePacketManipulator = None
            
        self.packetGen = self.factories.make('make_packet_generator', experiment['packetGen'])
        self.encoder, self.encoderVectorType = self.factories.make('make_encoder',
                                           experiment['code'], 
                                           experiment['packetGen']['length'])
        self.mapper, self.mapperVectorType, self.Es = \
                        self.factories.make('make_mapper', experiment['map'])
        self.channel, self.channelVectorType, self.n0 = \
                        self.factories.make('make_channel', experiment['channel'],
                                                 self.Es)
        self.demap, self.demapType = \
                self.factories.make('make_demapper', experiment['map'], experiment['demap'])
        self.decoder = self.factories.make('make_decoder',
                                           experiment['code'], 
                                           experiment['packetGen']['length'],
                                           experiment['decoder'],
                                           experiment['map'],
                                           experiment['channel'])
        self.detector = self.factories.make('make_detector',
                                            experiment['detector'],
                                            experiment['code'],
                                            experiment['packetGen']['length'],
                                            self.Es)
        self.protocol = self.factories.make('make_protocol',
                                            experiment['protocol'],
                                            experiment['code'],
                                            experiment['packetGen']['length'])
        
        self.statistics = self.factories.make('make_statistics', experiment['statistics'])
        self.statistics.reset()

        # buffers between components
        self.encodeBuffer = self.encoderVectorType();
        
        if self.mapper is not None:
            self.mapperBuffer = self.mapperVectorType();
        else:
            self.mapperBuffer = self.encodeBuffer
            
        self.channelBuffer = self.channelVectorType();
        
        if self.demap is not None:
            self.demapperBuffer = self.demapType();

        
    def seed(self, seed):
        """
        Seeds all components with the given seed
        """
        self.random.seed(seed)
        self.packetGen.seed(self.random.tomaxint(4).astype(numpy.uint32))
        self.channel.seed(self.random.tomaxint(4).astype(numpy.uint32))
        
        if self.prePacketManipulator is not None:
            if 'seed' in dir(self.prePacketManipulator):
                self.prePacketManipulator.seed(self.random.randint(1,1<<62))
        
    def runPacket(self):
        """
        Runs a single packet through the simulator components
        """
        verbosity = self.verbosity

        # if the pipeline has a pre-packet function, run it now to allow it
        # to modify components before the packet
        if self.prePacketManipulator is not None:
            self.prePacketManipulator.process(self)

        # Generate packet, and set it to other components
        packet = self.packetGen.get()
        self.encoder.setPacket(packet)
        self.detector.setPacket(0, packet)
        self.statistics.setPacket(0, packet)
        if verbosity >= 3:
            print "packet = %s" % packet.encode("hex")
        
        # Go through protocol, encoding and decoding as necessary
        lastNumChannelSymbols = 0
        nextNumChannelSymbols = self.protocol.numSymbolsNextDecode(0)
            
        while nextNumChannelSymbols > 0:
            if verbosity >= 3:
                print "NumSymbols ", nextNumChannelSymbols
            # figure out how many encoder symbols are necessary to get
            # nextNumChannelSymbols channel symbols
            minNeededChannelOutputs = nextNumChannelSymbols - lastNumChannelSymbols
            if self.mapper is not None:
                minNeededMapperOutputs = self.channel.forecast(minNeededChannelOutputs)
                minNeededEncoderOutputs = self.mapper.forecast(minNeededMapperOutputs)
            else:
                minNeededEncoderOutputs = self.channel.forecast(minNeededChannelOutputs)
                
            # Run symbols through the simulation chain
            self.encoder.encode(minNeededEncoderOutputs, self.encodeBuffer)
            if self.mapper is not None:
                self.mapper.process(self.encodeBuffer, self.mapperBuffer)
            self.channel.process(self.mapperBuffer, self.channelBuffer)
            if self.demap is not None:
                # the decoder works with LLRs, demap and then decode
                self.demap.process(self.channelBuffer, self.n0, self.demapperBuffer)
                self.decoder.add(self.demapperBuffer)
            else:
                # the decoder works on symbols directly
                self.decoder.add(self.channelBuffer, self.n0)

            if verbosity >= 3:
                print "EncoderOutputs:", list(self.encodeBuffer)
                print "ChannelOutputs:", list(self.channelBuffer)
                if self.demap is not None:
                    print "Demapped:", list(self.demapperBuffer)
            
            # update how many channel symbols were processed so far.
            # note that the actual number of processed symbols can be larger
            # than nextNumChannelSymbols because components can choose to
            # generate more symbols if necessary, which is why we update
            # using the actual number of symbols we processed.
            lastNumChannelSymbols += self.channelBuffer.size()
            
            # decode
            decodeResult = self.decoder.decode()
            isFinished = self.detector.isFinished(0, decodeResult)
            
            self.protocol.setResult(0, 
                               lastNumChannelSymbols,
                               isFinished)
            # Should we continue? If so, how many symbols?
            nextNumChannelSymbols = self.protocol.numSymbolsNextDecode(0)

            self.statistics.update(0,
                              lastNumChannelSymbols,
                              decodeResult,
                              isFinished or (nextNumChannelSymbols == 0))
            
            if verbosity >= 3:
                print "decodeResult: ", decodeResult.packet.encode('hex')
            if verbosity >= 2:
                print "isFinished: ", isFinished, " nextDecode at ", nextNumChannelSymbols
        
        # Clean up
        self.protocol.resetPacket(0)
        self.decoder.reset()
        self.statistics.done(0)
        lastNumChannelSymbols = 0
        
        
    def runExperiment(self, experiment, initialSeed, numPackets):
        # Get components
        self.getComponents(experiment)
                
        # initialize random
        self.seed(initialSeed)
        
        for packetInd in xrange(numPackets):
            self.runPacket()
            
        return self.statistics.getSummary()
