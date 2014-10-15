using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Windows.Forms;



namespace SipperSimulator
{
  class SipperCamera
  {
    private Thread  monitor           = null;
    private bool    monitorRunning    = false;
    private bool    monitorTerminate  = false;
    
    private bool    recording         = false;

    private int     blockStart        = 0;
    private int     blockEnd          = 99999999;
    private int     blockCurrent      = 0;
    
    private Random  frRandomGen       = new Random ();

    private string  baseFileName         = "";
    private int     baseFileSeqNum       = 0;
    private string  fileDesc             = "";
    private string  fileName             = "";
    private int     fileNameCounterIndex = 0;


    public  SipperCamera ()
    {
      monitorRunning   = false;
      monitorTerminate = false;

      monitor = new Thread (new ThreadStart (Monitor));
      monitor.Name = "SipperCameraStatus";
      monitor.Start ();
    }


    public void  Dispose ()
    {
      if  (monitor != null)
      {
        if  (monitor.IsAlive)
        {
          monitor.Abort ();
          Thread.Sleep (100);
        }
        monitor = null;
      }
    }  /* Dispose */



    public  bool  MonitorRunning ()  
    {
      bool  stillRunning = true;
      lock (this)
      {
        stillRunning = monitorRunning;
      }
      
      return  stillRunning;
    }



    public void   TerminateMonitorThreadReq ()
    {
      lock (this)
      {
        monitorTerminate = true;
      }
      
    }  /* TerminateMonitorThread */


  
    public  string  StartRecording ()
    {
      string  result ="";
      
      if  (recording)
        return  "";
     
      lock  (this)
      {
        recording = true;
        blockStart = blockCurrent;
      }

      string  numStr = "_" + baseFileSeqNum.ToString ("00");
      
      fileName = baseFileName + numStr;
      
      /*
       result = "\n\n\n" +
               "fileNameBase = "           + baseFileName          + ", " +
               "numberString = "           + numStr                + ", " +
               "fileNameCounterIndex = "   + fileNameCounterIndex  +
               "\n\n" +
               "SIPPER_FILENAME_SIZE = 30" +
               "\n\n" +
               "fileNameBase now = "       + fileName              +          
               "\n\n" +
               "Opening "                  + fileName              + ".  " +
               "StartLBA= "                + HexStr (blockStart)   + ", "  +
               "EndLBA= "                  + HexStr (blockEnd)     +
               "\n\n";
      */

      result = "\n\n\n" +
               "Opening " + fileName + ".  StartLBA= " + HexStr (blockStart) + ", EndLBA= " + HexStr (blockEnd) +
               "\n\n";

      // Opening sip_08_100m_01.  StartLBA= 001F98E2, EndLBA= 0BA50E41


/*
fileNameBase = ImgTest_filename, numberString = _01, fileNameCounterIndex = 7

SIPPER_FILENAME_SIZE = 30

fileNameBase now = ImgTest_01

Opening ImgTest_01.  StartLBA= 001F29C0, EndLBA= 0BA50E41
*/

      fileNameCounterIndex++;
      baseFileSeqNum++;

      return  result;
    }  /* StartRecording */






    public string  StopRecording ()
    {
      string  result =  "";
      if (!recording)
        return result;

      lock (this)
      {
        recording = false;
      }

      result = "\n\n" +
               "Writing FCB at LBA " + HexStr (blockStart) + "." +
               "\n\n" +
               "File Closed." +
               "\n\n";
                    
      return  result;
    }  /* StopRecording */


    
    
    public void  SetFileName (string _fileName)
    {
      baseFileName = _fileName;
      baseFileSeqNum = 0;
    }
        
    
    public void  SetFileDesc (string _fileDesc)
    {
      fileDesc = _fileDesc;
    }


    public string  HexStr (int a)
    {
      uint uiDecimal = 0;

      try
      {
        uiDecimal = checked((uint)System.Convert.ToUInt32 (a));
      }
      catch (System.OverflowException exception) 
      {
        // Show overflow message and return
        MessageBox.Show ("Overflow\n"+exception);				
      }

      // Format unsigned integer value to hex and show in another textbox
      string  s = String.Format ("{0:x2}", uiDecimal);
      if  (s.Length < 8)
        s = "0" + s;
        
      return  s;
     }  /* HexStr */



/*
bs=00006733 bc=00006B2B be=0BA50E41 fr=066 da=01/05/2007 ti=10:14:15 rs=r 
*/
    public  string  StatusStr ()
    {
      int  flowRate = frRandomGen.Next (200);
      DateTime d = DateTime.Now;
    
      string  s;
      lock (this)
      {
        s = "bs=" + HexStr   (blockStart)    + " " +
            "bc=" + HexStr   (blockCurrent)  + " " +
            "be=" + HexStr   (blockEnd)      + " " +
            "fr=" + flowRate.ToString ("000")   + " " +
            "da=" + d.Month.ToString  ("00")    + "/" +
                    d.Day.ToString    ("00")    + "/" +
                    d.Year.ToString   ("0000")  + " " +
            "ti=" + d.Hour.ToString   ("00")    + ":" +
                    d.Minute.ToString ("00")    + ":" +
                    d.Second.ToString ("00")    + " "  +
            "rs=" + (recording ? "r" : "s");
      }
      
      return  s;
    }



    public  void  Monitor ()
    {
      monitorRunning = true;
      
      while  (true)
      {
        if  (monitorTerminate)
          break;
          
        lock (this)
        {
          if  (recording)
          {
            blockCurrent += 1000;
          }
        }
        
        Thread.Sleep (100);
      }
          
      
      monitorRunning = false;
    
    }  /* Monitor */
  }
}
