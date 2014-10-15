#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include  "MemoryDebug.h"

using namespace std;



#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;



#include "ConfusionMatrix2.h"
#include "CrossValidation.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "BinaryJob.h"
#include "BinaryJobList.h"
#include "FeatureSelectionTypes.h"
#include "Processor.h"


BinaryJob::BinaryJob (const BinaryJob&  j):
  accuracy           (j.accuracy),
  accuracyNorm       (j.accuracyNorm),
  aParm              (j.aParm),
  avgPredProb        (j.avgPredProb),
  chgFeatureNum      (j.chgFeatureNum),
  cParm              (j.cParm),
  decisiveProbMean   (j.decisiveProbMean),
  decisiveProbStdDev (j.decisiveProbStdDev),
  features           (j.features),
  fMeasure           (j.fMeasure),
  gammaParm          (j.gammaParm),
  grade              (j.grade),
  jobId              (j.jobId),
  log                (j.log),
  numOfRounds        (j.numOfRounds),
  processor          (j.processor),
  parentId           (j.parentId),
  processingTime     (j.processingTime),
  randomNum          (j.randomNum),
  status             (j.status),
  testAccuracy       (j.testAccuracy),
  testAccuracyNorm   (j.testAccuracyNorm),
  testAvgPredProb    (j.testAvgPredProb),
  testGrade          (j.testGrade),
  testFMeasure       (j.testFMeasure),
  testNumSVs         (j.testNumSVs),
  validateOnly       (j.validateOnly)
  
{
}
 


BinaryJob::BinaryJob (ProcessorPtr    _processor,
                      int             _parentId,
                      FeatureNumList  _features,
                      int             _chgFeatureNum
                     ):

  accuracy           (-1.0f),
  accuracyNorm       (-1.0f),
  aParm              (100.0f),
  avgPredProb        (-1.0f),
  chgFeatureNum      (_chgFeatureNum),
  //crossValidation    (NULL),
  cParm              (1.0),
  decisiveProbMean   (0.0f),
  decisiveProbStdDev (0.0f),
  features           (_features),
  fMeasure           (-1.0f),
  gammaParm          (0.4096),
  grade              (0.0f),
  log                (_processor->Log ()),
  numOfRounds        (1),
  processor          (_processor),
  jobId              (_processor->AllocateNextJobId ()),
  parentId           (_parentId),
  processingTime     (-1.0f),
  randomNum          (rand () % 1000),
  status             (bjOpen),
  testAccuracy       (-1.0f),
  testAccuracyNorm   (-1.0f),
  testAvgPredProb    (-1.0f),
  testGrade          (-1.0f),
  testFMeasure       (-1.0f),
  testNumSVs         (-1),
  validateOnly       (false)

{
  cout << "BinaryJob::BinaryJob      Please don't use anymore" << endl;  
}




BinaryJob::BinaryJob (ProcessorPtr    _processor,
                      int             _parentId,
                      FeatureNumList  _features,
                      int             _chgFeatureNum,
                      int32           _numOfRounds,
                      double          _cParm,
                      double          _gammaParm,
                      float           _aParm
                     ):

  accuracy           (-1.0f),
  accuracyNorm       (-1.0f),
  aParm              (_aParm),
  avgPredProb        (-1.0f),
  chgFeatureNum      (_chgFeatureNum),
  cParm              (_cParm),
  //crossValidation    (NULL),
  decisiveProbMean   (0.0f),
  decisiveProbStdDev (0.0f),
  features           (_features),
  fMeasure           (0.0f),
  gammaParm          (_gammaParm),
  grade              (0.0f),
  log                (_processor->Log ()),
  numOfRounds        (_numOfRounds),
  processor          (_processor),
  jobId              (_processor->AllocateNextJobId ()),
  parentId           (_parentId),
  processingTime     (-1.0f),
  randomNum          (rand () % 10000),
  status             (bjOpen),
  testAccuracy       (-1.0f),
  testAccuracyNorm   (-1.0f),
  testAvgPredProb    (-1.0f),
  testGrade          (-1.0f),
  testFMeasure       (-1.0f),
  testNumSVs         (-1),
  validateOnly       (false)
{}





