# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief LDPC factory 
class LdpcFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'ldpc':
            return None
        
        if codeSpec['n'] != 648:
            raise RuntimeError, "packet size currently unsupported"
        rateNumerator, rateDenominator = codeSpec['rate']
        code = wireless.codes.ldpc.getWifiLDPC648(rateNumerator, rateDenominator)
        
        encoder = wireless.codes.ldpc.MatrixLDPCEncoder(
                                                code, 
                                                codeSpec['bitsPerSymbol'])
        return encoder, wireless.vectorus

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if decodeSpec['type'] != 'ldpc-float-bp':
            return None
        
        if codeSpec['n'] != 648:
            raise RuntimeError, "packet size currently unsupported"
        rateNumerator, rateDenominator = codeSpec['rate']
        code = wireless.codes.ldpc.getWifiLDPC648(rateNumerator, rateDenominator)
        
        return wireless.codes.ldpc.MatrixLDPCDecoder(code, decodeSpec['numIter'])
