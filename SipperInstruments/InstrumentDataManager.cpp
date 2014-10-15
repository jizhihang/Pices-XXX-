#include  "FirstIncludes.h"
#include  <stdio.h>
#include  <fstream>
#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;

#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "RunLog.h"
using namespace KKU;


#include  "CTD_Plus_Data.h"
#include  "InstrumentData.h"
#include  "InstrumentDataReport.h"
#include  "SipperFile.h"
#include  "SipperHeaderRec.h"
#include  "InstrumentDataManager.h"
using namespace SipperHardware;


InstrumentDataManager::InstrumentDataManager (const KKStr&   _sipperFileName,
                                              SipperFilePtr  _sipperFile,
                                              const KKStr&   _reportDir,
                                              RunLog&        _log
                                             ):
     activeBattery          (0),
     batteryLevels          (4, 0),   // For batteries 1 - 4;  we don't use 0.
     compReport             (NULL),
     compReportLastCTDSecs  (0),
     compReportLastScanLine (0),
     ctdData                (false),
     ctdDataRead            (false),
     dataReports            (NULL),
     gpsTime                (),
     headerRec              (NULL),
     latitude               (0.0),
     log                    (_log),
     longitude              (0.0),
     meter1FlowRate         (0.0f),
     meter2FlowRate         (0.0f),
     pitch                  (0.0f),
     recordRate             (0.0f),
     reportDir              (_reportDir),
     roll                   (0.0f),
     sipperFile             (_sipperFile),
     sipperFileName         (_sipperFileName),
     textReports            (NULL),

     depthLastReported      (0),
     depthMax               (1000),
     lastScanLine           (0),
     scanLinesPerMeterDepth (NULL)
{
  int32  x; 

  if  (!reportDir.Empty ())
    osCreateDirectoryPath (reportDir);

  dataReports = new InstrumentDataReportPtr[MaxNumOfInstruments];
  textReports = new InstrumentDataReportPtr[MaxNumOfInstruments];
  for  (x = 0;  x < MaxNumOfInstruments;  x++)
  {
    dataReports[x] = NULL;
    textReports[x] = NULL;
  }

  if  (!reportDir.Empty ())
  {
    KKStr compReportFileName = osAddSlash (reportDir) + osGetRootName (sipperFileName) + "_CompReport.txt";
    compReport = new ofstream (compReportFileName.Str ());

    // Place Initial Header Lines

    *compReport << "// Comprehensice Instrument Data"                << endl
                << "// Date"        << "\t" << osGetLocalDateTime () << endl
                << "// SipperFile"  << "\t" << sipperFileName  << "\t" << "TimeStamp"   << "\t" << osGetFileDateTime (sipperFileName) << endl
                << "//" << endl;

    *compReport << "Scan" << "\t" << "GPS"  << "\t"  <<  ""         << "\t" << ""          << "\t" << ""      << "\t" << ""     << "\t" << CTD_Plus_Data::Header1Str () << "\t" << "Flow"   << "\t" << "Flow"   << "\t" << "Active"  << "\t" << "Bat"  << "\t" << "Bat"  << "\t" << "Bat"  << "\t"  << "Bat";
    *compReport << std::endl;
    *compReport << "Line" << "\t" << "Time" << "\t"  << "Latitude"  << "\t" << "Longitude" << "\t" << "Pitch" << "\t" << "Roll" << "\t" << CTD_Plus_Data::Header2Str () << "\t" << "Meter1" << "\t" << "Meter2" << "\t" << "Battery" << "\t" << "1"    << "\t" << "2"    << "\t" << "3"    << "\t"  << "4";
    *compReport << std::endl;
  }

  scanLinesPerMeterDepth = new uint32[depthMax];
  for (x = 0;  x < depthMax;  x++)
    scanLinesPerMeterDepth[x] = 0;

  InitializeLatitude ();
}



