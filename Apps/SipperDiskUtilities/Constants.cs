using System;
using System.Collections.Generic;
using System.Text;

namespace SipperDiskUtilities
{
  class Constants
  {
    public  const int     LOGICAL_BLOCK_SIZE           = 512;
    public  const int     MASTER_BOOT_RECORD_SECTOR    = 0;
    public  const int     MAX_SIPPER_FILES             = 2048;
    public  const int     SIPPER_FILENAME_SIZE         = 32;
    public  const int     SIPPER_FILE_DESCRIPTION_SIZE = 305;
    public  const int     SIPPER_PARTITION             =   1;
    public  const string  SIPPER_PARITION_ID           = "USFSIPPER  \0";
    public  const int     SIPPER_PARTITION_ID_SIZE     = 12;
    
    
    public  static void   FillBuffer (byte[]   buffer,
                                      ref int  nextIndex,
                                      UInt32   n
                                     )
    {
     // UInt32 == uint
    
      byte[]  bs = BitConverter.GetBytes (n);
      for  (int x = 0;  x < bs.Length;  x++)
      {
        buffer[nextIndex] = bs[x];  nextIndex++;
      }
    }  /* FillBuffer */
  }
}
