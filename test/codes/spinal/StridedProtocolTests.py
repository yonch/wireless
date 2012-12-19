'''
Created on Mar 12, 2011

@author: yonch
'''
import unittest
import spinal
import wireless
import numpy


class StridedProtocolTests(unittest.TestCase):
    def test_001_no_repetitions(self):
        
        NUM_TESTED_LOCATIONS = 500
        
        for modulus in xrange(1,30):
            for numLastValueSymbols in [1,2,7]:
                punc = spinal.StridedPuncturingSchedule(modulus,numLastValueSymbols)
                numSymbolsFullPass = modulus - 1 + numLastValueSymbols
                spineIndices = wireless.vectorus()
                proto = spinal.StridedProtocol(1,
                                               modulus,
                                               NUM_TESTED_LOCATIONS,
                                               numLastValueSymbols,
                                               1)
                
                lastNumSymbols = 0
                
                nextNumSymbols = proto.numSymbolsNextDecode(0)
                while(nextNumSymbols != 0):
                    # How many symbols were added?
                    numAddedSymbols = nextNumSymbols - lastNumSymbols
                    self.assertNotEqual(numAddedSymbols, 0, "Protocol did not advance!")
                    if nextNumSymbols != NUM_TESTED_LOCATIONS:
                        self.assertGreaterEqual(numAddedSymbols, 
                                                numSymbolsFullPass / 8, 
                                                "Expected the number of symbols between decodes to fit the puncturing")
                        self.assertLessEqual(numAddedSymbols, 
                                             (numSymbolsFullPass + 7) / 8, 
                                             "Expected the number of symbols between decodes to fit the puncturing")
                    
                    # Get the spine indices from the puncturing schedule
                    punc.batchNext(numAddedSymbols, spineIndices)
                    
                    # Make sure that the sequence in spineIndices is strictly 
                    # increasing
                    for i in xrange(spineIndices.size() - 1):
                        self.assertGreaterEqual(spineIndices[i+1], spineIndices[i],
                                                "Expected the spine location between decodes to be a non-decreasing sequence, but location %d violates this" % (lastNumSymbols + i))
                    
                    # Advance to next protocol demand
                    proto.setResult(0, nextNumSymbols, False)
                    lastNumSymbols = nextNumSymbols
                    nextNumSymbols = proto.numSymbolsNextDecode(0)

    def test_002_with_repetitions(self):
        
        NUM_TESTED_LOCATIONS = 90
        
        for modulus in xrange(1,30):
            for numLastValueSymbols in [1,2,7]:
                noRepetitionProto = spinal.StridedProtocol(1,
                                               modulus,
                                               NUM_TESTED_LOCATIONS,
                                               numLastValueSymbols,
                                               1)
                for numRepetitions in xrange(1,11):
                    proto = spinal.StridedProtocol(1,
                                                   modulus,
                                                   NUM_TESTED_LOCATIONS * numRepetitions,
                                                   numLastValueSymbols,
                                                   numRepetitions)

                    noRep = noRepetitionProto.numSymbolsNextDecode(0)
                    withRep = proto.numSymbolsNextDecode(0)
                    self.assertEqual(noRep * numRepetitions, withRep)
                    
                    while noRep != 0:
                        noRepetitionProto.setResult(0, noRep, False)
                        noRep = noRepetitionProto.numSymbolsNextDecode(0)
                        proto.setResult(0, withRep, False)
                        withRep = proto.numSymbolsNextDecode(0)
                        self.assertEqual(noRep * numRepetitions, withRep)
                    
                    noRepetitionProto.resetPacket(0)
                

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.PuncturingScheduleTests']
    unittest.main()