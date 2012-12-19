
import unittest
import math
import numpy
import random

import codeBench as cb
import beliefPropagation as bp

class ParityEncoderTests(unittest.TestCase):
    def test_001_OneAtATime_encoder_explicitly(self):
        PACKET_LENGTH_BYTES = 32
        NUM_SYMBOL_BITS = 10
        NUM_SYMBOLS_TO_ENCODE = 128
        neighborGenerator = bp.RegularNeighborGenerator(PACKET_LENGTH_BYTES * 8,
                                                        4,
                                                        12345)
        
        symbolFunc = bp.ShiftedOneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        packet = numpy.random.bytes(PACKET_LENGTH_BYTES)

        encoder = bp.NLRegularOnePacketEncoder(PACKET_LENGTH_BYTES * 8,
                                      neighborGenerator,
                                      symbolFunc)

        
        # Get some symbols
        encodedSymbols = numpy.ones([NUM_SYMBOLS_TO_ENCODE], dtype=numpy.int32) * 1919191
        encoder.encode(packet,
                       0,
                       NUM_SYMBOLS_TO_ENCODE - 1,
                       encodedSymbols)
        
        # reset neighborGenerator
        neighborGenerator.reset()
        
        # Check each symbol
        for symInd, encoderSym in enumerate(encodedSymbols):
            hash = bp.OneAtATime(symInd << 8)
            
            # get participating bits
            indices = [neighborGenerator.next() for i in xrange(neighborGenerator.count())]
            bits = 0
            for bitInd in sorted(indices):
                bits = (bits << 1) | ((ord(packet[bitInd / 8]) >> (bitInd % 8)) & 0x1)
            
            # update hash with bits
            hash.update(bits)
            
            # get symbol
            sym = hash.digest() & ((1 << NUM_SYMBOL_BITS) - 1)
            
            # check symbol is as expected
            self.assertEqual(sym, encoderSym)
            
            # prepare neighbor generator for next loop iteration
            neighborGenerator.nextNode()

    def test_002_OneAtATime_encoder_correctly_encodes_substreams(self):
        PACKET_LENGTH_BYTES = 4
        NUM_SYMBOL_BITS = 10
        NUM_SYMBOLS_TO_ENCODE = 200
        NUM_SUBSET_ENCODES = 10
        neighborGenerator = bp.RegularNeighborGenerator(PACKET_LENGTH_BYTES * 8,
                                                        4,
                                                        12345)
        
        symbolFunc = bp.ShiftedOneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        packet = numpy.random.bytes(PACKET_LENGTH_BYTES)

        encoder = bp.NLRegularOnePacketEncoder(PACKET_LENGTH_BYTES * 8,
                                      neighborGenerator,
                                      symbolFunc)

        
        # Get some symbols
        encodedSymbols = numpy.ones([NUM_SYMBOLS_TO_ENCODE], dtype=numpy.int32) * 1919191
        encoder.encode(packet,
                       0,
                       NUM_SYMBOLS_TO_ENCODE - 1,
                       encodedSymbols)
        
        for i in xrange(NUM_SUBSET_ENCODES):
            subsetStart = random.randint(0, NUM_SYMBOLS_TO_ENCODE - 1)
            subsetLength = random.randint(1, NUM_SYMBOLS_TO_ENCODE - subsetStart)
            encodedSubset = numpy.ones([subsetLength], dtype=numpy.int32) * 6565656
            
            encoder.encode(packet,
                           subsetStart,
                           subsetStart + subsetLength - 1,
                           encodedSubset)
            self.assertEqual(list(encodedSubset), list(encodedSymbols[subsetStart:subsetStart+subsetLength]))

    def test_003_Noiseless_OneAtATime_decoder(self):
        PACKET_LENGTH_BYTES = 20
        NUM_SYMBOL_BITS = 10
        NUM_GENERATED_SYMBOLS = PACKET_LENGTH_BYTES*8
        neighborGenerator = bp.RegularNeighborGenerator(PACKET_LENGTH_BYTES * 8,
                                                        4,
                                                        12345)
        
        symbolFunc = bp.ShiftedOneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        packet = numpy.random.bytes(PACKET_LENGTH_BYTES)

        # make encoder
        encoder = bp.NLRegularOnePacketEncoder(PACKET_LENGTH_BYTES * 8,
                                      neighborGenerator,
                                      symbolFunc)

        # Get some symbols
        encodedSymbols = numpy.ones([NUM_GENERATED_SYMBOLS], dtype=numpy.int32) * 1919191
        encoder.encode(packet,
                       0,
                       NUM_GENERATED_SYMBOLS - 1,
                       encodedSymbols)
        
        # make decoder
        symbolFunc = bp.OneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        mapper = cb.LinearMapper(NUM_SYMBOL_BITS, NUM_SYMBOL_BITS)
        
        decoder = bp.NLRegularOnePacketDecoder(neighborGenerator,
                                      symbolFunc,
                                      mapper,
                                      PACKET_LENGTH_BYTES * 8,
                                      2,
                                      100.0)
        
        # attempt decode
        result = decoder.decode(NUM_GENERATED_SYMBOLS,
                                encodedSymbols)
        
        # check decode result
        self.assertEquals(result.packet, packet)


    def test_004_Noisy_OneAtATime_decoder(self):
        PACKET_LENGTH_BYTES = 24
        NUM_SYMBOL_BITS = 10
        TRANSMITTED_PRECISION_BITS = 13
        NUM_GENERATED_SYMBOLS = PACKET_LENGTH_BYTES*8
        SNR_DB = 12.0
        neighborGenerator = bp.RegularNeighborGenerator(PACKET_LENGTH_BYTES * 8,
                                                        6,
                                                        12345)
        
        symbolFunc = bp.ShiftedOneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        packet = numpy.random.bytes(PACKET_LENGTH_BYTES)

        # make encoder
        encoder = bp.NLRegularOnePacketEncoder(PACKET_LENGTH_BYTES * 8,
                                      neighborGenerator,
                                      symbolFunc)

        # Get some symbols
        encodedSymbols = numpy.ones([NUM_GENERATED_SYMBOLS], dtype=numpy.int32) * 1919191
        encoder.encode(packet,
                       0,
                       NUM_GENERATED_SYMBOLS - 1,
                       encodedSymbols)
        
        # map symbols
        mapper = bp.LinearMapper(NUM_SYMBOL_BITS,TRANSMITTED_PRECISION_BITS)
        mapper.process(encodedSymbols)
#        print "mapped symbols ", encodedSymbols
        
        # Noisify
        SNR_ratio = math.pow(10.0, SNR_DB/10.0)
        noiseAveragePower = mapper.getAveragePower() / SNR_ratio
        stddev = math.sqrt(noiseAveragePower)
        channel = cb.AWGNChannel(stddev)
        channel.process(encodedSymbols)
#        print "noisy symbols ", encodedSymbols
        
        # make decoder
        symbolFunc = bp.OneAtATimeSymbolFunction(NUM_SYMBOL_BITS)
        
        decoder = bp.NLRegularOnePacketDecoder(neighborGenerator,
                                      symbolFunc,
                                      mapper,
                                      PACKET_LENGTH_BYTES * 8,
                                      40,
                                      stddev)
        
        # attempt decode
        result = decoder.decode(NUM_GENERATED_SYMBOLS,
                                encodedSymbols)
        
        # check decode result
        self.assertEquals(result.packet, packet)


if __name__ == "__main__":
    unittest.main()        