#include "FirstIncludes.h"

// Kurt Kramer  2012-03-01  
// Added a Second Best Guess Confusion Matrix.  That is a confusion matrix that only reports the
// results using the second best class as the predicted class when the first one wss wrong.
 
#include <string>
#include <ctype.h>
#include <math.h>
#include <stdio.h> 
#include <time.h>

#ifndef  WIN32
#include  <unistd.h>
#endif

#include <iomanip>
#include <iostream>
#include <fstream>
//#include  <ostream>
#include <iostream>
#include <map>
#include <vector>

#include "MemoryDebug.h"


#ifdef WIN32
#include <windows.h>
#else
#endif

using namespace  std;


#include "BasicTypes.h"
#include "CmdLineExpander.h"
using namespace  KKU;

#include "Instrument.h"
#include "InstrumentDataFileManager.h"
#include "InstrumentDataManager.h"
#include "SipperBuff.h"
using namespace SipperHardware;


#include "DataBase.h"
#include "SipperCruise.h"
#include "SipperStation.h"
#include "SipperDeployment.h"
using namespace  MLL;



#include "SvnVersion.h"
#include "MarineSnowReport.h"


#define Q(x) #x
#define QUOTE(x) Q(x)



class  DeploymentSummary
{
public:
  DeploymentSummary (const KKStr&  _cruiseName,
                     const KKStr&  _stationName,
                     const KKStr&  _deploymentNum,
                     double        _volumeSampled,
                     VectorInt32   _sizeThresholds,
                     VectorDouble  _integratedAbundance,
                     VectorInt32   _integratedCounts
                    ):
      cruiseName          (_cruiseName),
      stationName         (_stationName),
      deploymentNum       (_deploymentNum),
      volumeSampled       (_volumeSampled),
      sizeThresholds      (_sizeThresholds),
      integratedAbundance (_integratedAbundance),
      integratedCounts    (_integratedCounts)
  {
  }

  KKStr  cruiseName;
  KKStr  stationName;
  KKStr  deploymentNum;

  double volumeSampled;

  VectorInt32   sizeThresholds;
  VectorDouble  integratedAbundance;
  VectorInt32   integratedCounts;
};



