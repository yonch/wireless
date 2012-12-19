'''
Created on Mar 17, 2011

@author: yonch
'''
import unittest
import spinal
import spinal.reference
import numpy
import random


class CachedEncoderTests(unittest.TestCase):
    def test_001_Produces_correct_symbols(self):
        PACKET_LENGTH_BITS = 192
        NUM_PASSES = 60
        NUM_CACHED_PASSES = 30
        NUM_LAST_CODE_STEP_SYMBOLS = 2
        NUM_PACKETS = 10
        NUM_COMPARISONS = 200
        
        codeParams = spinal.reference.CodeParams(salsaNumRounds = 12,
                                         hashWordSize = 64,
                                         numBitsPerCodingStep = 4,
                                         symbolSizeBits = 10,
                                         transmittedPointPrecisionBits = 16)
        
        # get packets
        packets = [numpy.random.bytes(PACKET_LENGTH_BITS / 8) for i in xrange(NUM_PACKETS)]
        
        assert(PACKET_LENGTH_BITS % codeParams.numBitsPerCodingStep == 0)
        
        
        # decide on symbol schedule
        numCodingSteps = PACKET_LENGTH_BITS / codeParams.numBitsPerCodingStep
        numSymbolsForCodingStep = numpy.ones([numCodingSteps], dtype=numpy.uint32)
        numSymbolsForCodingStep *= NUM_PASSES
        numSymbolsForCodingStep[-1] *= NUM_LAST_CODE_STEP_SYMBOLS

        fullPassLength = numCodingSteps - 1 + NUM_LAST_CODE_STEP_SYMBOLS
        
        # uncached C++ encoder
        uncachedEncoder = spinal.SalsaEncoder(
                            codeParams.numBitsPerCodingStep,
                            numCodingSteps,
                            spinal.StridedPuncturingSchedule(numCodingSteps, 
                                                  NUM_LAST_CODE_STEP_SYMBOLS),
                            codeParams.symbolSizeBits)
        
        # cached C++ encoder
        cachedEncoder = spinal.CachedSalsaEncoder(
                            uncachedEncoder,
                            NUM_PACKETS,
                            NUM_CACHED_PASSES * fullPassLength)

        for i in xrange(NUM_PACKETS):
            cachedEncoder.setPacket(i, packets[i])
        
        for compInd in xrange(NUM_COMPARISONS):
            packetInd = random.randint(0, NUM_PACKETS-1)
            firstSymbolInd = random.randint(0, NUM_PASSES*fullPassLength - 1)
            lastSymbolInd = random.randint(firstSymbolInd, NUM_PASSES*fullPassLength - 1)
            
            # encode using uncached
            uncachedSymbols = numpy.ones([lastSymbolInd - firstSymbolInd + 1], dtype=numpy.int32) * 0x7F7F7F7F
            uncachedEncoder.encode(packets[packetInd],
                                   firstSymbolInd,
                                   lastSymbolInd,
                                   uncachedSymbols)
            
            # encode using cached
            cachedSymbols = numpy.ones([lastSymbolInd - firstSymbolInd + 1], dtype=numpy.int32) * 0x77777777
            cachedEncoder.getSymbols(packetInd, 
                                     firstSymbolInd, 
                                     lastSymbolInd, 
                                     cachedSymbols)
            
            # compare
            self.assertTrue( (uncachedSymbols == cachedSymbols).all() )
                    

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()