using System;
using System.Collections.Generic;
using System.Text;



namespace SipperFile
{
  class  InstrumentDataReportFlowMeter: InstrumentDataReport
  {
    public  const int  NumTurnSignalsToTrack = 5;  // This needs to be a odd number,  so that we track from a
                                                   // "rise to rise" and "fall to fall"
    public  const int  ScanLinesPerSecond    = 25000;


    int    flowMeterCurSlot       = 0;     // Indicates the slot last used.
    int    flowMeterTrailingSlot  = 0;     // Indicates the slot that is behind current slot.
    int[]  flowMeterTurnScanLines = null;  // A circular buffer that will keep track of scan lines.





    public InstrumentDataReportFlowMeter (InstrumentDataManager _manager,
                                          byte                  _deviceId
                                         )
      :
        base (_manager, _deviceId)
    {
      int x;

      flowMeterTurnScanLines = new int[NumTurnSignalsToTrack];
      for  (x = 0;  x < NumTurnSignalsToTrack;  x++)
        flowMeterTurnScanLines[x] = 0;

      flowMeterTrailingSlot = flowMeterCurSlot - NumTurnSignalsToTrack + 1;
      if  (flowMeterTrailingSlot < 0)
        flowMeterTrailingSlot += NumTurnSignalsToTrack;
    }



  
    override  public void  ReportData (bool  text,
                                       char  ch,
                                       int   curScanLine
                                      )
    {
      if  (text)
      {
        flowMeterCurSlot++;
        if  (flowMeterCurSlot >= NumTurnSignalsToTrack)
          flowMeterCurSlot = 0;

        flowMeterTrailingSlot++;
        if  (flowMeterTrailingSlot >= NumTurnSignalsToTrack)
          flowMeterTrailingSlot = 0;
        flowMeterTurnScanLines[flowMeterCurSlot] = curScanLine;

        double  scanLines = (double) (curScanLine - flowMeterTurnScanLines[flowMeterTrailingSlot]);
        double  numOfSecs = scanLines / (double) ScanLinesPerSecond;
        double  meters    = (double) (NumTurnSignalsToTrack) / 98.0;
        double  flowRate  = meters / numOfSecs;
        Manager ().CurrentDataRow ().FlowRate1 = (float)flowRate;
      }
      else
      {
        float  flowRate = (float)((byte)ch) / 100.0f;
        Manager ().CurrentDataRow ().FlowRate2 = flowRate;
      }
    }
  }  /* InstrumentDataReportFlowMeter */
  
}