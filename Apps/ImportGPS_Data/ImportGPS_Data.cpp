#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>

#include  <map>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "SipperCruise.h"
#include  "SipperDeployment.h"
#include  "SipperFile.h"
using namespace  SipperHardware;

#include  "DataBase.h"
using namespace MLL;


#include "ImportGPS_Data.h"


using namespace  ImportGPS_DataNameSpace;

// -SrcDir  D:\Pices\Cruises\WB1101\GpsData

// E:\Users\kkramer\Dropbox\Sipper\GPS_Data
// -cruise  SD01       -FF SpecialtyDiverI   -src ".\2010-09"

// -cruise WB0813  -FF WeatherBird  -src "D:\Users\kkramer\Dropbox\Sipper\GPS_Data\2013-08_Weatherbird2\MetData"


class  SipperFileEntry
{
public:
  SipperFileEntry (SipperFilePtr    _sipperFile,
                   const DateTime&  _ctdStartTime,
                   const DateTime&  _ctdEndTime,
                   const int32&     _gpsAdjSecs
                  ):
      sipperFile    (_sipperFile),
      ctdStartTime  (_ctdStartTime),
      ctdEndTime    (_ctdEndTime),
      gpsAdjSecs    (_gpsAdjSecs)
    {
      gpsStartTime = ctdStartTime;
      gpsStartTime.AddSeconds (gpsAdjSecs);
      gpsEndTime   = ctdEndTime;
      gpsEndTime.AddSeconds (gpsAdjSecs);
    }

  SipperFilePtr  sipperFile;   // We will not own this entry.
  DateTime       ctdStartTime;
  DateTime       ctdEndTime;
  int32          gpsAdjSecs;    // Time that needs to be subtracted from 'GPS' time to get CTD time.
  DateTime       gpsStartTime;
  DateTime       gpsEndTime;
};
typedef  SipperFileEntry*  SipperFileEntryPtr;


class  ImportGPS_DataNameSpace::SipperFileEntryList:  public KKQueue<SipperFileEntry>
{
public:
  SipperFileEntryList (const SipperFileList&  sipperFiles,
                       DataBasePtr            dbConn,
                       RunLog&                log
                      ):  
      KKQueue<SipperFileEntry> (true)
  {
    SipperDeploymentPtr deployment = NULL;

    SipperFileList::const_iterator  idx;
    for  (idx = sipperFiles.begin ();  idx != sipperFiles.end ();  ++idx)
    {
      SipperFilePtr  sf = *idx;

      if  (sf->SipperFileName ().StartsWith ("SML751001005A_06"))
      {
        cout << "Put break point here." << endl;
      }

      if  ((deployment == NULL)  ||  
           (deployment->CruiseName    () != sf->CruiseName    ())  ||
           (deployment->StationName   () != sf->StationName   ())  ||
           (deployment->DeploymentNum () != sf->DeploymentNum ())
          )
      {
        delete  deployment;
        deployment = NULL;
        deployment = dbConn->SipperDeploymentLoad (sf->CruiseName (), sf->StationName (), sf->DeploymentNum ());
      }

      int32 gpsAdjSecs = 0;
      if  (deployment)
      {
        DateTime  validDateTimeThreshold (2001, 1, 1, 0, 0 ,0);
        if  ((deployment->SyncTimeStampGPS () > validDateTimeThreshold)  &&  (deployment->SyncTimeStampCTD () > validDateTimeThreshold))
          gpsAdjSecs = (int32)(deployment->SyncTimeStampGPS ().ToSeconds () - deployment->SyncTimeStampCTD ().Seconds ());
      }
      
      int32     sipperFileId = -1;
      uint32    numScanLines = 0;
      DateTime  ctdStartTime;
      DateTime  ctdEndTime;

      dbConn->SipperFilesGetCTDDateTime (sf->SipperFileName (), sipperFileId, numScanLines, ctdStartTime, ctdEndTime);
      if  (sipperFileId < 0)
        continue;

      log.Level (10) << sf->SipperFileName () << "\t" 
                     << ctdStartTime.YYYY_MM_DD_HH_MM_SS () << "\t" 
                     << ctdEndTime.YYYY_MM_DD_HH_MM_SS () << "\t" 
                     << gpsAdjSecs
                     << endl;
      PushOnBack (new SipperFileEntry (sf, ctdStartTime, ctdEndTime, gpsAdjSecs));
    }
    SortByStartTime ();
  }