BinaryJob::BinaryJob (ProcessorPtr _processor,
                      KKStrParser   statusLine
                      ):

  accuracy           (-1.0f),
  accuracyNorm       (-1.0f),
  aParm              (-1.0f),
  avgPredProb        (-1.0f),
  chgFeatureNum      (-1),
  cParm              (-1.0),
  //crossValidation    (NULL),
  decisiveProbMean   (0.0f),
  decisiveProbStdDev (0.0f),
  features           (_processor->FileDesc ()),
  fMeasure           (-1.0f),
  gammaParm          (0.0),
  log                (_processor->Log ()),
  jobId              (-1),
  numOfRounds        (1),
  parentId           (-1),
  processor          (_processor),
  processingTime     (-1.0f),
  status             (bjNULL),
  testAccuracy       (-1.0f),
  testAccuracyNorm   (-1.0f),
  testAvgPredProb    (-1.0f),
  testGrade          (-1.0f),
  testFMeasure       (-1.0f),
  testNumSVs         (-1),
  validateOnly       (false)

{
  ProcessStatusStr (statusLine);
}



BinaryJob::BinaryJob (RunLog&      _log,
                      FileDescPtr  _fileDesc
                     ):

  accuracy           (-1.0f),
  accuracyNorm       (-1.0f),
  aParm              (-1.0f),
  avgPredProb        (-1.0f),
  chgFeatureNum      (0),
  cParm              (-1.0),
  decisiveProbMean   (0.0f),
  decisiveProbStdDev (0.0f),
  features           (_fileDesc),
  fMeasure           (-1.0f),
  gammaParm          (0.0),
  grade              (0.0f),
  log                (log),
  numOfRounds        (1),
  processor          (NULL),
  jobId              (-1),
  parentId           (-1),
  processingTime     (-1.0f),
  randomNum          (0),
  status             (bjOpen),
  testAccuracy       (-1.0f),
  testAccuracyNorm   (-1.0f),
  testAvgPredProb    (-1.0f),
  testGrade          (-1.0f),
  testFMeasure       (-1.0f),
  testNumSVs         (-1),
  validateOnly       (false)
{}





BinaryJob::~BinaryJob ()
{
  //delete  crossValidation;
}



BinaryJobPtr  BinaryJob::CreateDuplicateJob (BinaryJob&  j)
{
  if  (j.JobType () == jtRandomSplit)
    return  new JobRandomSplit (dynamic_cast<JobRandomSplit&> (j));

  else if  (j.JobType () == jtValidation)
    return  new JobValidation (dynamic_cast<JobValidation&> (j));

  else
    return  new BinaryJob (j);
}





JobTypes  BinaryJob::JobType ()  const
{
  return  jtBinaryCombo;
}


KKStr   BinaryJob::JobTypeStr ()  const
{
  switch  (JobType ())
  {
  case  jtBinaryCombo:  return  "BinaryComboJob";
  case  jtRandomSplit:  return  "RandomSplitJob";
  case  jtValidation:   return  "JobValidation";
  }

  return  "";
}




BinaryJobPtr  BinaryJob::CreateTestJob (BinaryJobPtr  sourceJob)
{

  BinaryJobPtr  testJob = new BinaryJob (sourceJob->processor,
                                         sourceJob->jobId,
                                         sourceJob->features,
                                         sourceJob->chgFeatureNum,
                                         sourceJob->numOfRounds,
                                         sourceJob->cParm,
                                         sourceJob->gammaParm,
                                         sourceJob->aParm
                                        );

  int  newJobId = testJob->JobId ();

  testJob->ReFresh (*sourceJob);

  testJob->JobId (newJobId);
  testJob->ValidateOnly (true);
  testJob->Status (bjOpen);

  return  testJob;
}  /* CreateTestJob */




