#include "FirstIncludes.h"

#include <stdio.h>
#include <string>
#include <sstream>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
//		StackTrace	"   at MLL.TrainingProcess2.{ctor}(TrainingProcess2* , KKStr* , FileDesc* , RunLog* , Boolean , Boolean* , KKStr* )   at PicesInterface.TrainingModel2.LoadExistingTrainedModel() in d:\users\kkramer\iec\sippersoftware\picesinterface\trainingmodel2.cpp:line 714"	System::String^


#ifdef  WIN32
#include <ostream>
#include <windows.h>
#else
#include <fstream>
#endif


#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RBTree.h"
#include "Str.h"
using namespace  KKU;


#include "TrainingProcess2.h"
#include "ClassAssignments.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "ImageFeaturesDataIndexed.h"
#include "ImageFeaturesNameIndexed.h"
#include "Model.h"
#include "ModelDual.h"
#include "ModelOldSVM.h"
#include "ModelKnn.h"
#include "ModelSvmBase.h"
#include "TrainingClass.h"
#include "TrainingConfiguration2.h"
using namespace  MLL;


namespace  MLL
{

TrainingProcess2::TrainingProcess2 (const KKStr&         _configFileName,
                                    FeatureVectorListPtr _excludeList,
                                    FileDescPtr          _fileDesc,
                                    RunLog&              _log,
                                    ostream*             _report,
                                    bool                 _forceRebuild,
                                    bool                 _checkForDuplicates,
                                    volatile const bool& _cancelFlag,
                                    KKStr&               _statusMessage
                                   ):
  abort                     (false),
  buildDateTime             (DateTime (1900, 1, 1, 0, 0, 0)),
  cancelFlag                (_cancelFlag),
  config                    (NULL),
  configFileName            (_configFileName),
  configFileNameSpecified   (_configFileName),

