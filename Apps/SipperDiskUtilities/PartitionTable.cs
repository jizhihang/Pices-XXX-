using System;
using System.Collections.Generic;
using System.Text;

namespace SipperDiskUtilities
{

  public  class  PartitionTable  
  {
    PartitionTableEntry[]  partitions;


    /**
     * @brief Constructs a Partition Table by reading in each of the 4 partition table entries from a supplied buffer.
     * @param[in]      buff       Buffer to read data from.
     * @param[in,out]  nextIndex  Next byte from the buffer to be read.  Upon retun the caller can use this as the starting point 
     *                            for the next data structure.
     */
    public PartitionTable (byte[] buffer,
                           ref int  nextIndex
                          )
    {
      partitions = new PartitionTableEntry[4];
      for  (int x = 0;  x < 4;  x++)
        partitions[x] = new PartitionTableEntry (buffer, ref nextIndex);
    }



    static  public  int  BuffSpaceNeeded ()
    {
      return  4 * PartitionTableEntry.BuffSpaceNeeded ();
    }



    public  PartitionTableEntry  GetSipperPartitionTableEntry ()
    {
      return  partitions[Constants.SIPPER_PARTITION];
    }
    
    

    public  Int64  GetPartitionSectorByteOffset (int partitionNum)
    {
      return  partitions[partitionNum].GetPartitionSectorByteOffset ();
    }  /* GetPartitionSectorByteOffset */
 


    public  Int64  GetSipperDirectoryByteOffset ()
    {
      return  partitions[Constants.SIPPER_PARTITION].GetSipperDirectoryByteOffset ();
    }
  };  /* PartitionTable */
  

}
