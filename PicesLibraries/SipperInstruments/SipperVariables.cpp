#include "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>


#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace std;


#include "OSservices.h"
using namespace KKU;

#include "SipperVariables.h"
using namespace  SipperHardware;


KKU::KKStrPtr       SipperVariables::picesHomeDir = NULL;
KKU::GoalKeeperPtr  SipperVariables::blocker      = NULL;



SipperVariables::SipperVariables ()
{
}



SipperVariables::~SipperVariables ()
{
}



void  SipperVariables::FinalCleanUp ()
{
  delete  picesHomeDir;  picesHomeDir = NULL;
  delete  blocker;       blocker = NULL;
}



bool  SipperVariables::allowUpdates            = false;
bool  SipperVariables::picesHomeDirInitialized = false;



bool  SipperVariables::ValidPicesHomeDir (const KKStr&  dir)
{
  // The Pices home directory will have a couple of sub directories below it which must exist.
  return   ((osValidDirectory (osAddSlash (dir) + "exe"))               &&
            (osValidDirectory (osAddSlash (dir) + "DataFiles"))         &&
            (osValidDirectory (osAddSlash (dir) + "MySQL"))             &&
            (osValidDirectory (osAddSlash (dir) + "TrainingLibraries"))
           );
}




KKStr  SipperVariables::PicesHomeDir ()
{
  if  (picesHomeDir != NULL)
    return *picesHomeDir;
  
  if  (blocker == NULL)
    GoalKeeper::Create ("SipperVariables", blocker);

  blocker->StartBlock ();

  if  (!picesHomeDirInitialized)
  {
    picesHomeDir = osGetEnvVariable ("PicesHomeDir");
    if  (picesHomeDir == NULL)
    {
      KKStr  fullAppName = osGetFullPathOfApplication ();
      KKStr  pathToApp = osGetPathPartOfFile (fullAppName);
      KKStr  homeDir = osGetParentDirPath (pathToApp);
      if  (ValidPicesHomeDir (homeDir))
      {
        picesHomeDir = new KKStr (homeDir);
      }
      else
      {
        KKStr  curDir = osGetCurrentDirectory ();
        if  (ValidPicesHomeDir (curDir))
        {
          picesHomeDir = new KKStr (curDir);
        }
        else
        {
          homeDir = osGetParentDirPath (curDir);
          if  (ValidPicesHomeDir (homeDir))
          {
            picesHomeDir = new KKStr (homeDir);
          }
          else
          {
            osDisplayWarning ("Can Not locate a valid PICES Hom Directory;    make sure 'PicesHomeDir' is setup correctly.");
          }
        }
      }
    }

    picesHomeDirInitialized = true;
    ReadPermissions ();
    atexit (SipperVariables::FinalCleanUp);
  }

  ReadPermissions ();

  blocker->EndBlock ();

  if  (picesHomeDir == NULL)
    return KKStr::EmptyStr ();
  else
    return *picesHomeDir;

}  /* PicesHomeDir */



void  SipperVariables::SetPicesHomeDir (const KKStr&  _picesHomeDir)
{
  picesHomeDir = new KKStr (_picesHomeDir);
}



KKStr  SipperVariables::PicesCmdLineProgDir ()
{
  KKStr  s = osAddSlash (PicesHomeDir ()) + "exe";
  return s;
}




KKStr  SipperVariables::InstrumentDataFilesDir ()
{
  KKStr  s = osAddSlash (PicesDataFilesDirectory ()) + "InstrumentDataFiles";
  return  s;
}
 


KKStr  SipperVariables::InstrumentDataReportDir ()
{
  return  osAddSlash (SipperVariables::PicesReportDir ()) + "InstrumentData";
}


