using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using System.Drawing;

namespace SipperFile
{
  public class SipperBaseFile
  {
    private    bool                  blocked              = false;
    private    int                   blockedCount         = 0;
    private    long                  byteOffsetNextRead   = 0;
    
    protected  bool                  eof                  = false;
    private    bool                  eofReached           = false;
    private    string                errorDesc            = "";
    private    FileStream            fileStream           = null;
    private    BinaryReader          f                    = null;
    private    Stream                stream               = null;
    private    long                  fileSizeInBytes      = 0;
    private    string                fileName             = "";
    private    long[]                frameByteOffsets     = null;
    private    int                   frameNumLastRead     = -1;
    private    int                   frameNumNextRead     = 0;
    private    int                   largestKnowmFrameNum = -1;
    private    long                  largestKnownScanLine = -1;
    protected  long                  lastScanLineNum      = 0;

    protected  InstrumentDataManager instrumentDataManager = null;
    private    int                   maxNumOfFrames      = 0;
    public     const int             ScanLinesPerFrame   = 4096;
    public     const int             PixelsPerScanLine   = 4096;


    private Thread               updateFrameByteOffsetsThread    = null;
    private bool                 updateFrameByteOffsetsRunning   = false;
    private bool                 updateFrameByteOffsetsTerminate = false;
    private InstrumentDataList   history                         = null;


    public  bool  Eof                  ()  {return  eof;}
    public  bool  EofReached           ()  {return  eofReached;}
    public  long  FileSizeInBytes      ()  {return  fileSizeInBytes;}
    public  int   LargestKnowmFrameNum ()  {return  largestKnowmFrameNum;}
    public  long  LargestKnownScanLine ()  {return  largestKnownScanLine;}
 
 
    private const int RecordingRateNumSamplingFrames = 2;




    public SipperBaseFile (Stream                _stream,
                           InstrumentDataManager _instrumentDataManager
                          )
    {
      stream                = _stream;
      instrumentDataManager = _instrumentDataManager;
      fileStream            = null;

      try
      {
        OpenStream ();
      }
      catch (Exception e)
      {
        throw new Exception ("'SipperBaseFile',  Could not OpenFile file.", e);
      }
    }



    public SipperBaseFile (string                 _fileName,
                           InstrumentDataManager  _instrumentDataManager
                          )
    {
      fileName              = _fileName;
      instrumentDataManager = _instrumentDataManager;
        
        
      try
      {  
        OpenFile ();
      }
      catch  (IOException e)
      {
        throw e;
      }
      catch  (Exception  e2)
      {
        throw  e2;
      }
    }




    virtual public  void  Dispose ()
    {
      if  (updateFrameByteOffsetsRunning)
        FrameBytesOffsetsUpdateThreadTerminate ();

      if  (f != null)
      {
        f.Close ();
        f = null;
      }
      
      if  (fileStream != null)
      {
        fileStream.Close ();
        fileStream.Dispose ();
        fileStream =null;
      }
    } /* Dispose */


    
    virtual public  long  EstimatedNumberOfScanLines ()
    {
      long  bytesTillKnownFrameNum = frameByteOffsets[largestKnowmFrameNum];
      long  largestKnownScanLine = largestKnowmFrameNum * ScanLinesPerFrame;

      double  avgBytesPerScanLines = (double)bytesTillKnownFrameNum / (double)largestKnownScanLine;

      int  estimatedNumScanLines = (int)(0.5 + (double)fileSizeInBytes / avgBytesPerScanLines);
      return  estimatedNumScanLines;
    }  /* EstimatedNumberOfScanLines */



    virtual public  int  ScanLineSize ()
    {
      return  4096;
    }



