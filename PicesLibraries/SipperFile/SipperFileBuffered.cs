using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;


namespace SipperFile
{
  public  class  SipperFileBuffered
  {
    private  SipperClock      clock           = null;
    private  SipperFrame[]    frames          = null;
    private  SipperBaseFile   sipperFile      = null;
    private  bool             weOwnSipperFile = false;


    
    //  Used to describe which logical column
    private  int              cropColLeft      = 0;     // Scan Column that we start with
    private  int              cropColRight     = 4095;  // Scan Column that we end with;  other clumns will be ignored.
    private  int              croppedWidth     = 4096;
    private  float            dispColsPerScanCol = 1.0f;  // Should be inverse of ratio;
    private  float            dispRowsPerScanRow = 1.0f;
    private  int              pixelsPerDisLine = 4096;
    private  float            ratio;
    private  float[]          scanColPerDispColRatio = null;
    private  int[]            scanColPerDispColTarget = null;
    private  int              scanLineSize       = 4096;

    public  int   CropColLeft                ()  {return  cropColLeft;}
    public  int   CropColRight               ()  {return  cropColRight;}
    public  long  EstimatedNumberOfScanLines ()  {return  sipperFile.EstimatedNumberOfScanLines ();}
    public  long  FileSizeInBytes            ()  {return  sipperFile.FileSizeInBytes      ();}
    public  int   LargestKnowmFrameNum       ()  {return  sipperFile.LargestKnowmFrameNum ();}
    public  long  LargestKnownScanLine       ()  {return  sipperFile.LargestKnownScanLine ();}
    public  int   PixelsPerDisLine           ()  {return  pixelsPerDisLine;}
    public  float Ratio                      ()  {return  ratio;}



    public  bool  EofReached
    {
      get
      {
        if  (sipperFile == null)
          return true;
        return  sipperFile.EofReached ();
      }
    } /* EofReached */



    public  void  FrameBytesOffsetsUpdateThreadStart (InstrumentDataList  _history)
    {
      sipperFile.FrameBytesOffsetsUpdateThreadStart (_history);
    }


    public void  FrameBytesOffsetsUpdateThreadTerminate ()
    {
      sipperFile.FrameBytesOffsetsUpdateThreadTerminate ();
    }





    public  SipperFileBuffered (SipperBaseFile  _sipperFile)
    {
      weOwnSipperFile = false;
      sipperFile = _sipperFile;
      Initialization ();
    }




    public  SipperFileBuffered (string                _fileName,
                                InstrumentDataManager _dataManger
                               )
    {
      try
      {
        sipperFile = OpenAppropriateFormat (_fileName, _dataManger);
      }
      catch  (IOException  ioException)
      {
        throw ioException;
      }
      
      if  (sipperFile == null)
      {
        throw new Exception("SipperFileBuffered:   Could not determine format of file[" + _fileName + "]");
      }
      
      weOwnSipperFile = true;
      Initialization ();
    }




    public  SipperFileBuffered (Stream                _stream,
                                InstrumentDataManager _dataManger
                               )
    {
      // We will first make sure that '_fileName' exists.
      sipperFile = OpenAppropriateFormat (_stream, _dataManger);
      if  (sipperFile == null)
      {
        throw new Exception("SipperFileBuffered:   Could not determine format of stream.");
      }
      
      weOwnSipperFile = true;
      Initialization ();
    }



    public  void  Dispose ()
    {
      if  (weOwnSipperFile)
      {
        sipperFile.Dispose ();
        sipperFile = null;
      }
    }  /* Dispose */




