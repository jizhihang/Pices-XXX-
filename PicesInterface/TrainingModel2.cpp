#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "ImageIO.h"
#include  "Raster.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace KKU;


#include  "InstrumentDataFileManager.h"

#include  "Classifier2.h"
#include  "ClassProb.h"
#include  "FeatureFileIOPices.h"
#include  "TrainingConfiguration2.h"

#include  "PicesFeatureVector.h"
#include  "PicesFeatureVectorList.h"
#include  "PicesSipperVariables.h"
#include  "PicesTrainingConfiguration.h"
#include  "PicesKKStr.h"
#include  "PicesRaster.h"
#include  "SipperVariables.h"

#include  "TrainingModel2.h"

using namespace PicesInterface;


using namespace std;
using namespace System;
using namespace System::Drawing::Imaging;
using namespace System::Windows::Forms;
using namespace System::Threading;
  
  
namespace  PicesInterface
{
TrainingModel2::TrainingModel2 (PicesRunLog^     _picesRunLog,
                                System::String^  _modelName
                               ):

              cancelFlag                 (new bool),
              classifier                 (NULL),
              classes                    (NULL),
              classList                  (gcnew  PicesClassList ()),
              colorValues                (nullptr),
              config                     (NULL),
              crossProbTable             (NULL),
              crossProbTableNumClasses   (0),
              curMemoryPressure          (0),
              loadTrainingLibraryRunning (false),
              modelName                  (System::String::Copy (_modelName)),
              picesRunLog                (_picesRunLog),
              probabilities              (NULL),
              runLog                     (NULL),
              runLogFileName             (nullptr),
              runLogLastLineNum          (0),
              statusMsg                  (new KKStr ()),
              trainer                    (NULL),
              trainerWeOwn               (false),
              valid                      (new bool (true)),
              votes                      (NULL)
               
{
  if  (System::String::IsNullOrEmpty (_modelName))
  {
    *valid = false;
    return;
  }

  GC::Collect ();

  CreateRunLog ();

  osRunAsABackGroundProcess ();

  UpdateMemoryPressure ();
}





TrainingModel2::TrainingModel2 (PicesRunLog^                 _picesRunLog,
                                PicesTrainingConfiguration^  _config
                               ):

