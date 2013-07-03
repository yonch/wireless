# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless

##
# \ingroup factories
# \brief Mappers factory 
class MapFactory(object):
    def make_mapper(self, spec):
        if spec['type'] == 'linear':
            mapper = wireless.mappers.LinearMapper(spec['bitsPerSymbol'],
                                            spec['precisionBits'])
            return mapper, wireless.vector_symbol, mapper.getAveragePower()
        elif spec['type'] == 'gray':
            mapper = wireless.mappers.GrayMapper(spec['bitsPerSymbol'],
                                          spec['precisionBits'])
            return mapper, wireless.vector_symbol, mapper.getAveragePower()
        elif spec['type'] == 'trunc-norm-v2':
            mapper = wireless.mappers.GaussianMapper(spec['bitsPerSymbol'],
                                              spec['precisionBits'],
                                              spec['numStandardDevs'])
            return mapper, wireless.vector_symbol, mapper.getAveragePower()
        elif spec['type'] == 'QPSK':
            mapper = wireless.mappers.QPSKMapper()
            return mapper, wireless.vector_csymbol, mapper.getAveragePower()
        elif spec['type'] == 'QAM':
            mapper = wireless.mappers.QamMapper(spec['bitsPerSymbol'])
            return mapper, wireless.vector_csymbol, mapper.getAveragePower()
        elif spec['type'] == 'null':
            return None, None, spec['avgPower']
        elif spec['type'] == 'soft':
            mapper = wireless.mappers.SoftMapper(spec['bitsPerSymbol'])
            return mapper, wireless.vector_softsymbol, mapper.getAveragePower()
        elif spec['type'] == 'complex-linear':
            mapper = wireless.mappers.ComplexLinearMapper(spec['bitsPerDimension'])
            return mapper, wireless.vector_csymbol, mapper.getAveragePower()
        else:
            return None