  duplicateCount            (0),
  featuresAlreadyNormalized (false),
  trainingExamples          (NULL),
  excludeList               (_excludeList),
  fileDesc                  (_fileDesc),
  mlClasses              (NULL),
  log                       (_log),
  model                     (NULL),
  report                    (_report),
  savedModelName            (),
  statusMessage             (_statusMessage),
  weOwnConfig               (true),
  weOwnTrainingExamples     (true),
  weOwnMLClasses         (true)

{
  log.Level (10) << "TrainingProcess2::TrainingProcess2   9 Parameters." << endl;

  trainingExamples = new FeatureVectorList (_fileDesc, true,  log);

  configFileName = TrainingConfiguration2::GetEffectiveConfigFileName (configFileName);

  savedModelName = osRemoveExtension (configFileName) + ".Save";

  bool      changesMadeToTrainingLibrary = false;
  DateTime  configTimeStamp;
  DateTime  savedModelTimeStamp;
  DateTime  latestTrainingImageTimeStamp;

  bool  useExistingSavedModel = true;
  if  (_forceRebuild)
  {
    useExistingSavedModel = false;
    savedModelTimeStamp = osGetLocalDateTime ();
  }

  else if  (osFileExists (savedModelName))
  {
    savedModelTimeStamp = osGetFileDateTime (savedModelName);
  }

  else
  {
    // Setting 'savedModelTimeStamp' to current time,  so later in function when trying to determine if 
    // it is ok to use saved version, it will fail test and rebuild new model.
    savedModelTimeStamp = osGetLocalDateTime ();
    useExistingSavedModel = false;
  }

  config = new TrainingConfiguration2 (_fileDesc,
                                       configFileName, 
                                       log
                                      );
  if  (!config->FormatGood ())
  {
    if  (report)
    {
      *report << endl << endl
              << "****** Invalid Configuration File[" << configFileName << "]  ******" << endl
              << endl;
    }

    log.Level (-1) << "TrainingProcess2  Invalid Configuration File Specified." << endl;
    Abort (true);
    return;
  }

  mlClasses = config->ExtractClassList ();
  mlClasses->SortByName ();

  configTimeStamp = osGetFileDateTime (configFileName);
  if  (configTimeStamp > savedModelTimeStamp)
    useExistingSavedModel = false;

  cout  << "*PHASE_MSG* Extracting Training Class Data" << endl;

  try
  {ExtractTrainingClassFeatures (latestTrainingImageTimeStamp, changesMadeToTrainingLibrary);}
  catch (std::exception& e1)
  {
    log.Level (-1) << "TrainingProcess2    *** EXCEPTION *** occured calling 'ExtractTrainingClassFeatures'." << endl
                   << "   Exception[" << e1.what () << "]" << endl;
    Abort (true);
  }
  catch (...)
  {
    log.Level (-1) << "TrainingProcess2    *** EXCEPTION *** occured calling 'ExtractTrainingClassFeatures'." << endl;
    Abort (true);
  }
    
  if  (cancelFlag)
    Abort (true);

  if  (Abort ())
  {
    log.Level (-1) << "TrainingProcess2    *** Aborted During Feature Extraction ***" << endl;
    return;
  }

  if  ((latestTrainingImageTimeStamp > savedModelTimeStamp)  ||  (changesMadeToTrainingLibrary))
    useExistingSavedModel = false;

  if  (excludeList)
  {
    useExistingSavedModel = false;

    int32  origSizeOfExamples = trainingExamples->QueueSize ();
    RemoveExcludeListFromTrainingData ();
    if  (origSizeOfExamples != trainingExamples->QueueSize ())
       useExistingSavedModel = false;
  }

  if  (useExistingSavedModel)
  {
    statusMessage = "Loading existing trained model[" + osGetRootNameWithExtension (savedModelName) + "]";

    // We are not going to need the training trainingExamples we loaded so we can go ahead and delete them now.
    delete  trainingExamples;  trainingExamples = NULL;

    buildDateTime = savedModelTimeStamp;  // Set to timestamp of save file for now.  Will be overwriiting by the
                                          // 'BuildDateTime' field in the save file if it exists.

    ifstream  in (savedModelName.Str ());
    if  (!in.is_open ())
    {
      log.Level (-1) << endl << endl 
                     << "TrainingProcess2    *** ERROR ***    Could not existing Training Model[" << savedModelName << "]" << endl
                     << endl;

      useExistingSavedModel = false;
    }
    else
    {
      try
      {
        bool  successful = false;
        Read (in, successful);
        if  (!successful)
        {
          log.Level (-1) << endl << endl << endl
                         << "TrainingProcess2      *** Invalid Formaty in SaveFile[" << savedModelName << "]  we will have to rebuild model." << endl
                         << endl;

          useExistingSavedModel = false;
          delete  model;
          model = NULL;
          // Since we have to build the model anyway we need to reload th etrainingExamples.
          delete  config;              config             = NULL;
          delete  trainingExamples;    trainingExamples   = NULL;
          delete  mlClasses;        mlClasses       = NULL;

          trainingExamples = new FeatureVectorList (_fileDesc, true,  log);

          config = new  TrainingConfiguration2 (_fileDesc,
                                                configFileName, 
                                                log
                                               );

          if  (!config->FormatGood ())
          {
            log.Level (-1) << "TrainingProcess2  Invalid Configuration File Specified." << endl;
            Abort (true);
            return;
          }

          mlClasses = config->ExtractClassList ();
          mlClasses->SortByName ();

          ExtractTrainingClassFeatures (latestTrainingImageTimeStamp, changesMadeToTrainingLibrary);
          if  (cancelFlag)
            Abort (true);
        }

        in.close ();
      }
      catch (std::exception e2)
      {
        log.Level (-1) << "TrainingProcess2    *** EXCEPTION *** occured while processing exisating model." << endl
                       << "   Exception[" << e2.what () << ":]" << endl;
        Abort (true);
      }
      catch (...)
      {
        log.Level (-1) << "TrainingProcess2    *** EXCEPTION *** occured while processing exisating model." << endl;
        Abort (true);
      }
    }
  }

  if  ((!useExistingSavedModel)  &&  (!cancelFlag))
  {
    statusMessage = "Building Support Vector Machine";

    if  (_checkForDuplicates)
      CheckForDuplicates ();

    // trainer->ReportTraningClassStatistics (*report);
    CreateModelsFromTrainingData ();
    if  (Abort ())
    {
      log.Level (-1) << "TrainingProcess2    *** Aborted During Model Creation ***" << endl;
      return;
    }

    if  (!excludeList)
    {
      SaveResults ();
      if  (Abort ())
      {
        log.Level (-1) << "TrainingProcess2    *** Aborted ***" << endl;
        return;
      }
    }
  }

  //  At this point we should no longer need the training trainingExamples.  
  delete  trainingExamples;  trainingExamples = NULL;

  log.Level (20) << "TrainingProcess2::TrainingProcess2  Exiting Constructor" << endl;
}



//*****************************************************************************************
//*   Will build a new model from scratch for the specified class level. Will also remove *
//*   duplicates.                                                                         *
//*****************************************************************************************
TrainingProcess2::TrainingProcess2 (const KKStr&          _configFileName,
                                    FeatureVectorListPtr  _excludeList,
                                    FileDescPtr           _fileDesc,
                                    RunLog&               _log,
                                    uint32                _level,
                                    volatile const bool&  _cancelFlag,
                                    KKStr&                _statusMessage
                                   ):

  abort                     (false),
  buildDateTime             (DateTime (1900,1,1,0, 0, 0)),
  cancelFlag                (_cancelFlag),
  config                    (NULL),
  configFileName            (_configFileName),
  configFileNameSpecified   (_configFileName),

