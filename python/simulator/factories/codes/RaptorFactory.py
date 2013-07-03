# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Raptor encoders and decoders 
class RaptorFactory(object):
    @staticmethod
    def make_encoder(codeSpec, packetLength):
        if codeSpec['type'] != 'raptor':
            return None
        
        if packetLength in [9500, 1024, 2048, 3072, 4096, 256]:
            import os.path
            dirname = wireless.util.config.get_data_dir()
            filename = os.path.join(dirname, 'ldpc', 'LDPC_%d.it' % packetLength)
            encoder = wireless.codes.fountain.RaptorEncoder(filename)
            return encoder, wireless.vectorus
        else:
            raise RuntimeError, "Unsupported packet size %d" % packetLength

    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if codeSpec['type'] != 'raptor':
            return None
        
        if packetLength in [9500, 1024, 2048, 3072, 4096, 256]:
            import os.path
            dirname = wireless.util.config.get_data_dir()
            filename = os.path.join(dirname, 'ldpc', 'LDPC_%d.it' % packetLength)
            decoder = wireless.codes.fountain.RaptorDecoder(filename,
                                                                decodeSpec['numIter'])
            return decoder
        else:
            raise RuntimeError, "Unsupported packet size %d" % packetLength
