#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
//#include  <ostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include  "InstrumentDataBatteryMeter.h"
#include  "InstrumentDataManager.h"

using  namespace SipperHardware;




InstrumentDataBatteryMeter::InstrumentDataBatteryMeter (InstrumentDataManagerPtr _manager,
                                                        const KKStr&            _reportDir,
                                                        bool                     _text,
                                                        int32                    _instrumentId
                                                       ):

    InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, false),
    curTextLine              (),
    curTextLineStartScanLine (-1),
    lastTextLine             (),
    numOfBatteries           (4),
    prevEolChar              (0)

{
  while  (batteryLevels.size () < numOfBatteries)
    batteryLevels.push_back (0.0f);
}




InstrumentDataBatteryMeter::~InstrumentDataBatteryMeter ()
{
  report << curTextLine << endl;
}




void  InstrumentDataBatteryMeter::ProcessBatteryData (const KKStr&  txt)
{
  // We will be expecting 5 fields;  
  // <Current battery> <\t> <Bat 0 Voltage>  <\t> .... <\t> <Bat-3 Voltage>
    
  VectorKKStr  fields = txt.Split (',');
  if  (fields.size () < (1 + numOfBatteries))
    return;
      
  // activeBattery is '1' based  that is batteries '1' - '4';  so 
  // batteryLevels[0] = battery level for battery 1.
  int32  activeBattery = fields[0].ToInt ();
  if  ((activeBattery < 1)  ||  ((uint32)activeBattery > numOfBatteries))
    return;

  uint32  x;
  for  (x = 0;  x < numOfBatteries;  x++)
    batteryLevels[x] = fields[x + 1].ToFloat ();

  manager->BatteryData (curTextLineStartScanLine, activeBattery, batteryLevels);
}  /* ProcessBatteryData */




void  InstrumentDataBatteryMeter::ReportInstrumentData (uint32 curScanLine,
                                                        uchar  sensorData
                                                       )
{
  if  ((sensorData == '\n')  ||  (sensorData == '\r'))
  {
    char  lastEolChar = prevEolChar;
    prevEolChar = sensorData;

    if  (lastEolChar == '\n')
    {
      if  (sensorData == '\r')
        return;
    }

    else if  (lastEolChar == '\r')
      if  (sensorData == '\n')
        return;

    if  (!curTextLine.Empty ())
      ProcessBatteryData (curTextLine);

    report << curTextLineStartScanLine << "\t" << curTextLine << endl;
    lastTextLine = curTextLine;
    curTextLine = "";
    curTextLineStartScanLine = curScanLine;
    return;
  }

  prevEolChar = 0;
  if  (curTextLine.Empty ())
    curTextLineStartScanLine = curScanLine;

  curTextLine.Append ((char)sensorData);
}  /* ReportInstrumentData */
