'''
Created on Apr 10, 2011

@author: yonch
'''
import unittest
import os
import shutil
from ExperimentRepository import *



class Test(unittest.TestCase):


    def test_001_some_operations(self):
        import uuid
        REPO_FILE = '/tmp/testedRepoDir%s' % uuid.uuid4().hex
        os.mkdir(REPO_FILE)
        
        try:
            # Allocate repository
            repo = ExperimentRepository(REPO_FILE, readOnly=False)
            
            # Repo should not contain any value
            self.assertFalse(repo.hasExperiment({'type':'test', 'val':0}))
            
            # add experiment and check its existence
            eID = repo.addExperiment({'type':'test', 'val':0})
            self.assertTrue(repo.hasExperiment({'type':'test', 'val':0}))
            self.assertEqual(repo.getExperiment(eID), {'type':'test', 'val':0})
            self.assertEqual(repo.getExperimentID({'type':'test', 'val':0}), eID)
            self.assertEqual(repo.getRequiredCount(eID), 0)
            self.assertEqual(repo.getResultCount(eID), 0)
            self.assertEqual(repo.experimentIDs, [eID])
            
            # add another experiment and check its values
            eID2 = repo.addExperiment({'type':'test', 'val':1}, 890)
            self.assertEqual(repo.getExperimentID({'type':'test', 'val':1}), eID2)
            self.assertTrue(repo.hasExperiment({'type':'test', 'val':1}))
            self.assertEqual(repo.getExperiment(eID2), {'type':'test', 'val':1})
            self.assertEqual(repo.getRequiredCount(eID2), 890)
            self.assertEqual(repo.getResultCount(eID2), 0)
            self.assertEqual(set(repo.experimentIDs), set([eID,eID2]))
            
            # change second experiment's required count
            repo.setRequiredCount(eID2, 222)
            self.assertEqual(repo.getRequiredCount(eID2), 222)
            
            # make sure that this manipulation did not change first added value
            self.assertTrue(repo.hasExperiment({'type':'test', 'val':0}))
            self.assertEqual(repo.getExperiment(eID), {'type':'test', 'val':0})
            self.assertEqual(repo.getExperimentID({'type':'test', 'val':0}), eID)
            self.assertEqual(repo.getRequiredCount(eID), 0)
            self.assertEqual(repo.getResultCount(eID), 0)
            self.assertEqual(set(repo.experimentIDs), set([eID,eID2]))
            
            # Add some results
            repo.addResults(eID2, map(str, range(10,20)))
            self.assertEqual(repo.getResultCount(eID2), 10)
            repo.addResults(eID2, map(str, range(100,110)))
            self.assertEqual(repo.getResultCount(eID2), 20)
            
            res15 = repo.getResults(eID2, 15)
            allRes = map(str, range(10,20) + range(100,110))
            self.assertEqual(set(repo.getResults(eID2, 20)), 
                             set(allRes))
            self.assertTrue(set(res15).issubset(set(allRes))                            )
            
            # Adding results should not change required count
            self.assertEqual(repo.getRequiredCount(eID2), 222)
                        
            # make sure that this manipulation did not change first added value
            self.assertTrue(repo.hasExperiment({'type':'test', 'val':0}))
            self.assertEqual(repo.getExperiment(eID), {'type':'test', 'val':0})
            self.assertEqual(repo.getExperimentID({'type':'test', 'val':0}), eID)
            self.assertEqual(repo.getRequiredCount(eID), 0)
            self.assertEqual(repo.getResultCount(eID), 0)
            self.assertEqual(set(repo.experimentIDs), set([eID,eID2]))
            
            # test 'require' method
            repo.require(eID2, 111)
            self.assertEqual(repo.getRequiredCount(eID2), 222)
            repo.require(eID2, 222)
            self.assertEqual(repo.getRequiredCount(eID2), 222)
            repo.require(eID2, 333)
            self.assertEqual(repo.getRequiredCount(eID2), 333)
            repo.require(eID2, 222)
            self.assertEqual(repo.getRequiredCount(eID2), 333)

            
        finally:
            # remove the directory, to keep things clean
            shutil.rmtree(REPO_FILE)
        
    def test_002_getPending(self):
        import uuid
        REPO_FILE = '/tmp/testedRepoDir%s' % uuid.uuid4().hex
        os.mkdir(REPO_FILE)
        NUM_ELEMENTS = 10
        
        try:
            # Allocate repository
            repo = ExperimentRepository(REPO_FILE, readOnly=False)
            
            self.assertEqual(repo.getPending(), [])
            
            elements = []
            for elem in xrange(NUM_ELEMENTS):
                elements.append(repo.addExperiment(elem))

            self.assertEqual(repo.getPending(), [])
                
            for i in xrange(NUM_ELEMENTS):
                repo.require(elements[i],i+1)
            
            self.assertEqual(sorted(repo.getPending()), sorted(zip(elements,range(1,NUM_ELEMENTS+1))))

            # increasing and decreasing requirement should change result
            for i in xrange(NUM_ELEMENTS):
                repo.setRequiredCount(elements[i],NUM_ELEMENTS-i)
            self.assertEqual(set(repo.getPending()), set(zip(elements,range(NUM_ELEMENTS,0,-1))))
                
           
        finally:
            # remove the directory, to keep things clean
            shutil.rmtree(REPO_FILE)
        

    def test_003_double_add_same_bundle_should_fail(self):
        import uuid
        REPO_FILE = '/tmp/testedRepoDir%s' % uuid.uuid4().hex
        os.mkdir(REPO_FILE)
        
        try:
            # Allocate repository
            repo = ExperimentRepository(REPO_FILE, readOnly=False)
            
            eID1 = repo.addExperiment("ex1", 1)
            eID2 = repo.addExperiment("ex2")
            
            # add result
            repo.addBundle("bundle0", {eID1: 'hello', eID2:'world'})
            
            # re-add result. should raise
            self.assertRaises(Exception, 
                              repo.addBundle, "bundle0", {eID1: 'hello', eID2:'world'})
           
        finally:
            # remove the directory, to keep things clean
            shutil.rmtree(REPO_FILE)
        

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testExperimentRepository']
    unittest.main()