void    BinaryJob::ReFresh (BinaryJob&  j)
{
  if  (validateOnly != j.validateOnly)
  {
    log.Level (10) << endl 
      << "BinaryJob::ReFresh  validateOnly changed.  JobId[" << jobId << "]  Was[" << (validateOnly ? "Yes" : "No") << "]  New[" << (j.validateOnly ? "Yes" : "No") << "]" << endl
      << endl;
  }

  accuracy           = j.accuracy;
  accuracyNorm       = j.accuracyNorm;
  aParm              = j.aParm;
  avgPredProb        = j.avgPredProb;
  chgFeatureNum      = j.chgFeatureNum;
  cParm              = j.cParm;
  decisiveProbMean   = j.decisiveProbMean;
  decisiveProbStdDev = j.decisiveProbStdDev;
  grade              = j.grade;
  features           = j.features;
  fMeasure           = j.fMeasure;
  gammaParm          = j.gammaParm;
  jobId              = j.jobId;
  numOfRounds        = j.numOfRounds;
  parentId           = j.parentId;
  processingTime     = j.processingTime;
  processor          = j.processor;
  randomNum          = j.randomNum;
  status             = j.status;
  testAccuracy       = j.testAccuracy;
  testAccuracyNorm   = j.testAccuracyNorm;
  testAvgPredProb    = j.testAvgPredProb;
  testFMeasure       = j.testFMeasure;
  testGrade          = j.testGrade;
  testNumSVs         = j.testNumSVs;
  validateOnly       = j.validateOnly;
}  /* ReFresh */




KKStr  BinaryJob::BinaryJobStatusToStr (bjBinaryJobStatus  status)
{
  if  (status == bjOpen)
    return  "Open";

  if  (status == bjStarted)
    return "Started";

  if  (status == bjDone)
    return "Done";

  if  (status == bjExpanded)
    return "Expanded";

  return  "";
}  /* BinaryJobStatusToStr */




bjBinaryJobStatus  BinaryJob::BinaryJobStatusFromStr (KKStr  statusStr)
{
  statusStr.Upper ();

  if  (statusStr == "OPEN")
    return  bjOpen;

  if  (statusStr == "STARTED")
    return bjStarted;

  if  (statusStr == "DONE")
    return bjDone;

  if  (statusStr == "EXPANDED")
    return  bjExpanded;

  return bjNULL;
}  /* BinaryJobStatusToStr */




KKStr  BinaryJob::StatusStr () const
{
  return  BinaryJobStatusToStr (status);
}  /* StatusStr */




