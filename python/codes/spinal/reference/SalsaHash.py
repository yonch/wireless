# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

def upsample32(x,y):
    return (x << 32) | y

##
# \ingroup spinal_reference
# \brief Salsa hash function
def salsaHash(prevState, data, numRounds, HASH_WORD_SIZE = 32):
    '''
    A reference of our salsa hash, that could be used to verify return values
    '''
    HASH_MASK = 0xFFFFFFFF
    import struct
    def PLUS(x,y):
        return ((x+y) & HASH_MASK)
    def ROTATE(x, c):
        return (((x << c) | (x >> (32 - c))) & HASH_MASK)
    def XOR(x,y):
        return (x ^ y)
    tau = "expand 16-byte k";

    j6 = data;
    j7 = data;
    j8 = 0;
    j9 = 0;

    j1 = j11 = (prevState >> 0) & 0xFFFF;
    j2 = j12 = (prevState >> 16) & 0xFFFF;
    j3 = j13 = (prevState >> 32) & 0xFFFF;
    j4 = j14 = (prevState >> 48) & 0xFFFF;

    j0 = struct.unpack("I", tau[0:4])[0];
    j5 = struct.unpack("I", tau[4:8])[0];
    j10 = struct.unpack("I", tau[8:12])[0];
    j15 = struct.unpack("I", tau[12:16])[0];

    x0 = j0;
    x1 = j1;
    x2 = j2;
    x3 = j3;
    x4 = j4;
    x5 = j5;
    x6 = j6;
    x7 = j7;
    x8 = j8;
    x9 = j9;
    x10 = j10;
    x11 = j11;
    x12 = j12;
    x13 = j13;
    x14 = j14;
    x15 = j15;
    for i in xrange(0,numRounds,2):
        x4 = XOR( x4,ROTATE(PLUS( x0,x12), 7));
        x8 = XOR( x8,ROTATE(PLUS( x4, x0), 9));
        x12 = XOR(x12,ROTATE(PLUS( x8, x4),13));
        x0 = XOR( x0,ROTATE(PLUS(x12, x8),18));
        x9 = XOR( x9,ROTATE(PLUS( x5, x1), 7));
        x13 = XOR(x13,ROTATE(PLUS( x9, x5), 9));
        x1 = XOR( x1,ROTATE(PLUS(x13, x9),13));
        x5 = XOR( x5,ROTATE(PLUS( x1,x13),18));
        x14 = XOR(x14,ROTATE(PLUS(x10, x6), 7));
        x2 = XOR( x2,ROTATE(PLUS(x14,x10), 9));
        x6 = XOR( x6,ROTATE(PLUS( x2,x14),13));
        x10 = XOR(x10,ROTATE(PLUS( x6, x2),18));
        x3 = XOR( x3,ROTATE(PLUS(x15,x11), 7));
        x7 = XOR( x7,ROTATE(PLUS( x3,x15), 9));
        x11 = XOR(x11,ROTATE(PLUS( x7, x3),13));
        x15 = XOR(x15,ROTATE(PLUS(x11, x7),18));
        x1 = XOR( x1,ROTATE(PLUS( x0, x3), 7));
        x2 = XOR( x2,ROTATE(PLUS( x1, x0), 9));
        x3 = XOR( x3,ROTATE(PLUS( x2, x1),13));
        x0 = XOR( x0,ROTATE(PLUS( x3, x2),18));
        x6 = XOR( x6,ROTATE(PLUS( x5, x4), 7));
        x7 = XOR( x7,ROTATE(PLUS( x6, x5), 9));
        x4 = XOR( x4,ROTATE(PLUS( x7, x6),13));
        x5 = XOR( x5,ROTATE(PLUS( x4, x7),18));
        x11 = XOR(x11,ROTATE(PLUS(x10, x9), 7));
        x8 = XOR( x8,ROTATE(PLUS(x11,x10), 9));
        x9 = XOR( x9,ROTATE(PLUS( x8,x11),13));
        x10 = XOR(x10,ROTATE(PLUS( x9, x8),18));
        x12 = XOR(x12,ROTATE(PLUS(x15,x14), 7));
        x13 = XOR(x13,ROTATE(PLUS(x12,x15), 9));
        x14 = XOR(x14,ROTATE(PLUS(x13,x12),13));
        x15 = XOR(x15,ROTATE(PLUS(x14,x13),18));

    x0 = PLUS(x0,j0);
    x1 = PLUS(x1,j1);
    x2 = PLUS(x2,j2);
    x3 = PLUS(x3,j3);
    x4 = PLUS(x4,j4);
    x5 = PLUS(x5,j5);
    x6 = PLUS(x6,j6);
    x7 = PLUS(x7,j7);
    x8 = PLUS(x8,j8);
    x9 = PLUS(x9,j9);
    x10 = PLUS(x10,j10);
    x11 = PLUS(x11,j11);
    x12 = PLUS(x12,j12);
    x13 = PLUS(x13,j13);
    x14 = PLUS(x14,j14);
    x15 = PLUS(x15,j15);

    newState = ((x0 ^ x4 ^ x8 ^ x12 ^ x2 ^ x6 ^ x10 ^ x14) & HASH_MASK) | \
            (((x1 ^ x5 ^ x9 ^ x13 ^ x3 ^ x7 ^ x11 ^ x15) & HASH_MASK) << 32)
        
    return newState, [upsample32(x1,x0), upsample32(x3,x2),
                      upsample32(x5,x4), upsample32(x7,x6),
                      upsample32(x9,x8), upsample32(x11,x10),
                      upsample32(x13,x12), upsample32(x15,x14)]

def unlimitedHash(prevState, data, updateBufferLength, numRounds, HASH_WORD_SIZE = 32):
    """ returns the updateBuffer at least as big as requested, by hashing more 
        times if necessary """
    
    newState, updateBuffer = salsaHash(prevState, data, numRounds, HASH_WORD_SIZE)
    for i in xrange(8, updateBufferLength, 8):
        updateBuffer += salsaHash(prevState, ((i / 8) << 16) + data, numRounds, HASH_WORD_SIZE)[1]
    return newState, updateBuffer
    