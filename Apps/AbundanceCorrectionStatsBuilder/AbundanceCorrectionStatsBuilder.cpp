#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <math.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"
using namespace std;

#include "BasicTypes.h"
#include "Compressor.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;

#include "InstrumentDataFileManager.h"
#include "SipperVariables.h"
using namespace SipperHardware;

#include "AbundanceCorrectionMatrix.h"
#include "ConfusionMatrix2.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureVector.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "ImageFeatures.h"
using namespace MLL;


#include "AbundanceCorrectionStatsBuilder.h"
#include "TrainTestThread.h"

using namespace  AbundanceCorrectionApplication;


AbundanceCorrectionStatsBuilder::AbundanceCorrectionStatsBuilder (int     argc, 
                                                                  char**  argv
                                                                 ):

   Application (argc, argv),
   allClasses              (NULL),
   config                  (NULL),
   configClasses           (NULL),
   fileDesc                (NULL),
   maxNumActiveThreads     (1),
   msgQueue                (NULL),
   normalizationParms      (NULL),
   numOfFolds              (10),
   oneOrMoreThreadsCrashed (false),
   otherClass              (NULL),
   otherClassData          (NULL),
   queueDone               (NULL),
   queueReady              (NULL),
   queueRunning            (NULL),
   report                  (NULL),
   reportFileName          (),
   trainLibData            (NULL),
   trainLibDataClasses     (NULL)
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

  maxNumActiveThreads = Min (6, osGetNumberOfProcessors ());

  //maxNumActiveThreads = 1;  // KKKK  For debugging only.


  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  InstrumentDataFileManager::InitializePush ();
  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

  queueDone    = new TrainTestThreadList (true);
  queueReady   = new TrainTestThreadList (true);
  queueRunning = new TrainTestThreadList (true);

}



AbundanceCorrectionStatsBuilder::~AbundanceCorrectionStatsBuilder ()
{
  if  (!reportFileName.Empty ())
    delete  report;
  report = NULL;

  delete allClasses;           allClasses          = NULL;
  delete trainLibData;         trainLibData        = NULL;
  delete otherClassData;       otherClassData      = NULL;
  delete config;               config              = NULL;
  delete trainLibDataClasses;  trainLibDataClasses = NULL;
  delete configClasses;        configClasses       = NULL;
  delete normalizationParms;   normalizationParms  = NULL;
  delete msgQueue;             msgQueue            = NULL;
  delete queueDone;            queueDone           = NULL;
  delete queueReady;           queueReady          = NULL;
  delete queueRunning;         queueRunning        = NULL;

  InstrumentDataFileManager::InitializePop ();
}




bool  AbundanceCorrectionStatsBuilder::ProcessCmdLineParameter (char   parmSwitchCode, 
                                                                KKStr  parmSwitch, 
                                                                KKStr  parmValue
                                                               )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  if  (parmSwitch.EqualIgnoreCase ("-c")  ||  parmSwitch.EqualIgnoreCase ("-config"))
  {
    configFileName = parmValue;
    configFileFullPath = TrainingConfiguration2::GetEffectiveConfigFileName (configFileName);
    if  (!osFileExists (configFileFullPath))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Invalid '-config' [" << configFileName << "] file." << endl;
      Abort (true);
    }
  }

  else if  (parmSwitch.EqualIgnoreCase ("-folds")  ||  parmSwitch.EqualIgnoreCase ("-numFolds")  ||  parmSwitch.EqualIgnoreCase ("-numOfFolds"))
  {
    numOfFolds = parmValue.ToInt ();
    if  ((numOfFolds < 2)  ||  (numOfFolds > 1000))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Invalid '-NumFolds' [" << numOfFolds << "] Invlaid;  valid values (2 - 1000)." << endl;
      Abort (true);
    }
  }

  else if  (parmSwitch.EqualIgnoreCase ("-r")  ||  parmSwitch.EqualIgnoreCase ("-report")  ||  parmSwitch.EqualIgnoreCase ("-ReportFileName"))
    reportFileName = parmValue;

	return  !Abort ();
}  /* ProcessCmdLineParameter */




