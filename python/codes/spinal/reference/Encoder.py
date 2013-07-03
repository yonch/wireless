# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
from Hash import hash_func
from RNG import RNG

##
# \ingroup spinal_reference
# \brief Spinal encoder
class Encoder(object):
    """
    Spinal encoder.
    Encodes the given message in a rateless manner, producing constellation
       points as output.
       
    After instantiating an encoder, the get_symbol() method is called, with the
       index of the spine value to extract the next symbol from. This allows 
       arbitrary puncturing schedules.
    """

    def __init__(self, k, map_func, message):
        """
        Constructor
        @param k the number of bits in each block fed into the hash function
        @param map_func a function that maps a 16-bit pseudo-random number
             into a constellation point
        @param message a string with the message to be transmitted
        """
        self.k = k

        self.map_func = map_func
        
        # divide the mesage into blocks of k bits
        self.message_blocks = self._divide_message_into_blocks(message, self.k)
        
        # calculate the spine
        self.spine = []
        spine_value = 0
        for block in self.message_blocks:
            # Calculate the next spine value
            spine_value = hash_func(spine_value, block)
            
            # Add the new value to the spine
            self.spine.append(spine_value)
        
        # Make RNGs for each spine value
        self.rngs = [RNG(spine_value) for spine_value in self.spine]
        
    def get_symbol(self, i):
        '''
        Gets the next symbol from the i'th spine value
        '''
        # Use the suitable RNG to get the next symbol.
        rng_output = self.rngs[i].next()
        
        # map into a symbol, and return
        return self.map_func(rng_output) 
            

    #######################
    # internal functions
    #######################
    def _divide_message_into_blocks(self, message, k):
        '''
        Divides the given message into blocks of k bits.
        '''
        # Convert the message into a large integer, with first byte as LSB,
        # last byte as MSB
        reversed_message = "".join(reversed(list(message)))
        message_as_num = int(reversed_message.encode('hex'),16)
    

        # mask will extract the lowest k bits from a number
        mask = (1 << k) - 1
        
        # n is number of message bits
        n = 8 * len(message)

        # calculate how many blocks the message contains
        num_blocks = (n + (k - 1)) / k
        
        # extract the blocks
        blocks = []
        for block_index in xrange(num_blocks):
            message_block = (message_as_num >> (block_index * k)) & mask
            blocks.append(message_block)
            
        return blocks
