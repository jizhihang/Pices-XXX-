using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using System.Collections.Specialized;

using PicesInterface;


namespace SipperFile
{
  public class SipperBuff4Bit: SipperBaseFile
  {

    private  Byte[]  convTable4BitTo8Bit = null;
    private  Byte[]  convTable8BitTo4Bit = null;
    private  Byte[]  compensationTable   = null;


    private  List<char>  headerText = null;


    public class   DecodeOpRec
    {
      public   uint  opCode;      //  Bits 0 thru 3
      public   uint  bits5Thru7;  //  Bits 4 thru 7

      //  For OpCodes 4 thru 7
      public   bool  oneByteRunLenRec = false; 

      public   uint   runLength     = 0;
      public   Byte   runLengthChar = 0;
      public   uint   textBlockLenHighByte = 0;  // If text block;  containg 1st 4 bits of length.
      public   uint   rawStringLen = 0;
      public   uint   rawStringBytesToRead = 0;
      public   Byte   rawPixel = 0;

      public   uint   bytesToReadHighOrder = 0;


      
      public DecodeOpRec (Byte    b,
                          Byte[]  convTable4BitTo8Bit
                         )
      {
        ByteIntoHalfs (b, out bits5Thru7, 
                          out opCode
                      );

        switch  (opCode)
        {
          case  0:  // End of Line
            break;

          case  1:  // Text Block
            {
              textBlockLenHighByte = bits5Thru7 * 256;
            }
            break;


          case  2:  // Instrument Data
            break;

          case  3:
            break;  // Unused.

          case  4:  // Run Length of 2
          case  5:  // Run Length of 3
          case  6:  // Run Length of 4
          case  7:  // Run Length of 5
          case  8:  // Run Length of 6
          case  9:  // Run Length of 7
            {
              oneByteRunLenRec = true;
              runLength = opCode - 2;
              runLengthChar = convTable4BitTo8Bit[bits5Thru7];
            }
            break;

          case 10:
            {
              // Will need to get next byte to determine runLength;
              runLength = 0;
              runLengthChar = convTable4BitTo8Bit[bits5Thru7];
            }
            break;


          case  11:  // Raw-String (1 Pixel Length).
            {
              rawStringLen = 1;
              rawStringBytesToRead = 0;
              rawPixel = convTable4BitTo8Bit[bits5Thru7];
            }
            break;


          case  12:   // Raw-String (Even length 2 thru 32).
            {
              rawStringBytesToRead = bits5Thru7 + 1;
              rawStringLen = rawStringBytesToRead * 2;
            }
            break;


          case  13:   // Raw-String (Odd Length 1 thru 513).
            {
              bytesToReadHighOrder = 16 * bits5Thru7;
              rawStringBytesToRead = 0;  // Have to read next byte to get number of additional Raw String bytes to read.
            }
            break;


          case  14:
            break;  // Unused.


          case  15:
            break;  // Unused.

        }
      }


      private  void  ByteIntoHalfs (Byte b,  
                                    out  uint  upper,   // bits 0 - 3
                                    out  uint  lower    // bits 4 - 7
                                   )
      {
        lower = b % 16u;
        upper = (uint)b / 16u;
        return;
      }
    };  /* DecodeOpRec */



    public  class  DecodeRawByte
    {
      public  byte   pixel0;
      public  byte   pixel1;
      public  byte   lower4Bits;
      public  byte   higher4Bits;


      public  DecodeRawByte (Byte    b,
                             Byte[]  convTable4BitTo8Bit
                            )
      {
        lower4Bits  = (byte)((uint)b % 16u);
        higher4Bits = (byte)((uint)b / 16u);

        pixel0 = convTable4BitTo8Bit[lower4Bits];
        pixel1 = convTable4BitTo8Bit[higher4Bits];
      }


      public  void  FillIntoScanLine (byte[]    scanLine,
                                      ref int   idx
                                     )
      {
        if  (idx < scanLine.Length)
        {
          scanLine[idx] = pixel0;
          ++idx;
        }

        if  (idx < scanLine.Length)
        {
          scanLine[idx] = pixel1;
          ++idx;
        }
      }
    };



    private  DecodeOpRec  []  decodeRecs     = null;
    private  DecodeRawByte[]  decodeRawBytes = null;




    
    public  SipperBuff4Bit (string                _fileName,
                            InstrumentDataManager _instrumentDataManager
                           ): 
                          base (_fileName, _instrumentDataManager)
    {
      BuildConversionTables ();
      BuildDecodeRecs ();
      ReadHeader ();
    }