KKStr  BinaryJob::ToStatusStr ()
{
  KKStr  statusStr (200);  // Preallocating 200 bytes.

  statusStr << "JobId"          << "\t" << jobId                                          << "\t"
            << "ParentId"       << "\t" << parentId                                       << "\t"
            << "Status"         << "\t" << BinaryJobStatusToStr (status)                  << "\t"
            << "ChgFeatureNum"  << "\t" << chgFeatureNum                                  << "\t"
            << "ProcTime"       << "\t" << StrFormatDouble (processingTime, "zzz0.000")   << "\t"
            << "Accuracy"       << "\t" << StrFormatDouble (accuracy,       "zzz0.00000") << "\t"
            << "AccuracyNorm"   << "\t" << StrFormatDouble (accuracyNorm,   "zzz0.00000") << "\t"
            << "AvgPredProb"    << "\t" << StrFormatDouble (avgPredProb,    "zzz0.00000") << "\t"
            << "DecProbMean"    << "\t" << decisiveProbMean                               << "\t"
            << "DecProbStdDev"  << "\t" << decisiveProbStdDev                             << "\t"
            << "FMeasure"       << "\t" << StrFormatDouble (fMeasure,       "zzz0.00000") << "\t"
            << "Grade"          << "\t" << StrFormatDouble (grade,          "zzz0.00000") << "\t"
            << "Features"       << "\t" << features.ToString ()                           << "\t"
            << "RandomNum"      << "\t" << randomNum                                      << "\t"
            << "NumOfRounds"    << "\t" << numOfRounds                                    << "\t"
            << "CParm"          << "\t" << cParm                                          << "\t"
            << "GammaParm"      << "\t" << gammaParm                                      << "\t"
            << "AParm"          << "\t" << aParm;

  if   (testAccuracy > 0.0f)
    statusStr << "\t" << "TestAccuracy"     << "\t" << StrFormatDouble (testAccuracy, "zz0.000");

  if   (testAccuracyNorm > 0.0f)
    statusStr << "\t" << "TestAccuracyNorm" << "\t" << StrFormatDouble (testAccuracyNorm, "zz0.000");

  if  (testAvgPredProb > 0.0f)
    statusStr << "\t" << "TestAvgPredProb"  << "\t" << StrFormatDouble (testAvgPredProb, "zz0.000");

  if  (testGrade > 0.0f)
    statusStr << "\t" << "TestGrade"       << "\t" << StrFormatDouble (testGrade, "zz0.000");

  if  (testFMeasure > 0.0f)
    statusStr << "\t" << "TestFMeasure"    << "\t" << StrFormatDouble (testFMeasure, "zzz0.00000");

  if  (testNumSVs > 0)
    statusStr << "\t" << "TestNumSVs"      << "\t" << StrFormatInt (testNumSVs, "zzzz0");

  if  (validateOnly)
    statusStr << "\t" << "ValidateOnly"    << "\t" << (validateOnly ? "Yes" : "No");

  return  statusStr;
}  /* ToStatusStr */




void  BinaryJob::ProcessStatusStr (KKStrParser&  statusStr)
{
  if  (log.Level () > 20)
    log.Level (30) << "BinaryJob::ProcessStatusStr[" << statusStr.Str () << "]" << endl;
  KKStr  fieldName;
  KKStr  fieldValue;

  fieldName = statusStr.GetNextToken ("\t\n\r");

  while  (!fieldName.Empty ())
  {
    fieldName.Upper ();

    fieldValue = statusStr.GetNextToken ("\t\n\r");
    fieldValue.TrimLeft ("\n\r\t ");
    fieldValue.TrimRight ("\n\r\t ");

    if  (fieldName == "JOBID")
      jobId = fieldValue.ToInt ();

    else  if  (fieldName == "PARENTID")
      parentId = fieldValue.ToInt ();

    else  if  (fieldName == "STATUS")
      status = BinaryJobStatusFromStr (fieldValue);

    else  if  (fieldName == "CHGFEATURENUM")
      chgFeatureNum = fieldValue.ToInt ();

    else  if  ((fieldName == "PROCESSINGTIME")  ||  (fieldName == "PROCTIME"))
      processingTime = fieldValue.ToFloat ();

    else  if  (fieldName == "FEATURES")
    {
      bool valid;
      features = FeatureNumList (processor->FileDesc (), fieldValue, valid);
    }

    else if  (fieldName == "RANDOMNUM")
      randomNum = fieldValue.ToInt ();

    else if  (fieldName == "ACCURACY")
      accuracy = fieldValue.ToFloat ();

    else if  (fieldName == "ACCURACYNORM")
      accuracyNorm = fieldValue.ToFloat ();

    else if  (fieldName == "AVGPREDPROB")
      avgPredProb = fieldValue.ToFloat ();

    else if  (fieldName == "DECPROBMEAN")
      decisiveProbMean = fieldValue.ToFloat ();

    else if  (fieldName == "DECPROBSTDDEV")
      decisiveProbStdDev = fieldValue.ToFloat ();
    
    else if  (fieldName == "FMEASURE")
      fMeasure = fieldValue.ToFloat ();

    else if  (fieldName == "GRADE")
      grade = fieldValue.ToFloat ();

    else if  (fieldName.EqualIgnoreCase ("NumOfRounds"))
      numOfRounds = fieldValue.ToInt32 ();

    else if  (fieldName == "CPARM")
      cParm = fieldValue.ToDouble ();

    else if  (fieldName == "GAMMAPARM")
      gammaParm = fieldValue.ToDouble ();

    else if  (fieldName == "APARM")
      aParm = fieldValue.ToFloat ();

    else if  (fieldName == "TESTACCURACY")
      testAccuracy = fieldValue.ToFloat ();

    else if  (fieldName == "TESTACCURACYNORM")
      testAccuracyNorm = fieldValue.ToFloat ();

    else if  (fieldName == "TESTAVGPREDPROB")
      testAvgPredProb = fieldValue.ToFloat ();

    else if  (fieldName == "TESTGRADE")
      testGrade = fieldValue.ToFloat ();

    else if  (fieldName == "TESTFMEASURE")
      testFMeasure = fieldValue.ToFloat ();

    else if  (fieldName == "TESTNUMSVS")
      testNumSVs = fieldValue.ToInt ();

    else if  (fieldName == "VALIDATEONLY")
    {
      validateOnly = fieldValue.ToBool ();
    }

    else
    {
      ProcessStatusField (fieldName, fieldValue);
    }

    fieldName = statusStr.GetNextToken ("\t\n\r");
    if  (fieldName.Empty ())
      fieldName = statusStr.GetNextToken ("\t\n\r");
  }
}  /* ProcessStatusStr */



