# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

from wireless.statistics import ErrorRateStatistics

class StatisticsFactory(object):
    def make_statistics(self, spec):
        if spec['type'] == 'errors':
            return ErrorRateStatistics(1)
        else:
            return None
