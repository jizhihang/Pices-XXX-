using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Text;


namespace SipperDiskUtilities
{
  public  class  SipperDiskFile: Stream
  {
    SipperFileControlBlock  fcb = null;
    
    long                    logicalPosition         = 0;
    long                    logicalFileSize         = 0;
    bool                    opened                  = false;

    long                    physicalFileSize        = 0;
    long                    physicalFileSizeSectors = 0;
    long                    physicalPosition        = 0;  // relative to the first sector on the disk
    SipperDisk              sipperDisk              = null;
    byte[]                  userMsgBuff             = null;
    int                     userMsgBuffLen          = 0;


    public  SipperDiskFile (SipperFileControlBlock  _fcb,
                            SipperDisk              _sipperDisk
                           ):
        base ()
    {
      opened                   = true;
      fcb                      = _fcb;
      sipperDisk               = _sipperDisk;
      physicalFileSize         = fcb.FileSizeInBytes;      
      physicalFileSizeSectors  = fcb.FileSizeInSectores;
      
      ConsructUserMessageBuffer ();
      
      logicalFileSize =  physicalFileSize + userMsgBuffLen;
      AdjustPhysicalPositionFromLogical ();
    }



    protected override void Dispose (bool disposing)
    {
      if  (opened)
        Close ();
      base.Dispose (disposing);
    }
    

    protected new void Dispose ()
    {
      if  (opened)
        Close ();
      base.Dispose ();
    }



    public  override  void  Close ()
    {
      if  (!opened)
        return;
    
      opened = false;
      base.Close ();
      
      // Let the controlling 'SipperDisk' objcet know that we are being closed.
      sipperDisk.CloseSipperDiskFile (this);
    }



    private  void  ConsructUserMessageBuffer ()
    {
      // 1st count how many bytes will be needed.
      
      string  userMessage = fcb.UserMessage ();

      while  ((userMessage.Length % 256) != 0)
        userMessage += " ";
      int  buffSpaceNeeded = userMessage.Length * 2;  // Two bytes for every character.
      
      // I want the buffer size to be a mutiple of a logical block(sector)
      int  buffSectorsNeeded = buffSpaceNeeded / Constants.LOGICAL_BLOCK_SIZE;
      if  ((buffSpaceNeeded % Constants.LOGICAL_BLOCK_SIZE) != 0)
        buffSectorsNeeded++;
        
       // We now need to pad the uerMessag to use up any usnised buffesapce 
      int  bytesToBeAllocated = buffSectorsNeeded * Constants.LOGICAL_BLOCK_SIZE;
      
      int  totalMessageWidth = bytesToBeAllocated / 2;  // Two bytes in buffer for every character in message
      userMessage.PadRight (totalMessageWidth);


      // Now that we have the exact contents of teh UseMessage and the buffersize 
      userMsgBuff = new byte[bytesToBeAllocated];
      
      int x = 0, y = 0;
      
      for  (x = 0;  x < userMessage.Length;  x++)
      {
        // '0100 0110' Bit pattern for 1st byte of Sipper 2 Byte record for UserText Message.
        userMsgBuff[y] = (byte)(userMessage[x]);  y++;
        userMsgBuff[y] = 70;                      y++;
      }
      
      userMsgBuffLen = userMsgBuff.Length;
    }  /* ConsructUserMessageBuffer */
     




    private  void  AdjustPhysicalPositionFromLogical ()
    {
      physicalPosition = logicalPosition - userMsgBuffLen;
    }




    private  long  CalcDiskPosition (long  physicalPosition)
    {
      long  diskPosition = (long)(fcb.StartSector) * (long)(Constants.LOGICAL_BLOCK_SIZE) + physicalPosition;
      return  diskPosition;
    }  /* CalcDiskPosition */



    override  public  bool  CanRead  {get  {return true;}}
    override  public  bool  CanSeek  {get  {return true;}}
    override  public  bool  CanWrite {get  {return false;}}
    override  public  long  Length   {get  {return  logicalFileSize;}}



    override  public  long  Position
    {
      get {return  logicalPosition;}
      set 
      {
        if  ((value < 0)  ||  (value >= logicalFileSize))
           throw new ArgumentException ("Invalid Position[" + value.ToString () + "]" + "  FileSize[" + logicalFileSize.ToString () + "]");
        logicalPosition = value;
      }
    }


    public override void Flush ()
    {
      throw new Exception ("The method or operation is not implemented.");
    }


