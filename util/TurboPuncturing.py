"""
This file contains functions to convert CML style puncturing configurations
for turbo codes to "interleaving sequences" good for the IT++ implementation
"""

import numpy as np

def getInterleaving(msg_len, num_g1, num_g2, constraint_length, punc1, punc2, 
                    tail1, tail2):
    """
    Returns, a list where there is an element for each bit in the punctured
        output seqeuence, where in the IT++ sequence it was taken from
    @param msg_len: the length of the message being encoded
    @param num_g1: the number of generators in the first encoder
    @param num_g2: the number of generators in the second encoder
    @param constraint_length: the constraint length of the code. There will be
        constraint_length - 1 bits in the tail of each encoder
    @param punc1: a matrix specifying the puncturing schedule of non-tail bits
        of the first encoder
    @param punc2: a matrix specifying the puncturing schedule of non-tail bits
        of the second encoder
    @param tail1: a matrix specifying the puncturing schedule of tail bits
        of the first encoder
    @param tail2: a matrix specifying the puncturing schedule of tail bits
        of the second encoder
    """
    
    sequence = []
    assert(punc1.shape[0] == num_g1)
    assert(punc2.shape[0] == num_g2)
    assert(tail1.shape == (num_g1, num_g1))
    assert(tail2.shape == (num_g2, num_g2))
    N = num_g1 + num_g2 - 1
    
    # the IT++ encoder punctures the systematic bits from second encoder
    assert (punc2[0,:] == 0).all()
    
    heartPunc = np.concatenate((punc1, punc2[1:,:]), axis = 0)
    # take column after column, to produce the sequence
    heartPunc = np.reshape(heartPunc, -1, order='F')
    
    # reshape the tail vectors
    tailPunc1 = tail1.reshape(-1, order='F')
    tailPunc2 = tail2.reshape(-1, order='F')
    
    # number of tail bits
    kk = constraint_length - 1
    
    # get heart's interleaving sequence
    for i in xrange(N * msg_len):
        sequence.extend([i] * heartPunc[i % heartPunc.size])
    
    # get tail's interleaving sequence. this should take care of the different
    # ordering of tail bits between CML and IT++, where CML puts takes a tail 
    # bit from every generator in both encoders and then the next tail bit, and  
    # CML first takes every bit from first encoder then every bit from second.
    for i in xrange(kk):
        # take the few bits from first tail
        for j in xrange(num_g1):
            sequence.extend([(N * msg_len) + (i * num_g1) + j] * tailPunc1[(i * num_g1 + j) % tailPunc1.size])
            
        # take the few bits from second tail
        for j in xrange(num_g2):
            sequence.extend([(N * msg_len) + (kk * num_g1) + (i * num_g2) + j] * tailPunc2[(i * num_g2 + j) % tailPunc2.size])
    
    return sequence

def getCDMA2000(scenario):
    """
    Gets the output puncturing/interleaving sequence for CDMA2000's scenario as 
        declared in CML's Cdma2000Scenarios.m 
    """
    if scenario in [1,2,3,4]:
        return getInterleaving(msg_len=12282, num_g1=3, num_g2=3,
                               constraint_length=4,
                               punc1 = np.array(np.mat('1 1; 1 1; 1 0')),
                               punc2 = np.array(np.mat('0 0; 0 1; 1 1')),
                               tail1 = np.array(np.mat('2 2 2; 1 1 1; 1 1 1')),
                               tail2 = np.array(np.mat('2 2 2; 1 1 1; 1 1 1')))
    elif scenario == 5:
        return getInterleaving(msg_len=1530, num_g1=3, num_g2=3,
                               constraint_length=4,
                               punc1 = np.array(np.mat('1 1; 1 1; 1 1')),
                               punc2 = np.array(np.mat('0 0; 1 1; 1 1')),
                               tail1 = np.array(np.mat('3 3 3; 1 1 1; 1 1 1')),
                               tail2 = np.array(np.mat('3 3 3; 1 1 1; 1 1 1')))
    elif scenario == 6:
        return getInterleaving(msg_len=1530, num_g1=3, num_g2=3,
                               constraint_length=4,
                               punc1 = np.array(np.mat('1 1; 1 1; 1 0')),
                               punc2 = np.array(np.mat('0 0; 0 1; 1 1')),
                               tail1 = np.array(np.mat('2 2 2; 1 1 1; 1 1 1')),
                               tail2 = np.array(np.mat('2 2 2; 1 1 1; 1 1 1')))
    elif scenario == 7:
        return getInterleaving(msg_len=1530, num_g1=2, num_g2=2,
                               constraint_length=4,
                               punc1 = np.array(np.mat('1 1; 1 1')),
                               punc2 = np.array(np.mat('0 0; 1 1')),
                               tail1 = np.array(np.mat('2 2 2; 1 1 1')),
                               tail2 = np.array(np.mat('2 2 2; 1 1 1')))
    elif scenario == 8:
        return getInterleaving(msg_len=1530, num_g1=2, num_g2=2,
                               constraint_length=4,
                               punc1 = np.array(np.mat('1 1; 1 0')),
                               punc2 = np.array(np.mat('0 0; 0 1')),
                               tail1 = np.array(np.mat('1 1 1; 1 1 1')),
                               tail2 = np.array(np.mat('1 1 1; 1 1 1')))
    