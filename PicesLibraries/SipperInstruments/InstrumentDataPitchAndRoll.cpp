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



#include  "InstrumentDataPitchAndRoll.h"
#include  "InstrumentDataManager.h"
using  namespace SipperHardware;



InstrumentDataPitchAndRoll::InstrumentDataPitchAndRoll (InstrumentDataManagerPtr _manager,
                                                        const KKStr&            _reportDir,
                                                        bool                     _text,
                                                        int32                    _instrumentId
                                                       ):

    InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, false),
    curTextLine              (),
    curTextLineStartScanLine (-1),
    lastTextLine             (),
    prevEolChar              (0)
{
}



InstrumentDataPitchAndRoll::~InstrumentDataPitchAndRoll ()
{
  report << curTextLine;
}




void  InstrumentDataPitchAndRoll::ProcessData (const KKStr&  txt)
{
  VectorKKStr  fields = txt.Split (" \t\n\r");
  if  (fields.size () < 4)
     return;

  float  pitch = -999.99f;
  float  roll  = -999.99f;

  KKStr  fieldName  = "";
  KKStr  fieldValue = "";
  uint32   fieldNum   = 0;

  while  (fieldNum < fields.size ())
  {
    fieldName  = fields[fieldNum];
    fieldName.Upper ();
    fieldNum++;

    if  (fieldNum < fields.size ())
    {
      fieldValue = fields[fieldNum];  
      fieldNum++;
    }
    else
    {
      fieldValue = "";
    }

    if  (fieldName == "R")
    {
      roll = fieldValue.ToFloat ();
    }
    else if  (fieldName == "P")
    {
      pitch = fieldValue.ToFloat ();
    }
  } 

  manager->PitchAndRollData (curTextLineStartScanLine, pitch, roll);
}  /* ProcessData */




void  InstrumentDataPitchAndRoll::ReportInstrumentData (uint32 curScanLine,
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
      ProcessData (curTextLine);

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
