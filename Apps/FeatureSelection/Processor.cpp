#include "FirstIncludes.h"

#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "MemoryDebug.h"

using namespace std;

#include <sys/types.h>
#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

#define  MaxCParmValue 20


#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;


#include "MLClass.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "Processor.h"
#include "BinaryJob.h"
#include "BinaryJobList.h"
#include "FeatureImpact.h"
#include "FeatureSelection.h"
#include "FeatureSelectionTypes.h"
#include "JobRandomSplit.h"

/**
 @brief  Processor Constructor for managing FinalResults jobs.
 @details  Make sure the the _summaryResultsFileName is deleted before we start processing.
 */
Processor::Processor (FeatureSelectionPtr  _featureSelection,
                      int                  _processorId,
                      BinaryClassPtr       _binaryClass,
                      FileDescPtr          _fileDesc,
                      fsProcessorStatus    _status
                     ):
  bestA                         (-1.0),
  beamSize                      (1),
  beamSearchFeatureCount        (-1),
  bestC                         (-1.0),
  bestGamma                     (-1.0),
  bestNumOfRounds               (-1),
  aGrowthRate                   (1.0f),
  cGrowthRate                   (1.0f),
  gammaGrowthRate               (1.0f),
  binaryJobs                    (NULL),
  byBinaryClasses               (false),
  class1                        (NULL),
  class2                        (NULL),
  classesThisProcess            (NULL),
  config                        (NULL),
  expansionLimit                (50),
  expansionsSinceHighGrade      (-1),
  featureSelection              (_featureSelection),
  fileDesc                      (_fileDesc),
  firstJobAvailableForExpansion (0),
  dateTimeFirstOneFound         (false),
  highGrade                     (-1.0f),
  mlClasses                  (NULL),
  lastCpuTimeReported           (0.0),
  lockFileOpened                (false),
  lockFile                      (-1),
  log                           (featureSelection->Log ()),
  nextJobId                     (0),
  noMoreJobsLeftToExpand        (false),
  numJobsAtATime                (10),
  numOfExpansions               (0),
  procId                        (-1),
  processorId                   (_processorId),
  quitRunning                   (false),
  resultType                    (frtNULL),
  searchMethod                  (smOnePassOnly),
  searchType                    (stNULL),
  selectionMethod               (SelectionMethod_NULL),
  statusFileNextByte            (0),
  status                        (_status),
  summaryResultsFileName        (),
  testData                      (NULL),
  totalCpuTimeUsed              (0.0),
  trainingData                  (NULL)

{
  resultType = _binaryClass->ResultType ();
  log.Level (10) << "Processor::Processor   Summary[" << FinalResultTypeToStr (resultType) << "]" << endl;
  procId = osGetProcessId ();

  summaryResultsFileName = _binaryClass->FinalResultsFileName ();

  lockFileName   = osGetRootName (summaryResultsFileName) + ".Lock";
  statusFileName = osGetRootName (summaryResultsFileName) + ".Status";

  resultFileName = summaryResultsFileName;

  Block ();

  config = new TrainingConfiguration2 (fileDesc, _binaryClass->ConfigFileName (), log, false);
  switch  (_binaryClass->ResultType ())
  {
  case  frtMfsFeaturesSel:
  case  frtMfsParmsTuned:
  case  frtMfsParmsTunedFeaturesSel:
    config->MachineType (OneVsOne);
    break;


  case  frtBfsFeaturesSel:
  case  frtBfsParmsTuned:
  case  frtBfsFeaturesSelParmsTuned:
    config->MachineType (BinaryCombos);
    break;

  }

  selectionMethod = _binaryClass->SelectionMethod ();
  config->SelectionMethod (_binaryClass->SelectionMethod ());

  alreadyNormalized = featureSelection->AlreadyNormalized ();
  mlClasses      = featureSelection->MLClasses ();
  numJobsAtATime    = featureSelection->NumJobsAtATime ();

  lastCpuTimeReported = osGetSystemTimeUsed ();

  classesThisProcess = new MLClassConstList (*mlClasses);

  trainingData = BuildOurBinaryFeatureData (featureSelection->TrainingData ());
  if  (featureSelection->TestData ())
    testData = BuildOurBinaryFeatureData (featureSelection->TestData ());

  if  (!osFileExists (statusFileName))
    InitializeStatusFileRandomSplits (_binaryClass);

  StatusFileLoad ();

  if  (!featureSelection->JustGettingStats ())
  {
    ofstream* statusFile = OpenStatusFile (ios::app);
    *statusFile 
      << "//" << endl
      << "// New Processor Object." << endl
      << "// Processor Id[" << this->procId          << "]" << endl
      << "// Date/Time   [" << osGetLocalDateTime () << "]." << endl
      << "//"                                        << endl
      << "// Config Parameters[" << config->ModelParameterCmdLine () << "]" << endl
      << "//" << endl;
    statusFile->close ();
    delete  statusFile;
    statusFile = NULL;
  }

  EndBlock ();

  log.Level (10) << "Processor    Exiting" << endl;
}



/**
 @brief  Processor constructor for managing searcg jobs, either feature selection or parameter search.
 */
Processor::Processor (FeatureSelectionPtr  _featureSelection,
                      bool                 _byBinaryClasses,
                      int                  _processorId,
                      FileDescPtr          _fileDesc,
                      stSearchTypes        _searchType,
                      MLClassConstPtr   _class1,
                      MLClassConstPtr   _class2,
                      smSearchMethod       _searchMethod,
                      fsProcessorStatus    _status
                     ):

  bestA                         (-1.0),
  beamSize                      (1),
  bestC                         (-1.0),
  bestGamma                     (-1.0),
  bestNumOfRounds               (-1),
  beamSearchFeatureCount        (-1),
  aGrowthRate                   (1.0f),
  cGrowthRate                   (1.0f),
  gammaGrowthRate               (1.0f),
  binaryJobs                    (NULL),
  byBinaryClasses               (_byBinaryClasses),
  class1                        (_class1),
  class2                        (_class2),
  classesThisProcess            (NULL),
  config                        (NULL),
  expansionLimit                (50),
  expansionsSinceHighGrade      (-1),
  featureSelection              (_featureSelection),
  fileDesc                      (_fileDesc),
  firstJobAvailableForExpansion (0),
  dateTimeFirstOneFound         (false),
  highGrade                     (-1.0f),
  mlClasses                  (NULL),
  lastCpuTimeReported           (0.0),
  lockFileOpened                (false),
  lockFile                      (-1),
  log                           (_featureSelection->Log ()),
  nextJobId                     (0),
  noMoreJobsLeftToExpand        (false),
  numJobsAtATime                (10),
  numOfExpansions               (0),
  procId                        (-1),
  processorId                   (_processorId),
  quitRunning                   (false),
  resultType                    (frtNULL),
  searchMethod                  (_searchMethod),
  searchType                    (_searchType),
  selectionMethod               (SelectionMethod_NULL),
  summaryResultsFileName        (),
  statusFileNextByte            (0),
  status                        (_status),
  testData                      (NULL),
  totalCpuTimeUsed              (0.0),
  trainingData                  (NULL)

{        
  log.Level (10) << "Processor::Processor" << endl;
  procId = osGetProcessId ();

  beamSize          = featureSelection->BeamSize ();
  expansionLimit    = featureSelection->ExpansionLimit ();
  alreadyNormalized = featureSelection->AlreadyNormalized ();
  mlClasses      = featureSelection->MLClasses ();
  numJobsAtATime    = featureSelection->NumJobsAtATime ();
  numOfFolds        = featureSelection->NumOfFolds ();

  lastCpuTimeReported = osGetSystemTimeUsed ();

  KKStr  classNames;
  if  (byBinaryClasses)
  {
    classNames = Class1Name () + "-" + Class2Name ();
    classesThisProcess = new MLClassConstList ();
    classesThisProcess->PushOnBack (class1);
    classesThisProcess->PushOnBack (class2);
  }
  else
  {
    classNames = "AllClasses";
    classesThisProcess = new MLClassConstList (*mlClasses);
  }


  KKStr  searchTypeStr = "Parm";
  if  (searchType == stFeatureSelectionSearch)
    searchTypeStr = "Feature";

  lockFileName   = classNames + ".Lock";

  if  (featureSelection->MajorStep () == msGenerateBruitSvmSearch)
  {
    statusFileName = classNames + "_BruitParmSearch.Status";
    resultFileName = classNames + "_BruitParmSearch.Results";
    searchMethod   = smGrid;
  }

  else if  (featureSelection->ProcessingOrder () == poStandard)
  {
    if  (searchType == stFeatureSelectionSearch)
    {
     statusFileName = classNames + "_Feature.Status";
      resultFileName = classNames + "_Feature.Results";
    }

    else if  (searchType == stParameterSearch)
    {
     statusFileName = classNames + "_Parm.Status";
      resultFileName = classNames + "_Parm.Results";
    }

    else
    {
      statusFileName = classNames + ".Status";
      resultFileName = classNames + ".Results";
    }
  }

  else if  (featureSelection->ProcessingOrder () == poHall)
  {
    switch  (featureSelection->MajorStep ())
    {
    case  msMfsFeatureSelection:
    case  msBfsFeatureSelection:
      statusFileName = classNames + "_Feature.Status";
      resultFileName = classNames + "_Feature.Results";
      break;

    case  msMfsParmTuningPre:
    case  msBfsParmTuningPre:
      statusFileName = classNames + "_Parm-Pre.Status";
      resultFileName = classNames + "_Parm-Pre.Results";
      break;

    case  msMfsParmTuningPost:
    case  msBfsParmTuningPost:
      statusFileName = classNames + "_Parm-Post.Status";
      resultFileName = classNames + "_Parm-Post.Results";
      break;

    default:
      log.Level (-1) << endl << endl
          << "Processor::Processor   ***ERROR***    Invalid MajorStep[" << MajorStepToStr (featureSelection->MajorStep ()) << "]  defined for parameter search processing." << endl
          << endl;
      exit (-1);
    }
  }

  else
  {
    log.Level (-1) << endl << endl
      << "Processor::Processor   ***ERROR***    Invalid ProcessingOrder[" << ProcessingOrderToStr (featureSelection->ProcessingOrder ()) << "]  defined for parameter search processing." << endl
      << endl;
    exit (-1);
  }


  config = new TrainingConfiguration2 (*featureSelection->Config ());
  if  (byBinaryClasses)
  {
    config->EmptyTrainingClasses ();
    config->AddATrainingClass (new TrainingClass ("", Class1Name (), "F", 1.0f, *mlClasses)); 
    config->AddATrainingClass (new TrainingClass ("", Class2Name (), "F", 1.0f, *mlClasses)); 
    config->MachineType (OneVsOne);  
  }

  if  ((featureSelection->MajorStep () == msMfsFeatureSelection)  &&   (featureSelection->ProcessingOrder () == poHall))
  {
    if  (config->ModelingMethod () == Model::mtUsfCasCor)
    {
      config->Number_of_rounds (2);
    }
    else
    {
      int  numOfFeatures = featureSelection->InitialFeatures ().NumSelFeatures ();
      config->C_Param (1.0);
      config->Gamma (1.0 / (double)numOfFeatures);
      config->A_Param (100.0);
    }
  }


  Block ();

  trainingData = BuildOurBinaryFeatureData (featureSelection->TrainingData ());
  if  (featureSelection->TestData ())
    testData = BuildOurBinaryFeatureData (featureSelection->TestData ());

  if  (!osFileExists (statusFileName))
  {
    if  (searchType == stFeatureSelectionSearch)
      InitializeStatusFile ();

    else if  (searchType == stParameterSearch)
      InitializeStatusFileParameterSearch ();
  }

  StatusFileLoad ();

  if  (!featureSelection->JustGettingStats ())
  {
    ofstream* statusFile = OpenStatusFile (ios::app);
    *statusFile 
      << "//" << endl
      << "// New Processor Object." << endl
      << "// Process Id[" << procId                << "]" << endl
      << "// Date/Time [" << osGetLocalDateTime () << "]." << endl
      << "//"                                         << endl
      << "// Config Parameters[" << config->ModelParameterCmdLine () << "]" << endl
      << "//" << endl;
    statusFile->close ();
    delete  statusFile;
    statusFile = NULL;
  }

  EndBlock ();
  log.Level (10) << "Processor    Exiting Constructor" << endl;
}




Processor::~Processor ()
{
  log.Level (10) << "~Processor" << endl;
  delete  classesThisProcess;   classesThisProcess = NULL;
  delete  binaryJobs;           binaryJobs         = NULL;
  delete  config;               config             = NULL;
  delete  testData;             testData           = NULL;
  delete  trainingData;         trainingData       = NULL;
  //EndBlock ();
}



const FeatureNumList&   Processor::InitialFeatures () const
{
  return  featureSelection->InitialFeatures ();
}



MLClassConstPtr  Processor::PositiveClass ()  const
{
  if  (featureSelection)
    return  featureSelection->PositiveClass ();
  return NULL;
}  /* PositiveClass */