              cancelFlag                 (new bool),
              classifier                 (NULL),
              classes                    (NULL),
              classList                  (gcnew  PicesClassList ()),
              colorValues                (nullptr),
              config                     (NULL),
              crossProbTable             (NULL),
              crossProbTableNumClasses   (0),
              curMemoryPressure          (0),
              loadTrainingLibraryRunning (false),
              modelName                  (_config->ModelName ()),
              picesRunLog                (_picesRunLog),
              probabilities              (NULL),
              runLog                     (NULL),
              runLogFileName             (nullptr),
              runLogLastLineNum          (0),
              statusMsg                  (new KKStr ()),
              trainer                    (NULL),
              trainerWeOwn               (false),
              valid                      (new bool (true)),
              votes                      (NULL)
               
{
  if  (_config == nullptr)
  {
    *valid = false;
    return;
  }

  GC::Collect ();
  CreateRunLog ();
  osRunAsABackGroundProcess ();
  config = new TrainingConfiguration2 (*(_config->Config ()));
  UpdateMemoryPressure ();
}






TrainingModel2::TrainingModel2 (PicesRunLog^                _picesRunLog,
                                System::IO::DirectoryInfo^  _directoryInfo
                               ):
              cancelFlag                 (new bool),
              classifier                 (NULL),
              classes                    (NULL),
              classList                  (gcnew  PicesClassList ()),
              config                     (NULL),
              crossProbTable             (NULL),
              crossProbTableNumClasses   (0),
              curMemoryPressure          (0),
              loadTrainingLibraryRunning (false),
              modelName                  (nullptr),
              picesRunLog                (_picesRunLog),
              probabilities              (NULL),
              runLog                     (NULL),
              runLogFileName             (nullptr),
              runLogLastLineNum          (0),
              statusMsg                  (new KKStr ()),
              trainer                    (NULL),
              trainerWeOwn               (false),
              valid                      (new bool (true)),
              votes                      (NULL)
{
  if  (_directoryInfo == nullptr)
  {
    *valid = false;
    return;
  }

  osRunAsABackGroundProcess ();

  GC::Collect ();

  KKStr  errorMessage;

  KKStr  dirName    = PicesKKStr::SystemStringToKKStr (_directoryInfo->FullName);
  bool   successful = false;

  KKStr  modelNameKKStr = osGetRootNameOfDirectory (dirName) + ".cfg";
  modelName = PicesKKStr::KKStrToSystenStr (modelNameKKStr);

  CreateRunLog ();

  KKStr  configFileName = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + modelNameKKStr;

  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (*runLog);


  try
  {
    config = TrainingConfiguration2::CreateFromDirectoryStructure 
                                          (fd,
                                           configFileName,
                                           dirName,
                                           *runLog,
                                           successful,
                                           errorMessage
                                          );
  }
  catch  (Exception^ e)
  {
    successful = false;
    errorMessage = PicesKKStr::SystemStringToKKStr (e->ToString ());
  }
  if  (!successful)
  {
    delete config;   config = NULL;
    *valid = false;
  }
  else
  {
    delete  classes;  classes = NULL;
    classes = config->ExtractClassList ();
    PopulateCSharpClassList ();
  }

  UpdateMemoryPressure ();
}





TrainingModel2::TrainingModel2 (PicesRunLog^         _picesRunLog,
                                TrainingProcess2Ptr  _trainer
                               ):
              cancelFlag                 (new bool),
              classifier                 (NULL),
              classes                    (NULL),
              classList                  (gcnew  PicesClassList ()),
              config                     (NULL),
              crossProbTable             (NULL),
              crossProbTableNumClasses   (0),
              curMemoryPressure          (0),
              loadTrainingLibraryRunning (false),
              modelName                  (nullptr),
              picesRunLog                (_picesRunLog),
              probabilities              (NULL),
              runLog                     (NULL),
              runLogFileName             (nullptr),
              runLogLastLineNum          (0),
              statusMsg                  (new KKStr ()),
              trainer                    (_trainer),
              trainerWeOwn               (false),
              valid                      (new bool (true)),
              votes                      (NULL)
{
  if  (!_trainer)
  {
    *valid = false;
    return;
  }

  runLog = new RunLog ();

  osRunAsABackGroundProcess ();

  GC::Collect ();

  KKStr  errorMessage;

  if  (trainer->Config ())
    config = new TrainingConfiguration2 (*(trainer->Config ()));

  if  (trainer->Abort ())
    *valid = false;

  classifier = new Classifier2 (trainer, *runLog);
  delete  classes;  classes = NULL;

  if  (trainer->MLClasses ())
    classes = new MLClassConstList (*(trainer->MLClasses ()));

  PopulateCSharpClassList ();

  modelName = PicesKKStr::KKStrToSystenStr (osGetRootName (config->FileName ()));

  UpdateMemoryPressure ();
}




TrainingModel2::~TrainingModel2 ()
{
  this->!TrainingModel2 (); 
}



TrainingModel2::!TrainingModel2()
{
  CleanUpUnmanagedResources ();
  GC::RemoveMemoryPressure (curMemoryPressure);
  curMemoryPressure = 0;
}


void  TrainingModel2::CleanUp ()
{
  CleanUpUnmanagedResources ();
}



void  TrainingModel2::UpdateMemoryPressure ()
{
  int  newMemoryPressure = 2 * sizeof (bool) + 
                           crossProbTableNumClasses * crossProbTableNumClasses * sizeof (double) +   // crossProbTable
                           crossProbTableNumClasses *  sizeof (double) +                             // probabilities
                           crossProbTableNumClasses *  sizeof (int);                                 // votes

  if  (classifier)  newMemoryPressure += classifier->MemoryConsumedEstimated ();
  if  (classes)     newMemoryPressure += classes->MemoryConsumedEstimated ();
  if  (config)      newMemoryPressure += config->MemoryConsumedEstimated ();
  if  (statusMsg)   newMemoryPressure += statusMsg->MemoryConsumedEstimated ();

  if  (trainer  && trainerWeOwn)
    newMemoryPressure += trainer->MemoryConsumedEstimated ();

  int  deltaMemoryPressure = newMemoryPressure - curMemoryPressure;
  if  (deltaMemoryPressure > 0)
    GC::AddMemoryPressure (deltaMemoryPressure);

  else if  (deltaMemoryPressure < 0)
    GC::RemoveMemoryPressure (-deltaMemoryPressure);

  curMemoryPressure = newMemoryPressure;
}  /* UpdateMemoryPressure */




void  TrainingModel2::CleanUpUnmanagedResources ()
{
  delete  classifier;
  classifier = NULL;

  if  (trainerWeOwn)
  {
    try
    {
      delete  trainer;         
      trainer = NULL;
    }
    catch  (const exception&  e1)
    {
      runLog->Level (-1) << endl
        << "TrainingModel2::CleanUpUnmanagedResources  ***ERROR***     Exception destroying the 'Trainer Objecvt." << endl
        << "                Exception[" << e1.what () << "]" << endl
        << endl;

    }

    catch  (const char* e2)
    {
      runLog->Level (-1) << endl
        << "TrainingModel2::CleanUpUnmanagedResources  ***ERROR***     Exception destroying the 'Trainer Objecvt." << endl
        << "                Exception[" << e2 << "]" << endl
        << endl;
    }

    catch  (...)
    {
      runLog->Level (-1) << endl
        << "TrainingModel2::CleanUpUnmanagedResources  ***ERROR***     Exception destroying the 'Trainer Objecvt." << endl
        << endl;

    }
  }
  trainer = NULL;
 
  delete  cancelFlag;      cancelFlag     = NULL;
  delete  classes;         classes        = NULL;
  delete  config;          config         = NULL;
  delete  statusMsg;       statusMsg      = NULL;
  delete  valid;           valid          = NULL;
  delete  probabilities;   probabilities  = NULL;
  delete  votes;           votes          = NULL;

  if  (crossProbTable)
  {
    for  (int x = 0;  x < crossProbTableNumClasses;  x++)
      delete  crossProbTable[x];
    delete  crossProbTable;
    crossProbTable = NULL;
  }


  if  (runLog)
  {
    (*runLog).Level (10) << "TrainingModel2::CleanUpUnmanagedResources  Done Cleaning Up." << endl;
    delete  runLog;
    runLog = NULL;
  }

  GC::Collect ();
}  /* CleanUpUnmanagedResources */



TrainingModel2^   TrainingModel2::CreateTrainingModelLeft ()
{
  if  ((ModelType != ModelTypes::Dual)  ||  (!trainer))
    return nullptr;

  TrainingProcess2Ptr  leftTP = trainer->TrainingProcessLeft ();
  if  (!leftTP)
    return nullptr;

  return gcnew TrainingModel2 (picesRunLog, leftTP);
}  /* CreateTrainingModelLeft */



TrainingModel2^   TrainingModel2::CreateTrainingModelRight ()
{
  if  ((ModelType != ModelTypes::Dual)  ||  (!trainer))
    return nullptr;

  TrainingProcess2Ptr  rightTP = trainer->TrainingProcessRight ();
  if  (!rightTP)
    return nullptr;

  return gcnew TrainingModel2 (picesRunLog, rightTP);
}  /* CreateTrainingModelRight */




void  TrainingModel2::CreateRunLog ()
{
  if  (runLog == NULL)
  {
    String^  dir = OSservices::AddSlash (PicesSipperVariables::PicesTempDirectory ()) + "RunLogs";
    OSservices::CreateDirectoryPath (dir);

    bool fileNameAlreadyUsed = true;
    do  
    {
      runLogFileName = OSservices::AddSlash (dir) + OSservices::GetRootName (modelName) + "_RunLog_" + System::DateTime::Now.ToString ("yyyyMMdd-HHmmss-fff") + ".txt";
      fileNameAlreadyUsed  = File::Exists (runLogFileName);
      if  (fileNameAlreadyUsed)
        Thread::Sleep (100);
    }  
      while  (fileNameAlreadyUsed);

    runLog = new RunLog (PicesKKStr::SystemStringToKKStr (runLogFileName).Str ());
  }
}  /* CreateRunLog */



String^  TrainingModel2::LastRunLogTextLine ()
{
  if  (runLog == NULL)
    return nullptr;

  String^  lastLine = nullptr;

  if  (runLogLastLineNum != runLog->LineCount ())
  {
    lastLine = gcnew String (PicesKKStr::KKStrToSystenStr (runLog->LastLine ()));
    runLogLastLineNum = runLog->LineCount ();
  }

  return  lastLine;
}  /* LastRunLogTextLine */






System::DateTime  TrainingModel2::BuildDateTime::get ()
{
  if  (!trainer)
    return System::DateTime (1900, 1, 1);

  return  OSservices::KKuDateTimeToSystemDateTime (trainer->BuildDateTime ());
}



TrainingModel2::ModelTypes   TrainingModel2::ModelType::get ()  
{
  if  (trainer)
    return  static_cast<ModelTypes> (trainer->ModelType ());
  else
    return ModelTypes::Null;
}  /* ModelTypes */



PicesClass^  TrainingModel2::OtherClass::get ()
{
  MLClassConstPtr  otherClass = NULL;

  if  (config)
    otherClass = config->OtherClass ();

  else if  (!trainer)
  {
    TrainingConfiguration2Ptr trainConfig = trainer->Config ();
    if  (config)
      otherClass = trainConfig->OtherClass ();
  }

  if  (otherClass)
    return PicesClassList::GetUniqueClass (otherClass);
  else
    return nullptr;
}




String^  TrainingModel2::RootDir::get ()
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (configToUse)
     return PicesKKStr::KKStrToSystenStr (configToUse->RootDir ());
  else
     return  OSservices::AddSlash (PicesSipperVariables::PicesHomeDir ()) + OSservices::GetRootName (modelName);
}



