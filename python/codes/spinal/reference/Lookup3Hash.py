# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).
import numpy

##
# \ingroup spinal_reference
# \brief Initializes lookup3 state
def lookup3Init(val):
    return numpy.array([0xdeadbeef + val] * 3, dtype=numpy.uint32);


##
# \ingroup spinal_reference
# \brief Updates lookup3 state
def lookup3Update(state, data):
    def rot(x,k):
        return (((x) << (k)) | ((x) >> (32-(k))))
    state[1] += data
    state[2] ^= state[1]; state[2] -= rot(state[1],14); \
    state[0] ^= state[2]; state[0] -= rot(state[2],11); \
    state[1] ^= state[0]; state[1] -= rot(state[0],25); \
    state[2] ^= state[1]; state[2] -= rot(state[1],16); \
    state[0] ^= state[2]; state[0] -= rot(state[2],4);  \
    state[1] ^= state[0]; state[1] -= rot(state[0],14); \
    state[2] ^= state[1]; state[2] -= rot(state[1],24); \

##
# \ingroup spinal_reference
# \brief Returns digest from lookup3 state
def lookup3Digest(state):
    return state[2];
