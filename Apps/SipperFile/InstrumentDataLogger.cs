using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using System.Text;

//*************************************************************************
//*   The default logger that is to be used by InstrumentDataManager.  If *
//*  you want to have more control over the instrumentation data then     *
//*  sub-class from this class and overide the 'xxxxxxx' method.          *
//*************************************************************************


namespace SipperFile
{


  public  class  InstrumentDataLogger
  {
    public  delegate  void  ReportLineCallBack (byte    deviceId, 
                                                string  txt
                                              );


    public InstrumentDataLogger ()
    {
      logFileName = "";
      logFile = null;
    }



    public  InstrumentDataLogger (string  _logFileName)
    {
      logFileName = _logFileName;
      try 
      {
        logFile = new StreamWriter (logFileName, true);
        weOwnLogFile = true;
      }
      catch  (Exception)
      {
        MessageBox.Show ("Error opening LogFile[" + logFileName + "]");
        logFile = null;
      }
    }




    public InstrumentDataLogger (StreamWriter _logFile)
    {
      logFile = _logFile;
    }
  


    public  InstrumentDataLogger (ReportLineCallBack _reportLineCallBack)
    {
      reportLineCallBack = _reportLineCallBack;
      logFileName = "";
      logFile = null;
    }



    public void  Dispose ()
    {
      if  (weOwnLogFile  &&  (logFile != null))
      {
        logFile.Close ();
        logFile = null;
      }
    }


  
    public virtual void  
              LogInstrumentData (byte    deviceId,
                                 string  txt
                                )
    {
      if  (logFile != null)
      {
        logFile.WriteLine (deviceId.ToString ("#0") + ":" + txt);
        logFile.Flush ();
      }
      
      if  (reportLineCallBack != null)
        reportLineCallBack (deviceId, txt);
      
    }  /* LogInstrumentData */
    
    private  StreamWriter        logFile             = null;
    private  string              logFileName         = "";
    private  ReportLineCallBack  reportLineCallBack  = null;
    private  bool                weOwnLogFile        = false;
  }  /* InstrumentDataLogger */
}