void  TrainingModel2::SaveConfiguration ()
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  KKStr  fileName = configToUse->FileName ();
  if  (fileName.Empty ())
    fileName = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + PicesKKStr::SystemStringToKKStr (modelName);

  configToUse->Save (fileName);
}  /* SaveConfiguration*/





void  TrainingModel2::PopulateCSharpClassList ()
{
  osRunAsABackGroundProcess ();

  if  (classList != nullptr)
  {
    classList = nullptr;

    delete  probabilities;  probabilities = NULL;
    delete  votes;          votes = NULL;
    
    if  (crossProbTable)
    {
      for  (int x = 0;  x < crossProbTableNumClasses;  x++)
        delete  crossProbTable[x];
      delete  crossProbTable;
      crossProbTable = NULL;
    }
  }

  classList = gcnew PicesClassList ();
  if  (classes != NULL)
  {
    crossProbTableNumClasses = classes->QueueSize ();
    crossProbTable = new double*[crossProbTableNumClasses];
    int  idx;
    for  (idx = 0;  idx < crossProbTableNumClasses;  idx++)
    {
      MLClassConstPtr  ic = classes->IdxToPtr (idx);
      System::String^  className = PicesKKStr::KKStrToSystenStr (ic->Name ());
      PicesClass^  cSharpClass = PicesClassList::GetUniqueClass (className, "");
      classList->Add (cSharpClass);

      crossProbTable[idx] = new double[crossProbTableNumClasses];

      for  (int  idx2 = 0;  idx2 < crossProbTableNumClasses;  idx2++)
         crossProbTable[idx][idx2] = 0.0;
    }

    probabilities = new double[classes->QueueSize () + 10];
    votes         = new int   [classes->QueueSize () + 10];
  }
}  /* PopulateCSharpClassList */





bool  TrainingModel2::IncludesClass (PicesClass^  mlClass)
{
  if  (mlClass == nullptr)
    return false;

  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (!configToUse)
    return false;

  KKStr  dirPath = configToUse->DirectoryPathForClass (mlClass->UnmanagedMLClass ());
  
  return  (!dirPath.Empty ());
}  /* IncludesClass */





// Will return a list of classes that belong to this model. 
// It will be created from "classList".  So the the caller 
// can do with it as they want.
PicesClassList^  TrainingModel2::MLClasses ()
{
  if  (classList == nullptr)
    return  nullptr;

  return gcnew PicesClassList (classList);
}



void  TrainingModel2::LoadExistingModelOtherwiseBuild (PicesMsgQueue^  msgQueue)
{
  if  ((runLog != NULL)  &&  (msgQueue != nullptr))
    runLog->AttachMsgQueue (msgQueue->MsgQueue ());

  LoadExistingTrainedModel ();
  if  ((!trainer)  &&  (!(*cancelFlag)))
    LoadTrainigLibrary (false);

  if  ((!(*cancelFlag))  &&  (*valid))
  {
    delete  classes;  classes = NULL;
    if  (trainer->MLClasses ())
      classes = new MLClassConstList (*(trainer->MLClasses ()));
    else
      throw gcnew Exception ("TrainingModel2::LoadExistingModelOtherwiseBuild     (trainer->MLClasses() == NULL)");

    PopulateCSharpClassList ();
  }

  UpdateMemoryPressure ();
} /* LoadExistingModelOtherwiseBuild */




void  TrainingModel2::LoadTrainingModelForGivenLevel (uint            level,
                                                      PicesMsgQueue^  msgQueue
                                                     )
{
  GC::Collect ();

  int x = 0;

  InstrumentDataFileManager::InitializePush ();

  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (*runLog);

  KKU::KKStr  configFileName = PicesKKStr::SystemStringToKKStr (modelName);

  *cancelFlag = false;

  if  ((msgQueue != nullptr)  &&  (runLog != NULL))
    runLog->AttachMsgQueue (msgQueue->MsgQueue ());

  try
  {
    trainerWeOwn = true;
    trainer = new TrainingProcess2 (configFileName,
                                    NULL,
                                    fd,
                                    *runLog,
                                    level,
                                    *cancelFlag,
                                    *statusMsg
                                   );
  }
  catch (System::AccessViolationException^ z)
  {
    (*runLog).Level (-1) << PicesKKStr::SystemStringToKKStr (z->ToString ()) << endl;
    delete  trainer;  trainer = NULL;
    delete  classes;  classes = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    return;
  }

  if  (trainer->Abort ())
  {
    *valid = false;
    delete  trainer;    trainer = NULL;
  }
  else
  {
    *valid = true;
    delete  classes;  classes = NULL;

    if  (trainer->MLClasses ())
      classes = new MLClassConstList (*(trainer->MLClasses ()));
    else
      throw gcnew Exception ("TrainingModel2::LoadTrainingModelForGivenLevel     (trainer->MLClasses() == NULL)");

    classifier = new Classifier2 (trainer, *runLog);
    PopulateCSharpClassList ();
  }

  InstrumentDataFileManager::InitializePop ();
  UpdateMemoryPressure ();
}  /* LoadTrainingModelForGivenLevel*/




void  TrainingModel2::LoadExistingTrainedModel ()

