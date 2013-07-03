# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief LT encoders/decoders factory 
class LTFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'LT':
            return None
        
        neighborGen = wireless.codes.fountain.LTParityNeighborGenerator(packetLength, 0xdeadbeef)
        symbolFunc = wireless.util.hashes.BitwiseXorSymbolFunction()
        encoder = wireless.codes.fountain.LTEncoder(packetLength, neighborGen, symbolFunc)
        return encoder, wireless.vectorus

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if decodeSpec['type'] != 'LT':
            return None
        
        decoder = wireless.codes.fountain.LTDecoder(packetLength, 
                                                        2 * packetLength, 
                                                        decodeSpec['numIter'])
        return decoder
