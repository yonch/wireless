# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Turbo codes encoders and decoders
class TurboFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'turbo':
            return None
        
        encoder = wireless.codes.turbo.TurboEncoder(packetLength)
        return encoder, wireless.vectorus

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if codeSpec['type'] != 'turbo':
            return None
        
        if decodeSpec['type'] != 'regular':
            raise RuntimeError, 'Unknown decoder type, only regular decoder is known'
    
        decoder = wireless.codes.turbo.TurboDecoder(packetLength)
    
        return decoder
