using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SipperDiskUtilities
{
  class  SipperPartitionSector
  {
    private  char[]  sipperId       = new char[Constants.SIPPER_PARTITION_ID_SIZE];
    private  UInt32  totalSectors   = 0;
    private  UInt32  fileDataSector = 0;
    private  UInt32  endSector      = 0;
    private  UInt32  nextFcbSector  = 0;
    private  UInt32  nextFileHandle = 0;
    private  byte[]  unUsedSpace    = null;


    public long  DiskSizeBytes   {get  {return ((long)totalSectors * (long)(Constants.LOGICAL_BLOCK_SIZE));}}


    public  UInt32  TotalSectors    {get  {return  totalSectors;}    set  {totalSectors    = value;}}
    public  UInt32  FileDataSector  {get  {return  fileDataSector;}  set  {fileDataSector  = value;}}
    public  UInt32  EndSector       {get  {return  endSector;}       set  {endSector       = value;}}
    public  UInt32  NextFcbSector   {get  {return  nextFcbSector;}   set  {nextFcbSector   = value;}}
    public  UInt32  NextFileHandle  {get  {return  nextFileHandle;}  set  {nextFileHandle  = value;}}



     public  bool  Equals (SipperPartitionSector  other)
     {
       if  ((totalSectors    != other.TotalSectors)    ||
            (fileDataSector  != other.fileDataSector)  ||
            (endSector       != other.endSector)       ||
            (nextFcbSector   != other.nextFcbSector)   ||
            (nextFileHandle  != other.nextFileHandle)
           )
         return  false;
       else 
         return  true;
     }


    //*****************************************************************************************
    //*        Construnct a 'SipperPartitionSector' from the bytes read from Disk             *
    //*****************************************************************************************
    public  SipperPartitionSector  (byte[]   buffer,
                                    ref int  nextIndex
                                   )
    {
      int  x = 0;
      for  (x = 0;  x < Constants.SIPPER_PARTITION_ID_SIZE;  x++)
      {
        sipperId[x] = (char)buffer[nextIndex];  nextIndex++;
      }

      totalSectors    = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      fileDataSector  = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      endSector       = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      nextFcbSector   = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
      nextFileHandle  = BitConverter.ToUInt32 (buffer, nextIndex);  nextIndex += sizeof (UInt32);
        
      int  sizeOfUnUsedSpace = Constants.LOGICAL_BLOCK_SIZE - 
                               (sipperId.Length + 5 * sizeof (UInt32));
      unUsedSpace = new byte[sizeOfUnUsedSpace];
      for  (x = 0;  x < unUsedSpace.Length;  x++)
      {
        unUsedSpace[x] = buffer[nextIndex];  nextIndex++;
      }
    }



    
    //*****************************************************************************************
    //* Construncting a new'SipperPartitionSector' representing an emtpty drive.              *
    //*****************************************************************************************
    public  SipperPartitionSector (PartitionTableEntry  sipperPartition)  
    {
      int  x = 0;
      
      int  sipperIdLen = Math.Min (Constants.SIPPER_PARTITION_ID_SIZE, Constants.SIPPER_PARITION_ID.Length);
      
      sipperId = new char[Constants.SIPPER_PARTITION_ID_SIZE];
      for  (x = 0;  x < sipperIdLen;  x++)
        sipperId[x] = Constants.SIPPER_PARITION_ID[x];


      // Used code from  'Sipper3DLL.c'  function 'InitializeSIPPERPartition' as a model for
      // filling in the following fields.
      totalSectors    = sipperPartition.TotalSectors;
      fileDataSector  = sipperPartition.RelativeSector + (uint)Constants.MAX_SIPPER_FILES + (uint)1;

      endSector       = sipperPartition.RelativeSector + sipperPartition.TotalSectors;   // I worry about this line, seems that we should be subtracting '1' from it.
      
      nextFcbSector   = sipperPartition.RelativeSector + 1;
      nextFileHandle  = 0;

      int  sizeOfUnUsedSpace = Constants.LOGICAL_BLOCK_SIZE - (sipperId.Length + 5 * sizeof (UInt32));
      unUsedSpace     = new byte[sizeOfUnUsedSpace];
      for  (x = 0;  x < sizeOfUnUsedSpace;  x++)
        unUsedSpace[x] = 0;
    }




    static  public  int  BuffSpaceNeeded ()
    {
      return  Constants.LOGICAL_BLOCK_SIZE;
    }  /* BuffSpaceNeeded */





    public  void  FillBuffer (byte[]   buffer,
                              ref int  nextIndex
                             )
    {
      int  x = 0;
      
      for  (x = 0;  x < Constants.SIPPER_PARTITION_ID_SIZE;  x++)
      {
        buffer[nextIndex] = (byte)sipperId[x];  nextIndex++;
      }
        
      Constants.FillBuffer (buffer, ref nextIndex, totalSectors);
      Constants.FillBuffer (buffer, ref nextIndex, fileDataSector);
      Constants.FillBuffer (buffer, ref nextIndex, endSector);
      Constants.FillBuffer (buffer, ref nextIndex, nextFcbSector);
      Constants.FillBuffer (buffer, ref nextIndex, nextFileHandle);
        
      for  (x = 0;  x < unUsedSpace.Length;  x++)
      {
        buffer[nextIndex] = unUsedSpace[x];  nextIndex++;
      }
    }  /* FillBuffer */





    public  bool  IsASipperPartion ()
    {
      string  s = new string (sipperId);
      return  (s == Constants.SIPPER_PARITION_ID);
    }  /* IsASipperPartion */




    static public  SipperPartitionSector  CreateFromFileStream (FileStream fs)
    {
      byte[] buff = new byte[BuffSpaceNeeded ()];

      fs.Read(buff, 0, buff.Length);
      int nextIndex = 0;
      return new SipperPartitionSector (buff, ref nextIndex);
    }
    

    
    public  void  Write (FileStream fs)
    {
      int  nextIdx = 0;
      byte[]  buffer = new byte[BuffSpaceNeeded ()];
      FillBuffer (buffer, ref nextIdx);
      fs.Write (buffer, 0, buffer.Length);
    } /* Write */
    
  };  /* SipperPartitionSector */
}