KKStr  SipperVariables::SipperInterfaceDir ()
{
  KKStrPtr  sidPtr = osGetEnvVariable ("SipperInterfaceDir");
  if  (sidPtr)
    return  KKStr (*sidPtr);

  KKStr  sipperInterfaceDir = osAddSlash (PicesHomeDir ()) +  "SipperInterfaceDir";
  return  sipperInterfaceDir;
}



KKStr  SipperVariables::SipperInterfaceLogDir ()
{
  KKStr  sipperInterfaceLogDir = osAddSlash (SipperInterfaceDir ()) + "LogFiles";
  return  sipperInterfaceLogDir;
}  /* SipperInterfaceLogDir */





KKStr  SipperVariables::PicesConfigurationDirectory ()
{
  KKStr  configDir = osAddSlash (osAddSlash (PicesHomeDir ()) + "Configurations");
  return  configDir;
}



KKStr  SipperVariables::SipperConfigrationFileName ()
{
  KKStr  configFileName = osAddSlash (PicesConfigurationDirectory ()) + "SipperConfiguration.txt";
  return  configFileName;
}



KKStr  SipperVariables::PicesDataFilesDirectory ()
{
  return  osAddSlash (PicesHomeDir ()) + "DataFiles";
}



KKStr  SipperVariables::SipperDirectoryListFileName ()
{
  KKStr  sipperDirectoryListFileName = osAddSlash (PicesConfigurationDirectory ()) + "SipperDirectoryList.cfg";
  return  sipperDirectoryListFileName;
}



KKStr  SipperVariables::SipperFilesDefaultDir ()
{
  KKStr  sipperFilesDefaultDir = osAddSlash (PicesHomeDir ()) + "SipperFiles";
  return  sipperFilesDefaultDir;
}



KKStr  SipperVariables::TrainingModelsConfigurationDir ()
{
  KKStr  trainingModelsConfigurationDir = osAddSlash (PicesDataFilesDirectory ()) + "TrainingModels";
  return  trainingModelsConfigurationDir;
}


KKStr  SipperVariables::TrainingModelsSaveFilesDir ()
{
  KKStr  trainingModelsSaveFilesDir = osAddSlash (TrainingModelsConfigurationDir ()) + "SaveFiles";
  return  trainingModelsSaveFilesDir;
}



KKStr  SipperVariables::TrainingLibrariesDir ()
{
  return  osAddSlash (PicesHomeDir ()) + "TrainingLibraries";
}


KKStr  SipperVariables::ValidationInfoDir ()
{
  return osAddSlash (SipperVariables::PicesDataFilesDirectory ()) + "ValidationHistory";
}



KKStr  SipperVariables::PicesReportDir ()
{
  return  osAddSlash (PicesHomeDir ()) + "Reports";
}



KKStr  SipperVariables::PicesDataFileDirectory ()
{
  return  osAddSlash (PicesHomeDir ()) + "DataFiles";
}



KKStr  SipperVariables::PicesTempDirectory ()
{
  KKStr  tempDirName = osAddSlash (PicesDataFileDirectory ()) + "Temp";
  if  (!osValidDirectory (tempDirName))
    osCreateDirectoryPath (tempDirName);
  return  tempDirName;
}



int32  SipperVariables::LocateEnvStrStart (const KKStr&  str)
{
  int32  x = 0;
  int32  y = 1;
  int32  len = str.Len ();
  const char*  s = str.Str ();

  while  (y < len)
  {
    if  (s[y] == 0)
      return -1;

    if  (s[x] == '$')
    {
      if  ((s[y] == '(')  ||  (s[y] == '{')  ||  (s[y] == '['))
        return  x;
    }

    x++;
    y++;
  }

  return  -1;
}  /* LocateEnvStrStart */




