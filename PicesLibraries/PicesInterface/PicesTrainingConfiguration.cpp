#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <strstream>
#include  <vector>
using namespace std;

#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "MemoryDebug.h"

#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "DuplicateImages.h"
#include  "FileDesc.h"
#include  "FeatureFileIOPices.h"
#include  "SipperVariables.h"
#include  "TrainingConfiguration2.h"
using namespace  MLL;


#include  "PicesTrainingConfiguration.h"

#include  "PicesKKStr.h"
#include  "PicesRunLog.h"
using namespace PicesInterface;

using namespace System;
using namespace System::Threading;

namespace  PicesInterface
{

PicesTrainingConfiguration::PicesTrainingConfiguration (PicesRunLog^  _log):
  cancelFlag (new bool (false)),
  classes           (NULL),
  config            (NULL),
  curMemoryPressure (0),
  loadLogStream     (nullptr),
  log               (_log),
  valid             (gcnew System::Boolean (false))

{
  AdjustCurMemoryPressure ();
}



PicesTrainingConfiguration::PicesTrainingConfiguration (String^       _configFileName,
                                                        PicesRunLog^  _log
                                                       ):
  cancelFlag (new bool (false)),
  classes           (NULL),
  config            (NULL),
  curMemoryPressure (0),
  loadLogStream     (nullptr),
  log               (_log),
  valid             (gcnew System::Boolean (false))
{
  strstream  logStr;
  KKStr  configFileName = PicesKKStr::SystemStringToKKStr (_configFileName);
  FileDescPtr  fileDesc = FeatureFileIOPices::NewPlanktonFile (log->Log ());
  config = new TrainingConfiguration2 (fileDesc, configFileName, log->Log (), true);
  valid = gcnew System::Boolean (config->FormatGood ());
  if  (!config->FormatGood ())
     loadLogStream = gcnew String (logStr.str ());
  AdjustCurMemoryPressure ();
}



PicesTrainingConfiguration::PicesTrainingConfiguration (DirectoryInfo^  di,
                                                        PicesRunLog^    _log
                                                       ):
  cancelFlag        (new bool (false)),
  classes           (NULL),
  config            (NULL),
  curMemoryPressure (0),
  loadLogStream     (nullptr),
  log               (_log),
  valid             (gcnew Boolean (false))

{
  osRunAsABackGroundProcess ();

  KKStr  errorMessage;

  KKStr  dirName    = PicesKKStr::SystemStringToKKStr (di->FullName);
  bool   successful = false;

  KKStr  modelNameKKStr = osGetRootNameOfDirectory (dirName) + ".cfg";
  KKStr  configFileName = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + modelNameKKStr;
    
  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (log->Log ());
  delete  classes;  classes = NULL;

  config = TrainingConfiguration2::CreateFromDirectoryStructure 
                                        (fd,
                                         configFileName,
                                         dirName,
                                         log->Log (),
                                         successful,
                                         errorMessage
                                        );

  valid = gcnew System::Boolean (successful);

  classes = config->ExtractClassList ();

  // We are nvere ever suposed to delete a 'FileDesc' object.  they are managed wholly in the "FileDesc" class.  
  // Other parts of the app may and probably will be using this same instance.
  //delete  fd;  fd = NULL;
  AdjustCurMemoryPressure ();
}  /* CreateTrainingModelFromDirectory */
  


PicesTrainingConfiguration::~PicesTrainingConfiguration ()
{
  this->!PicesTrainingConfiguration ();
}



PicesTrainingConfiguration::!PicesTrainingConfiguration ()
{
  delete  cancelFlag;  cancelFlag = NULL;
  delete  config;      config     = NULL;
  delete  classes;     classes    = NULL;
  if  (curMemoryPressure > 0)
    GC::RemoveMemoryPressure (curMemoryPressure);
  curMemoryPressure = 0;
}


void  PicesTrainingConfiguration::AdjustCurMemoryPressure ()
{
  int  newMemoryPressure = 0;
  if  (config)   newMemoryPressure += config->MemoryConsumedEstimated ();
  if  (classes)  newMemoryPressure += classes->MemoryConsumedEstimated ();

  int  deltaMemororyPressure = newMemoryPressure - curMemoryPressure;
  if  (deltaMemororyPressure > 0)
    GC::AddMemoryPressure (deltaMemororyPressure);

  else if  (deltaMemororyPressure < 0)
    GC::RemoveMemoryPressure (-deltaMemororyPressure);

  curMemoryPressure = newMemoryPressure;
}




void  PicesTrainingConfiguration::CancelLoad ()  // Call this methid to set the 'canclFlag' to true.
{
  *cancelFlag = true;
}


String^   PicesTrainingConfiguration::RootDir ()
{
  return  PicesKKStr::KKStrToSystenStr (config->RootDir ());
}



String^   PicesTrainingConfiguration::ModelName ()
{
  if  (!config)
    return  System::String::Empty;

  return  PicesKKStr::KKStrToSystenStr (osGetRootNameWithExtension (config->ConfigFileNameSpecified ()));
}  /* ModelName */





System::Boolean  PicesTrainingConfiguration::Valid ()
{
  return  *valid;
}  /* Valid */




uint  PicesTrainingConfiguration::NumHierarchialLevels ()
{
  if  (config)
    return  config->NumHierarchialLevels ();
  else
    return 0;
}




PicesClassList^  PicesTrainingConfiguration::ExtractClassList ()
{
  if  (config == NULL)
    return  nullptr;
  MLClassConstListPtr  classes = config->ExtractClassList ();

  PicesClassList^  results = gcnew PicesClassList (classes);
  delete classes;
  classes = NULL;

  return  results;
}




String^  PicesTrainingConfiguration::GetEffectiveConfigFileName (String^  configFileName)
{
  KKStr  effectiveFileName = TrainingConfiguration2::GetEffectiveConfigFileName (PicesKKStr::SystemStringToKKStr (configFileName));
  return  PicesKKStr::KKStrToSystenStr (effectiveFileName);
}



PicesClass^  PicesTrainingConfiguration::OtherClass ()
{
  PicesClass^ otherClass = nullptr;

  if  (config != NULL)
  {
    MLClassConstPtr  oc = config->OtherClass ();
    if  (oc != NULL)
      otherClass = PicesClassList::GetUniqueClass (oc);
  }
  return  otherClass;
}  /* OtherClass */




PicesFeatureVectorList^  PicesTrainingConfiguration::LoadFeatureDataFromTrainingLibraries (bool^ _changesMadeToTrainingLibraries)
{
  if  (!config)
    return  nullptr;

  *cancelFlag = false;

  bool  changesMadeToTrainingLibraries = false;

  KKU::DateTime  latestImageTimeStamp;

  FeatureVectorListPtr  data = config->LoadFeatureDataFromTrainingLibraries 
         (latestImageTimeStamp,
          changesMadeToTrainingLibraries,
          *cancelFlag
         );

  if  (data == NULL)
    return nullptr;

  if  (*cancelFlag)
  {
    delete  data;
    data=NULL;
   return nullptr;
  }

  {
    // Eliminate Duplicate data.
    DuplicateImages dup (data, this->log->Log ());
    dup.PurgeDuplicates (data, NULL);
  }

  ImageFeaturesListPtr  imageData = new ImageFeaturesList (*data, false);
  imageData->Owner (true);
  data->Owner (false);
  delete  data;
  data = NULL;

  PicesFeatureVectorList^ pfvl = gcnew PicesFeatureVectorList (*imageData);
  imageData->Owner (false);
  delete  imageData;  imageData = NULL;

  _changesMadeToTrainingLibraries = changesMadeToTrainingLibraries;

  //*_changesMadeToTrainingLibraries = changesMadeToTrainingLibraries;

  return  pfvl;
}  /* LoadFeatureDataFromTrainingLibraries */




PicesTrainingClassList^   PicesTrainingConfiguration::TrainingClasses ()
{
  PicesTrainingClassList^  results = gcnew PicesTrainingClassList ();

  const TrainingClassList&  trainingClasses = config->TrainingClasses ();
  TrainingClassList::const_iterator  idx;
  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    const TrainingClassPtr  tc = *idx;
    TrainingClassPtr  newTc = new TrainingClass (*tc);
    results->Add (gcnew PicesTrainingClass (newTc));
  }

  return  results;
}  /* TrainingClasses */




void   PicesTrainingConfiguration::Save (String^  configFileName)
{
  KKStr  fileName = PicesKKStr::SystemStringToKKStr (configFileName);
  config->Save (fileName);
}  /* Save */

}  /* PicesInterface */