using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Windows.Forms;

using PicesInterface;


namespace SipperFile
{
  public class Sipper3File: SipperBaseFile
  {
    static  private  Sipper3DecodeRec[]  decodeRecs = null;



    public  Sipper3File (string                _fileName,
                         InstrumentDataManager _instrumentDataManager
                        ): 
                          base (_fileName, _instrumentDataManager)
    {
      if  (decodeRecs == null)
        BuildDecodeRecs ();
    }


    public  Sipper3File (Stream                _stream,
                         InstrumentDataManager _instrumentDataManager
                        ): 
                          base (_stream, _instrumentDataManager)
    {
      if  (decodeRecs == null)
        BuildDecodeRecs ();
    }




    public  override void  Dispose ()    
    {
      base.Dispose ();
    } /* Dispose */



    static  private  void  BuildDecodeRecs ()
    {
      decodeRecs = new Sipper3DecodeRec[65536];
      uint x = 0;
      for (x = 0; x < decodeRecs.Length; x++)
        decodeRecs[x] = new Sipper3DecodeRec((ushort)x);
    }  /* BuildDecodeRecs */




    override public int  LengthNextScanLine ()
    {
      bool eol = false;

      int  scanLineLen = 0;

      ushort rec = GetNextRecord();
      
      int  recCount = 0;

      while  ((!eof) && (!eol))
      {
        recCount++;
        Sipper3DecodeRec dr = decodeRecs[rec];
        if  (dr.ImageData())
        {
          scanLineLen += dr.PixelSpaceNeeded();
          if  (dr.Eol())
            eol = true;
          else
            rec = GetNextRecord();
        }
        else
        {
          rec = GetNextRecord();
        }
      }

      return scanLineLen;
    }  /* LengthNextScanLine */   




    override public  void  SkipNextScanLine()
    {
      bool eol = false;

      ushort  rec = GetNextRecord ();

      while  ((!eof)  &&  (!eol))
      {
        Sipper3DecodeRec dr = decodeRecs[rec];
        if  (dr.ImageData ())
        {
          if  (dr.Eol ())
            eol = true;
          else 
            rec = GetNextRecord ();
        }
        else
        {
          if  (instrumentDataManager != null)
          {
            instrumentDataManager.ReportData (dr, (int)lastScanLineNum);
          }
          rec = GetNextRecord();
        }
      }
    }  /* SkipNextScanLine */   



    override public  void  ReadNextScanLine (byte[]  scanLine)
    {
      bool eol = false;
      int  idx = 0;
      
      bool  bwImageryFound = false;

      ushort  rec = GetNextRecord ();
      
      while  ((!eof)  &&  (!eol))
      {
        Sipper3DecodeRec dr = decodeRecs[rec];
        if  (dr.ImageData ())
        {
          if  (!dr.GrayScale ())
            bwImageryFound = true;

          dr.AddToScanLine (scanLine, ref idx);
          if  (dr.Eol ())
            eol = true;
          else 
            rec = GetNextRecord ();
        }
        else
        {
          if  (instrumentDataManager != null)
          {
            instrumentDataManager.ReportData (dr, (int)lastScanLineNum);
          }
          rec = GetNextRecord();
        }
      }

      if  (bwImageryFound)
      {
        scanLine[10] = 255;
        scanLine[11] = 255;
        scanLine[12] = 255;
        scanLine[13] = 255;
        scanLine[14] = 255;
        scanLine[15] = 255;
        scanLine[16] = 255;
        scanLine[17] = 255;
        scanLine[18] = 255;
        scanLine[19] = 255;
        scanLine[20] = 255;
        scanLine[21] = 255;
        scanLine[22] = 255;
        scanLine[23] = 255;
        scanLine[24] = 255;
        scanLine[25] = 255;
        scanLine[26] = 255;
        scanLine[27] = 255;
        scanLine[28] = 255;
        scanLine[29] = 255;
      }

    }  /* ReadNextScanLine */



    public  void  SaveRawCtdData (StreamWriter   sw,
                                  PicesMsgQueue  msgQueue,
                                  ref bool       successful
                                 )
    {
      ulong  bytesRead = 0;
      ulong  bytesWritten = 0;
      ushort  rec = GetNextRecord ();

      ulong  tenMeg = 10 * 1024 * 1024;

      while  (!eof)
      {
        bytesRead += 2;

        if  ((msgQueue != null)  &&  ((bytesRead % tenMeg) == 0))
        {
          msgQueue.AddMsg ("Bytes Read: "        + bytesRead.ToString    ("##,###,###,##0") + "\t" +
                           "Ctd-Data-Written: "  + bytesWritten.ToString ("##,###,###,##0")
                          );
        }

        //if  (bytesRead >=  377711000)
        {
          Sipper3DecodeRec dr = decodeRecs[rec];
          if  (!dr.ImageData ())
          {
            byte  deviceId = dr.SensorNum ();
            if  ((deviceId == 16)  &&  dr.Text ())
            {
              sw.Write (dr.SensorData ());
              ++bytesWritten;
            }
          }
        }
        rec = GetNextRecord();
      }

      if  (msgQueue != null)
      {
        msgQueue.AddMsg ("");
        msgQueue.AddMsg ("Done Scanning");
        msgQueue.AddMsg ("Bytes Read: "        + bytesRead.ToString    ("##,###,###,##0") + "\t" +
                         "Ctd-Data-Written: "  + bytesWritten.ToString ("##,###,###,##0")
                        );
      }

      successful = true;
    }  /* SaveRawCtdData */



  }  /* Sipper3File */
}
