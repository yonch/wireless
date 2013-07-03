# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

from wireless.statistics import ErrorRateStatistics, ErrorLocationStatistics, FirstErrorStatistics

##
# \ingroup factories
# \brief Statistics factory 
class StatisticsFactory(object):
    def make_statistics(self, spec):
        if spec['type'] == 'errors':
            return ErrorRateStatistics(1)
        elif spec['type'] == 'bit-statistics':
            return ErrorLocationStatistics(1)
        elif spec['type'] == 'first-error':
            return FirstErrorStatistics(1)
        else:
            return None
