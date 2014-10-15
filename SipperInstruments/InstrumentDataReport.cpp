#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;


#include  "InstrumentDataBatteryMeter.h"
#include  "InstrumentDataReport.h"
#include  "InstrumentDataFlowMeter1.h"
#include  "InstrumentDataPitchAndRoll.h"
#include  "InstrumentDataGPS.h"
#include  "InstrumentDataUserMsgs.h"
#include  "InstrumentDataCTDplus.h"

#include  "Instrument.h"
#include  "InstrumentDataManager.h"
#include  "SipperHeaderRec.h"
using namespace SipperHardware;


const char*  InstrumentDataReport::InstrumentNames[] = 
{
  "Synchronization_pulse",  //  0
  "Real-Time_Clock",        //  1
  "FIFO_Buffer_Status",     //  2
  "FireWire_Error",         //  3
  "Internal_Temperature",   //  4
  "Internal_Humidity",      //  5
  "User_Text_Message",      //  6
  "Other_Internal_Status",  //  7
  "CTD",                    //  8
  "GPS",                    //  9
  "Flow_meter_1",           // 10
  "Flow_meter_2",           // 11
  "OPC",                    // 12
  "Fluorometer",            // 13
  "Transmissometer",        // 14
  "Camera_messages",        // 15
  "Serisl Port 0",          // 16
  "Serisl Port 1",          // 17
  "Serisl Port 2",          // 18
  "Serisl Port 3",          // 19 
  NULL
};




InstrumentDataReport::InstrumentDataReport (InstrumentDataManagerPtr _manager,
                                            const KKStr&             _reportDir,
                                            bool                     _text,
                                            int32                    _instrumentId,
                                            bool                     _useReportWithLineNumbers
                                           ):

  instrumentId             (_instrumentId),
  lastScanLineReported     (9999999),
  manager                  (_manager),
  text                     (_text),
  useReportWithLineNumbers (_useReportWithLineNumbers)

{
  if  (!_reportDir.Empty ())
  {
    reportFileName    = osAddSlash (_reportDir) + InstrumentName () + "_" + (text ? "ASCII" : "DATA") + ".txt";
    report.open (reportFileName.Str ());

    if  (useReportWithLineNumbers)
    {
      reportFileNameWLN = osAddSlash (_reportDir) + InstrumentName () + "_" + (text ? "ASCII" : "DATA") + "_wln.txt";
      reportWLN.open (reportFileNameWLN.Str ());
    }
  }
}




InstrumentDataReport::~InstrumentDataReport ()
{
  if  (report.is_open ())
  {
    report.close ();
    reportWLN.close ();
  }
}






InstrumentDataReportPtr  
    InstrumentDataReport::CreateInstrumentDataReport (InstrumentDataManagerPtr _manager,
                                                      const KKStr&             _reportDir,
                                                      bool                     _text,
                                                      int32                    _instrumentId
                                                     )
{
  InstrumentDataReportPtr  r = NULL;

  if  (_instrumentId >= 16)
  {
    SipperHeaderRecPtr  hr = _manager->HeaderRec ();

    if  (hr)
    {
      uint32  serialPort = _instrumentId - 1;
 
      InstrumentPtr  instrument = hr->SerialPortAssignment (serialPort);

      if  (instrument)
      {
         r = instrument->CreateInstrumentDataReporter (_manager, _reportDir, _instrumentId);
      }
    }
  }

  if  (!r)
  {
    if  (_instrumentId == 6)
    {
      r = new  InstrumentDataUserMsgs (_manager, _reportDir, _text, _instrumentId);
    }

    else if  (_instrumentId == 9)
    {
      r = new  InstrumentDataGPS (_manager, _reportDir, _text, _instrumentId);
    }

    else if  (_instrumentId == 10)
    {
      r = new  InstrumentDataFlowMeter1 (_manager, _reportDir, _text, _instrumentId);
    }

    else if  (_instrumentId == 16)
    {
      //r = new InstrumentDataSerialPort0 (_manager, _reportDir, _text, _instrumentId);
      r = new InstrumentDataCTDplus (_manager,_reportDir, _text, _instrumentId);
    }


    else if  (_instrumentId == 17)
    {
      //r = new InstrumentDataSerialPort0 (_manager, _reportDir, _text, _instrumentId);
      r = new InstrumentDataPitchAndRoll (_manager,_reportDir, _text, _instrumentId);
    }


    else if  (_instrumentId == 18)
    {
      //r = new InstrumentDataSerialPort0 (_manager, _reportDir, _text, _instrumentId);
      r = new InstrumentDataBatteryMeter (_manager,_reportDir, _text, _instrumentId);
    }

    else
    {
      r = new InstrumentDataReport (_manager, _reportDir, _text, _instrumentId, 
                                    true           // true = Generate report with line numbers.
                                   );
    }
  }

  return r;
}  /* CreateInstrumentDataReport */




KKStr  InstrumentDataReport::InstrumentName ()
{
  int32  x = 0;
  while  ((x != instrumentId)  &&  (InstrumentNames[x]))
    x++;

  if  (InstrumentNames[x])
    return  InstrumentNames[x];

  return  "Sensor_" + StrFormatInt (instrumentId, "00");
}  /* InstrumentName */





void  InstrumentDataReport::ReportInstrumentData (uint32 curScanLine,
                                                  uchar  sensorData
                                                 )
{
  if  (useReportWithLineNumbers)
  {
    if  (lastScanLineReported != curScanLine)
    {
      lastScanLineReported = curScanLine;
      if  (reportWLN.is_open ())
      {
        if  (curScanLine >= 0)
          reportWLN << endl;
        reportWLN << curScanLine << "\t";
      }
    }

    if  (curScanLine >= 0)
      reportWLN << (int32)sensorData << "\t" << (char)sensorData;
  }

  if  (sensorData != 13)
  {
    if  (report.is_open ())
      report << (char)sensorData;

    if  (text)
    {
      if  (sensorData == '\n')
      {
        lastTextLine = curTextLine;
        curTextLine  = "";
  
        if  (report.is_open ())
          report.flush ();
      }
      else
      {
        if  (curTextLine.Len () > 5000)
        {
          // Text Lines Should not get this int32.  What ever is providing the data is not providing a End of Line Character ('\n')  
          lastTextLine = curTextLine;
          curTextLine  = "";
          if  (report.is_open ())
            report.flush ();
        }

        curTextLine.Append ((char)sensorData);
      }
    }
  }
}  /* ReportInstrumentData */