    private  void  Initialization ()
    {
      clock = new SipperClock();
      int x = 0;
      frames = new SipperFrame[3];
      
      bool  found = true;
      
      for (x = 0; x < frames.Length; x++)
      {
        frames[x] = new SipperFrame (clock);
        sipperFile.FrameRead (x, frames[x], ref found);
        if  (!found)
        {
          frames[x].FrameNum = -1;
          frames[x].ScanLineFirst = -1;
          frames[x].ScanLineLast = -1;
        }
      }

      SetPixelsPerDisplayLine (scanLineSize, 0, scanLineSize - 1);
    }  /* Initialization */



    
    public  void  SetPixelsPerDisplayLine (int _pixelsPerDisLine,
                                           int _cropColLeft,
                                           int _cropClRight
                                          )
    {
      pixelsPerDisLine = _pixelsPerDisLine;
      cropColLeft      = _cropColLeft;
      cropColRight     = _cropClRight;

      CalculateColumnRatios ();
    }




    public  int  ScanLineSize ()
    {
      return  sipperFile.ScanLineSize ();
    }




    private  void  CalculateColumnRatios ()
    {
      croppedWidth = 1 + cropColRight - cropColLeft;
      ratio = (float)croppedWidth / (float)pixelsPerDisLine;
      dispColsPerScanCol = (float)pixelsPerDisLine / (float)croppedWidth;
      dispRowsPerScanRow = (float)pixelsPerDisLine / (float)croppedWidth;

      scanColPerDispColRatio  = new float[scanLineSize];
      scanColPerDispColTarget = new int  [scanLineSize];

      float  targetDispColCur  = 0.0f;
      float  targetDispColNext = 1.0f / ratio;

      int    targetDispColCurInt  = 0;
      int    targetDispColNextInt = (int)Math.Floor (targetDispColNext);
      
      int scanLineCol = 0;

      while  (scanLineCol < cropColLeft)
      {
        scanColPerDispColTarget[scanLineCol] = -1;
        scanColPerDispColRatio[scanLineCol] = 0.0f;
        scanLineCol++;
      }

      while  (scanLineCol <= cropColRight)
      {
        scanColPerDispColTarget[scanLineCol] = (int)Math.Floor (targetDispColCur);

        if  (Math.Floor (targetDispColCur) == Math.Floor (targetDispColNext))
        {
          scanColPerDispColRatio[scanLineCol] = 1.0f;
        }
        else
        {
          scanColPerDispColRatio[scanLineCol] = ((float)targetDispColNextInt - targetDispColCur) / dispColsPerScanCol;
        }

        scanLineCol++;
        targetDispColCur = targetDispColNext;
        targetDispColNext = (float)(scanLineCol - cropColLeft) / ratio;

        targetDispColCurInt  = targetDispColNextInt;
        targetDispColNextInt = (int)Math.Floor (targetDispColNext);
      }

      while  (scanLineCol < scanLineSize)
      {
        scanColPerDispColTarget[scanLineCol] = int.MaxValue;
        scanColPerDispColRatio[scanLineCol] = 0.0f;
        scanLineCol++;
      }

    }  /* CalculateTargetCols */




