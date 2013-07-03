# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import numpy
import wireless

##
# \ingroup statistics
# \brief Collected statistics on location of errors 
class ErrorLocationStatisticsResult(object):
    """
    A result object for the ErrorLocationStatistics.
    Records the error pattern of a single message. Where errors occurred, the corresponding bit is
      on.
    """
    def __init__(self):
        self.error_locations = None
        
    def __str__(self):
        if self.error_locations is None:
            return None
        else:
            return self.error_locations.encode('hex')
    
    def dumps(self):
        if self.error_locations is None:
            raise RuntimeError, "error_locations has not been set, cannot dump"
            
        return self.error_locations
    
    def loads(self, s):
        self.error_locations = s
        return


##
# \ingroup statistics
# \brief Statistic collector of location of errors 
class ErrorLocationStatistics(object):
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
        self.statistics = ErrorLocationStatisticsResult()
    
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
                self.statistics.error_locations = \
                    wireless.Utils.XorStrings(self.packet, decodeResult.packet)
                
                self.mayUpdate = False
            else:
                raise RuntimeError, "returned finished twice! (this is not strictly an error but curious)"
        
    def done(self, packetInd):
        self.packet = None
    
    def getSummary(self):
        return self.statistics
