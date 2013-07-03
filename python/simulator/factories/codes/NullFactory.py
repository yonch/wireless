# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Null encoder/decoder factories
class NullFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'null':
            return None
        
        return wireless.codes.null.NullEncoder(packetLength), wireless.vectorus

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if codeSpec['type'] != 'null':
            return None
        
        return wireless.codes.null.NullDecoder(packetLength)
