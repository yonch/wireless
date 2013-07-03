# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless

##
# \ingroup factories
# \brief Demappers factory 
class DemapFactory(object):
    def make_demapper(self, mapSpec, demapSpec):
        if demapSpec['type'] == 'null':
            return None, None
        elif demapSpec['type'] == 'ml-gray':
            mapper = wireless.mappers.GrayMapper(mapSpec['bitsPerSymbol'],
                                          mapSpec['precisionBits'])
            
#            SNR_dB = demapSpec['SNR_dB']
#            SNR_ratio = math.pow(10.0, SNR_dB/10.0)
#            noiseAveragePower = mapper.getAveragePower() / SNR_ratio
#            stddev = math.sqrt(noiseAveragePower)
            
            return (wireless.demappers.GrayDemapper(mapper, 
                                           mapSpec['bitsPerSymbol']), 
                    wireless.vectorf)
        elif demapSpec['type'] == 'BSC':
            return (wireless.demappers.BscDemapper(demapSpec['flipProb']),
                    wireless.vectorf)
        elif demapSpec['type'] == 'QAM':
            if mapSpec['type'] != 'QAM':
                raise RuntimeError,'Only QAM mapper is supported with QAM demapper at this time'
            if not demapSpec.has_key('useApprox'): 
                demapSpec['useApprox'] = False
            itppDemapper = wireless.itpp.QAM(2 ** mapSpec['bitsPerSymbol'])
            return (wireless.demappers.ItppComplexDemapper(itppDemapper, mapSpec['bitsPerSymbol'], demapSpec['useApprox']),
                    wireless.vector_llr)
        else:
            return None