DeploymentSummary*  MarineSnowReportDeployment (SipperDeploymentPtr  deployment,
                                                DataBase&            db
                                               )
{
  cout << deployment->CruiseName () << "\t" << deployment->StationName () << "\t" << deployment->DeploymentNum () << endl;


  // 1)Determines Midpoint of deploymet from Instruments table.
  KKStr  sqlStr = "Call ImagesSizeDataByDepthSipper9(" + deployment->CruiseName ().QuotedStr () + "," + deployment->StationName ().QuotedStr () + "," + deployment->DeploymentNum ().QuotedStr () +",1.0)";

  VectorKKStr  columnNames;
  KKStrMatrixPtr results = db.QueryStatementReturnAllColumns (sqlStr.Str (), sqlStr.Len (), columnNames);
  if  (results == NULL)
  {
    cout << "No results returend" << endl;
    return NULL;
  }

  int32  numRows = results->NumRows ();
  int32  numCols = results->NumCols ();
  int32  numCountCols = numCols - 8;

  if  (numRows == 0)
  {
    delete  results;
    results = NULL;
    return  NULL;
  }

  VectorInt32  sizeThresholds;

  VectorKKStr  countHeader (numCountCols);
  for  (int32 x = 0;  x < numCountCols;  ++x)
  {
    KKStr  columnName = columnNames[8 + x];
    countHeader[x] = columnName;

    int32  sizeThreshold = 0;
    int32 idx = columnName.LocateCharacter ('_');
    if  (idx > 0)
      sizeThreshold = columnName.SubStrPart (idx + 1).ToInt32 ();
    sizeThresholds.push_back (sizeThreshold);
  }

  bool  cancelFlag = false;
  InstrumentDataMeansListPtr  instData = db.InstrumentDataBinByMeterDepth (deployment->CruiseName (), deployment->StationName (), deployment->DeploymentNum (), 1.0f, cancelFlag);
  if  (!instData)
  {
    cout << "no Instrument data." << endl << endl;
    return NULL;
  }

  KKStr svnVersionStr = QUOTE (_SVNVERSION_);

  KKStr  reportRootName = deployment->CruiseName () + "-" + deployment->StationName () + "-" + deployment->DeploymentNum ();
  KKStr  reportFileRootName1 = "C:\\Temp\\MarineSnow\\" + reportRootName + ".txt";

  ofstream  r1 (reportFileRootName1.Str ());

  r1 << "Cruise"     << "\t" << deployment->CruiseName    () << endl
     << "Station"    << "\t" << deployment->StationName   () << endl
     << "Deployment" << "\t" << deployment->DeploymentNum () << endl
     << "DateTime"   << "\t" << osGetLocalDateTime ()        << endl
     << "DataBase"   << "\t" << db.ServerDescription ()      << endl
     << "ProgName"   << "\t" << osGetProgName ()             << endl
     << "BuildTime"  << "\t" << __DATE__ << " " << __TIME__  << endl
     << "SvnVersion" << "\t" << svnVersionStr                << endl
     << "HostName"   << "\t" << osGetHostName ()             << endl
     << "UserName"   << "\t" << osGetUserName ()             << endl
     << endl
     << endl;

  KKStr  h1 (1024);
  KKStr  h2 (1024);

  h1 << ""        << "\t" << "Depth"  << "\t" << "VolumeSampled" << "\t" << "Image"     << "\t"  << "Pixel";
  h2 << "Down/Up" << "\t" << "(m)"    << "\t" << "m^3"           << "\t" << "Abundance" << "\t"  << "Count";

  for  (int32 c = 0;  c < numCountCols;  ++c)
  {
    h1 << "\t" << "";
    h2 << "\t" << countHeader[c];
  }

  VectorDouble  integratedAbundance (sizeThresholds.size (), 0.0);
  VectorInt32   integratedCounts    (sizeThresholds.size (), 0);
  double        totalVolumeSampled = 0.0;

  KKStr  temperatureUOM   = InstrumentData::TemperatureUnit   ();
  KKStr  salinityUOM      = InstrumentData::SalinityUnit      ();
  KKStr  denisityUOM      = InstrumentData::DensityUnit       ();
  KKStr  fluorescenceUOM  = InstrumentData::FluorescenceUnit  ();
  KKStr  oxygenUOM        = InstrumentData::OxygenUnit        ();
  KKStr  transmisivityUOM = InstrumentData::TransmisivityUnit ();
  KKStr  turbidityUOM     = InstrumentData::TurbidityUnit     ();

  h1 << "\t" << "Temperature"   << "\t" << "Salinity"  << "\t" << "Denisity"  << "\t" << "Fluorescence"  << "\t" << "Fluorescence-Sensor" << "\t" << "Oxygen"  << "\t" << "Oxygen"  << "\t" << "transmisivity"  << "\t" << "turbidity";
  h2 << "\t" << temperatureUOM  << "\t" << salinityUOM << "\t" << denisityUOM << "\t" << fluorescenceUOM << "\t" << "Volts"               << "\t" << oxygenUOM << "\t" << "umol/kg" << "\t" << transmisivityUOM << "\t" << turbidityUOM;

  r1 << endl << endl
     << "Abundance (Count/m^3)" << endl
     << endl
     << h1 << endl
     << h2 << endl;

  for  (int32 rowIdx = 0;  rowIdx < numRows;  ++rowIdx)
  {
    KKStrList&  row = (*results)[rowIdx];

    bool  downCast     = row[3].ToBool  ();
    int32  bucketIdx   = row[4].ToInt32 ();
    float  bucketDepth = row[5].ToFloat ();
    int32  imageCount  = row[6].ToInt32 ();
    int32  pixelCount  = row[7].ToInt32 ();

    VectorInt32  counts (numCountCols, 0);
    for  (int32 x = 0;  x < numCountCols;  ++x)
      counts[x] = row[8 + x].ToInt32 ();

    float  volumeSampled = 0.0f;
    InstrumentDataMeansPtr  idm = instData->LookUp (downCast, bucketDepth);
    if  (idm)
      volumeSampled = idm->volumeSampled;

    totalVolumeSampled += volumeSampled;

    KKStr  begOfLine (128);
    begOfLine << (downCast ? "Down" : "Up") << "\t" << bucketDepth << "\t" << volumeSampled << "\t" << imageCount << "\t" << pixelCount;

    r1 << begOfLine;

    for  (int32 x = 0;  x < numCountCols;  ++x)
    {
      float  density = 0.0f;
      if  (volumeSampled != 0.0)
        density = (float)counts[x] / volumeSampled;
      r1 << "\t" << density;
      integratedAbundance[x] += density;
      integratedCounts   [x] += counts[x];
    }

    if  (idm)
    {
      KKStr  instDataStr (256);

      instDataStr << "\t" << idm->temperatureMean
                  << "\t" << idm->salinityMean 
                  << "\t" << idm->denisityMean
                  << "\t" << idm->fluorescenceMean
                  << "\t" << idm->fluorescenceSensorMean
                  << "\t" << idm->oxygenMean
                  << "\t" << idm->Oxygen_molPerKg_Mean ()
                  << "\t" << idm->transmisivityMean 
                  << "\t" << idm->turbidityMean;

      r1 << instDataStr;
    }

    r1 << endl;
  }

  r1 << endl;

  r1 << "" << "\t" << "" << "\t" << "" << "\t" << "Integrated Abundance" << "\t" << "";
  for  (uint32 x = 0;  x < integratedAbundance.size ();  ++x)
  {
    r1 << "\t" << integratedAbundance[x];
  }
  r1 << endl;

  r1 << "" << "\t" << "" << "\t" << "" << "\t" << "Integrated Abundance-Size" << "\t" << "";
  for  (uint32 x = 0;  x < integratedAbundance.size ();  ++x)
  {
    r1 << "\t" << (integratedAbundance[x] * sizeThresholds[x]);
  }
  r1 << endl;

  r1 << "" << "\t" << "" << "\t" << "" << "\t" << "Integrated log10(Abundance)" << "\t" << "";
  for  (uint32 x = 0;  x < integratedAbundance.size ();  ++x)
  {
    double  zed = integratedAbundance[x];
    double  zed2 = 0.0;
    if  (zed != 0.0)
       zed2 = log10 (zed);
    r1 << "\t" << zed2;
  }
  r1 << endl;

  r1 << "" << "\t" << "" << "\t" << "" << "\t" << "Integrated log10(Abundance-Size)" << "\t" << "";
  for  (uint32 x = 0;  x < integratedAbundance.size ();  ++x)
  {
    double  zed = integratedAbundance[x] * (double)(sizeThresholds[x]);
    double  zed2 = 0.0;
    if  (zed != 0.0)
       zed2 = log10 (zed);
    r1 << "\t" << zed2;
  }
  r1 << endl;


  r1 << endl << endl << endl
     << "Count" << endl
     << endl;

  r1 << h1 << endl;
  r1 << h2 << endl;

  for  (int32 rowIdx = 0;  rowIdx < numRows;  ++rowIdx)
  {
    KKStrList&  row = (*results)[rowIdx];

    bool  downCast     = row[3].ToBool  ();
    int32  bucketIdx   = row[4].ToInt32 ();
    float  bucketDepth = row[5].ToFloat ();
    int32  imageCount  = row[6].ToInt32 ();
    int32  pixelCount  = row[7].ToInt32 ();

    VectorInt32  counts (numCountCols, 0);
    for  (int32 x = 0;  x < numCountCols;  ++x)
      counts[x] = row[8 + x].ToInt32 ();

    float  volumeSampled = 0.0f;
    InstrumentDataMeansPtr  idm = instData->LookUp (downCast, bucketDepth);
    if  (idm)
      volumeSampled = idm->volumeSampled;

    KKStr  begOfLine (128);
    begOfLine << (downCast ? "Down" : "Up") << "\t" << bucketDepth << "\t" << volumeSampled << "\t" << imageCount << "\t" << pixelCount;

    r1 << begOfLine;

    for  (int32 x = 0;  x < numCountCols;  ++x)
      r1 << "\t" << counts[x];

    if  (idm)
    {
      KKStr  instDataStr (256);

      instDataStr << "\t" << idm->temperatureMean
                  << "\t" << idm->salinityMean 
                  << "\t" << idm->denisityMean
                  << "\t" << idm->fluorescenceMean
                  << "\t" << idm->fluorescenceSensorMean
                  << "\t" << idm->oxygenMean
                  << "\t" << idm->Oxygen_molPerKg_Mean ()
                  << "\t" << idm->transmisivityMean 
                  << "\t" << idm->turbidityMean;

      r1 << instDataStr;
    }

    r1 << endl;
  }

  r1 << "" << "\t" << "" << "\t" << "" << "\t" << "Integrated Counts" << "\t" << "";
  for  (uint32 x = 0;  x < integratedCounts.size ();  ++x)
    r1 << "\t" << integratedCounts[x];

  delete  instData;  instData = NULL;
  delete  results;   results  = NULL;

  return  new DeploymentSummary (deployment->CruiseName (), deployment->StationName (), deployment->DeploymentNum (), totalVolumeSampled, sizeThresholds, integratedAbundance, integratedCounts);
}  /* MarineSnowReportDeployment */




