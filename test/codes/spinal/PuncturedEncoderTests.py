'''
Created on Jan 9, 2011

@author: yonch
'''
import unittest
import hashes
import spinal
import spinal.reference
import wireless
import numpy


class PuncturedEncoderTests(unittest.TestCase):
    
    def test_001_Punctured_Encoder(self):
        PACKET_LENGTH_BITS = 192
        NUM_PASSES = 5
        NUM_LAST_CODE_STEP_SYMBOLS = 2
        
        codeParams = spinal.reference.CodeParams(salsaNumRounds = 12,
                                         hashWordSize = 64,
                                         numBitsPerCodingStep = 4,
                                         symbolSizeBits = 10,
                                         transmittedPointPrecisionBits = 10,
                                         lookaheadDepth = 0)
        
        # get packet
        packet = numpy.random.bytes(PACKET_LENGTH_BITS / 8)
        
        assert(PACKET_LENGTH_BITS % codeParams.numBitsPerCodingStep == 0)
        
        numCodingSteps = PACKET_LENGTH_BITS / codeParams.numBitsPerCodingStep
        totalNumSymbols = (numCodingSteps - 1 + NUM_LAST_CODE_STEP_SYMBOLS) * NUM_PASSES
     
        # decide on symbol schedule
        cppSpineValueIndices = wireless.vectorus()
        for codeStep in xrange(numCodingSteps - 1):
            for i in xrange(NUM_PASSES):
                cppSpineValueIndices.push_back(codeStep)
        # For last code step
        for i in xrange(NUM_PASSES * NUM_LAST_CODE_STEP_SYMBOLS):
            cppSpineValueIndices.push_back(numCodingSteps - 1)

        
        # Make c++ encoder without puncturing
        cppEncoder = spinal.CodeFactory(
                codeParams.numBitsPerCodingStep,
                numCodingSteps,
                codeParams.symbolSizeBits) \
            .salsa().encoder()
        unpuncturedSymbols = wireless.vectorus()

        
        # Encode unpunctured with C++ encoder
        cppEncoder.setPacket(packet)
        cppEncoder.encode(cppSpineValueIndices,
                          unpuncturedSymbols)
        
        # Make puncturing schedule
        punc = spinal.StridedPuncturingSchedule(numCodingSteps,
                                                NUM_LAST_CODE_STEP_SYMBOLS)
        
        # Make encoder with puncturing
        puncturedEncoder = spinal.PuncturedEncoder(cppEncoder,
                                                   punc)
        
        # Encode with puncturing
        puncturedSymbols = wireless.vectorus()
        puncturedEncoder.setPacket(packet)
        puncturedEncoder.encode(totalNumSymbols, puncturedSymbols)
        
        # Make a dictionary from spine value to all locations that are produced
        # from that spine value
        d = {}
        punc.reset()
        for i in xrange(totalNumSymbols):
            d.setdefault(punc.next(), []).append(i)
        
        # Check the two encoders produced the same symbols (taking into account
        #    the puncturing schedule)
        for codeStep in xrange(numCodingSteps - 1):
            for i in xrange(NUM_PASSES):
                self.assertEqual(unpuncturedSymbols[codeStep * NUM_PASSES + i], 
                                 puncturedSymbols[ d[codeStep][i] ],
                                 "location %d should contain the %dth sample from spine value %d, but does not match" \
                                    % (d[codeStep][i], i, codeStep))
        # last codestep
        codeStep = (numCodingSteps - 1)
        for i in xrange(NUM_PASSES * NUM_LAST_CODE_STEP_SYMBOLS):
            self.assertEqual(unpuncturedSymbols[codeStep * NUM_PASSES + i], 
                             puncturedSymbols[ d[codeStep][i] ],
                             "location %d should contain the %dth sample from spine value %d, but does not match" \
                                % (d[codeStep][i], i, codeStep))


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()