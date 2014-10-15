using System;
using System.Collections.Generic;
using System.Text;

/**
 @class SipperDiskUtilities.PartitionTableEntry
 @brief  Reprsents a single partition table enry.
 @details
 @author  Kurt Kramer
 @code
 *****************************************************************************
 *  Developer:  Kurt Kramer                                                  *
 *  Date:       2007-Sept-05                                                 *
 *                                                                           *
 *  This cde was inspired by the Sipper3DLL.c code written by  Jim Patten    *
 * --------------------------------------------------------------------------*
 *  Meant to support the reading of data off the SIPPER harddrive.  This     *
 * class in particular represents a single partition entry in a Partition    *
 * table off the Master Boot record.                                         *
 *****************************************************************************
 @endcode
 */


namespace SipperDiskUtilities
{
  public  class  PartitionTableEntry
  {
    byte    bootID;
    byte    startHead;
    byte    startSector;
    byte    startCylinder;
    byte    systemID;
    byte    endHead;
    byte    endSector;
    byte    endCylinder;
    UInt32  relativeSector;   // aka  Starting Logivcal Block Address
    UInt32  totalSectors;     // Size of partition in sectors


    public   UInt32  RelativeSector  {get {return relativeSector;}}
    public   UInt32  TotalSectors    {get {return totalSectors;}}
    

    public  long  DiskSizeBytes  {get  {return  totalSectors * Constants.LOGICAL_BLOCK_SIZE;}}
    
    /**
     * @brief Constructs a Partition Table Entry from the contents of a buffer.
     * @param[in]      buff       Buffer to read data from.
     * @param[in,out]  nextIndex  Next byte from the bufer to be read.  Constructor will increment this as it processes each
     *                            character in the buff. The caller can use this as the starting point for the next partition
     *                            table entry.
     */
     
    public  PartitionTableEntry  (byte[]   buff, 
                                  ref int  nextIndex
                                 )
    {
      bootID         = buff[nextIndex];  nextIndex++;
      startHead      = buff[nextIndex];  nextIndex++;
      startSector    = buff[nextIndex];  nextIndex++;
      startCylinder  = buff[nextIndex];  nextIndex++;
      systemID       = buff[nextIndex];  nextIndex++;
      endHead        = buff[nextIndex];  nextIndex++;
      endSector      = buff[nextIndex];  nextIndex++;
      endCylinder    = buff[nextIndex];  nextIndex++;
      relativeSector = BitConverter.ToUInt32 (buff, nextIndex);  nextIndex = nextIndex + sizeof (Int32);
      totalSectors   = BitConverter.ToUInt32 (buff, nextIndex);  nextIndex = nextIndex + sizeof (Int32);
    }
      

    static  public  int  BuffSpaceNeeded () 
    {
      return  8 + 2 * sizeof (UInt32);
    }



    public  Int64  GetPartitionSectorByteOffset ()
    {
      Int64  sectorByteOffset = 0;
      sectorByteOffset = relativeSector * Constants.LOGICAL_BLOCK_SIZE;
      return sectorByteOffset;
    }  /* GetPartitionSectorByteOffset */


    public  Int64  GetSipperDirectoryByteOffset ()
    {
      Int64  sipperDirectoryByteOffset = 0;
      sipperDirectoryByteOffset =  relativeSector *  Constants.LOGICAL_BLOCK_SIZE + SipperPartitionSector.BuffSpaceNeeded ();
      return  sipperDirectoryByteOffset;      
    }  /* GetSipperDirectoryByteOffset */

  };  /* PartitionTableEntry */


}
