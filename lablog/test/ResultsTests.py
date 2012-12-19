'''
Created on Apr 11, 2011

@author: yonch
'''
import unittest
from Results import Results
import uuid
import os
import shutil
from ExperimentRepository import ExperimentRepository


class Test(unittest.TestCase):


    def test_001_adding_results_succeeds(self):
        results = Results()
        
        results.append('a', 1)
        self.assertEqual(results.resDict, {'a':[1]})
        
        results.append('a', 2)
        self.assertEqual(results.resDict, {'a':[1,2]})

        results.append('b', 3)
        self.assertEqual(results.resDict, {'a':[1,2], 'b':[3]})
    
    def test_002_dump_and_read_works(self):
        results = Results()
        
        results.append('a', 1)
        results.append('a', 2)
        results.append('b', 3)
        
        DIR = '/tmp/lablog-result-dump-test-' + uuid.uuid4().hex
        os.mkdir(DIR)
        
        try:
            bundleID = results.bundleID
            results.dump(DIR)
            
            # make sure that results are readonly
            self.assertRaises(RuntimeError, results.append, 'c', 4)
            
            results = Results.fromFile(os.path.join(DIR, bundleID+".resBundle"))
            self.assertEqual(results.resDict, {'a':[1,2], 'b':[3]})
            self.assertEqual(bundleID, results.bundleID)
            
            # new results instance should be readonly
            self.assertRaises(RuntimeError, results.append, 'c', 4)
        finally:
            
            shutil.rmtree(DIR)
        
    def test_003_add_to_repository(self):
        REPO_FILE = '/tmp/lablog-result-add-to-repo-test-' + uuid.uuid4().hex
        os.mkdir(REPO_FILE)

        try:
            repo = ExperimentRepository(REPO_FILE, readOnly = False)
            
            eID = repo.addExperiment('a')
            eID2 = repo.addExperiment('b')
            results = Results()
            
            results.append(eID, "1")
            results.append(eID, "2")
            results.append(eID2, "3")
            
            # add results to repository
            results.addToRepo(repo)
            
            # make sure that results are readonly
            self.assertRaises(RuntimeError, results.append, 'c', "4")
            
            # repository should contain the right results
            self.assertEqual(set(repo.getResults(eID, 2)),
                             set(['1','2']))
            self.assertEqual(set(repo.getResults(eID2, 1)),
                             set(['3']))
            
            # double add should raise            
            self.assertRaises(Exception,
                              results.addToRepo, repo)
        finally:

            shutil.rmtree(REPO_FILE)



if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.test_001_']
    unittest.main()