KKStr  SipperVariables::SubstituteInEvironmentVariables (const KKStr&  src)
{
  int32  x = LocateEnvStrStart (src);
  if  (x < 0)
    return  src;

  char  startChar = src[x + 1];
  char  endChar = ')';
  if  (startChar == '(')
    endChar = ')';

  else if  (startChar == '{')
    endChar = '}';

  else if  (startChar == '[')
    endChar = ']';

  KKStr  str (src);

  while  (x >= 0)
  {
    KKStr  beforeEnvStr = str.SubStrPart (0, x - 1);
    str = str.SubStrPart (x + 2);
    x = str.LocateCharacter (endChar);
    if  (x < 0)
      return  src;

    KKStr  envStrName   = str.SubStrPart (0, x - 1);
    KKStr  afterStrName = str.SubStrPart (x + 1);

    KKStrPtr envStrValue = osGetEnvVariable (envStrName);
    if  (envStrValue == NULL)
    {
      if  (envStrName.EqualIgnoreCase ("PicesHomeDir"))
        envStrValue = new KKStr (SipperVariables::PicesHomeDir ());
    }

    if  (envStrValue == NULL)
      return  src;

    str = beforeEnvStr + (*envStrValue)  + afterStrName;
    delete  envStrValue;
    x = LocateEnvStrStart (str);
  }

  return  str;
}  /* osSubstituteInEvironmentVariables */


KKStr  SipperVariables::LatitudeToStr  (double latitude)
{
  bool  north = (latitude >= 0.0);
  latitude = fabs (latitude);

  while  (latitude > 90)
  {
    latitude = 180 - latitude;
    if  (latitude < 0.0)
    {
      latitude = fabs (latitude);
      north = !north;
    }
  }

  double  latitudeDegrees = 0.0;
  double  latitudeMinutes = modf (latitude, &latitudeDegrees) * 60.0;

  KKStr  latStr (12);
  latStr << StrFormatInt ((int)latitudeDegrees, "00") 
         << ":"
         << StrFormatDouble (latitudeMinutes, "#00.0000")
         << " "
         << (north ? "N" : "S");
  return  latStr;
}  /* LatitudeToStr */



KKStr  SipperVariables::LongitudeToStr  (double longitude)
{
  bool  east = longitude >= 0.0;
  longitude = fabs (longitude);
  while  (longitude > 180.0)
  {
    longitude = longitude - 360.0;
    if  (longitude < 0.0)
    {
      longitude = fabs (longitude);
      east = !east;
    }
  }

  double  longitudeDegrees = 0.0;
  double  longitudeMinutes = modf (longitude, &longitudeDegrees) * 60.0;

  KKStr  longStr (12);
  longStr << StrFormatInt ((int)longitudeDegrees, "##0")
    << ":" 
    << StrFormatDouble (longitudeMinutes, "#00.0000")
    << " "
    << (east  ? "E" : "W");
  return  longStr;
}  /* LongitudeToStr */



bool  SipperVariables::AllowUpdates ()
{
  if  (!picesHomeDirInitialized)
    PicesHomeDir ();

  return  allowUpdates;
}




void    SipperVariables::ReadPermissions ()
{
  allowUpdates = false;
  KKStr  configFileName = osAddSlash (SipperVariables::PicesConfigurationDirectory ()) + "Permissions.cfg";

  FILE* in = osFOPEN (configFileName.Str (), "r");
  if  (!in)
  {
    cerr << "DataBaseServerList::ReadPermissions    Error loading ConfigFileName[" <<  configFileName << "]" << endl;
    return;
  }

  char  buff[1024];
  while  (fgets (buff, sizeof (buff), in))
  {
    buff[sizeof(buff) - 1] = 0;  // Just to make sure that bufer not comepletely full
    KKStr  ln (buff);

    if  (ln.Len () < 3)
      continue;

    if  ((ln[0] == '/')  &&  (ln[1] == '/'))
      continue;

    KKStr  lineName = ln.ExtractToken2 ("\t=");
    if  (lineName.EqualIgnoreCase ("AllowUpdates"))
    {
      allowUpdates = ln.ToBool ();
    }
  }

  fclose (in);
}
