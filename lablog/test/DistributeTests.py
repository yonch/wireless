'''
Created on Apr 11, 2011

@author: yonch
'''
import unittest
from Distribute import *
import random

class Test(unittest.TestCase):

    def test_001_should_observe_min(self):
        for numTasks in xrange(10,100):
            for lst in distribute([(1,numTasks)], 7, 10):
                self.assertTrue(sum(x[1] for x in lst) >= 10)

    def test_002_under_minimum_should_work(self):
        for numIDs in xrange(1,30):
            pending = [(i,1) for i in xrange(numIDs)]
            distributed = list(distribute(pending, 10, 29))
            self.assertEqual(len(distributed), 1)
            self.assertEquals(set(pending),
                              set(distributed[0]))
    
    def test_003_should_preserve_counts(self):
        pending = {}
        for i in xrange(50):
            pending[i] = random.randint(0,200)
        resPending = {}
        for lst in distribute(pending.items(), 13, 53):
            for id,num in lst:
                resPending.setdefault(id,0)
                resPending[id] += num
        
        self.assertEqual(pending, resPending)
    
    def test_004_should_distribute_to_num_processors(self):
        pending = {}
        for i in xrange(50):
            pending[i] = random.randint(0,200)
        
        self.assertEquals(13, len(list(distribute(pending.items(), 13, 53))))

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.test_001_should_observe_min']
    unittest.main()