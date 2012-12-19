
#
# This file is used to construct the sparse matrices needed for the MatrixLDPC
#   implementation, from the specifications in the 802.11n-2009 standard.
#

import numpy

def outputCpp(rowPtrs, cols, vals, n, rateNumerator, rateDenominator):
    print "#define CODE%d_RATE_%d_%d_NUM_ROWS %d" % (n, rateNumerator, rateDenominator, len(rowPtrs))
    print "#define CODE%d_RATE_%d_%d_NUM_VALUES %d" % (n, rateNumerator, rateDenominator, len(vals))
    print "const unsigned int code%d_rate_%d_%d_rowPtrs[CODE%d_RATE_%d_%d_NUM_ROWS] = { %s };" % \
        (n, rateNumerator, rateDenominator, n, rateNumerator, rateDenominator, ','.join(map(str,rowPtrs)))
    print "const unsigned int code%d_rate_%d_%d_cols[CODE%d_RATE_%d_%d_NUM_VALUES] = { %s };" % \
        (n, rateNumerator, rateDenominator, n, rateNumerator, rateDenominator, ','.join(map(str,cols)))
    print "const unsigned char code%d_rate_%d_%d_vals[CODE%d_RATE_%d_%d_NUM_VALUES] = { %s };" % \
        (n, rateNumerator, rateDenominator, n, rateNumerator, rateDenominator, ','.join(map(str,vals)))

def getMatrixRepresentation(values, shouldTranspose, n, Z, rateNumerator, rateDenominator):
    """
    @param values: a list of strings, ('-' for empty cell). These 
        represent the values
    @param shouldTranspose: if True, the values are given column by column  
        (first col1 from top to bottom, then col2 from top to bottom, etc).
        If False, values are given in rows.
    @param n: the number of coded bits (length of codeword)
    @param Z: the number of bits per matrix
    @param rateNumerator: the numerator of the rate (ie 3 for R=3/4)
    @param rateDenominator: the denominator of the rate (ie 4 for R=3/4)
    @return a tuple of lists (rowPtrs, values, columnIndices)
    
    """
    
    assert(n % Z == 0)
    assert(n % rateDenominator == 0)
    k = n * rateNumerator / rateDenominator
    
    nWords = n / Z
    kWords = k / Z
    
    def mapOne(val):
        if val=='-':
            return -1
        else:
            return int(val)
        
    # Map strings to values
    m = numpy.array([mapOne(x) for x in values], dtype = numpy.int32)
    print m
    
    # Shape into matrix
    if shouldTranspose:
        m = m.reshape([nWords,nWords - kWords]).transpose()
    else:
        m = m.reshape([nWords - kWords, nWords])
    print m

    # Disregard cyclic part
    m = m[:,0:kWords]
    print m
    
    rowNumNonempty = (m != -1).sum(1)
    rowPtrs = numpy.cumsum([0] + list(rowNumNonempty[:-1]))
    print rowPtrs
    
    cols = [(i % kWords) for i,v in enumerate(list(m.flatten())) if v!=-1]
    vals = [v for i,v in enumerate(list(m.flatten())) if v!=-1]
    print cols
    print numpy.array(cols)[rowPtrs]
    print vals
    
    outputCpp(list(rowPtrs), cols, vals, n, rateNumerator, rateDenominator)
    return list(rowPtrs), cols, vals

    
    
