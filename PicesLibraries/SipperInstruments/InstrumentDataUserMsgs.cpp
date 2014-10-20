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


#include  "InstrumentDataUserMsgs.h"
#include  "InstrumentDataManager.h"
using namespace SipperHardware;

InstrumentDataUserMsgs::InstrumentDataUserMsgs (InstrumentDataManagerPtr _manager,
                                                const KKStr&            _reportDir,
                                                bool                     _text,
                                                int32                    _instrumentId
                                               ):

    InstrumentDataReport     (_manager, _reportDir, _text, _instrumentId, false),
    curTextLine              (),
    curTextLineStartScanLine (-1),
    lastTextLine             (),
    prevEolChar              (0)
{
}



InstrumentDataUserMsgs::~InstrumentDataUserMsgs ()
{
  report << curTextLine;
}






void  InstrumentDataUserMsgs::ReportInstrumentData (uint32 curScanLine,
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

    //report << endl;
    //report << curTextLineStartScanLine << "\t" << curTextLine << endl;
    lastTextLine = curTextLine;
    curTextLine = "";
    curTextLineStartScanLine = curScanLine;
  }

  report << (char)sensorData;
  report.flush ();

  prevEolChar = 0;
  if  (curTextLine.Empty ())
    curTextLineStartScanLine = curScanLine;

  curTextLine.Append ((char)sensorData);
}  /* ReportInstrumentData */
