
import sqlite3
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

class SqliteExperimentRepository(object):
    """
    Stores required experiments and results. Users specify what experiments
        they want to run, and how many samples they require. The repository
        can then answer queries about what experiments are missing, and stores
        new results.
        
    Data structure:
        experiments dict: a dictionary experimentID -> experimentSpec
        requiredCount: a dictionary    experimentID -> requiredCount
        resultsDict: a dictionary      experimentID -> list of tuples
                                                      (resultID, numResults)
        bundleIDs: a dictionary        bundleID -> list of resultIDs
        each result file is a list with numResults entries. Each of the entries
            is of the format the experiment specifies.
    
    """
    def __init__(self, repoFile):
        sqlite3.register_converter("pickle", lambda x: cPickle.loads(b64decode(x)))
        
        self.conn = sqlite3.connect(repoFile, 
                                    detect_types = sqlite3.PARSE_DECLTYPES)
        self.conn.execute('PRAGMA foreign_keys = ON')
        if self.conn.execute('PRAGMA foreign_keys;').fetchall() != [(1,)]:
            raise RuntimeError, "This version of SQLite does not support" + \
                                " foreign key enforecement"
        
        self.c = self.conn.cursor()
        
        # if tables do not exist, create them
        try:
            self.conn.execute("select 1 from metaTable")
        except:
            self._createTables()
            
    def _serialize(self, val):
        return b64encode(cPickle.dumps(val, protocol=2))
    
    def _createTables(self):
        self.c.execute("CREATE TABLE experiments (id TEXT PRIMARY KEY, \
                                                  spec pickle, \
                                                  reqCount INTEGER)")
        self.c.execute("CREATE TABLE results (eid TEXT NOT NULL, \
                                              result pickle NOT NULL,\
                                              FOREIGN KEY(eid) REFERENCES experiments(id) ON DELETE CASCADE)")
        self.c.execute("CREATE INDEX result_eid_index ON results(eid)")
        self.c.execute("CREATE TABLE bundles (bundleID TEXT PRIMARY KEY)")
        
        self.c.execute("CREATE TABLE metaTable (key TEXT PRIMARY KEY,\
                                            value NONE)")
        self.conn.commit()
    
    def _getExperimentIDs(self):
        """
        Returns all the experiment IDs
        """
        return [str(x[0]) for x in self.conn.execute("SELECT id FROM experiments").fetchall()]
    
    experimentIDs = property(_getExperimentIDs) 

    def _getBundleIDs(self):
        """
        Returns all the experiment IDs
        """
        return [x[0] for x in self.conn.execute("SELECT bundleID FROM bundles").fetchall()]
    
    bundleIDs = property(_getBundleIDs)
    
    def commit(self):
        self.conn.commit()
    
    def _hasExperimentHash(self, expHash):
        return self.conn.execute(
                  "SELECT count() FROM experiments WHERE id=?", 
                  (expHash,)).fetchone() == (1,)
    
    def hasExperiment(self, experimentSpec):
        expHash = toUniqueHash(experimentSpec).encode('hex')
        return self._hasExperimentHash(expHash)
    
    def getExperimentID(self, experimentSpec):
        expHash = toUniqueHash(experimentSpec).encode('hex')
        
        if not self._hasExperimentHash(expHash):
            raise RuntimeError, "Experiment does not exist"
        else:
            return expHash
    
    def addExperiment(self, experimentSpec, numRequired = 0):
        expHash = toUniqueHash(experimentSpec).encode('hex')

        self.c.execute("INSERT INTO experiments (id,spec,reqCount) VALUES (?,?,?)",
                          (expHash, 
                           self._serialize(experimentSpec), 
                           numRequired))
        
        return expHash
    
    def deleteExperiment(self, eID):
        """
        This deletes experiment and all related results. Bundles associated are
            never deleted.
        
        """
        
        self.c.execute("DELETE FROM experiments WHERE id=?",
                          (eID,))
    
    def getExperiment(self, eID):
        return self.conn.execute("SELECT spec FROM experiments WHERE id=?",
                                 (eID,)).fetchone()[0]
    
    def getRequiredCount(self, eID):
        return self.conn.execute("SELECT reqCount FROM experiments WHERE id=?",
                                 (eID,)).fetchone()[0]
    
    def setRequiredCount(self, eID, numRequired):
        self.c.execute("UPDATE experiments SET reqCount=? WHERE id=?",
                          (numRequired, eID))
    
    def require(self, eID, numRequired):
        """
        Enlarges the number of required experiments, if it is smaller than
            'numRequired'.
        """
        curRequirement = self.getRequiredCount(eID)
        if curRequirement < numRequired:
            self.setRequiredCount(eID, numRequired)
    
    def unrequireAll(self):
        """
        Sets requirements for all experiments to 0
        
        """
        
        self.c.execute("UPDATE experiments SET reqCount=0")

    def getResultCount(self, eID):
        return self.conn.execute("SELECT count() FROM results WHERE eid=?",
                                 (eID,)).fetchone()[0]
    
    def getResults(self, eID, numResults):
        self.c.execute("SELECT result FROM results WHERE eid=? LIMIT ?",
                       (eID, numResults))
        results = [x[0] for x in self.c.fetchall()]
        if len(results) != numResults:
            raise RuntimeError, "not enough results in files"
        
        return results
         
    def addResults(self, eID, resultList):
        self.c.executemany("INSERT INTO results (eid, result) VALUES (?,?)",
                           [(eID, self._serialize(res)) for res in resultList])
    
    def addBundle(self, bundleID, resultDict):
        """
        adds the bundleID. raises if bundleID already exists
        @param bundleID the ID of the bundle
        @param resultDict: a dictionary  eID -> List of results
        
        """
        
        # Add the bundle ID into the bundle table
        self.c.execute("INSERT INTO bundles (bundleID) VALUES (?)",
                       (bundleID,))
        
        def getResultTuples(resultDict):
            for eID, resList in resultDict.items():
                for res in resList:
                    yield (eID, self._serialize(res))
        
        self.c.executemany("INSERT INTO results (eid, result) VALUES (?,?)",
                           getResultTuples(resultDict))
        
        
    def getPending(self):
        """
        @return: a list of tuples (experimentID, numMissing) for 
            experiments with numMissing > 0.
        
        """
        
        eIDs = self.conn.execute("""
            SELECT id, reqCount - IFNULL(numResults,0) as pending
            FROM 
                experiments 
                LEFT JOIN 
                (SELECT eid as id, COUNT() as numResults FROM results GROUP BY eid) AS countTable
                USING (id)
            WHERE
                pending > 0
            """).fetchall()
            
        return [(str(x[0]),x[1]) for x in eIDs]
    
    def dump(self, fname):
        try:
            from speed import speed
        except:
            speed = lambda x:x
        
        d = {}
        
        # get experiments
        print "getting experiments"
        experiments = []
        for eID in speed(self.experimentIDs):
            spec = self.getExperiment(eID)
            reqCount = self.getRequiredCount(eID)
            experiments.append((eID, spec, reqCount))
        
        d['experiments'] = experiments
        
        # get results
        print "getting results"
        d['results'] = self.conn.execute("SELECT eid, result FROM results").fetchall()
        
        # get bundles
        print "getting bundles"
        d['bundles'] = [x[0] for x in self.conn.execute("SELECT bundleID FROM bundles")]
        
        # dump to file
        print "dumping to file"
        import cPickle
        cPickle.dump(d, file(fname,'w'), protocol=2)
    
    def load(self, fname):
        try:
            from speed import speed
        except:
            speed = lambda x:x

        import cPickle
        d = cPickle.load(file(fname))
        
        # Adding experiments
        print "Adding experiments"
        for eID, spec, reqCount in speed(d['experiments']):
            self.addExperiment(spec, reqCount)
            
        # Adding bundles
        print "Adding bundles"
        for bundleID in speed(d['bundles']):
            self.addBundle(bundleID[0], {})
            
        print "Assembling results"
        resDict = {}
        for res in speed(d['results']):
            resDict.setdefault(res[0].encode('hex'),[] ).append(res[1])
        
        print "Adding results"
        import uuid
        self.addBundle(uuid.uuid4().hex, resDict)
            
        
        