  class  Comparer
  {
  public:
    bool  operator() (SipperFileEntryPtr  s1,
                      SipperFileEntryPtr  s2
                     )
    {
      return  s1->gpsStartTime < s2->gpsStartTime;
    }
  };

  void  SortByStartTime ()
  {
    Comparer  comparer;
    sort (begin (), end (), comparer);
  }


  SipperFilePtr  LocateByCtdDateStamp (const DateTime  dt)
  {
    SipperFileEntryPtr  lastOneBeforeDT = NULL;
    SipperFileEntryList::const_iterator idx;
    for  (idx = begin ();  idx != end ();  ++idx)
    {
      SipperFileEntryPtr  sfe = *idx;

      if  ((sfe->ctdStartTime <= dt)  &&  (sfe->ctdEndTime >= dt))
        return sfe->sipperFile;

      if  (sfe->ctdStartTime <= dt)
        lastOneBeforeDT = sfe;
    }
    return NULL;
  }



  SipperFileEntryPtr  LocateByGpsTimeStamp (const DateTime  dt)
  {
    SipperFileEntryPtr  firstOneAfterDT = NULL;
    SipperFileEntryList::const_iterator idx;
    for  (idx = begin ();  idx != end ();  ++idx)
    {
      SipperFileEntryPtr  sfe = *idx;
      if  ((sfe->gpsStartTime <= dt)  &&  (sfe->gpsEndTime >= dt))
        return sfe;

      if  (firstOneAfterDT == NULL)
      {
        if  (sfe->gpsStartTime > dt)
          firstOneAfterDT = sfe;
      }
    }

    if  (firstOneAfterDT != NULL)
    {
      DateTime  deltaTime = firstOneAfterDT->gpsStartTime - dt;
      if   (deltaTime.ToSeconds () > 600)
        firstOneAfterDT = NULL;
      else
      {
        cout << "Used 'lastOneBeforeDT'" << endl;
      }
    }

    return firstOneAfterDT;
  }
}; /* SipperFileEntryList */

typedef  ImportGPS_DataNameSpace::SipperFileEntryList*  SipperFileEntryListPtr;