  duplicateCount            (0),
  featuresAlreadyNormalized (false),
  trainingExamples          (NULL),
  excludeList               (_excludeList),
  fileDesc                  (_fileDesc),
  mlClasses              (NULL),
  log                       (_log),
  model                     (NULL),
  report                    (NULL),
  savedModelName            (),
  statusMessage             (_statusMessage),
  weOwnConfig               (true),
  weOwnTrainingExamples     (true),
  weOwnMLClasses         (true)
{
  log.Level (20) << "TrainingProcess2::TrainingProcess2" << endl;

  trainingExamples = new FeatureVectorList (_fileDesc, true,  log);

  configFileName = TrainingConfiguration2::GetEffectiveConfigFileName (configFileName);

  config = new  TrainingConfiguration2 (_fileDesc,
                                        configFileName, 
                                        log
                                       );
  if  (!config->FormatGood ())
  {
    statusMessage = "Configuration file is invalid.";
    log.Level (-1) << "TrainingProcess2  Invalid Configuration File Specified." << endl;
    Abort (true);
    return;
  }

  mlClasses = config->ExtractClassList ();

  {
    DateTime  latestTrainingImageTimeStamp;
    bool      changesMadeToTrainingLibrary;
  
    ExtractTrainingClassFeatures (latestTrainingImageTimeStamp, changesMadeToTrainingLibrary);
    if  (cancelFlag)
      Abort (true);

    if  (Abort ())
    {
      statusMessage = "*** Aborted During Feature Extraction ***";
      log.Level (-1) << "TrainingProcess2    *** Aborted During Feature Extraction ***" << endl;
      return;
    }
  }

  if  (excludeList)
    RemoveExcludeListFromTrainingData ();

  statusMessage = "Building Support Vector Machine";

  CheckForDuplicates ();

  if  (_level < mlClasses->NumHierarchialLevels ())
  {
    // We need to build data that is specific to the spcified _level.
    FeatureVectorListPtr  newExamples = trainingExamples->ExtractExamplesForHierarchyLevel (_level);
    delete  trainingExamples;
    trainingExamples = newExamples;
    newExamples = NULL;

    TrainingConfiguration2Ptr  newConfig = config->GenerateAConfiguraionForAHierarchialLevel (_level);
    delete  config;
    config = newConfig;
    newConfig = NULL;

    delete  mlClasses;
    mlClasses = config->ExtractClassList ();
    mlClasses->SortByName ();
  }

  CreateModelsFromTrainingData ();
  if  (Abort ())
  {
    statusMessage = "*** Aborted During Model Creation ***";
    log.Level (-1) << "TrainingProcess2    *** Aborted During Model Creation ***" << endl;
    return;
  }

  log.Level (20) << "TrainingProcess2::TrainingProcess2  Exiting Constructor" << endl;
}






TrainingProcess2::TrainingProcess2 (const KKStr&          _configFileName,
                                    FileDescPtr           _fileDesc,
                                    RunLog&               _log,
                                    bool                  _featuresAlreadyNormalized,
                                    volatile const bool&  _cancelFlag,
                                    KKStr&                _statusMessage
                                   ):

  abort                     (false),
  buildDateTime             (DateTime (1900,1,1,0, 0, 0)),
  cancelFlag                (_cancelFlag),
  config                    (NULL),
  configFileName            (_configFileName),
  configFileNameSpecified   (_configFileName),

  duplicateCount            (0),
  featuresAlreadyNormalized (_featuresAlreadyNormalized),
  trainingExamples          (NULL),
  excludeList               (NULL),
  fileDesc                  (_fileDesc),
  mlClasses              (new MLClassConstList ()),
  log                       (_log),
  model                     (NULL),
  report                    (NULL),
  savedModelName            (),
  statusMessage             (_statusMessage),
  weOwnConfig               (true),
  weOwnTrainingExamples     (true),
  weOwnMLClasses         (true)

{
  log.Level (20) << "TrainingProcess2::TrainingProcess2     Loading an existing trained model" << endl;

  configFileName = TrainingConfiguration2::GetEffectiveConfigFileName (configFileName);

  savedModelName = osRemoveExtension (configFileName) + ".Save";
  
  KKU::DateTime  savedModelTimeStamp  = osGetFileDateTime (savedModelName);


  ifstream  in (savedModelName.Str ());
  if  (!in.is_open ())
  {
    log.Level (-1) << endl << endl 
                   << "TrainingProcess2    *** ERROR ***  Training Model Save File[" << savedModelName << "] can not be opened." << endl
                   << endl;
    Abort (true);
    return;
  }

  buildDateTime = savedModelTimeStamp;

  bool  successful = false;


  Read (in, successful);

  if  ((!successful)  ||  (model && (!model->ValidModel ())))
  {
    log.Level (-1) << endl << endl 
                   << "TrainingProcess2    *** ERROR ***  Training Model Save File[" << savedModelName << "] Invalid Format." << endl
                   << endl;
    Abort (true);
  }

  in.close ();

  log.Level (20) << "TrainingProcess2::TrainingProcess2(6 parms)   Exiting." << endl;

  statusMessage = "All Done";
  return;
}





TrainingProcess2::TrainingProcess2 (istream&              _in,
                                    FileDescPtr           _fileDesc,
                                    RunLog&               _log,
                                    bool                  _featuresAlreadyNormalized,
                                    volatile const bool&  _cancelFlag,
                                    KKStr&                _statusMessage
                                   ):