    private  void  OpenFile ()
    {
      try
      {
        fileStream = new FileStream (fileName, 
                                     FileMode.Open,
                                     FileAccess.Read,  // We only want to read this file
                                     FileShare.Read    // Fine for someone else to open for 'Read'
                                    );
        stream = fileStream;
      }
      catch  (IOException  ioException)
      {
        f = null;
        errorDesc = "'SipperBaseFile',  I/O Error opening File[" + fileName + "]";
        throw ioException;
      }
      catch  (Exception e)
      {
        f = null;
        errorDesc = "'SipperBaseFile',  Error opening File[" + fileName + "]";
        throw new Exception (errorDesc, e);
      }

      InitializeStream ();
    }  /* OpenFile */


    
    private  void  OpenStream ()
    {
      InitializeStream ();
    }  /* OpenStream */






    private  void  InitializeStream ()
    {
      lastScanLineNum = 0;
      fileSizeInBytes = stream.Length;
      maxNumOfFrames = (int)((Int64)fileSizeInBytes / (Int64)(ScanLinesPerFrame * 4));
      f = new BinaryReader (stream);
      InitializeFrameOffsetData ();
    }  /* InitializeStream */
    



    private  void  InitializeFrameOffsetData ()
    {
      maxNumOfFrames = (int)((Int64)fileSizeInBytes / (Int64)(ScanLinesPerFrame * 4));

      frameByteOffsets = new long[maxNumOfFrames];
      for (int x = 0; x < maxNumOfFrames; x++)
        frameByteOffsets[x] = -1;
      frameByteOffsets[0] = 0;
      largestKnowmFrameNum = 0;
      lastScanLineNum = 0;
    }  /* InitializeFraeOffsetData */




    protected  byte  GetNextByte ()
    {
      byte  result = 0;
      try 
      {
        result = f.ReadByte ();
        byteOffsetNextRead += 1;
      }
      catch  
      {
        eof = true;
      }
     
      return  result;
    }  /* GetNextByte */



    protected  uint  GetNextUint ()
    {
      uint  result = 0;
      try 
      {
        result = f.ReadUInt32 ();
        byteOffsetNextRead += 4;
      }
      catch  
      {
        eof = true;
      }
     
      return  result;
    }  /* GetNextUint */



    protected  byte[]  GetNextBuff (uint  buffLen)
    {
      byte[] buff = null;
      try 
      {
        buff = f.ReadBytes ((int)buffLen);
        if  (buff != null)
           byteOffsetNextRead += buff.Length;
      }
      catch  
      {
        eof = true;
        buff = null;
      }
     
      return  buff;
    }  /* GetNextBuff */






    protected   ushort  GetNextRecord ()
    {
      ushort  rec = 0;
      
      try 
      {
        rec = f.ReadUInt16 ();
        byteOffsetNextRead += 2;
      }
      catch  
      {
        eof = true;
      }
     
      return  rec;
    }  /* GetNextRecord */





    public bool  ValidateFormat ()
    {
      bool validSipperFormat = false;


      int numGoodLinesRead = 0;
      int x;

      for (x = 0; ((x < 1000)  &&  !eof); x++)
      {
        lastScanLineNum = 0;
        int scanLineLen = LengthNextScanLine ();
        if  ((scanLineLen == 4096)  ||  (scanLineLen == 2048))
          numGoodLinesRead++;
      }

      float  percentGood = (float)numGoodLinesRead / (float)(x - 1);
      if (percentGood > 0.95f)
        validSipperFormat = true;

      return validSipperFormat;
    }  /* ValidateFormat */

   
   
   
   
    virtual  public int  LengthNextScanLine ()
    {
      return  0;
    }


    virtual  public  void  SkipNextScanLine ()
    {
    }  /* SkipNextScanLine */   



    virtual  public  void  ReadNextScanLine (byte[]  scanLine)
    {
    }  /* ReadNextScanLine */   




    public  byte[][]  AllocateFrame ()
    {
      int  x;
      byte[][]  frame = new byte[ScanLinesPerFrame][];
      for  (x = 0;  x < ScanLinesPerFrame;  x++)
        frame[x] = new byte[PixelsPerScanLine];
      
      return frame;
    }



