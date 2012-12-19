
import unittest
import math
import numpy

import wireless as rf


class LayerSuperpositionTests(unittest.TestCase):
    def test_001_layering_works(self):
        for batchSize in [1,2,3,7]:
            for numPasses in [1,2,3,7]:
                for passLength in [1,2,3,7,11]:
                    G = numpy.zeros([numPasses, batchSize], dtype = numpy.complex64)
                    G += numpy.random.random((numPasses, batchSize)) + 1j*numpy.random.random((numPasses, batchSize))
                    layers = numpy.random.random((batchSize, passLength)) + 1j*numpy.random.random((batchSize, passLength))
                    
                    enc = rf.codes.strider.LayerSuperposition(passLength, G)
                    for i in xrange(batchSize):
                        enc.setLayer(i, rf.general.vector_csymbol(list(layers[i,:])))
                    
                    expectedResult = list((numpy.matrix(G) * numpy.matrix(layers)).flat)
                    
                    gotResult = [enc.next() for i in xrange(passLength * numPasses)]
                    
                    self.assertEqual(len(expectedResult), len(gotResult))
                    for i in xrange(len(expectedResult)):
                        self.assertAlmostEqual(expectedResult[i], gotResult[i], 5)

if __name__ == "__main__":
    unittest.main()        