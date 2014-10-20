#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"
using namespace std;


#include  "BasicTypes.h"
#include  "Str.h"
using namespace KKU;


#include  "InstrumentDataFlowMeter1.h"
#include  "InstrumentDataManager.h"
using namespace SipperHardware;




InstrumentDataFlowMeter1::InstrumentDataFlowMeter1 (InstrumentDataManagerPtr _manager,
                                                    const KKStr&            _reportDir,
                                                    bool                     _text,
                                                    int32                    _instrumentId
                                                  ):

    InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, false),
    flowMeterCurSlot       (-1),
    flowMeterTurnScanLines (NULL)

{
  if  (text)
  {
    int32 x;
    flowMeterTurnScanLines = new uint32[NumTurnSignalsToTrack];
    for (x = 0;  x < NumTurnSignalsToTrack;  x++)
      flowMeterTurnScanLines[x] = 0;

    flowMeterTrailingSlot = flowMeterCurSlot - NumTurnSignalsToTrack + 1;
    if  (flowMeterTrailingSlot < 0)
      flowMeterTrailingSlot += NumTurnSignalsToTrack;
  }
}



InstrumentDataFlowMeter1::~InstrumentDataFlowMeter1 ()
{
  if  (flowMeterTurnScanLines)
    delete  flowMeterTurnScanLines;  flowMeterTurnScanLines = NULL;
}



void  InstrumentDataFlowMeter1::ReportInstrumentData (uint32 curScanLine,
                                                      uchar  sensorData
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

    double  scanLines = double (curScanLine - flowMeterTurnScanLines[flowMeterTrailingSlot]);
    double  numOfSecs = scanLines / double (ScanLinesPerSecond);
    double  meters    = double (NumTurnSignalsToTrack) / 98.0;
    double flowRate = meters / numOfSecs;
    manager->Meter2FlowRate (curScanLine, float (flowRate));
    report << curScanLine << "\t" << char (sensorData) << "\t" << flowRate << endl;
  }
  else
  {
    float flowRate = (float)sensorData / 100.0f;
    manager->Meter1FlowRate (curScanLine, flowRate);
    report << curScanLine << "\t" << flowRate << endl;
  }
}  /* ReportInstrumentData */