    public override long  Seek  (long offset, SeekOrigin origin)
    {

      long  effectiveOffSet = 0;
      switch  (origin)
      {
        case  SeekOrigin.Begin:    effectiveOffSet = offset;                    break;
        case  SeekOrigin.Current:  effectiveOffSet = logicalPosition + offset;  break;
        case  SeekOrigin.End:      effectiveOffSet = logicalFileSize - offset;  break;
      }
      
      
      if  (effectiveOffSet < 0)
         throw new ArgumentException ("Invalid Position[" + effectiveOffSet.ToString () + "]" + "  FileSize[" + logicalFileSize.ToString () + "]");

      if  (effectiveOffSet >= logicalFileSize)
        effectiveOffSet = logicalFileSize;

      logicalPosition = effectiveOffSet;
      AdjustPhysicalPositionFromLogical ();
      
      return  logicalPosition;
    }  /* Seek */




    public override void SetLength (long value)
    {
      throw new Exception ("The method or operation is not implemented.");
    }


    public override  int  ReadByte ()
    {
      byte[]  buff = new byte[10];
      int  bytesRead = Read (buff, 0, 1);
      if  (bytesRead < 1)
        return -1;
      else
        return  buff[0];
    }  /* ReadByte */





    public override int  Read (byte[]  buffer, 
                               int     offset, 
                               int     count
                              )
    {
      int  x = 0;
      int  bytesRead = 0;
      
      if  (physicalPosition < 0)
      {
        // We are still in the User Message portion of the Sipper File.  
        // This part we logically constructed from the File Control Block (fcb).
        // We need to feed these bytes first before we feed the data from the
        // disk.
        
        int  numBytsLeftToReadFromUserMsgBuff = (int)((long)userMsgBuffLen - logicalPosition);
        int  bytesWeCanGetFromUserMsgBuff = Math.Min (numBytsLeftToReadFromUserMsgBuff, count);
        
        for  (x = 0;  x < bytesWeCanGetFromUserMsgBuff;  x++)
        {
          buffer[offset] = userMsgBuff[logicalPosition];
          offset++;
          logicalPosition++;
          physicalPosition++;
        }
        
        count = count - bytesWeCanGetFromUserMsgBuff;
        bytesRead +=  bytesWeCanGetFromUserMsgBuff;
      }

      if  (count > 0)
      {
        sipperDisk.BlockStart ();

        long  diskPosition = CalcDiskPosition (physicalPosition);        
        if  (diskPosition != sipperDisk.FSposition)
        {
          // Someone else accessed 'fs' and moved the current file position.  We will
          // need to use seek to put it back to where it should be for this
          // 'SipperDiskFile'  object.
          sipperDisk.FSseek (diskPosition, SeekOrigin.Begin);
        }
      
        long  newLogicalPosition = logicalPosition + (long)count;
        if  (newLogicalPosition > logicalFileSize)
        {
          int  numBytesWeArOver = (int)(newLogicalPosition - logicalFileSize);
          count = count - numBytesWeArOver;
          newLogicalPosition = logicalFileSize;
        }
      
        int  bytesReadFromDrive = 0;
        try
        {
          if  (offset > 0)
          {
            int zzzz = offset;
          }

          bytesReadFromDrive = sipperDisk.FSread (buffer, offset, count);
        }
        catch  (Exception e2)
        {
          long  diskByteOffset = CalcDiskPosition (physicalPosition);
          sipperDisk.BlockEnd ();

          String  exceptionStr = e2.ToString ();

          throw new IOException ("Exception reading SIPPER disk"                                                 + "\n" +
                                 "     FileName        [" + fcb.FileName                                  + "]"  + "\n" +
                                 "     FileByteOffset  [" + physicalPosition.ToString ("###,###,###,##0") + "]"  + "\n" +
                                 "     DiskBytePosition[" + diskByteOffset.ToString   ("###,###,###,##0") + "]"  + "\n" +
                                 "     Count           [" + count.ToString            ("###,###,###,##0") + "]." + "\n\n" +
                                 exceptionStr
                                );
        }

        sipperDisk.BlockEnd ();

        physicalPosition += bytesReadFromDrive;
        logicalPosition  += bytesReadFromDrive;
        bytesRead        += bytesReadFromDrive;
      }

      /*
      {
        x = 0;
        int  y = 1;
        byte z = 0;
        while  (x < (count - 1))
        {
          z = buffer[x];
          buffer[x] = buffer[y];
          buffer[y] = z;
          x += 2;
          y += 2;
        }
      }
       */

      return  bytesRead;
    }  /* Read */




    public override void Write (byte[] buffer, int offset, int count)
    {
      throw new Exception ("The method or operation is not implemented.");
    }

     
  }  /* SiperDiskFile */
}
