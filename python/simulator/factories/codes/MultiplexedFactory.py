# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Encoder instances that produce symbols from multiple encoders
class MultiplexedFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'multiplexed':
            return None
        
        numEncoders = codeSpec['numEncoders']
        subPacketLength = packetLength / numEncoders
        encoders = [EncoderFactory._make(codeSpec['spec'], subPacketLength)[0] for i in xrange(numEncoders)]
        encodersVec = wireless.codes.vector_encoder()
        for e in encoders:
            encodersVec.push_back(e)
        lengthsVec = wireless.vectorui([subPacketLength] * numEncoders)
        
        encoder = wireless.codes.EncoderMultiplexer(encodersVec, lengthsVec)
        return encoder, wireless.vectorus