void  BinaryJob::ProcessStatusField (const KKStr&  fieldName,
                                     const KKStr&  fieldValue
                                    )
{
   log.Level (-1) << "BinaryJob::ProcessStatusField  Invalid Field Name[" << fieldName << "]  fieldValue[" << fieldValue << "]." << endl;
}  /* ProcessStatusField */





BinaryJobPtr   BinaryJob::CheckTwoListsForExitingJob (FeatureNumList&   featuresToLookFor,
                                                      BinaryJobListPtr  exitingJobs1,
                                                      BinaryJobListPtr  exitingJobs2
                                                     )
{
 BinaryJobPtr  existingJob = NULL;
 
 if  (exitingJobs1)
   existingJob = exitingJobs1->LookUpByFeatureNums (featuresToLookFor);

 if  ((exitingJobs2)  &&  (!existingJob))
   existingJob = exitingJobs2->LookUpByFeatureNums (featuresToLookFor);

  return  existingJob;
}  /* CheckTwoListsForExitingJob */




BinaryJobListPtr  BinaryJob::ExpandBestCaseBackward (BinaryJobListPtr  exitingJobs1,
                                                     BinaryJobListPtr  exitingJobs2
                                                    )
{
  log.Level (20) << "BinaryJob::ExpandBestCaseBackward   JobId[" << jobId << "]" << endl;
  BinaryJobListPtr  expandedJobs = new BinaryJobList (processor);

  if  (features.NumOfFeatures () < 2)
    return expandedJobs;

  int  x;

  for  (x = 0;  x < features.NumOfFeatures ();  x++)
  {
    int  featureToRemove = features[x];

    FeatureNumList newFeatures = features - featureToRemove;

    // See if job already exists.
    BinaryJobPtr  existingJob = CheckTwoListsForExitingJob (newFeatures, exitingJobs1, exitingJobs2);
    if  (!existingJob)
    { 
      BinaryJobPtr newJob = new BinaryJob (processor,
                                           jobId,        // ParentId
                                           newFeatures,
                                           featureToRemove,
                                           numOfRounds,
                                           cParm,
                                           gammaParm,
                                           aParm
                                          );

      expandedJobs->PushOnBack (newJob);
    }
  }

  return  expandedJobs;
}  /* ExpandBestCaseBackward */





