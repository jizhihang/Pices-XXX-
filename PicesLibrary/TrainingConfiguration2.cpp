#include "FirstIncludes.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;

#include "InstrumentDataFileManager.h"
#include "SipperVariables.h"
using namespace  SipperHardware;


#include "TrainingConfiguration2.h"
#include "BinaryClassParms.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "ImageFeatures.h"
#include "Model.h"
#include "ModelParamDual.h"
#include "ModelParamKnn.h"
#include "ModelParamOldSVM.h"
#include "ModelParamSvmBase.h"
#include "ModelParamUsfCasCor.h"
#include "NormalizationParms.h"
using namespace  MLL;



void  TrainingConfiguration2::CreateModelParameters (const KKStr&           _parameterStr,
                                                     const FeatureNumList&  _selFeatures,
                                                     int32                  _sectionLineNum,
                                                     int32                  _parametersLineNum, 
                                                     int32                  _featuresIncludedLineNum
                                                    )
{
  delete  modelParameters;
  modelParameters = NULL;

  if  (_parametersLineNum < 0)
  {
    KKStr errMsg = "'Parameters' setting was not specified.";
    log.Level (-1) << endl << endl << "CreateModelParameters   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (_sectionLineNum, errMsg);
    FormatGood (false);
  }

  switch  (modelingMethod)
  {
  case  Model::mtOldSVM:    modelParameters = new ModelParamOldSVM    (fileDesc, log);
                            break;

  case  Model::mtSvmBase:   modelParameters = new ModelParamSvmBase   (fileDesc, log);
                            break;

  case  Model::mtKNN:       modelParameters = new ModelParamKnn       (fileDesc, log);
                            break;

  case  Model::mtUsfCasCor: modelParameters = new ModelParamUsfCasCor (fileDesc, log);
                            break;

  case  Model::mtDual:      modelParameters = new ModelParamDual      (fileDesc, log);
                            break;

  default:
    log.Level (-1) << endl << endl
      << "TrainingConfiguration2::CreateModelParameters  ***ERROR***   Invalid modeling method[" 
      << Model::ModelTypeToStr (modelingMethod) << "] selected at this time." << endl
      << endl;
    break;
  }

  if  (modelParameters)
  {  
    bool  validParameterFormat = false;
    // We apply '_selFeatures' first;  this way if features were specified in '_parameterStr'  they
    // will override what '_selFeatures'.  This is important because qquite often the caller does 
    // not know the features so they assume all.
    modelParameters->SelectedFeatures (_selFeatures);
    modelParameters->ParseCmdLine (_parameterStr, validParameterFormat);

    if  (!modelParameters->ValidParam ())
    {
      KKStr errMsg = "Parameter setting is invalid";
      log.Level (-1) << endl << endl << "TrainingConfiguration2::CreateModelParameters  ***ERROR***   " << errMsg << endl << endl;
      FormatErrorsAdd (_parametersLineNum, errMsg);
      FormatGood (false);
    }

    else if  (_featuresIncludedLineNum > 0)
    {
      modelParameters->SelectedFeatures (_selFeatures);
    }
  }
}  /* CreateModelParameters */






TrainingConfiguration2::TrainingConfiguration2 (FileDescPtr   _fileDesc,
                                                const KKStr&  _configFileName, 
                                                RunLog&       _log,
                                                bool          _validateDirectories
                                               ):

  Configuration (GetEffectiveConfigFileName (_configFileName), _log),

  configFileNameSpecified   (_configFileName),
  fileDesc                  (_fileDesc),
  mlClasses              (NULL),
  mlClassesWeOwnIt       (false),
  log                       (_log),
  modelingMethod            (Model::mtNULL),
  examplesPerClass          (0),
  modelParameters           (NULL),
  noiseGuaranteedSize       (0),
  noiseMLClass           (NULL),
  noiseTrainingClass        (NULL),
  normalizationParms        (NULL),
  otherClass                (NULL),
  otherClassLineNum         (-1),
  rootDir                   (),
  trainingClasses           ("", true, 10),
  validateDirectories       (_validateDirectories)

