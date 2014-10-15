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

#include  "Compressor.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "SipperFile.h"
#include  "SipperVariables.h"
using namespace SipperHardware;


#include  "DataBase.h"
using namespace MLL;


#include "ReFreshInstrumentData.h"




ReFreshInstrumentData::ReFreshInstrumentData (int     argc, 
                                              char**  argv
                                             ):

   Application    (argc, argv),
   cruiseName     (),
   dbConn         (NULL),
   deploymentNum  (),
   stationName    ()
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

  InstrumentDataFileManager::InitializePush ();
  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }
}





ReFreshInstrumentData::~ReFreshInstrumentData ()
{
  delete  dbConn;  dbConn = NULL;
}

// -cn SMP751001 


bool  ReFreshInstrumentData::ProcessCmdLineParameter (
                                                      char    parmSwitchCode, 
                                                      KKStr   parmSwitch, 
                                                      KKStr   parmValue
                                                     )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  parmSwitch.Upper ();

  if  (parmSwitch.EqualIgnoreCase ("-Cruise")      ||  
       parmSwitch.EqualIgnoreCase ("-CruiseName")  ||  
       parmSwitch.EqualIgnoreCase ("-CN")
      )
  {
    cruiseName = parmValue;
  }

  else if  (parmSwitch.EqualIgnoreCase ("-Station")      ||  
            parmSwitch.EqualIgnoreCase ("-StationName")  ||  
            parmSwitch.EqualIgnoreCase ("-SN")
           )
  {
    stationName = parmValue;
  }

  else if  (parmSwitch.EqualIgnoreCase ("-DeploymentNum")      ||  
            parmSwitch.EqualIgnoreCase ("-Deployment")  ||  
            parmSwitch.EqualIgnoreCase ("-DN")
           )
  {
    deploymentNum = parmValue;
  }

  else
  {
    Abort (true);
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */


           

/******************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   ReFreshInstrumentData::DisplayCommandLineParameters ()
{
  log.Level (0) << "ReFreshInstrumentData"                << endl;
  log.Level (0)                                           << endl;
  log.Level (0) << "    -CruiseName  <Cruise Name>"       << endl;
  log.Level (0) << "    -StationName <Statuion NAme>"     << endl;
  log.Level (0) << "    -Deployment  <CDeploymentNum>"    << endl;
  log.Level (0)                                           << endl;
}  /* DisplayCommandLineParameters */





void   ReFreshInstrumentData::Main ()
{
  if  (Abort ())
    return;

  DateTime  runDate = osGetLocalDateTime ();

  KKStr reportFileDir = osAddSlash (SipperVariables::PicesReportDir ()) + "ReFreshInstrumentData";
  osCreateDirectoryPath (reportFileDir);

  reportFileName = osAddSlash (reportFileDir) + "ReFreshInstrumentData";
  if  (!cruiseName.Empty ())
    reportFileName << "_" << cruiseName;
  if  (!stationName.Empty ())
    reportFileName << "_" << stationName;
  if  (!deploymentNum.Empty ())
    deploymentNum << "_" << stationName;
  reportFileName << "_" << runDate.YYYYMMDDHHMMSS () + ".txt";


  dbConn = new DataBase (log);
  if  (!dbConn->Valid ())
  {
    log.Level (-1) << endl << endl << "Could not connect to Database" << endl << endl;
    exit (-1);
  }

  log.Level (10) << "ReFreshInstrumentData::Main   Connected to database[" << dbConn->ServerDescription () << "]." << endl;

  ofstream r (reportFileName.Str ());

  r << "ReFreshInstrumentData"  << endl
    << endl
    << "RunDate"     << "\t" << runDate                      << endl
    << "Cruise"      << "\t" << cruiseName                   << endl
    << "Station"     << "\t" << stationName                  << endl
    << "Deployment"  << "\t" << deploymentNum                << endl
    << "DataBase"    << "\t" << dbConn->ServerDescription () << endl
    << endl;
    
  InstrumentDataFileManager::InitializePush ();
  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad (cruiseName, stationName, deploymentNum);
  if  ((!sipperFiles)  ||  (sipperFiles->QueueSize () < 1))
  {
    log.Level (-1) << endl << endl << "No SIPPER Files Found." << endl << endl;
    r << "*** No SIPPER File Located in Database ***" << endl << endl;
    delete  sipperFiles;
    sipperFiles = NULL;
    return;
  }

  KKU::uint  numSipperFiles = sipperFiles->size ();
  KKU::uint  sipperFileNum = 0;

  log.Level (10) << "Number Of Sipper Files[" << numSipperFiles << "]." << endl;
  r << "Number Of Sipper Files" << "\t" << numSipperFiles << endl;

  SipperFileList::iterator  idx;

  bool   cancelFlag = false;
  char   msgBuff[1024];

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;
    sipperFileNum++;

    KKStr  fullSipperFileName = InstrumentDataFileManager::GetFullSipperFileName (sf->SipperFileName ());
    if  (fullSipperFileName.Empty ())
    {
      log.Level (-1) << endl << "Sipper File: " << sf->SipperFileName () <<  "*** Not Found ***" << endl << endl;
      r << endl << "Sipper File: " << sf->SipperFileName () <<  "*** Not Found ***" << endl << endl;
      continue;
    }

    log.Level (10) << " " << sipperFileNum << " of " << numSipperFiles << "  File [" << sf->SipperFileName () << "]" << endl;
    r << osGetLocalDateTime () << "\t" << sf->SipperFileName () << "\t" << fullSipperFileName << endl;
    dbConn->InstrumentDataReFreshSipperFile (sf->SipperFileName (), cancelFlag, msgBuff, sizeof (msgBuff));
  }

  r << endl << "Done Processing" << endl;

  InstrumentDataFileManager::InitializePop ();

  delete  sipperFiles;
  sipperFiles = NULL;
}  /* Main */




int  main (int     argc,
           char**  argv
          )
{
  ReFreshInstrumentData  reFreshInstrumentData (argc, argv);
  if  (reFreshInstrumentData.Abort ())
    return -11;

  reFreshInstrumentData.Main ();
  if  (reFreshInstrumentData.Abort ())
    return -11;
  else
    return 0;
}