VectorInt  Processor::GetSmallestJobIdByFeatureCount ()
{
  int  maxNumFeatures = fileDesc->NumOfFields ();

  VectorInt  featureCountsByJobId (maxNumFeatures + 1, 0);

  if  (binaryJobs)
  {
    BinaryJobList::iterator  idx;
    for  (idx = binaryJobs->begin ();  idx != binaryJobs->end ();  idx++)
    {
      BinaryJobPtr j = *idx;
      int featureCount =  j->FeatureCount ();

      if  ((featureCount < 0)  ||  (featureCount > maxNumFeatures))
        continue;

      //if  (j->JobId () < featureCountsByJobId[featureCount])
      //  featureCountsByJobId[featureCount] = j->JobId ();
      featureCountsByJobId[featureCount]++;
    }
  }

  return  featureCountsByJobId;
}  /* GetSmallestJobIdByFeatureCount */



VectorDouble  Processor::GetCpuCyclesByFeatureCount ()
{
  int  maxNumFeatures = fileDesc->NumOfFields ();

  VectorDouble  cpuCyclesByFeatureCount (maxNumFeatures + 1, 0.0);

  if  (binaryJobs)
  {
    BinaryJobList::iterator  idx;
    for  (idx = binaryJobs->begin ();  idx != binaryJobs->end ();  idx++)
    {
      BinaryJobPtr j = *idx;
      int featureCount =  j->FeatureCount ();

      if  ((featureCount < 0)  ||  (featureCount > maxNumFeatures))
        continue;

      //if  (j->JobId () < featureCountsByJobId[featureCount])
      //  featureCountsByJobId[featureCount] = j->JobId ();
      cpuCyclesByFeatureCount[featureCount] += j->ProcessingTime ();
    }
  }

  return  cpuCyclesByFeatureCount;
}  /* GetCpuCyclesByFeatureCount */




FeatureVectorListPtr  Processor::RandomSplit (int splitNum)
{
  return  featureSelection->RandomSplit (splitNum);
}  /* RandomSplit */






FeatureVectorListPtr   Processor::TestData ()
{
  if  (!testData)
    testData = featureSelection->TestData ();
  return  testData;
}



FeatureVectorListPtr  Processor::ValidationData ()
{
  return  featureSelection->ValidationData ();
}




BinaryJobListPtr  Processor::BinaryJobs () 
{
  if  (binaryJobs == NULL)
  {
    StatusFileLoad ();
    if  (binaryJobs == NULL)
      binaryJobs = new BinaryJobList (this);
  }

  return  binaryJobs;
}  /* BinaryJobs */




const KKStr&  Processor::Class1Name    () const
{
  if  (class1)
    return  class1->Name ();
  else
    return  KKStr::EmptyStr ();
}


const KKStr&  Processor::Class2Name    () const
{
  if  (class2)
    return  class2->Name ();
  else
    return  KKStr::EmptyStr ();
}



FeatureVectorListPtr  Processor::BuildOurBinaryFeatureData (FeatureVectorListPtr  srcData)
{
  log.Level (10) << "Processor::BuildOurBinaryFeatureData." << endl;

  if  (srcData->FileDesc () != fileDesc)
  {
    log.Level (-1) << endl
                   << endl
                   << "Processor::BuildOurBinaryFeatureData       ***ERROR***" << endl
                   << "                                       Mismatch of Filedesc"  << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  FeatureVectorListPtr  examplesForOurClasses = NULL;

  if  (byBinaryClasses)
  {
    examplesForOurClasses = new FeatureVectorList (fileDesc, 
                                                   false,      // false = this instance will not own its contents.
                                                   log, 
                                                   srcData->QueueSize ()
                                                  );

    FeatureVectorList::iterator  idx;

    FeatureVectorPtr  i = NULL;

    int  binaryClassCount     = 0;
    int  halfBinaryClassCount = 0;
    int  thirdClassCount      = 0;
    for  (idx = srcData->begin ();  idx != srcData->end ();  idx++)
    {
      i = *idx;
      if  ((i->MLClass () == class1)  ||  (i->MLClass () == class2))
      {
        binaryClassCount++;
        halfBinaryClassCount = binaryClassCount / 2;
        examplesForOurClasses->PushOnBack (i);
      }
    }
  }
  else
  {
    examplesForOurClasses = new FeatureVectorList (*srcData, false);
  }

  log.Level (10) << "BuildOurBinaryFeatureData  NumExamples["  << examplesForOurClasses->QueueSize () << "]." << endl;

  return  examplesForOurClasses;
}  /* BuildOurBinaryFeatureData */




void    Processor::Block ()
{
  log.Level (20) << "Processor::Block." << endl;

  if  (lockFileOpened)
  {
    // We have out Lock and EndLock calls out of order.
    log.Level (-1)  << endl << endl 
                    << "Processor::Block      *** WE ALREADY HAVE A BLOCK ESTABLISHED ***." << endl
                    << endl;
    return;
  }
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName.Str (), O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      float  zed = (float)((procId + rand ()) % 10) + 2.0f;
      log.Level (10) << "Processor::Block - We are locked out[" << count << "]  for [" << zed << "] secs."  << endl;
      osSleep (zed);
    }
  }  while  (lockFile < 0);

  if  (count > 0)
    log.Level (10) << "Processor::Block   Lock has been established." << endl;

  lockFileOpened = true;

  log.Level (20) << "Processor::Block - Lock is Established." << endl;
}  /* Block */




