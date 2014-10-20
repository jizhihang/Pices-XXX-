using System;

namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataReport.
	/// </summary>
	public class InstrumentDataReport
	{
    private string                 curLine    = "";
    private byte                   deviceId   = 0;
    private char                   lastEolCh  = '\0';
    private InstrumentDataManager  manager    = null;

    public  InstrumentDataManager  Manager ()  {return manager;}


		public InstrumentDataReport (InstrumentDataManager  _manager,
		                             byte                   _deviceId
		                            )
		{
      manager  = _manager;
      deviceId = _deviceId;
		}



    public void  Dispose ()
    {
    }  /* Dispose */


    public  string  CurLine ()  {return curLine;}


    public void  ReportData (String  txt)
    {
      int  x;
      for  (x = 0;  x < txt.Length;  x++)
        ReportData (txt[x]);
    }  /* ReportData */



    public  void  ReportData (char  ch)
    {
      if  ((ch == '\n')  ||  (ch == '\r'))
      {
        if  (((ch == '\n')  &&  (lastEolCh == '\r'))  ||
             ((ch == '\r')  &&  (lastEolCh == '\n'))
            )
        {
          // We have a sequence of 'cr','lf'  or  'lf','cr'  we can ignore this
          // character because the prev char would have forced a 'eol'
          lastEolCh = '\0';
        }
        else
        {
          // We are at the end of the text line,  we can now try parsing it.
          manager.LogTxtLine (deviceId, curLine);
          try
          {
            ParseTxtLine (curLine);
          }
          catch  (Exception  e4)
          {
            manager.LogTxtLine (deviceId, "Exception occured during call to 'ParseTxtLine'." + "\n\n" + e4.ToString ());
          }
          curLine = "";
          lastEolCh = ch;
        }
      }
      else
      {
        // Add character to end of current line
        curLine += ch;
        //if  (curLine.Length > 200)
        if  (curLine.Length > 150)
        {
          // We are not getting any '\r' or '\n' characters from this 
          // instrument.  The specialized device is going to have to 
          // deal with it.
          try
          {
            ParseTxtBlock (ref curLine);
          }
          catch  (Exception e3)
          {
            manager.LogTxtLine (deviceId, "Exception occured during call to 'ParseTxtBlock'." + "\n\n" + e3.ToString ());
            curLine = "";
          }
          lastEolCh = ch;
        }
      }
    }  /* ReportData */




    virtual  public  void   ParseTxtBlock (ref String  curLine)
    {
      // We have no idea what to do with this data so we will ignore it !!!

    }



    virtual  public  void  ReportData (bool  text,
                                       char  ch,
                                       int   curScanLine
                                      )
    {
      if  (text)
      {
        ReportData (ch);
      }
      else
      {
        string  t = curScanLine.ToString () + ":" + ((byte)ch).ToString ();
        manager.LogTxtLine (deviceId, t);
      }
    }  /* ReportData */



    virtual public void  ParseTxtLine (string  txtLine)
    {

    }

  }
}
