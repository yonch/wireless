# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import numpy
import wireless
import struct

##
# \ingroup statistics
# \brief Collected statistics on bit-index of first bit error 
class FirstErrorStatisticsResult(object):
    """
    A result object for the FirstErrorStatistics.
    Records the error pattern of a single message. Where errors occurred, the corresponding bit is
      on.
    """
    def __init__(self):
        self.first_error_location = None
        
    def __str__(self):
        if self.first_error_location is None:
            return None
        else:
            return str(self.first_error_location)
    
    def dumps(self):
        if self.first_error_location is None:
            raise RuntimeError, "first_error_location has not been set, cannot dump"
            
        return struct.pack('<i', self.first_error_location)
    
    def loads(self, s):
        self.first_error_location = struct.unpack('<i', s)[0]
        return


##
# \ingroup statistics
# \brief Statistics collection of bit-index of first bit error 
class FirstErrorStatistics(object):
    """Collects statistics on when decoders finished, and amount of error on
        finish
        
        This was extended from the FinishStatistics class.
    """
    
    def __init__(self, numPackets):
        if numPackets != 1:
            raise RuntimeError, "numPackets should be = 1"

        self.reset()
        
    def reset(self):
        # keeps the correct packets and whether statistics were collected for them 
        self.mayUpdate = False
        self.packet = None
        
        # keep statistics
        self.statistics = FirstErrorStatisticsResult()
    
    def setPacket(self, packetInd, packet):
        assert(packetInd == 0)
        if self.packet is not None:
            raise RuntimeError, "packet %d already in statistics instance"
        
        self.packet = packet
        self.mayUpdate = True
    
    def update(self, packetInd, numSymbols, decodeResult, isFinished):
        # sanity check
        if numSymbols == 0:
            raise RuntimeError, "numSymbols in update() should never be 0"
        
        # if this is the first time when finished() == True, update statistics
        if isFinished:
            if self.mayUpdate:
                self.statistics.first_error_location = \
                    wireless.Utils.find_first_difference(self.packet, decodeResult.packet)
                
                self.mayUpdate = False
            else:
                raise RuntimeError, "returned finished twice! (this is not strictly an error but curious)"
        
    def done(self, packetInd):
        self.packet = None
    
    def getSummary(self):
        return self.statistics
