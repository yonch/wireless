# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import numpy

class FastFourwayProtocol(object):
    def __init__(self, numPackets, maxPasses, puncturingSchedule):
        self.puncturingSchedule = puncturingSchedule
        
        self.numPackets = numPackets
        
        self.nextDecode = \
                    numpy.ones([numPackets], dtype = numpy.uint32) * 4
        if maxPasses < 4:
            raise RuntimeError, "this protocol requires at least 4 passes"
        self.maxPasses = maxPasses
        
        
        
    def numSymbolsNextDecode(self, packetInd):
        return self.puncturingSchedule.numSymbolsAfterPasses(int(self.nextDecode[packetInd]))
    
    def setResult(self, packetInd, numSymbols, isFinished):
        if numSymbols != self.puncturingSchedule.numSymbolsAfterPasses(int(self.nextDecode[packetInd])):
            raise RuntimeError, "unexpected result from packet %d"%packetInd
        
        if isFinished or (self.nextDecode[packetInd] == self.maxPasses):
            self.nextDecode[packetInd] = 0
        else:
            if self.nextDecode[packetInd] < 20:
                self.nextDecode[packetInd] += 1
            elif self.nextDecode[packetInd] < 24:
                self.nextDecode[packetInd] += 2
            else:
                self.nextDecode[packetInd] += 4
        
    def resetPacket(self, packetInd):
        self.nextDecode[packetInd] = 4