{
  if  (!fileDesc)
  {
    KKStr  errMsg = "TrainingConfiguration2    ***ERROR***   FileDesc == NULL";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (!FormatGood ())
  {
    log.Level (-1) << endl << "TrainingConfiguration2    ***ERROR***  Format of Configuration File is Invalid." << endl << endl;
    return;
  }

  mlClasses = new MLClassConstList ();
  mlClassesWeOwnIt = true;

  ValidateConfiguration ();
  
  if  (examplesPerClass < 1)
  {
    log.Level (10) << "TrainingConfiguration2 - examplesPerClass not specified.  Defaulting to 1300." << endl;
    examplesPerClass = 1300;
  }

  if  (!FormatGood ())
    log.Level (-1) << "***ERROR***, TrainingConfiguration2  Format of Configuration File is Invalid.";

  else if  (rootDir.Empty ())
    DetermineWhatTheRootDirectoryIs ();
}




TrainingConfiguration2::TrainingConfiguration2 (FileDescPtr             _fileDesc,       // Use this one if you want to
                                                MLClassConstListPtr  _mlClasses,   // create a default Configuration 
                                                KKStr                   _parameterStr,   // object.
                                                RunLog&                 _log
                                               ):
  Configuration             (_log),

  configFileNameSpecified   (""),
  fileDesc                  (_fileDesc),
  mlClasses              (NULL),
  mlClassesWeOwnIt       (false),
  log                       (_log),
  modelingMethod            (Model::mtNULL),
  examplesPerClass          (0),
  modelParameters           (NULL),
  noiseGuaranteedSize       (0),
  noiseMLClass           (NULL),
  noiseTrainingClass        (NULL),
  normalizationParms        (NULL),
  otherClass                (NULL),
  otherClassLineNum         (-1),
  trainingClasses           ("", true, 10),
  validateDirectories       (false)

{
  if  (!fileDesc)
  {
    KKStr  errMsg = "TrainingConfiguration2    ***ERROR***   FileDesc == NULL";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (_mlClasses)
    mlClasses = new MLClassConstList (*_mlClasses);
  else
    mlClasses = new MLClassConstList ();
  mlClassesWeOwnIt = true;

  {
    MLClassConstList::iterator idx;
    for  (idx = mlClasses->begin ();  idx != mlClasses->end ();  idx++)
    {
      MLClassConstPtr mlClass = *idx;
      TrainingClassPtr  tc = new TrainingClass ("", mlClass->Name (), "W", 1.0f, *mlClasses);
      trainingClasses.PushOnBack (tc);
    }
  }

  if  (modelingMethod == Model::mtNULL)
    modelingMethod = Model::mtOldSVM;

  {
    examplesPerClass = int32_max;
    FeatureNumList  selectedFeatures (fileDesc);
    selectedFeatures.SetAllFeatures ();
    CreateModelParameters (_parameterStr, selectedFeatures, 1, 1, 1);
    if  (!modelParameters  ||  (!modelParameters->ValidParam ()))
    {
      log.Level (-1) << endl << endl 
        << "TrainingConfiguration2   ***ERROR***   Invalid Parameters." << endl
        << "    Parameters[" << _parameterStr << "]" << endl
        << endl;
      FormatGood (false);
    }
  }
}


  
  
TrainingConfiguration2::TrainingConfiguration2 (FileDescPtr             _fileDesc,          // Use this one if you want to
                                                MLClassConstListPtr  _mlClasses,      // create a default Configuration 
                                                ModelParamPtr           _modelParameters,   /**< Will take ownership of this instance. */
                                                RunLog&                 _log
                                               ):
  Configuration             (_log),

  configFileNameSpecified   (""),
  fileDesc                  (_fileDesc),
  mlClasses              (NULL),
  mlClassesWeOwnIt       (false),
  log                       (_log),
  modelingMethod            (Model::mtNULL),
  examplesPerClass          (0),
  modelParameters           (_modelParameters),
  noiseGuaranteedSize       (0),
  noiseMLClass           (NULL),
  noiseTrainingClass        (NULL),
  normalizationParms        (NULL),
  otherClass                (NULL),
  otherClassLineNum         (-1),
  trainingClasses           ("", true, 10),
  validateDirectories       (false)

{
  if  (!fileDesc)
  {
    KKStr  errMsg = "TrainingConfiguration2    ***ERROR***   FileDesc == NULL";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (_mlClasses)
    mlClasses = new MLClassConstList (*_mlClasses);
  else
    mlClasses = new MLClassConstList ();
  mlClassesWeOwnIt = true;

  {
    MLClassConstList::iterator idx;
    for  (idx = mlClasses->begin ();  idx != mlClasses->end ();  idx++)
    {
      MLClassConstPtr mlClass = *idx;
      TrainingClassPtr  tc = new TrainingClass ("", mlClass->Name (), "W", 1.0f, *mlClasses);
      trainingClasses.PushOnBack (tc);
    }
  }

  switch  (_modelParameters->ModelParamType ())
  {
  case  ModelParam::mptDual:        modelingMethod =   Model::mtDual;       break;
  case  ModelParam::mptKNN:         modelingMethod =   Model::mtKNN;        break;
  case  ModelParam::mptOldSVM:      modelingMethod =   Model::mtOldSVM;     break;
  case  ModelParam::mptSvmBase:     modelingMethod =   Model::mtSvmBase;    break;
  case  ModelParam::mptUsfCasCor:   modelingMethod =   Model::mtUsfCasCor;  break;
  }

  {
    examplesPerClass = int32_max;
    FeatureNumList  selectedFeatures (fileDesc);
    selectedFeatures.SetAllFeatures ();
  }
}



  
  
  
  
  
  
TrainingConfiguration2::TrainingConfiguration2 (const TrainingConfiguration2&  tc):
  Configuration             (tc),

  configFileNameSpecified   (tc.configFileNameSpecified),
  fileDesc                  (tc.fileDesc),
  mlClasses              (NULL),
  mlClassesWeOwnIt       (false),
  log                       (tc.log),
  modelingMethod            (tc.modelingMethod),
  examplesPerClass          (tc.examplesPerClass),
  modelParameters           (NULL),
  noiseGuaranteedSize       (tc.noiseGuaranteedSize),
  noiseMLClass           (tc.noiseMLClass),
  noiseTrainingClass        (tc.noiseTrainingClass),
  normalizationParms        (NULL),
  otherClass                (tc.otherClass),
  otherClassLineNum         (tc.otherClassLineNum),
  rootDir                   (tc.rootDir),
  trainingClasses           (tc.rootDir, true, tc.trainingClasses.QueueSize ()),
  validateDirectories       (tc.validateDirectories)

{
  {
    int32  x;

    for  (x = 0;  x < tc.trainingClasses.QueueSize ();  x++)
    {
      TrainingClassPtr  trainingClass = tc.trainingClasses.IdxToPtr (x);
      trainingClasses.PushOnBack (new TrainingClass (*trainingClass));
    }
  }

  if  (tc.mlClasses)
  {
    mlClasses = new MLClassConstList (*tc.mlClasses);
    mlClassesWeOwnIt = true;
  }

  if  (tc.normalizationParms)
    normalizationParms  = new NormalizationParms (*tc.normalizationParms);

  if  (tc.modelParameters)
    modelParameters  = tc.modelParameters->Duplicate ();
}




ModelParamOldSVMPtr    TrainingConfiguration2::OldSvmParameters ()  const
{
  if  (modelParameters  &&  (modelingMethod == Model::mtOldSVM))
    return  dynamic_cast<ModelParamOldSVMPtr>(modelParameters);
  else
    return NULL;
}




KKStr  TrainingConfiguration2::DirectoryPathForClass (MLClassConstPtr  mlClass)  const
{
  TrainingClassList::const_iterator  idx;
  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    const TrainingClassPtr  tc = *idx;
    if  (tc->MLClass () == mlClass)
      return  tc->ExpandedDirectory (rootDir);
  }

  return  KKStr::EmptyStr ();
}  /* DirectoryPathForClass */



uint32  TrainingConfiguration2::NumHierarchialLevels ()  const
{
  TrainingClassList::const_iterator  idx;

  uint16  numHierarchialLevels = 0;

  for  (idx = trainingClasses.begin ();  idx !=  trainingClasses.end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;
    numHierarchialLevels = Max (numHierarchialLevels, tc->MLClass ()->NumHierarchialLevels ());
  }

  return  numHierarchialLevels;
}  /* NumHierarchialLevels */



void  TrainingConfiguration2::SyncronizeMLClassListWithTrainingClassList ()
{
  if  (mlClassesWeOwnIt)
  {
    delete  mlClasses;
    mlClasses = NULL;
  }

  mlClasses = new MLClassConstList ();
  mlClassesWeOwnIt = true;
  TrainingClassList::const_iterator  idx;

  for  (idx = trainingClasses.begin ();  idx !=  trainingClasses.end ();  idx++)
  {
    MLClassConstPtr  ic = (*idx)->MLClass ();
    if  (mlClasses->PtrToIdx (ic) < 0)
      mlClasses->AddMLClass ((MLClassPtr)ic);
  }
}  /* SyncronizeMLClassListWithTrainingClassList */





MLClassConstListPtr   TrainingConfiguration2::ExtractListOfClassesForAGivenHierarchialLevel (uint32 level)   const
{
  MLClassConstListPtr  classes = new MLClassConstList ();
  TrainingClassList::const_iterator  idx;

  for  (idx = trainingClasses.begin ();  idx !=  trainingClasses.end ();  idx++)
  {
    MLClassConstPtr  ic = (*idx)->MLClass ();
    MLClassConstPtr  claassForGivenLevel = ic->MLClassForGivenHierarchialLevel (level);
    if  (classes->PtrToIdx (claassForGivenLevel) < 0)
      classes->AddMLClass (claassForGivenLevel);
  }

  return  classes;
}  /* ExtractListOfClassesForAGivenHierarchialLevel */




TrainingConfiguration2Ptr  TrainingConfiguration2::GenerateAConfiguraionForAHierarchialLevel (uint32 level)
{
  log.Level (10) << "TrainingConfiguration2::GenerateAConfiguraionForAHierarchialLevel  level[" << level << "]" << endl;
  TrainingConfiguration2Ptr  hierarchialConfig = new TrainingConfiguration2 (*this);
  hierarchialConfig->EmptyTrainingClasses ();

  MLClassConstListPtr  hierarchialClassList = ExtractListOfClassesForAGivenHierarchialLevel (level);

  TrainingClassList::const_iterator  idx;
  
  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    const TrainingClassPtr tc = *idx;
    MLClassConstPtr  ic = tc->MLClass ()->MLClassForGivenHierarchialLevel (level);
    hierarchialConfig->AddATrainingClass (new TrainingClass (tc->Directory (), ic->Name (), "W", 1.0f, *hierarchialClassList));
  }

  hierarchialConfig->SyncronizeMLClassListWithTrainingClassList ();

  delete  hierarchialClassList;
  return  hierarchialConfig;
} /* GenerateAConfiguraionForAHierarchialLevel */




void  TrainingConfiguration2::Save (const KKStr& fileName)
{
  ofstream  o (fileName.Str ());

  {
    // Global Section
    o << "[Global]" << endl
      << "MODELING_METHOD = " << ModelTypeToStr (modelingMethod)   << endl
      << "ROOT_DIR = "        << rootDir                           << endl
      << "Parameters="        << modelParameters->ToCmdLineStr ()  << endl;

    if  (examplesPerClass > 0)
      o << "Examples_Per_Class=" << examplesPerClass << endl;

    o << "Features_Included=" << modelParameters->SelectedFeatures ().ToString () << endl;

    if  (otherClass != NULL)
      o << "OtherClass=" << otherClass->Name () << endl;
    o << endl;
  }


  if  (noiseTrainingClass)
  {
    o << "[NOISE_IMAGES]" << endl;

    if (noiseGuaranteedSize > 0)
      o << "GUARANTEED_SIZE=" << noiseGuaranteedSize << endl;

    o << "Dir="        << noiseTrainingClass->Directory () << endl;
    o << "Class_Name=" << noiseTrainingClass->Name ()      << endl;
    o << endl;
  }


  TrainingClassList::iterator tcIDX;
  for  (tcIDX = trainingClasses.begin ();  tcIDX !=  trainingClasses.end ();  tcIDX++)
  {
    TrainingClassPtr  tc = *tcIDX;

    o << "[TRAINING_CLASS]" << endl;

    const KKStr&  trainClassDir = tc->Directory ();
    if  (!trainClassDir.StartsWith (rootDir))
    {
      KKStr  dirStr = tc->Directory ();
      dirStr.TrimLeft ();
      dirStr.TrimRight ();
      if  (!dirStr.Empty ())
        o << "Dir=" << dirStr  << endl;
    }
    o << "Class_Name=" << tc->Name () << endl;
    if  (tc->Weight () != 1.0f)
      o << "Weight=" << tc->Weight () << endl;
    o << endl;
  }

  ModelParamOldSVMPtr  oldSVMParams = OldSvmParameters ();
  if  (oldSVMParams)
  {
    const BinaryClassParmsListPtr  binaryParms = oldSVMParams->BinaryParmsList ();
    if  (binaryParms)
    {
      BinaryClassParmsList::const_iterator  idx;

      for  (idx = binaryParms->begin ();  idx != binaryParms->end ();  idx++)
      {
        const BinaryClassParmsPtr  binaryClass = *idx;
        o << "[TWO_CLASS_PARAMETERS]" << endl;
        o << "CLASS1="            << binaryClass->Class1Name ()            << endl;
        o << "CLASS2="            << binaryClass->Class2Name ()            << endl;
        o << "Parameters="        << binaryClass->Param ().ToCmdLineStr () << endl;
        o << "Features_Included=" << binaryClass->SelectedFeatures ()      << endl;
        o << "Weight="            << binaryClass->Weight ()                << endl;
        o << endl;
      }
    }

    o << endl;
  }

  o.close ();
}  /* Save */




TrainingConfiguration2::~TrainingConfiguration2 ()
{
  delete  noiseTrainingClass;  noiseTrainingClass = NULL;
  delete  normalizationParms;  normalizationParms = NULL;
  delete  modelParameters;     modelParameters    = NULL;

  if  (mlClassesWeOwnIt)
  {
    delete  mlClasses;
    mlClasses = NULL;
  }
}



void   TrainingConfiguration2::RootDir (const KKStr& _rootDir)
{
  rootDir = _rootDir;
  trainingClasses.RootDir (rootDir);
}



void   TrainingConfiguration2::ModelParameters (ModelParamPtr  _modelParameters)   
{
  delete  modelParameters;
  modelParameters = _modelParameters->Duplicate ();
}



KKStr  TrainingConfiguration2::ModelParameterCmdLine ()  const
{
  ModelParamPtr  p = ModelParameters ();
  if  (!p)
    return "";
  else
    return p->ToCmdLineStr ();
}  /* ModelParameterCmdLine */




TrainingConfiguration2Ptr  TrainingConfiguration2::CreateFromFeatureVectorList
                                                    (FeatureVectorList&  _examples,
                                                     const KKStr&        _parameterStr, 
                                                     RunLog&             _log
                                                    )
{
  _log.Level (10) << "TrainingConfiguration2::CreateFromFeatureVectorList" << endl;

  KKStr  parameterStr = _parameterStr;
  if  (parameterStr.Empty ())
    parameterStr = "-m 200 -s 0 -n 0.11 -t 2 -g 0.024717  -c 10  -u 100  -up  -mt OneVsOne  -sm P";
    
  FileDescPtr  fileDesc = _examples.FileDesc ();

  MLClassConstListPtr  mlClasses = _examples.ExtractMLClassConstList ();
  mlClasses->SortByName ();

  TrainingConfiguration2Ptr  config 
      = new TrainingConfiguration2 (fileDesc,
                                    mlClasses, 
                                    parameterStr, 
                                    _log
                                   );

  config->SetFeatureNums (_examples.AllFeatures ());

  delete  mlClasses;  mlClasses = NULL;

  return  config;
}  /* CreateFromFeatureVectorList */



MLClassConstListPtr   TrainingConfiguration2::ExtractClassList ()  const
{
  TrainingClassList::const_iterator  idx;

  MLClassConstListPtr  classes = new MLClassConstList ();

  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;

    if  (classes->PtrToIdx (tc->MLClass ()) < 0)
      classes->PushOnBack (tc->MLClass ());
  }

  classes->SortByName ();

  return  classes;
}   /* ExtractClassList */




