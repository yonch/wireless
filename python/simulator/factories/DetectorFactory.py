# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

import wireless

##
# \ingroup factories
# \brief Detectors factory
class DetectorFactory(object):
    def make_detector(self, detectorSpec, codeSpec, packetLength, avgPower):
        if detectorSpec['type'] == 'oracle':
            return wireless.DefaultOracleDetector(1)
        elif detectorSpec['type'] == 'chi2':
            if codeSpec['type'] != 'spinal':
                raise RuntimeError, "chi2 detection only supported for spinal codes"

            from Chi2Detector import Chi2Detector
            from common import spinalGetSpineLength
            from common import spinalGetPuncturing
            
            spineLength = spinalGetSpineLength(codeSpec, packetLength)
            puncturing = spinalGetPuncturing(codeSpec, spineLength)
            
            return Chi2Detector(
                           1,
                           detectorSpec['detectionError'],
                           spineLength,
                           packetLength,
                           puncturing,
                           avgPower)
        elif detectorSpec['type'] == 'crc16':
            return wireless.DefaultCrcDetector(1)
        else:
            return None
