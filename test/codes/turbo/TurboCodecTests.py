
import unittest
import math
import numpy

import wireless as rf

class TurboCodecTests(unittest.TestCase):

    def test_001_test_vectors_encode(self):
        # input and output bits were prepared using CML, using Cdma2000 scenario
        #    number 5.
        messageStr = file('test-message-1530.dat').read()
        codewordStr = file('test-codeword-1530.dat').read()
        
        # get encoder
        enc = rf.codes.turbo.TurboEncoder()
        enc.setPacket(messageStr)
        
        # encode
        encoderOutput = rf.vectorus()
        enc.encode(7668, encoderOutput)
        
        # compare number of bits
        self.assertEqual(7668, encoderOutput.size(), "encoder didn't produce the expected number of bits")
        
        # compare to test vector
        for i in xrange(encoderOutput.size()):
            self.assertEqual((ord(codewordStr[i / 8]) >> (i % 8)) & 0x1, 
                             encoderOutput[i], 
                             'output differs on bit %d' % i)

    def test_002_test_vectors_decode(self):
        # input and output bits were prepared using CML, using Cdma2000 scenario
        #    number 5.
        messageStr = file('test-message-1530.dat').read()
        codewordStr = file('test-codeword-1530.dat').read()
        
        # get encoder
        dec = rf.codes.turbo.TurboDecoder()
        
        # make LLR vector
        import random
        llr = rf.vectorf()
        for i in xrange(7668):
            bit = (ord(codewordStr[i / 8]) >> (i % 8)) & 0x1
            
            # flip bits randomly
            if random.random() < 0.21:
                bit = bit ^ 0x1
                
            if bit:
                llr.push_back(-1)
            else:
                llr.push_back(1)
        
        # decode
        dec.add(llr)
        res = dec.decode()

        # compare result to known message
        self.assertEqual(messageStr, res.packet, "decoded message doesn't match the original packet's message")



## Convert inputBits to a string
#def bitArrToChar(arr):
#    c = 0
#    for i,b in enumerate(arr):
#        c |= (b << i)
#    return chr(c)
#
#inputBits = map(int, codewordBitsStr)
#inputChars = [bitArrToChar(inputBits[8*i:8*i+8]) for i in xrange((len(inputBits)+7) /8)]
#inputStr = "".join(inputChars)
        
if __name__ == "__main__":
    unittest.main()        