{
  int x = 0;

  GC::Collect ();

  InstrumentDataFileManager::InitializePush ();

  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (*runLog);

  KKU::KKStr  configFileName = PicesKKStr::SystemStringToKKStr (modelName);

  *cancelFlag = false;

 

  try
  {
    trainerWeOwn = true;
    trainer = new TrainingProcess2 (configFileName, 
                                    fd, 
                                    *runLog,
                                    false,         // false = Features are not Already Normalized
                                    *cancelFlag, 
                                    *statusMsg
                                   );
  }
  catch (System::AccessViolationException^ z)
  {
    (*runLog).Level (-1) << PicesKKStr::SystemStringToKKStr (z->ToString ()) << endl;
    delete  trainer;  trainer = NULL;
    delete  classes;  classes = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    UpdateMemoryPressure ();
    return;
  }

  if  (CancelFlag)
  {
    delete  trainer;  trainer = NULL;
    delete  classes;  classes = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    UpdateMemoryPressure ();
    return;
  }

  if  (trainer->Abort ())
  {
    delete  trainer;  trainer = NULL;
    delete  classes;  classes = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    UpdateMemoryPressure ();
    return;
  }
  else
  {
    *valid = true;
    delete  classes;  classes = NULL;

    if  (trainer->MLClasses ())
      classes = new MLClassConstList (*(trainer->MLClasses ()));
    else
      throw gcnew Exception ("LoadExistingTrainedModel    (trainer->MLClasses() == NULL)");

    classifier = new Classifier2 (trainer, *runLog);
    PopulateCSharpClassList ();
  }

  InstrumentDataFileManager::InitializePop ();

  UpdateMemoryPressure ();

  GC::Collect ();
}  /* LoadExistingTrainedModel */



void  TrainingModel2::LoadTrainigLibrary (bool  forceRebuild)
{
  ErrorMsgsClear ();

  InstrumentDataFileManager::InitializePush ();

  int x = 0;

  GC::Collect ();

  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (*runLog);

  KKU::KKStr  configFileName = PicesKKStr::SystemStringToKKStr (modelName);

  trainerWeOwn = true;

  *cancelFlag = false;

  try
  {
    /**@todo There is a memory corruption when th eprocess is canecelled. */
    trainer = new TrainingProcess2 (configFileName, 
                                    NULL,              // Exclude List
                                    fd, 
                                    *runLog,
                                    NULL,              // report file stream
                                    forceRebuild,
                                    true,             // false = don't check for duplicates.
                                    *cancelFlag, 
                                    *statusMsg
                                   );
  }
  catch (System::AccessViolationException^ z)
  {
    (*runLog) << PicesKKStr::SystemStringToKKStr (z->ToString ()) << endl;

    if  (trainer)
    {
      try
      {
        ErrorMsgsAdd (trainer->ConfigFileFormatErrors ());
      }
      catch  (Exception^)  
      {
        (*runLog) << "TrainingModel2::LoadTrainigLibrary   ***ERROR***  Exception calling 'ErrorMsgsAdd'." << endl;
      }
    }

    delete  trainer;  trainer = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    UpdateMemoryPressure ();
    return;
  }
  catch  (System::Exception^  z2)
  {
    (*runLog) << PicesKKStr::SystemStringToKKStr (z2->ToString ()) << endl;
    if  (trainer)
    {
      try{ErrorMsgsAdd (trainer->ConfigFileFormatErrors ());}
      catch  (Exception^)  
      {
      (*runLog) << "TrainingModel2::LoadTrainigLibrary   ***ERROR***  Exception calling 'ErrorMsgsAdd'." << endl;
      }
    }
    delete  trainer;  trainer = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    UpdateMemoryPressure ();
    return;
  }

  if  (*cancelFlag)
  {
    *valid = false;
    delete  trainer;
    trainer = NULL;
  }

  else if  (trainer->Abort ())
  {
    try  {ErrorMsgsAdd (trainer->ConfigFileFormatErrors ());}  
    catch  (Exception^)  
    {
      (*runLog) << "TrainingModel2::LoadTrainigLibrary   ***ERROR***  Exception calling 'ErrorMsgsAdd'." << endl;
    }
    *valid = false;
    delete  trainer;
    trainer = NULL;
  }
  else
  {
    *valid = true;
    delete  classes;  classes = NULL;
    if  (trainer->MLClasses ())
      classes = new MLClassConstList (*(trainer->MLClasses ()));
    else
      throw gcnew Exception ("TrainingModel2::LoadTrainigLibrary    (trainer->MLClasses() == NULL)");
    classifier = new Classifier2 (trainer, *runLog);
    PopulateCSharpClassList ();
  }

  InstrumentDataFileManager::InitializePop ();

  UpdateMemoryPressure ();
  GC::Collect ();
}  /* LoadTrainigLibrary */



void  TrainingModel2::BuildTrainingModel (PicesFeatureVectorList^  picesTrainingData)
{
  GC::Collect ();

  FileDescPtr fd = FeatureFileIOPices::NewPlanktonFile (*runLog);

  KKU::KKStr  configFileName = PicesKKStr::SystemStringToKKStr (modelName);
  *cancelFlag = false;

  FeatureVectorListPtr  trainingData = new FeatureVectorList (fd, false, *runLog, 1000);
  for each (PicesFeatureVector^ pfv in picesTrainingData)
    trainingData->PushOnBack (pfv->Features ());

  classes = trainingData->ExtractMLClassConstList ();
  classes->SortByName ();
  PopulateCSharpClassList ();

  try
  {
    trainerWeOwn = true;
    trainer = new TrainingProcess2 (config, 
                                    trainingData, 
                                    NULL, 
                                    fd, 
                                    *runLog,
                                    false,              // false = Features are NOT already normalized.
                                    *cancelFlag, 
                                    *statusMsg
                                   ); 
  }
  catch (System::AccessViolationException^ z)
  {
    (*runLog) << PicesKKStr::SystemStringToKKStr (z->ToString ()) << endl;
    delete  trainer;  trainer = NULL;
    *valid = false;
    InstrumentDataFileManager::InitializePop ();
    return;
  }

  if  (trainer->Abort ())
  {
    *valid = false;
    delete  trainer;    trainer = NULL;
  }
  else
  {
    trainerWeOwn = true;
    trainer->CreateModelsFromTrainingData ();

    if  (trainer->Abort ())
    {
      if  (errorMsgs != nullptr)
        ErrorMsgsAdd ("TrainingModel2::BuildTrainingModel   Error occured buiding training model.");
      *valid = false;
    }
    else
    {
      *valid = true;
      classifier = new Classifier2 (trainer, *runLog);
    }
  }

  InstrumentDataFileManager::InitializePop ();

  UpdateMemoryPressure ();
  GC::Collect ();
}  /* BuildTrainingModel */



