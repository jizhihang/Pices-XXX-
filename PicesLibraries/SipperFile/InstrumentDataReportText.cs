using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace SipperFile
{
	/// <summary>
	/// Summary description for InstrumentDataReportInclination.
	/// </summary>
	public class InstrumentDataReportText: InstrumentDataReport
	{
    private  List<string>  messages = null;


    public  List<string>  Messages ()  {return  messages;}


    public  string  AllMessages ()
    {
      string  msg = "";
      
      for  (int x = 0;  x < messages.Count;  x++)
      {
        if  (x < 1)
          msg += messages[x];
        else
          msg += "\t" + messages[x];
      }
      
      if  (msg == "")
        msg = CurLine();
      else
        msg += "\t" + CurLine ();
        
      return  msg;
    }  /* AllMessages */


		public  InstrumentDataReportText (InstrumentDataManager  _manager,
		                                  byte                   _deviceId
		                                 ):
          base (_manager, _deviceId)
		{
		  messages = new List<string> ();
		}


    override  public void  ParseTxtLine (string  txtLine)
    {
      messages.Add (txtLine);
    }  /* ParseTxtLine */


    
	}  /* InstrumentDataReportText */
}
