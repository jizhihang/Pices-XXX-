using System;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
  public class  Sipper2DecodeRec
  {
    enum OPCode
    {
      Null,
      FillInBlankPixels,
      RawPixels,
    }

    private  OPCode  operation      = OPCode.Null;
    private  byte    cameraNum      = 0;
    private  bool    eol            = false;
    private  bool    flow           = false;
    private  bool    raw            = true;
    private  ushort  numBlankPixels = 0;
    private  byte[]  pixels         = null;


    static byte[] grayScalValues = { 0, 36, 73, 109, 146, 182, 219, 255};


    // From document emailed from Scott Samson  on Fri 10/11/2002 12:54 PM
    // Sipper Format Spec version 3.1
    
    //C# = 0 if camera 0 (4096 pixel Dalsa Camera)
    //C# = 1 if camera 1 (4096 pixel Dalsa Camera)
    //EOL = 1 if an end of line was encountered (ie. 4096th pixels on current line are fully recorded)
    //EOL = 0 if end of line NOT encountered (ie. This record contains pixels for pixel # <4096)
    //RAW = 1 if the DATA bits are raw (gray) encoded 
    //RAW = 0 if the DATA bits are compressed in run-length format
    //FLOW = 0 if external flowmeter is in position 0
    //FLOW = 1 if external flowmeter is in position 1
    //DATA11..0 = Data bits (described below)


    public byte  CameraNum ()  {return  cameraNum;}
    public bool  Eol       ()  {return  eol;}



    public  Sipper2DecodeRec (ushort  d)
    {
      bool[] bits = DecodeUshortIntoBits (d);
      
      if  (bits[15])
        cameraNum = 1;
      else
        cameraNum = 0;
      
      eol  = bits[14];
      raw  = bits[13];
      flow = bits[12];
      
      if  (raw)
      {
        operation = OPCode.RawPixels;
        pixels = new byte[4];
        pixels[0] = grayScalValues[ThreeBitsToByte (bits[11], bits[10], bits[ 9])];
        pixels[1] = grayScalValues[ThreeBitsToByte (bits[ 8], bits[ 7], bits[ 6])];
        pixels[2] = grayScalValues[ThreeBitsToByte (bits[ 5], bits[ 4], bits[ 3])];
        pixels[3] = grayScalValues[ThreeBitsToByte (bits[ 2], bits[ 1], bits[ 0])];
      }
      else
      {
        // Fill in blanks spaces
        operation = OPCode.FillInBlankPixels;
        numBlankPixels =  
           (ushort)
           (
            ((bits[11] ? 2048 : 0) +
             (bits[10] ? 1024 : 0) +
             (bits[ 9] ?  512 : 0) +
             (bits[ 8] ?  256 : 0) +
             (bits[ 7] ?  128 : 0) +
             (bits[ 6] ?   64 : 0) +
             (bits[ 5] ?   32 : 0) +
             (bits[ 4] ?   16 : 0) +
             (bits[ 3] ?    8 : 0) +
             (bits[ 2] ?    4 : 0) +
             (bits[ 1] ?    2 : 0) +
             (bits[ 0] ?    1 : 0)
            ) 
            * 4
           );
            
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
     

    
    
    private  bool[]  DecodeUshortIntoBits (ushort d)
    {
      bool[]  results = new bool[16];


      /*
      results[0] = ((d & 0x0001) != 0);
      results[1] = ((d & 0x0002) != 0);
      results[2] = ((d & 0x0004) != 0);
      results[3] = ((d & 0x0008) != 0);
      results[4] = ((d & 0x0010) != 0);
      results[5] = ((d & 0x0020) != 0);
      results[6] = ((d & 0x0040) != 0);
      results[7] = ((d & 0x0080) != 0);
      results[8] = ((d & 0x0100) != 0);
      results[9] = ((d & 0x0200) != 0);
      results[10] = ((d & 0x0400) != 0);
      results[11] = ((d & 0x0800) != 0);
      results[12] = ((d & 0x1000) != 0);
      results[13] = ((d & 0x2000) != 0);
      results[14] = ((d & 0x4000) != 0);
      results[15] = ((d & 0x8000) != 0);
      */

      results[ 8] = ((d & 0x0001) != 0);
      results[ 9] = ((d & 0x0002) != 0);
      results[10] = ((d & 0x0004) != 0);
      results[11] = ((d & 0x0008) != 0);
      results[12] = ((d & 0x0010) != 0);
      results[13] = ((d & 0x0020) != 0);
      results[14] = ((d & 0x0040) != 0);
      results[15] = ((d & 0x0080) != 0);
      results[ 0] = ((d & 0x0100) != 0);
      results[ 1] = ((d & 0x0200) != 0);
      results[ 2] = ((d & 0x0400) != 0);
      results[ 3] = ((d & 0x0800) != 0);
      results[ 4] = ((d & 0x1000) != 0);
      results[ 5] = ((d & 0x2000) != 0);
      results[ 6] = ((d & 0x4000) != 0);
      results[ 7] = ((d & 0x8000) != 0);
      return  results;
    }  /* DecodeUshortIntoBits */



    public int  PixelSpaceNeeded ()
    {
      switch (operation)
      {
        case OPCode.RawPixels:          return  pixels.Length;
        case OPCode.FillInBlankPixels:  return  numBlankPixels;
      }
      
      return  0;
    }  /* AddToScanLine */



    public  void  AddToScanLine (byte[]   scanLine,
                                 ref int  idx
                                )
    {
      switch  (operation)
      {
        case OPCode.RawPixels:
        {
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
      }      
    }  /* AddToScanLine */
  }  /* Sipper2DecodeRec */
}