InstrumentDataManager::InstrumentDataManager (const KKStr&   _sipperFileName,
                                              SipperFilePtr  _sipperFile,
                                              RunLog&        _log
                                             ):
     activeBattery          (0),
     batteryLevels          (4, 0),   // For batteries 1 - 4;  we don't use 0.
     compReport             (NULL),
     compReportLastCTDSecs  (0),
     compReportLastScanLine (0),
     ctdData                (false),
     ctdDataRead            (false),
     dataReports            (NULL),
     gpsTime                (),
     headerRec              (NULL),
     latitude               (0.0),
     log                    (_log),
     longitude              (0.0),
     meter1FlowRate         (0.0f),
     meter2FlowRate         (0.0f),
     pitch                  (0.0f),
     recordRate             (0.0f),
     reportDir              (""),
     roll                   (0.0f),
     sipperFile             (_sipperFile),
     sipperFileName         (_sipperFileName),
     textReports            (NULL),
     
     depthLastReported      (0),
     depthMax               (1000),
     lastScanLine           (0),
     scanLinesPerMeterDepth (NULL)

{
  int32  x; 

  dataReports = new InstrumentDataReportPtr[MaxNumOfInstruments];
  textReports = new InstrumentDataReportPtr[MaxNumOfInstruments];
  for  (x = 0;  x < MaxNumOfInstruments;  x++)
  {
    dataReports[x] = NULL;
    textReports[x] = NULL;
  }

  scanLinesPerMeterDepth = new uint32[depthMax];
  for (x = 0;  x < depthMax;  x++)
    scanLinesPerMeterDepth[x] = 0;

  InitializeLatitude ();
}



InstrumentDataManager::~InstrumentDataManager ()
{
  if  (dataReports)
  {
    for  (int32 x = 0;  x < MaxNumOfInstruments;  x++)
      if  (dataReports[x])
        delete  dataReports[x];
  }

  if  (textReports)
  {
    for  (int32 x = 0;  x < MaxNumOfInstruments;  x++)
      if  (textReports[x])
        delete  textReports[x];
  }

  if  (compReport)
  {
    compReport->close ();
    delete  compReport;
    compReport = NULL;
  }
 
  delete  headerRec;               headerRec   = NULL;
  delete  dataReports;             dataReports = NULL;
  delete  textReports;             textReports = NULL;
  delete  scanLinesPerMeterDepth;  scanLinesPerMeterDepth = NULL;
}



void  InstrumentDataManager::InitializeLatitude ()
{
  // because we do not always have access to trhe SipperFile, or the SIPPER file does
  // not have the latitude info in it we need to deduce the approximate lattitude from 
  // the sipper file name.
  if  (sipperFile)
  {
	const KKStr&  cruiseName = sipperFile->CruiseName ();
    if  (sipperFile->Latitude () > 0.1)
    {
      latitude = sipperFile->Latitude ();
    }

    //else if  ((sipperFile->CruiseName ().EqualIgnoreCase ("ETP2007"))  ||  (sipperFile->CruiseName ().EqualIgnoreCase ("ETP2008")))
    else if  ((cruiseName.EqualIgnoreCase ("ETP2007"))  ||  (cruiseName.EqualIgnoreCase ("ETP2008")))
    {
      // it is for a fact a panama cruise
      latitude = 11.0 + (59.7 / 60.0);
    }
    else
    {
      // We will assume it must be one of the GULF cruises.
      latitude = 29 + (30.0 / 60.0);
    }
  }
  else
  {
    if  ((sipperFileName.SubStrPart (0, 6) == "ETP2008")  ||
         (sipperFileName.SubStrPart (0, 4) == "ETP08")    ||
         (sipperFileName.SubStrPart (0, 6).EqualIgnoreCase ("Station"))
        )
    {
      // it is for a fact a panama cruise
      latitude = 11.0 + (59.7 / 60.0);
    }
    else
    {
      // We will assume it must be one of the GULF cruises.
      latitude = 29 + (30.0 / 60.0);
    }
  }

  return;       
}  /* InitializeLatitude */



