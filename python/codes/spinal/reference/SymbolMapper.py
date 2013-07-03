# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

##
# \ingroup spinal_reference
# \brief Mapper from hash outputs to symbols
class SymbolMapper(object):
    """
    Maps a 16-bit integer into a constellation point.
    """
    
    def __init__(self, c, precision):
        """
        Constructor.
        @param c the number of bits to extract from the pseudo random input
        @param precision how many total bits should the constellation point 
             contain. This allows all computation to be done with integer 
             arithmetic, treating noise as quantized to given precision.
        """
        self.c = c
        self.precision = precision
    
    def map(self, value):
        """
        @param value a 16-bit pseudo random number, to be mapped into a constellation point
        """
        # calculate a bitmask to extract only the lowest c bits
        mask = (1 << self.c) - 1
        
        # Get only the lowest c bits
        bits = value & mask

        # "map" bits into a constellation point by adding precision
        addedPrecisionBits = self.precision - self.c
        withAddedPrecision = bits << addedPrecisionBits
        
        return withAddedPrecision

    def get_signal_average_power(self):
        """
        @return the average power of the output signal, given the inputs are
           uniform (pseudo-random inputs are indeed uniform)
        """
        # N = number of constellation points
        N = (1 << self.c)
        
        # encoder power is the second moment of uniform discrete distribution
        # around its mean (see http://mathworld.wolfram.com/DiscreteUniformDistribution.html)
        encoder_power_without_added_precision = (N - 1)*(N + 1) / 12.0
        
        # need to add precision. Since the average is over the signal squared,
        # we need to add square the precision
        added_precision_bits = self.precision - self.c
        encoder_power = encoder_power_without_added_precision * (1 << (2 * added_precision_bits))
        
        return encoder_power
    