TrainingConfiguration2Ptr  TrainingConfiguration2::CreateFromDirectoryStructure 
                                                    (FileDescPtr   _fileDesc,
                                                     const KKStr&  _existingConfigFileName,
                                                     const KKStr&  _subDir,
                                                     RunLog&       _log,
                                                     bool&         _successful,
                                                     KKStr&        _errorMessage
                                                    )
{
  _log.Level (10) << "TrainingConfiguration2::CreateFromDirectoryStructure"  << endl
                  << "                       Feature Data from SubDir[" << _subDir << "]." << endl
                  << endl;

  _successful = true;

  if  (_fileDesc == NULL)
    _fileDesc = FeatureFileIOPices::NewPlanktonFile (_log);

  KKStr  directoryConfigFileName = osGetRootNameOfDirectory (_subDir);
  if  (directoryConfigFileName.Empty ())
    directoryConfigFileName = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + "Root.cfg";
  else
    directoryConfigFileName = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + directoryConfigFileName + ".cfg";

  TrainingConfiguration2Ptr  config = NULL;


  if  (!_existingConfigFileName.Empty ())
  {
    if  (osFileExists (_existingConfigFileName))
    {
      config = new TrainingConfiguration2 (_fileDesc, 
                                           _existingConfigFileName, 
                                           _log, 
                                           false
                                          );
      config->RootDir (_subDir);
      if  (!(config->FormatGood ()))
      {
        delete  config;
        config = NULL;
      }
    }
  }


  if  (!config)
  {
    if  (osFileExists (directoryConfigFileName))
    {
      config = new TrainingConfiguration2 (_fileDesc,
                                           directoryConfigFileName, 
                                           _log, 
                                           false
                                          );
      config->RootDir (_subDir);
      if  (!(config->FormatGood ()))
      {
        delete  config;
        config = NULL;
      }
    }
  }

  if  (!config)
  {
    config = new TrainingConfiguration2 (_fileDesc,
                                         NULL,      // Not supplying the MLClassList
                                         "-s 0 -n 0.11 -t 2 -g 0.01507  -c 12  -u 100  -up  -mt OneVsOne  -sm P",
                                         _log
                                        );
    config->RootDir (_subDir);
  }


  if  (config->ModelParameters () == NULL)
  {
    bool  _validFormat = true;
    KKStr  svmParameterStr = "-s 0 -n 0.11 -t 2 -g 0.01507  -c 12  -u 100  -up  -mt OneVsOne  -sm P";

    ModelParamOldSVMPtr  oldSVMparameters = new ModelParamOldSVM (_fileDesc, _log);
    oldSVMparameters->ParseCmdLine (svmParameterStr, _validFormat);
    config->ModelParameters (oldSVMparameters);
    config->ImagesPerClass (1000);
    delete  oldSVMparameters;
    oldSVMparameters = NULL;
  }
  

  TrainingClassListPtr  origTraniningClasses = config->TrainingClasses ().DuplicateListAndContents ();

  config->EmptyTrainingClasses ();

  
  delete  config->mlClasses;
  config->mlClasses = new MLClassConstList ();
  config->mlClassesWeOwnIt = true;


  KKStr  subDirUnderRoot = "";
  config->BuildTrainingClassListFromDirectoryStructure (_subDir,
                                                        subDirUnderRoot,
                                                        _successful, 
                                                        _errorMessage
                                                       );

  {
    // We can now update the traningClasses found with that which is in origTraniningClasses.
    TrainingClassList::iterator  idx;

    for  (idx = config->trainingClasses.begin ();  idx != config->trainingClasses.end ();  idx++)
    {
      TrainingClassPtr  tc = *idx;

      // I am not sure about this line.  Might need top use rootDir from original configuration file.
      TrainingClassPtr  origTC = origTraniningClasses->LocateByDirectory (tc->ExpandedDirectory (_subDir));
      if  (origTC)
      {
        tc->MLClass      (origTC->MLClass ());
        tc->FeatureFileName (origTC->FeatureFileName ());
      }
    }
  }

  delete  origTraniningClasses;
  origTraniningClasses = NULL;

  if  (config->Gamma () == 0.0)
    config->Gamma (0.01507);

  config->Save (directoryConfigFileName);

  return  config;
} /* CreateFromDirectoryTree */




void  TrainingConfiguration2::BuildTrainingClassListFromDirectoryStructure (const KKStr&  rootDir,
                                                                            const KKStr&  subDir,
                                                                            bool&         successful,
                                                                            KKStr&        errorMessage
                                                                           )
{
  log.Level (10) << "BuildTrainingClassListFromDirectoryStructure  subDir[" << subDir << "]" << endl;

  KKStr  currentDirectory = osAddSlash (rootDir) + subDir;

  KKStr  dirSearchPath = osAddSlash (currentDirectory) + "*.*";
  {
    // Are there an image files in this directory ?
    KKStrListPtr  imageFilesInDir = osGetListOfImageFiles (dirSearchPath);
    bool  thereAreImageFilesInDir = (imageFilesInDir->QueueSize () > 0);
    delete  imageFilesInDir;
    imageFilesInDir = NULL;

    if  (thereAreImageFilesInDir)
    {
      KKStr  className = MLClass::GetClassNameFromDirName (currentDirectory);
      AddATrainingClass (new TrainingClass (subDir, className, "W", 1.0f, *mlClasses));
    }
  }

  successful = true;
  KKStrListPtr  subDirectories = osGetListOfDirectories (dirSearchPath);
  KKStrList::iterator  sdIDX;
  for  (sdIDX = subDirectories->begin ();  (sdIDX != subDirectories->end ())  &&  successful;  ++sdIDX)
  {
    KKStr  subDirName (**sdIDX);
    KKStr  newDirPath = "";
    if  (subDir.Empty ())
       newDirPath = subDirName;
    else
       newDirPath = osAddSlash (subDir) + subDirName;
    BuildTrainingClassListFromDirectoryStructure (rootDir, newDirPath, successful, errorMessage);
  }
  delete  subDirectories;
  subDirectories = NULL;
}  /* BuildTrainingClassListFromDirectoryStructure */




/**
 * @brief  Add a Training class to configuration
 *         Will take ownership of allocation.
 */
void  TrainingConfiguration2::AddATrainingClass (TrainingClassPtr  _trainClass)
{
  trainingClasses.PushOnBack (_trainClass);
}  /* AddATrainingClass */




void   TrainingConfiguration2::AddATrainingClass (MLClassConstPtr  _newClass)  // Will assume that images for this class will 
{                                                                                 // be saved off the RootDirectory using its own 
                                                                                  // name for the subdirectory name.
  TrainingClassPtr  tc = new TrainingClass ("",
                                            _newClass->Name (),
                                            "",
                                            1.0f,                          // Weight given to this Class during training
                                            *mlClasses
                                           );
  AddATrainingClass (tc);
}



