using System;
using System.Collections.Generic;
using System.Text;




namespace SipperFile
{
  class  InstrumentDataReportIllumination: InstrumentDataReport
  {
    public  const int  NumTurnSignalsToTrack = 5;  // This needs to be a odd number,  so that we track from a
                                                   // "rise to rise" and "fall to fall"
    public  const int  ScanLinesPerSecond    = 25000;


    public InstrumentDataReportIllumination (InstrumentDataManager _manager,
                                             byte                  _deviceId
                                            )
      :
        base (_manager, _deviceId)
    {
    }



  
    override  public void  ReportData (bool  text,
                                       char  ch,
                                       int   curScanLine
                                      )
    {
      if  (text)
      {
        this.ReportData (ch);
      }
      else
      {
        Manager ().CurrentDataRow ().Illumination = (byte)ch;
      }
    }
  }  /* InstrumentDataReportIllumination */
  
}