/******************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   AbundanceCorrectionStatsBuilder::DisplayCommandLineParameters ()
{
  log.Level (0) << "AbundanceCorrectionStatsBuilder"                           << endl;
  log.Level (0)                                                                << endl;
  log.Level (0) << "    -Config   <Configuration File>"                        << endl;
  log.Level (0)                                                                << endl;
  log.Level (0) << "    -folds  <number of folds>  Will default to 10."        << endl;
  log.Level (0)                                                                << endl;
  log.Level (0) << "    -Report <Report File Name>  defaults to cout."         << endl;
  log.Level (0)                                                                << endl;
}  /* DisplayCommandLineParameters */

// -C GulfOilBroad2_Discreate_Dual  -folds 2  -Report C:\Temp\GulfOilBroad2_Discreate_Dual_AbundanceReport.txt

// -c NRDA_small_DWH_Dual -folds 10 -report  C:\Temp\NRDA_small_DWH_Dual_AbindanceAdg.txt



void   AbundanceCorrectionStatsBuilder::PrintComandLineParameters ()
{
  *report << "ConfigFileName"      << "\t" << configFileName        << endl
          << "ConfigFileFullPath"  << "\t" << configFileFullPath    << endl
          << "NumOfFolds"          << "\t" << numOfFolds            << endl
          << "ReportFileName"      << "\t" << reportFileName        << endl
          << "RunDateTime"         << "\t" << osGetLocalDateTime () << endl
          << endl;
}


void   AbundanceCorrectionStatsBuilder::PrintStartStatistics ()
{
  if  (trainLibData)
  {
    *report << endl << endl << "Traing Library Stats" << endl;
    trainLibData->PrintClassStatistics (*report);
  }

  if  (otherClassData)
  {
    *report << endl << endl << "Other Class Stats" << endl;
    otherClassData->PrintClassStatistics (*report);
  }
}  /* PrintStartStatistics */



void  AbundanceCorrectionStatsBuilder::RemoveDuplicateImages ()
{
  *report << endl << endl;

  FeatureVectorListPtr  allExamples = new FeatureVectorList (fileDesc, 
                                                             false,    // 'false' = will not own contents.
                                                             log
                                                            ); 
  allExamples->AddQueue (*trainLibData);
  allExamples->AddQueue (*otherClassData);
  
  DuplicateImages  dupChecker (allExamples, log);
  if  (dupChecker.DuplicatesFound ())
  {
    *report << "DUPLICATE IMAGES DETECTED." << endl;
    dupChecker.ReportDuplicates (*report);
    
    FeatureVectorListPtr  dups = dupChecker.ListOfExamplesToDelete ();
    if  (dups)
    {
      FeatureVectorList::iterator  idx;
      for  (idx = dups->begin ();  idx != dups->end ();  ++idx)
      {
        FeatureVectorPtr  fv = *idx;
        trainLibData->DeleteEntry (fv);
        otherClassData->DeleteEntry (fv);
      }
      delete  dups;
      dups = NULL;
    }
  }
  else
  {
    *report << "No duplicates detected." << endl;
  }

  delete  allExamples;  allExamples = NULL;

  *report << endl << endl;
}  /* RemoveDuplicateImages */



void   AbundanceCorrectionStatsBuilder::TerminateThreads ()
{
  TrainTestThreadList::iterator  idx;

  log.Level (10) << "TerminateThreads   Flagging all running threads to termnate." << endl;
  for  (idx = queueRunning->begin ();  idx != queueRunning->end ();  ++idx)
  {
    TrainTestThreadPtr  thread = *idx;
    thread->CancelThread ();
  }

  int  loopCount = 0;
  int  numThreadsRunning = 0;
  do
  {
    osSleep (0.1f);
    numThreadsRunning = 0;
    for  (idx = queueRunning->begin ();  idx != queueRunning->end ();  ++idx)
    {
      TrainTestThreadPtr  thread = *idx;
      if  ((thread->Status () != tsStopped)  &&  (thread->Status () != tsNotStarted))
        ++numThreadsRunning;
    }

    if  ((loopCount  % 10) == 0)
      log.Level (10) << "TerminateThreads   [" << numThreadsRunning << "]  Threads are still running." << endl;

    ++loopCount;
  }  while  (numThreadsRunning > 0);
}  /* TerminateThreads */



