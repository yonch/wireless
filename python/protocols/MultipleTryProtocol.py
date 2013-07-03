# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import numpy

##
# \ingroup protocols
# \brief Attempts to decode at a set of times that is given beforehand 
class MultipleTryProtocol(object):
    '''
    A protocol that attempts to decode at a set of times that is given 
        beforehand
    '''

    def __init__(self, numPackets, numSymbolsList):
        '''
        Constructor
        @param numPackets: the number of packets the protocol handles in 
            parallel
        @param numSymbolsList: a list with the sequence of numSymbols where
            decoding attempts should be performed
        '''
        
        self.packetIndex = numpy.zeros([numPackets], dtype=numpy.uint32)
        self.numSymbolsList = numSymbolsList + [0]
        
    def numSymbolsNextDecode(self, packetInd):
        # get num symbols
        res = self.numSymbolsList[self.packetIndex[packetInd]]
        
        # update next index to be returned
        self.packetIndex[packetInd] += 1
        
        return res
        
    def setResult(self, packetInd, numSymbols, isFinished):
        # if isFinished == True, set packet to terminate.
        if isFinished:
            self.packetIndex[packetInd] = len(self.numSymbolsList) - 1
    
    def resetPacket(self, packetInd):
        
        self.packetIndex[packetInd] = 0
 