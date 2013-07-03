# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

##
# \ingroup spinal_reference
# \brief  Hashes together two integers, s and m.
def hash_func(s, m):
    '''
    Hashes together two integers, s and m.
    @return a 32-bit integer, result of the hash.
    '''
    from Lookup3Hash import lookup3Init, lookup3Update, lookup3Digest
    
    # initialize with s
    hash_state = lookup3Init(s)
    
    # add m into hashed state
    lookup3Update(hash_state, m)
    
    # digest the hash and return the output value
    return lookup3Digest(hash_state)