void  PrintSummaryReports (DataBasePtr                  db,
                           vector<DeploymentSummary*>&  summaries
                          )
{
  uint32  x = 0;

  ofstream  r ("C:\\Temp\\MarineSnow\\Summary.txt");

  KKStr svnVersionStr = QUOTE (_SVNVERSION_);

  r  << "DateTime"   << "\t" << osGetLocalDateTime ()        << endl
     << "DataBase"   << "\t" << db->ServerDescription ()     << endl
     << "ProgName"   << "\t" << osGetProgName ()             << endl
     << "BuildTime"  << "\t" << __DATE__ << " " << __TIME__  << endl
     << "SvnVersion" << "\t" << svnVersionStr                << endl
     << "HostName"   << "\t" << osGetHostName ()             << endl
     << "UserName"   << "\t" << osGetUserName ()             << endl
     << endl
     << endl
     << endl;

  r << "Summary  Integrated Counts" << endl
    << endl;

  vector<DeploymentSummary*>::iterator  idx;

  idx = summaries.begin ();

  VectorInt32  sizeThresholds = (*idx)->sizeThresholds;

  r << ""       << "\t" << ""        << "\t" << ""           << "\t" << "VolumeSampled" << endl;
  r << "Cruise" << "\t" << "Station" << "\t" << "Deployment" << "\t" << "m^3";
  for  (x = 0;  x < sizeThresholds.size ();  ++x)
    r << "\t" << sizeThresholds[x];
  r << endl;

  for  (idx = summaries.begin ();  idx != summaries.end ();  ++idx)
  {
    DeploymentSummary*  ds = *idx;

    r << ds->cruiseName << "\t" << ds->stationName << "\t" << ds->deploymentNum << "\t" << ds->volumeSampled;

    for  (x = 0;  x < ds->integratedCounts.size ();  ++x)
    {
      r << "\t" << ds->integratedCounts[x];
    }
    r << endl;
  }
  r << endl << endl << endl << endl;



  r << "Summary  Integrated Abundance" << endl
    << endl;

  r << ""       << "\t" << ""        << "\t" << ""           << "\t" << "VolumeSampled" << endl;
  r << "Cruise" << "\t" << "Station" << "\t" << "Deployment" << "\t" << "m^3";
  for  (x = 0;  x < sizeThresholds.size ();  ++x)
    r << "\t" << sizeThresholds[x];
  r << endl;

  for  (idx = summaries.begin ();  idx != summaries.end ();  ++idx)
  {
    DeploymentSummary*  ds = *idx;

    r << ds->cruiseName << "\t" << ds->stationName << "\t" << ds->deploymentNum << "\t" << ds->volumeSampled;

    for  (x = 0;  x < ds->sizeThresholds.size ();  ++x)
    {
      r << "\t" << ds->integratedAbundance[x];
    }
    r << endl;
  }
  r << endl << endl << endl << endl;




  r << "Summary  Integrated log10(Abundance)" << endl
    << endl;

  r << ""       << "\t" << ""        << "\t" << ""           << "\t" << "VolumeSampled" << endl;
  r << "Cruise" << "\t" << "Station" << "\t" << "Deployment" << "\t" << "m^3";
  for  (x = 0;  x < sizeThresholds.size ();  ++x)
    r << "\t" << sizeThresholds[x];
  r << endl;

  for  (idx = summaries.begin ();  idx != summaries.end ();  ++idx)
  {
    DeploymentSummary*  ds = *idx;

    r << ds->cruiseName << "\t" << ds->stationName << "\t" << ds->deploymentNum << "\t" << ds->volumeSampled;

    for  (x = 0;  x < ds->sizeThresholds.size ();  ++x)
    {
      double  zed = (double)(ds->integratedAbundance[x]);
      double log10AbundanceSize = 0.0;
      if (zed != 0.0)
        log10AbundanceSize = log10 (zed);
      r << "\t" << log10AbundanceSize;
    }
    r << endl;
  }
  r << endl << endl << endl << endl;





  r << "Summary  Integrated log10(Abundance-Size)" << endl
    << endl;

  r << ""       << "\t" << ""        << "\t" << ""           << "\t" << "VolumeSampled" << endl;
  r << "Cruise" << "\t" << "Station" << "\t" << "Deployment" << "\t" << "m^3";
  for  (x = 0;  x < sizeThresholds.size ();  ++x)
    r << "\t" << sizeThresholds[x];
  r << endl;

  for  (idx = summaries.begin ();  idx != summaries.end ();  ++idx)
  {
    DeploymentSummary*  ds = *idx;

    r << ds->cruiseName << "\t" << ds->stationName << "\t" << ds->deploymentNum << "\t" << ds->volumeSampled;

    for  (x = 0;  x < ds->sizeThresholds.size ();  ++x)
    {
      double  zed = (double)(ds->sizeThresholds[x]) * ds->integratedAbundance[x];
      double log10AbundanceSize = 0.0;
      if (zed != 0.0)
        log10AbundanceSize = log10 (zed);
      r << "\t" << log10AbundanceSize;
    }
    r << endl;
  }
  r << endl << endl << endl;
}  /* PrintSummaryReports */




void  MarineSnowReport ()
{
  RunLog  runLog;
  DataBasePtr  db = new DataBase (runLog);

  vector<DeploymentSummary*>  summaries;

  SipperDeploymentListPtr  deployments = db->SipperDeploymentLoad ("","");

  int  loopCount = 0;

  SipperDeploymentList::const_iterator  idx;
  for  (idx = deployments->begin ();  idx != deployments->end ();  ++idx)
  {
    SipperDeploymentPtr  deployment = *idx;

    if  ((deployment->CruiseName ().EqualIgnoreCase ("ETP2007"))  ||
         (deployment->CruiseName ().EqualIgnoreCase ("ETP2008"))  ||
         (deployment->CruiseName ().SubStrPart (0, 2).EqualIgnoreCase ("HRS"))
        )
     continue;
         
    DeploymentSummary*  sumary = MarineSnowReportDeployment (*idx, *db);
    if  (sumary)
      summaries.push_back (sumary);
    ++loopCount;
  }

  PrintSummaryReports (db, summaries);

  delete  db;
  db = NULL;
}  /* MarineSnowReport */









int  main (int    argc, 
           char** argv
          )
{

  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

  MarineSnowReport ();

  return 0;
}

