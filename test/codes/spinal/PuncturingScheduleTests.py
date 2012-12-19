'''
Created on Mar 12, 2011

@author: yonch
'''
import unittest
import spinal
import numpy


class PuncturingScheduleTests(unittest.TestCase):
    def test_001_passLength(self):
        """
        The puncturing schedule should return the right pass length:
        
        If number of code steps, N, is divided evenly by 4, then the length
            of the pass should be N/4 + numLastSymbolsPerPass.
        Otherwise, length of pass should be similar (but N/4 should be rounded
            up)
        """
        MAX_NUM_CODE_STEPS = 20
        for numCodeSteps in xrange(1, MAX_NUM_CODE_STEPS + 1):
            for numLastStepSymbolsPerPass in xrange(3):
                punc = spinal.StridedPuncturingSchedule(numCodeSteps, 
                                             numLastStepSymbolsPerPass)
                
                self.assertEquals(
                        punc.numSymbolsPerPass(),
                        ((numCodeSteps + 3) / 4) + numLastStepSymbolsPerPass)
                
    def test_002_puncturing(self):
        """
        This test considers a scenario where several passes are generated 
            contiguously.
        We produce the sequence telling which spine value each symbol was 
            generated from, in an inefficient but straightforward way. This
            sequence is then used to check the correctness of the puncturing
            schedule implementation.
        """
        MAX_NUM_CODE_STEPS = 20
        NUM_PASSES = 20
        for numCodeSteps in xrange(1, MAX_NUM_CODE_STEPS + 1):
            for numLastStepSymbolsPerPass in xrange(3):
                codePassTransmitted = range(numCodeSteps)
                codeStepsMod4 = numCodeSteps % 4
                
                # the number of code steps is increased to be a multiple of 4.
                # the last code step is transmitted in the added symbols
                if codeStepsMod4 != 0:
                    codePassTransmitted += ([numCodeSteps-1] * (4-codeStepsMod4))
                
                # Also, in order to transmit more of last code step, we add
                # 4*numLastStepSymbolsPerPass of the last code step
                codePassTransmitted += ([numCodeSteps-1] * (4*numLastStepSymbolsPerPass))
                
                passes = []
                shiftMap = [1,3,0,2]
                for passInd in xrange(NUM_PASSES):
                    passes += codePassTransmitted[shiftMap[passInd % 4]::4]
                
                numSymbolsPerPass = len(codePassTransmitted) / 4
                
                punc = spinal.StridedPuncturingSchedule(numCodeSteps, 
                                             numLastStepSymbolsPerPass)
                
                # Check validity of numSymbolsForCodeStep
                for passInd in xrange(0,NUM_PASSES):
                    testedPasses = passes[:(passInd+1)*numSymbolsPerPass]
                    for step in xrange(numCodeSteps):
                        self.assertEqual(punc.numSymbolsForCodeStep(passInd+1, step),
                                         testedPasses.count(step))
                
                # Check validity of symbolIndexInPassStream
                for ind, step in enumerate(passes):
                    symbolNumInStep = passes[:ind+1].count(step) - 1

                    self.assertEqual(
                        punc.symbolIndexInPassStream(step, symbolNumInStep),
                        ind)
                

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()