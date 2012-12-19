'''
Created on Apr 10, 2011

@author: yonch
'''

import os
import cPickle

try:
    from speed import speed
except:
    speed = lambda x:x


def distribute(pending, numProcessors, minAmount):
    '''
    Distributes the experiments that have less results than the required amount
    @param pending: list of tuples: (key, numToDistribute)
    @param numProcessors: number of processors to distribute to
    @param minAmount: Distributor will not distribute less than this
        amount per processor, unless the total amount of tasks is smaller
        than minAmount.
    @return: iterator that returns a single node's list of tuples (eID, num)

    '''

    totalPending = sum(x[1] for x in pending)
    
    minAmount = max(1, minAmount)
    
    numProcessors = max(1, min(numProcessors, totalPending / minAmount))
    
    while totalPending > 0:
        curAmount = totalPending / numProcessors

        remaining = curAmount
        res = []
        while remaining > 0:
            firstId, firstMissing = pending[0]
            if firstMissing <= remaining:
                # We return all the experiment in this result
                res.append((firstId, firstMissing))
                remaining -= firstMissing
                pending = pending[1:]
            else:
                # We fragment this experiment
                res.append((firstId, remaining))
                pending = [(firstId, firstMissing - remaining)] + pending[1:]
                remaining = 0

        numProcessors -= 1
        totalPending -= curAmount
        yield res
            
def dumpRequests(requestIterable, fileDirectory):
    '''
    Storing the requests with cPickle in randomly chosen filenames in the
        given directory
    @param requestIterable: iterable that provides requests
    @param fileDirectory: directory to write request files

    '''
    
    import uuid
    for reqInd, request in speed(enumerate(requestIterable)):
        fname = "spec%06d.request" % reqInd
        fname = os.path.join(fileDirectory, fname)
        
        file(fname, 'w').write(request)    