    public  void  FrameRead (int         frameNum,
                             SipperFrame frame,
                             ref bool    found
                            )
    {
      BlockStart ();
    
      if  (frameNum > largestKnowmFrameNum)
      {
        UpdateFrameByteOffsetsForFrameNum (frameNum);
        if  (frameNum > largestKnowmFrameNum)
        {
          // this can happen when there are lesss than 'framNum' frames
          // in the Siiper file.
          found = false;
          BlockEnd ();
          return;
        }
      }
      
      found = true;
      
      long  tempFilePos =  f.BaseStream.Seek (frameByteOffsets[frameNum], SeekOrigin.Begin);
      
      byteOffsetNextRead = frameByteOffsets[frameNum];
      eof = false;
      frameNumLastRead = frameNum - 1;
      frameNumNextRead = frameNum;


      lastScanLineNum      = frameNum * ScanLinesPerFrame - 1;
      frame.FrameNum       = frameNum;
      frame.ScanLineFirst  = lastScanLineNum + 1;
      frame.ScanLineLast   = lastScanLineNum + ScanLinesPerFrame;
        
      ReadNextFrame (frame.ScanLines);

      BlockEnd();
    }  /* FrameRead */                   




    private  void  UpdateFrameByteOffsetsForFrameNum (int frameNum)
    {
      if  (frameNum <= largestKnowmFrameNum)
        return;

      int  x;
   
      f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);
      
      byteOffsetNextRead = frameByteOffsets[largestKnowmFrameNum];
      lastScanLineNum = largestKnowmFrameNum * ScanLinesPerFrame - 1;
      frameNumNextRead = largestKnowmFrameNum;
      eof = false;

      byte[] scanLine = new byte[PixelsPerScanLine];

      while  ((largestKnowmFrameNum < frameNum)  &&  (!eofReached))
      {
        x = 0;
        while  ((x < ScanLinesPerFrame)  &&  (!eof))
        {
          ReadNextScanLine (scanLine);
          lastScanLineNum++;
          x++;
        }
        if  (eof)
        {
          eofReached = true;
          eof = false;
          f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);
            
          byteOffsetNextRead = frameByteOffsets[largestKnowmFrameNum];
          lastScanLineNum = largestKnowmFrameNum * ScanLinesPerFrame - 1;
          frameNumNextRead = largestKnowmFrameNum;
        }
        else
        {
          frameNumLastRead = largestKnowmFrameNum;
          largestKnowmFrameNum++;

          if  (largestKnowmFrameNum >= frameByteOffsets.Length)
            ExpeandFrameByteOffsets ();

          frameByteOffsets[largestKnowmFrameNum] = byteOffsetNextRead;
          frameNumNextRead = largestKnowmFrameNum;
        }
      }