  abort                     (false),
  buildDateTime             (DateTime (1900,1,1,0, 0, 0)),
  cancelFlag                (_cancelFlag),
  config                    (NULL),
  configFileName            (),
  configFileNameSpecified   (),

  duplicateCount            (0),
  featuresAlreadyNormalized (_featuresAlreadyNormalized),
  trainingExamples          (NULL),
  excludeList               (NULL),
  fileDesc                  (_fileDesc),
  mlClasses              (new MLClassConstList ()),
  log                       (_log),
  model                     (NULL),
  report                    (NULL),
  savedModelName            (),
  statusMessage             (_statusMessage),
  weOwnConfig               (true),
  weOwnTrainingExamples     (true),
  weOwnMLClasses         (true)

{
  log.Level (20) << "TrainingProcess2::TrainingProcess2     Loading an existing trained model" << endl;

  KKU::DateTime  savedModelTimeStamp  = osGetFileDateTime (savedModelName);

  bool  successful = false;

  Read (_in, successful);

  if  ((!successful)  ||  (model && (!model->ValidModel ())))
  {
    log.Level (-1) << endl << endl 
                   << "TrainingProcess2    *** ERROR ***  Training Model Save File[" << savedModelName << "] Invalid Format." << endl
                   << endl;
    Abort (true);
  }

  log.Level (20) << "TrainingProcess2::TrainingProcess2(6 parms)   Exiting." << endl;

  statusMessage = "All Done";
  return;
}





TrainingProcess2::TrainingProcess2 (TrainingConfiguration2Ptr _config, 
                                    FeatureVectorListPtr      _trainingExamples,
                                    ostream*                  _report,
                                    FileDescPtr               _fileDesc,
                                    RunLog&                   _log,
                                    bool                      _featuresAlreadyNormalized,
                                    volatile const bool&      _cancelFlag,
                                    KKStr&                    _statusMessage
                                   )
:

    abort                     (false),
    buildDateTime             (DateTime (1900,1,1,0, 0, 0)),
    cancelFlag                (_cancelFlag),
    config                    (_config),
    configFileName            (),
    configFileNameSpecified   (),
    duplicateCount            (0),
    trainingExamples          (_trainingExamples),
    excludeList               (NULL),
    fileDesc                  (_fileDesc),
    featuresAlreadyNormalized (_featuresAlreadyNormalized),
    mlClasses              (NULL),
    log                       (_log),
    model                     (NULL),
    report                    (_report),
    savedModelName            (),
    statusMessage             (_statusMessage),
    weOwnConfig               (false),
    weOwnTrainingExamples     (false),
    weOwnMLClasses         (false)

{
  log.Level (20) << "TrainingProcess2::TrainingProcess2" << endl;

  if  (!config)
  {
    log.Level (-1) << "TrainingProcess2  No Configuration File Specified." << endl;
    Abort (true);
  }

  else  if  (!config->FormatGood ())
  {
    log.Level (-1) << "TrainingProcess2  Format error in Configuration File["
                   << config->FileName () << "]."
                   << endl; 
    Abort (true);
  }

  else
  {
    configFileName = config->FileName ();
    configFileNameSpecified = config->ConfigFileNameSpecified ();
    mlClasses = config->ExtractClassList ();
    weOwnMLClasses = true;
  }
}



TrainingProcess2::~TrainingProcess2 ()
{
  log.Level (20) << "TrainingProcess2::~TrainingProcess2  for Config[" << configFileName << "]" << endl;

  try
  {
    delete  model;
    model = NULL;
  }
  catch (...)
  {
    log.Level(-1) << "TrainingProcess2::~TrainingProcess2   Exception deleting model." << endl;
  }
  model = NULL;

  if  (weOwnTrainingExamples)
  {
    delete  trainingExamples;
    trainingExamples  = NULL;
  }

  if  (weOwnMLClasses)
  {
    delete mlClasses;  mlClasses = NULL;
  }

  if  (weOwnConfig)
  {
    delete  config;  config = NULL;
  }
}



int32  TrainingProcess2::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (TrainingProcess2)
    +  configFileName.MemoryConsumedEstimated ()
    +  configFileNameSpecified.MemoryConsumedEstimated ()
    +  savedModelName.MemoryConsumedEstimated ();

  if  (config)
    memoryConsumedEstimated += config->MemoryConsumedEstimated ();

  if  (weOwnTrainingExamples  &&  (trainingExamples != NULL))
    memoryConsumedEstimated += trainingExamples->MemoryConsumedEstimated ();