ImportGPS_Data::ImportGPS_Data (int     argc, 
                                char**  argv
                               ):

   Application  (argc, argv),
   cruise            (NULL),
   cruiseStr         (),
   dbConn            (NULL),
   deltaHours        (0.0f),
   deployments       (NULL),
   fileFormat        (GDF_NULL),
   sipperFiles       (NULL),
   sipperFileEntries (NULL),
   srcDirectory      ()
{
  if  (argc < 2)
  {
    log.Level (-1) << endl << endl 
                   << "No Command Line Parameters were provided." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

  if  (srcDirectory.Empty ())
  {
    log.Level (-1) << endl << endl 
                   << "ImportGPS_Data    ***ERROR***   you must provide at least one '-src' parameter." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  if  (!osValidDirectory (srcDirectory))
  {
    log.Level (-1) << endl << endl 
                   << "ImportGPS_Data    ***ERROR***   The specified Source Directory[" << srcDirectory << "] is not valid." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  if  (cruiseStr.Empty ())
  {
    log.Level (-1) << endl << "ImportGPS_Data::ImportGPS_Data   ***ERROR***     Cruse must be stecified." << endl;
    Abort (true);
  }
}



ImportGPS_Data::~ImportGPS_Data ()
{
  delete  cruise;             cruise            = NULL;
  delete  dbConn;             dbConn            = NULL;
  delete  deployments;        deployments       = NULL;
  delete  sipperFileEntries;  sipperFileEntries = NULL;
  delete  sipperFiles;        sipperFiles       = NULL;
}



ImportGPS_Data::GpsDataFormat  ImportGPS_Data::GpsDataFormatFromStr (const KKStr& str)
{
  if  ((str.EqualIgnoreCase ("WB"))  ||  (str.EqualIgnoreCase ("WEATHERBIRD")))
    return  GDF_WeatherBird;

  else if  (str.EqualIgnoreCase ("GPGGA"))
    return  GDF_GPGGA;

  else if  (str.EqualIgnoreCase ("SpecialtyDiverI")  ||  str.EqualIgnoreCase ("Specialty")  ||  str.EqualIgnoreCase ("SD") )
    return  GDF_SpecialtyDiverI;

  else
    return  GDF_NULL;
}



KKStr  ImportGPS_Data::GpsDataFormatToStr (GpsDataFormat  format)
{
  if  (format == GDF_WeatherBird)
    return "WeatherBird";

  else if  (format == GDF_GPGGA)
    return "GPGGA";

  else if  (format == GDF_SpecialtyDiverI)
    return "SpecialtyDiverI";

  return "";
}


// -cruise  SD01       -FF SpecialtyDiverI -src E:\Users\kkramer\Dropbox\Sipper\GPS_Data\2010-09
// -cruise  SMP751001  -FF WeatherBird     -src E:\Users\kkramer\Dropbox\Sipper\GPS_Data\2010-05


// -cruise  GG1002  -FF GPGGA              -src "D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2010-06\SCS time stamp 2 GU10-02  MC252"
// -cruise  GG1002  -FF GPGGA              -src "D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2010-06\SCS time stamp 3 GU 10-02 MC252"

// -cruise  SMP751001  -FF WeatherBird        -src D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2010-05
// -cruise  GG1002     -FF GPGGA              -src "D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2010-06"
// -cruise  SD01       -FF SpecialtyDiverI    -src "D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2010-09"
// -cruise  WB0911     -FF WeatherBird        -src "D:\Users\kkramer\DropBox\Dropbox\Sipper\GPS_Data\2011-01\WB1101\GpsData"  -DH 


bool  ImportGPS_Data::ProcessCmdLineParameter (char    parmSwitchCode, 
                                               KKStr   parmSwitch, 
                                               KKStr   parmValue
                                              )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();


  if  (parmSwitch.EqualIgnoreCase ("-DH")         ||  
       parmSwitch.EqualIgnoreCase ("-Delta")      ||  
       parmSwitch.EqualIgnoreCase ("-DeltaHour")  ||
       parmSwitch.EqualIgnoreCase ("-DeltaHours")
      )
  {
    deltaHours = parmValue.ToFloat ();
  }

  else
  if  (parmSwitch.EqualIgnoreCase ("-FF")      ||
       parmSwitch.EqualIgnoreCase ("-FORMAT")
      )
  {
    fileFormat = GpsDataFormatFromStr (parmValue);
    if  (fileFormat == GDF_NULL)
    {
      log.Level (-1) << endl << "Invalid File Format[" << parmValue << "] Specified." << endl;
      Abort (true);
    }
  }

  else
  if  (parmSwitch.EqualIgnoreCase ("-CRUISE"))
  {
    cruiseStr = parmValue;
  }

  else 
  if  (parmSwitch.EqualIgnoreCase ("-S")  ||  
       parmSwitch.EqualIgnoreCase ("-SRC")     ||  
       parmSwitch.EqualIgnoreCase ("-SOURCE")  ||
       parmSwitch.EqualIgnoreCase ("-SRCDIR")
      )
  {
    srcDirectory = parmValue;
    if  (!osValidDirectory (srcDirectory))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Invalid '-SrcDir' [" << srcDirectory << "]." << endl;
      Abort (true);
    }
  }

  else
  {
    log.Level (-1) << endl << endl
                   << "ImportGPS_Data::ProcessCmdLineParameter    ***ERROR***" << endl
                   << endl
                   << "             Invalid Parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }

	return  !Abort ();
}  /* ProcessCmdLineParameter */




void   ImportGPS_Data::DisplayCommandLineParameters ()
{
  log.Level (0) << "ImportGPS_Data"                                                                   << endl;
  log.Level (0)                                                                                       << endl;
  log.Level (0) << "    -SrcDir <Source Directory>  The directorty where GPS data files are located." << endl;
  log.Level (0)                                                                                       << endl;
  log.Level (0)                                                                                       << endl;
  log.Level (0) << "Examples:"                                                                        << endl;
  log.Level (0) << "  ImportGPS_Data  -SrcDir C:\\Pices\\Cruises\\EB1101\\"                           << endl;
  log.Level (0)                                                                                       << endl;
  log.Level (0) << "           Will scan the specified directory for Weatherbird GPS data files."     << endl;
  log.Level (0) << "           Each GPS data file in directory will be scanned and appropriate"       << endl;
  log.Level (0) << "           tables in PICES database will be updated."                             << endl;
  log.Level (0)                                                                                       << endl;
}  /* DisplayCommandLineParameters */



DateTime  lastEndDate (0, 1, 1, 0, 0, 0);


void  ImportGPS_Data::UpdateInstrumentData (const DateTime&  gpsTimeStamp,
                                            double           lattitude,
                                            double           longitude
                                           )
{
  DateTime  startDT;
  DateTime  endDT;

  if  ((lattitude == 0.0)  ||  (longitude == 0.0))
  {
    log.Level (-1) << endl << "ImportGPS_Data::UpdateInstrumentData      ***ERROR***   Lattitude[" << lattitude << "]  Longitude[" << longitude << "] is invalid." << endl;
    return;
  }

  SipperFileEntryPtr  sfe = sipperFileEntries->LocateByGpsTimeStamp (gpsTimeStamp);

  if  (sfe == NULL)
    return;

  // KAK 2013-06-20
  // Since SIPPER would be behind the ship and the GPS unit actually on the ship;  the GPS location 
  // with the same timestamp as that of the CTD instrument  would represent where SIPPER WILL BE, not 
  // WHERE IT IS AT.   For this reason decided to bias towards the GPS entry that occurred before the 
  // CTD data was collected.
  startDT = gpsTimeStamp;
  startDT.AddSeconds (0 - sfe->gpsAdjSecs);
  endDT   = startDT;
  endDT.AddSeconds (120);

  {
    DateTime  delta = startDT - lastEndDate;
    int64  deltaSecs = delta.Seconds ();
    if  (deltaSecs <= 0)
    {
    }
    else if  (deltaSecs < 240)
    {
      startDT = lastEndDate;
      startDT.AddSeconds (1);
    }

    if  (deltaSecs > 240)
    {
      startDT.AddSeconds (-240);
    }
  }

  {
    int64  deltaTime = (endDT - startDT).ToSeconds ();
    if  (deltaTime > 500)
    {
      log.Level (-1) << endl
        << "StartDT[" << startDT.YYYY_MM_DD_HH_MM_SS () << "]  EndDT[" << endDT.YYYY_MM_DD_HH_MM_SS () << "]  Time Range [" << deltaTime << "] is too large." << endl;
    }
  }

  dbConn->InstrumentDataUpdateLatitudeAndLongitude (startDT, endDT, lattitude, longitude);
  lastEndDate = endDT;
}  /* UpdateInstrumentData */




/**
 *@brief  Imports GPS data from files created by Weatherbird system.
 *@details  Originally wriiten to upload GPS data for GULF 2010-may cruise
 */
void  ImportGPS_Data::ImportGPSDataWeatherBird (const KKStr&  fileName)
{
  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl 
       << "ImpotrtGPSData  Could not open file[" << fileName << "]" << endl
       << endl;
    return;
  }

  log.Level (10) << endl << endl << endl << endl << endl
    << "ImpotrtGPSData   FileName[" << fileName << "]" << endl << endl
    << endl;

  char  buff[20480];
  bool  firstPass = true;

  int  lastHour = 0;

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln = buff;

    if  (ln.StartsWith ("2010/05/07, 09:07:51, 23.0, 23.2, 1013.4,"))
    {
      cout << "We arived at Break Point" << endl;
    }

    ln.TrimLeft ();
    char  ch = ln[0];
    if  ((ch < '0')  ||  (ch > '9'))
      continue;


    KKStr  dateStr = ln.ExtractToken2 (",");
    KKStr  timeStr = ln.ExtractToken2 (",");

    KKStr  WXTP_TaStr = ln.ExtractToken2 (",");
    KKStr  WXTS_TaStr = ln.ExtractToken2 (",");

    KKStr  WXTS_PaStr = ln.ExtractToken2 (",");
    KKStr  WXTP_PaStr = ln.ExtractToken2 (",");
    KKStr  dec_latStr = ln.ExtractToken2 (",");
    KKStr  dec_lonStr = ln.ExtractToken2 (",");

    KKStr  cogStr = ln.ExtractToken2 (",");
    KKStr  sogStr = ln.ExtractToken2 (",");

    DateType  gmtDate (dateStr);
    TimeType  gmtTime (timeStr);

    DateTime  gmtDateTime (gmtDate, gmtTime);
    DateTime  localTime = gmtDateTime;
    localTime.SecondsAdd ((long)(deltaHours * 3600.0f));

    UpdateInstrumentData (gmtDateTime, dec_latStr.ToDouble (), dec_lonStr.ToDouble ());

    if  (gmtTime.Hour () != lastHour)
    {
      lastHour = gmtTime.Hour ();
      log.Level (10) << "File[" << osGetRootName (fileName) << "]  GMT Time[" << gmtDate.MMM_DD_YYYY () << " - " << gmtTime.HH_MM_SS () << "]" << endl;
    }
  }

  i.close ();
}  /* ImportGPSDataWeatherBird */





