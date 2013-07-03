# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless

##
# \ingroup factories
# \brief Packet generator factory
class PacketGenFactory(object):
    def make_packet_generator(self, spec):
        if spec['type'] == 'random':
            return wireless.PacketGenerator(spec['length'])
        elif spec['type'] == 'crc16':
            return wireless.CrcPacketGenerator(spec['length'])
        else:
            return None