BinaryJobListPtr  BinaryJob::ExpandBestCaseForward (BinaryJobListPtr  exitingJobs1,
                                                    BinaryJobListPtr  exitingJobs2
                                                   )
{
  log.Level (20) << "BinaryJob::ExpandBestCaseForward   JobId[" << jobId << "]" << endl;

  BinaryJobListPtr  expandedJobs = new BinaryJobList (processor);

  if  (features.NumOfFeatures () > 55)
    return expandedJobs;

  int  x;

  const FeatureNumList&   initialFeatures (processor->InitialFeatures ());

  for  (x = 0;  x < initialFeatures.NumSelFeatures ();  x++)
  {
    if  (features.InList (x))
      continue;

    FeatureNumList newFeatures = features + initialFeatures[x];

    // See if job already exists.
    BinaryJobPtr  existingJob = CheckTwoListsForExitingJob (newFeatures, exitingJobs1, exitingJobs2);
    if  (!existingJob)
    { 
      BinaryJobPtr newJob = new BinaryJob (processor,
                                           jobId,        // ParentId
                                           newFeatures,
                                           x,
                                           numOfRounds,
                                           cParm,
                                           gammaParm,
                                           aParm
                                          );

      expandedJobs->PushOnBack (newJob);
    }
  }

  return  expandedJobs;
}  /* ExpandBestCaseForward */




void  BinaryJob::EvaluateNode ()
{
  log.Level (9) << "  " << endl;
  log.Level (9) << "BinaryJob::EvaluteNode JobId[" << jobId << "]  FeatureCount[" << features.NumOfFeatures () << "]" << endl;
  status = bjStarted;

  TrainingConfiguration2Ptr  config = processor->Config ();

  config->SetFeatureNums (features);
  config->C_Param (cParm);
  config->Gamma   (gammaParm);
  config->A_Param (aParm);
  config->Number_of_rounds (numOfRounds);

  bool  cancelFlag = false;

  CrossValidationPtr  crossValidation = new CrossValidation  
                                           (processor->Config (),
                                            processor->TrainingData (),
                                            processor->ClassesThisProcess (),
                                            processor->NumOfFolds (),
                                            processor->AlreadyNormalized (),
                                            processor->FileDesc (),
                                            log,
                                            cancelFlag
                                           );

  if  (validateOnly)
  {
    log.Level (10) << "EvaluteNode   validateOnly." << endl;
    if  (processor->TestData () == NULL)
    {
      crossValidation->NumOfFolds (processor->NumOfFolds () * 2);
      crossValidation->RunCrossValidation ();
    }
    else
    {
      crossValidation->RunValidationOnly (processor->TestData ());
    }

    testAccuracy     = crossValidation->Accuracy ();
    testAccuracyNorm = crossValidation->AccuracyNorm ();
    testAvgPredProb  = (float)crossValidation->AvgPredProb () * 100.0f;
    testFMeasure     = crossValidation->ConfussionMatrix ()->FMeasure (processor->PositiveClass (), log);

    if  (processor->GradingMethod () == gmAccuracy)
      testGrade = testAccuracy;

    if  (processor->GradingMethod () == gmAccuracyNorm)
      testGrade = testAccuracyNorm;

    else if  (processor->GradingMethod () == gmFMeasure)
      testGrade = testFMeasure;

    else
      testGrade = testAccuracy;

    testNumSVs      = crossValidation->NumOfSupportVectors ();
  }
  else
  {
    crossValidation->RunCrossValidation ();

    //crossValidation->CalculateThirdClassDecisiveProbability (decisiveProbMean, decisiveProbStdDev);

    accuracy     = crossValidation->Accuracy ();
    accuracyNorm = crossValidation->AccuracyNorm ();
    avgPredProb  = (float)crossValidation->AvgPredProb () * 100.0f;
    fMeasure     = crossValidation->ConfussionMatrix ()->FMeasure (processor->PositiveClass (), log);

    if  (processor->GradingMethod () == gmAccuracy)
       grade = accuracy;

    if  (processor->GradingMethod () == gmAccuracyNorm)
       grade = accuracyNorm;

    else if  (processor->GradingMethod () == gmFMeasure)
       grade = fMeasure;

    else
       grade = accuracy;

    processingTime = crossValidation->TestTimeTotal () + crossValidation->TrainTimeTotal ();
  }

  delete  crossValidation;  crossValidation = NULL;

  status = bjDone;
}  /* EvaluteNode */