void   AbundanceCorrectionStatsBuilder::ManageThreads ()
{
  uint32  loopCount = 0;
  bool  terminate = false;
  while  (!terminate)
  {
    {
      KKStrPtr  msg = msgQueue->GetNextMsg ();
      while  (msg)
      {
        log.Level (10) << *msg << endl;
        msg = msgQueue->GetNextMsg ();
      }
    }

    if  ((queueReady->QueueSize () < 1)  &&  (queueRunning->QueueSize () < 1))
    {
     terminate = true;
     break;
    }

    {
      // Move any completed threads from 'QueueRunning'  to  'QueueDone'
      TrainTestThreadList::iterator  idx = queueRunning->begin ();
      while  (idx != queueRunning->end ())
      {
        TrainTestThreadPtr thread = *idx;

        if  (thread->Crashed ())
        {
          oneOrMoreThreadsCrashed = true;
          log.Level (10) << endl << endl
            << "ManageThreads    ***ERROR***      Thresad[" << thread->Desc () << "]   Crashed."  << endl
            << endl;
        }

        if  (thread->Status () == tsStopped)
        {
          idx = queueRunning->erase (idx);
          queueDone->PushOnBack (thread);
        }
        else
        {
          ++idx;
        }
      }
    }

    if  (oneOrMoreThreadsCrashed)
    {
      terminate = true;
      log.Level (10) << "ManageThreads    ***ERROR***   One or more threads crashed;   terminating application." << endl
        << endl;
      TerminateThreads ();
    }

    while  ((queueRunning->QueueSize () < maxNumActiveThreads)  &&  (queueReady->QueueSize () > 0))
    {
      TrainTestThreadPtr  nextThreadToStart = queueReady->PopFromFront ();
      if  (nextThreadToStart != NULL)
      {
        log.Level (10) << "ManageThreads   Starting thread[" << nextThreadToStart->ThreadName () << "]" << endl;
        queueRunning->PushOnBack (nextThreadToStart);
        nextThreadToStart->Start ();
      }
    }

    if  ((loopCount  % 20) == 0)
    {
      cout << "Ready:"   << queueReady->QueueSize ()   << "  "
           << "Running:" << queueRunning->QueueSize () << "  "
           << "Done:"    << queueDone->QueueSize ()    << "  "
           << endl;
    }

    osSleep (0.5f);
    ++loopCount;
  }
}  /* ManageThreads */