    private  SipperBaseFile  OpenAppropriateFormat (string                 _fileName,
                                                    InstrumentDataManager  _dataManger
                                                   )
    {
      // First Lets try Sipper3 format
      
      bool   validSipper2      = false;
      bool   validSipper3      = false;
      bool   validSipperSimple = false;
      bool   validLarcos4Bit   = false;
      
      
      // We will first try to see if a Sipper3 file.
      try
      {
        sipperFile = new Sipper3File (_fileName, _dataManger);
        validSipper3 = sipperFile.ValidateFormat ();
      }
      catch  (IOException ioException)
      {
        throw  new IOException ("OpenAppropriateFormat  Could not open file", ioException);      
      }
      catch  (Exception)
      {
        validSipper3 = false;
        sipperFile = null;
      }

      if  ((sipperFile != null)  &&  (validSipper3))
        return  sipperFile;

      if  (sipperFile != null)
      {
         sipperFile.Dispose ();
         sipperFile = null;
      }


      // Try 4Bit Larcos Format
      try
      {
        sipperFile = new SipperBuff4Bit (_fileName, _dataManger);
        validLarcos4Bit = sipperFile.ValidateFormat ();
      }
      catch  (IOException ioException)
      {
        throw  new IOException ("OpenAppropriateFormat  Could not open file", ioException);      
      }
      catch  (Exception)
      {
        validLarcos4Bit = false;
        sipperFile = null;
      }

      if  ((sipperFile != null)  &&  (validLarcos4Bit))
        return  sipperFile;

      if  (sipperFile != null)
      {
         sipperFile.Dispose ();
         sipperFile = null;
      }


      // Now Try try the Sipple Sipper Format.
      try  
      {
        sipperFile = new SipperFileSimple (_fileName, _dataManger);
        validSipperSimple = sipperFile.ValidateFormat ();
      }
      catch  (Exception)
      {
        validSipperSimple = false;
        sipperFile = null;
      }

      if  ((sipperFile != null) && (validSipperSimple))
        return sipperFile;

      if  (sipperFile != null)
        sipperFile.Dispose ();
        
      sipperFile = null;



      // Now we will try to see if valid  Sipper2 format
      try  
      {
        sipperFile = new Sipper2File (_fileName, _dataManger);
        validSipper2 = sipperFile.ValidateFormat ();
      }
      catch  (Exception)
      {
        validSipper2 = false;
        sipperFile = null;
      }

      if  ((sipperFile != null) && (validSipper2))
        return sipperFile;


      if  (sipperFile != null)
        sipperFile.Dispose ();
        
      sipperFile = null;

      return  null;
    }  /* OpenAppropriateFormat  */




    private  SipperBaseFile  OpenAppropriateFormat (Stream                 _stream,
                                                    InstrumentDataManager  _dataManger
                                                   )
    {
      //*******************************************************
      // First Lets try Sipper3 format

      bool  validSipper2      = false;
      bool  validSipper3      = false;
      bool  validSipperSimple = false;
      bool  validLarcos4Bit   = false;

      long  startPos = _stream.Position;


      // We will first try to see if a Sipper3 file.
      try
      {
        sipperFile = new Sipper3File (_stream, _dataManger);
        validSipper3 = sipperFile.ValidateFormat ();
      }
      catch  (Exception)
      {
        validSipper3 = false;
        sipperFile = null;
      }
      if  ((sipperFile != null)  &&  (validSipper3))
        return  sipperFile;
      if  (sipperFile != null)
      {
         //sipperFile.Dispose ();
         //sipperFile = null;
      }



      //*******************************************************
      // Try 4Bit Larcos Format
      try
      {
        _stream.Seek (0, SeekOrigin.Begin);
        sipperFile = new SipperBuff4Bit (_stream, _dataManger);
        validLarcos4Bit = sipperFile.ValidateFormat ();
      }
      catch  (IOException ioException)
      {
        throw  new IOException ("OpenAppropriateFormat  Could not open file", ioException);      
      }
      catch  (Exception)
      {
        validLarcos4Bit = false;
        sipperFile = null;
      }

      if  ((sipperFile != null)  &&  (validLarcos4Bit))
        return  sipperFile;



      //*******************************************************
      // Try SipperSimple format.
      _stream.Position = startPos;
      try
      {
        sipperFile = new SipperFileSimple (_stream, _dataManger);
        validSipperSimple = sipperFile.ValidateFormat ();
      }
      catch  (Exception)
      {
        validSipperSimple = false;
        sipperFile = null;
      }
      if  ((sipperFile != null)  &&  (validSipperSimple))
        return  sipperFile;


      // Now we will try to see if valid  Sipper2 format
      try  
      {
        sipperFile = new Sipper2File (_stream, _dataManger);
        validSipper2 = sipperFile.ValidateFormat ();
      }
      catch  (Exception)
      {
        validSipper2 = false;
        sipperFile = null;
      }

      if  ((sipperFile != null) && (validSipper2))
        return sipperFile;


      if  (sipperFile != null)
        sipperFile.Dispose ();
        
      sipperFile = null;

      return  null;
    }  /* OpenAppropriateFormat  */



