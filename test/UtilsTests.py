
import unittest
import math
import numpy
import random
import struct

import wireless as rf
import wireless

class UtilsTests(unittest.TestCase):

    def test_001_string_bit_extraction(self):
        # make random packet
        packet = numpy.random.bytes(10);
        
        # transform packet into a vector
        packetVec = rf.vectorb()
        rf.Utils.stringToVector(packet, packetVec)
        
        # for different bit extractions
        for pos in xrange(30):
            for length in xrange(1, 30):
                # extract bits using string
                subPacket = rf.Utils.returnStringBits(packet, pos, length)

                # get extracted bits into a vector
                subVec = rf.vectorb()
                rf.Utils.stringToVector(subPacket, subVec)
                
                # compare two extraction methods
                for i in xrange(length):
                    if packetVec[pos+i] != subVec[i]:
                        pass
                    self.assertEqual(packetVec[pos+i], 
                                     subVec[i], 
                                     "mismatch: original packet at location %d doesn't match extracted bits at location %d" % (pos+i,i))
                
                # make sure that bits that were not extracted are zeros
                for i in xrange(length, subVec.size()):
                    self.assertEqual(subVec[i], 0, "padding bit was supposed to be 0 at location %d" % i)

    def test_002_string_bit_copying(self):
        # Note that this test relies on bit extraction
        
        # make random bytes
        src = numpy.random.bytes(10)
        dst = numpy.random.bytes(10)
        
        # for different bit extractions
        for dst_pos in xrange(17):
            for src_pos in xrange(17):
                for length in xrange(1, 30):
                    # copy source bits into dst
                    res = rf.Utils.returnCopiedBits(dst, dst_pos, 
                                                    src, src_pos,
                                                    length)
                    
                    # make sure the beginning of the string is intact
                    if dst_pos > 0:
                        self.assertEqual(
                            rf.Utils.returnStringBits(dst,0,dst_pos), 
                            rf.Utils.returnStringBits(res,0,dst_pos),
                            "Beginning of the packet was corrupted")
                    # bits should be copied okay
                    self.assertEqual(
                            rf.Utils.returnStringBits(src,src_pos,length),
                            rf.Utils.returnStringBits(res,dst_pos,length),
                            "Bits were not copied successfully")
                    # end of dst should be left untouched
                    self.assertEqual(
                        rf.Utils.returnStringBits(dst,dst_pos+length,80-(dst_pos+length)), 
                        rf.Utils.returnStringBits(res,dst_pos+length,80-(dst_pos+length)),
                        "End of the packet was corrupted")

    def test_003_blockify_uint32(self):
        # Note that this test relies on bit extraction
        
        # make random bytes
        src = numpy.random.bytes(1000)
        
        # for different block lengths
        for block_size in xrange(1,26):
            # blockify the string
            blocks = rf.vectorui()
            rf.Utils.BlockifyUint32(src, block_size * 300, block_size, blocks)
            
            # check output
            for i in xrange(300):
                # get the block's bits from the string
                blockBits = rf.Utils.returnStringBits(src, 
                                                      i * block_size,
                                                      block_size)
                
                # convert into a uint32
                blockVal = 0
                for j, ch in enumerate(blockBits):
                    blockVal += ord(ch) << (8 * j)
                
                self.assertEqual(blockVal, blocks[i], 
                                 "block_size %d; block %d doesn't match: expected %d got %d" % (block_size, i, blockVal, blocks[i]))

        
if __name__ == "__main__":
    unittest.main()        