InstrumentDataPtr  InstrumentDataManager::CreateInstrumentData ()  const
{
  InstrumentDataPtr  id = new   InstrumentData (lastScanLine,
                                                0,            // byteOffset
                                                ctdData.Date (),
                                                activeBattery,
                                                latitude,
                                                longitude,
                                                ctdData.BatteryVoltage      (),
                                                ctdData.Conductivity        (),
                                                ctdData.Density             (),
                                                ctdData.Depth               (),
                                                meter1FlowRate,
                                                meter2FlowRate,
                                                ctdData.Fluorescence           (),
                                                ctdData.FluorescenceSensor     (),
                                                ctdData.CdomFluorescence       (),
                                                ctdData.CdomFluorescenceSensor (),
                                                0.0f,                       // Illumination
                                                ctdData.Oxygen              (),
                                                ctdData.OxygenSensor        (),
                                                ctdData.Pressure            (),
                                                recordRate,
                                                ctdData.Salinity            (),
                                                ctdData.SoundVelocity       (),
                                                ctdData.Temperature         (),
                                                ctdData.Transmisivity       (),
                                                ctdData.TransmisivitySensor (),
                                                ctdData.Turbidity           (),
                                                ctdData.TurbiditySensor     (),
                                                pitch,
                                                roll,
                                                batteryLevels[0],
                                                batteryLevels[1],
                                                batteryLevels[2],
                                                batteryLevels[3]
                                               );

  return  id;
}  /* CreateInstrumentData */




void  InstrumentDataManager::AssignSerialPorts (const SipperHeaderRecPtr  _sipperHeaderRec)
{
  headerRec = NULL;

  headerRec = new SipperHeaderRec (*_sipperHeaderRec);
}   /* AssignSerialPorts */




VectorUlong*  InstrumentDataManager::ScanLinesPerMeterDepth ()  const
{
  VectorUlong*  _scanLinesPerMeterDepth = new VectorUlong (depthMax, 0);
  for  (int32 x = 0;  x < depthMax;  x++)
     (*_scanLinesPerMeterDepth)[x] = scanLinesPerMeterDepth[x];

  return  _scanLinesPerMeterDepth;
}  /* ScanLinesPerMeterDepth */





void  InstrumentDataManager::CompReportLine (int32  scanLine)
{
  uint32  x;

  int32  depthInt = (int32)ctdData.Depth ();
  if  (depthInt != depthLastReported)
  {
    uint32  deltaScanLines = scanLine - lastScanLine;
    int32  x = Min (depthLastReported, depthMax - 1);
    scanLinesPerMeterDepth[x] += deltaScanLines;
    depthLastReported = depthInt;
    lastScanLine = scanLine;
  }


  if  (compReport)
  {
    uint64  numCTDsecs = ctdData.Date ().Seconds ();

    if  (((numCTDsecs - compReportLastCTDSecs) > 1)  ||
         ((scanLine   - compReportLastScanLine) > 25000)
        )
    {
      compReportLastCTDSecs   = numCTDsecs;
      compReportLastScanLine  = scanLine;

      *compReport << scanLine                                     << "\t"
                  << gpsTime                                      << "\t"
                  << StrFormatDouble (latitude,  "##0.0000000")   << "\t"
                  << StrFormatDouble (longitude, "##0.0000000")   << "\t"
                  << StrFormatDouble (pitch,     "--0.000")       << "\t"
                  << StrFormatDouble (roll,      "--0.000")       << "\t"
                  << ctdData.ToStrNoLabels ()                     << "\t"
                  << StrFormatDouble (meter1FlowRate, "##0.00")   << "\t"
                  << StrFormatDouble (meter2FlowRate, "##0.000")  << "\t"
                  << StrFormatInt    (activeBattery,  "zz0");

      for  (x = 0;  x < batteryLevels.size ();  x++)
      {
        *compReport << "\t" << StrFormatDouble (batteryLevels[x], "zz0.000");
      }

      *compReport << endl;
    }
  }
}  /* CompReportLine */