# 648, 1/2
s = '0\n22\n6\n2\n23\n24\n25\n13\n7\n11\n25\n3\n-\n0\n-\n-\n-\n-\n-\n24\n20\n-\n-\n-\n-\n-\n0\n-\n-\n23\n-\n-\n-\n-\n8\n-\n-\n-\n-\n0\n-\n1\n-\n-\n16\n-\n-\n-\n0\n17\n10\n20\n3\n17\n8\n0\n22\n19\n23\n16\n0\n-\n-\n-\n-\n-\n-\n-\n10\n-\n18\n-\n-\n0\n-\n-\n-\n3\n-\n8\n-\n-\n-\n-\n-\n0\n-\n-\n-\n-\n-\n-\n-\n-\n14\n2\n0\n12\n24\n25\n0\n10\n7\n6\n23\n13\n9\n25\n-\n-\n-\n0\n-\n-\n18\n-\n-\n-\n-\n5\n-\n-\n0\n-\n9\n-\n-\n-\n-\n3\n-\n-\n0\n-\n-\n-\n11\n-\n-\n-\n-\n17\n-\n-\n1\n-\n-\n-\n-\n-\n0\n-\n-\n-\n-\n1\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n-\n0\n0'
# 648, 2/3
s = '25 26 14 - 20 - 2 - 4 - - 8 - 16 - 18 1 0 - - - - - -\n10 9 15 11 - 0 - 1 - - 18 - 8 - 10 - - 0 0 - - - - -\n16 2 20 26 21 - 6 - 1 26 - 7 - - - - - - 0 0 - - - -\n10 13 5 0 - 3 - 7 - - 26 - - 13 - 16 - - - 0 0 - - -\n23 14 24 - 12 - 19 - 17 - - - 20 - 21 - 0 - - - 0 0 - -\n6 22 9 20 - 25 - 17 - 8 - 14 - 18 - - - - - - - 0 0 -\n14 23 21 11 20 - 24 - 18 - 19 - - - - 22 - - - - - - 0 0\n17 11 11 20 - 21 - 26 - 3 - - 18 - 26 - 1 - - - - - - 0'
# 648, 3/4
s = '16 17 22 24 9 3 14 - 4 2 7 - 26 - 2 - 21 - 1 0 - - - -\n25 12 12 3 3 26 6 21 - 15 22 - 15 - 4 - - 16 - 0 0 - - -\n25 18 26 16 22 23 9 - 0 - 4 - 4 - 8 23 11 - - - 0 0 - -\n9 7 0 1 17 - - 7 3 - 3 23 - 16 - - 21 - 0 - - 0 0 -\n24 5 26 7 1 - - 15 24 15 - 8 - 13 - 13 - 11 - - - - 0 0\n2 2 19 14 24 1 15 19 - 21 - 2 - 24 - 3 - 2 1 - - - - 0'
# 648, 5/6
s = '17 13 8 21 9 3 18 12 10 0 4 15 19 2 5 10 26 19 13 13 1 0 - -\n3 12 11 14 11 25 5 18 0 9 2 26 26 10 24 7 14 20 4 2 - 0 0 -\n22 16 4 3 10 21 12 5 21 14 19 5 - 8 5 18 11 5 5 15 0 - 0 0\n7 7 14 14 4 16 16 24 24 10 1 7 15 6 10 26 8 18 21 14 1 - - 0'
# 1296, 1/2
s = '40 - - - 22 - 49 23 43 - - - 1 0 - - - - - - - - - -\n50 1 - - 48 35 - - 13 - 30 - - 0 0 - - - - - - - - -\n39 50 - - 4 - 2 - - - - 49 - - 0 0 - - - - - - - -\n33 - - 38 37 - - 4 1 - - - - - - 0 0 - - - - - - -\n45 - - - 0 22 - - 20 42 - - - - - - 0 0 - - - - - -\n51 - - 48 35 - - - 44 - 18 - - - - - - 0 0 - - - - -\n47 11 - - - 17 - - 51 - - - 0 - - - - - 0 0 - - - -\n5 - 25 - 6 - 45 - 13 40 - - - - - - - - - 0 0 - - -\n33 - - 34 24 - - - 23 - - 46 - - - - - - - - 0 0 - -\n1 - 27 - 1 - - - 38 - 44 - - - - - - - - - - 0 0 -\n- 18 - - 23 - - 8 0 35 - - - - - - - - - - - - 0 0\n49 - 17 - 30 - - - 34 - - 19 1 - - - - - - - - - - 0'
# 1296, 2/3
s = '39 31 22 43 - 40 4 - 11 - - 50 - - - 6 1 0 - - - - - -\n25 52 41 2 6 - 14 - 34 - - - 24 - 37 - - 0 0 - - - - -\n43 31 29 0 21 - 28 - - 2 - - 7 - 17 - - - 0 0 - - - -\n20 33 48 - 4 13 - 26 - - 22 - - 46 42 - - - - 0 0 - - -\n45 7 18 51 12 25 - - - 50 - - 5 - - - 0 - - - 0 0 - -\n35 40 32 16 5 - - 18 - - 43 51 - 32 - - - - - - - 0 0 -\n9 24 13 22 28 - - 37 - - 25 - - 52 - 13 - - - - - - 0 0\n32 22 4 21 16 - - - 27 28 - 38 - - - 8 1 - - - - - - 0'
# 1296, 3/4
s = '39 40 51 41 3 29 8 36 - 14 - 6 - 33 - 11 - 4 1 0 - - - -\n48 21 47 9 48 35 51 - 38 - 28 - 34 - 50 - 50 - - 0 0 - - -\n30 39 28 42 50 39 5 17 - 6 - 18 - 20 - 15 - 40 - - 0 0 - -\n29 0 1 43 36 30 47 - 49 - 47 - 3 - 35 - 34 - 0 - - 0 0 -\n1 32 11 23 10 44 12 7 - 48 - 4 - 9 - 17 - 16 - - - - 0 0\n13 7 15 47 23 16 47 - 43 - 29 - 52 - 2 - 53 - 1 - - - - 0'
# 1296, 5/6
s = '48 29 37 52 2 16 6 14 53 31 34 5 18 42 53 31 45 - 46 52 1 0 - -\n17 4 30 7 43 11 24 6 14 21 6 39 17 40 47 7 15 41 19 - - 0 0 -\n7 2 51 31 46 23 16 11 53 40 10 7 46 53 33 35 - 25 35 38 0 - 0 0\n19 48 41 1 10 7 36 47 5 29 52 52 31 10 26 6 3 2 - 51 1 - - 0'
# 1944, 1/2
s = '57 - - - 50 - 11 - 50 - 79 - 1 0 - - - - - - - - - -\n3 - 28 - 0 - - - 55 7 - - - 0 0 - - - - - - - - -\n30 - - - 24 37 - - 56 14 - - - - 0 0 - - - - - - - -\n62 53 - - 53 - - 3 35 - - - - - - 0 0 - - - - - - -\n40 - - 20 66 - - 22 28 - - - - - - - 0 0 - - - - - -\n0 - - - 8 - 42 - 50 - - 8 - - - - - 0 0 - - - - -\n69 79 79 - - - 56 - 52 - - - 0 - - - - - 0 0 - - - -\n65 - - - 38 57 - - 72 - 27 - - - - - - - - 0 0 - - -\n64 - - - 14 52 - - 30 - - 32 - - - - - - - - 0 0 - -\n- 45 - 70 0 - - - 77 9 - - - - - - - - - - - 0 0 -\n2 56 - 57 35 - - - - - 12 - - - - - - - - - - - 0 0\n24 - 61 - 60 - - 27 51 - - 16 1 - - - - - - - - - - 0'
# 1944, 2/3
s = '61 75 4 63 56 - - - - - - 8 - 2 17 25 1 0 - - - - - -\n56 74 77 20 - - - 64 24 4 67 - 7 - - - - 0 0 - - - - -\n28 21 68 10 7 14 65 - - - 23 - - - 75 - - - 0 0 - - - -\n48 38 43 78 76 - - - - 5 36 - 15 72 - - - - - 0 0 - - -\n40 2 53 25 - 52 62 - 20 - - 44 - - - - 0 - - - 0 0 - -\n69 23 64 10 22 - 21 - - - - - 68 23 29 - - - - - - 0 0 -\n12 0 68 20 55 61 - 40 - - - 52 - - - 44 - - - - - - 0 0\n58 8 34 64 78 - - 11 78 24 - - - - - 58 1 - - - - - - 0'
# 1944, 3/4
s = '48 29 28 39 9 61 - - - 63 45 80 - - - 37 32 22 1 0 - - - -\n4 49 42 48 11 30 - - - 49 17 41 37 15 - 54 - - - 0 0 - - -\n35 76 78 51 37 35 21 - 17 64 - - - 59 7 - - 32 - - 0 0 - -\n9 65 44 9 54 56 73 34 42 - - - 35 - - - 46 39 0 - - 0 0 -\n3 62 7 80 68 26 - 80 55 - 36 - 26 - 9 - 72 - - - - - 0 0\n26 75 33 21 69 59 3 38 - - - 35 - 62 36 26 - - 1 - - - - 0'
# 1944, 5/6
s = '13 48 80 66 4 74 7 30 76 52 37 60 - 49 73 31 74 73 23 - 1 0 - -\n69 63 74 56 64 77 57 65 6 16 51 - 64 - 68 9 48 62 54 27 - 0 0 -\n51 15 0 80 24 25 42 54 44 71 71 9 67 35 - 58 - 29 - 53 0 - 0 0\n16 29 36 41 44 56 59 37 50 24 - 65 4 65 52 - 4 - 73 52 1 - - 0'

vals = s.replace(' ', '\n').splitlines()
for i,x in enumerate(vals):
    if x == '':
        print i
        
rowPtrs, cols, vals = getMatrixRepresentation(vals, False, 1944, 81, 5, 6)
print "rowPtrs: ", rowPtrs
print "cols: ", cols
print "vals: ", vals
print "numRows ", len(rowPtrs)
print "numVals ", len(vals)
