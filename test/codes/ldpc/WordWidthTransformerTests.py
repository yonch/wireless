'''
Created on Mar 17, 2011

@author: yonch
'''
import unittest
import codeBench
import numpy
import random
import math


class WordWidthTransformerTests(unittest.TestCase):
    def test_001_nonimportant_bits_do_not_influence(self):
        src = (numpy.random.randint(0, high=1024, size=30).astype(numpy.int32) << 1)
        
        dst = numpy.ones([15], dtype = numpy.int32) * 0xFFFF
        
        codeBench.transformSymbols(src, dst, 1, 2)
        
        self.assertTrue((dst == 0).all())
        
    def test_002_random_test_correctness(self):
        for srcNumBits in [1,2,7,8,31]:
            for dstNumBits in [1,2,7,8,31]:
                src = numpy.random.randint(0, 
                                           high=((1<<srcNumBits)-1), 
                                           size=31* 7 * 8 / srcNumBits).astype(numpy.int32)
        
                dst = numpy.ones([31*7*8 / dstNumBits], dtype = numpy.int32) * 0x7FFFFFFF
        
                codeBench.transformSymbols(src, dst, srcNumBits, dstNumBits)
                
                # check all bits
                for i in xrange(31*7*8):
                    self.assertEquals((src[i / srcNumBits] >> (i % srcNumBits)) & 0x1,
                                      (dst[i / dstNumBits] >> (i % dstNumBits)) & 0x1)
        

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()