void   Processor::EndBlock ()
{
 #ifndef  WIN32
 int  returnCd;
 #endif
  
  log.Level (20) << "Processor::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << endl << endl << endl
                   << "Processor::EndBlock          *** Lock file is not opened ***" << endl;
    return;
  }

  close (lockFile);
  lockFileOpened = false;

  #ifdef  WIN32
  if  (!DeleteFile (lockFileName.Str ()))
  {
     DWORD fileAttributes = GetFileAttributes (lockFileName.Str ());
     fileAttributes = FILE_ATTRIBUTE_NORMAL;
     if  (!SetFileAttributes (lockFileName.Str (), fileAttributes))
     {
       DWORD  lastErrorNum = GetLastError ();
       log.Level (-1) << "Processor::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName.Str ()))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "Processor::EndBlock - Error["  << (int32)lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName.Str ());
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */




void  Processor::ProcessStatusFileLineJobStatusChange (KKStrParser&  statusLineStr)
{
  int expandedJobId = statusLineStr.GetNextTokenInt ("\t");
  BinaryJobPtr  j = binaryJobs->LookUpByJobId (expandedJobId);
  if  (!j)
  {
    log.Level (-1) << endl << endl << endl 
                   << "ProcessStatusLineJobStatusChange    ***Error***    Could not locate Expanded" << endl
                   << endl
                   << "                                    JobId[" << expandedJobId << "]"  << endl
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }

  KKStr statusStr = statusLineStr.GetNextToken ("\t");
  statusStr.TrimLeft ();
  statusStr.TrimRight ();
  bjBinaryJobStatus status = BinaryJob::BinaryJobStatusFromStr (statusStr);
  if  (status == bjNULL)
  {
    log.Level (-1) << endl << endl << endl 
                   << "ProcessStatusLineJobStatusChange    ***Error***      Invalid Status Specified" << endl
                   << endl
                   << "                                    JobId["  << expandedJobId << "]"  << endl
                   << "                                    Status[" << statusStr     << "]"  << endl
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }
  
  j->Status (status);
  
}  /* ProcessStatusLineJobStatusChange */



void  Processor::ProcessStatusFileLine (KKStrParser&  statusStr)
{
  if  (statusStr.SubStrPart (0, 1) == "//")
  {
    // A coment line;  we can ignore it.
    return;
  }

  statusStr.TrimWhiteSpace (" \n\r");

  KKStr  fieldName;

  fieldName = statusStr.GetNextToken ("\t\n\r");
  if  (fieldName.Empty ())
  {
    // A empty line we will ignore it.
    return;
  }

  fieldName.Upper ();

  if  (fieldName == "BEAMSIZE")
    beamSize = statusStr.GetNextTokenInt ();

  else if  ((fieldName == "BINARYJOB")  ||  (fieldName == "BINARYCOMBOJOB"))
  {
    BinaryJobPtr  j = new BinaryJob (this, statusStr);
    BinaryJobPtr  existingJob = binaryJobs->LookUpByJobId (j->JobId ());
    if  (existingJob)
    {
      if  (j->ValidateOnly () != existingJob->ValidateOnly ())
      {
        log.Level (-1) << endl 
          << "Processor::ProcessStatusFileLine  ValidateOnly Does not Match   JobId[" << j->JobId () << "]  existing[" << existingJob->ValidateOnly () << "]  New[" << j->ValidateOnly () << "]" << endl
          << "statusStr[" << statusStr.Str () << "]" << endl
          << endl;
      }
      existingJob->ReFresh (*j);
      delete  j;  j = NULL;
    }
    else
    {
      existingJob = binaryJobs->LookUpByParameters (j->Features     (),
                                                    j->NumOfRounds  (),
                                                    j->CParm        (), 
                                                    j->GammaParm    (), 
                                                    j->AParm        (), 
                                                    j->ValidateOnly ()
                                                   );
      if  (existingJob)
      {
        log.Level (-1) << endl << endl 
                       <<"Processor::ProcessStatusFileLine     ***ERROR***     Two jobs with same parameters  ***" << endl
                       << endl
                       << "     ExistingJob[" << existingJob->JobId () << "]  New Job[" << j->JobId () << "]" << endl
                       << endl;
        delete  j;  j = NULL;
      }
      else
      {
        binaryJobs->PushOnBack (j);
      }
    }
  }


  else if  ((fieldName.CompareIgnoreCase ("RandomSplitJob") == 0)  ||  
            (fieldName.CompareIgnoreCase ("JobRandomSplit") == 0)
           )
  {
    BinaryJobPtr  j = new JobRandomSplit (this, statusStr);
    BinaryJobPtr  existingJob = binaryJobs->LookUpByJobId (j->JobId ());
    if  (existingJob)
    {
      existingJob->ReFresh (*j);
      delete  j;  j = NULL;
    }
    else
    {
      binaryJobs->PushOnBack (j);
    }
  }


  else if  (fieldName.CompareIgnoreCase ("JobValidation") == 0)
  {
    BinaryJobPtr  j = new JobValidation (this, statusStr);
    BinaryJobPtr  existingJob = binaryJobs->LookUpByJobId (j->JobId ());
    if  (existingJob)
    {
      existingJob->ReFresh (*j);
      delete  j;  j = NULL;
    }
    else
    {
      binaryJobs->PushOnBack (j);
    }
  }

  else if  (fieldName == "BEST_A")
    bestA = statusStr.GetNextTokenFloat ();

  else if  (fieldName == "BEST_C")
    bestC = statusStr.GetNextTokenDouble ();

  else if  (fieldName == "BEST_GAMMA")
    bestGamma = statusStr.GetNextTokenDouble ();

  else if  (fieldName.EqualIgnoreCase ("Best_NumOfRounds"))
    bestNumOfRounds = statusStr.GetNextTokenInt ();

  else if  (fieldName == "A_GROWTHRATE")
    aGrowthRate = (float)statusStr.GetNextTokenDouble ();

  else if  (fieldName == "C_GROWTHRATE")
    cGrowthRate = (float)statusStr.GetNextTokenDouble ();

  else if  (fieldName == "GAMMA_GROWTHRATE")
    gammaGrowthRate = (float)statusStr.GetNextTokenDouble ();

  else if  (fieldName == "CLASS1NAME")
    class1 = mlClasses->GetMLClassPtr (statusStr.GetNextToken ());

  else if  (fieldName == "CLASS2NAME")
    class2 = mlClasses->GetMLClassPtr (statusStr.GetNextToken ());

  else if  (fieldName == "CPUTIMEUSED")
  {
    int  processorIdThisLine = 0;
    double  cpuTimeUsed = statusStr.GetNextTokenDouble ("\t\n\r");
    KKStr  hostName;

    totalCpuTimeUsed += cpuTimeUsed;

    while  (statusStr.MoreTokens ())
    {
      KKStr  nextField = statusStr.GetNextToken ("\n\r\t");
    if  (nextField.EqualIgnoreCase ("ProcessorId"))
        processorIdThisLine = statusStr.GetNextTokenInt ("\n\r\t");

      else if  (nextField.EqualIgnoreCase ("HostName"))
        hostName = statusStr.GetNextToken ("\t\n\r");       
    }

    cpuCyclesByProcId.AddCycles (processorIdThisLine, cpuTimeUsed);
  }

  else if  (fieldName == "CURRENTDATETIME")
  {
    KKU::DateTime  dateTime = statusStr.GetNextTokenDateTime ();
    if  (!dateTimeFirstOneFound)
    {
      dateTimeFirstOneFound = true;
      dateTimeStarted = dateTime;
    }
    dateTimeEnded = dateTime;
  }

  else if  (fieldName == "EXPSINCEHIGHGRADE")
    expansionsSinceHighGrade = statusStr.GetNextTokenInt ();

  else if  (fieldName == "EXPANSIONLIMIT")
    expansionLimit = statusStr.GetNextTokenInt ();

  else if  (fieldName == "FIRSTJOBAVAILABLEFOREXPANSION")
    firstJobAvailableForExpansion = statusStr.GetNextTokenInt ();

  else if  (fieldName == "HIGHGRADE")
    highGrade = statusStr.GetNextTokenFloat ();

  else if  (fieldName == "JOBSTATUSCHANGE")
  {
    ProcessStatusFileLineJobStatusChange (statusStr);
  }

  else if  (fieldName == "NEXTJOBID")
    nextJobId = statusStr.GetNextTokenInt ();

  else if  (fieldName.EqualIgnoreCase ("NumJobsAtATime"))
    numJobsAtATime = statusStr.GetNextTokenInt ();

  else if  (fieldName == "NUMOFEXPANSIONS")
    numOfExpansions = statusStr.GetNextTokenInt ();

  else if  (fieldName == "PROCESSORID")
    processorId = statusStr.GetNextTokenInt ();

  else if  (fieldName == "QUITRUNNING")
    quitRunning = true;

  else if  (fieldName == "RESTART")
    quitRunning = false;

  else if  (fieldName == "SEARCHMETHOD")
  {
    searchMethod = SearchMethodFromStr (statusStr.GetNextToken ());
    if  ((searchMethod == smBeam)  &&  (beamSearchFeatureCount < 1))
      beamSearchFeatureCount = binaryJobs->SmallestNumberOfFeaturesExpanded ();
  }

  else if  (fieldName.CompareIgnoreCase ("SelectionMethod") == 0)
    selectionMethod = SelectionMethodFromStr (statusStr.GetNextToken ());


  else if  (fieldName == "STATUS")
    status = ProcessorStatusFromStr (statusStr.GetNextToken ());

  else
  {
    log.Level (-1) << "Processor::ProcessStatusFileLine  Invalid Field Name[" << fieldName << "]." << endl;
  }
}  /* ProcessStatusFileLine */




void  Processor::StatusFileLoad ()
{
  log.Level (10) << "Processor::StatusFileLoad." << endl;

  char  buff[20480];

  log.Level (10) << "Processor::StatusFileLoad     statusFileName[" << statusFileName << "]"  << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "Processor::LoadCurrentStatusFile    ***ERROR***   Can not open Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  KKStr  fieldName;
  KKStr  fieldValue;

  delete  binaryJobs;
  binaryJobs = new BinaryJobList (this);

  while  (fgets (buff, sizeof (buff), statusFile))
  {
    KKStrParser  statusStr (buff);
    ProcessStatusFileLine (statusStr);
  }

  statusFileNextByte = ftell (statusFile);

  fclose  (statusFile);

  log.Level (20) << "Processor::StatusFileLoad    Exiting." << endl;

}  /* StatusFileLoad */




void  Processor::StatusFileRefresh ()
{
  // we only want to read in any new changes to the status file.
  log.Level (10) << "Processor::StatusFileRefresh     statusFileName[" << statusFileName << "]"  << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
     log.Level (-1) << endl
                    << "Processor::LoadCurrentStatusFile   ***ERROR***    Can not open Status File[" 
                    << statusFileName << "]." 
                    << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  char  buff[20480];

  fseek (statusFile, statusFileNextByte, SEEK_SET);
  while  (fgets (buff, sizeof (buff), statusFile))
  {
    KKStrParser statusStr (buff);
    ProcessStatusFileLine (statusStr);
  }

  statusFileNextByte = ftell (statusFile);

  fflush (statusFile);
  fclose (statusFile);

  log.Level (20) << "Processor::StatusFileRefresh     Exiting."  << endl;
}  /* StatusFileRefresh */




ofstream*    Processor::OpenStatusFile (ios::openmode  openMode)
{
  log.Level (20) << "Processor::OpenStatusFile."  << endl;

  int  openAttempts = 0;

  ofstream*  statusFile  = new ofstream (statusFileName.Str (), openMode);

  while  ((!statusFile->is_open ()) &&  (openAttempts < 4))
  {
    openAttempts++;
    osSleep (2.0f);
    log.Level (10) << "OpenStatusFile   Open Attempt[" << openAttempts << "]." << endl;
    statusFile  = new ofstream (statusFileName.Str (), openMode);
    if  (!statusFile->is_open ())
    {
      log.Level (0) << "OpenStatusFile    -   *** WARNING ***  Can Not Open Status File[" 
                    << statusFileName.Str () << "]   -Will Retry-." 
                    << endl;
    }
  }

  if  (!statusFile->is_open ())
  {
     log.Level (-1) << "OpenStatusFile   ***ERROR***  Can not Create Status File[" << statusFileName.Str () << "]." << endl;
     EndBlock ();
     osWaitForEnter ();
     exit (-1);
  }

  log.Level (20) << "Processor::OpenStatusFile     Exiting"  << endl;

  return  statusFile;
}  /* OpenStatusFile */




void  Processor::WriteStatusFile ()
{
  log.Level (10) << "Processor::WriteStatusFile" << endl;

  ofstream*  statusFile = OpenStatusFile (ios::out);

  *statusFile << "// Date/Time [" << osGetLocalDateTime ()                                << "]" << endl
              << "//"                                                                            << endl
              << "// Status File for Classes["  << Class1Name () << ", " << Class2Name () << "]" << endl
              << "//"                                                                            << endl
              << "// Config Parameters[" << config->ModelParameterCmdLine ()              << "]" << endl
              << "//" << endl;

  *statusFile               << "ProcessorId"  << "\t" << processorId   << endl;
  if  (class1)  *statusFile << "Class1Name"   << "\t" << Class1Name () << endl;
  if  (class2)  *statusFile << "Class2Name"   << "\t" << Class2Name () << endl;

  *statusFile << "Status"                         << "\t" << ProcessorStatusToStr (status)     << endl
              << "SearchMethod"                   << "\t" << SearchMethodToStr (searchMethod)  << endl
              << "SelectionMethod"                << "\t" << SelectionMethodToStr (selectionMethod) << endl
              << "BeamSize"                       << "\t" << beamSize                          << endl
              << "NextJobId"                      << "\t" << nextJobId                         << endl
              << "NumOfExpansions"                << "\t" << numOfExpansions                   << endl
              << "ExpansionLimit"                 << "\t" << expansionLimit                    << endl
              << "HighGrade"                      << "\t" << highGrade                         << endl
              << "ExpSinceHighGrade"              << "\t" << expansionsSinceHighGrade          << endl
              << "FirstJobAvailableForExpansion"  << "\t" << firstJobAvailableForExpansion     << endl
              << "CurrentDateTime"                << "\t" << osGetLocalDateTime ()             << endl;

  WriteStatusFileBestParmValues (*statusFile);
  WriteStatusFileGrowthRates (*statusFile);

  int  x;

  for  (x = 0;  x < binaryJobs->QueueSize ();  x++)
  {
    BinaryJobPtr  j = binaryJobs->IdxToPtr (x);
    *statusFile << j->JobTypeStr ()  << "\t" << j->ToStatusStr () << endl;
  }

  statusFile->flush ();
  statusFile->close ();
  delete  statusFile;

  log.Level (10) << "Processor::WriteStatusFile    Exiting" << endl;
}  /* WriteStatusFile */



void  Processor::WriteStatusFileGrowthRates (ostream&  statusFile)
{
  int  precision = statusFile.precision ();
  statusFile.precision (10);
  statusFile  << "A_GrowthRate"     << "\t" << aGrowthRate     << std::endl
              << "C_GrowthRate"     << "\t" << cGrowthRate     << std::endl
              << "Gamma_GrowthRate" << "\t" << gammaGrowthRate << std::endl;

  statusFile.precision (precision);
}  /* WriteStatusFileGrowthRates */



void  Processor::WriteStatusFileBestParmValues (ostream&  statusFile)
{
  int  precision = statusFile.precision ();
  statusFile.precision (13);
  statusFile << "Best_C"            << "\t"  << bestC            << std::endl;
  statusFile << "Best_GAMMA"        << "\t"  << bestGamma        << std::endl;
  statusFile << "Best_A"            << "\t"  << bestA            << std::endl;
  statusFile << "Best_NumOfRounds"  << "\t"  << bestNumOfRounds  << std::endl;

  statusFile.precision (precision);
}  /* WriteStatusFileBestParmValues */




void  Processor::ReportCpuTimeUsed (ofstream* statusFile)
{
  // While we have the status file open lets report CPU time used so far
  double  currentCpuTime = osGetSystemTimeUsed ();
  double  cpuTimeUsed = fabs (currentCpuTime - lastCpuTimeReported);
  lastCpuTimeReported = currentCpuTime;
  *statusFile << "CpuTimeUsed"     << "\t" << cpuTimeUsed           << "\t"
              << "ProcessorId"     << "\t" << procId                << "\t"
              << "HostName"        << "\t" << featureSelection->HostName ()
              << endl;

  *statusFile << "CurrentDateTime" << "\t" << osGetLocalDateTime () << endl;
}  /* ReportCpuTimeUsed */



void  Processor::InitializeStatusFileParameterSearch ()
{
  log.Level (10) << "Processor::InitializeStatusFileParameterSearch" << endl;

  searchMethod = smGrid;

  delete  binaryJobs;
  binaryJobs = new BinaryJobList (this);
  
  WriteStatusFile ();

  ofstream* statusFile = OpenStatusFile (ios::app);

  if  (config->ModelType () == Model::mtUsfCasCor)
  {
    CreateParameterJobsUsfCasCor (statusFile, 1, 30);
  }
  else
  {
    if  ((featureSelection->ParamSelCriteria () == jscBruitForce)  ||  (featureSelection->MajorStep () == msGenerateBruitSvmSearch))
    {
      cGrowthRate     = 1.15f;
      gammaGrowthRate = 1.3f;
      aGrowthRate     = 1.0;
      WriteStatusFileGrowthRates (*statusFile);

      CreateParameterJobs (statusFile, -1,
                           cGrowthRate,     1.0,      MaxCParmValue,   // c     (GrowthRate, Min, Max)
                           gammaGrowthRate, 0.00001,     5.0,          // gamma (GrowthRate, Min, Max)
                           aGrowthRate,     100.0f,    100.0f          // A     (GrowthRate, Min, Max)
                          );
    }
    else
    {
      cGrowthRate     = 1.1f;
      gammaGrowthRate = 2.0f;
      aGrowthRate     = 1.0;
      WriteStatusFileGrowthRates (*statusFile);

      CreateParameterJobs (statusFile, -1,
                           cGrowthRate,     1.0,        1.0,    // 
                           gammaGrowthRate, 0.00001,   20.0,    // 
                           aGrowthRate,     100.0f,   100.0f    // 
                          );
    }
  }


  ReportCpuTimeUsed (statusFile);

  statusFile->flush ();
  statusFile->close ();
  delete  statusFile;  statusFile = NULL;

  log.Level (10) << "Processor::InitializeStatusFileParameterSearch    Exiting" << endl;
}  /* InitializeStatusFileParameterSearch */




void  Processor::InitializeStatusFileRandomSplits (BinaryClassPtr   _binaryClass)
{
  log.Level (10) << "Processor::InitializeStatusFileRandomSplits" << endl;

  searchMethod = smOnePassOnly;

  delete  binaryJobs;
  binaryJobs = new BinaryJobList (this);
  
  WriteStatusFile ();

  ofstream* statusFile = OpenStatusFile (ios::app);

  CreateRandomSplitsJobs (statusFile);
  if  (ValidationData () != NULL)
    CreateValidationJob (statusFile, _binaryClass);

  ReportCpuTimeUsed (statusFile);

  statusFile->flush ();
  statusFile->close ();
  delete  statusFile;  statusFile = NULL;

  log.Level (10) << "Processor::InitializeStatusFileRandomSplits    Exiting" << endl;
}  /* InitializeStatusFileRandomSplits */




void  Processor::InitializeStatusFile ()
{
  log.Level (10) << "Processor::InializeStatusFile" << endl;

  delete  binaryJobs;
  binaryJobs = new BinaryJobList (this);

  BinaryJobPtr  firstJob = new BinaryJob (this, 
                                          -1,                              // ParentId
                                          featureSelection->InitialFeatures (),
                                          -1,                              // ChgFeature
                                          config->NumOfRounds  (),
                                          config->C_Param      (),
                                          (float)config->Gamma (),
                                          config->A_Param      ()
                                         );
  binaryJobs->PushOnBack (firstJob);

  WriteStatusFile ();

  log.Level (10) << "Processor::InializeStatusFile    Exiting" << endl;

}  /* InitializeStatusFile */




void  Processor::FlagJobsForTesting (ofstream*         statusFile,
                                     BinaryJobListPtr  candidats
                                    )
{
  log.Level (10) << "Processor::FlagJobsForTesting"  << endl;
  BinaryJobList::iterator  idx;
  for  (idx = candidats->begin ();  idx != candidats->end ();  idx++)
  {
    BinaryJobPtr j = *idx;

    // Make sure a validation job does not already exists.
    BinaryJobPtr existingJob = binaryJobs->LookUpByParameters 
                                            (j->Features (), 
                                             j->NumOfRounds (),
                                             j->CParm (), 
                                             j->GammaParm (), 
                                             j->AParm (), 
                                             true
                                            );
    if  (existingJob)
    {
      continue;
    }

    BinaryJobPtr newJob = new BinaryJob (this, j->JobId (),
                                         j->Features (), 
                                         j->ChgFeatureNum (), 
                                         j->NumOfRounds (),
                                         j->CParm (),
                                         j->GammaParm (),
                                         j->AParm ()
                                        );

    newJob->Accuracy           (j->Accuracy           ());
    newJob->AccuracyNorm       (j->AccuracyNorm       ());
    newJob->AvgPredProb        (j->AvgPredProb        ());
    newJob->DecisiveProbMean   (j->DecisiveProbMean   ());
    newJob->DecisiveProbStdDev (j->DecisiveProbStdDev ());
    newJob->FMeasure           (j->FMeasure           ());
    newJob->Grade              (j->Grade              ());
    newJob->ProcessingTime     (j->ProcessingTime     ());
    newJob->ValidateOnly (true);
    newJob->Status (bjOpen);

    *statusFile << newJob->JobTypeStr () << "\t" << newJob->ToStatusStr () << endl;
    binaryJobs->PushOnBack (newJob);
  }

  *statusFile << "NextJobId" << "\t" << nextJobId << endl;

  log.Level (10) << "Processor::FlagJobsForTesting        Exiting"  << endl;
}  /* FlagJobsForTesting */




void  Processor::CreateParameterJobsUsfCasCor (ofstream*  statusFile, 
                                               int32      minNumOfRounds,
                                               int32      maxNumOfRounds
                                              )
{
  log.Level (10) << "Processor::CreateParameterJobsUsfCasCor" << endl;
  
  // It is assumed that we have a 'EndBlock
  *statusFile << "// CreateParameterJobsUsfCasCor" << endl
              << "//  minNumOfRounds[" << minNumOfRounds << "]" << endl
              << "//  maxNumOfRounds[" << maxNumOfRounds << "]" << endl
              << "//" << endl;

  FeatureNumList featureNums (fileDesc);

  if  (byBinaryClasses)
    featureNums = config->GetFeatureNums (class1, class2);
  else
    featureNums = config->GetFeatureNums ();


  int32  numOfRounds =  minNumOfRounds;
  while  (numOfRounds < maxNumOfRounds)
  {
    BinaryJobPtr existingJob = binaryJobs->LookUpByParameters (featureNums, 
                                                               numOfRounds, 
                                                               1.0,   // cParam
                                                               1.0,   // gammaParam
                                                               100.0,
                                                               false
                                                              );
    {
      if  (!existingJob)
      {
        BinaryJobPtr  j = new BinaryJob (this, 
                                         -1,
                                         featureNums,
                                         -1,
                                         numOfRounds,
                                         1.0,   // cParam
                                         1.0,   // gammaParam
                                         100.0  // aParam
                                        );
        *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
        binaryJobs->PushOnBack (j);
      }
    }

    ++numOfRounds;
  }

  *statusFile << "NextJobId" << "\t" << nextJobId << endl;

  log.Level (10) << "Processor::CreateParameterJobsUsfCasCor    Exiting" << endl;
}  /* CreateParameterJobsUsfCasCor */



void  Processor::CreateParameterJobs (ofstream*  statusFile,
                                      int        parentJobId,
                                      float      cParmGrowthRate,
                                      double     cParmMin,
                                      double     cParmMax,
                                      float      gammaParmGrowthRate,
                                      double     gammaParmMin,
                                      double     gammaParmMax,
                                      float      aParmGrowthRate,
                                      float      aParmMin,
                                      float      aParmMax
                                     )
{
  log.Level (10) << "Processor::CreateParameterJobs" << endl;
  
  // It is assumed that we have a 'EndBlock
  *statusFile << "// CreateParameterJobs" << endl
              << "//  C      GrowthRate[" << cParmGrowthRate     << "]" << "\t" << "Min[" << cParmMin     << "]" << "\t" << "Max[" << cParmMax     << "]" << endl
              << "//  Gamma  GrowthRate[" << gammaParmGrowthRate << "]" << "\t" << "Min[" << gammaParmMin << "]" << "\t" << "Max[" << gammaParmMax << "]" << endl
              << "//  A      GrowthRate[" << aParmGrowthRate     << "]" << "\t" << "Min[" << aParmMin     << "]" << "\t" << "Max[" << aParmMax     << "]" << endl
              << "//" << endl;

  FeatureNumList featureNums (fileDesc);

  if  (byBinaryClasses)
    featureNums = config->GetFeatureNums (class1, class2);
  else
    featureNums = config->GetFeatureNums ();

  float  aParameter = aParmMin;
  while  (aParameter <= aParmMax)
  {
    double  cParameter = cParmMin;

    while  (cParameter <= cParmMax)
    {
      double  gammaParameter = gammaParmMin;
      while  (gammaParameter <= gammaParmMax)
      {
        BinaryJobPtr existingJob = binaryJobs->LookUpByParameters (featureNums, 1, cParameter, gammaParameter, aParameter, false);
        if  (!existingJob)
        {
          BinaryJobPtr  j = new BinaryJob (this,
                                           parentJobId,
                                           featureNums,
                                           -1,
                                           1,               // numOfRounds
                                           cParameter,
                                           gammaParameter,
                                           aParameter
                                          );
          *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
          binaryJobs->PushOnBack (j);
        }

        if  (gammaParmGrowthRate <= 1.0f)
          break;
        gammaParameter = gammaParameter * gammaParmGrowthRate;
      }

      if  (cParmGrowthRate <= 1.0f)
        break;
      cParameter = cParameter * cParmGrowthRate;
    }

    if  (aParmGrowthRate <= 1.0f)
      break;

    aParameter = aParameter * aParmGrowthRate;
  }

  *statusFile << "NextJobId" << "\t" << nextJobId << endl;

  log.Level (10) << "Processor::CreateParameterJobs    Exiting" << endl;
}  /* CreateParameterJobs */







void  Processor::CreateRandomSplitsJobs (ostream*  statusFile)
{
  log.Level (10) << "Processor::CreateRandomSplitsJobs" << endl;

  int  splitNum = 0;
  for  (splitNum = 0; splitNum < featureSelection->RandomSplitsNum ();  splitNum++)
  {
    JobRandomSplitPtr j = 
      new JobRandomSplit (this,
                          -1,                         // parentId
                          config->GetFeatureNums (),
                          -1,                         //_chgFeatureNum,
                          config->NumOfRounds (),
                          config->C_Param (),
                          config->Gamma   (),
                          config->A_Param (),
                          splitNum
                         );

    *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
    binaryJobs->PushOnBack (j);
  }
}  /* CreateRandomSplitsJobs */




void  Processor::CreateValidationJob (ostream*        statusFile,
                                      BinaryClassPtr  _binaryClass
                                     )
{
  log.Level (10) << "Processor::CreateValidationJob" << endl;

  double  cParm       = config->C_Param     ();
  double  gamma       = config->Gamma       ();
  double  aParm       = config->A_Param     ();
  int     numOfRounds = config->NumOfRounds ();

  if  (ResultType () == frtNoTuningAllFeatures)
  {
    cParm = 1.0;
    gamma = 1.0 / (double)(InitialFeatures ().NumSelFeatures ());
    aParm = 100.0;
    numOfRounds = 1;
  }

  JobValidationPtr  j = 
      new JobValidation (this,
                         -1,                         // parentId
                         config->GetFeatureNums (),
                         -1,                         //_chgFeatureNum,
                         numOfRounds,
                         cParm,
                         gamma,
                         (float)aParm,
                         _binaryClass->ConfigFileName ()
                         );

  *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
  binaryJobs->PushOnBack (j);
}  /* CreateValidationJob */




int  Processor::AllocateNextJobId ()
{
  int  jobId = nextJobId;
  nextJobId++;
  return  jobId;
}




void   Processor::Update (ProcessorPtr  p)
{
  beamSize                  = p->beamSize;
  expansionsSinceHighGrade  = p->expansionsSinceHighGrade;
  highGrade                 = p->highGrade;
  nextJobId                 = p->nextJobId;
  numOfExpansions           = p->numOfExpansions;
  searchMethod              = p->searchMethod;
  status                    = p->status;
}







KKStr  Processor::ToStatusStr ()
{
  KKStr  statusStr (200);  // Preallocate 200 bytes

  statusStr << "ProcessorId"                   << "\t" << processorId                       << "\t"
            << "Class1Name"                    << "\t" << Class1Name ()                     << "\t"
            << "Class1Name"                    << "\t" << Class2Name ()                     << "\t"
            << "Status"                        << "\t" << ProcessorStatusToStr (status)     << "\t"
            << "SearchMethod"                  << "\t" << SearchMethodToStr (searchMethod)  << "\t"
            << "NextJobId"                     << "\t" << nextJobId                         << "\t"
            << "NumOfExpansions"               << "\t" << numOfExpansions                   << "\t"
            << "ExpansionLimit"                << "\t" << expansionLimit                    << "\t"
            << "HighGrade"                     << "\t" << StrFormatDouble (highGrade, "zz0.000000")  << "\t"
            << "ExpSinceHighGrade"             << "\t" << expansionsSinceHighGrade          << "\t"
            << "FirstJobAvailableForExpansion" << "\t" << firstJobAvailableForExpansion;

  return  statusStr;
}  /* ToStatusStr */



void  Processor::UpdateExpandedJobs (ofstream*         statusFile,
                                     BinaryJobPtr      expandedJob,
                                     BinaryJobListPtr  newJobs
                                    )
{
  expandedJob->Status (bjExpanded);
  *statusFile << "JobStatusChange" << "\t" << expandedJob->JobId () << "\t" << expandedJob->StatusStr () << endl;

  BinaryJobList::iterator  idx;
  for  (idx = newJobs->begin ();  idx != newJobs->end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
    binaryJobs->PushOnBack (j);
  }

  *statusFile << "NextJobId" << "\t" << nextJobId << endl;
}  /* UpdateExpandedJobs */




void  Processor::ProcessNextBestCaseExpansion (ofstream*  statusFile,
                                               int&       numJobsCreated
                                              )
{
  log.Level (20) << "Processor::ProcessNextBestCaseExpansion   numOfExpansions[" << numOfExpansions << "]" << endl;

  numJobsCreated = 0;

  // binaryJobs->SortByAccuracy ();
  binaryJobs->SortByGrade ();

  int  numThatWeExpanded = 0;
  int  idx = 0;

  numJobsCreated = 0;

  while  ((numThatWeExpanded < beamSize)  &&  (idx < binaryJobs->QueueSize ()))
  {
    BinaryJobPtr  jobToExpand = binaryJobs->IdxToPtr (idx);
    if  ((jobToExpand->Status () == bjDone)  &&  (jobToExpand->Features ().NumOfFeatures () > 1))
    {
      // We found a candidate;
      {
        BinaryJobListPtr  expandedJobs = jobToExpand->ExpandBestCaseBackward (binaryJobs, NULL);
        expandedJobs->Owner (false);
        UpdateExpandedJobs (statusFile, jobToExpand, expandedJobs);
        if  (expandedJobs->QueueSize () > 0)
        {
          numThatWeExpanded++;
          numJobsCreated += expandedJobs->QueueSize ();
        }
        delete  expandedJobs; expandedJobs = NULL;
      }


      if  ((numOfExpansions % 10) == 0)
      {
        // Will also Expand Forward
        BinaryJobListPtr  expandedJobs = jobToExpand->ExpandBestCaseForward (binaryJobs, NULL);
        expandedJobs->Owner (false);
        UpdateExpandedJobs (statusFile, jobToExpand, expandedJobs);
        if  (expandedJobs->QueueSize () > 0)
        {
          numThatWeExpanded++;
          numJobsCreated += expandedJobs->QueueSize ();
        }
        delete  expandedJobs;  expandedJobs = NULL;
      }
    }

    idx++;
  }


  if  ((numJobsCreated > 1)  &&  ((numOfExpansions % 10) == 0))
  {
    // We need to pick one at random
    BinaryJobPtr  jobToExpand = binaryJobs->SelectOneDoneJobAtRandomFromTop10Percent ();
    if  (jobToExpand)
    {
      jobToExpand->Status (bjExpanded);
      BinaryJobListPtr  expandedJobs = jobToExpand->ExpandBestCaseBackward (binaryJobs, NULL);
      expandedJobs->Owner (false);

      UpdateExpandedJobs (statusFile, jobToExpand, expandedJobs);
      delete  expandedJobs;  expandedJobs = NULL;
    }
  }

  if  ((numJobsCreated < 1)  &&  (binaryJobs->AreAllJobsDone ()))
  {
    // Since we have not expanded any Jobs then we are done with doing a Beam serach and now it is time to 
    // test our results.

    searchMethod = smTestResults;
    beamSize   = 1;

    *statusFile << "SearchMethod"  << "\t" << SearchMethodToStr (searchMethod)  << endl;
    *statusFile << "BeamSize"      << "\t" << beamSize                          << endl;
    ProcessTestResultsExpansion (statusFile, numJobsCreated);
  }


  log.Level (20) << "Processor::ProcessNextBestCaseExpansion     Exiting" << endl;

}  /* ProcessNextBestCaseExpansion */




void  Processor::ProcessBeamExpansion (ofstream*  statusFile,
                                       int&       numJobsCreated
                                      )
{
  log.Level (20) << "Processor::ProcessBeamExpansion" << endl;

  numJobsCreated = 0;

  binaryJobs->SortByGrade ();

  BinaryJobListPtr  newJobs = new BinaryJobList (this);

  int  firstJobCreated   = -1;
  int  numThatWeExpanded = 0;
  int  idx = 0;

  while  ((numThatWeExpanded < beamSize)  &&  (idx < binaryJobs->QueueSize ()))
  {
    BinaryJobPtr  jobToExpand = binaryJobs->IdxToPtr (idx);

    if  ((jobToExpand->Status() == bjDone)                        &&  
         (jobToExpand->Features ().NumOfFeatures () > 1)          &&
         (jobToExpand->JobId () >= firstJobAvailableForExpansion)
        )
    {
      // We found a candidate;
      BinaryJobListPtr  expandedJobs = jobToExpand->ExpandBestCaseBackward (binaryJobs, newJobs);
      expandedJobs->Owner (false);
      UpdateExpandedJobs (statusFile, jobToExpand, expandedJobs);
      if  (expandedJobs->QueueSize () > 0)
      {
        numThatWeExpanded++;
        numJobsCreated += expandedJobs->QueueSize ();

        if  (numThatWeExpanded == 1)
          firstJobCreated = expandedJobs->IdxToPtr (0)->JobId ();
      }

      delete  expandedJobs;
    }

    idx++;
  }

  if  (firstJobCreated > 0)
  {
    firstJobAvailableForExpansion = firstJobCreated;
    *statusFile << "FirstJobAvailableForExpansion"  << "\t"  << firstJobAvailableForExpansion << endl;
  }


  if  ((numThatWeExpanded < 1)  &&  (binaryJobs->AreAllJobsDone ()))
  {
    // Since we have none expanded any Jobs then we are done with doing a Beam serach and now it is time to 
    // test our results.

    searchMethod = smTestResults;
    beamSize   = 1;

    *statusFile << "SearchMethod"  << "\t" << SearchMethodToStr (searchMethod)  << endl;
    *statusFile << "BeamSize"      << "\t" << beamSize                          << endl;
    ProcessTestResultsExpansion (statusFile, numJobsCreated);
  }

  delete  newJobs;  newJobs = NULL;

}  /* ProcessBeamExpansion */





float  CalcGrowthRate (double minValue,
                       double maxValue,
                       int    numDivisions
                      )
{
  double logRange = log (maxValue) - log (minValue);
  double growthRate = exp (logRange / (double)numDivisions);
  return  float (growthRate);
}  /* CalcGrowthRate */





void  Processor::ProcessGridSearchExpansionBruitForce (ofstream*  statusFile,
                                                       int&       numJobsCreated
                                                      )
{
  *statusFile << "//" << endl
              << "//  Grid Expansion Bruit-Force  [" << numOfExpansions << "]"  << endl
              << "//  Current Date/Time           [" << osGetLocalDateTime () << "]"  << endl
              << "//" << endl;


  numJobsCreated = 0;

  int  numJobsBeforeExpansion = binaryJobs->QueueSize ();

  //cout << "We are at the expansion routine."  << endl;
  //osWaitForEnter ();
  //exit (-1);

/*
  if  (numOfExpansions == 0)
  {
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (1.0f);

    highestGradedJobs->ComputeMinAndMaxParmValues (cMin, cMax, gammaMin, gammaMax, aMin, aMax);
    gammaMin = gammaMin / (gammaGrowthRate / 2.0);
    gammaMax = gammaMax * (gammaGrowthRate / 2.0);

    cMin = cMin / (cGrowthRate / 2.0);
    cMax = cMax * (cGrowthRate / 2.0);

    aMin = 0.1f;
    aMax = 100000.0f;

    cGrowthRate     = CalcGrowthRate (cMin,     cMax,     8);
    gammaGrowthRate = CalcGrowthRate (gammaMin, gammaMax, 8);
    aGrowthRate     = CalcGrowthRate (aMin,     aMax,     16);

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile, -1,
                         cGrowthRate,      cMin,      cMax,
                         gammaGrowthRate,  gammaMin,  gammaMax,
                         aGrowthRate,      aMin,      aMax
                        );

    delete  highestGradedJobs;
  }
*/

  numJobsCreated = binaryJobs->QueueSize () - numJobsBeforeExpansion;
}  /* ProcessGridSearchExpansionBruitForce */





void  Processor::ProcessGridSearchExpansionUsfCasCor (ofstream*  statusFile,
                                                      int&       numJobsCreated
                                                     )
{
  *statusFile << "//" << endl
              << "//  Grid Expansion Most Accurate  [" << numOfExpansions       << "]"  << endl
              << "//  Current Date/Time             [" << osGetLocalDateTime () << "]"  << endl
              << endl;

  numJobsCreated = 0;

  int  numJobsBeforeExpansion = binaryJobs->QueueSize ();

  if  (numOfExpansions == 0)
  {
    // We will now take the test best C and Gamma parameters found and test them against the Test Data Set.
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (5);
    FlagJobsForTesting (statusFile, highestGradedJobs);
    delete  highestGradedJobs;
    highestGradedJobs = NULL;
  }


  else if  (numOfExpansions == 1)
  {
    // It is finally time for us to select the best job
    BinaryJobPtr theBestJobForMe = binaryJobs->HighestGradedTestJob ();
    bestA           = theBestJobForMe->AParm ();
    bestC           = theBestJobForMe->CParm ();
    bestGamma       = theBestJobForMe->GammaParm ();
    bestNumOfRounds = theBestJobForMe->NumOfRounds ();
    WriteStatusFileBestParmValues (*statusFile);
    // By not creating any new jobs this processor will terminate.
  }


  numJobsCreated = binaryJobs->QueueSize () - numJobsBeforeExpansion;
}  /* ProcessGridSearchExpansionUsfCasCor */






void  Processor::ProcessGridSearchExpansionMostAccurate (ofstream*  statusFile,
                                                         int&       numJobsCreated
                                                        )
{
  *statusFile << "//" << endl
              << "//  Grid Expansion Most Accurate  [" << numOfExpansions       << "]"  << endl
              << "//  Current Date/Time             [" << osGetLocalDateTime () << "]"  << endl
              << endl;

  numJobsCreated = 0;

  double  cMin,     cMax;
  double  gammaMin, gammaMax;
  float   aMin,     aMax;

  int  numJobsBeforeExpansion = binaryJobs->QueueSize ();

  if  (numOfExpansions == 0)
  {
    // This is our very first expansion
    BinaryJobPtr  highestGradedJob = binaryJobs->HighestGradedJob ();
    if  (highestGradedJob == NULL)
    {
      // This should not happen;  this means there are no jobs
      log.Level (-1) << endl << endl 
                     << "Processor::ProcessGridSearchExpansionMostAccurate     ***ERROR***"  << endl
                     << endl
                     << "             There are NO JOBS in the queue;  no way this should be able to happen." << endl
                     << endl;
      exit (-1);
    }


    gammaMin = highestGradedJob->GammaParm () / gammaGrowthRate;
    gammaMax = highestGradedJob->GammaParm () * gammaGrowthRate;

    cMin = 1.0;
    cMax = MaxCParmValue; 

    cGrowthRate     = 10.0f;
    gammaGrowthRate = 1.2f;
    aGrowthRate     = 1.0f;
    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  highestGradedJob->JobId (),
                         cGrowthRate,      cMin,      cMax,
                         gammaGrowthRate,  gammaMin,  gammaMax,
                         aGrowthRate,      100.0f,    100.0f
                        );

  }


  else if  (numOfExpansions == 1)
  {
    // This is our second expansion
    BinaryJobPtr  highestGradedJob = binaryJobs->HighestGradedJob ();

    cMin     = cMax     = highestGradedJob->CParm     ();
    gammaMin = gammaMax = highestGradedJob->GammaParm ();

    cMin = cMin / cGrowthRate;
    cMax = cMax * cGrowthRate;
    cGrowthRate = CalcGrowthRate (cMin, cMax, 10);

    gammaMin = gammaMin / gammaGrowthRate;
    gammaMax = gammaMax * gammaGrowthRate;
    gammaGrowthRate = CalcGrowthRate (gammaMin, gammaMax, 8);

    aGrowthRate = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile, highestGradedJob->JobId (),
                         cGrowthRate,       cMin,      cMax,
                         gammaGrowthRate,   gammaMin,  gammaMax,
                         aGrowthRate,       100.0f,    100.0f
                        );
  }


  else if  (numOfExpansions == 2)
  {
    // This is our third expansion;  we will now do a very FINE search arround Gamma and "C".

    BinaryJobPtr  highestGradedJob = binaryJobs->HighestGradedJob ();

    cMin = highestGradedJob->CParm () / cGrowthRate;
    cMax = highestGradedJob->CParm () * cGrowthRate;

    gammaMin = highestGradedJob->GammaParm () / gammaGrowthRate;
    gammaMax = highestGradedJob->GammaParm () * gammaGrowthRate;
 
    // Now calc the Growth rates for the next expansion.
    cGrowthRate     = CalcGrowthRate (cMin,     cMax,     8);
    gammaGrowthRate = CalcGrowthRate (gammaMin, gammaMax, 8);
    aGrowthRate     = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  highestGradedJob->JobId (),
                         cGrowthRate,     cMin,      cMax,
                         gammaGrowthRate, gammaMin,  gammaMax,
                         aGrowthRate,     100,       100
                       );
  }


  else if  (numOfExpansions == 3)
  {
    // We will now take the test best C and Gamma parameters found and test them against the Test Data Set.
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (64);
    FlagJobsForTesting (statusFile, highestGradedJobs);
    delete  highestGradedJobs;
    highestGradedJobs = NULL;
  }


  else if  (numOfExpansions == 4)
  {
    // We will now take best test results and settle on that "C" and "gamma".  From this point forwards 
    // we are interested in tuning the "A" parameter only using the "C" and "gamma that we decided on.

    BinaryJobPtr  highestGradedTestJob = binaryJobs->HighestGradedTestJob ();
    bestC           = highestGradedTestJob->CParm ();
    bestGamma       = highestGradedTestJob->GammaParm ();
    bestNumOfRounds = highestGradedTestJob->NumOfRounds ();

    WriteStatusFileBestParmValues (*statusFile);

    cGrowthRate     = 1.0f;
    gammaGrowthRate = 1.0f;

    aMin = 0.1f;
    aMax = 10000000.0f;
    aGrowthRate = CalcGrowthRate (aMin, aMax, 64);

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  highestGradedTestJob->JobId (),
                         cGrowthRate,     bestC,      bestC,
                         gammaGrowthRate, bestGamma,  bestGamma,
                         aGrowthRate,     aMin,       aMax
                        );
  }


  else if  (numOfExpansions == 5)
  {
    // We will locate the job that has the smallest difference between accuracy and predicted probability and 
    // then perform a finner search arround its neighborhood.

    // Because  bestC and bestGamma are being writen then read from a status file some of the lesset insignificant
    // digits might get lost;  so we add and subtract 0.0000001 to bestGamma.
    BinaryJobListPtr  targetJobs = binaryJobs->LookUpByParameters ((bestC     - 0.00001), (bestC     + 0.00001), 
                                                                   (bestGamma - 0.00001), (bestGamma + 0.00001)
                                                                  );
    BinaryJobPtr  lowestDeltaProbAccJob = targetJobs->LowestDeltaAccuracyProbJob ();

    aMin = lowestDeltaProbAccJob->AParm () / aGrowthRate;
    aMax = lowestDeltaProbAccJob->AParm () * aGrowthRate;

    aGrowthRate = CalcGrowthRate (aMin, aMax, 64);
    cGrowthRate     = 1.0f;
    gammaGrowthRate = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  lowestDeltaProbAccJob->JobId (),
                         cGrowthRate,     bestC,      bestC,
                         gammaGrowthRate, bestGamma,  bestGamma,
                         aGrowthRate,     aMin,       aMax
                        );


    delete  targetJobs;
    targetJobs = NULL;
  }

  else if  (numOfExpansions == 6)
  {
    // We will locate the job that has the smallest difference between accuracy and predicted probability and 
    // then perform a finer search arround its neighborhod.

    // Because  bestC and bestGamma are being written to the status file and then read back from a status file some of the least insignificant
    // digits might get lost;  so we add and subtract 0.0000001 to bestGamma.
    BinaryJobListPtr  targetJobs = binaryJobs->LookUpByParameters ((bestC     - 0.00001), (bestC     + 0.00001), 
                                                                   (bestGamma - 0.00001), (bestGamma + 0.00001)
                                                                  );
    BinaryJobPtr  lowestDeltaProbAccJob = targetJobs->LowestDeltaAccuracyProbJob ();

    aMin = lowestDeltaProbAccJob->AParm () / aGrowthRate;
    aMax = lowestDeltaProbAccJob->AParm () * aGrowthRate;

    // Now calc growth rate we will use for this pass.
    aGrowthRate = CalcGrowthRate (aMin, aMax, 64);
    cGrowthRate     = 1.0f;
    gammaGrowthRate = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  lowestDeltaProbAccJob->JobId (),
                         cGrowthRate,     bestC,      bestC,
                         gammaGrowthRate, bestGamma,  bestGamma,
                         aGrowthRate,     aMin,       aMax
                         );


    delete  targetJobs;
    targetJobs = NULL;
  }

  else if  (numOfExpansions == 7)
  {
    // Will now test the 64 jobs with smallest deltaAccuracyProb
    BinaryJobListPtr  targetJobs = binaryJobs->LookUpByParameters ((bestC     - 0.00001), (bestC     + 0.00001), 
                                                                   (bestGamma - 0.00001), (bestGamma + 0.00001)
                                                                  );
    targetJobs->SortDeltaProbAccuarcy ();

    BinaryJobListPtr  jobsToTest = new BinaryJobList (this);
    jobsToTest->Owner (false);

    BinaryJobList::iterator  idx;
    for  (idx = targetJobs->begin ();  (idx != targetJobs->end ())  &&  (jobsToTest->QueueSize () < 64);  idx++)
    {
      BinaryJobPtr j = *idx;
      if  (binaryJobs->LookUpByParameters (j->Features (),
                                           j->NumOfRounds (),
                                           j->CParm (), 
                                           j->GammaParm (), 
                                           j->AParm (), 
                                           true
                                          ) == NULL)
        jobsToTest->PushOnBack (j);
    }

    FlagJobsForTesting (statusFile, jobsToTest);
   
    delete  jobsToTest;
    jobsToTest = NULL;

    delete  targetJobs;
    targetJobs = NULL;
  }


  else if  (numOfExpansions == 8)
  {
    // It is finally time for us to select the best job
    BinaryJobListPtr  targetJobs = binaryJobs->LookUpByParameters ((bestC     - 0.00001), (bestC     + 0.00001), 
                                                                   (bestGamma - 0.00001), (bestGamma + 0.00001)
                                                                  );
    BinaryJobPtr theBestJobForMe = targetJobs->LowestDeltaAccuracyProbTestJob ();
    bestA = theBestJobForMe->AParm ();
    WriteStatusFileBestParmValues (*statusFile);
    delete  targetJobs;  targetJobs = NULL;
    // By not creating any new jobs this processor will terminate.
  }


  numJobsCreated = binaryJobs->QueueSize () - numJobsBeforeExpansion;
}  /* ProcessGridSearchExpansionMostAccurate */






