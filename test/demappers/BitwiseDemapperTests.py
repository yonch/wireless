'''
Created on Mar 17, 2011

@author: yonch
'''
import unittest
import codeBench
import numpy
import random
import math


class BitwiseDemapperTests(unittest.TestCase):
    def test_001_Gray_demapper(self):
        SNR = 3
        mapper = codeBench.GrayMapper(3, 6)
        sigma = math.sqrt(mapper.getAveragePower() / SNR)
        demapper = codeBench.GrayDemapper(mapper,
                                          3,
                                          sigma)
        
        for x in xrange(-64, 128):
            for bit in xrange(3):
                probs = numpy.zeros([2], dtype=numpy.float32)
                for y in xrange(8):
                    delta = 1.0*x - mapper.map(y)
                    p = math.exp(-1.0 * delta*delta / (2*sigma*sigma))
                    if (y & (1 << bit)):
                        probs[1] += p
                    else:
                        probs[0] += p
                expectedResult = numpy.log(probs[1] / probs[0])
                
                gotResult = demapper.demap(x,bit)
                if expectedResult > 70:
                    self.assertTrue(gotResult > 60)
                elif expectedResult < -70:
                    if not gotResult < -60:
                        pass
                    self.assertTrue(gotResult < -60)
                else:
                    self.assertAlmostEquals(expectedResult, gotResult, places=5)
        

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()