const SVMparam&  TrainingConfiguration2::SVMparamREF ()  const
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
  {
    SVMparamPtr s =  oldSVMparms->SvmParameters ();
    return  *s;
  }
  else
  {
    KKStr  errMsg = "TrainingConfiguration2::SVMparamREF   ***ERROR***   (modelingMethod != mmOldSVM)";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw new KKStrException (errMsg);
  }
}  /* SVMparam */



SVMparamPtr  TrainingConfiguration2::SVMparamToUse ()  const
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
  {
    SVMparamPtr s =  oldSVMparms->SvmParameters  ();
    return  s;
  }
  else
  {
    return NULL;
  }
}  /* SVMparamToUse */





float   TrainingConfiguration2::A_Param () const
{
  if  (modelParameters)
    return  modelParameters->A_Param ();
  else
    return 0.0f;
}



void  TrainingConfiguration2::A_Param (float  _aParam)
{
  if  (modelParameters)
    modelParameters->A_Param (_aParam);
}  /* A_Param */



int32  TrainingConfiguration2::BitsToReduceBy () const
{
  if  (modelParameters)
    return modelParameters->BitsToReduceBy ();
  else
    return 0;
}  /* BitsToReduceBy */



void  TrainingConfiguration2::BitsToReduceBy (int32  bitReduction)
{
  if  (modelParameters)
    modelParameters->BitsToReduceBy (bitReduction);
} /* BitsToReduceBy */





double  TrainingConfiguration2::Gamma () const
{
  if  (modelParameters)
    return  modelParameters->Gamma ();
  else
    return 0.0;
}  /* Gamma */



void  TrainingConfiguration2::Gamma (double  _gamma)
{
  if  (modelParameters)
    modelParameters->Gamma (_gamma);
}  /* Gamma */



double  TrainingConfiguration2::C_Param () const
{
  if  (modelParameters)
    return  modelParameters->C_Param ();
  else
    return 0.0;
}



void  TrainingConfiguration2::C_Param (double _CCC)
{
  if  (modelParameters)
    modelParameters->C_Param (_CCC);
}



int32  TrainingConfiguration2::Number_of_rounds ()  const
{
  if  (modelParameters  &&  (modelParameters->ModelParamType () == ModelParam::mptUsfCasCor))
  {
    return  dynamic_cast<ModelParamUsfCasCor*>(modelParameters)->Number_of_rounds ();
  }
  else
  {
    return  -1;
  }
}



void   TrainingConfiguration2::Number_of_rounds (int32  _number_of_rounds)
{
  if  (modelParameters  &&  (modelParameters->ModelParamType () == ModelParam::mptUsfCasCor))
  {
    dynamic_cast<ModelParamUsfCasCor*>(modelParameters)->Number_of_rounds (_number_of_rounds);
  }
}



void  TrainingConfiguration2::UnBalancedBits  (int32 _unBalancedBits)
{
  if  (modelParameters)
    modelParameters->UnBalancedBits (_unBalancedBits);
}  /* UnBalancedBits */




void  TrainingConfiguration2::UnBalancedBitsStr  (const KKStr&  _unBalancedBitsStr)
{
  if  (modelParameters)
    modelParameters->UnBalancedBitsStr (_unBalancedBitsStr);
}  /* UnBalancedBitsStr */





void  TrainingConfiguration2::MachineType (SVM_MachineType _machineType)
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    oldSVMparms->MachineType (_machineType);

}  /* MachineType */




SVM_MachineType  TrainingConfiguration2::MachineType ()  const
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    return oldSVMparms->MachineType ();
  else
    return  MachineType_NULL;
}  /* MachineType */




void  TrainingConfiguration2::CompressionMethod (SVM_CompressionMethod  _compMethod)
{
  if  (modelParameters)
    modelParameters->CompressionMethod ((ModelParam::CompressionMethodType)_compMethod);
}  /* CompressionMethod */




SVM_CompressionMethod  TrainingConfiguration2::CompressionMethod ()  const
{
  if  (modelParameters)
    return (SVM_CompressionMethod)modelParameters->CompressionMethod ();
  else
    return  BRnoCompression;
}  /* CompressionMethod */




void  TrainingConfiguration2::SelectionMethod (SVM_SelectionMethod  _selectionMethod)
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    oldSVMparms->SelectionMethod (_selectionMethod);
}  /* SelectionMethod */



SVM_SelectionMethod  TrainingConfiguration2::SelectionMethod   ()  const
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    return (SVM_SelectionMethod)oldSVMparms->SelectionMethod ();
  else
    return SelectionMethod_NULL;
}  /* SelectionMethod */



void  TrainingConfiguration2::SetFeatureNums (const  FeatureNumList&  features)
{
  if  (features.FileDesc () != fileDesc)
  {
    // The featureNumList being passed in uses a different fileDesc than 
    // TrainingConfiguration2,  this should never be able to happen, so something
    // has gove very wrong.

    KKStr  errMsg = "TrainingConfiguration2::SetFeatureNums      ***ERROR***     MisMatch in FileDesc.";
    log.Level (-1) << endl << endl << errMsg << endl << endl;
    throw KKStrException (errMsg);
  }

  if  (modelParameters)
    modelParameters->SelectedFeatures (features);

}  /* SetFeatureNums */




void  TrainingConfiguration2::SetFeatureNums (MLClassConstPtr     class1,
                                              MLClassConstPtr     class2,
                                              const FeatureNumList&  _features,
                                              float                  _weight
                                             )
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
  {
    oldSVMparms->SetFeatureNums (class1, class2, _features, _weight);
  }
}  /* SetFeatureNums */





void  TrainingConfiguration2::EncodingMethod (SVM_EncodingMethod _encodingMethod)
{
  if  (modelParameters)
    modelParameters->EncodingMethod ((ModelParam::EncodingMethodType)_encodingMethod);
}



SVM_EncodingMethod   TrainingConfiguration2::EncodingMethod ()  const
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (param)
    return  param->EncodingMethod ();
  else
    return  NoEncoding;
}  /* EncodingMethod */




void  TrainingConfiguration2::WeightBitReduction (bool  _weightBitReduction) 
{
  if  (modelParameters)
    modelParameters->WeightBitReduction (_weightBitReduction);
}



bool  TrainingConfiguration2::WeightBitReduction ()  const 
{
  if  (modelParameters)
    return modelParameters->WeightBitReduction ();
  else
    return 0;
}




double  TrainingConfiguration2::C_Param (MLClassConstPtr  class1,
                                         MLClassConstPtr  class2
                                        )  const
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    return  oldSVMparms->C_Param (class1, class2);
  else
    return 0.0;
}




void  TrainingConfiguration2::C_Param (MLClassConstPtr  class1,
                                       MLClassConstPtr  class2,
                                       double              cParam
                                      )
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    oldSVMparms->C_Param (class1, class2, cParam);
}  /* C_Param */







void  TrainingConfiguration2::SetBinaryClassFields (MLClassConstPtr             class1,
                                                    MLClassConstPtr             class2,
                                                    const SVM233::svm_parameter&   _param,
                                                    const FeatureNumList&          _features,
                                                    float                          _weight
                                                   )
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    oldSVMparms->SetBinaryClassFields (class1, class2, _param, _features, _weight);
}  /* SetBinaryClassFields */




void  TrainingConfiguration2::SetTrainingClasses (TrainingClassListPtr  _trainingClasses)
{
  EmptyTrainingClasses ();
  TrainingClassList::iterator  idx;

  for  (idx = _trainingClasses->begin ();  idx != _trainingClasses->end ();  idx++)
    AddATrainingClass (*idx);
}  /* SetTrainingClasses */




float  TrainingConfiguration2::AvgNumOfFeatures ()  const
{
  if  (modelParameters)
    return modelParameters->AvgMumOfFeatures ();
  return 0.0f;
}  /* AvgMumOfFeatures */




float  TrainingConfiguration2::AvgNumOfFeatures (FeatureVectorListPtr  trainExamples)  const
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (param)
    return param->AvgNumOfFeatures (trainExamples);
  else
    return  0.0f;
}




SVM_KernalType  TrainingConfiguration2::KernalType ()  const
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (param)
    return param->KernalType ();
  return  KT_Linear;
}  /* KernalType */




void   TrainingConfiguration2::KernalType (SVM_KernalType _kernalType)
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (param)
    return param->KernalType (_kernalType);
}  /* KernalType */




int32  TrainingConfiguration2::UnBalancedBits  ()  const
{
  if  (modelParameters)
    return  modelParameters->UnBalancedBits ();
  else
    return 0;
}




KKStr  TrainingConfiguration2::UnBalancedBitsStr  ()  const
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (param)
    return param->UnBalancedBitsStr ();
  else
    return  KKStr::EmptyStr ();
}   /* UnBalancedBitsStr */





FeatureNumList  TrainingConfiguration2::GetFeatureNums ()  const
{
  if  (modelParameters)
    return modelParameters->SelectedFeatures ();
  else
    return FeatureNumList (fileDesc);
}  /* GetFeatureNums */