    public  SipperBuff4Bit (Stream                _stream,
                            InstrumentDataManager _instrumentDataManager
                           ): 
                          base (_stream, _instrumentDataManager)
    {
      BuildConversionTables ();
      BuildDecodeRecs ();
      ReadHeader ();
    }




    public  override void  Dispose ()    
    {
      base.Dispose ();
    } /* Dispose */



    private  void  ReadHeader ()
    {
      List<char>  headerText = new List<char> ();

      byte ch = GetNextByte ();
      while  ((ch != 0)  &&  (!eof)  &&  (headerText.Count < 4096))
      {
        char c = Convert.ToChar (ch);
        headerText.Add (c);
        ch = GetNextByte ();
      }
    }



    private  void  BuildDecodeRecs ()
    {
      if  (convTable4BitTo8Bit == null)
        BuildConversionTables ();

      decodeRecs = new DecodeOpRec[256];
      int x = 0;
      for (x = 0;  x < decodeRecs.Length;  x++)
        decodeRecs[x] = new DecodeOpRec((Byte)x, convTable4BitTo8Bit);

      decodeRawBytes = new DecodeRawByte[256];
      for (x = 0;  x < decodeRawBytes.Length;  x++)
        decodeRawBytes[x] = new DecodeRawByte((byte)x, convTable4BitTo8Bit);
    }  /* BuildDecodeRecs */



    private  void  BuildConversionTables ()
    {
      int x = 0;
      int y = 0;
      int inc = 256 / 16;
      convTable4BitTo8Bit = new Byte[16];
      convTable8BitTo4Bit = new Byte[256];
      for  (x = 0;  x < 16;  ++x)
      {
        int  this8Bit = x * inc;
        int  next8Bit = (x + 1) * inc;

        int  fourBitTo8BitNum = (int)(this8Bit + (int)(((float)x / 16.0f)  * (float)inc));

        convTable4BitTo8Bit[x] = (Byte)fourBitTo8BitNum;
        for  (y = this8Bit;  y < next8Bit;  ++y)
           convTable8BitTo4Bit[y] = (byte)x;
      }

      compensationTable = new byte[256];
      for  (int pv = 0;  pv < 256;  ++pv)
      {
         Byte encodedValue = convTable8BitTo4Bit[pv];
         compensationTable[pv] = convTable4BitTo8Bit[encodedValue];
      }
    }  /* BuildConversionTables */





    override   public int  LengthNextScanLine ()
    {
      bool eol = false;

      uint  scanLineLen = 0;

      List<DecodeOpRec>  history = new List<DecodeOpRec> ();
      int[]  histogram = new int[16];
      for (int x = 0;  x < histogram.Length;  ++x)
        histogram[x] = 0;

      Byte rec =  GetNextByte ();
      
      int  recCount = 0;

      while  ((!eof) && (!eol))
      {
        recCount++;
        DecodeOpRec  dr = decodeRecs[rec];
        history.Add (dr);
        histogram[dr.opCode]++;

        switch  (dr.opCode)
        {
          case  0:  // End of Line
            eol = true;
            break;

          case  1:  // Text Block
            {
              ProcessTextBlock (dr);
            }
            break;


          case  2:  // Instrument Data
            {
              ProcessInstrumentData (dr);
            }
            break;

          case  3:
            break;  // Unused.

          case  4:  // Run Length of 2
          case  5:  // Run Length of 3
          case  6:  // Run Length of 4
          case  7:  // Run Length of 5
          case  8:  // Run Length of 6
          case  9:  // Run Length of 7
            {
              scanLineLen+= dr.runLength;
            }
            break;

          case 10:
            {
              // Will need to get next byte to determine runLength;
              uint  runLen = GetNextByte () + (uint)1;
              scanLineLen += runLen;
            }
            break;


          case  11:  // Raw-String (1 Pixel Length).
            {
              scanLineLen += 1;
            }
            break;


          case  12:   // Raw-String (Even length 2 thru 32).
            {
              GetNextBuff ((uint)dr.rawStringBytesToRead);
              scanLineLen += dr.rawStringLen;
            }
            break;


          case  13:   // Raw-String (Odd Length 1 thru 513).
            {
              byte  nextByte = GetNextByte ();
              uint  bytesToRead = (dr.bytesToReadHighOrder + decodeRawBytes[nextByte].lower4Bits) + 1;
              uint  rawStrLen = 1 + bytesToRead * 2;
              GetNextBuff ((uint)bytesToRead);
              scanLineLen += rawStrLen;
            }
            break;


          case  14:
            break;  // Unused.


          case  15:
            break;  // Unused.

        }

        if  (!eol)
        {
          rec =  GetNextByte ();
        }
      }

      return (int)scanLineLen;
    }  /* LengthNextScanLine */   