/**
 */
void  ImportGPS_Data::ImportGPSDataGPGGA (const KKStr&  fileName)
{
  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl 
       << "ImpotrtGPSData  Could not open file[" << fileName << "]" << endl
       << endl;
    return;
  }
  log.Level (10) << endl << endl << endl << endl << endl
    << "ImpotrtGPSData   FileName[" << fileName << "]" << endl << endl
    << endl;

  char  buff[20480];
  bool  firstPass = true;

  int  lastMinute = 0;
  int  linesRead = 0;

  KKStr  ln (256);

  while  (i.getline (buff, sizeof (buff)))
  {
    linesRead++;
    ln = buff;
    ln.TrimLeft ();
     
    if  (ln.LocateStr ("GPGGA") < 0)
      continue;

    VectorKKStr  fields = ln.Parse (",");
    if  (fields.size () < 8)
      continue;

    if  (!fields[2].EqualIgnoreCase ("$GPGGA"))
      continue;

    /*
    0           1             2        3         4      5       6       7   8  
06/01/2010, 23:59:59.818,  $GPGGA,  235958,  2840.927,  N,  08828.458,  W,  2,  09,22.10,0,M,,,14,0000*12
06/02/2010, 00:00:10.818,  $GPGGA,  000009,  2840.931,  N,  08828.482,  W,  1,  09,0.89,0,M,,,,*2D
06/02/2010, 00:00:21.802,  $GPGGA,  000020,  2840.929,  N,  08828.505,  W,  1,  09,0.89,0,M,,,,*21
06/02/2010, 00:00:31.818,  $GPGGA,  000030,  2840.924,  N,  08828.526,  W,  1,  09,0.89,0,M,,,,*2C
06/02/2010, 00:00:42.818,  $GPGGA,  000041,  2840.917,  N,  08828.547,  W,  1,  09,0.89,0,M,,,,*2D
06/02/2010, 00:00:53.802,  $GPGGA,  000052,  2840.906,  N,  08828.568,  W,  1,  09,1.00,0,M,,,,*22
06/02/2010, 00:01:03.802,  $GPGGA,  000102,  2840.895,  N,  08828.585,  W,  1,  09,0.89,0,M,,,,*2E
06/02/2010, 00:01:13.818,  $GPGGA,  000112,  2840.883,  N,  08828.600,  W,  1,  09,0.89,0,M,,,,*26
   */

    KKStr  dateStr = fields[0];
    KKStr  timeStr = fields[1];

    KKStr  latStr = fields[4];
    KKStr  logStr = fields[6];

    int  x = latStr.LocateCharacter ('.');
    if  (x < 0) 
      continue;

    KKStr latMinStr = latStr.SubStrPart (x - 2);
    KKStr latDegStr = latStr.SubStrPart (0, x - 3);

    double latitude = latDegStr.ToDouble () + latMinStr.ToDouble () / 60.0;
    if  (fields[5].EqualIgnoreCase ("S"))
      latitude = 0.0 - latitude;


    x = logStr.LocateCharacter ('.');
    if  (x < 0) 
      continue;

    KKStr logMinStr = logStr.SubStrPart (x - 2);
    KKStr logDegStr = logStr.SubStrPart (0, x - 3);

    double longitude = logDegStr.ToDouble () + logMinStr.ToDouble () / 60.0;
    if  (fields[7].EqualIgnoreCase ("W"))
      longitude = 0.0 - longitude;

    DateType  gmtDate (dateStr);
    TimeType  gmtTime (timeStr);
    DateTime  gmtDateTime (gmtDate, gmtTime);

    UpdateInstrumentData (gmtDateTime, latitude, longitude);

    if  (gmtTime.Minute () != lastMinute)
    {
      lastMinute = gmtTime.Minute ();
      log.Level (10) << "LinesRead[" << linesRead << "]  File[" << osGetRootName (fileName) << "]  GMT Time[" << gmtDate.MMM_DD_YYYY () << " - " << gmtTime.HH_MM_SS () << "]" << endl;
    }

  }

  i.close ();
}  /* ImportGPSDataGPGGA */