void  Processor::ProcessGridSearchExpansionFastestFromBest (ofstream*  statusFile,
                                                            int&       numJobsCreated
                                                           )
{
  *statusFile << "//" << endl
              << "//  Grid Expansion    [" << numOfExpansions       << "]"  << endl
              << "//  Current Date/Time [" << osGetLocalDateTime () << "]"  << endl
              << "//" << endl;

  numJobsCreated = 0;

  float   aMin,     aMax;
  double  cMin,     cMax;
  double  gammaMin, gammaMax;

  int  numJobsBeforeExpansion = binaryJobs->QueueSize ();

  if  (numOfExpansions == 0)
  {
    // This is our very first expansion

    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (3.0,  // Get the 3% highest graded jobs
                                                                           10    // or a minimum of 10;  which ever is greater.
                                                                          );  

    
    // We wish to eliminate the really slow jobs now;  any job that runs longer than twice
    // the fastest job should be eliminated.
    BinaryJobPtr  fastestJob = highestGradedJobs->FastestJob ();
    double  fastestTime = fastestJob->ProcessingTime ();
    double  acceptableTime = Max (5.0, fastestTime * 2.0);
    BinaryJobListPtr  fastestJobs = highestGradedJobs->ExtractByMinimumProcessintTime (acceptableTime);

    fastestJobs->ComputeMinAndMaxParmValues (cMin, cMax, gammaMin, gammaMax, aMin, aMax);
    gammaMin = gammaMin / 2;
    gammaMax = gammaMax * 2;

    cMin = 1.0;
    cMax = MaxCParmValue;

    cGrowthRate     = 10.0f;
    gammaGrowthRate = 1.2f;
    aGrowthRate     = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  -1,
                         cGrowthRate,     cMin,      cMax,
                         gammaGrowthRate, gammaMin,  gammaMax,
                         aGrowthRate,     100.0f,    100.0f
                        );

    delete  fastestJobs;
    delete  highestGradedJobs;
  }


  else if  (numOfExpansions == 1)
  {
    // This is our second expansion
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (1.0f, // Get the 1.0% highest accuracy jobs
                                                                           10    // or minimum 10 best which ever is greater.
                                                                          );

    BinaryJobPtr  fastestJob = highestGradedJobs->FastestJob ();
    double  fastestTime = fastestJob->ProcessingTime ();
    double  acceptableTime = Max (2.0, fastestTime * 1.3);
    BinaryJobListPtr  fastestJobs = highestGradedJobs->ExtractByMinimumProcessintTime (acceptableTime);
    fastestJobs->ComputeMinAndMaxParmValues (cMin, cMax, gammaMin, gammaMax, aMin, aMax);

    cMin = cMin / cGrowthRate;
    cMax = cMax * cGrowthRate;
    cGrowthRate = CalcGrowthRate (cMin, cMax, 12);

    gammaMin = gammaMin / gammaGrowthRate;
    gammaMax = gammaMax * gammaGrowthRate;
    gammaGrowthRate = CalcGrowthRate (gammaMin, gammaMax, 12);

    aGrowthRate = 1.0f;

    WriteStatusFileGrowthRates (*statusFile);

    CreateParameterJobs (statusFile,  -1,
                         cGrowthRate,     cMin,      cMax,
                         gammaGrowthRate, gammaMin,  gammaMax,
                         aGrowthRate,     100.0f,    100.0f
                        );


    delete  fastestJobs;
    delete  highestGradedJobs;
  }


  else if  (numOfExpansions == 2)
  {
    // This is our third expansion
    //
    // We will select the best jobs so far with respect top accuracy and running time.   
    // and for the best 10 we will vary the 'A' parameter to try and get the 'AvgPredProb'
    // in line with training 'accuracy'.
    //
    binaryJobs->SortByGrade ();
    for  (int x = 0;  ((x < 10)  &&  (x < binaryJobs->QueueSize () ));  x++)
    {
      BinaryJobPtr  j = binaryJobs->IdxToPtr (x);
      binaryJobs->FindNeighboringParameterValues (j, cMin, cMax, 2.0f, gammaMin, gammaMax, 2.0f, aMin, aMax, 1.0);

      cGrowthRate     = CalcGrowthRate (cMin,     cMax,     3);
      gammaGrowthRate = CalcGrowthRate (gammaMin, gammaMax, 3);
      aGrowthRate = 10.0f;

      WriteStatusFileGrowthRates (*statusFile);

      CreateParameterJobs (statusFile, j->JobId (),
                           cGrowthRate,     cMin,      cMax,
                           gammaGrowthRate, gammaMin,  gammaMax,
                           aGrowthRate,     0.1f,      100000.0f
                         );
    }
  }


  else if  (numOfExpansions == 3)
  {
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (0.5f, 10);
    highestGradedJobs->ComputeMinAndMaxParmValues (cMin, cMax, gammaMin, gammaMax, aMin, aMax);
    aMin = Max (0.00001f, aMin / 5.0f);
    aMax = Max (0.00001f, aMax * 5.0f);
    aGrowthRate = CalcGrowthRate (aMin, aMax, 20);

    highestGradedJobs->SortByGrade ();

    for  (int x = 0;  ((x < 10)  &&  (x < highestGradedJobs->QueueSize () ));  x++)
    {
      BinaryJobPtr  j = highestGradedJobs->IdxToPtr (x);

      cMin     = cMax     = j->CParm ();
      gammaMin = gammaMax = j->GammaParm ();

      CreateParameterJobs (statusFile, j->JobId  (),
                           1.03f,        cMin,      cMax,
                           1.03f,        gammaMin,  gammaMax,
                           aGrowthRate,  aMin,      aMax
                         );
    }

    delete  highestGradedJobs;
  }


  else if  (numOfExpansions == 4)
  {
    // This is our fifth expansion
    //
    //  We are now interested in refining the 'A' probability.  The best job for each 'C' and
    //  'gamma' pair of parmaters are selected and a search of the local 'A' paraeter area.
    //
    BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (0.3f, 10);
    highestGradedJobs->SortDeltaProbAccuarcy ();

    for  (int x = 0;  ((x < 10)  &&  (x < highestGradedJobs->QueueSize () ));  x++)
    {
      BinaryJobPtr  j = highestGradedJobs->IdxToPtr (x);
      highestGradedJobs->FindNeighboringParameterValues (j, cMin, cMax, 2.0f, gammaMin, gammaMax, 2.0f, aMin, aMax, 2.0);
      cMin     = cMax     = j->CParm ();
      gammaMin = gammaMax = j->GammaParm ();
      aGrowthRate = CalcGrowthRate (aMin, aMax, 20);
      cGrowthRate     = 1.03f;
      gammaGrowthRate = 1.03f;
      WriteStatusFileGrowthRates (*statusFile);


      CreateParameterJobs (statusFile, j->JobId (),
                           cGrowthRate,     cMin,      cMax,
                           gammaGrowthRate, gammaMin,  gammaMax,
                           aGrowthRate,     aMin,      aMax
                         );
    }
  }


  else if  (numOfExpansions == 5)
  {
    //if  (testData)
    {
      BinaryJobListPtr  highestGradedJobs = binaryJobs->ExtractHighestGrade (0.3f, -1); 
      highestGradedJobs->SortDeltaProbAccuarcy ();

      BinaryJobListPtr  jobsToTest = new BinaryJobList (this);
      jobsToTest->Owner (false);
      for  (int x = 0;  (x < 64)  &&  (x < highestGradedJobs->QueueSize ());  x++)
        jobsToTest->PushOnBack (highestGradedJobs->IdxToPtr (x));
     
      FlagJobsForTesting (statusFile, jobsToTest);

      delete  jobsToTest;          jobsToTest        = NULL;
      delete  highestGradedJobs;   highestGradedJobs = NULL;
    }
  }

  else if  (numOfExpansions == 6)
  {
    // By not creating any new Jobs the search will terminate then.
  }


  numJobsCreated = binaryJobs->QueueSize () - numJobsBeforeExpansion;
}  /* ProcessGridSearchExpansionFastestFromBest */