    override  public  void  SkipNextScanLine()
    {
      LengthNextScanLine ();
    }  /* SkipNextScanLine */   



    override  public  void  ReadNextScanLine (byte[]  scanLine)
    {
      bool eol = false;
      int  idx = 0;
      
      byte  rec = GetNextByte ();
      
      while  ((!eof)  &&  (!eol))
      {
        DecodeOpRec dr = decodeRecs[rec];
        switch  (dr.opCode)
        {
          case  0:  // End of Line
            eol = true;
            break;

          case  1:  // Text Block
            {
              ProcessTextBlock (dr);
            }
            break;


          case  2:  // Instrument Data
            {
              //
              ProcessInstrumentData (dr);
            }
            break;

          case  3:
            break;  // Unused.

          case  4:  // Run Length of 2
          case  5:  // Run Length of 3
          case  6:  // Run Length of 4
          case  7:  // Run Length of 5
          case  8:  // Run Length of 6
          case  9:  // Run Length of 7
            {
              int endIdx = idx + (int)dr.runLength - 1;
              if  (endIdx >= scanLine.Length)
                endIdx = scanLine.Length - 1;
              while  (idx <= endIdx)
              {
                scanLine[idx] = dr.runLengthChar;
                ++idx;
              }
            }
            break;

          case 10:
            {
              // Will need to get next byte to determine runLength;
              int runLen = GetNextByte () + 1;
              int endIdx = idx + runLen - 1;
              if  (endIdx >= scanLine.Length)
                endIdx = scanLine.Length - 1;
              while  (idx <= endIdx)
              {
                scanLine[idx] = dr.runLengthChar;
                ++idx;
              }
            }
            break;


          case  11:  // Raw-String (1 Pixel Length).
            {
              scanLine[idx] = dr.rawPixel;
              ++idx;
            }
            break;


          case  12:   // Raw-String (Even length 2 thru 32).
            {
              byte[]  buff = GetNextBuff ((uint)dr.rawStringBytesToRead);
              if  (buff != null)
              {
                for  (int x = 0;  x < buff.Length;  ++x)
                 decodeRawBytes[buff[x]].FillIntoScanLine (scanLine, ref idx);
              }
            }
            break;


          case  13:   // Raw-String (Odd Length 1 thru 513).
            {
              byte  nextByte = GetNextByte ();

              DecodeRawByte  rb = decodeRawBytes[nextByte];
              uint  bytesToRead = (dr.bytesToReadHighOrder + rb.lower4Bits) + 1;
              if  (idx < scanLine.Length)
              {
                scanLine[idx] = rb.pixel1;
              }

              byte[]  buff = GetNextBuff ((uint)bytesToRead);
              if  (buff != null)
              {
                for  (int x = 0;  x < buff.Length;  ++x)
                 decodeRawBytes[buff[x]].FillIntoScanLine (scanLine, ref idx);
              }
            }
            break;


          case  14:
            break;  // Unused.


          case  15:
            break;  // Unused.

        }

        if  (!eol)
        {
          rec =  GetNextByte ();
        }
      }
    }  /* ReadNextScanLine */



    private  char[]  ProcessTextBlock (DecodeOpRec  dr)
    {
      uint  lowerTextBlockLen = GetNextByte ();
      uint  textBlockLen = dr.textBlockLenHighByte + lowerTextBlockLen;

      byte[]  buff = GetNextBuff (textBlockLen);
      if  (buff == null)
      {
        char[]  tb = new char[1];
        tb[0] = ' ';
        return tb;
      }

      char[]  textBlock = new char[buff.Length];
      for (int x = 0;  x < buff.Length;  ++x)
        textBlock[x] = (char)buff[x];

      return  textBlock;
    }  /* ProcessTextBlock */




    private  void  ProcessInstrumentData (DecodeOpRec  dr)
    {
       UInt32  scanLine = GetNextUint ();
       UInt32  data     = GetNextUint ();
    }


  }  /* SipperBuff4Bit */
}