void  ImportGPS_Data::ImportGPSDataSpecialtyDiverI (const KKStr&  fileName)
{
  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl 
       << "ImportGPSDataSpecialtyDiverI  Could not open file[" << fileName << "]" << endl
       << endl;
    return;
  }

  log.Level (10) << endl << endl << endl << endl << endl
    << "ImportGPSDataSpecialtyDiverI   FileName[" << fileName << "]" << endl << endl
    << endl;

  char  buff[20480];
  bool  firstPass = true;

  i.getline (buff, sizeof (buff));
  KKStr  firstLine (buff);
  while  (firstLine.Len () < 10)
  {
    i.getline (buff, sizeof (buff));
    firstLine = buff;
  }

  //12:17:20    09-10-2010     S1-2.lin
  //
  //Specialty Diver I
  // 
  //    LAT          LONG        TIME
  //
  //
  //N028 00.9239  W 088 41.2726 12:18:43
  //N028 00.9620  W 088 41.2558 12:19:23
  //N028 00.9981  W 088 41.2350 12:20:03
  //N028 01.0288  W 088 41.2131 12:20:43
  KKStr timeStr      = firstLine.ExtractToken2 (" ");
  KKStr dateStr      = firstLine.ExtractToken2 (" ");
  KKStr dateStrMonth = dateStr.ExtractToken2 ("-");
  KKStr dateStrDay   = dateStr.ExtractToken2 ("-");
  KKStr dateStrYear  = dateStr.ExtractToken2 ("-");

  KKStr lastFieldStr = firstLine.ExtractToken2 (" ");

  DateType  fileDate (dateStrYear.ToInt16 (), (uchar)dateStrMonth.ToInt16 (), (uchar)dateStrDay.ToInt16 ());
  TimeType  fileTime (timeStr);

  int  lastMinute = 0;

  TimeType  lastTime (0, 0, 0);

  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln = buff;
    ln.TrimLeft ();

    if  (ln.Len () < 20)
      continue;

    if  (ln.SubStrPart (0, 8) == "Specialty")
      continue;

    char  latNorthSouth = ln[0];
    char  ch = ln[0];
    if  ((latNorthSouth != 'N')  &&  (latNorthSouth != 'S'))
      continue;

    //0         1         2         3
    //012345678901234567890123456789012345
    //N028 00.8789  W 088 41.2866 12:18:03
    int    latDeg  = ln.SubStrPart (1,3).ToInt ();
    double latMins = ln.SubStrPart (4, 12).ToDouble ();
    double  latitude = (double)latDeg + latMins / 60.0;
    if  (latNorthSouth == 'S')
      latitude = 0.0 - latitude;

    char   longEastWest = ln[14];
    int    longDeg      = ln.SubStrPart (16, 18).ToInt ();
    double longMins     = ln.SubStrPart (19, 26).ToDouble ();
    double longitude    = (double)longDeg + longMins / 60.0;
    if  (longEastWest == 'W')
      longitude = 0.0 - longitude;

    TimeType  lineTime (ln.SubStrPart (28, 35));

    if  (lineTime < lastTime)
      fileDate.AddDays (1);

    DateTime  gpsDateTime (fileDate, lineTime);

    UpdateInstrumentData (gpsDateTime, latitude, longitude);

    if  (gpsDateTime.Time ().Minute () != lastMinute)
    {
      lastMinute = gpsDateTime.Time ().Minute ();
      log.Level (10) << "File[" << osGetRootName (fileName) << "]  GMT Time[" << gpsDateTime.HH_MM_SS () << " - " << gpsDateTime.HH_MM_SS () << "]" << endl;
    }
    
    lastTime = lineTime;
  }

  i.close ();
}  /* ImportGPSDataSpecialtyDiverI */






