
import unittest
import math
import numpy
import random

import wireless as rf
import itpp
import wireless

class LayeredEncoderTests(unittest.TestCase):

    def test_001_test_message_encode(self):
        messageStr = file('strider-message.dat').read()
        
        # get turbo encoder
        turbo = rf.codes.strider.StriderTurboCode.createEncoder(1530)
        # get QPSK mapper
        mapper = rf.mappers.QPSKMapper()
        # get G
        G = numpy.empty((27,33), dtype=numpy.complex128)
        rf.codes.strider.getStriderGeneratorMatrix(G)
        # get strider encoder
        enc = rf.codes.strider.LayeredEncoder(turbo, 
                                              mapper,
                                              1530*33,
                                              3840,
                                              G,
                                              False)
        enc.setPacket(messageStr)
        
        # encode
        encoderOutput = rf.vector_csymbol()
        enc.encode(3840*27, encoderOutput)
        
        # Read expected output
        import array
        reals = array.array('d', file('strider_packets_doubles_real.dat').read())
        imags = array.array('d', file('strider_packets_doubles_imag.dat').read())
        
        # compare number of bits
        self.assertEqual(len(reals), encoderOutput.size(), "encoder didn't produce the expected number of symbols")
        
        # compare to test vector
        for i in xrange(encoderOutput.size()):
            self.assertAlmostEquals(reals[i], encoderOutput[i].real, 4, 
                                    "Real part doesn't match at index %d, expected %f got %f" % (i, reals[i], encoderOutput[i].real)) 
            self.assertAlmostEquals(imags[i], encoderOutput[i].imag, 4, 
                                    "Imaginary part doesn't match at index %d, expected %f got %f" % (i, imags[i], encoderOutput[i].imag)) 


    def test_002_test_known_message_decode(self):
        NUM_GENERATED_PASSES = 3
        
        # get turbo encoder
        turbo_enc = rf.codes.strider.StriderTurboCode.createEncoder(1530)
        # get QPSK mapper
        mapper = rf.mappers.QPSKMapper()
        # get G
        G = numpy.empty((27,33), dtype=numpy.complex128)
        rf.codes.strider.getStriderGeneratorMatrix(G)
        # get turbo decoder
        turbo_dec = rf.codes.strider.StriderTurboCode.createDecoder(1530)
        # get demapper
        qpsk = itpp.QPSK()
        demapper = rf.demappers.ItppComplexDemapper(qpsk, 2, False)
        # get composite decoder
        composite_dec = rf.codes.ComplexSymbolToLLRDecoderAdaptor(demapper, turbo_dec)
        
        # get strider decoder
        dec = rf.codes.strider.ComplexLayeredDecoder(3840,
                                              G,
                                              1530,
                                              composite_dec,
                                              turbo_enc,
                                              mapper,
                                              False)

        # read symbols to be decoded        
        import array
        reals = array.array('d', file('strider_packets_doubles_real.dat').read())
        imags = array.array('d', file('strider_packets_doubles_imag.dat').read())
        syms = [reals[i] + imags[i]*1j for i in xrange(len(reals))]
        passes = wireless.vector_csymbol(syms[:3840*NUM_GENERATED_PASSES])
        
        # attempt to decode
        dec.reset()
        dec.add(passes,0.01)
        res = dec.decode()
        
        message = file('strider-message.dat').read()
        self.assertEqual(len(res.packet), len(message))
        self.assertEqual(res.packet, message)

    def test_003_test_message_with_noise(self):
        NUM_GENERATED_PASSES = 5
        N0 = 0.06
        
        # get strider decoder
        dec = rf.codes.strider.StriderFactory.createDecoder(1530)
        
        # get turbo encoder
        enc = rf.codes.strider.StriderFactory.createEncoder(1530)
        
        for i in xrange(2):
            # get random message
            message = numpy.random.bytes(6179)
            message += (chr(numpy.random.randint(0,4) & 0x3))
            # encode
            enc.setPacket(message)
            encoderOutput = rf.vector_csymbol()
            enc.encode(3840*NUM_GENERATED_PASSES, encoderOutput)
    
            # Add noise
            sigma = math.sqrt(N0 / 2.0)
            for i in xrange(encoderOutput.size()):
                encoderOutput[i] += random.normalvariate(0,sigma) 
                encoderOutput[i] += 1j * random.normalvariate(0,sigma) 
            
            # attempt to decode
            dec.reset()
            dec.add(encoderOutput,N0)
            res = dec.decode()
            
            self.assertEqual(len(res.packet), len(message))
            self.assertEqual(res.packet, message)

        
if __name__ == "__main__":
    unittest.main()        
