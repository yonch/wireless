# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import numpy
import wireless

from wireless.util.serialization.results_pb2 import ErrorRateResults

##
# \ingroup statistics
# \brief Collected statistics on number of errors 
class ErrorRateStatisticsResult(object):
    """
    A result object for the FinishStatistics.
    Contains a dictionary:   numSymbols --> (numPackets, numCorrectPackets)
    An entry numSymbols --> (numPackets, numCorrectPackets) means that the 
        protocol decided to terminate numPackets packets after numSymbols 
        symbols, and that after checking the decoded packet against the sent
        packet, numCorrectPackets matched (ie were decoded correctly)
    """
    def __init__(self):
        self.stats = {}
        
    def addPacket(self, numSymbols, numBitErrors):
        """
        Adds a packet to the statistics. The decode terminated after numSymbols
        symbols, and was correct if isCorrect == True
        """
        # get current count
        numFinished, totalPacketErrors, totalBitErrors = self.stats.get(numSymbols, (0,0,0))
        
        # update counts

        numFinished += 1
        totalBitErrors += numBitErrors
        if (numBitErrors > 0):
            totalPacketErrors += 1
        
        # write result back
        self.stats[numSymbols] = (numFinished, totalPacketErrors, totalBitErrors)
    
    def __str__(self):
        return str(sorted(self.stats.items()))
    
    def dumps(self):
        res = ErrorRateResults()
        for numSymbols, (numFinished, totalPacketErrors, totalBitErrors) in self.stats.items():
            point = res.points.add()
            point.num_symbols = int(numSymbols)
            point.num_finished = int(numFinished)
            point.num_packet_errors = int(totalPacketErrors)
            point.num_bit_errors = int(totalBitErrors)
            
        return res.SerializeToString()
    
    def loads(self, s):
        res = ErrorRateResults.FromString(s)
        for point in res.points:
            self.stats[point.num_symbols] = (point.num_finished, 
                                              point.num_packet_errors,
                                              point.num_bit_errors)
        return



##
# \ingroup statistics
# \brief Statistics collector of number of errors 
class ErrorRateStatistics(object):
    """Collects statistics on when decoders finished, and amount of error on
        finish
        
        This was extended from the FinishStatistics class.
    """
    
    def __init__(self, numPackets):
        self.numPackets = numPackets
        
        self.reset()
        
    def reset(self):
        # keeps the correct packets and whether statistics were collected for them 
        self.mayUpdate = numpy.zeros([self.numPackets], dtype=numpy.bool)
        self.packets = dict()
        
        # keep statistics
        self.statistics = ErrorRateStatisticsResult()
    
    def setPacket(self, packetInd, packet):
        if packetInd in self.packets:
            raise RuntimeError, "packet %d already in statistics instance"
        self.packets[packetInd] = packet
        self.mayUpdate[packetInd] = True
    
    def update(self, packetInd, numSymbols, decodeResult, isFinished):
        # sanity check
        if numSymbols == 0:
            raise RuntimeError, "numSymbols in update() should never be 0"
        
        # if this is the first time when finished() == True, update statistics
        if isFinished:
            if self.mayUpdate[packetInd]:
                numBitErrors = wireless.Utils.getHammingDistance(decodeResult.packet, self.packets[packetInd])
                
                self.statistics.addPacket(numSymbols, numBitErrors)
                
                self.mayUpdate[packetInd] = False
            else:
                raise RuntimeError, "returned finished twice! (this is not strictly an error but curious)"
        
    def done(self, packetInd):
        del self.packets[packetInd]
    
    def getSummary(self):
        return self.statistics