int32  TrainingConfiguration2::NumOfFeaturesAfterEncoding ()  const
{
  if  (modelParameters)
    return modelParameters->NumOfFeaturesAfterEncoding ();
  else
    return 0;
}  /* NumOfFeaturesAfterEncoding */



void   TrainingConfiguration2::ExamplesPerClass (int32 _examplesPerClass)
{
  examplesPerClass = _examplesPerClass;
}



int32  TrainingConfiguration2::ExamplesPerClass ()  const
{
  return examplesPerClass;
}  /* ImagesPerClass */





FeatureNumList   TrainingConfiguration2::GetFeatureNums (MLClassConstPtr  class1,
                                                         MLClassConstPtr  class2
                                                        )
{
  ModelParamOldSVMPtr  oldSVMparms = OldSvmParameters ();
  if  (oldSVMparms)
    return  oldSVMparms->GetFeatureNums (class1, class2);
  else
    return  FeatureNumList::AllFeatures (fileDesc);
}  /* GetFeatureNums */





void  TrainingConfiguration2::SetModelParameters (ModelParamPtr  _svmParanters,
                                                  int32            _examplesPerClass
                                                 )
{
  delete  modelParameters;
  modelParameters   = _svmParanters->Duplicate ();
  examplesPerClass  = _examplesPerClass;
}  /* SetModel3Parameters */




TrainingClassPtr  TrainingConfiguration2::ValidateClassConfig (int32  sectionNum)
{
  int32  classNameLineNum = 0;
  int32  dirLineNum       = 0;
  int32  forgroundLineNum = 0;
  int32  weightLineNum    = 0;

  int32  sectionLineNum = SectionLineNum (sectionNum);

  KKStrConstPtr  classDir  = SettingValue (sectionNum, "DIR",        dirLineNum);
  KKStrConstPtr  className = SettingValue (sectionNum, "CLASS_NAME", classNameLineNum);
  KKStrConstPtr  weightStr = SettingValue (sectionNum, "WEIGHT",     weightLineNum);

  KKStr  classDirToUse = "";
  if  ((classDir)  &&  (!classDir->Empty ()))
    classDirToUse = *classDir;

  else if  (className)
    classDirToUse = osAddSlash (rootDir) + *className;

  else
    classDirToUse = "";

  KKStr  foreground (SettingValue (sectionNum, "FOREGROUND", forgroundLineNum));
  if  (!className)
  {
    KKStr errMsg = "'TRAINING_CLASS' section, CLASS_NAME not defined.";
    log.Level (-1) << "ValidateClassConfig   " << errMsg  << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
    return  NULL;
  }

  if  (classDirToUse.Empty ())
  {
    KKStr  errMsg = "'TRAINING_CLASS' section, Directory not defined.";
    log.Level (-1) << "ValidateClassConfig   " << errMsg << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
    return  NULL;
  }

  float  weight = 1.0f;
  if  ((weightStr != NULL)  &&  (weightStr->Len () > 0))
  {
    weight = weightStr->ToFloat ();
    if  (weight <= 0.0f)
    {
      KKStr  errMsg = "Class[" + (*className)  + "]    Invalid Weight Parameter.";
      log.Level (-1) << "ValidateClassConfig   " << errMsg << endl;
      FormatErrorsAdd (weightLineNum, errMsg);
      FormatGood (false);
      return  NULL;
    }
  }

  TrainingClassPtr tc = new TrainingClass (classDirToUse, *className, foreground, weight, *mlClasses);

  if  (validateDirectories)
  {
    KKStr  expandedDir = tc->ExpandedDirectory (rootDir);

    if  (!osValidDirectory (expandedDir))
    {
      KKStr  errMsg = "Invalid Directory Specified[" + expandedDir + "].";
      log.Level (-1) << endl << "ValidateClassConfig   ***ERROR***  " << errMsg << endl << endl;
      FormatErrorsAdd (dirLineNum, errMsg);
      FormatGood (false);
      delete  tc;  tc = NULL;
      return NULL;
    }

    if  (foreground.Empty ())
      foreground = "W";

    foreground.Upper ();
    if  ((foreground != "B")  &&  (foreground != "W"))
    {
      KKStr  errMsg = "Invalid FOREGROUND Specified[" + foreground + "].";
      log.Level (-1) << endl << "ValidateClassConfig   ***ERROR***  " << errMsg <<  endl << endl;
      FormatErrorsAdd (forgroundLineNum, errMsg);
      FormatGood (false);
      delete  tc;  tc = NULL;
      return  NULL;
    }
  }

  return  tc;
}  /* ValidateClassConfig */



void TrainingConfiguration2::ValidateOtherClass (MLClassConstPtr  otherClass,
                                                 int32               otherClassLineNum
                                                )
{
  KKStr  classDirToUse = osAddSlash (rootDir) + otherClass->Name ();
  if  (!osValidDirectory (classDirToUse))
  {
    KKStr  errMsg = "OtherClass  Directory [" + classDirToUse + "]  does not exist.";
    log.Level (-1) << endl << "ValidateOtherClass   ***ERROR***  " << errMsg << endl << endl;
    FormatErrorsAdd (otherClassLineNum, errMsg);
    FormatGood (false);
  }
}  /* ValidateOtherClass */




void  TrainingConfiguration2::ValidateTrainingClassConfig (int32  sectionNum)
{
  TrainingClassPtr  trainingClass = ValidateClassConfig (sectionNum);
  if  (trainingClass)
  {
    trainingClasses.AddTrainingClass (trainingClass);
  }

}  /* ValidateTrainingClassConfig */



FeatureNumListPtr  TrainingConfiguration2::DeriveFeaturesSelected (int32  sectionNum)
{
  int32  sectionLineNum = SectionLineNum (sectionNum);

  int32  featuresIncludedLineNum = 0;
  int32  featuresExcludedLineNum = 0;

  KKStr  includedFeaturesStr (SettingValue (sectionNum, "FEATURES_INCLUDED", featuresIncludedLineNum));
  KKStr  excludedFeaturesStr (SettingValue (sectionNum, "FEATURES_EXCLUDED", featuresExcludedLineNum));

  FeatureNumListPtr  selectedFeatures;

  bool valid = true;

  if  (includedFeaturesStr.Empty ()  &&  excludedFeaturesStr.Empty ())
  {
    // We will assume user want's all features.
    selectedFeatures = new FeatureNumList (FeatureNumList::AllFeatures (fileDesc));
  }

  else if  (!includedFeaturesStr.Empty ())
  {
    selectedFeatures = new FeatureNumList (fileDesc, FeatureNumList::IncludeFeatureNums, includedFeaturesStr, valid);
  }

  else  if  (!excludedFeaturesStr.Empty ())
  {
    selectedFeatures = new FeatureNumList (fileDesc, FeatureNumList::ExcludeFeatureNums, excludedFeaturesStr, valid);
  }

  else
  {
    KKStr errMsg = "Can not specify both Include and Exclude Features";
    log.Level (-1) << endl << "DeriveFeaturesSelected   ***ERROR***  " << errMsg << endl << endl;

    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
    return NULL;
  }

  if  (!valid)
  {
    FormatGood (false);
    delete  selectedFeatures;
    return  NULL;
  }

  return  selectedFeatures;
} /* DeriveFeaturesSelected */



