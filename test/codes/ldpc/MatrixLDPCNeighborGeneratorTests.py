
import unittest

import codeBench as cb
import beliefPropagation as bp
import numpy

class MatrixLDPCNeighborGeneratorTests(unittest.TestCase):
    
    def addToIndicesModuluZ(self, indices, y, Z):
        """
        Given a list of indices, adds y to x, keeping (x / Z) constant.
        ie, adds y to x % Z with overflow not affecting x/Z
        
        """
        
        return [Z * (x / Z) + ((x + y) % Z) for x in indices]
            
    
    def test_001_wifi_matrix(self):
        code = cb.getWifiLDPC648(1,2)
        generator = bp.MatrixLDPCNeighborGenerator(code)
        
        # row 0 in table
        for i in xrange(27):
            generator.set(i)
            self.assertEqual(generator.count(), 7)
            expectedIndices = self.addToIndicesModuluZ(
                            [0,4*27,5*27,8*27, 11*27,12*27+1, 13*27], i, 27)
            gotIndices = [generator.next() for j in xrange(generator.count())]
            self.assertEqual(gotIndices, expectedIndices)
        
        # row 6 in table
        for i in xrange(27):
            generator.set(6*27+i)
            self.assertEqual(generator.count(), 7)
            expectedIndices = self.addToIndicesModuluZ(
                            [25,4*27+8,8*27+7, 9*27+18,12*27,18*27, 19*27], i, 27)
            gotIndices = [generator.next() for j in xrange(generator.count())]
            self.assertEqual(gotIndices, expectedIndices)
        
        # row 10 in table
        for i in xrange(27):
            generator.set(270 + i)
            self.assertEqual(generator.count(), 8)
            expectedIndices = self.addToIndicesModuluZ(
                            [25,2*27+8,4*27+23,5*27+18,7*27+14,8*27+9,22*27,23*27], i, 27)
            gotIndices = [generator.next() for j in xrange(generator.count())]
            self.assertEqual(gotIndices, expectedIndices)
        
        # row 11 in table
        for i in xrange(27):
            generator.set(11*27 + i)
            self.assertEqual(generator.count(), 7)
            expectedIndices = self.addToIndicesModuluZ(
                            [3,4*27+16,7*27+2,8*27+25,9*27+5,12*27+1,23*27], i, 27)
            gotIndices = [generator.next() for j in xrange(generator.count())]
            self.assertEqual(gotIndices, expectedIndices)
    
    def test_002_compare_encoder_output_to_generator(self):
        code = cb.getWifiLDPC648(1,2)
        generator = bp.MatrixLDPCNeighborGenerator(code)
        encoder = cb.MatrixLDPCEncoder(code, 1, 1)
        
        encodedBits = numpy.empty([648], dtype=numpy.int32)
        packet = numpy.random.bytes(((648/2)+7)/8)
        encoder.setPacket(0, packet)
        encoder.getSymbols(0,0,647,encodedBits)
        
        generator = bp.MatrixLDPCNeighborGenerator(code)
        
        # check each parity bit
        for i in xrange(648/2,648):
            bit = 0
            generator.set(i - 648/2)
            while generator.hasMore():
                bit += encodedBits[generator.next()]
            
            self.assertEqual(bit % 2, 0)

        
        
if __name__ == "__main__":
    unittest.main()