void  Processor::ProcessTestResultsExpansion (ofstream*  statusFile,
                                              int&       numJobsCreated
                                             )
{
  log.Level (10) << "Processor::ProcessTestResultsExpansion"  << endl;
  // This expansion only gets run once.  Its job is to perform a test on the results of the feature test.  
  // We will locate the jobs that have the highest training accuracy by feature count.  Then for each one 
  // of them we will test against the test data set.

  BinaryJobListPtr  jobsToTest = binaryJobs->CreateTestJobsForHighetsGradePerFeatureCount ();
  jobsToTest->Owner (false);

  BinaryJobList::iterator idx;
  for  (idx = jobsToTest->begin ();   idx != jobsToTest->end ();  idx++)
  {
    BinaryJobPtr  j = *idx;
    binaryJobs->PushOnBack (j);
    *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
  }

  *statusFile << "NextJobId" << "\t" << nextJobId << endl;
  numJobsCreated = jobsToTest->QueueSize ();

  searchMethod = smTestResults;
  *statusFile << "SearchMethod"  << "\t" << SearchMethodToStr (searchMethod)  << endl;
 
  delete  jobsToTest;

  log.Level (20) << "Processor::ProcessTestResultsExpansion     exiting"  << endl;
}  /* ProcessTestResultsExpansion */