//****************************************************************
//*  Kurt  Oct-19-2002                                           *
//*                                                              *
//*  Added next method to support Multiple Modeling Methods.     *
//****************************************************************
void   TrainingConfiguration2::ValidateGlobalSection (int32  sectionNum)
{
  int32  methodLineNum     = 0;
  int32  rootDirLineNum    = 0;
  int32  sectionLineNum    = SectionLineNum (sectionNum);

  KKStr  modelingMethodStr  (SettingValue (sectionNum, "MODELING_METHOD", methodLineNum));
  modelingMethodStr.Upper ();

  log.Level (30) << "ValidateGlobalSection - ModelingMethod[" << modelingMethod << "]   LineNum[" << methodLineNum << "]." << endl;

  modelingMethod = ModelTypeFromStr (modelingMethodStr);

  if  (methodLineNum < 0)
  {
    KKStr errMsg = "No Modeling_Method was specified.";
    log.Level (-1) << endl << "ValidateGlobalSection  ***ERROR***  " << errMsg << endl << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
  }

  if  (modelingMethod == Model::mtNULL)
  {
    // We have a invalid Modeling Method Paraeter.
    KKStr  errMsg = "Invalid Modeling Method[" + modelingMethodStr + "] on Line[" << methodLineNum << "] was specified.";
    log.Level (-1) << endl << "ValidateGlobalSection   ***ERROR***    " << errMsg << endl << endl;
    FormatErrorsAdd (methodLineNum, errMsg);
    FormatGood (false);
  }

  KKStr  rootDirStr  (SettingValue (sectionNum, "ROOT_DIR", rootDirLineNum));
  if  (rootDirLineNum < 0)
  {
    // No Root Directory was specified.
    rootDir = osAddSlash (SipperVariables::TrainingLibrariesDir ()) + osGetRootName (FileName ());
    log.Level (10) << endl
        << "ValidateGlobalSection   'Root_Dir'  Was not specified defaulting to[" << rootDir << "]" << endl
        << endl;
  }
  else
  {
    rootDir = SipperVariables::SubstituteInEvironmentVariables (rootDirStr);
    if  (!rootDir.Empty ()  &&  (!osValidDirectory (rootDir))  &&  validateDirectories)
    {
      KKStr errMsg = "Invalid RootDir[" + rootDirStr + "]  Specified on line[" + StrFormatInt (rootDirLineNum, "ZZZ0") + "].  Expanded to[" + rootDir + "]";
      log.Level (-1) << endl << endl << "ValidateGlobalSection   ***ERROR*** " << errMsg << endl << endl;
      FormatErrorsAdd (rootDirLineNum, errMsg);
      FormatGood (false);
    }
  }

  int32  parametersLineNum        = -1;
  int32  featuresIncludedLineNum  = -1;
  int32  examplesPerClassLineNum  = -1;

  KKStr  modelParametersStr  = SettingValue (sectionNum, "Parameters",        parametersLineNum);
  KKStr  featuresIncludedStr = SettingValue (sectionNum, "Features_Included", featuresIncludedLineNum);

  if  ((parametersLineNum < 0)  &&  (modelingMethod == Model::mtOldSVM))
    modelParametersStr = SettingValue ("Model3", "Parameters", parametersLineNum);

  if  ((featuresIncludedLineNum < 0)  &&  (modelingMethod == Model::mtOldSVM))
    featuresIncludedStr = SettingValue ("Model3", "Features_Included", featuresIncludedLineNum);

  if  (parametersLineNum < 0)
  {
    KKStr errMsg = "'Parameters' setting was not specified.";
    log.Level (-1) << endl << endl << "ValidateGlobalSection   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
  }

  if  (featuresIncludedStr.Empty ())
    featuresIncludedStr = "All";

  bool validFeaturesNums = false;
  FeatureNumList selFeatures (fileDesc, featuresIncludedStr, validFeaturesNums);
  if  (!validFeaturesNums)
  {
    KKStr  errMsg = "Invalid Features_Included[" + featuresIncludedStr + "].";
    log.Level (-1) << endl << "ValidateGlobalSection   ***ERROR***   "  << errMsg << endl << endl;
    FormatErrorsAdd (featuresIncludedLineNum, errMsg);
    FormatGood (false);
  }

  {
    KKStr  examplesPerClassStr = SettingValue (sectionNum, "Examples_Per_Class", examplesPerClassLineNum);
    if  (examplesPerClassStr.Empty ())
      examplesPerClassStr = SettingValue (sectionNum, "Images_Per_Class", examplesPerClassLineNum);

    if  ((examplesPerClassLineNum < 0)  &&  (modelingMethod == Model::mtOldSVM))
    {
      examplesPerClassStr = SettingValue ("Model3", "Examples_Per_Class", examplesPerClassLineNum);
      if  (examplesPerClassStr.Empty ())
        examplesPerClassStr = SettingValue ("Model3", "Images_Per_Class", examplesPerClassLineNum);
    }

    if  (examplesPerClassStr.Empty ())
      examplesPerClass = int32_max;

    else
      examplesPerClass = examplesPerClassStr.ToInt ();

    if  (examplesPerClass < 1)
    {
      KKStr  errMsg = "Invalid Examples_Per_Class[" + examplesPerClassStr + "].";
      log.Level (-1) << endl << "ValidateGlobalSection   ***ERROR***   " << errMsg << endl;
      FormatErrorsAdd (examplesPerClassLineNum, errMsg);
      FormatGood (false);
    }
  }

  CreateModelParameters (modelParametersStr, selFeatures, sectionLineNum, parametersLineNum, featuresIncludedLineNum);
  if  (modelParameters)
  {
    if  (examplesPerClassLineNum >= 0)
      modelParameters->ExamplesPerClass (examplesPerClass);
    else
      examplesPerClass = modelParameters->ExamplesPerClass ();
  }

  KKStr  otherClassName (SettingValue (sectionNum, "OtherClass", otherClassLineNum));
  if  (otherClassLineNum >= 0)
  {
    if  (otherClassName.Empty ())
    {
      KKStr  errMsg = "OtherClass specified but no name provided.";
      log.Level (-1) << endl << "ValidateGlobalSection   ***ERROR***   " << errMsg << endl;
      FormatErrorsAdd (examplesPerClassLineNum, errMsg);
      FormatGood (false);
    }

    otherClass = MLClass::CreateNewMLClass (otherClassName, -1);
    ValidateOtherClass (otherClass, otherClassLineNum);
  }
} /* ValidateGlobalSection */






void   TrainingConfiguration2::ValidateTwoClassParameters (int32  sectionNum)
{
  if  (!modelParameters)
  {
    log.Level (-1) << "ValidateTwoClassParameters - Global Parameters must be defined first."
                   << endl;
    FormatGood (false);
  }

  int32  class1NameLineNum = -1;
  int32  class2NameLineNum = -1;
  int32  parameterLinenum  = -1;
  int32  weightLineNum     = -1;

  int32  sectionLineNum = SectionLineNum (sectionNum);

  KKStr  class1Name   (SettingValue (sectionNum, "CLASS1",     class1NameLineNum));
  KKStr  class2Name   (SettingValue (sectionNum, "CLASS2",     class2NameLineNum));
  KKStr  parameterStr (SettingValue (sectionNum, "PARAMETERS", parameterLinenum));
  KKStr  weightStr    (SettingValue (sectionNum, "WEIGHT",     weightLineNum));

  if  (class1NameLineNum < 0)
  {
    KKStr errMsg = "'Class1' setting was not defined.";
    log.Level (-1) << endl << "ValidateTwoClassParameters   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
  }

  if  (class2NameLineNum < 0)
  {
    KKStr errMsg = "'Class2' setting was not defined.";
    log.Level (-1) << endl << "ValidateTwoClassParameters   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (sectionLineNum, errMsg);
    FormatGood (false);
  }

  MLClassConstPtr  class1 = mlClasses->LookUpByName (class1Name);
  MLClassConstPtr  class2 = mlClasses->LookUpByName (class2Name);

  if  (!class1)
  {
    log.Level (20) << "ValidateTwoClassParameters  LineNum[" << class1NameLineNum << "]  Class[" << class1Name << "] not part of classifier."  << endl;
    return;
  }

  if  (!class2)
  {
    log.Level (20) << "ValidateTwoClassParameters  LineNum[" << class2NameLineNum << "]  Class[" << class2Name << "] not part of classifier."  << endl;
    return;
  }

  FeatureNumListPtr  selectedFeatures = DeriveFeaturesSelected (sectionNum);
  if  (!selectedFeatures)
    selectedFeatures = new FeatureNumList (modelParameters->SelectedFeatures ());


  if  ((class1NameLineNum > 0)  &&  (!class1))
  {
    KKStr errMsg = "Class[" + class1Name + "] is not defined.";
    log.Level (-1) << endl << "ValidateTwoClassParameters   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (class1NameLineNum, errMsg);
    FormatGood (false);
  }

  if  ((class2NameLineNum > 0)  &&  (!class2))
  {
    KKStr errMsg = "Class[" + class2Name + "] is not defined.";
    log.Level (-1) << endl << "ValidateTwoClassParameters   ***ERROR***   " << errMsg << endl << endl;
    FormatErrorsAdd (class2NameLineNum, errMsg);
    FormatGood (false);
  }

  if  (!FormatGood ())
    return;

  if  (class1  &&  class2)
  {
    float  weight = 1.0f;
    if  (!weightStr.Empty ())
      weight = (float)atof (weightStr.Str ());

    ModelParamOldSVMPtr oldSvmParameters = OldSvmParameters ();
    if  (oldSvmParameters)
    {
      SVM233::svm_parameter binaryParam (oldSvmParameters->Param ());
      if  (!parameterStr.Empty ())
      {
        SVM233::svm_parameter  tempParam (parameterStr);
        binaryParam = tempParam;
      }

      oldSvmParameters->AddBinaryClassParms (class1, class2, binaryParam, *selectedFeatures, weight);
    }
  }
  delete  selectedFeatures;
}  /* ValidateTwoClassParameters */