void   InstrumentDataManager::CTDdataPlus  (int32                 _scanLine,
                                            const CTD_Plus_Data&  _ctdData
                                           )
{
  if  (_ctdData.ValidData ())
  {
    if  (!ctdDataRead)
    {
      ctdDataRead = true;
    }

    ctdData = _ctdData;

    CompReportLine (_scanLine);
  }
} /* CTDdataPlus */




void   InstrumentDataManager::BatteryData (int32               _scanLine,
                                           int32               _activeBattery, 
                                           const VectorFloat&  _batteryLevels
                                          )
{
  if  ((activeBattery  == _activeBattery)  &&  ( batteryLevels == _batteryLevels))
    return;

  activeBattery = _activeBattery;
  batteryLevels = _batteryLevels;
  CompReportLine (_scanLine);
}  /* BatteryData */



void   InstrumentDataManager::PitchAndRollData (int32  _scanLine,
                                                float  _pitch,
                                                float  _roll
                                               )
{
  if  ((pitch == _pitch)  &&  (roll == _roll))
    return;

  pitch = _pitch;
  roll  = _roll;
  CompReportLine (_scanLine);
}  /* PitchAndRollData */




void   InstrumentDataManager::Location (int32            _scanLine,
                                        double           _latitude,
                                        double           _longitude,
                                        const TimeType&  _gpsTime
                                       )
{
  if  ((_latitude != latitude)  ||  (_longitude != longitude)  ||  (_gpsTime != gpsTime))
  {
    latitude  = _latitude;
    longitude = _longitude;
    gpsTime   = _gpsTime;

    CompReportLine (_scanLine);
  }
}  /* Location */




void   InstrumentDataManager::Meter1FlowRate (int32  _scanLine, 
                                              float  _meter1FlowRate
                                             )
{
  if  (_meter1FlowRate != meter1FlowRate)
  {
    meter1FlowRate = _meter1FlowRate;
    CompReportLine (_scanLine);
  }
}  /* Meter1FlowRate */



void   InstrumentDataManager::Meter2FlowRate (int32  _scanLine, 
                                              float  _meter2FlowRate
                                             )
{
  if  (_meter2FlowRate != meter2FlowRate)
  {
    meter2FlowRate = _meter2FlowRate;
    CompReportLine (_scanLine);
  }
}  /* Meter2FlowRate */




void   InstrumentDataManager::FlowRates (int32 _scanLine,
                                         float _meter1FlowRate,
                                         float _meter2FlowRate
                                        )
{
  if  ((_meter1FlowRate != meter1FlowRate)  ||  (_meter2FlowRate != meter2FlowRate))
  {
    meter1FlowRate = _meter1FlowRate;
    meter2FlowRate = _meter2FlowRate;
    CompReportLine (_scanLine);
  }
}  /* FlowRates */




void  InstrumentDataManager::ReportInstrumentData (uint32 curScanLine,
                                                   uchar  instrumentId,
                                                   bool   text,
                                                   uchar  data
                                                  )
{
  if  (instrumentId >= MaxNumOfInstruments)
  {
    log.Level (-1) << endl
                   << "InstrumentDataManager::ReportInstrumentData   *** ERROR ***" << std::endl
                   << "                        instrumentId[" << (int32)instrumentId << "]  Exceeds maximum InstrumentId." << std::endl
                   << std::endl;
    //osWaitForEnter ();
    //exit (-1);
  }

  if  (text)
  {
    if  (!textReports[instrumentId])
      textReports[instrumentId] = InstrumentDataReport::CreateInstrumentDataReport (this, reportDir, text, (int32)instrumentId);
    textReports[instrumentId]->ReportInstrumentData (curScanLine, data);
  }
  else
  {
    if  (!dataReports[instrumentId])
      dataReports[instrumentId] = InstrumentDataReport::CreateInstrumentDataReport (this, reportDir, text, (int32)instrumentId);
    dataReports[instrumentId]->ReportInstrumentData (curScanLine, data);
  }

}  /* ReportInstrumentData */