    private  SipperFrame  OldestFrame ()
    {
      int  x = 0;
      
      SipperFrame  oldest     = frames[0];
      long         oldestTime = oldest.Time;
      if  (oldest.FrameNum == -1)
        return  oldest;
      
      for  (x = 1;  x < frames.Length;  x++)
      {
        if  (frames[x].Time < oldestTime)
        {
          oldest = frames[x];
          oldestTime = oldest.Time;
        }
      }
      
      oldest.Time = 0;
      oldest.FrameNum = -1;
      oldest.ScanLineFirst = -1;
      oldest.ScanLineLast = -1;
      
      return  oldest;
    }  /* OldestFrame */





    private  SipperFrame  GetFrameWithScanLine (long  scanLine)
    {
      int  frameNum = (int)(scanLine / Sipper3File.ScanLinesPerFrame);
      int  x = 0;
      bool  frameFound = true;
      
      SipperFrame  f = null;
      
      for  (x = 0;  x < frames.Length;  x++)
      {
        f = frames[x];
        if  (f.FrameNum == frameNum)
          return  f;
      }
      
      f = OldestFrame ();
      
      sipperFile.FrameRead (frameNum, f, ref frameFound);
      if  (!frameFound)
        f = null;

      return  f;
    }  /* GetFrameWithScanLine */
  
  
  
  
    public   byte[][]  GetScanLines  (long  scanLineStart,
                                      long  scanLineEnd
                                     )
    {
      long  numScanLines = scanLineEnd - scanLineStart + 1;
      byte[][]  scanLines = new byte[numScanLines][];
      int       scanLinesNextRow = 0;
      
      long  nextScanLine = scanLineStart;

      while  (nextScanLine <= scanLineEnd)
      {
        SipperFrame f = GetFrameWithScanLine (nextScanLine);
        if  (f == null)
          break;
        
        if  (sipperFile.EofReached ()  &&  (sipperFile.LargestKnownScanLine () < nextScanLine))
          break;
        
        byte[][] thisFrame = f.ScanLines;
        int  thisFrameNextRow = 0;
        
        long  thisFrameFirstRow  = nextScanLine - f.ScanLineFirst;
        long  thisFrameLastRow = Math.Min (scanLineEnd - f.ScanLineFirst, (thisFrame.Length - 1));

        thisFrameNextRow = (int)thisFrameFirstRow;

        while (thisFrameNextRow <= thisFrameLastRow)
        {
          scanLines[scanLinesNextRow] = thisFrame[thisFrameNextRow];
          nextScanLine++;
          scanLinesNextRow++;
          thisFrameNextRow++;
        }
      }
      
      if  (scanLinesNextRow == 0)
        scanLines = null;
      
      return  scanLines;
    } /* GetScanLines */                               
  
  
  
  
  
    private  void  UpdateDisplayRow (byte[]   scanLine,
                                     ushort[] displayRow,
                                     double   scanLineFrac      // Percentagof this row thatshoul contribute to the displayRow
                                    )
    {
      int  scanCol = cropColLeft;
      int  dispCol = 0;

      while  (scanCol <= cropColRight)
      {
        byte  pixelValue = scanLine[scanCol];
        if  (pixelValue > 0)
        {
          dispCol = scanColPerDispColTarget [scanCol];
          if  (scanColPerDispColRatio [scanCol] == 1.0f)
          {
            displayRow[dispCol] += (byte)(scanLineFrac * (float)(pixelValue));
          }
          else
          {
            displayRow[dispCol] += (byte)(scanLineFrac * scanColPerDispColRatio [scanCol] * (float)(pixelValue));
            float leftOver = 1.0f - scanColPerDispColRatio [scanCol];
            displayRow[dispCol + 1] += (byte)(scanLineFrac * leftOver * (float)(pixelValue));
          }
        }

        scanCol++;
      }
    }  /* UpdateDisplayRow */