void   TrainingConfiguration2::ValidateConfiguration ()
{
  // Validate TrainingClasses
  
  this->FormatErrorsClear ();

  int32 numOfSections = NumOfSections ();

  for  (int32  sectionNum = 0; sectionNum < numOfSections; sectionNum++)
  {
    KKStr  sectionName (SectionName (sectionNum));
    sectionName.Upper ();

    if  (sectionName == "TRAINING_CLASS")
    {
      ValidateTrainingClassConfig (sectionNum);
    }

    else if  (sectionName == "NOISE_IMAGES")
    {
       noiseTrainingClass = ValidateClassConfig (sectionNum);
       if  (noiseTrainingClass)
       {
         int32  noiseGuaranteedSizeLineNum = -1;
         noiseMLClass = noiseTrainingClass->MLClass ();
         ((MLClassPtr)noiseMLClass)->UnDefined (true);
         KKStr   noiseGuaranteedSizeStr (SettingValue ((int)sectionNum, "GUARANTEED_SIZE", noiseGuaranteedSizeLineNum));
         noiseGuaranteedSize = atoi (noiseGuaranteedSizeStr.Str ());
       }
       else
       {
         log.Level (-1) << "ValidateConfiguration *** Inavlid Noise Class Definition ***" 
                        << endl;
         FormatGood (false);
       }
    }


    else if  (sectionName == "GLOBAL")
    {
      ValidateGlobalSection (sectionNum);
    }


    else if  ((sectionName == "TWO_CLASS_PARAMETERS")  ||
              (sectionName == "TWOCLASSPARAMETERS")    ||
              (sectionName == "TWO_CLASSPARAMETERS")
             )
    {
      ValidateTwoClassParameters (sectionNum);
    }
  }


  if  (trainingClasses.QueueSize () < 1)
  {
    log.Level (-1) << endl << "ValidateConfiguration ***ERROR***    No Training Classes defined." << endl;
    FormatGood (false);
  }


  if  (modelParameters == NULL)
  {
    log.Level (-1) << "ValidateConfiguration ***ERROR***       SVM Parametres not specified." << endl;
    FormatGood (false);
  }

  if  (otherClass != NULL)
  {
    if  (trainingClasses.LocateByMLClass (otherClass) != NULL)
    {
      KKStr  errMsg = "OtherClass[" + otherClass->Name () + "] was also defined as a TrainingClass.";
      log.Level (-1) << endl << "ValidateConfiguration   ***ERROR***    " << errMsg << endl << endl;
      FormatErrorsAdd (otherClassLineNum, errMsg);
      FormatGood (false);
    }
  }

}  /* ValidateConfiguration */






TrainingClassPtr   TrainingConfiguration2::LocateByMLClassName (const KKStr&  className)
{
  return  trainingClasses.LocateByMLClassName (className);
}




void   TrainingConfiguration2::EmptyTrainingClasses ()
{
  while  (trainingClasses.QueueSize () > 0)
  {
    TrainingClassPtr  trainingClass = trainingClasses.PopFromBack ();
    delete  trainingClass;
  }
}  /* DeleteTrainingClasses */





bool  TrainingConfiguration2::NormalizeNominalFeatures ()
{
  ModelParamOldSVMPtr oldSvmParameters = OldSvmParameters ();
  if  (oldSvmParameters)
    return oldSvmParameters->NormalizeNominalFeatures ();

  else if  (modelParameters)
    return modelParameters->NormalizeNominalFeatures ();

  else
    return false;
}  /* NormalizeNominalFeatures */




KKStr&  MLL::operator<< (KKStr&                              left,
                         TrainingConfiguration2::ModelTypes  modelingMethod
                        )
{
  left.Append (TrainingConfiguration2::ModelTypeToStr (modelingMethod));
  return  left;
}



ostream&  MLL::operator<< (std::ostream&                       os,
                           TrainingConfiguration2::ModelTypes  modelingMethod
                          )
{
  os << TrainingConfiguration2::ModelTypeToStr (modelingMethod);
  return os;
}




FeatureVectorListPtr   TrainingConfiguration2::LoadOtherClasssExamples ()
{
  if  (!otherClass)
    return NULL;

  KKStr otherClassDirName = "";

  if  (rootDir.Empty ())
    otherClassDirName = otherClass->Name ();
  else
    otherClassDirName = osAddSlash (rootDir) + otherClass->Name ();

  otherClassDirName = SipperVariables::SubstituteInEvironmentVariables (otherClassDirName);

  MLClassConstList  tempClasses;
  bool  cancelFlag  = false;
  bool  changesMade = false;
  DateTime  latestTimeStamp;

  FeatureVectorListPtr  otherClassExamples  = FeatureFileIOPices::FeatureDataReSink 
                                (otherClassDirName,
                                 otherClass->Name () + ".data",
                                 otherClass,
                                 true,   //  Make all entries in this directory 'otherClass'
                                 tempClasses,
                                 cancelFlag,
                                 changesMade,
                                 latestTimeStamp,
                                 log
                                );

  return  otherClassExamples;
}  /* LoadOtherClasssExamples */




/*!
 \brief  Load traning data from the trainig libraries.
 \details
 \code
 **********************************************************************************************  
 * Description:   Loads all the FeatureData from the Training Library.  The images in the     *
 *                subdirectoris sepecified by the 'TrainingClass' entries will be used as the *
 *                primary source.  Data from existing FeatureData files will be used unless   *
 *                they are out of date.  In that case the data will be recomputed from the    *
 *                original images.                                                            *
 *                                                                                            *
 * ImageFileName: This field that is in each 'ImageFeatures' object will reflect the sub-     *
 *                directory from where it was retrieved.  Bewteen that and the 'rootDir' field*
 *                you will be able to get the full path to then original image.               *
 **********************************************************************************************  
 \endcode
 \param[out] latestImageTimeStamp  Timestamp of the most current image loaded.
 \param[out] changesMadeToTrainingLibraries  True if any image needed to have its features recalculated
                                             or images were removed or added to a training directory.
 \param[in]  cancelFlag  Will monitior this flag.  If it gets set to true will terminate the load and return.
 */
FeatureVectorListPtr  TrainingConfiguration2::LoadFeatureDataFromTrainingLibraries (DateTime&  latestImageTimeStamp,
                                                                                    bool&      changesMadeToTrainingLibraries,
                                                                                    bool&      cancelFlag
                                                                                   )
{
  log.Level (10) << "TrainingConfiguration2::LoadFeatureDataFromTrainingLibraries - Starting." << endl;


  InstrumentDataFileManager::InitializePush ();

  bool  errorOccured = false;

  FeatureVectorListPtr  featureData = new ImageFeaturesList (fileDesc, 
                                                             true,     // true = we own data in list
                                                             log, 
                                                             1000
                                                            );

  changesMadeToTrainingLibraries = false;

  //****************************************************************************
  // Make sure that there are no existing *.data files that we are going to use.
  // We need to do this in a seperate pass because more than one entry may refer 
  // to the same Class and hense the same *.data file.


  //***********************************************************
  //  We will first extract the Raw features from each Class 

  TrainingClassList::const_iterator  tcIDX;

  DateTime  latestTimeStamp;
  bool      changesMadeToThisTrainingClass;

  for  (tcIDX = trainingClasses.begin ();  (tcIDX != trainingClasses.end ()  &&  (!cancelFlag)  &&  (!errorOccured));  tcIDX++)
  {
    const TrainingClassPtr trainingClass = *tcIDX;

    log.Level (20) << "LoadFeatureDataFromTrainingLibraries   Starting on Class[" << trainingClass->Name () << "]." << endl;

    FeatureVectorListPtr  featureDataThisClass = ExtractFeatures (trainingClass, latestTimeStamp, changesMadeToThisTrainingClass, cancelFlag);

    if  (!featureDataThisClass)
    {
      log.Level (-1) << endl << endl << endl
                     << "Error Loading Loading feature data for class[" << trainingClass->Name () << "]." << endl;
      errorOccured = true;
    }
    else
    {
      if  (latestTimeStamp > latestImageTimeStamp)
        latestImageTimeStamp = latestTimeStamp;

      if  (changesMadeToThisTrainingClass)
        changesMadeToTrainingLibraries = true;

      featureData->AddQueue (*featureDataThisClass);
      featureDataThisClass->Owner (false);
    }
    delete  featureDataThisClass;  featureDataThisClass = NULL;
  }


  if  (NoiseTrainingClass ()  &&  (!cancelFlag)  &&  (!errorOccured))
  {
    log.Level (30) << "LoadFeatureDataFromTrainingLibraries  Loading Noise Class [" << NoiseTrainingClass ()->ExpandedDirectory (rootDir) << "]" << endl;
    FeatureVectorListPtr  noiseFeatureData = ExtractFeatures (NoiseTrainingClass (), latestTimeStamp, changesMadeToThisTrainingClass, cancelFlag);
    if  (!noiseFeatureData)
    {
      log.Level (-1) << endl << endl << endl
                     << "Error Loading Loading feature data for Noise Images, Directory[" 
                     << NoiseTrainingClass ()->ExpandedDirectory (rootDir) 
                     << "]." 
                     << endl;
      delete  featureData;  featureData = NULL;
      errorOccured = true;
    }
    else
    {
      if  (latestTimeStamp > latestImageTimeStamp)
        latestImageTimeStamp = latestTimeStamp;

      if  (changesMadeToThisTrainingClass)
        changesMadeToTrainingLibraries = true;

      featureData->AddQueue (*noiseFeatureData);
      noiseFeatureData->Owner (false);
      delete  noiseFeatureData;  noiseFeatureData = NULL;
    }
  }


  InstrumentDataFileManager::InitializePop ();

  if  (cancelFlag  ||  errorOccured)
  {
    log.Level (-1) << "LoadFeatureDataFromTrainingLibraries     ***ERROR***    CancelFlag or ErrorOcured  set to 'true'." << endl;
    delete  featureData;  
    featureData = NULL;
  }

  log.Level (20) << "LoadFeatureDataFromTrainingLibraries   Exiting" << endl;
  return  featureData;
}  /*  LoadFeatureDataFromTrainingLibraries */