PicesPredictionList^   TrainingModel2::BinaryProbailitiesForClass (PicesClass^  leftClass)
{
  if  (!crossProbTable)
    return  nullptr;

  int  idx = classList->LookUpIndex (leftClass);
  if  (idx < 0)
    return  nullptr;

  if  (idx >= crossProbTableNumClasses)
  {
    (*runLog).Level (-1) << "\n"
                         <<  "TrainingModel2::BinaryProbailitiesForClass   ***ERROR***" << "\n" 
                         <<  "                   idx[" << idx << "] >= crossProbTableNumClasses[" << crossProbTableNumClasses << "]" << "\n"
                         << endl;
    return  nullptr;
  }

  if  (crossProbTableNumClasses != classList->Count)
    return  nullptr;

  PicesPredictionList^  predictions = gcnew PicesPredictionList ();
  for  (int x = 0;  x < crossProbTableNumClasses;  x++)
  {
    predictions->Add (gcnew PicesPrediction (classList[x], 1, crossProbTable[idx][x]));
  }

  return  predictions;
}  /* BinaryProbailitiesForClass */




PicesPredictionList^   TrainingModel2::PredictProbabilities (PicesFeatureVector^  picesFeatureVector)
{
  ImageFeaturesPtr  unKnownExample = new ImageFeatures (*(picesFeatureVector->Features ()));
  int  numClasses = classes->QueueSize ();

  try
  {
    classifier->ProbabilitiesByClass (*classes, unKnownExample, votes, probabilities);
  }
  catch  (KKStrException& e2)
  {
    KKStr  errMsg = "TrainingModel2::PredictProbabilities   KKStrException occured calling 'ProbabilitiesByClass'.\n\n" + e2.ToString ();
    System::Windows::Forms::MessageBox::Show (PicesKKStr::KKStrToSystenStr (errMsg), "TrainingModel2::PredictProbabilities");
    delete  unKnownExample;  unKnownExample = NULL;
    return nullptr;
  }
  catch  (exception& e)
  {
    KKStr  errMsg = "TrainingModel2::PredictProbabilities   std::exception occured calling 'ProbabilitiesByClass'.\n\n";
    errMsg << e.what ();
    System::Windows::Forms::MessageBox::Show (PicesKKStr::KKStrToSystenStr (errMsg), "TrainingModel2::PredictProbabilities");
    delete  unKnownExample;  unKnownExample = NULL;
    return nullptr;
  }
  catch  (...)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured calling 'ProbabilitiesByClass'."
                                              "TrainingModel2::PredictProbabilities"
                                             );
    delete  unKnownExample;  unKnownExample = NULL;
    return nullptr;
  }

  delete  unKnownExample;  unKnownExample = NULL;
   

  try
  {
    classifier->RetrieveCrossProbTable (*classes, crossProbTable);
  }
  catch  (...)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured calling 'RetrieveCrossProbTable'."
                                              "TrainingModel2::PredictProbabilities"
                                             );
    return nullptr;
  }

  PicesPredictionList^  predictions = gcnew PicesPredictionList (numClasses);
  for  (int idx = 0;  idx < numClasses;  idx++)
    predictions->Add (gcnew PicesPrediction (classList[idx], votes[idx], probabilities[idx]));

  if  (classifier->SelectionMethod () == SelectByVoting)
     predictions->SortByVotingHighToLow ();
  else
     predictions->SortByProbabilityHighToLow ();

  return  predictions;
}  /* PredictProbabilities */






PicesPrediction^  TrainingModel2::PredictClass (PicesFeatureVector^  example)
{
  FeatureVector  testExample (*(example->Features ()));  // We need a local copy; because the ClassifyImage function is going 
                                                         // to normalize the data.

  MLClassConstPtr  class1Pred     = NULL;
  MLClassConstPtr  class2Pred     = NULL;
  int                 class1Votes    = -1;
  int                 class2Votes    = -1;
  double              class1Prob     = -1.0f;
  double              class2Prob     = -1.0f;
  double              knownClassProb = -1.0f;
  int                 numOfWinners   = -1;
  double              breakTie       = -1.0f;
  double              compact        = -1.0;
  try
  {
    classifier->ClassifyAImage (testExample, 
                                class1Pred, 
                                class2Pred,
                                class1Votes,
                                class2Votes,
                                knownClassProb,
                                class1Prob,
                                class2Prob, 
                                numOfWinners,
                                breakTie,
                                compact
                               );
  }
  catch  (Exception^ e)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured when calling 'Classifyer::ClassifyAImage'  in  'TrainingModel2::PredictClass'" + "\n\n" +
                                              e->ToString (),
                                              "TrainingModel2::PredictClass"
                                             );
    class1Pred = nullptr;
  }

  PicesClass^  predictedClass = classList->LookUpByUnmanagedClass (class1Pred);
  if  (predictedClass == nullptr)
  {
    predictedClass = PicesClassList::GetUniqueClass (class1Pred);
    classList->Add (predictedClass);
  }

  PicesPrediction^  prediction = gcnew PicesPrediction (predictedClass, class1Votes, class1Prob);

  return  prediction;
}  /* PredictClass */




void  TrainingModel2::PredictClass (PicesFeatureVector^  featureVector,
                                    PicesPrediction^     prediction1,
                                    PicesPrediction^     prediction2
                                   )
{
  if  (!classifier)
    return;

  MLClassConstPtr  class1Pred     = NULL;
  MLClassConstPtr  class2Pred     = NULL;
  int                 class1Votes    = -1;
  int                 class2Votes    = -1;
  double              class1Prob     = -1.0f;
  double              class2Prob     = -1.0f;
  double              knownClassProb = -1.0f;
  int                 numOfWinners   = -1;
  double              breakTie       = -1.0f;
  double              compact        = -1.0;
  try
  {
    classifier->ClassifyAImage (*(featureVector->UnManagedClass ()),
                                class1Pred, 
                                class2Pred,
                                class1Votes,
                                class2Votes,
                                knownClassProb,
                                class1Prob,
                                class2Prob, 
                                numOfWinners,
                                breakTie,
                                compact
                               );
  }
  catch  (Exception^ e)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured when calling 'Classifyer::ClassifyAImage'  in  'TrainingModel2::PredictClass'" + "\n\n" +
                                              e->ToString (),
                                              "TrainingModel2::PredictClass"
                                             );
    class1Pred = nullptr;
  }


  if  (class1Pred != nullptr)
  {
    prediction1->MLClass  = PicesClassList::GetUniqueClass (class1Pred);
    prediction1->Probability = class1Prob;
    prediction1->Votes       = class1Votes;
  }
  
  if  (class2Pred != nullptr)
  { 
    prediction2->MLClass  = PicesClassList::GetUniqueClass (class2Pred);
    prediction2->Probability = class2Prob;
    prediction2->Votes       = class2Votes;
  }

  return;
}   /* PredictClass */