    public   void  GetDisplayRows  (long      displayRowStart,
                                    long      displayRowEnd,
                                    byte[][]  raster
                                   )
    {
      int x = 0;
      int y = 0;

      double  startScanRow = displayRowStart     * (double)ratio;
      double  endScanRow   = (displayRowEnd + 1) * (double)ratio;

      int  startScanRowInt = (int)Math.Floor (startScanRow);
      int  endScanRowInt   = (int)Math.Floor (endScanRow);

      long  numDisplayRows = displayRowEnd - displayRowStart + 1;

      byte[][]    scanRows = GetScanLines  (startScanRowInt, endScanRowInt);

      //kak   When weOwnSipperFile resize from Max Screen when displaying from PicesInterface Comander weOwnSipperFile sometimes Get no Data WHY?
      if  (scanRows == null)
        return;


      ushort[][]  displayRows = new ushort[numDisplayRows][];
      for  (x = 0;  x < numDisplayRows;  x++)
        displayRows[x] = new ushort[pixelsPerDisLine];

      int     scanRow = 0;
      double  curDisplayRow  = (float)startScanRowInt / ratio;
      double  nextDisplayRow = curDisplayRow + dispRowsPerScanRow;

      int    curDisplayRowInt  = (int)Math.Floor (curDisplayRow);
      int    nextDisplayRowInt = (int)Math.Floor (nextDisplayRow);

      int    scanRowZeroed = 0;

      for  (scanRow = startScanRowInt;  scanRow <= endScanRowInt;  scanRow++)
      {
        curDisplayRow = (double)scanRow / (double)ratio;
        nextDisplayRow = curDisplayRow + dispRowsPerScanRow;
        curDisplayRowInt  = (int)Math.Floor (curDisplayRow);
        nextDisplayRowInt = (int)Math.Floor (nextDisplayRow);
     
        try
        {
          if  (curDisplayRowInt == nextDisplayRowInt)
          {
            if  (curDisplayRowInt <= displayRowEnd)
              UpdateDisplayRow (scanRows[scanRowZeroed], displayRows[curDisplayRowInt - displayRowStart], 1.0f);
          }
          else
          {
            double  fracNextLogicalRow = (nextDisplayRow - (double)nextDisplayRowInt) / (double)dispRowsPerScanRow;
            double  fracCurLogicalRow  = 1.0 - fracNextLogicalRow;
            if  (curDisplayRowInt >= displayRowStart)
            {
              if  (scanRowZeroed >= scanRows.Length)
                MessageBox.Show ("'scanRowZeroed'  is too large");
              
              long  displayRowToUpdate = curDisplayRowInt - displayRowStart;
              if  (displayRowToUpdate < displayRows.Length)
                UpdateDisplayRow (scanRows[scanRowZeroed], displayRows[displayRowToUpdate], fracCurLogicalRow);
            }
            if  (nextDisplayRowInt <= displayRowEnd)
            {
              UpdateDisplayRow (scanRows[scanRowZeroed], displayRows[1 + curDisplayRowInt - displayRowStart], fracNextLogicalRow);
            }
          }
        }
        catch  (Exception  e)
        {
          MessageBox.Show (e.ToString (), "GetDisplayRows");
        }

        scanRowZeroed++;
      }
      
      float ratioSquared = ratio * ratio;
      
      for  (x = 0;  x < numDisplayRows;  x++)
      {
        ushort[]  oneDisplayRow = displayRows[x];
        byte[]    oneRasterRow  = raster[x];  
        for  (y = 0;  y < pixelsPerDisLine;  y++)
          oneRasterRow[y] = (byte)(0.5f + (float)oneDisplayRow[y] / ratioSquared);
      }


      return;
    } /* GetDisplayRows */                               
  
  


    public  int  ScanColumnFromDisplayCol (int displayColumn)
    {
      int  scanCol = (int)(0.5f + (float)displayColumn / (float)pixelsPerDisLine * (float)croppedWidth) + cropColLeft;
      return  scanCol;
    }

 
  }
}