  if  ((mlClasses != NULL)  &&  weOwnMLClasses)
    memoryConsumedEstimated += mlClasses->MemoryConsumedEstimated ();

  if  (model)
    memoryConsumedEstimated += model->MemoryConsumedEstimated ();

  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



ModelOldSVMPtr  TrainingProcess2::OldSVMModel ()  const
{
  if  (model->ModelType () == Model::mtOldSVM)
    return dynamic_cast<ModelOldSVMPtr> (model);
  else
    return NULL;
}



KKStr  TrainingProcess2::ModelDescription  ()  const
{
  if  (model)
    return model->Description ();
  else
    return "No Model";
}



Model::ModelTypes   TrainingProcess2::ModelType ()  const
{
  if  (model)
    return model->ModelType ();
  else
    return Model::mtNULL;
}



KKStr   TrainingProcess2::ModelTypeStr ()  const
{
  if  (model)
    return model->ModelTypeStr ();
  else
    return "No_Model";
}



SVMModelPtr  TrainingProcess2::Model3 ()
{
  ModelOldSVMPtr  oldSvmModel = OldSVMModel ();
  if  (oldSvmModel)
    return oldSvmModel->SvmModel ();
  else
    return NULL;
}


void  TrainingProcess2::WriteXml (ostream&  o)
{
  log.Level (20) << "TrainingProcess2::WriteXml" << endl;

  bool  successful = true;

  o << "<TrainingProcess2>" << endl;

  o << "ConfigFileName"          << "\t" << configFileName                 << endl;
  o << "ConfigFileNameSpecified" << "\t" << configFileNameSpecified        << endl;

  o << "BuildDateTime"   << "\t" << buildDateTime << std::endl;
  
  o << "ClassList" << "\t" << mlClasses->ToTabDelimitedStr () << std::endl;

  if  (model)
  {
    try
    {
      model->WriteXML (o);
    }
    catch  (KKStrException e)
    {
      log.Level (-1) << endl 
              << "TrainingProcess2::WriteXml   ***ERROR***   Exception SavingModel" << endl
              << "       Exception[" << e.ToString () << "]" << endl
              << endl;
    }
  }

  o << "</TrainingProcess2>" << endl;

  log.Level  (20) << "TrainingProcess2::SaveResults   Exiting" << endl;
}  /* WriteXml */



void  TrainingProcess2::Read (istream&  in,
                              bool&     successful
                             )

{
  log.Level (20) << "TrainingProcess2::Read" << endl;

  successful = true;
 
  delete  mlClasses;
  mlClasses  = NULL;
  
  char  buff[50000];
  memset (buff, 0, sizeof (buff));
  while  (in.getline (buff, sizeof (buff) - 1))
  {
    buff[sizeof (buff) - 1] = 0;
    KKStr  line (buff);
    line.TrimRight ();
    KKStr  lineName = line.ExtractToken2 ("\t");
    lineName.Upper ();
    lineName.TrimLeft ();
    lineName.TrimRight ();

    if  (lineName.EqualIgnoreCase ("<TrainingProcess2>"))
      continue;

    else if  (lineName.EqualIgnoreCase ("</TrainingProcess2>"))
      break;

    else if  (lineName.EqualIgnoreCase ("<SVMModel>"))
    {
      // We are looking at an old 'Save' file format.  We need to rebuild a new model and save it.
      log.Level (-1) << endl << endl 
        << "TrainingProcess2::Read   ***WARNING***   Save file is of Old Format;  it will need to be rebuilt."  << endl
        << endl;
     successful = false;
     break;
    }

    else if  (lineName == "BUILDDATETIME")
    {
      line.TrimRight (" \n\r\t");
      buildDateTime = DateTime (line);
    }

    else if  (lineName.EqualIgnoreCase ("ConfigFileName"))
      configFileName = line.ExtractToken2 ("\n\t\r");

    else if  (lineName.EqualIgnoreCase ("ConfigFileNameSpecified"))
      configFileNameSpecified = line.ExtractToken2 ("\n\t\r");
    
    else if  (lineName == "CLASSLIST")
    {
      log.Level (20) << "TrainingProcess2::Read    Classes[" << line << "]" << endl;
      delete  mlClasses;

      mlClasses = new MLClassConstList ();
      weOwnMLClasses = true;
      while  (!line.Empty ())
      {
        KKStr  className = line.ExtractToken2 ("\t\n\r");
        MLClassPtr  c = MLClass::CreateNewMLClass (className);
        mlClasses->PushOnBack (c);
      }
    }

    else if  (lineName.EqualIgnoreCase ("<Model>"))
    {
      try
      {
        model = Model::CreateFromStream (in, fileDesc, cancelFlag, log);
        if  (model == NULL)
        {
          successful = false;
          log.Level (-1) << endl << endl
            << "TrainingProcess2::Read   ***ERROR***  Could not create model from stream." << endl
            << endl;
        }
      }
      catch  (const KKStrException&  e)
      {
        successful = false;
        log.Level (-1) << endl << endl
          << "TrainingProcess2::Read   ***ERROR***  Exception occurecd reading from file." << endl
          << "     Exception[" << e.ToString () << "]." << endl
          << endl;
      }
      catch  (...)
      {
        successful = false;
        log.Level (-1) << endl << endl
          << "TrainingProcess2::Read   ***ERROR***  Exception occurecd reading from file." << endl
          << endl;
      }
    }
  }

  if  (successful)
  {
    if  (mlClasses == NULL)
    {
      log.Level (-1) << endl
                     << endl
                     << "    **** ERROR ****" << endl
                     << endl
                     << "TrainingProcess2::Read  - ClassList were not defined." << endl
                     << endl;
      successful = false;
    }
  }

  log.Level (20) << "TrainingProcess2::Read    Done Reading in existing model." << endl;
}  /* read */




void  TrainingProcess2::RemoveExcludeListFromTrainingData ()
{
  ImageFeaturesDataIndexed dataIndex (*excludeList);
  ImageFeaturesNameIndexed nameIndex (*excludeList);

  FeatureVectorList  trainingExamplesToDelete 
    (fileDesc, 
     false,    /* owner = false */
     log, 
     100       /* estimated size */
    );

  FeatureVectorList::iterator idx;
  for  (idx = trainingExamples->begin ();  idx != trainingExamples->end ();  idx++)
  {
    FeatureVectorPtr  example = *idx;

    KKStr  rootImageFileName = osGetRootName (example->ImageFileName ());
    if  (nameIndex.GetEqual (rootImageFileName) != NULL)
    {
      // Need to remove this example from training data.
      trainingExamplesToDelete.PushOnBack (example);
    }

    else if  (dataIndex.GetEqual (example))
    {
      // Need to remove this example from training data.
      trainingExamplesToDelete.PushOnBack (example);
    }
  }


  if  (trainingExamplesToDelete.QueueSize () == 0)
  {
    if  (report)
    {
      *report << endl << endl << endl
              << "    Images Being Classifed Removed From Training Data"      << endl
              << "==========================================================" << endl
              << "No trainingExamples from source directory detected in training data." << endl
              << endl << endl;
    }
  }
  else
  {
    if  (report)
    {
      *report << endl << endl << endl
              << "Images Being Classifed Removed From Training Data" << endl
              << "=================================================" << endl;

      trainingExamplesToDelete.PrintClassStatistics (*report);

      *report << endl;
    }

    for  (idx = trainingExamplesToDelete.begin ();  idx != trainingExamplesToDelete.end ();  idx++)
    {
      FeatureVectorPtr  example = *idx;

      if  (report)
        *report << example->ImageFileName () << "\t" << example->MLClassName () << endl;

      trainingExamples->DeleteEntry (example);
    }
    if  (report)
      *report << endl << endl << endl;
  }

} /* RemoveExcludeListFromTrainingData */




void  TrainingProcess2::CheckForDuplicates ()
{
  // Lets check for duplicate trainingExamples in training data.  Just to get a count, no other reason
  DuplicateImages  dupDetector (trainingExamples, log);
  duplicateCount = dupDetector.DuplicateCount ();
  if  (duplicateCount > 0)
  {
    log.Level (-1) << endl << endl
                   << "ConstructOneVsOneModel        *** WARNING ***   Dupliactes Detected in Training Data" << endl
                   << "                              DuplicateCount[" << duplicateCount << "]" << endl
                   << endl;

    if  (report)
      dupDetector.ReportDuplicates (*report);

    dupDetector.PurgeDuplicates (trainingExamples, NULL);
  }
}  /* CheckForDuplicates */




void  TrainingProcess2::ExtractFeatures (const TrainingClassPtr  trainingClass,
                                         DateTime&               latestTimeStamp,
                                         bool&                   changesMade
                                        )
{
  KKStr  expDirName = trainingClass->ExpandedDirectory (config->RootDir ());

  log.Level (30) << "TrainingProcess2::ExtractFeatures - Extracting Features from Directory["
                 << expDirName                          << "],  into file["
                 << trainingClass->FeatureFileName   () << "]."
                 << endl;

  KKStr  featureFileName = osMakeFullFileName (expDirName, trainingClass->FeatureFileName ());

  statusMessage = "Extracting features for class[" + trainingClass->MLClass ()->Name () + "].";

  FeatureVectorListPtr  extractedImages = FeatureFileIOPices::FeatureDataReSink 
                                (expDirName,
                                 trainingClass->FeatureFileName (),
                                 trainingClass->MLClass (),
                                 true,   //  Make all entries in this directory 'trainingClass->MLClass ()'
                                 *mlClasses,
                                 cancelFlag,
                                 changesMade,
                                 latestTimeStamp,
                                 log
                                );
  //  We will use list from ReSink  to add to trainingExamples


  if  ((extractedImages == NULL)  ||  (extractedImages->QueueSize () < 1))
  {
    log.Level (-1) << endl
                   << "ExtractFeatures    *** No Training Examples Found ***"   
                   << "                 Class    [" << trainingClass->Name () << "]"
                   << "                 Directory[" << expDirName             << "]"
                   << endl;
    Abort (true);
  }

  trainingExamples->AddQueue (*extractedImages);
  extractedImages->Owner (false);
  delete  extractedImages;
  extractedImages = NULL;

  log.Level (30) << "TrainingProcess2::ExtractFeatures  Exiting" << endl;
}  /* ExtractFeatures */




void  TrainingProcess2::ExtractTrainingClassFeatures (DateTime&  latestImageTimeStamp,
                                                      bool&      changesMadeToTrainingLibraries
                                                     )
{
  log.Level (20) << "TrainingProcess2::ExtractTraingClassFeatures - Starting." << endl;

  changesMadeToTrainingLibraries = false;

  //****************************************************************************
  // Make sure that there are no existing *.data files that we are going to use.
  // We need to do this in a seperate pass because more than one entry may refer 
  // to the same Class and hense the same *.data file.


  //***********************************************************
  //  We will first extract the Raw features from each Class 

  const TrainingClassList&   trainingClasses = config->TrainingClasses ();
  TrainingClassList::const_iterator  tcIDX;

  DateTime  latestTimeStamp;
  bool      changesMadeToThisTrainingClass;

  for  (tcIDX = trainingClasses.begin ();  ((tcIDX != trainingClasses.end ())  &&  (!cancelFlag)  &&  (!Abort ()));  tcIDX++)
  {
    const TrainingClassPtr trainingClass = *tcIDX;

    log.Level (20) << "TrainingProcess2::ExtractTraingClassFeatures Starting on Class[" 
                   << trainingClass->Name () << "]."
                   << endl;

    ExtractFeatures (trainingClass, latestTimeStamp, changesMadeToThisTrainingClass);
    if  (latestTimeStamp > latestImageTimeStamp)
      latestImageTimeStamp = latestTimeStamp;

    if  (changesMadeToThisTrainingClass)
      changesMadeToTrainingLibraries = true;
  }

  // DONE Extracting Raw features from All classes.
  if  ((!Abort ())  &&  (!cancelFlag))
  {
    if  (config->NoiseTrainingClass ())
    {
      cout  << "*PHASE_MSG2* Training Class[" << config->NoiseTrainingClass ()->Directory () << "]" << endl;
      ExtractFeatures (config->NoiseTrainingClass (), latestTimeStamp, changesMadeToThisTrainingClass);
      if  (latestTimeStamp > latestImageTimeStamp)
        latestImageTimeStamp = latestTimeStamp;

      if  (changesMadeToThisTrainingClass)
        changesMadeToTrainingLibraries = true;
    }
  }

  trainingExamples->RandomizeOrder ();

  log.Level (20) << "TrainingProcess2::ExtractTrainingClassFeatures   Exiting" << endl;
}  /*  ExtractTrainingClassFeatures */





void  TrainingProcess2::ReportTraningClassStatistics (ostream&  report)
{
  report << endl
         << "Training Class Statistics" << endl
         << endl;
  trainingExamples->PrintClassStatistics (report);
}  /* ReportTraningClassStatistics */




void  TrainingProcess2::AddImagesToTrainingLibray (FeatureVectorList&  trainingImages,
                                                   FeatureVectorList&  imagesToAdd
                                                  )
{
  int32   idx        = 0;

  int32  numOfImages = imagesToAdd.QueueSize ();

  FeatureVectorPtr  image = NULL;

  for  (idx = 0;  idx < numOfImages;  idx++)
  {
    image = imagesToAdd.IdxToPtr (idx);

    if  (image->FeatureDataValid ())
    {
      trainingImages.PushOnBack (image);
    }
    else
    {
      log.Level (-1) << endl << endl
          << "TrainingProcess2::AddImagesToTrainingLibray    ***ERROR***,  Image["  << image->ImageFileName () << "], Class[" << image->ClassName () << "]"
          << "  Has Invalid Feature Data."
          << endl << endl;

      if  (report)
      {
        *report << "** ERROR **,  Image["  << image->ImageFileName () << "], Class[" << image->ClassName () << "]"
                << "  Has Invalid Feature Data."
                << endl;
      }
    }
  }
}  /* AddImagesToTrainingLibray */



void  TrainingProcess2::CreateModelsFromTrainingData ()
{
  log.Level (20) << "TrainingProcess2::CreateModelsFromTrainingData    Starting" << endl;

  if  (config->ModelParameters () == NULL)
  {
    log.Level (-1) << endl 
                   << "TrainingProcess2::CreateModelsFromTrainingData    ***ERROR***"  << endl
                   << endl
                   << "                   Parameters not specified in configuration file." << endl
                   << endl;
    Abort (true);
    return;
  }

  // Will Create the Appropriate Model class given 'config->ModelingMethod ()'.
  model = Model::CreateAModel (config->ModelingMethod (), 
                               osGetRootName (configFileName),
                               *(config->ModelParameters ()), 
                               fileDesc, 
                               cancelFlag, 
                               log
                              );

  try
  {
    model->TrainModel (trainingExamples, 
                       featuresAlreadyNormalized,
                       false     // false = 'model' We are not giving ownership of TrainingExdamples to model.
                      );
  }
  catch  (const KKStrException&  e)
  {
    log.Level (-1) << endl << endl
      << "TrainingProcess2::CreateModelsFromTrainingData   ***ERROR***   Exception occurecd while running 'Model::TrainModel'." << endl
      << "     Exception[" << e.ToString () << endl
      << endl;
    Abort (true);
  }
  catch  (...)
  {
    log.Level (-1) << endl << endl
      << "TrainingProcess2::CreateModelsFromTrainingData   ***ERROR***   Exception occurecd while running 'Model::TrainModel'." << endl
      << endl;
    Abort (true);
  }

  if  (!model->ValidModel ())
  {
    Abort (true);
  }

  //trainingExamples->Owner (false);
  buildDateTime = osGetLocalDateTime ();
  log.Level (20) << "TrainingProcess2::CreateModelsFromTrainingData    Ending" << endl;
} /* CreateModelsFromTrainingData */





//****************************************************************************
//*                                                                          *
//****************************************************************************
void  TrainingProcess2::SaveResults ()
{
  log.Level (20) << "TrainingProcess2::SaveResults" << endl;

  //KKStr  baseModelName = osRemoveExtension (configFileName) + ".Save";

  ofstream o (savedModelName.Str ());

  if  (model)
    model->RootFileName (osGetRootName (this->ConfigFileName ()));

  WriteXml (o);

  o.close ();

  log.Level  (20) << "TrainingProcess2::SaveResults   Exiting" << endl;
}  /* SaveResults */



ModelParamPtr  TrainingProcess2::Parameters () const
{
  if  (!model)
    return NULL;
  else
    return model->Param ();
}  /* Parameters */




int32  TrainingProcess2::NumOfSupportVectors ()  const
{
  ModelOldSVMPtr  oldSvmModel = OldSVMModel ();
  if  (oldSvmModel)
    return oldSvmModel->NumOfSupportVectors ();
  else
    return 0;
}



void  TrainingProcess2::SupportVectorStatistics (int32&  numSVs,
                                                 int32&  totalNumSVs
                                                )
{
  numSVs = 0;
  totalNumSVs = 0;
  ModelOldSVMPtr  oldSvmModel = OldSVMModel ();
  if  (oldSvmModel)
    oldSvmModel->SupportVectorStatistics (numSVs, totalNumSVs);
}




TrainingProcess2Ptr   TrainingProcess2::TrainingProcessLeft ()
{
  if  ((!model)  ||  (model->ModelType () != Model::mtDual))
    return NULL;
  return  dynamic_cast<ModelDualPtr>(model)->Trainer1 ();
}



TrainingProcess2Ptr   TrainingProcess2::TrainingProcessRight ()
{
  if  ((!model)  ||  (model->ModelType () != Model::mtDual))
    return NULL;
  return  dynamic_cast<ModelDualPtr>(model)->Trainer2 ();
}






double TrainingProcess2::ReductionTime()  const
{
 if (model)
   return model->ReductionTime();
 else
   return 0.0;
}


const KKStr&  TrainingProcess2::BitReductionByFeature () const
{
  if  (model)
    return  model->BitReductionByFeature ();
  else
    return  KKStr::EmptyStr ();
}



const CompressionStats&  TrainingProcess2::CompStats ()  const
{
  return  (model->Compression_Stats ());
}



int32 TrainingProcess2::ReductionPreExampleCount()  const
{
  if (model)
    return model->ReductionPreExampleCount();
  else
    return 0;
}



int32 TrainingProcess2::ReductionPostExampleCount()  const
{
  if  (model)
    return model->ReductionPostExampleCount();
  else
    return 0;
}



float TrainingProcess2::ReductionRatio()  const
{
  if  (model)
    return model->ReductionRatio();
  else
    return 0.0f;
}



double   TrainingProcess2::TrainingTime ()  const
{
  if (model)
    return model->TrainingTime();
  else
    return 0.0;
}



int32  TrainingProcess2::DuplicateDataCount () const
{
  return  duplicateCount;
}



/**
 @brief If there is a config file; will return a list of its FormatErrors ().  
 */
VectorKKStr  TrainingProcess2::ConfigFileFormatErrors ()  const
{
  if  (config)
    return config->FormatErrorsWithLineNumbers ();
  else
    return VectorKKStr ();
}  /* ConfigFileFormatErrors */


}  /* MLL */
