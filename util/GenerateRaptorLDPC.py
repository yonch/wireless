
from scipy.stats import binom
import itpp

def generateRaptorLDPC(numInfoBits, rate, outFilename):
    """
    Generates a left 4-degree regular, random right degree LDPC code with
    'numInfoBits' information bits.
    
    @param numInfoBits: the number of message bits the LDPC code will handle
    @param rate: the rate of the LDPC code
    @param outFilename: filename where the bits will be written
    """
    
    leftDegree = 4
    
    numVariables = int(numInfoBits / rate)
    print "numVariables:", numVariables
    
    dist = binom(numInfoBits, 1.0 * leftDegree / (numVariables - numInfoBits))
    maxRightDegree = int(dist.isf(1e-8))
    itRight = itpp.vec(maxRightDegree)
    for i in xrange(maxRightDegree):
        itRight[i] = dist.pmf(i+1)
    itRight[maxRightDegree-1] = itRight[maxRightDegree-1] + 1e-8

    itLeft = itpp.vec(leftDegree)
    for i in xrange(leftDegree-1):
        itLeft[i] = 0
    itLeft[leftDegree - 1] = 1.0
    print itLeft
    
    H = itpp.LDPC_Parity_Irregular()
    H.generate(numVariables, itLeft, itRight, "rand", itpp.ivec("150 8"))
    H.display_stats()
    print "got code with ", (H.get_nvar() - H.get_ncheck()), " nodes"
    
    
    G = itpp.LDPC_Generator_Systematic(H);
    C = itpp.LDPC_Code(H, G)
    C.save_code(outFilename);
    
#generateRaptorLDPC(9500 + 30, 0.9525, 'LDPC_9500.it')
#generateRaptorLDPC(256 + 0, 0.95, 'LDPC_256.it')
generateRaptorLDPC(256 + 0, 0.95, 'LDPC_256.it')