# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

##
# \ingroup spinal_reference
# \brief A pseudo random number generator based on the hash specified in the Hash class.
class RNG(object):
    '''
    A pseudo random number generator based on the hash specified in the Hash class.
    
    The RNG is initialized with an integer seed, and produces 16-bit pseudo
       random numbers using the next() method.
       
    Internally, the RNG uses a hash function to generate outputs. The hash 
       function produces 32-bit numbers, and both upper 16-bits and lower 
       16-bits are used as outputs. As an optimization, the seed is also used
       as the output of the hash, saving one application of the hash function.
    '''

    def __init__(self, seed):
        '''
        Constructor
        @param seed: the seed of the RNG.
        '''
        self.seed = seed
        self.next_values = [(seed >> 16) & 0xFFFF, seed & 0xFFFF]
        self.i = 3610617884

        
    def next(self):
        '''
        Returns the next 16-bit value from the RNG
        '''
        # If there are no values ready to be returned, produce new values
        if len(self.next_values) == 0:
            self._produce_more_values()
            
        # dequeue next values
        val = self.next_values[0]
        self.next_values = self.next_values[1:]
        
        return val
    
    #######################
    # internal functions
    #######################
    def _produce_more_values(self):
        '''
        helper function, produces the next values to be output from the RNG, and
            adds them to self.next_values. This function should only be called 
            from within the RNG class
        '''
        from Hash import hash_func
        
        # Hash the seed with the current index
        hash_output = hash_func(self.seed, self.i)
        
        # Get two 16-bit words from the state
        self.next_values = self.next_values + [(hash_output >> 16) & 0xFFFF, hash_output & 0xFFFF]
        
        # increment current index
        self.i = (self.i + 3243335647) & ((1 << 32) - 1)