void  TrainingModel2::PredictClass (System::String^   imageFileName,
                                   System::Array^    raster,
                                   System::Single^   depth,
                                   System::Single^   salinity, 
                                   System::Single^   oxygen, 
                                   System::Single^   florences,
                                   PicesPrediction^  prediction1,
                                   PicesPrediction^  prediction2
                                  )
{
  if  (!classifier)
    return;

  PicesFeatureVector^  fv = gcnew PicesFeatureVector (raster, imageFileName, picesRunLog);
  fv->AddInstrumentData (depth, salinity, oxygen, florences);


  PredictClass (fv, prediction1, prediction2);

  return;
}  /* PredictClass */





void   TrainingModel2::ProbabilitiesByClassDual (PicesFeatureVector^    picesFeatureVector,
                                                 String^%               classifier1Desc,
                                                 String^%               classifier2Desc,
                                                 PicesPredictionList^%  classifier1Results,
                                                 PicesPredictionList^%  classifier2Results
                                                )
{
  classifier1Results = nullptr;
  classifier2Results = nullptr;

  if  (!classifier)
    return;

  ClassProbListPtr  c1Results = NULL;
  ClassProbListPtr  c2Results = NULL;

  KKStr  class1Desc;
  KKStr  class2Desc;

  classifier->ProbabilitiesByClassDual (picesFeatureVector->UnManagedClass (), class1Desc, class2Desc, c1Results, c2Results);
  classifier1Desc = PicesKKStr::KKStrToSystenStr (class1Desc);
  classifier2Desc = PicesKKStr::KKStrToSystenStr (class2Desc);

  if  (c1Results)
    classifier1Results = gcnew PicesPredictionList (*c1Results);

  if  (c2Results)
    classifier2Results = gcnew PicesPredictionList (*c2Results);

  delete  c1Results;  c1Results = NULL;
  delete  c2Results;  c2Results = NULL;

}  /* ProbabilitiesByClassDual */




PicesPredictionList^   TrainingModel2::PredictProbabilities (System::String^   imageFileName,
                                                             PicesRasterList^  saveCalcImages
                                                            )
{
  bool  sucessful = false;

  RasterSipperListPtr  unManagedIntermediateImages = NULL;
  if  (saveCalcImages != nullptr)
    unManagedIntermediateImages = new RasterSipperList (true);
  
  KKStr  fn = PicesKKStr::SystemStringToKKStr (imageFileName);

  MLClassConstPtr  unKnownClass = classes->IdxToPtr (0);

  ImageFeaturesPtr  fv = NULL;
  try
  {
    fv = new ImageFeatures (fn, unKnownClass, sucessful, unManagedIntermediateImages);
  }
  catch  (Exception^ e)
  {
    fv = NULL;
    sucessful = false;
    System::Windows::Forms::MessageBox::Show ("Exception occured when Constructing a 'ImageFeatures' object" + "\n\n" +
                                              e->ToString (),
                                              "TrainingModel2::PredictProbabilities"
                                             );
  }
  catch  (...)
  {
    fv = NULL;
    sucessful = false;
    System::Windows::Forms::MessageBox::Show ("Exception occured when Constructing a 'ImageFeatures' object\n\nTrainingModel2::PredictProbabilities");
  }

  if  (!sucessful)
  {
    delete  unManagedIntermediateImages;  unManagedIntermediateImages = NULL;
    delete  fv;                           fv = NULL;
    return  nullptr;
  }

  if  (unManagedIntermediateImages)
  {
    saveCalcImages->AddList (unManagedIntermediateImages);
    unManagedIntermediateImages = NULL;
  }

  PicesFeatureVector^  pfv = gcnew PicesFeatureVector (fv);  // 'pfv' will take ownership of 'fv'

  bool  cancelFlag = false;

  InstrumentDataPtr  id = NULL;
  id = InstrumentDataFileManager::GetClosestInstrumentData (fn, cancelFlag, *runLog);
  if  (id)
  {
    // We do not own the Instrument data; and 'PicesInstrumentData'  wants to take ownership
    // so we need to create a duplicate.
    id = new InstrumentData (*id);
    PicesInstrumentData^ pid = gcnew PicesInstrumentData (id);
    pfv->AddInstrumentData (pid);
  }
  PicesPredictionList^  predictions = nullptr;
  try
  {
    predictions = PredictProbabilities (pfv);
  }
  catch (Exception^ e)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured calling 'PredictProbabilities'" + "\n\n" +
                                              e->ToString (),
                                              "TrainingModel2::PredictProbabilities"
                                             );
    predictions = nullptr;
  }
  catch  (...)
  {
    System::Windows::Forms::MessageBox::Show ("Exception occured calling 'PredictProbabilities'."
                                              "TrainingModel2::PredictProbabilities"
                                             );
    predictions = nullptr;
  }

  delete  pfv;
  pfv = nullptr;

  return  predictions;
}  /* PredictProbabilities */



array<String^>^  TrainingModel2::SupportVectorNames (PicesClass^ c1,
                                                     PicesClass^ c2
                                                    )
{
  if  (!classifier)
    return nullptr;

  MLClassPtr  c1Pices = c1->UnmanagedMLClass ();
  MLClassPtr  c2Pices = c2->UnmanagedMLClass ();

  vector<KKStr> fvNames = classifier->SupportVectorNames (c1Pices, c2Pices);

  array<String^>^  results = gcnew array<String^> (fvNames.size ());
  for  (uint zed = 0;  zed < fvNames.size ();  zed++)
    results[zed] = PicesKKStr::KKStrToSystenStr (fvNames[zed]);
    
  return  results;
}



array<PicesInterface::ProbNamePair^>^   
                     TrainingModel2::FindWorstSupportVectors (PicesFeatureVector^  picesFeatureVector,
                                                              int                  numToFind,
                                                              PicesClass^          c1,
                                                              PicesClass^          c2
                                                             )
{
  if  (!classifier)
    return nullptr;

  MLClassPtr  c1Pices = c1->UnmanagedMLClass ();
  MLClassPtr  c2Pices = c2->UnmanagedMLClass ();

  // Will make duplicate of featuire vector Because the Classifier will normalize the data.
  FeatureVectorPtr  dupFV = new FeatureVector (*picesFeatureVector->UnManagedClass ());

  vector<MLL::ProbNamePair> worstExamples 
    = classifier->FindWorstSupportVectors (dupFV, 
                                           numToFind, 
                                           c1Pices, 
                                           c2Pices
                                          );

  array<PicesInterface::ProbNamePair^>^  results = gcnew array<PicesInterface::ProbNamePair^> (worstExamples.size ());
  for  (uint zed = 0;  zed < worstExamples.size ();  zed++)
    results[zed] = gcnew PicesInterface::ProbNamePair (worstExamples[zed].name, worstExamples[zed].probability);

  delete  dupFV;  dupFV = NULL;

  return  results;
}  /* FindWorstSupportVectors */