      if  (lastScanLineNum > largestKnownScanLine)
        largestKnownScanLine = lastScanLineNum;
    }  /* UpdateFrameByteOffsetsForFrameNum */



    private  void  ExpeandFrameByteOffsets ()
    {
      int  newMaxNumOfFrames = (int)(frameByteOffsets.Length * 1.2);
      long[]  temp = new long[newMaxNumOfFrames];
      int  x = 0;
      while  (x < frameByteOffsets.Length)
        temp[x] = frameByteOffsets[x];

      while  (x < temp.Length)
        temp[x] = -1;

      frameByteOffsets = temp;
      return;
    }  /* ExpeandFrameByteOffsets */


    
    public  void  ReadNextFrame (byte[][]  frame)
    {
      while  (frameNumNextRead >= frameByteOffsets.Length)
        ExpeandFrameByteOffsets ();

      frameByteOffsets[frameNumNextRead] = byteOffsetNextRead;
      
      int  row = 0;
      while  ((!eof)  &&  (row < frame.Length))
      {
        ReadNextScanLine (frame[row]);
        lastScanLineNum++;

        row++;
      }

      frameNumLastRead = frameNumNextRead;

      if  (!eof)
      {
        frameNumNextRead++;

        if (frameNumNextRead > largestKnowmFrameNum)
        {
          largestKnowmFrameNum = frameNumNextRead;

          while  (largestKnowmFrameNum >= frameByteOffsets.Length)
            ExpeandFrameByteOffsets ();

          frameByteOffsets[largestKnowmFrameNum] = byteOffsetNextRead;
        }
      }
      
      if  (lastScanLineNum > largestKnownScanLine)
        largestKnownScanLine = lastScanLineNum;

    }  /* ReadNextFrame */



    private  void  BlockStart ()
    {
      bool  weAreBlockedOut  = false;
      
      lock (this)
      {
        if  (blocked)
        {
          weAreBlockedOut = true;
          blockedCount++;
        }
        else
        {
          blocked = true;
        }
      }
      
      if  (!blocked)
        return;

      while  (weAreBlockedOut)
      {
        Thread.Sleep (10);

        lock (this)
        {
          weAreBlockedOut = blocked;
          if  (!weAreBlockedOut)
          {
            blocked = true;
            blockedCount--;
          }
        }
      } 
    }  /* BlockStart */




    private  void  BlockEnd  ()
    {
      lock (this)
      {
        blocked = false;
      }
    }  /* BlockEnd */




    public  void  FrameBytesOffsetsUpdateThreadStart (InstrumentDataList  _history)
    {
      if  ((updateFrameByteOffsetsRunning) || (updateFrameByteOffsetsThread != null))
      {
        MessageBox.Show ("Attempted to start 'UpdateFrameByteOffsets'  but already running", "StartFrameBytesOffsetsUpdateThread");
        return;
      }

      history = _history;

      updateFrameByteOffsetsThread = new Thread (new ThreadStart (UpdateFrameByteOffsets));
      updateFrameByteOffsetsThread.Name = "UpdateFrameByteOffsets";
      updateFrameByteOffsetsThread.Start ();
    }  /* FrameBytesOffsetsUpdateThreadStart */



    public  void  FrameBytesOffsetsUpdateThreadTerminate ()
    {
      if  ((!updateFrameByteOffsetsRunning)  ||  (updateFrameByteOffsetsThread == null))
        return;
      
      updateFrameByteOffsetsTerminate = true;
      
      int  loopCount = 0;
      while  ((updateFrameByteOffsetsRunning)  &&  (loopCount < 20))
      {
        Thread.Sleep (10);
      }

      if ((updateFrameByteOffsetsRunning) || (updateFrameByteOffsetsThread != null))
      {
        updateFrameByteOffsetsThread.Abort ();
      }
      
      updateFrameByteOffsetsThread    = null;
      updateFrameByteOffsetsRunning   = false;
      updateFrameByteOffsetsTerminate = false;
    }  /* FrameBytesOffsetsUpdateThreadTerminate */




    private void  UpdateFrameByteOffsets ()
    {
      updateFrameByteOffsetsRunning = true;
    
      long  lastByteOffset   = 0;
      int   lastScanLineRow  = 0;
      int   scanLinesPerSec  = 25000;
      
      while ((!eofReached)  &&  (!updateFrameByteOffsetsTerminate))
      {
        BlockStart ();
        int loopCount = 0;

        f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);
          
        byteOffsetNextRead = frameByteOffsets[largestKnowmFrameNum];
        lastScanLineNum = largestKnowmFrameNum * ScanLinesPerFrame - 1;
        eof = false;
        
        while  ((!eofReached)  &&  (!updateFrameByteOffsetsTerminate)  &&  (loopCount < 50)  &&  (blockedCount < 1))
        {
          //int linesRead = 0;
          int linesRead = 1;
          SkipNextScanLine ();
          while  ((linesRead < ScanLinesPerFrame) && (!eof))
          {
            lastScanLineNum++;
            linesRead++;
            SkipNextScanLine ();
          }

          if  (eof)
          {
            eofReached = true;
            eof = false;
            
            if  (linesRead < 1)
            {
              largestKnowmFrameNum--;
              f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);
            }
            else
            {
              try  {f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);}
              catch  (Exception)  
              {
                largestKnowmFrameNum--;
                f.BaseStream.Seek (frameByteOffsets[largestKnowmFrameNum], SeekOrigin.Begin);
              }
            }
              
            byteOffsetNextRead = frameByteOffsets[largestKnowmFrameNum];
            lastScanLineNum = largestKnowmFrameNum * ScanLinesPerFrame - 1;
          }
          else
          {
            largestKnowmFrameNum++;

            while  (largestKnowmFrameNum >= frameByteOffsets.Length)
              ExpeandFrameByteOffsets ();

            frameByteOffsets[largestKnowmFrameNum] = byteOffsetNextRead;
            frameNumLastRead = largestKnowmFrameNum;
            frameNumNextRead = largestKnowmFrameNum + 1;
          
            if  ((instrumentDataManager != null)  &&  (history != null))
            {
              InstrumentData  dataRowForFrame = new InstrumentData (instrumentDataManager.CurrentDataRow ());
              
              int  frameNumPrev     = largestKnowmFrameNum - RecordingRateNumSamplingFrames;
              if  (frameNumPrev < 0)
                frameNumPrev = 0;
                
              int   scanLineRow      = largestKnowmFrameNum * ScanLinesPerFrame;
              int   scanLineRowPrev  = frameNumPrev * ScanLinesPerFrame;
              int   scanLinesDelta   = scanLineRow - scanLineRowPrev + 1;

              long  byteOffset      = frameByteOffsets[largestKnowmFrameNum];
              long  byteOffsetPrev  = frameByteOffsets[frameNumPrev];
              long  byteOffsetDelta = byteOffset - byteOffsetPrev + 1;
              
              double  secsDelta      = (double)scanLinesDelta / (double)scanLinesPerSec;
              double  bytesDeltaMega = (double)byteOffsetDelta / 1000000.0;
              
              double  megPerSec = bytesDeltaMega / secsDelta;

              dataRowForFrame.RecordingRate = (float)megPerSec;

              lastScanLineRow   = scanLineRow;
              lastByteOffset    = byteOffset;
              
              dataRowForFrame.Time = dataRowForFrame.CTDdate ();
            
              history.Add (dataRowForFrame);
            }
          }
          
          loopCount++;
        }

        if  (lastScanLineNum > largestKnownScanLine)
          largestKnownScanLine = lastScanLineNum;
        
        BlockEnd ();
        
        Thread.Sleep (10);
      }

      updateFrameByteOffsetsRunning = false;
    }  /* UpdateFrameByteOffsetsForFrameNum */




    public  byte[][]  CreateRatser (int  scanLineStart,
                                    int  numScanLines
                                   )
    {
      byte[]  scanLine = new byte[PixelsPerScanLine];
      
      f.BaseStream.Seek (0, SeekOrigin.Begin);
        
      byteOffsetNextRead = 0;
      
      int  scanLineNum = 0;  
      // Skip up to scanLineStart
      while  ((scanLineNum < scanLineStart)  &&  (!eof))
      {
        ReadNextScanLine (scanLine);
        scanLineNum++;
      }
      
      if  (eof)
        return  null;
        
      long  startingByteOffset = byteOffsetNextRead;
      int  height = 0;
      
      while  ((!eof)  &&  (height < numScanLines))
      {
        ReadNextScanLine(scanLine);
        scanLineNum++;
        height++;
      }
      
      f.BaseStream.Seek (startingByteOffset, SeekOrigin.Begin);

      byteOffsetNextRead = startingByteOffset;
      scanLineNum = scanLineStart;
      
      byte[][]  raster = new byte[height][];
      int  row = 0;
      for  (row = 0;  row < height;  row++)
      {
        raster[row] = new byte[PixelsPerScanLine];
        ReadNextScanLine (raster[row]);
        scanLineNum++;
      }

      return  raster;
    }  /* CreateRatser */


  }  /* SipperBaseFile */
}
