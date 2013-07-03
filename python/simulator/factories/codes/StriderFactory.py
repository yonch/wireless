# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Strider encoders and decoders 
class StriderFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'strider':
            return None
        
        if codeSpec.has_key('fragmentLength'):
            encoder = wireless.codes.strider.StriderFactory.createEncoder(codeSpec['fragmentLength'])
        else:
            encoder = wireless.codes.strider.StriderFactory.createEncoder(1530)
        return encoder, wireless.vector_csymbol

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if decodeSpec['type'] != 'strider':
            return None
        
        if codeSpec.has_key('fragmentLength'):
            return wireless.codes.strider.StriderFactory.createDecoder(codeSpec['fragmentLength'])
        else:
            return wireless.codes.strider.StriderFactory.createDecoder(1530)