KKStr  BinaryJob::ReportHeaderLine1 ()
{
  KKStr h1Line (132);
  h1Line << ""             << "\t"  // Jobid
         << ""             << "\t"  // Parent Job Id
         << "NumOfRounds"  << "\t"  // Parent Job Id
         << "C"            << "\t"  // C Paremeter
         << "Gamma"        << "\t"  // Gamma Paremeter
         << "A"            << "\t"  // A Parameter
         << ""             << "\t"  // Accuracy
         << "Norm"         << "\t"  // Norm Accuracy
         << "Average"      << "\t"  // Average Predicted Probability
         << ""             << "\t"  // fMeasure
         << "Processing"   << "\t"  // Processing Time
         << "Feature"      << "\t"  // Feature Changed
         << "Feature"      << "\t"  // Number of features
         << ""             << "\t"  // Features
         << "Test"         << "\t"  // Test Accuracy
         << "TestNorm"     << "\t"  // Test Accuracy Norm
         << "Test"         << "\t"  // Test PredProb
         << "Test"         << "\t"  // Test F-Measure
         << "Test";                 // test S/V's

  return  h1Line;   
}  /* ReportHeaderLine1 */






KKStr  BinaryJob::ReportHeaderLine2 ()
{
  KKStr h2Line (132);
  h2Line << "JobId"        << "\t"
         << "Parent"       << "\t"
         << "Parm"         << "\t"  // NumOfRounds
         << "Parm"         << "\t"  // C Paremeter
         << "Parm"         << "\t"  // Gamma Paremeter
         << "Parm"         << "\t"  // A Parameter
         << "Accuracy"     << "\t"  // Accuracy
         << "Accuracy"     << "\t"
         << "PredProb"     << "\t"  // Average Predicted Probability
         << "F-Measure"    << "\t"  
         << "Time"         << "\t"  // Processing Time
         << "Changed"      << "\t"  // Feature Changed
         << "Count"        << "\t"  // Number of features
         << "Features"     << "\t"  // Features
         << "Accuracy"     << "\t"  // Test Accuracy
         << "Accuracy"     << "\t"  // Test Accuracy Norm
         << "PredProb"     << "\t"  // 
         << "F-Measure"    << "\t"  // based off testAccuracy
         << "S/V's";                // Number Support vectors created while performing validation agaist tst set.

  return  h2Line;
}  /* ReportHeaderLine2 */



KKStr  BinaryJob::ReportLine ()  const
{
  KKStr  l (132);

  l << jobId                                                    << "\t"
    << parentId                                                 << "\t"
    << numOfRounds                                              << "\t"
    << cParm                                                    << "\t"
    << gammaParm                                                << "\t"
    << aParm                                                    << "\t"
    << StrFormatDouble (accuracy,          "##0.00000") << "%"  << "\t"
    << StrFormatDouble (accuracyNorm,      "##0.00000") << "%"  << "\t"
    << StrFormatDouble (avgPredProb,       "##0.0000")  << "%"  << "\t"
    << StrFormatDouble (fMeasure,          "##0.0000")  << "%"  << "\t"
    << StrFormatDouble (processingTime, "##,##0.00000")         << "\t"
    << chgFeatureNum                                            << "\t"
    << features.NumSelFeatures ()                               << "\t"
    << features.ToString ()                                     << "\t"
    << StrFormatDouble (testAccuracy,      "##0.00000") << "%"  << "\t"
    << StrFormatDouble (testAccuracyNorm,  "##0.00000") << "%"  << "\t"
    << StrFormatDouble (testAvgPredProb,   "##0.0000")  << "%"  << "\t"
    << StrFormatDouble (testFMeasure,      "##0.0000")  << "%"  << "\t"
    << StrFormatDouble (testNumSVs,        "###,##0");

  return  l;
}  /* ReportLine */


