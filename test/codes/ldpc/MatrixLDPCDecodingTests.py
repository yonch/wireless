
import unittest
import math
import numpy

import wireless as rf

class MatrixLDPCDecodingTests(unittest.TestCase):
    def test_001_decode_without_noise(self):
        code = rf.codes.ldpc.getWifiLDPC648(1,2)
        encoder = rf.codes.ldpc.MatrixLDPCEncoder(code, 1)

        encodedBits = rf.vectorus()
        packet = numpy.random.bytes(((648/2)+7)/8)
        encoder.setPacket(packet)
        encoder.encode(648,encodedBits)
        
        # convert encodedBits into LLR values.
        encodedLLRs = rf.vectorf()
        for i in xrange(648):
            encodedLLRs.push_back(8.0* encodedBits[i] - 4.0)

        decoder = rf.codes.ldpc.MatrixLDPCDecoder(code, 5)
        decoder.add(encodedLLRs)
        
        res = decoder.decode()
        
        self.assertEquals(res.packet[:40], packet[:40])
        self.assertEquals(ord(res.packet[40]) & 0x0F, ord(packet[40]) & 0x0F)


    def test_002_decode_with_BSC_noise(self):
        P = 0.96
        NUM_EXPERIMENTS = 20
        code = rf.codes.ldpc.getWifiLDPC648(1,2)
        encoder = rf.codes.ldpc.MatrixLDPCEncoder(code, 1)

        for experimentInd in xrange(NUM_EXPERIMENTS):
            encodedBits = rf.vectorus()
            packet = numpy.random.bytes(((648/2)+7)/8)
            encoder.setPacket(packet)
            encoder.encode(648,encodedBits)
            
            symVector = rf.vector_symbol()
            noisyVector = rf.vector_symbol()
            for b in list(encodedBits): symVector.push_back(b)
            noisifier = rf.channels.BscChannel(1 - P)
            noisifier.seed(numpy.array([numpy.random.randint(0,1<<31)], dtype=numpy.uint32))
            noisyBits = noisifier.process(symVector, noisyVector)
            
            # convert encodedBits into LLR values.
            logLLR = math.log(P/(1.0-P))
            encodedLLRs = rf.vectorf()
            for i in xrange(648):
                encodedLLRs.push_back(2.0 * logLLR * noisyVector[i] - logLLR)
            
            decoder = rf.codes.ldpc.MatrixLDPCDecoder(code, 50)
            decoder.add(encodedLLRs)
            
            res = decoder.decode()
            
            self.assertEquals(res.packet[:40], packet[:40])
            self.assertEquals(ord(res.packet[40]) & 0x0F, ord(packet[40]) & 0x0F)
        
    def test_003_decode_with_AWGN_noise_over_BPSK(self):
        SNR_dB = 0
        code = rf.codes.ldpc.getWifiLDPC648(1,2)
        encoder = rf.codes.ldpc.MatrixLDPCEncoder(code, 1)

        encodedBits = rf.vectorus()
        packet = numpy.random.bytes(((648/2)+7)/8)
        encoder.setPacket(packet)
        encoder.encode(648,encodedBits)
        
        # Convert into symbols with 20 bit precision
        symbols = rf.vector_symbol()
        mapper = rf.mappers.LinearMapper(1,20)
        mapper.process(encodedBits, symbols)

        # Apply AWGN noise
        noisySymbols = rf.vector_symbol()
        n0 = mapper.getAveragePower() / math.pow(10.0, SNR_dB/10.0)
        noisifier = rf.channels.SymbolAwgnChannel(n0)
        noisifier.seed(numpy.array([numpy.random.randint(0,1<<31)], dtype=numpy.uint32))
        noisifier.process(symbols,noisySymbols)
        
        # Soft-demap
        demapper = rf.demappers.GrayDemapper(rf.mappers.GrayMapper(1, 20), 1)
        receivedLLRs = rf.vectorf()
        demapper.process(noisySymbols, n0, receivedLLRs)
        
        # Decode
        decoder = rf.codes.ldpc.MatrixLDPCDecoder(code, 10)
        decoder.add(receivedLLRs)
        
        res = decoder.decode()
        
        self.assertEquals(res.packet[:40], packet[:40])
        self.assertEquals(ord(res.packet[40]) & 0x0F, ord(packet[40]) & 0x0F)
        
    def test_004_one_iteration_result_check(self):
        P = 0.94
        code = rf.codes.ldpc.getWifiLDPC648(1,2)
        encoder = rf.codes.ldpc.MatrixLDPCEncoder(code, 1)

        encodedBits = rf.vectorus()
        packet = '2659df41ec49b827bd494aaed625201cabfb2a75d5fc7d5f2c03c242794cb108271d54881bcf20ca09'.decode('hex')
        expected_packet = {1:'2659df41ec09ba273f4d5baed621201cabbb2a75d5fcfd5f2c03c242796cb108271d54a81bcf30ca09'.decode('hex'),
                           2:'2659df41ec49b827bf4d4aeed621201cabfb2a75d5fc7d5f2c03c242794cb108271d54881bcf30ca09'.decode('hex'),
                           3:'2659df41ec49b827bd494aaed625201cabfb2a75d5fc7d5f2c03c242794cb108271d54881bcf30ca09'.decode('hex'),
                           4:'2659df41ec49b827bd494aaed625201cabfb2a75d5fc7d5f2c03c242794cb108271d54881bcf20ca09'.decode('hex')}
        encoder.setPacket(packet)
        encoder.encode(648,encodedBits)
        
        symVector = rf.vector_symbol()
        noisyVector = rf.vector_symbol()
        for b in list(encodedBits): symVector.push_back(b)
        noisifier = rf.channels.BscChannel(1 - P)
        noisifier.seed(numpy.array([134123], dtype=numpy.uint32))
        noisyBits = noisifier.process(symVector, noisyVector)
        
        # convert encodedBits into LLR values.
        logLLR = math.log(P/(1.0-P))
        encodedLLRs = rf.vectorf()
        for i in xrange(648):
            encodedLLRs.push_back(2.0 * logLLR * noisyVector[i] - logLLR)
        
        for num_iters in sorted(expected_packet.keys()):
            decoder = rf.codes.ldpc.MatrixLDPCDecoder(code, num_iters)
            decoder.add(encodedLLRs)
            
            res = decoder.decode()
            #print res.packet[:41].encode('hex')
            self.assertEquals(res.packet, expected_packet[num_iters])
        

if __name__ == "__main__":
    unittest.main()        