array<PicesInterface::ProbNamePair^>^   
                     TrainingModel2::FindWorstSupportVectors2 (PicesFeatureVector^  picesFeatureVector,
                                                               int                  numToFind,
                                                               PicesClass^          c1,
                                                               PicesClass^          c2
                                                              )
{
  if  (!classifier)
    return nullptr;

  MLClassPtr  c1Pices = c1->UnmanagedMLClass ();
  MLClassPtr  c2Pices = c2->UnmanagedMLClass ();

  // Will make duplicate of featuire vector Because the Classifier will normalize the data.
  FeatureVectorPtr  dupFV = new FeatureVector (*picesFeatureVector->UnManagedClass ());

  vector<MLL::ProbNamePair> worstExamples 
    = classifier->FindWorstSupportVectors2 (dupFV, 
                                            numToFind, 
                                            c1Pices, 
                                            c2Pices
                                           );

  array<PicesInterface::ProbNamePair^>^  results = gcnew array<PicesInterface::ProbNamePair^> (worstExamples.size ());
  for  (uint zed = 0;  zed < worstExamples.size ();  zed++)
    results[zed] = gcnew PicesInterface::ProbNamePair (worstExamples[zed].name, worstExamples[zed].probability);

  delete  dupFV;  dupFV = NULL;

  return  results;
}  /* FindWorstSupportVectors2 */




Bitmap^  TrainingModel2::BuildBitmapFromRasterData (uchar**  r,
                                                   int      height,
                                                   int      width
                                                  )
{
  int  rowFirst = 99999;
  int  rowLast  = -1;
  int  colFirst = 99999;
  int  colLast  = -1;

  Bitmap^  image = gcnew Bitmap (width, height);
    

  // GDI+ return format is BGR, NOT RGB. 
  BitmapData^ bmData = image->LockBits (System::Drawing::Rectangle (0, 0, width, height),
                                        ImageLockMode::ReadWrite,
                                        PixelFormat::Format24bppRgb
                                       );
  int stride = bmData->Stride;
  System::IntPtr Scan0 = bmData->Scan0;

  {
    int  nOffset = stride - width * 3;
    int  bytesPerRow = width * 3 + nOffset;
    int  col = 0;
    int  row = 0;
    byte pixelValue = 0;
    byte lastPixelValue = 0;


    if  (colorValues == nullptr)
    {
      colorValues = gcnew array<Pen^>(256);
      for (int x = 0; x < 256; x++)
      {
        int y = 255 - x;
        colorValues[x] = gcnew Pen (Color::FromArgb(y, y, y));
      }
    }

    Pen^ curPen = colorValues[pixelValue];
    byte  red   = curPen->Color.R;
    byte  green = curPen->Color.G;
    byte  blue  = curPen->Color.B;

    byte* ptr = (byte*)(void*)Scan0;

    for  (row = 0;  row < height;  row++)
    {
      uchar*  datRow = r[row];
          
      for (col = 0;  col < width;  col++)
      {
        pixelValue = datRow[col];
        if  (pixelValue != lastPixelValue)
        {
          lastPixelValue = pixelValue;
          curPen = colorValues[lastPixelValue];
          red   = curPen->Color.R;
          green = curPen->Color.G;
          blue  = curPen->Color.B;
        }

        ptr[0] = blue;  ptr++;
        ptr[0] = green; ptr++;
        ptr[0] = red;   ptr++;

        if  (pixelValue < 255)
        {
          // We are looking at a forground pixel.
          if  (row < rowFirst )
            rowFirst = row;
          rowLast = row;

          if  (col < colFirst)
            colFirst = col;

          if  (col > colLast)
            colLast = col;
        }
      }
          
      ptr += nOffset;
    }
  }  

  image->UnlockBits (bmData);
  return  image;
}  /* BuildBitmapFromRasterData */







PicesPredictionList^   TrainingModel2::PredictProbabilities (System::String^  imageFileName,
                                                            System::Array^   raster,
                                                            System::Single^  depth,
                                                            System::Single^  salinity, 
                                                            System::Single^  oxygen, 
                                                            System::Single^  florences
                                                           )
{
  if  (!classifier)
    return nullptr;

  PicesFeatureVector^  fv = gcnew PicesFeatureVector (raster, imageFileName, picesRunLog);
  fv->AddInstrumentData (depth, salinity, oxygen, florences);
  PicesPredictionList^  predictions = this->PredictProbabilities (fv);
  delete  fv;  fv = nullptr;
  return  predictions;
}  /* PredictProbabilities */




String^   TrainingModel2::RunLogFileName::get ()
{
  if  (runLog != NULL)
    return  PicesKKStr::KKStrToSystenStr (runLog->FileName ());
  else
    return  String::Empty;
}  /* RunLogFileName */






void  TrainingModel2::CancelLoad ()   // Sets cancel flag to terminate loading of training model.
{
  *cancelFlag = true;
}



String^  TrainingModel2::DirectoryPathForClass (PicesClass^  mlClass)
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (configToUse == NULL)
    return nullptr;

  KKStr  dirPath = configToUse->DirectoryPathForClass (mlClass->UnmanagedMLClass ());
  return  PicesKKStr::KKStrToSystenStr (dirPath);
}  /* DirectoryPathForClass */




array<String^>^  TrainingModel2::GetListOfTrainingModels ()
{
  KKStr searchSpec = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + "*.cfg";
  KKStrListPtr   fileNames = osGetListOfFiles (searchSpec);
  if  (fileNames == NULL)
    return  nullptr;

  int  count = fileNames->QueueSize ();  
  if  (count < 1)
  {
    delete  fileNames;
    return nullptr;
  }

  cli::array<String^>^  configFiles 
    = gcnew cli::array<String^> (count);

  KKStrList::iterator  idx;
  int x;
  for  (idx = fileNames->begin (), x = 0;  idx != fileNames->end ();  idx++, x++)
  {
    KKStrPtr s = *idx;
    configFiles[x] = gcnew String (s->Str ());
  }

  delete  fileNames;
  fileNames = NULL;

  return  configFiles;
}  /* GetListOfTrainingModels */






uint  TrainingModel2::NumHierarchialLevels::get ()
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (configToUse)
    return  configToUse->NumHierarchialLevels ();
  else
    return 0;
}