void   AbundanceCorrectionStatsBuilder::CreateInitialThreadInstaces ()
{
  log.Level (10) << "AbundanceCorrectionStatsBuilder::CreateInitialThreadInstaces"  << endl;
  
  FeatureVectorListPtr  stratifiedTrainData = trainLibData->StratifyAmoungstClasses (numOfFolds);
  FeatureVectorListPtr  stratifiedOtherData = otherClassData->StratifyAmoungstClasses (numOfFolds);

  int32  numTrainExamples = stratifiedTrainData->QueueSize ();
  int32  numOtherExamples = stratifiedOtherData->QueueSize ();

  msgQueue = new MsgQueue ("AbundanceCorrectionStatsBuilder");

  int32  lastFvInFold = -1;
  int32  firstFvInFold = 0;

  int32  firstOtherFvInFold = 0;
  int32  lastOtherFvInFold = -1;

  for  (int32  foldNum = 0;  foldNum < numOfFolds;  ++foldNum)
  {
    firstFvInFold = lastFvInFold + 1;
    lastFvInFold  = (numTrainExamples * (foldNum + 1) / numOfFolds) - 1;

    firstOtherFvInFold = lastOtherFvInFold + 1;
    lastOtherFvInFold  = (numOtherExamples * (foldNum + 1) / numOfFolds) - 1;

    FeatureVectorListPtr  trainData = new FeatureVectorList (fileDesc, false, log);
    FeatureVectorListPtr  testData  = new FeatureVectorList (fileDesc, false, log);

    for  (int32 idx = 0;  idx < numTrainExamples;  ++idx)
    {
      FeatureVectorPtr fv = stratifiedTrainData->IdxToPtr (idx);
      if  ((idx >= firstFvInFold)  &&  (idx <= lastFvInFold))
        testData->PushOnBack (fv);
      else
        trainData->PushOnBack (fv);
    }

    // Add OtherClass exampes to test data.
    for  (int32 idx = firstOtherFvInFold;  idx <= lastOtherFvInFold;  ++idx)
    {
      FeatureVectorPtr fv = stratifiedOtherData->IdxToPtr (idx);
      testData->PushOnBack (fv);
    }

    RunLogPtr  threadRunLog = new RunLog ();
    threadRunLog->AttachMsgQueue (msgQueue);

    KKStr  threadName = "AbundanceCorrFold" + StrFormatInt (foldNum, "00");
    TrainTestThreadPtr  thread = new TrainTestThread 
               ("Fold_" + StrFormatInt (foldNum, "00"),
                this,
                config,
                allClasses,
                trainData,            // Will take ownesrship and delete in its destructor.
                trainLibDataClasses,
                testData,             // Will take ownesrship and delete in its destructor.
                otherClass,
                threadName,
                msgQueue,             // Will take ownesrship and delete in its destructor.
                threadRunLog
               );

    queueReady->PushOnBack (thread);
  }

  delete  stratifiedOtherData;  stratifiedOtherData = NULL;
  delete  stratifiedTrainData;  stratifiedTrainData = NULL;
}  /* CreateInitialThreadInstaces */



void   AbundanceCorrectionStatsBuilder::GenerateCrashReport ()
{
  *report << endl << endl
    << "ONE OR MORE THREADS CRASHED"  << endl
    << "==========================="  << endl
    << endl
    << "Report and statistics were not generated." << endl
    << endl;
}  /* GenerateCrashReport */




void   AbundanceCorrectionStatsBuilder::GenerateReportAndStats ()
{
  AbundanceCorrectionMatrix  abundanceCorrMatrix (*trainLibDataClasses, otherClass, log);
  ConfusionMatrix2           confusionMatrix (*allClasses);

  TrainTestThreadList::iterator  idx;
  for  (idx = queueDone->begin ();  idx != queueDone->end ();  ++idx)
  {
    TrainTestThreadPtr  thread = *idx;

    const AbundanceCorrectionMatrixPtr  acm = thread->AbundanceCorMatrix ();
    const ConfusionMatrix2Ptr           cm  = thread->ConfusionMatrix ();

    if  (acm)  abundanceCorrMatrix.AddIn (*acm, log);
    if  (cm)   confusionMatrix.AddIn     (*cm, log);
  }

  *report << endl << endl << endl;
  abundanceCorrMatrix.PrintConfusionMatrixTabDelimited (*report);

  *report << endl << endl << endl << endl;
  confusionMatrix.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl << endl;

  abundanceCorrMatrix.SaveForConfigFileName (configFileName);
}  /* GenerateReportAndStats */




