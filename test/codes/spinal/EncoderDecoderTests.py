'''
Created on Jan 9, 2011

@author: yonch
'''
import unittest
import hashes
import spinal
import spinal.reference
import wireless
import numpy


class EncoderDecoderTests(unittest.TestCase):
    def test_001_Salsa(self):
        NUM_ROUNDS = 12
        INITIAL_VALUE = 0xFF112899AB012344
        DATA = 0xFFAB0123
        NUM_COMPARED_VALUES = 300
        
        cppSalsa = hashes.SalsaUnlimitedHash(INITIAL_VALUE, DATA)
        
        pyDigest, pyState = spinal.reference.SalsaHash.unlimitedHash(INITIAL_VALUE, DATA, NUM_COMPARED_VALUES, NUM_ROUNDS)
        
        self.assertEquals(pyDigest, cppSalsa.getSeed())
        self.assertTrue(pyState[:NUM_COMPARED_VALUES] == [cppSalsa.next() for i in xrange(NUM_COMPARED_VALUES)])
        
    
    def test_002_Encoder(self):
        PACKET_LENGTH_BITS = 192
        NUM_PASSES = 5
        NUM_LAST_CODE_STEP_SYMBOLS = 2
        
        codeParams = spinal.reference.CodeParams(salsaNumRounds = 12,
                                         hashWordSize = 64,
                                         numBitsPerCodingStep = 4,
                                         symbolSizeBits = 16,
                                         transmittedPointPrecisionBits = 16,
                                         lookaheadDepth = 0)
        
        # get packet
        packet = numpy.random.bytes(PACKET_LENGTH_BITS / 8)
        
        assert(PACKET_LENGTH_BITS % codeParams.numBitsPerCodingStep == 0)
        
        # decide on symbol schedule
        numCodingSteps = PACKET_LENGTH_BITS / codeParams.numBitsPerCodingStep
        numSymbolsForCodingStep = numpy.ones([numCodingSteps], dtype=numpy.uint32)
        numSymbolsForCodingStep *= NUM_PASSES
        numSymbolsForCodingStep[-1] *= NUM_LAST_CODE_STEP_SYMBOLS
        
        # Create C++ encoder
        cppEncoder = spinal.CodeFactory(
                codeParams.numBitsPerCodingStep,
                numCodingSteps,
                codeParams.symbolSizeBits) \
            .salsa().encoder()
        cppSymbols = wireless.vectorus()
        
        # Convert symbol schedule to spine value indices array
        cppSpineValueIndices = wireless.vectorus()
        for codeStep in xrange(numCodingSteps):
            for i in xrange(numSymbolsForCodingStep[codeStep]):
                cppSpineValueIndices.push_back(codeStep)
            
        # Encode with c++ encoder
        cppEncoder.setPacket(packet)
        cppEncoder.encode(cppSpineValueIndices,
                          cppSymbols)
        
        # Encode with python encoder
        pyEncoder = spinal.reference.Encoder(codeParams)
        pySymbols = pyEncoder.encode(packet, numSymbolsForCodingStep)

        # Check the two encoders produced the same symbols
        self.assertTrue(pySymbols == list(cppSymbols))
                

    def test_003_Decoder(self):
        PACKET_LENGTH_BITS = 96*3
        NUM_PASSES = 5
        NUM_LAST_CODE_STEP_SYMBOLS = 2
        BEAM_WIDTH = 16

        codeParams = spinal.reference.CodeParams(salsaNumRounds = 12,
                                         hashWordSize = 64,
                                         numBitsPerCodingStep = 3,
                                         symbolSizeBits = 10,
                                         transmittedPointPrecisionBits = 16,
                                         lookaheadDepth = 0)
        
        # get packet
        packet = numpy.random.bytes(PACKET_LENGTH_BITS / 8)
        
        assert(PACKET_LENGTH_BITS % codeParams.numBitsPerCodingStep == 0)
        
        # decide on symbol schedule
        numCodingSteps = PACKET_LENGTH_BITS / codeParams.numBitsPerCodingStep
        cppSpineValueIndices = wireless.vectorus()
        for codeStep in xrange(numCodingSteps - 1):
            for i in xrange(NUM_PASSES):
                cppSpineValueIndices.push_back(codeStep)
        # For last code step
        for i in xrange(NUM_PASSES * NUM_LAST_CODE_STEP_SYMBOLS):
            cppSpineValueIndices.push_back(numCodingSteps - 1)
        
        
        # Create C++ encoder and mapper
        cppEncoder = spinal.CodeFactory(
                codeParams.numBitsPerCodingStep,
                numCodingSteps,
                codeParams.symbolSizeBits) \
            .salsa().encoder()
        cppMapper = wireless.LinearMapper(codeParams.symbolSizeBits,
                                           codeParams.transmittedPointPrecisionBits)
        cppEncoderSymbols = wireless.vectorus()
        cppMapperSymbols = wireless.vectori()
        
        # Encode with C++ encoder
        cppEncoder.setPacket(packet)
        cppEncoder.encode(cppSpineValueIndices,
                          cppEncoderSymbols)
        cppMapper.process(cppEncoderSymbols, cppMapperSymbols)
        
        # Decode with C++ decoder
        cppDecoder = spinal.CodeFactory(
                codeParams.numBitsPerCodingStep,
                numCodingSteps,
                codeParams.symbolSizeBits) \
            .salsa() \
            .linear(codeParams.transmittedPointPrecisionBits) \
            .beamDecoder(BEAM_WIDTH,
                         NUM_PASSES,
                         NUM_PASSES * NUM_LAST_CODE_STEP_SYMBOLS)
        cppDecoder.add(cppSpineValueIndices,
                       cppMapperSymbols)
        decodedPacket = cppDecoder.decode()
        
        self.assertEquals(decodedPacket.packet, packet)

    def test_004_DecoderIntermediateState(self):
        NUM_CODING_STEPS = 10
        BEAM_WIDTH = 16
        codeParams = spinal.reference.CodeParams(salsaNumRounds = 12,
                                         hashWordSize = 64,
                                         numBitsPerCodingStep = 3,
                                         symbolSizeBits = 10,
                                         transmittedPointPrecisionBits = 16,
                                         lookaheadDepth = 0)
        
        cppDecoder = spinal.CodeFactory(
                        codeParams.numBitsPerCodingStep,
                        NUM_CODING_STEPS,
                        codeParams.symbolSizeBits) \
                    .salsa() \
                    .linear(codeParams.transmittedPointPrecisionBits) \
                    .beamDecoder(BEAM_WIDTH) \
                    .decoder()

        cppDecoder.initialize()
        
        pyDecoder = spinal.reference.Decoder(codeParams, BEAM_WIDTH)
        
        for step in xrange(NUM_CODING_STEPS):
            # decide on number of symbols
            numSymbols = numpy.random.randint(1,100)
            
            # decide on symbols
            symbols = numpy.random.randint(int(-0.1 * (1 << codeParams.transmittedPointPrecisionBits)),
                                           int(1.1 * (1 << codeParams.transmittedPointPrecisionBits)),
                                           numSymbols).astype(numpy.int32)
            
            # Feed into both decoders
            cppDecoder.doCodeStep(len(symbols), step, symbols)
            pyDecoder.doRound(symbols)
            
            # Get list from the decoder suggestions
            cppInterm = spinal.IntermediateVector()
            cppDecoder.getIntermediate(cppInterm)
            suggList = [(interm.weight, interm.spineValue) for interm in list(cppInterm)]
            
            # Compare results
            self.assertEquals(suggList, sorted(pyDecoder.suggestions))
                                

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testEncoder']
    unittest.main()