void  Processor::ProcessTestJobsAndPickTheBest (ofstream*  statusFile,
                                                int&       numJobsCreated
                                               )
{
  log.Level (10) << "Processor::ProcessTestJobsAndPickTheBest"  << endl;

  // This will be handled seperastely in a different process. 
  // by not creating any jobs we can now terminate this search.

  log.Level (20) << "Processor::ProcessTestJobsAndPickTheBest         exiting"  << endl;
}  /* ProcessTestJobsAndPickTheBest */





void  Processor::ProcessNextExpansion (ofstream*  statusFile)
{
  log.Level (20) << "Processor::ProcessNextExpansion" << endl;

  ReportCpuTimeUsed (statusFile);


  if  (searchMethod == smOnePassOnly)
  {
    // We are processing Final Results;  In this case there is no expansion.
    // by not creating any new jobs it will cause the process to terminate.
  }

  else if  (searchMethod == smBestCaseNext)
  {
    if  (expansionsSinceHighGrade >= expansionLimit)
    {
      // SwithOverToBeamSearch
      searchMethod = smBeam;
      beamSize   = Max (5, beamSize);
      *statusFile << "SearchMethod"  << "\t" << SearchMethodToStr (searchMethod)  << endl;
      *statusFile << "BeamSize"      << "\t" << beamSize                          << endl;
    }
  }

  else if  (searchMethod == smTestResults)
  {
    BinaryJobListPtr  testJobs = binaryJobs->ExtractTestJobs ();
    testJobs->Owner (false);
    if  ((testJobs != NULL)  &&  (testJobs->QueueSize () > 0))
    {
      // We have already gereated and ran the Test Jobs.  We can now pick the best.
      searchMethod = smPickTheBest;
      *statusFile << "SearchMethod"  << "\t" << SearchMethodToStr (searchMethod)  << endl;
    }
    delete  testJobs;
  }


  int  numNewJobsCreated = 0;

  switch  (searchMethod)
  {
    case  smBestCaseNext:  ProcessNextBestCaseExpansion (statusFile, numNewJobsCreated);
                           break;

    case  smBeam:          ProcessBeamExpansion (statusFile, numNewJobsCreated);
                           break;
    
    case  smTestResults:   ProcessTestResultsExpansion (statusFile, numNewJobsCreated);
                           break;

    case  smPickTheBest:   ProcessTestJobsAndPickTheBest (statusFile, numNewJobsCreated);
                           break;

    case  smGrid:          {
                             if  (featureSelection->Config ()->ModelType () == Model::mtUsfCasCor)
                             {
                               ProcessGridSearchExpansionUsfCasCor (statusFile, numNewJobsCreated);
                             }

                             else if  
                                 ((featureSelection->ParamSelCriteria () == jscBruitForce)  ||  
                                  (featureSelection->MajorStep () ==  msGenerateBruitSvmSearch)
                                 )
                             {
                               ProcessGridSearchExpansionBruitForce (statusFile, numNewJobsCreated);
                             }

                             else if  (featureSelection->ParamSelCriteria () == jscMostAccurate)
                             {
                               ProcessGridSearchExpansionMostAccurate (statusFile, numNewJobsCreated);
                             }

                             else
                             {
                               ProcessGridSearchExpansionFastestFromBest (statusFile, numNewJobsCreated);
                             }
                           }
                           break;
  }
 
  if  (numNewJobsCreated > 0)
  {
    expansionsSinceHighGrade++;
    numOfExpansions++;

    *statusFile << "NumOfExpansions"    << "\t" << numOfExpansions           << endl
                << "ExpSinceHighGrade"  << "\t" << expansionsSinceHighGrade  << endl;
  }
  else
  {
    noMoreJobsLeftToExpand = true;
  }

}  /* ProcessNextExpansion */