void   ImportGPS_Data::Main ()
{
  if  (Abort ())
    return;

  dbConn = new DataBase (log);
  if  (!dbConn->Valid ())
  {
    log.Level (-1) << endl << endl 
      << "ImportGPS_Data::Main    ***ERROR***    Failed to connect to database."  << endl
      << endl;
    Abort (true);
    return;
  }

  cruise = dbConn->SipperCruiseLoad (cruiseStr);
  if  (!cruise)
  {
    log.Level (-1) << endl << endl 
      << "ImportGPS_Data::Main    ***ERROR***    Cruise[" << cruiseStr << "] not in database."  << endl
      << endl;
    Abort (true);
    return;
  }

  sipperFiles = dbConn->SipperFileLoad (cruiseStr, "", "");
  if  (!sipperFiles)
  {
    log.Level (-1) << endl << endl 
      << "ImportGPS_Data::Main    ***ERROR***    No SipperFile table entries found."  << endl
      << endl;
    Abort (true);
    return;
  }

  sipperFileEntries = new SipperFileEntryList (*sipperFiles, dbConn, log);

  deployments = dbConn->SipperDeploymentLoad (cruiseStr, "");
  if  ((!deployments)  ||  (deployments->QueueSize () < 1))
  {
    log.Level (-1) << endl << endl 
      << "ImportGPS_Data::Main    ***ERROR***    No Deployments found."  << endl
      << endl;
    Abort (true);
    return;
  }

  KKStrListPtr  filesToExtract = NULL;

  if  (fileFormat == GDF_WeatherBird)
    filesToExtract = osGetListOfFiles (osAddSlash (srcDirectory) + "*.csv");

  if  (filesToExtract == NULL)
  {
    filesToExtract = osGetListOfFiles (osAddSlash (srcDirectory) + "*.csv");
    if  (filesToExtract == NULL)
    {
      filesToExtract = osGetListOfFiles (osAddSlash (srcDirectory) + "*.Raw");
      if  (filesToExtract == NULL)
      {
        filesToExtract = osGetListOfFiles (osAddSlash (srcDirectory) + "*.Txt");
      }
    }
  }

  if  ((!filesToExtract)  ||  (filesToExtract->QueueSize () < 1))
  {
    log.Level (-1) << endl << endl 
      << "ImportGPS_Data::Main    ***ERROR***    No files found to extract."  << endl
      << endl;
    delete  filesToExtract;
    filesToExtract = NULL;
    Abort (true);
    return;
  }

  KKStrList::iterator  idx;
  for  (idx = filesToExtract->begin ();  idx != filesToExtract->end ();  idx++)
  {
    const KKStr&  gpsImportFileName = *idx;
    KKStr  fullFileName = osAddSlash (srcDirectory) + gpsImportFileName;
    switch  (fileFormat)
    {
    case  GDF_WeatherBird:     ImportGPSDataWeatherBird (fullFileName);
                               break;

    case  GDF_GPGGA:           ImportGPSDataGPGGA (fullFileName);
                               break;

    case  GDF_SpecialtyDiverI: ImportGPSDataSpecialtyDiverI (fullFileName);
                               break;
    }
  }

  delete  filesToExtract;
  filesToExtract = NULL;
}  /* Main */





int  main (int     argc,
           char**  argv
          )
{
  ImportGPS_Data  importGpsApp (argc, argv);
  if  (importGpsApp.Abort ())
    return 1;

  importGpsApp.Main ();
  if  (importGpsApp.Abort ())
    return 1;
  else
    return 0;
}