FeatureVectorListPtr  TrainingConfiguration2::ExtractFeatures (const TrainingClassPtr  trainingClass,
                                                               DateTime&               latestTimeStamp,
                                                               bool&                   changesMade,
                                                               bool&                   cancelFlag
                                                              )
{
  log.Level (30) << "TrainingConfiguration2::ExtractFeatures - Extracting Features from Directory["
                 << trainingClass->ExpandedDirectory (rootDir) << "],  into file["
                 << trainingClass->FeatureFileName   ()        << "]."
                 << endl;

  KKStr  expandedDir = trainingClass->ExpandedDirectory (rootDir);
  FeatureVectorListPtr  extractedExamples = FeatureFileIOPices::FeatureDataReSink 
                                (expandedDir,
                                 trainingClass->FeatureFileName (),
                                 trainingClass->MLClass (),
                                 true,   //  Make all entries in this directory 'trainingClass->MLClass ()'
                                 *mlClasses,
                                 cancelFlag,
                                 changesMade,
                                 latestTimeStamp,
                                 log
                                );
  {
    // We now want to reset the ImageFileNames stored in each 'ImageFeatures' object so that it 
    // reflects the sub-diorectory from where it was retrieved.  Bewteen that and the 'rootDir' 
    // field you will be able to get the full path to then original image.

    KKStr subDirName = trainingClass->Directory ();
    if  (!subDirName.Empty ())
      subDirName = osAddSlash (subDirName);

    FeatureVectorList::iterator  idx;
    for  (idx = extractedExamples->begin ();  idx != extractedExamples->end ();  idx++)
    {
      FeatureVectorPtr  fv = *idx;
      KKStr  newFileName =  subDirName + osGetRootNameWithExtension (fv->ImageFileName ());
      fv->ImageFileName (newFileName);
    }
  }

  log.Level (30) << "ExtractFeatures    Exiting" << endl;
  return  extractedExamples;
}  /* ExtractFeatures */





/**
 *@brief  Determine the correct csonfiguration file name.
 *@details
 *@code
 *********************************************************************************************************
 *  If no extension then add ".cfg" to end of file name.                                                 *
 *                                                                                                       * 
 *  If  Path Provided                                                                                    *
 *  {                                                                                                    *
 *     Return name as submitted with added extension.                                                    *
 *  }                                                                                                    *
 *  else                                                                                                 *
 *  {                                                                                                    *
 *     a. If file exists if default directory;  then return name with defalt directory path.             *
 *     b. If file exists in Training Model Directory  $(PicesHomeDir) + "\\DataFiles\\TrainingModels"    *
 *        then return with Training Model Directory.                                                     *
 *     c. Since not found then we will return name passed in with extension.                             *
 *  }                                                                                                    *
 *********************************************************************************************************
 *@endcode
 */
KKStr   TrainingConfiguration2::GetEffectiveConfigFileName (const  KKStr&  configFileName)
{
  if  (osFileExists (configFileName))
    return  configFileName;

  KKStr  rootNameEithExtension = osGetRootNameWithExtension (configFileName);
  KKStr  extension = osGetFileExtension (configFileName);
  KKStr  path  = osGetPathPartOfFile (configFileName);

  KKStr  configFileNameWithExtension = configFileName;
  if  (extension.Empty ())
  {
    configFileNameWithExtension = osRemoveExtension (configFileName) + ".cfg";
  }

  if  (!path.Empty ())
  {
    // Caller is telling us which directory to look in.  In this case there is nothing to do
    return configFileNameWithExtension;
  }

  // See if file exists
  KKStr  configFileNameInDefaultDirectory = osAddSlash (osGetCurrentDirectory ()) + configFileNameWithExtension;
  if  (osFileExists (configFileNameInDefaultDirectory))
    return  configFileNameInDefaultDirectory;

  KKStr  configNameInTrainingModelDir = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + configFileNameWithExtension;
  if  (osFileExists (configNameInTrainingModelDir))
  {
    return  configNameInTrainingModelDir;
  }

  return  configFileNameInDefaultDirectory;
}  /* GetEffectiveConfigFileName */



bool  TrainingConfiguration2::ConfigFileExists (const KKStr& _configFileName)
{
  KKStr  effectiveName = GetEffectiveConfigFileName (_configFileName);
  return osFileExists (effectiveName);
}  /* ConfigFileExists */




TrainingConfiguration2Ptr  TrainingConfiguration2::PromptForConfigurationFile (RunLog&  log)
{
  FileDescPtr  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  KKStr  configFileSpec = osAddSlash (SipperVariables::TrainingModelsConfigurationDir ()) + "*.cfg";
  KKStrListPtr   configFileList = osGetListOfFiles (configFileSpec);
  int32  numFiles = 0;
  int32 x;
  if  (configFileList != NULL)
    numFiles = configFileList->QueueSize ();
  if  (numFiles == 0)
  {
    delete  configFileList;  configFileList = NULL;
    cerr << endl << endl << endl
         << "There are no configuration files to choose from." << endl;
    osWaitForEnter ();
    return NULL;
  }

  KKStr  configFileName;

  TrainingConfiguration2Ptr  selectedConfig = NULL;

  int32  fileNum = 0;

  while  (!selectedConfig)
  {
    cout << endl << endl
         << "Configuration Files:" << endl;

    for  (x = 0;  x < numFiles;  x++)
    {
      KKStr&  fileName  = (*(configFileList->IdxToPtr (x)));
      cout << "   " << StrFormatInt ((x + 1), "##0") << ": " << fileName << endl;
    }

    cout << endl
         << "Select configuration file number or -1 to exit ? ";
    cin >> fileNum;
    if  (fileNum < 0)
      break;

    if  ((fileNum < 1)  ||  (fileNum > numFiles))
    {
      cerr << endl << endl 
           << "***ERROR***   Invalid Response, must be between '1' and '" << numFiles << "'." << endl;
      osWaitForEnter ();
      continue;
    }

    configFileName = (*(configFileList->IdxToPtr (fileNum - 1)));

    selectedConfig = new TrainingConfiguration2 (fileDesc, configFileName, log, true);
    if  (!(selectedConfig->FormatGood ()))
    {
      delete  selectedConfig;
      selectedConfig = NULL;
      cerr << endl << endl << endl
           << "***ERROR***   Configuration file[" << configFileName << "] has errors; select a different one." << endl;
      osWaitForEnter ();
    }

    delete  configFileList;
    configFileList = NULL;
  }

  delete configFileList;
  configFileList = NULL;

  return  selectedConfig;
}  /* PromptForConfigurationFile */




void   TrainingConfiguration2::DetermineWhatTheRootDirectoryIs ()
{
  TrainingClassList::iterator  idx;

  rootDir = "";

  if  (trainingClasses.QueueSize () < 0)
    return;

  uint32 zed = 0;

  VectorKKStr  rootDirParts = osSplitDirectoryPathIntoParts (trainingClasses[0].ExpandedDirectory (rootDir));
  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;
    VectorKKStr  parts = osSplitDirectoryPathIntoParts (tc->ExpandedDirectory (rootDir));

    while  (rootDirParts.size () > parts.size ())
      rootDirParts.pop_back ();
    
    zed = 0;
    while  (zed < rootDirParts.size ())
    {
      if  (!(rootDirParts[zed].EqualIgnoreCase (parts[zed])))
        break;
      zed++;
    }

    while  (zed < rootDirParts.size ())
      rootDirParts.pop_back ();
  }


  //***********************************************************************************
  // We need to update the 'trainingClasses' structure before the 'rootDir' variable.
  // This is because the following code that updates the 'TrainingClass' objects in
  // 'trainingClasses' will use the 'rootDir' variable in the 'ExpandedDirectory' 
  // method.
  for  (idx = trainingClasses.begin ();  idx != trainingClasses.end ();  idx++)
  {
    TrainingClassPtr  tc = *idx;
    VectorKKStr  parts = osSplitDirectoryPathIntoParts (tc->ExpandedDirectory (rootDir));

    KKStr newTrainClassSubDir = "";
    for (zed = (uint32)rootDirParts.size ();  zed < (uint32)parts.size ();  zed++)
      newTrainClassSubDir << parts[zed];

    tc->Directory (newTrainClassSubDir);
  }

  rootDir = "";
  for  (zed = 0;  zed < rootDirParts.size ();  zed++)
    rootDir << rootDirParts[zed];

}  /* DetermineWhatTheRootDirectoryIs */



const
BinaryClassParmsPtr    TrainingConfiguration2::BinaryClassParms (MLClassConstPtr  class1,
                                                                 MLClassConstPtr  class2
                                                                )  const
{
  const SVMparamPtr  param = SVMparamToUse ();

  if  (!param)
    return NULL;

  return param->GetParamtersToUseFor2ClassCombo (class1, class2);
}  /* BinaryClassParms */




BinaryClassParmsPtr   TrainingConfiguration2::GetBinaryClassParms (MLClassConstPtr  class1,
                                                                   MLClassConstPtr  class2
                                                                  )
{
  const SVMparamPtr  param = SVMparamToUse ();
  if  (!param)
    return NULL;

  return param->GetBinaryClassParms (class1, class2);
}