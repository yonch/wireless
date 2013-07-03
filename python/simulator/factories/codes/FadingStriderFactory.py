# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import wireless

##
# \ingroup factories
# \brief Factory to strider decoders over a fading channel
class FadingStriderFactory(object):
    @staticmethod
    def make_decoder(codeSpec, packetLength, decodeSpec, mapSpec, channelSpec):
        if decodeSpec['type'] != 'strider-fading':
            return None
        
        if channelSpec['type'] != 'coherence-fading_c':
            raise RuntimeError, "Strider fading channel currently only supports fading over complex numbers"
        return wireless.codes.strider.StriderFactory.createFadingDecoder(1530)