void   AbundanceCorrectionStatsBuilder::Main ()
{
  if  (Abort ())
    return;

  if  (reportFileName.Empty ())
  {
    DateTime  d = osGetLocalDateTime ();
    KKStr  reportDir = osAddSlash (SipperVariables::PicesReportDir ()) + "AbundanceAdjustments";
    osCreateDirectoryPath (reportDir);
    if  (configFileName.Empty ())
      reportFileName = osAddSlash (reportDir) + "NoConfigFile" + "_" + d.YYYYMMDDHHMMSS () + ".txt";
    else
      reportFileName = osAddSlash (reportDir) + osGetRootName (configFileName) + "_" + d.YYYYMMDDHHMMSS () + ".txt";
  }
 
  report = new ofstream (reportFileName.Str ());
  PrintComandLineParameters ();

  if  (configFileName.Empty ())
  {
    log.Level (-1) << endl << endl 
      << "AbundanceCorrectionStatsBuilder::Main   ***ERROR***    Configuration File was not specified." << endl
      << endl;
    Abort (true);
    *report << endl << "*** NO CONFIGURATION FILE SPECIFIED ***" << endl << endl;
    return;
  }

  delete  config;
  config = new TrainingConfiguration2 (fileDesc, 
                                       configFileFullPath, 
                                       log,
                                       true    /**<  'true' = validateDirectories. */
                                      );

  if  (!config->FormatGood ())
  {
    log.Level (-1) << endl
      << "AbundanceCorrectionStatsBuilder::Main   Config[" << configFileName << "] has invalid format." << endl
      << endl;

    VectorKKStr  errors = config->FormatErrorsWithLineNumbers ();
    VectorKKStr::const_iterator  idx;
    log.Level (-1) << endl;
    for  (idx = errors.begin ();  idx != errors.end ();  ++idx)
      log.Level (-1) << (*idx) << endl;
    log.Level (-1) << endl << endl;

    *report << endl << endl << "***   Configuratiuon file[" << configFileName << " contains formatting errors." << endl << endl;

    config->PrintFormatErrors (*report);

    return;
  }

  bool      changesMadeToTrainingLibraries = false;
  bool      cancelFlag = false;
  DateTime  latestImageTimeStamp;

  delete  trainLibData;
  trainLibData = config->LoadFeatureDataFromTrainingLibraries (latestImageTimeStamp, changesMadeToTrainingLibraries, cancelFlag);
  if  (!trainLibData)
  {
    log.Level (-1) << endl
      << "AbundanceCorrectionStatsBuilder::Main   ***ERROR***    No training data was loaded." << endl
      << endl;

    *report << endl << "*** Failed to load training data ***" << endl;
    return;
  }

  otherClass = config->OtherClass ();
  if  (!otherClass)
    otherClass = MLClass::CreateNewMLClass ("Other", -1);

  configClasses = config->ExtractClassList ();
  configClasses->SortByName ();
  if  (configClasses->PtrToIdx (otherClass) >= 0)
  {
    log.Level (-1) << endl
      << "AbundanceCorrectionStatsBuilder::Main   ***ERROR***   OtherClass[" << otherClass->Name () << "] is specified as a Training Class; it must be swepcified separatly." << endl
      << endl;
    *report << endl << "*** Failed to load other class data ***" << endl;
    return;
  }

  trainLibDataClasses = trainLibData->ExtractMLClassConstList ();
  trainLibDataClasses->SortByName ();

  if  ((*configClasses) != (*trainLibDataClasses))
  {
    Abort (true);
    log.Level (-1) << endl 
      << "AbundanceCorrectionStatsBuilder::Main   ***ERROR***   Class make up of training data does not correspond to configuration file." << endl
      << endl;

    *report << endl << "*** Training data contains different classes that Confg File  ***" << endl;
    return;
  }

  allClasses = new MLClassConstList (*trainLibDataClasses);
  allClasses->PushOnBack (otherClass);

  otherClassData = config->LoadOtherClasssExamples ();
  if  ((!otherClassData)  ||  (otherClassData->QueueSize () < 1))
  {
    Abort (true);
    log.Level (-1) << endl 
      << "AbundanceCorrectionStatsBuilder::Main   ***ERROR***   No Other[" << otherClass->Name () << "] class examples found in training library." << endl
      << endl;

    *report << endl << "***  No other class data loaded. ***" << endl;
    return;
  }

  PrintStartStatistics ();

  RemoveDuplicateImages ();

  normalizationParms = new NormalizationParms (config, *trainLibData, log);
  normalizationParms->NormalizeImages (trainLibData);
  normalizationParms->NormalizeImages (otherClassData);

  CreateInitialThreadInstaces ();

  ManageThreads ();

  if  (oneOrMoreThreadsCrashed)
  {
    GenerateCrashReport ();
  }
  else
  {
    GenerateReportAndStats ();
  }
}  /* Main */




void  main (int argc,  char** argv)
{
  AbundanceCorrectionStatsBuilder  app (argc, argv);
  if  (!app.Abort ())
  {
    app.Main ();
  }
}


