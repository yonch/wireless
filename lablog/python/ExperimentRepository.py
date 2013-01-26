
import bsddb.db as bdb
import os.path
import cPickle
from base64 import b64encode, b64decode
from struct import pack

def toUniqueString(d):
    if isinstance(d, int) or isinstance(d, float) or isinstance(d, long):
        s = 'n' + repr(d)
    elif isinstance(d, str):
        s = 's' + d
    elif isinstance(d, list):
        s = 'l' + reduce(lambda x,y: x+y, [toUniqueString(x) for x in d])
    elif isinstance(d, tuple):
        s = 't' + reduce(lambda x,y: x+y, [toUniqueString(x) for x in d])
    elif isinstance(d, dict):
        s = 'd' + reduce(lambda x,y: x+y, [toUniqueString(x) + toUniqueString(y) for x,y in sorted(d.items())]) 
    else:
        raise RuntimeError, "Unknown type"
    
    return pack("<L", len(s)) + s

def toUniqueHash(d):
    import hashlib
    return hashlib.sha1(toUniqueString(d)).digest()

class BundleExistsError (Exception):
    def __init__(self, bundle_id):
        self.bundle_id = bundle_id
    def __str__(self):
        return "bundle ID: " + repr(bundle_id)

class ExperimentRepository(object):
    """
    Stores required experiments and results. Users specify what experiments
        they want to run, and how many samples they require. The repository
        can then answer queries about what experiments are missing, and stores
        new results.
        
    Data structure:
        experiments.db: a dictionary experimentID -> experimentSpec
        requiredCount.db: a dictionary    experimentID -> requiredCount
        resultCount.db: a dictionary    experimentID -> resultCount
        results/<eId>.db: a record collection of strings, each string is a 
                            serialized result.
        bundleIDs: a hash tables        bundleID -> True
    """
    def __init__(self, repoDir, readOnly=True):
        self.readOnly = readOnly
        
        if readOnly:
            envFlags = bdb.DB_INIT_MPOOL
            self.dbFlags = bdb.DB_RDONLY
        else:
            envFlags = bdb.DB_CREATE | bdb.DB_INIT_TXN | bdb.DB_INIT_MPOOL | \
                bdb.DB_INIT_LOCK | bdb.DB_INIT_LOG # | bdb.DB_RECOVER_FATAL
            self.dbFlags = bdb.DB_CREATE | bdb.DB_READ_UNCOMMITTED | bdb.DB_AUTO_COMMIT
        
        localDbFlags = self.dbFlags
        
        
        self.env = bdb.DBEnv(0)
        # set size of locking filesystem
        self.env.set_lk_max_objects(100000)
        self.env.set_lk_max_locks(100000)
        # set size of logging manager
        self.env.set_lg_regionmax(1 << 20)
        # open the environment
        #self.env.remove(repoDir)
        self.env.open(repoDir, envFlags)
        # Lazy syncing should improve the throughput
        #self.env.set_flags(bdb.DB_TXN_NOSYNC, 1)
        
        self._experimentDb = bdb.DB(self.env)
        self._experimentDb.open("experiments.db", 
                                dbtype=bdb.DB_HASH, 
                                flags=localDbFlags)
        self._requiredCountDb = bdb.DB(self.env)
        self._requiredCountDb.open("requiredCount.db", 
                                dbtype=bdb.DB_HASH, 
                                flags=localDbFlags)
        self._resultCountDb = bdb.DB(self.env)
        self._resultCountDb.open("resultCount.db", 
                                dbtype=bdb.DB_HASH, 
                                flags=localDbFlags)
        self._bundleIdsDb = bdb.DB(self.env)
        self._bundleIdsDb.open("bundleIds.db", 
                                dbtype=bdb.DB_HASH, 
                                flags=localDbFlags)
        
        # make the directory where results will be stored
        try:
            os.mkdir(os.path.join(repoDir, 'results'))
        except:
            # already exists? anyway, we'll get an error later    
            pass
        
        if self.readOnly:
            self.txn = None
        else:
            self.txn = self.env.txn_begin()
    
    def close(self):
        # try to commit if transaction is still open
        if self.txn is not None:
            try:
                self.txn.commit()
            except bdb.DBError:
                # nothing to do in d'tor
                pass
        
        self._bundleIdsDb.close()
        self._resultCountDb.close()
        self._requiredCountDb.close()
        self._experimentDb.close()
        self.env.close()

    def __del__(self):
        self.close()
        
    def _serialize(self, val):
        return b64encode(cPickle.dumps(val, protocol=2))
    
    def _deserialize(self, s):
        return cPickle.loads(b64decode(s))
    
    def _getExperimentIDs(self):
        """
        Returns all the experiment IDs
        """
        if self.txn is not None:
            return self._experimentDb.keys(self.txn)
        else:
            return self._experimentDb.keys()
    
    experimentIDs = property(_getExperimentIDs) 

    def _getBundleIDs(self):
        """
        Returns all the bundle IDs
        """
        return self._bundleIdsDb.keys(self.txn)
        
    
    bundleIDs = property(_getBundleIDs)
    
    def _getExperimentDbFilename(self, eID):
        return "results/%s.db" % eID.encode('hex')

    def _getExperimentDB(self, eID):
        exprDb = bdb.DB(self.env)
        exprDb.open(self._getExperimentDbFilename(eID),
                    dbtype=bdb.DB_RECNO, 
                    flags=self.dbFlags)
                    #txn=self.txn)
        return exprDb

    def commit(self):
        if self.txn is None:
            raise RuntimeError, "No active transaction, are you in read-only mode?"
        
        # Commit the transaction
        self.txn.commit()
        
        # Start a new transaction
        self.txn = self.env.txn_begin()

    def abort(self):
        if self.txn is None:
            raise RuntimeError, "No active transaction, are you in read-only mode?"
        
        # Abort the transaction
        self.txn.abort()
        
        # Start a new transaction
        self.txn = self.env.txn_begin()
        
    
    def _hasExperimentHash(self, expHash):
        return self._experimentDb.has_key(expHash, txn=self.txn)

    def _assertWriteable(self):
        if self.readOnly:
            raise RuntimeError, "Opened in read only mode!"
    
    def hasExperiment(self, experimentSpec):
        expHash = toUniqueHash(experimentSpec)
        return self._hasExperimentHash(expHash)
    
    def getExperimentID(self, experimentSpec):
        expHash = toUniqueHash(experimentSpec)
        
        if not self._hasExperimentHash(expHash):
            raise RuntimeError, "Experiment does not exist"
        else:
            return expHash
    
    def addExperiment(self, experimentSpec, numRequired = 0):
        self._assertWriteable()

        expHash = toUniqueHash(experimentSpec)
        
        if not self._experimentDb.has_key(expHash, txn=self.txn):
            self._experimentDb.put(expHash, 
                                   self._serialize(experimentSpec), 
                                   txn=self.txn)
            if numRequired > 0:
                self.setRequiredCount(expHash, numRequired)
        else:
            self.require(expHash, numRequired)
        
        return expHash
    

    def deleteExperiment(self, eID):
        """
        This deletes experiment and all related results. Bundles associated are
            never deleted.
        
        """
        self._assertWriteable()

        try:
            self._experimentDb.delete(eID, txn=self.txn)
        except(bdb.DBNotFoundError):
            pass # will do best effort

        try:
            self._requiredCountDb.delete(eID, txn=self.txn)
        except(bdb.DBNotFoundError):
            pass # will do best effort
        
        try:
            self._resultCountDb.delete(eID, txn=self.txn)
        except(bdb.DBNotFoundError):
            pass # will do best effort
        
        experimentDbName = self._getExperimentDbFilename(eID)
        self.env.dbremove(experimentDbName, txn=self.txn)
        

    def getExperiment(self, eID):
        return self._deserialize(self._experimentDb.get(eID, txn=self.txn))
    
    def getRequiredCount(self, eID):
        return int(self._requiredCountDb.get(eID, default=0, txn=self.txn))
    
    def setRequiredCount(self, eID, numRequired):
        self._requiredCountDb.put(eID, str(numRequired), txn=self.txn)
        
    def require(self, eID, numRequired):
        """
        Enlarges the number of required experiments, if it is smaller than
            'numRequired'.
        """
        if numRequired == 0:
            return
        
        curRequirement = self.getRequiredCount(eID)
        if curRequirement < numRequired:
            self.setRequiredCount(eID, numRequired)
    
    def unrequireAll(self):
        """
        Sets requirements for all experiments to 0
        
        """
        for k in self._requiredCountDb.keys(self.txn):
            self._requiredCountDb.delete(k, txn=self.txn)

    def getResultCount(self, eID):
        return int(self._resultCountDb.get(eID, default="0", txn=self.txn))
            
    def getResults(self, eID, numResults):
        exprDb = self._getExperimentDB(eID)
        results = []
        cursor = exprDb.cursor(txn=self.txn)
        
        try:
            for i in xrange(numResults):
                k, data = cursor.get(flags=bdb.DB_NEXT)
                results.append(data)
        finally:
            cursor.close()
            exprDb.close()
        
        if len(results) != numResults:
            raise RuntimeError, "not enough results in files"
        
        return results
         
    def addResults(self, eID, resultList):
        self._assertWriteable()

        # remember number of results before insertion
        numBefore = self.getResultCount(eID)
        
        # add results
        exprDb = self._getExperimentDB(eID)
        try:
            for res in resultList:
                exprDb.append(res, txn=self.txn)
        except:
            exprDb.close()
            self.abort()
            raise
        
        # update count of results
        self._resultCountDb.put(eID, str(numBefore + len(resultList)), txn=self.txn)
        
        exprDb.close()
    
    def addBundle(self, bundleID, resultDict):
        """
        adds the bundleID. raises if bundleID already exists
        @param bundleID the ID of the bundle
        @param resultDict: a dictionary  eID -> List of results
        
        """
        self._assertWriteable()
        
        # Check bundle doesn't already exist
        if self._bundleIdsDb.has_key(bundleID, txn=self.txn):
            raise BundleExistsError(bundleID)
        
        # Add the bundle ID into the bundle table
        self._bundleIdsDb.put(bundleID, "", txn=self.txn)

        # Add results to suitable experiments
        for eID, resList in resultDict.items():
            self.addResults(eID, resList)
        
        
    def getPending(self):
        """
        @return: a list of tuples (experimentID, numMissing) for 
            experiments with numMissing > 0.
        
        """
        
        requiredList = self._requiredCountDb.items(self.txn)
        pendingList = []
        
        for eID, req in requiredList:
            req = int(req)
            present = int(self._resultCountDb.get(eID, default = "0", txn=self.txn))
            if present < req:
                pendingList.append( (eID, req-present ))
        
        return pendingList
    
    def dump(self, fname):
        raise RuntimeError, "not implemented"
    
    def load(self, fname):
        raise RuntimeError, "not implemented"
