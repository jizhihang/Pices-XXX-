using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  /// <summary>
  /// This class aids in the decoding of SIPPER 3 data files.  For each record extracted from the 
  /// SIPPEr file you create an instance of this class.  You can then use the provide access methods 
  /// to retrieve the decoded version.  'Sipper3File' will create an array of these classes.  One 
  /// for each possible bit combination (65536).
  /// </summary>
  public class  Sipper3DecodeRec
  {
    enum OPCode
    {
      Null,
      FillInBlankPixels,
      RawPixels,
      SensorData,
      SensorRelatedData
    }

    private  bool     eol             = false;
    private  bool     image           = true;
    private  OPCode   operation       = OPCode.Null;
    private  bool     grayScale       = true;
    private  bool     raw             = true;
    private  ushort   numBlankPixels  = 0;
    private  byte[]   pixels          = null;
    private  byte     sensorNum       = 0;
    private  char     sensorData      = ' ';
    private  bool     text            = false;

    public bool  Eol        () {return eol;}
    public bool  ImageData  () {return image;}
    public bool  GrayScale  () {return grayScale;}
    public char  SensorData () {return sensorData;}
    public byte  SensorNum  () {return sensorNum;}
    public bool  Text       () {return text;}


    /// <summary>
    /// Sipper3DecodeRec Constructor.  Will decode 'd' parameter.<see cref="Sipper3DecodeRec"/> class.
    /// </summary>
    /// <param name="d">An unsigned short that represents a Sipper3File record.</param>
    public  Sipper3DecodeRec (ushort  d)
    {
      bool[] bits = DecodeUshortIntoBits (d);
      image     = bits[15];
      eol       = bits[14];
      raw       = bits[13];
      grayScale = bits[12];
      
      if  (image && (!raw))
      {
        // Fill in blanks spaces
        operation = OPCode.FillInBlankPixels;
        numBlankPixels = (ushort)((d & (ushort)0x07ff) * 4);
      }
      
      else if  (image && raw && (!grayScale))
      {
        int  x = 11;
        int  y = 0;
        operation = OPCode.RawPixels;
        pixels = new byte[12];
        for  (x = 11; x >= 0;  x--, y++)
        {
          pixels[y] = (byte)(bits[x] ? 255 : 0);
        }
      }
      
      else if  (image  &&  raw  &&  grayScale)
      {
        byte[]  grayScalValues = {0, 36, 73, 109, 146, 182, 219, 255};
      
        operation = OPCode.RawPixels;
        pixels = new byte[4];
        pixels[0] = grayScalValues[ThreeBitsToByte (bits[11], bits[10], bits[ 9])];
        pixels[1] = grayScalValues[ThreeBitsToByte (bits[ 8], bits[ 7], bits[ 6])];
        pixels[2] = grayScalValues[ThreeBitsToByte (bits[ 5], bits[ 4], bits[ 3])];
        pixels[3] = grayScalValues[ThreeBitsToByte (bits[ 2], bits[ 1], bits[ 0])];
      }
      
      else if  ((!image)  &&  (!eol))
      {
        operation  = OPCode.SensorData;
        sensorNum  = SixBitsToByte (bits[13], bits[12], bits[11], bits[10], bits[9], bits[8]);
        sensorData = (char)(d & 0x00ff);
        eol = false;
      }

      else if ((!image) && eol)
      {
        operation  = OPCode.SensorRelatedData;
        sensorNum  = SixBitsToByte(bits[13], bits[12], bits[11], bits[10], bits[9], bits[8]);
        sensorData = (char)(d & 0x00ff);
        text       = eol;
        eol        = false;
      }
    }



    public  void  Dispose ()
    {
    }

    
    
    private  byte  ThreeBitsToByte (bool bit2,
                                    bool bit1,
                                    bool bit0
                                   )
    {
      byte  result = (byte)((bit2 ? 4 : 0)  +  (bit1 ? 2 : 0)  +  (bit0 ? 1 : 0));
      return  result;
    }   /* ThreeBitsToByte */



    private  byte  SixBitsToByte (bool bit5,
                                  bool bit4,
                                  bool bit3,
                                  bool bit2,
                                  bool bit1,
                                  bool bit0
                                 )
    {
      byte result = (byte)((bit5 ? 32 : 0) +
                           (bit4 ? 16 : 0) +
                           (bit3 ?  8 : 0) +
                           (bit2 ?  4 : 0) +
                           (bit1 ?  2 : 0) +
                           (bit0 ?  1 : 0));
      return  result;
    } /* SixBitsToByte */




    /// <summary>
    /// Decodes the ushort into 16 bits expressed as an bool[15].
    /// </summary>
    /// <param name="d">Unsigned Short that you want to decode into bits.</param>
    /// <returns>bool[16] where each element represents the coresponding bit in 'd'.</returns>
    private  bool[]  DecodeUshortIntoBits (ushort d)
    {
      bool[]  results = new bool[16];
      
      results[ 0] = ((d & 0x0001) != 0);
      results[ 1] = ((d & 0x0002) != 0);
      results[ 2] = ((d & 0x0004) != 0);
      results[ 3] = ((d & 0x0008) != 0);
      results[ 4] = ((d & 0x0010) != 0);
      results[ 5] = ((d & 0x0020) != 0);
      results[ 6] = ((d & 0x0040) != 0);
      results[ 7] = ((d & 0x0080) != 0);
      results[ 8] = ((d & 0x0100) != 0);
      results[ 9] = ((d & 0x0200) != 0);
      results[10] = ((d & 0x0400) != 0);
      results[11] = ((d & 0x0800) != 0);
      results[12] = ((d & 0x1000) != 0);
      results[13] = ((d & 0x2000) != 0);
      results[14] = ((d & 0x4000) != 0);
      results[15] = ((d & 0x8000) != 0);
      return  results;
    }  /* DecodeUshortIntoBits */



    /// <summary>
    /// determines the number of pixels that will be needed in the scan line to accomidate this decoded SipperRec.
    /// You would call this method before calling 'AddToScanLine' to make sure there is enough romm in can line.
    /// <see cref="AddToScanLine"/> 
    /// </summary>
    /// <returns>Number of pixels required on scan line.</returns>
    public int  PixelSpaceNeeded ()
    {
      switch (operation)
      {
        case OPCode.RawPixels:          return  pixels.Length;
        case OPCode.FillInBlankPixels:  return  numBlankPixels;
        case OPCode.SensorData:         return  0;
      }
      
      return  0;
    }  /* AddToScanLine */



    /// <summary>
    /// Add the image data stored in the SIPPER 3 data record to a scanLine.  If the 
    /// SIPPER3 rec a Instrument data record then nothing will happen.  The 'idx' 
    /// parameter will be adjused t its new position,
    /// </summary>
    /// <param name="scanLine">Scanline to add image data to.</param>
    /// <param name="idx">The next index in ScanLin to append image data to.</param>
    public  void  AddToScanLine (byte[]   scanLine,
                                 ref int  idx
                                )
    {
      switch  (operation)
      {
        case OPCode.RawPixels:
        {
          if  (!grayScale)
            grayScale = false;

          int  x = 0, y = idx;
          for  (x = 0;  (x < pixels.Length)  &&  (y < scanLine.Length);  x++, y++)
            scanLine[y] = pixels[x];
          idx += pixels.Length;
        }
        break;
        
        case OPCode.FillInBlankPixels:
        {
          int  x = 0;
          int  z = Math.Min (scanLine.Length, (idx + numBlankPixels));
          for  (x = idx;  x < z;  x++)
            scanLine[x] = 0;
          idx += numBlankPixels;
        }
        break;

        case OPCode.SensorData:
        {
        }
        break;
        
        case OPCode.SensorRelatedData:
        {
        }
        break;
      }

      if  (idx > 4095)
      {
        int zed = 100;
      }

      if (idx > 4120)
      {
        int zed2 = 100;
      }
    }  /* AddToScanLine */
  }  /* Sipper3DecodeRec */
}
