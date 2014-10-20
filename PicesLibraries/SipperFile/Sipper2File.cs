using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;



namespace SipperFile
{
  public class Sipper2File: SipperBaseFile
  {
    static  private  Sipper2DecodeRec[]  decodeRecs = null;

   

    public  Sipper2File (string                _fileName,
                         InstrumentDataManager _instrumentDataManager
                        ): 
                          base (_fileName, _instrumentDataManager)
    {
      if  (decodeRecs == null)
        BuildDecodeRecs ();
    }


    public  Sipper2File (Stream                  _stream,
                         InstrumentDataManager   _instrumentDataManager
                        ): 
                          base (_stream, _instrumentDataManager)
    {
      if  (decodeRecs == null)
        BuildDecodeRecs ();
    }



    public  override void Dispose ()    
    {
      base.Dispose ();
    } /* Dispose */




    static  private  void  BuildDecodeRecs ()
    {
      decodeRecs = new Sipper2DecodeRec [65535];
      ushort x = 0;
      for (x = 0; x < decodeRecs.Length; x++)
        decodeRecs[x] = new Sipper2DecodeRec (x);
    }  /* BuildDecodeRecs */




    override public int  LengthNextScanLine ()
    {
      bool eol = false;

      int  scanLineLen = 0;

      ushort rec = GetNextRecord();

      while ((!eof) && (!eol))
      {
        Sipper2DecodeRec dr = decodeRecs[rec];
        scanLineLen += dr.PixelSpaceNeeded();
        if (dr.Eol())
          eol = true;
        else
          rec = GetNextRecord();
      }

      return scanLineLen;
    }  /* LengthNextScanLine */   




    override public  void  SkipNextScanLine()
    {
      bool eol = false;
      
      ushort  rec = GetNextRecord ();
      
      while  ((!eof)  &&  (!eol))
      {
        Sipper2DecodeRec dr = decodeRecs[rec];
        if  (dr.Eol ())
          eol = true;
        else 
          rec = GetNextRecord ();
      }
    }  /* SkipNextScanLine */   




    override public  void  ReadNextScanLine (byte[]  scanLine)
    {
      bool eol = false;
      int  idx = 0;
      
      ushort  rec = GetNextRecord ();
      
      while  ((!eof)  &&  (!eol))
      {
        Sipper2DecodeRec  dr = decodeRecs[rec];
        dr.AddToScanLine (scanLine, ref idx);
        if  (dr.Eol ())
          eol = true;
        else 
          rec = GetNextRecord ();
      }     
    }  /* ReadNextScanLine */   

  }  /* Sipper2File */
}