KKStr  BinaryJob::ReportHeaderLineHTML ()
{
  KKStr  r (300);

  r << "<th align=\"center\">JobId</th>"
    << "<th align=\"center\">Parent</th>"
    << "<th align=\"center\">NumOfRounds<br />Parm</th>"
    << "<th align=\"center\">C<br />Parm</th>"
    << "<th align=\"center\">Gamma<br />Parm</th>"
    << "<th align=\"center\">A<br />Parm</th>"
    << "<th align=\"right\">Accuracy</th>"
    << "<th align=\"right\">Norm<br />Accuracy</th>"
    << "<th align=\"right\">Average<br />PredProb</th>"
    << "<th align=\"right\">FMeasure</th>"
    << "<th align=\"right\">Processing<br />Time</th>"
    << "<th align=\"center\">Feature<br />Changed</th>"
    << "<th align=\"center\">Feature<br />Count</th>"
    << "<th align=\"left\">Features</th>"
    << "<th align=\"right\">Test<br />Accuracy</th>"
    << "<th align=\"right\">Test<br />Norm<br />Accuracy</th>"
    << "<th align=\"right\">Test<br />PredProb</th>"
    << "<th align=\"right\">Test<br />F-Measure</th>"
    << "<th align=\"right\">Test<br />S/V's</th>";

  return r;
} /* ReportHeaderLineHTML */



KKStr  BinaryJob::ReportLineHTML ()  const
{
  KKStr  l (132);

  KKStr prsLeft = "";
  KKStr prsRight = "";
  if  (parentId >= 0)
  {
    prsLeft  << "<a href=\"#JI" << parentId << "\">";
    prsRight << "</a>";
  }

  l << "<td align=\"center\"><a name=\"JI" << jobId << "\" />" << jobId                       << "</td>"
    << "<td align=\"center\">" << prsLeft  << parentId << prsRight                            << "</td>"
    << "<td align=\"center\">" << numOfRounds                                                 << "</td>"
    << "<td align=\"center\">" << cParm                                                       << "</td>"
    << "<td align=\"center\">" << gammaParm                                                   << "</td>"
    << "<td align=\"center\">" << aParm                                                       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (accuracy,          "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (accuracyNorm,      "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (avgPredProb,       "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (fMeasure,          "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (processingTime, "ZZ,ZZ0.000")              << "</td>"
    << "<td align=\"center\">" << chgFeatureNum                                               << "</td>"
    << "<td align=\"center\">" << features.NumSelFeatures ()                                  << "</td>"
    << "<td align=\"left\">"   << features.ToString ()                                        << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (testAccuracy,      "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (testAccuracyNorm,  "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (testAvgPredProb,   "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (testFMeasure,      "ZZ0.000") << "%"       << "</td>"
    << "<td align=\"right\">"  << StrFormatDouble (testNumSVs,        "ZZZ,ZZ0")              << "</td>";

  return  l;
}  /* ReportLine */



float  DeltaProb (float  inProb,
                  float  inAccuracy
                 )
{
  const float  maxProbDesired = 99.999f;
  const float  power          = 2.0f;
  float  prob = inProb;
  if  (prob > maxProbDesired)
  {
    float zed = (prob - maxProbDesired) / (100.0f - maxProbDesired);
    prob = (pow (power, zed) - 1.0f) + maxProbDesired;
  }

  return fabs (inAccuracy - prob);
}



float  BinaryJob::DeltaProbAccuracy  () const 
{
  return  DeltaProb (avgPredProb, accuracy);
}




float  BinaryJob::TestDeltaProbAccuracy () const 
{ 
  return  DeltaProb (testAvgPredProb, testAccuracy);
}
