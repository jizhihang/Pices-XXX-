using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;



namespace SipperFile
{
  public class SipperFileSimple: SipperBaseFile
  {
    public  SipperFileSimple (string                _fileName,
                              InstrumentDataManager _instrumentDataManager
                             ): 
                             base (_fileName, _instrumentDataManager)
    {
    }


    public  SipperFileSimple (Stream                _stream,
                              InstrumentDataManager _instrumentDataManager
                             ): 
                              base (_stream, _instrumentDataManager)
    {
    }




    public  override void  Dispose ()    
    {
      base.Dispose ();
    } /* Dispose */


    


    override public int  LengthNextScanLine ()
    {
      uint  scanLineLen = GetNextUint ();
      if  (scanLineLen > 4096)
        return 0;

      byte[]  buff = GetNextBuff (scanLineLen);
      return  (int)scanLineLen;
    }  /* LengthNextScanLine */   




    override public  void  SkipNextScanLine()
    {
      uint  scanLineLen = GetNextUint ();
      byte[]  buff = GetNextBuff (scanLineLen);
    }  /* SkipNextScanLine */   



    override public  void  ReadNextScanLine (byte[]  scanLine)
    {
      int x = 0;
      uint  scanLineLen = GetNextUint ();
      for  (x = 0;  x < scanLineLen;  x++)
      {
        byte b = GetNextByte ();
        if  (x < scanLine.Length)
          scanLine[x] = b;
      }

      while  (x < scanLine.Length)
      {
        scanLine[x] = 0;
        ++x;
      }
    }  /* ReadNextScanLine */   
  }  /* SipperFileSimple */
}
