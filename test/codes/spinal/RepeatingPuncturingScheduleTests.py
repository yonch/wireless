'''
Created on Mar 12, 2011

@author: yonch
'''
import unittest
import wireless
import spinal
import numpy


class RepeatingPuncturingScheduleTests(unittest.TestCase):
    def test_001_repetitions_are_correct(self):
        """
        Make sure that the stream positions output by the repetition puncturing
        schedule are indeed repetitions of the original puncturing schedule.
        """
        NUM_CODE_STEPS = 20
        TESTED_POSITIONS = 100
        NUM_REPETITION_SET = [1,2,3,6]
        
        for numRepetitions in NUM_REPETITION_SET:
            for numLastStepSymbolsPerPass in xrange(1,4):
                punc = spinal.StridedPuncturingSchedule(NUM_CODE_STEPS, 
                                                        numLastStepSymbolsPerPass)
                punc2 = spinal.StridedPuncturingSchedule(NUM_CODE_STEPS, 
                                                        numLastStepSymbolsPerPass)
                repeatedPunc = spinal.RepeatingPuncturingSchedule(punc2,
                                                                  numRepetitions)
                
                indices = wireless.vectorus()
                
                # Run twice, once without resetting and another time after
                # resetting the puncturing schedules
                for resetIter in xrange(2):
                    punc.batchNext(TESTED_POSITIONS, indices)
                    unrepeated = list(indices)
         
                    repeatedPunc.batchNext(TESTED_POSITIONS * numRepetitions, indices)
                    repeated = list(indices)
                    
                    for i in range(len(repeated)):
                        self.assertEqual(repeated[i], unrepeated[i/numRepetitions], 
                                         "location %d in repeated schedule should match location %d in original schedule" %(i, i/numRepetitions))
                    
                    # We reset to check that the repeating puncturing schedule
                    # works well after a reset
                    punc.reset()
                    repeatedPunc.reset()
                

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()