int  TrainingModel2::ImagesPerClass::get ()
{
  TrainingConfiguration2Ptr tConfig = trainer->Config ();
  if  (tConfig)
    return  tConfig->ImagesPerClass ();
  else
    return -1;
}



String^  TrainingModel2::ParameterStr::get ()
{
  KKStr  parmStr;

  if  (trainer)
  {
    ModelParamPtr  parameters = trainer->Parameters ();
    if  (parameters)
      parmStr = parameters->ToCmdLineStr ();

    else
    {
      TrainingConfiguration2Ptr tConfig = trainer->Config ();
      if  (tConfig)
      {
        ModelParamPtr  parameters = tConfig->ModelParameters ();
        if  (parameters)
          parmStr = parameters->ToCmdLineStr ();
      }
    }
  }

  if  (parmStr.Empty ())
  {
    TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
    if  ((configToUse)  &&  (configToUse->ModelParameters ()))
      parmStr = configToUse->ModelParameters ()->ToCmdLineStr ();
  }

  return  PicesKKStr::KKStrToSystenStr (parmStr);
}  /* SvmParamStr::get () */




String^  TrainingModel2::ConfigFileName::get ()
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (configToUse)
    return PicesKKStr::KKStrToSystenStr (configToUse->FileName ());
  else
    return String::Empty;
}



void  TrainingModel2::AddClass (PicesClass^  newClass)
{
  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (configToUse == NULL)
    throw gcnew Exception ("Could not retrieve the configuration file.");

  MLClassPtr  c = newClass->UnmanagedMLClass ();

  KKStr  dirPath =  (configToUse->DirectoryPathForClass (c));
  if  (!dirPath.Empty ())
    throw gcnew Exception ("Class already in Training Model.");

  // kak 2010-05-09
  // We need to make sure the directory for this class exists.
  KKStr  classDirName = osAddSlash (configToUse->RootDir ()) + c->Name ();
  osCreateDirectoryPath (classDirName);

  configToUse->AddATrainingClass (c);
}   /* AddClass */





void  TrainingModel2::AddImageToTrainingLibray (String^       imageFileName,
                                                PicesRaster^  raster, 
                                                PicesClass^   mlClass,
                                                bool          onLine       // If set to true;  will prompt user if they want to add new class if not part of Traning Model.
                                               )
{
  if  (raster == nullptr)
    throw gcnew Exception ("No raster image provided;   \"raster == nullptr;\"  in method \"TrainingModel2::AddImageToTrainingLibray\"");

  if  (mlClass == nullptr)
    throw gcnew Exception ("No class provided;   \"mlClass == nullptr;\"  in method \"TrainingModel2::AddImageToTrainingLibray\"");

  TrainingConfiguration2Ptr  configToUse = GetConfigToUse ();
  if  (!configToUse)
    throw  gcnew Exception  ("No defined Configuration file available for ModelName[" + modelName + "]");

  if  (!this->IncludesClass (mlClass))
  {
    if  (onLine)
    {
      DialogResult dr = MessageBox::Show 
                            ("Class[" + mlClass->Name + "] not part of model[" + ModelName + "]" + "\n\n" +
                             "Do you want to add this Class to training model?",
                             "Add to Training Library",
                             MessageBoxButtons::YesNo
                            );
      if  (dr != DialogResult::Yes)
        return;
      AddClass (mlClass);
      SaveConfiguration ();
    }
    else
    {
      throw gcnew Exception ("Class[" + mlClass->Name + "] Not part of training model[" + ModelName + "]");
    }
  }

  KKStr  dirPathToCopyTo = configToUse->DirectoryPathForClass (mlClass->UnmanagedMLClass ());
  if  (dirPathToCopyTo.Empty ())
    throw gcnew Exception ("No Directory Path defined for Class[" + mlClass->Name + "]  Model[" + modelName + "]");

  if  (!osValidDirectory (dirPathToCopyTo))
    throw gcnew Exception ("TrainingModel2::AddImageToTrainingLibray    Directory[" + PicesKKStr::KKStrToSystenStr (dirPathToCopyTo) + "] does not exist.");

  // make sure that target directory exists.

  KKStr  rootName = osGetRootName (PicesKKStr::SystemStringToKKStr (imageFileName));
  KKStr  destFileName = osAddSlash (dirPathToCopyTo) + rootName + ".bmp";

  if  (osFileExists (destFileName))
  {
    System::String^  errMsg = "Image[" + PicesKKStr::KKStrToSystenStr (rootName) + "] already exists in Class[" + mlClass->Name + "]";
    if  (!onLine)
    {
      throw gcnew Exception (errMsg);
    }
    else
    {
      MessageBox::Show (errMsg, "Already in Training Library", MessageBoxButtons::OK);
      return;
    }
  }

  try
  {
    SaveImageGrayscaleInverted8Bit (*(raster->UnmanagedClass ()), destFileName);
  }
  catch  (KKStr  errMsg)
  {
    throw gcnew Exception (PicesKKStr::KKStrToSystenStr (errMsg));
  }
}  /* AddImageToTrainingLibray */
 



TrainingConfiguration2Ptr  TrainingModel2::GetConfigToUse ()
{
  TrainingConfiguration2Ptr  configToUse = NULL;
  if  (config)
    configToUse = config;
  else
    configToUse = trainer->Config ();

  if  (!configToUse)
  {
    FileDescPtr  fd = FeatureFileIOPices::NewPlanktonFile (*runLog);
    config  = new TrainingConfiguration2 (fd, PicesKKStr::SystemStringToKKStr (modelName), *runLog, false);
    // Don't need to delete 'fd'  'FileDesc' instances are kept in memry and shared when identicle.
    configToUse = config;
  }
  return  configToUse;
}  /* GetConfigToUse */






void  TrainingModel2::ErrorMsgsClear ()
{
  if  (errorMsgs == nullptr)
    errorMsgs = gcnew List<String^> ();
  else
    errorMsgs->Clear ();
}


void  TrainingModel2::ErrorMsgsAdd (String^  errorMsg)
{
  if  (errorMsgs == nullptr)
    errorMsgs = gcnew List<String^> ();

  errorMsgs->Add (errorMsg);
}


void  TrainingModel2::ErrorMsgsAdd (const VectorKKStr&  _errorMsgs)
{
  VectorKKStr::const_iterator  idx;
  for  (idx = _errorMsgs.begin ();  idx != _errorMsgs.end ();  idx++)
  {
    String^  msg = PicesKKStr::KKStrToSystenStr (*idx);
    ErrorMsgsAdd (msg);
  }
}  /* ErrorMsgsAdd */


}  /* PicesInterface */
