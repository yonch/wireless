# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
from Hash import hash_func
from RNG import RNG

##
# \ingroup spinal_reference
# \brief Spinal decoder
class Decoder(object):
    """
    Spinal decoder.
    
    To use this decoder to decode a message with a spine with n/k values, call
        advance() n/k times. On the i'th time, supplied parameter should be 
        a list of all symbols derived from the i'th spine value (ie using 
        encoder.get_symbol(i)).
         
    @note see advance() for assumption on inputs.
    """
    
    def __init__(self, k, B, d, map_func):
        """
        Constructor
        @param k the number of bits in each block fed into the hash function
        @param B beam width: the maximum number of sub-trees that the decoder
            keeps when advancing to next spine value
        @param d "depth": the number of layers in each of the sub-trees that
            the decoder keeps when advancing to next spine value
        @param map_func a function that maps a 16-bit pseudo-random number
             into a constellation point
        """
        self.k = k
        self.B = B
        self.d = d
        self.map_func = map_func     
        
        # self.wavefront is a list of tuples for each node in the wavefront.
        # Each tuple contains (path_metric, spine_value, path)
        #   where:
        #      path_metric is the sum of all edge weights from the root to the node
        #      spine_value is the spine value corresponding to the node
        #      path is a list of edges traversed from the root to the node.
        #         Each element in the list is a k-bit number (corresp. to the edge) 
        self.wavefront = [(0, 0, []) ]
        
    def advance(self, symbols):
        """
        Advances the decoder to the next spine value, calculating weights using
            the given symbols. The given symbols can be noisy.

        @note The symbols supplied to advance() should be in the same order as 
            they were produced: 
            if the program ran
                a = enc.get_symbol(0)
                b = enc.get_symbol(0)
            then the first call to advance should be advance([a,b]), 
                not advance([b,a]).
        """
        self._expand_wavefront(symbols)
        self._prune_wavefront()
    
    def get_most_likely(self):
        """
        @return a string, the most likely message to have been transmitted from 
            the wavefront.
        """
        # get the node with the smallest path metric. Note that this code breaks
        #   ties in favor of nodes with smaller spine value.
        path_metric, spine_value, path = min(self.wavefront)
        
        # join the path, first element makes LSB, last element makes MSB
        message_as_number = 0
        for block in reversed(path):
            message_as_number = (message_as_number << self.k) | block
        
        # transform the integer representation into the binary message
        n = (len(path) * self.k + 7) / 8
        message = ""
        for i in xrange(n):
            message += chr(message_as_number & 0xFF)
            message_as_number >>= 8

        return message 

    
    #######################
    # internal functions
    #######################
    def _expand_wavefront(self, symbols):
        """
        Given the wavefront of the current spine location, and symbols produced
        from the next spine location, advance the wavefront to contain spine 
        values from the next spine location.
        """
        
        k = self.k
        
        new_wavefront = []
        # For each node in the current wavefront
        for (path_metric, spine_value, path) in self.wavefront:
            # For each possible message block (2^k options)
            for edge in xrange( 1 << k ):
                # Calculate the new spine value
                new_spine_value = hash_func(spine_value, edge)
                
                # Get an RNG for the new spine value
                rng = RNG(new_spine_value)
                
                # Go over all received symbols, and compute the edge metric
                edge_metric = 0
                for received_symbol in symbols:
                    # What the transmitter would have produced if it had this spine value
                    node_symbol = self.map_func(rng.next())
                    
                    # Add the distance squared to the edge metric
                    distance = received_symbol - node_symbol
                    edge_metric += distance * distance
                    
                # The new path metric is the sum of all edges from the root
                new_path_metric = path_metric + edge_metric
                
                # Add new node to wavefront
                new_wavefront.append( (new_path_metric, new_spine_value, path + [edge]) )
        
        # Update the wavefront to the newly computed wavefront
        self.wavefront = new_wavefront

    def _prune_wavefront(self):
        """
        Given a wavefront with several sub-trees, prune to only keep the best B
            sub-trees
        """
        
        k = self.k
        d = self.d
        B = self.B
        
        # We prune in the granularity of sub-trees, each one has d layers.
        # The leaves of these sub-trees are in the wavefront. 
        # Each subtree has 2^(k(d-1)) such leaves in the wavefront.
        num_subtree_nodes = (1 << (k * (d - 1)))
        
        # Split nodes according to subtree.
        # subtrees is a list, each of its components is a list 2^(k(d-1)) nodes
        subtrees = []
        while (len(self.wavefront) > 0):
            # add the first 2^(k(d-1)) nodes to 'subtrees'
            subtrees.append(self.wavefront[:num_subtree_nodes])
            # remove the first 2^(k(d-1)) nodes from 'wavefront'
            self.wavefront = self.wavefront[num_subtree_nodes:]
        
        # sort subtrees according to the minimum path metric of nodes in the
        # subtree
        subtrees.sort(key = lambda x: min(y[0] for y in x))

        # sanity check
        if len(subtrees) > B and \
            min(y[0] for y in subtrees[B-1]) == min(y[0] for y in subtrees[B]):
            print "Warning: two pruned subtrees have the same score. Breaking the tie arbitrarily."
        
        # take only <= B sub-trees with best scores
        subtrees = subtrees[:B]
        
        # Set wavefront with all nodes in the retained sub-trees
        self.wavefront = reduce(lambda x,y:x+y, subtrees)    
    
