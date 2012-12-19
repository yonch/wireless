'''
Created on Apr 11, 2011

@author: yonch
'''
import os
import cPickle
import uuid

class Results(object):
    '''
    Holds results for a run, and supports serialization and de-serialization
    '''

    def __init__(self):
        '''
        Constructor
        '''
        
        self._reinit()
    
    def _reinit(self):
        self.resDict = {}
        self.bundleID = uuid.uuid4().hex
        self.readonly = False

    def append(self, eID, result):
        if self.readonly:
            raise RuntimeError, "instance is read-only"
        
        self.resDict.setdefault(eID, []).append(result)
    
    def dumps(self):
        """
        Returns a binary string containing the serialized result
        """
        cPickle.dumps((self.bundleID, self.resDict),
                      protocol=2)
    
    def dump(self, dirName):
        if self.readonly:
            raise RuntimeError, "instance is read-only"
        
        fname = self.bundleID + ".resBundle"
        fname = os.path.join(dirName, fname)
        
        cPickle.dump((self.bundleID, self.resDict),
                     file(fname, 'w'),
                     protocol=2)
        
        self.readonly = True
        
    def load(self, fname):
        self.bundleID, self.resDict = cPickle.load(file(fname))
        self.readonly = True
        
    def addToRepo(self, repo):
        # make sure there aren't two different versions of the same bundle
        self.readonly = True
        
        # add bundle to repository (making sure it doesn't already have it)
        repo.addBundle(self.bundleID, self.resDict)
                
    @classmethod
    def fromFile(cls, fname):
        """
        Reads a file into new instance
        @param fname: the file name where the resBundle is stored
        @return: the new instance
        """
        
        res = cls()
        res.load(fname)
        return res
        