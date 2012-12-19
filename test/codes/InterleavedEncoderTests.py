
import unittest
import math
import numpy
import random

import wireless as rf
import wireless.codes.ldpc as ldpc

class InterleavedEncoderTests(unittest.TestCase):

    def test_001_interleaving_works(self):
        NUM_TESTED_SYMBOLS = 2000
        
        # randomize message
        msg = numpy.random.bytes(40) + chr(numpy.random.randint(0,1<<4))
        # randomize interleaving seqeuence
        interleaving = [random.randrange(648) for i in xrange(NUM_TESTED_SYMBOLS)] + [647]

        # Encode the message using a reference encoder
        refEnc = ldpc.MatrixLDPCEncoder(ldpc.getWifiLDPC648(1,2), 1)
        refSeq = rf.vectorus()
        refEnc.setPacket(msg)
        refEnc.encode(648, refSeq)
        
        # Encode the message using an InterleavedEncoder
        innerEnc = ldpc.MatrixLDPCEncoder(ldpc.getWifiLDPC648(1,2), 1)
        interleavingVec = rf.vectorus(interleaving)
        enc = rf.codes.InterleavedEncoder(innerEnc, interleavingVec)
        enc.setPacket(msg)
        out = []
        tmpOut = rf.vectorus()
        numRemaining = NUM_TESTED_SYMBOLS
        while(numRemaining > 0):
            print "remaining", numRemaining
            numNew = random.randrange(1, min(10, numRemaining)+1)
            enc.encode(numNew, tmpOut)
            out.extend(list(tmpOut))
            numRemaining -= numNew
            
        # compare actual sequence to anticipated sequence
        for i in xrange(NUM_TESTED_SYMBOLS):
            self.assertEqual(out[i], refSeq[interleaving[i]], 
                             "output sequence doesn't match computed output at index %d" % i)
        
if __name__ == "__main__":
    unittest.main()        