void  Processor::GenerateFinalResultsReport ()
{
  log.Level (10) << "Processor::GenerateFinalResultsReport" << endl;
  ofstream r (resultFileName.Str ());
  
  r << "Current Time     [" << osGetLocalDateTime ()                                            << "]" << endl
    << "Class1           [" << Class1Name ()                                                    << "]" << endl
    << "Class2           [" << Class2Name ()                                                    << "]" << endl
    << "Model Type       [" << config->ModelTypeStr ()                                          << "]" << endl
    << "Config Parms     [" << config->ModelParameterCmdLine ()                                 << "]" << endl
    << "Train File       [" << featureSelection->TrainingDataFileName ()                        << "]" << endl
    << "Test  File       [" << featureSelection->TestDataFileName ()                            << "]" << endl
    << "Num Expansions   [" << numOfExpansions                                                  << "]" << endl
    << "Grading Method   [" << GradingMethodToStr (featureSelection->GradingMethod ())          << "]" << endl
    << "TotalCpuTimeUsed [" << TotalCpuTimeUsed ()                                              << "]" << endl
    << "ParamSelCriteria [" << JobSelectionCriteriaType (featureSelection->ParamSelCriteria ()) << "]" << endl
    << endl 
    << endl;

  
  r << "Training Data Stats" << endl;
  trainingData->PrintClassStatistics (r);

  if  (testData)
  {
    r << endl
      << endl
      << "Test Data Stats" << endl;
    testData->PrintClassStatistics (r);
  }

  r << endl << endl;

  binaryJobs->SortByTestGrade (featureSelection->FeatureCountPrefSmall ());
  binaryJobs->ReportResults (r);

  if  (searchType == stFeatureSelectionSearch)
  {
    r << endl << endl;
    FeatureImpact featureImpact (*binaryJobs);
    featureImpact.Report (r);
    r << endl << endl;
    featureImpact.ReportHTML (r);
  }

  r.close ();
}  /* GenerateFinalResultsReport */


FeatureImpactPtr  Processor::GetFeatureImpact ()
{
  FeatureImpactPtr  fi = new FeatureImpact (*binaryJobs);
  return  fi;
}



void  Processor::GenrateSvmResponseSheet ()
{
  ofstream  r (resultFileName.Str ());

  binaryJobs->GenrateSvmResponseSheet (r);

  r.close ();
}  /* GenrateSvmResponseSheet */



void  Processor::GenerateFinalResultsReportHTML ()
{
  log.Level (10) << "Processor::GenerateFinalResultsReportHTML" << endl;

  KKStr  htmlFileName = osRemoveExtension (resultFileName) + ".html";

  KKStr  captionStr = SearchTypeToStr (searchType) + " search results for "; 

  KKStr  titleStr   = "";
  if  (searchType == stFeatureSelectionSearch)
    titleStr = "Feature Selection ";
  else
    titleStr = "Parameter Tunning ";


  KKStr  class1Name = Class1Name ();
  if  (class1Name.Empty ())
    class1Name = "&nbsp";

  KKStr  class2Name = Class2Name ();
  if  (class2Name.Empty ())
    class2Name = "&nbsp";

  if  (class1 == NULL)
  {
    captionStr << "All Classes";
    titleStr   << "All Classes";
  }
  else
  {
    captionStr << Class1Name () << " and " << Class2Name ();
    titleStr   << Class1Name () << " and " << Class2Name ();
  }


  KKStr  paramSelCriteriaStr = JobSelectionCriteriaType (featureSelection->ParamSelCriteria ());

  ofstream r (htmlFileName.Str ());

  r << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">" << endl
    << "<html  xmlns=\"http://www.w3.org/1999/xhtml\">"                   << endl
    << "<head>"                                                           << endl
    << "<title>" << titleStr << "</title>"                                << endl
    //    <link rel="StyleSheet" href="../../sipper.css" type="text/css" />
    << "</head>"                                                          << endl
    << "<body>"                                                           << endl
    <<                                                                       endl
    << "<center>"                                                         << endl
    << "<h1>Run Time Parameters</h1>"                                     << endl
    << "<table align=\"center\" border=\"2\" cellpadding=\"4\">"          << endl
    << "<caption>" << captionStr << "</caption>"                          << endl
    << "<tr><td>Run Time</td>"          << "<td>" << osGetLocalDateTime ()                                    << "</td></tr>" << endl
    << "<tr><td>Search Type</td>"       << "<td>" << SearchTypeToStr (searchType)                             << "</td></tr>" << endl
    << "<tr><td>Class1</td>"            << "<td>" << class1Name                                               << "</td></tr>" << endl
    << "<tr><td>Class2</td>"            << "<td>" << class2Name                                               << "</td></tr>" << endl
    << "<tr><td>Config Parms</td>"      << "<td>" << config->ModelParameterCmdLine ()                         << "</td></tr>" << endl
    << "<tr><td>Train File</td>"        << "<td>" << featureSelection->TrainingDataFileName ()                << "</td></tr>" << endl
    << "<tr><td>Test  File</td>"        << "<td>" << featureSelection->TestDataFileName ()                    << "</td></tr>" << endl
    << "<tr><td>Num Expansions</td>"    << "<td>" << numOfExpansions                                          << "</td></tr>" << endl
    << "<tr><td>Grading Method</td>"    << "<td>" << GradingMethodToStr (featureSelection->GradingMethod ())  << "</td></tr>" << endl
    << "<tr><td>Cpu Time Used</td>"     << "<td>" << TotalCpuTimeUsed ()                                      << "</td></tr>" << endl
    << "<tr><td>ParamSelCriteria</td>"  << "<td>" << paramSelCriteriaStr                                      << "</td></tr>" << endl
    << "</table>" << endl
    << endl 
    << endl;

  r << "<br />" << endl
    << "<br />" << endl
    << "<h1>Trainning and Test data stats</h1>" << endl
    << "<br />" 
    << "<h2>Training Data Stats:</h2>" << endl;
  trainingData->PrintClassStatisticsHTML (r);

  if  (testData)
  {
    r << "<br />" << endl
      << "<h2>Test Data Stats:</h2>" << endl;
    testData->PrintClassStatisticsHTML (r);
  }

  r << "<br />" << endl 
    << "<br />" << endl;

  binaryJobs->SortByTestGrade (featureSelection->FeatureCountPrefSmall ());
  binaryJobs->ReportResultsHTML (r);

  if  (searchType == stFeatureSelectionSearch)
  {
    FeatureImpact featureImpact (*binaryJobs);

    r << "<br />" << endl
      << "<br />" << endl
      << "<h1>Selected Features.</h1>"  << endl;

    FeatureNumList  selFeatures = SelectBestFeatures ();
    r << "Selected Features:" << selFeatures.ToString () << endl;

    config->SetFeatureNums (selFeatures);

    r << "<br />" << endl
      << "<br />" << endl
      << "<h1>Feature Impact Statistics.</h1>"  << endl;
    featureImpact.ReportHTML (r);
  }

  if  (this->ByBinaryClasses ())
    GenerateFinalResultsReportValidationHTML (r);
  
  r << "</body>"   << endl
    << "</html>"   << endl
    << "</center>" << endl;

  r.close ();
}  /* GenerateFinalResultsReportHTML */



void  Processor::GenerateFinalResultsReportValidationHTML (ostream& r)
{
  r << "<br /><br />" << endl
    << "Validation Report" << endl
    << "<br />"            << endl
    << "<br />"            << endl
    << "Configuration Parameters:"  << " " << config->ModelParameterCmdLine ()  << endl
    << "Selected Features:"         << " " << config->GetFeatureNums ().ToString () << endl
    << "<br />"            << endl
    << "<br />"            << endl;

  if  (featureSelection->ValidationData () == NULL)
  {
    r << "***  No Validation data   ***" << endl
      << "<br /><br />"
      << endl;
    return;
  }

  FeatureVectorListPtr  validationData = BuildOurBinaryFeatureData (featureSelection->ValidationData ());
  if  (!validationData)
    return;

  bool  cancelFlag = false;
  CrossValidation  cv (config, trainingData, classesThisProcess, 1, true, fileDesc, log, cancelFlag);
  cv.RunValidationOnly (validationData, NULL);


  r << "<br /><br />" << endl
    << "Confusion Matrix" << endl;
  cv.ConfussionMatrix ()->PrintConfusionMatrixHTML (r);

  r << "<br /><br />" << endl
    << "Accuracy by Probability" << endl;
  cv.ConfussionMatrix ()->PrintAccuracyByProbByClassHTML (r);

  r << "<br /><br />" << endl
    << "Probability Confusion Matrix" << endl;
  cv.ConfussionMatrix ()->PrintConfusionMatrixAvgPredProbHTML (r);

  delete  validationData;  validationData = NULL;
}  /* GenerateFinalResultsReportValidationHTML */



void  Processor::ProcessRestart ()
{
  log.Level (10) << "ProcessRestart" << endl;

  Block ();

  StatusFileLoad ();

  if  (this->status != fsDone)
  {
    ofstream* statusFile = OpenStatusFile (ios::app);

    status = fsNotStarted;
    *statusFile << "ReStart" << endl;
    *statusFile << "Status"  << "\t" << ProcessorStatusToStr (fsNotStarted) << endl;


    BinaryJobList::iterator  idx;
    for  (idx = binaryJobs->begin ();  idx != binaryJobs->end ();   idx++)
    {
      BinaryJobPtr  j = *idx;
      if  (j->Status () == bjStarted)
      {
        j->Status (bjOpen);
        *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
      }
    }

    statusFile->flush ();
    statusFile->close ();
    delete  statusFile;  statusFile = NULL;
  }
 
  EndBlock ();
}  /* ProcessRestart */



void  Processor::SetQuitRunningFlag ()
{
  Block ();
  ofstream* statusFile = OpenStatusFile (ios::app);

  *statusFile << "QuitRunning" << endl;
  quitRunning = true;

  statusFile->flush ();
  statusFile->close ();
  delete  statusFile;  statusFile = NULL;

  EndBlock ();
}  /* SetQuitRunningFlag */




