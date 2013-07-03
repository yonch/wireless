# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless

from wireless.protocols import OneTryProtocol
from wireless.protocols import MultipleTryProtocol
from wireless.protocols import RateApproxProtocol

##
# \ingroup factories
# \brief Link-layer protocol factory
class ProtocolFactory(object):
    def make_protocol(self, protoSpec, codeSpec, packetLength):
        if protoSpec['type'] == 'one-try':
            return OneTryProtocol(1,
                                  protoSpec['numSymbols'])
        elif protoSpec['type'] == 'rate-approx':
            minSymbols = protoSpec.get('minSymbols',1)
            return RateApproxProtocol(1,
                                       protoSpec['maxSymbols'],
                                       protoSpec['delta'],
                                       minSymbols)
        elif protoSpec['type'] == 'multiple-try':
            return MultipleTryProtocol(1,
                                  protoSpec['numSymbolsList'])
        else:
            return None
