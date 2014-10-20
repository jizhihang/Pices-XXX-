using System;
using System.Collections.Generic;
using System.Text;

namespace SipperDiskUtilities
{
  /* Master Boot Record */
  public class  MasterBootRecord
  {
    const int        bootRecordCodeSize = 446;

    byte[]           bootRecordCode = new byte[bootRecordCodeSize];
    PartitionTable   partitionTable;
    UInt16           endOfSector;



    public  long  SipperPartitionBeginSector
    {
      get 
      {
        long  sipperPartitionBeginSector = -1;
        if  (partitionTable != null)
        {
          sipperPartitionBeginSector = partitionTable.GetSipperPartitionTableEntry ().RelativeSector;
        }
        
        return  sipperPartitionBeginSector;
      }
    }



    public  MasterBootRecord (byte[]   buffer,
                              ref int  nextIndex
                             )
    {
      int  x = 0;
      for  (x = 0;  x < bootRecordCode.Length;  x++)
      {
        bootRecordCode[x] = buffer[nextIndex];  nextIndex++;
      }
        
      partitionTable = new PartitionTable (buffer, ref nextIndex);
      endOfSector = BitConverter.ToUInt16 (buffer, nextIndex);  nextIndex = nextIndex + sizeof(UInt16);
    }
      



      
    public  void  Dispose ()
    {
    }




    static  public  int  BuffSpaceNeeded ()
    {
      return bootRecordCodeSize  +  PartitionTable.BuffSpaceNeeded ()  +  sizeof (UInt16); 
    }



     
    public  PartitionTableEntry  GetSipperPartitionTableEntry ()
    {
      return  partitionTable.GetSipperPartitionTableEntry ();
    }  /* GetSipperPartitionTableEntry */





    public Int64  GetPartitionSectorByteOffset (int partitionNum)
    {
      return  partitionTable.GetPartitionSectorByteOffset (partitionNum);
    }  /* GetPartitionSectorByteOffset */





    public  Int64  GetSipperDirectoryByteOffset ()
    {
      return  partitionTable.GetSipperDirectoryByteOffset ();
    }

  };  /* MasterBootRecord */

}