BinaryJobListPtr  Processor::GetNextSetOfJobs (BinaryJobListPtr  completedJobs)
{
  log.Level (20) << "Processor::GetNextSetOfJobs." << endl;

  Block  ();

  StatusFileRefresh ();
 
  if  (completedJobs)
  {
    // We will first write out results of jobs that have been completed,
    ofstream* statusFile = OpenStatusFile (ios::app);

    double  totalCpuSecsForThisBatch = 0.0;
    int     totalNumJobsThisbatch    = 0;

    BinaryJobList::iterator  idx;
    for  (idx = completedJobs->begin ();  idx != completedJobs->end ();  idx++)
    {
      BinaryJobPtr  j = *idx;

      totalCpuSecsForThisBatch += j->ProcessingTime ();
      totalNumJobsThisbatch++;

      *statusFile << j->JobTypeStr () << "\t" << j->ToStatusStr () << endl;
      if  (j->Grade () > highGrade)
      {
        highGrade = j->Grade ();
        expansionsSinceHighGrade = 0;
        *statusFile << "HighGrade"          << "\t" << StrFormatDouble (highGrade, "zz0.000000")  << endl;
        *statusFile << "ExpSinceHighGrade"  << "\t" << expansionsSinceHighGrade << endl;
      }

      BinaryJobPtr  existingJob = binaryJobs->LookUpByJobId (j->JobId ());
      if  (existingJob)
      {
        existingJob->ReFresh (*j);
      }
      else
      {
        delete  statusFile;  statusFile = NULL;
        log.Level (-1) << endl << endl << endl
                       << "GetNextSetOfJobs     *** ERROR ***"  << endl
                       << endl
                       << "           Could not locate Job[" << j->JobId () << "]" << endl
                       << endl;
        return  NULL;
      }
    }

    if  (totalCpuSecsForThisBatch > 5.0)
    {
      double secsPerJob = totalCpuSecsForThisBatch / (double)totalNumJobsThisbatch;
      int  newNumJobsAtATime = Max (1, (int)(5.0 / secsPerJob));
      if  (newNumJobsAtATime < numJobsAtATime)
      {
        numJobsAtATime = newNumJobsAtATime;
        *statusFile << "NumJobsAtATime" << "\t" << numJobsAtATime << endl;
      }
    }

    else if  (totalCpuSecsForThisBatch < 0.4)
    {
      double secsPerJob = totalCpuSecsForThisBatch / (double)totalNumJobsThisbatch;
      int  newNumJobsAtATime = Max (1, (int)(0.5 + 0.4 / totalCpuSecsForThisBatch));
      newNumJobsAtATime = Min (newNumJobsAtATime, 20);
      if  (newNumJobsAtATime > numJobsAtATime)
      {
        numJobsAtATime = Min (newNumJobsAtATime, numJobsAtATime + 2);
        *statusFile << "NumJobsAtATime" << "\t" << numJobsAtATime << endl;
      }
    }

    statusFile->close ();
    delete  statusFile;  statusFile = NULL;
  }

  BinaryJobListPtr  jobsToExecute = new BinaryJobList (this);
  jobsToExecute->Owner (false);

  if  (!quitRunning)
  {
    ofstream* statusFile = OpenStatusFile (ios::app);

    BinaryJobPtr  nextJob = binaryJobs->LocateOpenJob ();

    if  (!nextJob)
    {
      if  ((binaryJobs->AreAllJobsDone ())  ||  (searchMethod == smBestCaseNext))
      {
        // There are no jobs to do;  we will have to expand some existing jobs then
        ProcessNextExpansion (statusFile);
        nextJob = binaryJobs->LocateOpenJob ();
      }
      else
      {
        // There are still some jobs that are running.   We are going to go to
        // for now and try again later.
        //  
        // By leaving  "nextJob = NULL" we will drop straight through the rest of
        // this method and return to the caller with 'jobsToExecute' empty
        // signaling that it will need to sleep for a while before calling 
        // us again.
        double  currentCpuTime = osGetSystemTimeUsed ();
        double  cpuTimeUsed = fabs (currentCpuTime - lastCpuTimeReported);
        if  (cpuTimeUsed > 10.0)
        {
          // In case we crash bvefore we get a chance to write this out.  Lets report our current usage.
          ReportCpuTimeUsed (statusFile);
        }
      }
    }

    while  (nextJob  &&  (jobsToExecute->QueueSize () < numJobsAtATime))
    {
      jobsToExecute->PushOnBack (nextJob);
      nextJob->Status (bjStarted);
      *statusFile << "JobStatusChange" << "\t" << nextJob->JobId () << "\t" << nextJob->StatusStr () << endl;
      nextJob = binaryJobs->LocateOpenJob ();
    }

    statusFile->close ();
    delete  statusFile;  statusFile = NULL;
  }

  if  (jobsToExecute->QueueSize () < 1)
  {
    delete  jobsToExecute;
    jobsToExecute = NULL;
  }

  EndBlock ();

  log.Level (20) << "Processor::GetNextSetOfJobs   Exiting." << endl;

  return  jobsToExecute;
}  /* GetNextSetOfJobs */




bool  Processor::AreAllJobsDone ()
{
  if  (binaryJobs)
  {
    return  binaryJobs->AreAllJobsDone ();
  }
  else
  {
    // Will assume false for now.
    return  false;
  }
}  /* AreAllJobsAreDone */




void   Processor::Run ()
{
  log.Level (10) << "Processor::Run." << endl;

  bool  keepOnRunning = true;

  BinaryJobListPtr  executedJobs  = NULL;
  BinaryJobListPtr  jobsToExecute = GetNextSetOfJobs (NULL);

  while  (keepOnRunning  &&  (!quitRunning))
  {
    if  (jobsToExecute)
    {
      delete  executedJobs;  executedJobs = NULL;

      executedJobs = new BinaryJobList (this);
      executedJobs->Owner (true);

      BinaryJobList::iterator  idx;
      for  (idx = jobsToExecute->begin ();  idx != jobsToExecute->end ();  idx++)
      {
        BinaryJobPtr  j = *idx;
        j->EvaluateNode ();
        executedJobs->PushOnBack (BinaryJob::CreateDuplicateJob (*j));
      }
      delete  jobsToExecute;  jobsToExecute = NULL;
    }
    else
    {
      if  (!(binaryJobs->JobsStillRunning ()))
      {
        keepOnRunning = false;
      }
      else
      {
        // We will sleep for a bit until there are more jobs to run
        log.Level (10) << "Processor::Run     No jobs avaialble to run; will sleep a bit." << endl;
        osSleep ((float)(10 + rand () % 10)); 
      }
    }

    if  (keepOnRunning)
    {
      delete  jobsToExecute;
      jobsToExecute = GetNextSetOfJobs (executedJobs);
      delete  executedJobs;
      executedJobs = NULL;
  }
  }

  delete  jobsToExecute;  jobsToExecute = NULL;
 

  if   (noMoreJobsLeftToExpand  &&  ((status != fsDone)))
  {
    Block ();

    StatusFileRefresh ();

    if  (status != fsDone)
    {
      // GenerateFinalResultsReport ();

      if  (this->FeatureSelection ()->MajorStep () == msGenerateBruitSvmSearch)
        GenrateSvmResponseSheet ();
      else
        GenerateFinalResultsReportHTML ();
      
      status = fsDone;
    
      ofstream*  statusFile = OpenStatusFile (ios::app);

      *statusFile << "Status" << "\t" << ProcessorStatusToStr (status) << endl;

      ReportCpuTimeUsed (statusFile);

      statusFile->close ();
      delete  statusFile;  statusFile = NULL;
    }

    EndBlock ();
  }

  log.Level (10) << "Processor::Run    Exiting." << endl;
}  /* Run */




FeatureNumList  Processor::MergeKBestFeatureSelections (int  k,
                                                        int  minNumOfFeatures
                                                       )
{
  //delete  binaryJobs;  binaryJobs = NULL;
  //binaryJobs = BinaryJobList::LoadFromResultsFile (this, resultFileName);

  binaryJobs->SortByTestGrade (featureSelection->FeatureCountPrefSmall ());

  FeatureNumList  mergedFeatures (fileDesc);

  int  numFeatureSetsMerged = 0;
  int  nextIdx = 0;

  while  ((nextIdx < binaryJobs->QueueSize ())  &&
          ((mergedFeatures.NumOfFeatures () < minNumOfFeatures)  ||  (numFeatureSetsMerged < k))
         )
  {
    BinaryJobPtr  job = binaryJobs->IdxToPtr (nextIdx);

    if  (mergedFeatures.IsSubSet (job->Features ()))
    {
      log.Level (-1) << endl 
        << "Processor::MergeKBestFeatureSelections   ***WARNING***   Trying to merge a subset or parent." << endl
        << endl;
    }
    else
    {
    mergedFeatures += job->Features ();
      numFeatureSetsMerged++;
    }

    nextIdx++;
  }

  return  mergedFeatures;
}  /* MergeKBestFeatureSelections */




FeatureNumList  Processor::GetFeaturesFromTestResults ()  
{
  BinaryJobListPtr  testJobs = binaryJobs->ExtractTestJobs ();
  if  ((testJobs == NULL)  ||  (testJobs->QueueSize () < 1))
  {
    log.Level (-1) << endl << endl 
                   << "Processor::GetFeaturesFromTestResults     ***ERROR***"  << endl
                   << endl
                   << "                   There are no test jobs ***" << endl
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }
    
  testJobs->SortByTestGrade (featureSelection->FeatureCountPrefSmall ());   

  BinaryJobPtr bestJob = testJobs->FrontOfQueue ();
  FeatureNumList  features = bestJob->Features ();
  delete  testJobs;
  return  features;
}  /* GetFeaturesFromTestResults */





FeatureNumList  Processor::GetFeaturesRemoveHurtFul ()
{
  FeatureImpact featureImpact (*binaryJobs);
  FeatureNumList  features = featureImpact.FeatureThatDontHurt ();
  return  features;  
}




void  Processor::GetBestParametersFromTestResults (double&  cParm, 
                                                   double&  gammaParm, 
                                                   float&   aParm
                                                  )
{
  if  (featureSelection->ParamSelCriteria () == jscMostAccurate)
  {
    aParm     = bestA;
    cParm     = bestC;
    gammaParm = bestGamma;
    return;
  }

  else if  (featureSelection->ParamSelCriteria () == jscBruitForce)
  {
    aParm     = bestA;
    cParm     = bestC;
    gammaParm = bestGamma;
    return;
  }


  BinaryJobListPtr  testJobs = binaryJobs->ExtractTestJobs ();
  if  ((testJobs == NULL)  ||  (testJobs->QueueSize () < 1))
  {
    log.Level (-1) << endl << endl 
                   << "Processor::GetBestParametersFromTestResults     ***ERROR***"  << endl
                   << endl
                   << "                   There are no test jobs ***" << endl
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }
  
  BinaryJobListPtr  mostAccurate = testJobs->ExtractHighestTestGrade (0.6f, featureSelection->FeatureCountPrefSmall ());

  mostAccurate->SortDeltaTestProbAccuarcy ();
  BinaryJobPtr  bestJob = mostAccurate->FrontOfQueue ();
  

  delete  mostAccurate;   mostAccurate = NULL;
  delete  testJobs;       testJobs     = NULL;

  cParm     = bestJob->CParm     ();
  gammaParm = bestJob->GammaParm ();
  aParm     = bestJob->AParm     ();

  return;
}  /* GetBestParametersFromTestResults */



double  Processor::TotalProcessingTime ()  const
{
  if  (binaryJobs)
    return binaryJobs->TotalProcessingTime ();
  else
    return 0.0;
}  /* TotalProcessingTime */




FeatureNumList  Processor::SelectBestFeatures ()
{
  FeatureNumList  bestFeatures (fileDesc);

  if  (featureSelection->SearchType () == stFeatureSelectionSearch)
  {
    switch  (featureSelection->FeatureCriteria ())
    {
      case  fcMerge2Best:     bestFeatures = MergeKBestFeatureSelections (2, 2);
                              break;

      case  fcMerge3Best:     bestFeatures = MergeKBestFeatureSelections (3, 3);
                              break;

      case  fcMerge4Best:     bestFeatures = MergeKBestFeatureSelections (4, 4);
                              break;

      case  fcMerge5Best:     bestFeatures = MergeKBestFeatureSelections (5, 5);
                              break;

      case  fcMerge6Best:     bestFeatures = MergeKBestFeatureSelections (6, 6);
                              break;

      case  fcMerge7Best:     bestFeatures = MergeKBestFeatureSelections (7, 7);
                              break;

      case  fcBestTestSet:    bestFeatures = GetFeaturesFromTestResults ();
                              break;

      case  fcRemoveHurtful:  bestFeatures = GetFeaturesRemoveHurtFul ();
                              break;

    }
  }

  return  bestFeatures;
}  /* SelectBestFeatures */


