#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;

#include  "BitReductionJob.h"

#include  "BitReduction.h"
#include  "BitReductionJobList.h"
#include  "BitReducedExample.h"
#include  "CrossValidation.h"
#include  "FeatureEncoder.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOC45.h"
#include  "Orderings.h"
#include  "OSservices.h"
#include  "KKQueue.h"
#include  "RandomSampleJob.h"
#include  "RunLog.h"
#include  "StatisticalFunctions.h"
#include  "Str.h"
#include  "SVMparam.h"
#include  "TrainingProcess.h"
#include  "TrainingConfiguration.h"

#include  "BitReduction.h"



KKStr  LearningAlgorithmToStr (laLearningAlgorithms  _learningAlgorithm)
{
  switch  (_learningAlgorithm)
  {
  case  laSupportVectorMachine:  return  "SuportVectorMachine";
                                 break;

  case  laUSFcascor:             return  "USFcascor";
                                 break;

  default:  return "";
  }
}  /* LearningAlgorithmToStr */



KKStr  LearningAlgorithmToShortStr (laLearningAlgorithms  _learningAlgorithm)
{
  switch  (_learningAlgorithm)
  {
  case  laSupportVectorMachine:  return  "SVM";
                                 break;

  case  laUSFcascor:             return  "CASCOR";
                                 break;

  default:  return "NULL";
  }
}  /* LearningAlgorithmToStr */




laLearningAlgorithms  LearningAlgorithmFromStr (KKStr  s)
{
  s.Upper ();

  if  ((s.EqualIgnoreCase ("SuportVectorMachine"))  ||  (s == "SVM"))
    return  laSupportVectorMachine;

  if  ((s.EqualIgnoreCase ("USFcascor"))  ||  (s == "CASCOR"))
    return  laUSFcascor;

  return  laNULL;
}   /* LearningAlgorithmFromStr */





BitReductionJob::BitReductionJob (RunLog&                _log,
                                  int                    _jobId,
                                  laLearningAlgorithms   _learningAlgorithm,
                                  SVM_KernalType         _kernalType,
                                  SVM_SelectionMethod    _selectionMethod,
                                  SVM_EncodingMethod     _encodingMethod,
                                  SVM_CompressionMethod  _compMethod,
                                  bool                   _weighted,
                                  int                    _cParam,
                                  double                 _gamma,
                                  int                    _bitReduction,
                                  int                    _unBalBits,
                                  brjJobTypes            _jobType,
                                  uint                   _ordering,
                                  int                    _numOfFeatures,
                                  int                    _numFeatursAfterEncoding
                                 ):

         
         log                     (_log),
         version                 (BitRetReductionJobVersion),
         jobId                   (_jobId),
         status                  (brOpen),
         kernalType              (_kernalType),
         selectionMethod         (_selectionMethod),
         encodingMethod          (_encodingMethod),
         compMethod              (_compMethod),
         weighted                (_weighted),
         cParam                  (_cParam),
         gamma                   (_gamma),
         bitReduction            (_bitReduction),
         unBalBits               (_unBalBits),
         jobType                 (_jobType),
         numOfFeatures           (_numOfFeatures),
         numFeatursAfterEncoding (_numFeatursAfterEncoding),
         ordering                (_ordering),

         unBalFeatures           (),
         accuracy                (0.0f),
         supportVectors          (0.0f),
         redTime                 (0.0),
         trainTime               (0.0),
         testTime                (0.0),
         redPreCount             (0),
         redPostCount            (0),
         redRatio                (0.0),
         testStat                (0.0),
         randSampAccuracy        (0.0f),
         runTime                 (),

         // tuning job variables
         procTimeBaseLineAccuracy (0.0),
         procTimeSampleData       (0.0),
         procTimeFindStartBitRed  (0.0),
         procTimeBestBitRed       (0.0),
         procTimeBestUnBalBits    (0.0),
         baseLineAccuracy         (0.0),
         startBitReductionLevel   (0),
         sampledTrainingSetSize   (0),
         totalTuningTime          (0.0),
         randomSeed               (-1),
         learningAlgorithm        (_learningAlgorithm)
//         learningAlgorithm        (laSupportVectorMachine)
{
}





BitReductionJob::BitReductionJob (const BitReductionJob&  brj):

         log                     (brj.log),
         version                 (brj.version),
         jobId                   (brj.jobId),
         status                  (brj.status),
         kernalType              (brj.kernalType),
         selectionMethod         (brj.selectionMethod),
         encodingMethod          (brj.encodingMethod),
         compMethod              (brj.compMethod),
         weighted                (brj.weighted),
         cParam                  (brj.cParam),
         gamma                   (brj.gamma),
         bitReduction            (brj.bitReduction),
         unBalBits               (brj.unBalBits),
         jobType                 (brj.jobType),
         ordering                (brj.ordering),
         numOfFeatures           (brj.numOfFeatures),
         numFeatursAfterEncoding (brj.numFeatursAfterEncoding),

         unBalFeatures           (brj.unBalFeatures),
         accuracy                (brj.accuracy),
         supportVectors          (brj.supportVectors),
         redTime                 (brj.redTime),
         trainTime               (brj.trainTime),
         testTime                (brj.testTime),
         redPreCount             (brj.redPreCount),
         redPostCount            (brj.redPostCount),
         redRatio                (brj.redRatio),
         testStat                (brj.testStat),
         randSampAccuracy        (brj.randSampAccuracy),
         runTime                 (brj.runTime),

         // tuning job variables
         procTimeBaseLineAccuracy (brj.procTimeBaseLineAccuracy),
         procTimeSampleData       (brj.procTimeSampleData),
         procTimeFindStartBitRed  (brj.procTimeFindStartBitRed),
         procTimeBestBitRed       (brj.procTimeBestBitRed),
         procTimeBestUnBalBits    (brj.procTimeBestUnBalBits),
         baseLineAccuracy         (brj.baseLineAccuracy),
         startBitReductionLevel   (brj.startBitReductionLevel),
         sampledTrainingSetSize   (brj.sampledTrainingSetSize),
         totalTuningTime          (brj.totalTuningTime),
         randomSeed               (brj.randomSeed),
         learningAlgorithm        (brj.learningAlgorithm)
{
}




BitReductionJob::BitReductionJob (RunLog&        _log,
                                  const KKStr&  _toTabDelStr
                                 ):

         log                     (_log),
         version                 (-1),
         jobId                   (-1),
         status                  (brNULL),
         kernalType              (KT_NULL),
         selectionMethod         (SelectionMethod_NULL),
         encodingMethod          (NoEncoding),
         compMethod              (BRNull),
         weighted                (true),
         cParam                  (1),
         gamma                   (1),
         bitReduction            (0),
         unBalBits               (0),
         jobType                 (brjNULL),
         ordering                (0),
         numOfFeatures           (0),
         numFeatursAfterEncoding (0),

         unBalFeatures           (),
         accuracy                (0.0f),
         supportVectors          (0.0f),
         redTime                 (0.0),
         trainTime               (0.0),
         testTime                (0.0),
         redPreCount             (0),
         redPostCount            (0),
         redRatio                (0.0),
         testStat                (0.0),
         randSampAccuracy        (0.0f),
         runTime                 (),

         // tuning job variables
         procTimeBaseLineAccuracy (0.0),
         procTimeSampleData       (0.0),
         procTimeFindStartBitRed  (0.0),
         procTimeBestBitRed       (0.0),
         procTimeBestUnBalBits    (0.0),
         baseLineAccuracy         (0.0),
         startBitReductionLevel   (0),
         sampledTrainingSetSize   (0),
         totalTuningTime          (0.0),
         randomSeed               (-1),
         learningAlgorithm        (laSupportVectorMachine)
{
  ParseTabDelStr (_toTabDelStr);
}





BitReductionJob::~BitReductionJob ()
{
}




BitReductionJob&  BitReductionJob::operator+= (const BitReductionJob&  r)
{
  if  ((kernalType      != r.kernalType)       ||
       (selectionMethod != r.selectionMethod)  ||
       (encodingMethod  != r.encodingMethod)   ||
       (weighted        != r.weighted)         ||
       (cParam          != r.cParam)           ||
       (gamma           != r.gamma)        
      )
  {
    log.Level (-1) << endl << endl << endl
                   << "BitReductionJob::operator+=  *** ERROR ***    Not Same Paramters" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  accuracy         += r.accuracy;
  supportVectors   += r.supportVectors;
  redTime          += r.redTime;
  trainTime        += r.trainTime;
  testTime         += r.testTime;
  redPreCount      += r.redPreCount;
  redPostCount     += r.redPostCount;
  redRatio         += r.redRatio;
  randSampAccuracy += r.randSampAccuracy;


  // tuning job variables
  procTimeBaseLineAccuracy += r.procTimeBaseLineAccuracy;
  procTimeSampleData       += r.procTimeSampleData;
  procTimeFindStartBitRed  += r.procTimeFindStartBitRed;
  procTimeBestBitRed       += r.procTimeBestBitRed;
  procTimeBestUnBalBits    += r.procTimeBestUnBalBits;
  baseLineAccuracy         += r.baseLineAccuracy;
  totalTuningTime          += r.totalTuningTime;
  sampledTrainingSetSize   += r.sampledTrainingSetSize;

  return  *this;
}  /* operator+= */



BitReductionJob&  BitReductionJob::operator/= (float  r)
{
  accuracy         /= r;
  supportVectors   /= r;
  redTime          /= r;
  trainTime        /= r;
  testTime         /= r;
  redPreCount      =  int (0.5 + float (redPreCount) / float (r));
  redPostCount     =  int (0.5 + float (redPostCount) / float (r));
  redRatio         /= r;
  randSampAccuracy /= r;


  procTimeBaseLineAccuracy /= r;
  procTimeSampleData       /= r;
  procTimeFindStartBitRed  /= r;
  procTimeBestBitRed       /= r;
  procTimeBestUnBalBits    /= r;
  baseLineAccuracy         /= r;
  totalTuningTime          /= r;
  sampledTrainingSetSize   = (int)(0.5f + (float)sampledTrainingSetSize / (float)r);

  return  *this;
}  /* operator+= */






KKStr  BitReductionJob::JobTypeToStr (brjJobTypes  jobType)
{
  if  (jobType == brjRandomSampling)
    return "RandomSampling";

  else if  (jobType == brjNormalJob)
    return "Normal";

  else if  (jobType == brjRandomSplits)
    return "RandomSplits";

  else if  (jobType == brjSmallSample)
    return  "SmallSample";

  else if  (jobType == brjTuningTest)
    return  "TuningTest";

  return "";
}  /* JobTypeToStr */




brjJobTypes  BitReductionJob::JobTypeFromStr (const KKStr&  _str)
{
  KKStr str = _str.ToUpper ();
  
  if  (str == "RANDOMSAMPLING")
    return brjRandomSampling;

  else if  (str == "NORMAL")
    return brjNormalJob;

  else if  (str == "RANDOMSPLITS")
    return brjRandomSplits;

  else if  (str == "SMALLSAMPLE")
    return  brjSmallSample;

  else if  (str == "TUNINGTEST")
    return  brjTuningTest;

  return  brjNULL;
}  /* JobStatusFromStr */


KKStr  BitReductionJob::JobStatusToStr (brJobStatus  status)
{
  if  (status == brOpen)
    return  "Open";

  if  (status == brStarted)
    return "Started";

  if  (status == brDone)
    return "Done";

  return  "";
}  /* JobStatusToStr */




brJobStatus  BitReductionJob::JobStatusFromStr (KKStr  statusStr)
{
  statusStr.Upper ();

  if  (statusStr == "OPEN")
    return brOpen;

  if  (statusStr == "STARTED")
    return brStarted;

  if  (statusStr == "DONE")
    return brDone;


  return brNULL;
}  /* JobStatusFromStr */



KKStr  BitReductionJob::ToTabDelStr ()
{
  KKStr  str (200);  // Preallocating 200 bytes.

  str << "Version"           << "\t" << version                                  << "\t"
      << "JobId"             << "\t" << jobId                                    << "\t"
      << "Status"            << "\t" << JobStatusToStr (status)                  << "\t"
      << "Kernel"            << "\t" << KernalTypeToStr (kernalType)             << "\t"
      << "SelectionMethod"   << "\t" << SelectionMethodToStr (selectionMethod)   << "\t"
      << "Encoding"          << "\t" << EncodingMethodToStr  (encodingMethod)    << "\t"
      << "CompMethod"        << "\t" << CompressionMethodStr (compMethod) << "\t"
      << "Weighted"          << "\t" << (weighted ? "Yes" : "No")                << "\t"
      << "cParam"            << "\t" << cParam                                   << "\t"
      << "Gamma"             << "\t" << gamma                                    << "\t"
      << "BitReduction"      << "\t" << bitReduction                             << "\t"
      << "UnBalBits"         << "\t" << unBalBits                                << "\t"
      << "UnBalFeatures"     << "\t" << unBalFeatures                            << "\t"
      << "JobType"           << "\t" << JobTypeStr ()                            << "\t"
      << "Ordering"          << "\t" << ordering                                 << "\t"
      << "NumOfFeatures"     << "\t" << numOfFeatures                            << "\t"
      << "AfterEncoding"     << "\t" << numFeatursAfterEncoding                  << "\t"
      << "Accuracy"          << "\t" << accuracy                                 << "\t"
      << "SupportVectors"    << "\t" << supportVectors                           << "\t"
      << "RedTime"           << "\t" << redTime                                  << "\t"
      << "trainTime"         << "\t" << trainTime                                << "\t"
      << "TestTime"          << "\t" << testTime                                 << "\t"
      << "RedPreCount"       << "\t" << redPreCount                              << "\t"
      << "RedPostCount"      << "\t" << redPostCount                             << "\t"
      << "RedRatio"          << "\t" << redRatio                                 << "\t"
      << "TestStat"          << "\t" << testStat                                 << "\t"
      << "RandSampAccuracy"  << "\t" << randSampAccuracy                         << "\t"
      << "RunTime"           << "\t" << runTime                                  << "\t"
      << "LearningAlgorithm" << "\t" << LearningAlgorithmToStr (learningAlgorithm);


  if  (jobType == brjTuningTest)
  {
    // Bit Reduction Tuning Fields.
    str << "\t"
        << "ProcTimeBaseLineAccuracy"  << "\t" << procTimeBaseLineAccuracy << "\t"
        << "ProcTimeSampleData"        << "\t" << procTimeSampleData       << "\t"
        << "ProcTimeFindStartBitRed"   << "\t" << procTimeFindStartBitRed  << "\t"
        << "ProcTimeBestBitRed"        << "\t" << procTimeBestBitRed       << "\t"
        << "ProcTimeBestUnBalBits"     << "\t" << procTimeBestUnBalBits    << "\t"
        << "BaseLineAccuracy"          << "\t" << baseLineAccuracy         << "\t"
        << "StartBitReductionLevel"    << "\t" << startBitReductionLevel   << "\t"
        << "SampledTrainingSetSize"    << "\t" << sampledTrainingSetSize   << "\t"
        << "TotalTuningTime"           << "\t" << totalTuningTime          << "\t"
        << "RandomSeed"                << "\t" << StrFormatInt (randomSeed, "ZZZZZZZZZZZ0");
  }



  return  str;
}  /* ToString */






void  BitReductionJob::ParseTabDelStr (const KKStr&  _str)
{
  log.Level (30) << "BitReductionJob::ProcessStatusStr[" << _str << "]" << endl;
  KKStr  str = _str;

  KKStr  fieldName;
  KKStr  fieldValue;

  compMethod = BRNull;

  fieldName = str.ExtractQuotedStr ("\t", true);

  while  (!fieldName.Empty ())
  {
    fieldName.Upper ();

    fieldValue = str.ExtractQuotedStr ("\t", true);
    fieldValue.TrimLeft ("\n\r\t ");
    fieldValue.TrimRight ("\n\r\t ");


    if  (fieldName == "VERSION")
      version = atoi (fieldValue.Str ());

    else if  (fieldName == "JOBID")
      jobId = atoi (fieldValue.Str ());

    else if  (fieldName == "STATUS")
      status = JobStatusFromStr (fieldValue);

    else if  (fieldName == "KERNEL")
      kernalType = KernalTypeFromStr (fieldValue);

    else if  (fieldName == "SELECTIONMETHOD")
      selectionMethod = SelectionMethodFromStr (fieldValue);

    else if  (fieldName == "ENCODING")
      encodingMethod = EncodingMethodFromStr (fieldValue);

    else if  (fieldName == "COMPMETHOD")
      compMethod = CompressionMethodFromStr (fieldValue);

    else if  (fieldName == "WEIGHTED")

    {
      fieldValue.Upper ();
      weighted = ((fieldValue == "YES") ||  
                  (fieldValue == "Y")     ||
                  (fieldValue == "TRUE")  ||
                  (fieldValue == "T")
                 );
    }
    
    else if  (fieldName == "CPARAM")
      cParam = atoi (fieldValue.Str ());

    else if  (fieldName == "GAMMA")
      gamma = atof (fieldValue.Str ());

    else if  (fieldName == "BITREDUCTION")
      bitReduction = atoi (fieldValue.Str ());

    else if  (fieldName == "UNBALBITS")
      unBalBits = atoi (fieldValue.Str ());

    else if  (fieldName == "UNBALFEATURES")
      unBalFeatures = fieldValue;

    else if  (fieldName == "RANDOMSAMPLE")
    {
      // Before the creation of the JobType field we had this field called RandomSampling
      fieldValue.Upper ();
      if ((fieldValue == "YES") ||  
          (fieldValue == "Y")     ||
          (fieldValue == "TRUE")  ||
          (fieldValue == "T")
         )
      {
        jobType = brjRandomSampling;
      }
      else
      {
        jobType = brjNormalJob;
      }
    }

    else if  (fieldName == "JOBTYPE")
      jobType = JobTypeFromStr (fieldValue);

    else if  (fieldName == "ORDERING")
      ordering = atoi (fieldValue.Str ());

    else if  (fieldName == "NUMOFFEATURES")
      numOfFeatures = atoi (fieldValue.Str ());

    else if  (fieldName == "AFTERENCODING")
      numFeatursAfterEncoding = atoi (fieldValue.Str ());

    else if  (fieldName == "ACCURACY")
      accuracy = float (atof (fieldValue.Str ()));

    else if  (fieldName == "SUPPORTVECTORS")
      supportVectors = float (atof (fieldValue.Str ()));

    else if  (fieldName == "REDTIME")
      redTime = atof (fieldValue.Str ());

    else if  (fieldName == "TRAINTIME")
      trainTime = atof (fieldValue.Str ());

    else if  (fieldName == "TESTTIME")
      testTime = atof (fieldValue.Str ());

    else if  (fieldName == "REDPRECOUNT")
      redPreCount = atoi (fieldValue.Str ());

    else if  (fieldName == "REDPOSTCOUNT")
    {
      redPostCount = atoi (fieldValue.Str ());
    }

    else  if  (fieldName == "REDRATIO")
      redRatio = (float)atof (fieldValue.Str ());

    else if  (fieldName == "TESTSTAT")
      testStat = (float)atof (fieldValue.Str ());

    else if  (fieldName == "RANDSAMPACCURACY")
      randSampAccuracy = (float)atof (fieldValue.Str ());

    else if  (fieldName == "RUNTIME")
      runTime = DateTime (fieldValue);

    else if  (fieldName.EqualIgnoreCase ("LearningAlgorithm"))
      learningAlgorithm = LearningAlgorithmFromStr (fieldValue);

    else if  (fieldName == "PROCTIMEBASELINEACCURACY")
      procTimeBaseLineAccuracy = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("ProcTimeSampleData")== 0)
      procTimeSampleData = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("ProcTimeFindStartBitRed") == 0)
      procTimeFindStartBitRed = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("ProcTimeBestBitRed") == 0)
      procTimeBestBitRed = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("ProcTimeBestUnBalBits") == 0)
      procTimeBestUnBalBits = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("BaseLineAccuracy") ==0)
      baseLineAccuracy = (float)atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("StartBitReductionLevel") == 0)
      startBitReductionLevel = atoi(fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("SampledTrainingSetSize") == 0)
      sampledTrainingSetSize = atoi(fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("TotalTuningTime") == 0)
      totalTuningTime  = atof (fieldValue.Str ());

    else if  (fieldName.CompareIgnoreCase ("RandomSeed") == 0)
      randomSeed  = atol (fieldValue.Str ());

    else
    {
      log.Level (-1) << "BitReductionJob::ProcessStatusStr  Invalid Field Name[" << fieldName << "]." << endl;
    }

    fieldName = str.ExtractToken ("\t\n\r");
  }
}  /* ProcessStatusStr */




void  BitReductionJob::EvaluateRandomSample (TrainingConfigurationPtr  config,
                                             const KKStr&              trainDataFileName,
                                             FeatureVectorListPtr      trainData,
                                             FeatureVectorListPtr      testData,
                                             MLClassListPtr         mlClasses,
                                             ostream&                  r
                                            )
{
  OrderingsPtr  orderings = Orderings::CreateOrderingsObjFromFileIfAvaliable (trainData, 50, 1);

  config->KernalType         (kernalType);
  config->SelectionMethod    (selectionMethod);
  config->EncodingMethod     (encodingMethod);
  config->CompressionMethod  (compMethod);
  config->WeightBitReduction (weighted);
  config->C_Param            (cParam);
  config->Gamma              (gamma);
  config->BitsToReduceBy     (bitReduction);
  config->UnBalancedBits     (unBalBits);

  r << endl << endl << endl
    << "Random Sample Job[" << jobId << "]" << endl
    << "Kernal       [" << KernalTypeToStr (kernalType)           << "]" << endl
    << "Sel Method   [" << SelectionMethodToStr (selectionMethod) << "]" << endl
    << "Encoding     [" << EncodingMethodToStr  (encodingMethod)  << "]" << endl
    << "Compression  [" << CompressionMethodStr (compMethod)      << "]" << endl
    << "C Param      [" << cParam                                 << "]" << endl
    << "Gamma        [" << gamma                                  << "]" << endl
    << "BitReduction [" << bitReduction                           << "]" << endl
    << "UnBalanced   [" << unBalBits                              << "]" << endl
    << endl;

  RandomSampleJobListPtr  jobs = new RandomSampleJobList (log, 
                                                          config, 
                                                          trainDataFileName, 
                                                          trainData, 
                                                          orderings
                                                         );

  RandomSampleJob::ReportResultsHeader (r);
  {
    // Report results from jobs already completed.
    RandomSampleJobList::const_iterator idx;
    for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
    {
      RandomSampleJobPtr j = *idx;
      if  (j->Status () == rjDone)
        j->ReportResults (r);
    }
  }

  RandomSampleJobPtr  job = jobs->LocateOpenJob ();
  while  (job)
  {
    job->EvaluteNode (testData, mlClasses);
    jobs->Save ();
    job->ReportResults (r);
    job = jobs->LocateOpenJob ();
  }

  double  accuracyMean, accuracyStdDev, trainTimeMean, testTimeMean, supportVectorsMean;
  jobs->CalcAccuracyStats (accuracyMean, accuracyStdDev, trainTimeMean, testTimeMean, supportVectorsMean);

  accuracy       = float (accuracyMean);
  supportVectors = float (supportVectorsMean);
  redTime        = 0.0;
  trainTime      = trainTimeMean;
  testTime       = testTimeMean;

  redPreCount    = 0;
  redPostCount   = 0;
  redRatio       = 1.0;
  testStat       = 0.0;
  runTime        = osGetLocalDateTime ();

  delete  jobs;

  delete  orderings;
}  /* EvaluateRandomSample */



void  BitReductionJob::EvaluateRandomSplit (TrainingConfigurationPtr  _config,
                                            Orderings&                splitOrderings,
                                            MLClassList&           mlClasses,
                                            ostream&                  r
                                           )
{
  log.Level (9) << "  " << endl;
  log.Level (9) << "  " << endl;
  log.Level (9) << "BitReductionJob::EvaluateRandomSplit JobId[" << jobId << "]" << endl;

  status = brStarted;

  runTime = osGetLocalDateTime ();

  TrainingConfigurationPtr config = new TrainingConfiguration (*_config);

  config->KernalType         (kernalType);
  // config->SelectionMethod   (selectionMethod);  // We will use the one from the config file.
  config->EncodingMethod     (encodingMethod);
  config->CompressionMethod  (compMethod);
  config->WeightBitReduction (true);
  config->C_Param            (cParam);
  config->Gamma              (gamma);
  config->BitsToReduceBy     (bitReduction);
  config->UnBalancedBits     (unBalBits);

  FileDescPtr fileDesc = splitOrderings.FileDesc ();

  FeatureVectorListPtr  data = splitOrderings.Ordering (ordering);
  
  // We will now plit data into 80/20 Train(80) and Test (20)
  FeatureVectorList  trainData (fileDesc, false, log, 1000);
  FeatureVectorList  testData  (fileDesc, false, log, 1000);

  {
    int  numExamples  = data->QueueSize ();
    int  numTrainData = int (double (numExamples) * 0.80 + 0.5);

    FeatureVectorList::iterator  idx;
    for  (idx = data->begin ();  idx != data->end ();  idx++)
    {
      FeatureVectorPtr  example = *idx;
      if  (trainData.QueueSize () < numTrainData)
        trainData.PushOnBack (example);
      else
        testData.PushOnBack (example);
    }
  }


  {
    double  timeToTrain       = 0.0;
    double  timeToTest        = 0.0;
    float   accuracyMean      = 0.0f;
    double  numSupportVectors = 0.0;
    CompressionStats      compressionStats;
    TestASpecificConfiguraion  (learningAlgorithm, 
                                config,
                                false,     // false = data is NOT already normalized.
                                &trainData, 
                                &testData, 
                                accuracyMean, 
                                timeToTrain, 
                                timeToTest, 
                                NULL, 
                                compressionStats, 
                                numSupportVectors
                               );


    // With random Splits you can not calculate test stat without all random jobs 
    // being ran first;  and then only as a group.
    testStat = -1.0;

    unBalFeatures  = compressionStats.unBalFeatures;
    accuracy       = accuracyMean;
    supportVectors = float (numSupportVectors);
    redTime        = compressionStats.elapsed_cpu_time;
    trainTime      = timeToTrain;
    testTime       = timeToTest;
    redPreCount    = compressionStats.num_images_before;
    redPostCount   = compressionStats.num_images_after;
    redRatio       = compressionStats.compression_ratio;
  }

  if  (compMethod != BRnoCompression)
  {
    // Now that we have a compression level we can try random sampling the data

    FeatureVectorList  randomTrainData (fileDesc, false, log, redPostCount);

    {
      // We are now going to copy over the first 'redPostCount' number of examples from 'TrainData'.  Train data which is from 
      // 'splitOrderings' is already in random order.  We want to use this ordering specifically so that we will be 
      // consistent with other compression levels for the same set of config parameters.
      FeatureVectorList::iterator  idx;

      for  (idx = trainData.begin ();  ((idx != trainData.end ())  &&  (randomTrainData.QueueSize () < redPostCount));  idx++)
      {
        randomTrainData.PushOnBack (*idx);
      }
    }

    {
      config->CompressionMethod (BRnoCompression);

      double  timeToTrain       = 0.0;
      double  timeToTest        = 0.0;
      float   accuracyMean      = 0.0f;
      double  numSupportVectors = 0.0;
      CompressionStats      compressionStats;
      TestASpecificConfiguraion  (learningAlgorithm, 
                                  config,
                                  false,     // false = data is NOT already normalized.
                                  &randomTrainData, 
                                  &testData, 
                                  accuracyMean, 
                                  timeToTrain, 
                                  timeToTest, 
                                  NULL, 
                                  compressionStats, 
                                  numSupportVectors
                                );

      randSampAccuracy =  accuracyMean;
    }
  }

  runTime = osGetLocalDateTime ();

  delete  config;        config       = NULL;
  status = brDone;
}  /* EvaluateRandomSplit */
                                   


void  BitReductionJob::EvaluteNode (TrainingConfigurationPtr  _config,
                                    const KKStr&             trainDataFileName,
                                    FeatureVectorListPtr      trainData,
                                    FeatureVectorListPtr      testData,
                                    MLClassListPtr         mlClasses,
                                    bool*                     noCompClassedCorrectly,
                                    bool*                     classedCorrectly,
                                    ostream&                  r
                                   )
{
  log.Level (9) << "  " << endl;
  log.Level (9) << "  " << endl;
  log.Level (9) << "BitReductionJob::EvaluteNode JobId[" << jobId << "]" << endl;

  status = brStarted;

  runTime = osGetLocalDateTime ();

  TrainingConfigurationPtr config = new TrainingConfiguration (*_config);

  config->KernalType         (kernalType);
  // config->SelectionMethod   (selectionMethod);  // We will use the one from the config file.
  config->EncodingMethod     (encodingMethod);
  config->CompressionMethod  (compMethod);
  config->WeightBitReduction (true);
  config->C_Param            (cParam);
  config->Gamma              (gamma);
  config->BitsToReduceBy     (bitReduction);
  config->UnBalancedBits     (unBalBits);

  FileDescPtr fileDesc = trainData->FileDesc ();



  if  (jobType == brjRandomSampling)
  {
     EvaluateRandomSample (config,
                           trainDataFileName,
                           trainData,
                           testData,
                           mlClasses,
                           r
                          );
  }

  else if  ((jobType == brjNormalJob)  ||  (jobType == brjSmallSample))
  {
    double  timeToTrain       = 0.0;
    double  timeToTest        = 0.0;
    float   accuracyMean      = 0.0f;
    double  numSupportVectors = 0.0;
    CompressionStats      compressionStats;
    TestASpecificConfiguraion  (learningAlgorithm, 
                                config,
                                false,     // false = data is NOT already normalized.
                                trainData, 
                                testData, 
                                accuracyMean, 
                                timeToTrain, 
                                timeToTest, 
                                classedCorrectly, 
                                compressionStats, 
                                numSupportVectors
                               );


    if  (compMethod == BRnoCompression)
    {
      int x;
      for  (x = 0;  x < testData->QueueSize ();  x++)
        noCompClassedCorrectly[x] = classedCorrectly[x];
    }

    if  (noCompClassedCorrectly  &&  classedCorrectly)
      testStat = McNemarsTest (testData->QueueSize (), noCompClassedCorrectly, classedCorrectly);

    unBalFeatures  = compressionStats.UnBalFeatures ();
    accuracy       = accuracyMean;
    supportVectors = float (numSupportVectors);
    redTime        = compressionStats.elapsed_cpu_time;
    trainTime      = timeToTrain;
    testTime       = timeToTest;
    redPreCount    = compressionStats.num_images_before;
    redPostCount   = compressionStats.num_images_after;
    redRatio       = compressionStats.compression_ratio;
  }

  else if  (jobType == brjRandomSplits)
  {
    // Random Splits Job
  }

  runTime = osGetLocalDateTime ();

  delete  config;        config       = NULL;
  status = brDone;
}  /* EvaluteNode */






void  BitReductionJob::ReportTuningResultsHeader (ostream&   r)
{
  r << endl << endl << endl << endl;              


  r << ""                << "\t"   // Kernal
    << ""                << "\t"   // selectionMethod
    << ""                << "\t"   // encodingMethod
    << ""                << "\t"   // compMethod
    << ""                << "\t"   // weighted
    << ""                << "\t"   // cParam
    << ""                << "\t"   // Gamma
    << "Base"            << "\t"   // procTimeBaseLineAccuracy
    << "Base"            << "\t"   // baseLineAccuracy
    << ""                << "\t"   // procTimeSampleData
    << "Start"           << "\t"   // procTimeFindStartBitRed
    << "Start"           << "\t"   // startBitReductionLevel
    << "Best"            << "\t"   // procTimeBestBitRed
    << "Best"            << "\t"   // procTimeBestUnBalBits
    << "Tuning"          << "\t"   // totalTuningTime
    << ""                << "\t"   // bitReduction + "-" + unBalBits
    << ""                << "\t"   // unBalFeatures
    << ""                << "\t"   // trainTime
    << "Total"           << "\t"   // totalTuningTime + trainTime
    << ""                << "\t"   // testTime
    << ""                << "\t"   // accuracy
    << ""                << "\t"   // redRatio
    << endl;


  r << ""                << "\t"   // Kernal
    << "Sel"             << "\t"   // selectionMethod
    << ""                << "\t"   // encodingMethod
    << ""                << "\t"   // compMethod
    << ""                << "\t"   // weighted
    << ""                << "\t"   // cParam
    << ""                << "\t"   // Gamma
    << "Line"            << "\t"   // procTimeBaseLineAccuracy
    << "Line"            << "\t"   // baseLineAccuracy
    << "Sampling"        << "\t"   // procTimeSampleData
    << "BitRed"          << "\t"   // procTimeFindStartBitRed
    << "Start"           << "\t"   // startBitReductionLevel
    << "BitRed"          << "\t"   // procTimeBestBitRed
    << "UnBal"           << "\t"   // procTimeBestUnBalBits
    << "Total"           << "\t"   // totalTuningTime
    << "BitRed"          << "\t"   // bitReduction + "-" + unBalBits
    << "UnBal"           << "\t"   // unBalFeatures
    << "Train"           << "\t"   // trainTime
    << "TuneTrain"       << "\t"   // totalTuningTime + trainTime
    << "Test"            << "\t"   // testTime
    << "Test"            << "\t"   // accuracy
    << "Red"             << "\t"   // redRatio
    << "Random"          << "\t"   // randomSeed
    << endl;


  r << "Kernal"          << "\t"   // kernalType
    << "Method"          << "\t"   // selectionMethod
    << "Encoding"        << "\t"   // encodingMethod
    << "Compression"     << "\t"   // compMethod
    << "Wght"            << "\t"   // weighted
    << "C"               << "\t"   // cParam
    << "Gamma"           << "\t"   // gamma
    << "Time"            << "\t"   // procTimeBaseLineAccuracy
    << "Accuracy"        << "\t"   // baseLineAccuracy
    << "Time"            << "\t"   // procTimeSampleData
    << "Time"            << "\t"   // procTimeFindStartBitRed
    << "BitRed"          << "\t"   // startBitReductionLevel
    << "Time"            << "\t"   // procTimeBestBitRed
    << "Time"            << "\t"   // procTimeBestUnBalBits
    << "Time"            << "\t"   // totalTuningTime
    << "Level"           << "\t"   // bitReduction + "-" + unBalBits
    << "Features"        << "\t"   // unBalFeatures
    << "Time"            << "\t"   // trainTime
    << "Time"            << "\t"   // totalTuningTime + trainTime
    << "Time"            << "\t"   // testTime
    << "Accuracy"        << "\t"   // accuracy
    << "Ratio"           << "\t"   // redRatio
    << "Seed"            << "\t"   // randomSeed
    << endl;

  r.flush ();

}  /* ReportTuningResultsHeader */








void  BitReductionJob::ReportResultsHeader (ostream&   r)
{
  r << endl << endl << endl << endl;              


  r << ""                << "\t"   // Kernal
    << "Sel"             << "\t"   // Sel method
    << ""                << "\t"   // Encoding
    << ""                << "\t"   // Compression
    << ""                << "\t"   // Weight
    << ""                << "\t"   // C
    << ""                << "\t"   // Gamma
    << "Bit"             << "\t"   // BitReduction
    << "UnBal"           << "\t"   // Unbalanced BitReduction
    << "UnBal"           << "\t"   // UnBalanced Features
    << "Ftur"            << "\t"   // FeatureCount
    << "Enc"             << "\t"   // Encoded FeatureCount
    << "Mean"            << "\t"   // Mean Accuracy
    << "Acc"             << "\t"   // Accuracy Loss
    << "Mean"            << "\t"   // supportPointsMean
    << "Red"             << "\t"   // Reduction Time
    << "Train"           << "\t"   // Train Time
    << "Test"            << "\t"   // Test Time
    << "Train"           << "\t"   // Train SpeedUp
    << "Test"            << "\t"   // Test SpeedUp
    << "Red"             << "\t"   // Reduction Pre
    << "Red"             << "\t"   // Reduction Post
    << "Red"             << "\t"   // Reduction Ratio
    << "Test"            << "\t"   // Test Statistic.
    << "Rand"            << "\t"   // Random Split Accuracy
    << "Run"                       // Run Time
    << endl;

  r << "Kernal"          << "\t"
    << "Method"          << "\t"
    << "Encoding"        << "\t"
    << "Compression"     << "\t"
    << "Wght"            << "\t"
    << "C"               << "\t"
    << "Gamma"           << "\t"
    << "Red"             << "\t"
    << "Bits"            << "\t"  // UnBal Bits
    << "Fturs"           << "\t"  // UnBalanced Fatures Selected
    << "Count"           << "\t"  // FeatureCount
    << "FC"              << "\t"  // Encoded FeatureCount
    << "Acc"             << "\t"
    << "Loss"            << "\t"  // Accuracy Loss
    << "S/V's"           << "\t"
    << "Time"            << "\t"
    << "Time"            << "\t"  // Train Time
    << "Time"            << "\t"  // Test  Time
    << "SpdUp"           << "\t"  // Train SpeedUp
    << "SpdUp"           << "\t"  // Test  SpeedUp
    << "Pre"             << "\t"
    << "Post"            << "\t"
    << "Ratio"           << "\t"
    << "Stat"            << "\t"  // TestStat
    << "Accuracy"        << "\t"  // Random Split Accuracy
    << "Time"                     // Run Time
    << endl;

  r.flush ();

}  /* ReportResultsHeader */




void  BitReductionJob::ReportResults (ostream&         r,
                                      BitReductionJob* unCompressedJob
                                     )

{
  double  trainSpeedUp = 0.0;
  double  testSpeedUp  = 0.0;
  double  accLoss      = 0.0;

  if  (unCompressedJob)
  {
    double  totalTrainTime = (trainTime + redTime);
    if  (totalTrainTime > 0.0)
      trainSpeedUp = unCompressedJob->TrainTime () / totalTrainTime;

    if  (testTime > 0.0)
      testSpeedUp  = unCompressedJob->TestTime  () / testTime;

    if  (unCompressedJob->Accuracy () > 0.0)
      accLoss = 100.0 * (unCompressedJob->Accuracy () - accuracy) / unCompressedJob->Accuracy ();
  }

  r << KernalTypeToStr        (kernalType)       << "\t"
    << SelectionMethodToStr   (selectionMethod)  << "\t"
    << EncodingMethodToStr    (encodingMethod)   << "\t"
    << CompressionMethodStr   (compMethod)       << "\t"
    << (weighted  ?  "Yes" : "No")               << "\t"
    << cParam                                    << "\t"
    << gamma                                     << "\t";

  if  (compMethod == BRnoCompression)
    r << "---"                                   << "\t";
  else
    r << bitReduction                            << "\t";

  if  (compMethod != BRunBalancedVariance)
  {
    r << "---"                                   << "\t"
      << "---"                                   << "\t";
  }
  else
  {
    r << unBalBits                               << "\t"
      << unBalFeatures                           << "\t";
  }

  r << numOfFeatures                             << "\t"
    << numFeatursAfterEncoding                   << "\t"
    << accuracy << "%"                           << "\t";

  if  (compMethod != BRnoCompression)
    r << accLoss << "%"                          << "\t";
  else
    r << " - "                                   << "\t";

  r << supportVectors                            << "\t"
    << redTime                                   << "\t"
    << trainTime                                 << "\t"
    << testTime                                  << "\t";

  if  (compMethod != BRnoCompression)
  {
    r << trainSpeedUp                            << "\t"
      << testSpeedUp                             << "\t";
  }
  else
  {
    r << " -        "                            << "\t"
      << " -        "                            << "\t";
  }

  r << redPreCount                               << "\t"
    << redPostCount                              << "\t"
    << redRatio                                  << "\t"
    << testStat                                  << "\t"
    << randSampAccuracy                          << "\t"
    << runTime    
    << endl;

  r.flush ();


}  /* ReportResults */





void  BitReductionJob::ReportTuningResults (ostream&   r)
{

  r << KernalTypeToStr        (kernalType)               << "\t"
    << SelectionMethodToStr   (selectionMethod)          << "\t"
    << EncodingMethodToStr    (encodingMethod)           << "\t"
    << CompressionMethodStr   (compMethod)               << "\t"
    << (weighted  ?  "Yes" : "No")                       << "\t"
    << cParam                                            << "\t"
    << gamma                                             << "\t"
    << procTimeBaseLineAccuracy                          << "\t"
    << baseLineAccuracy << "%"                           << "\t"
    << procTimeSampleData                                << "\t"
    << procTimeFindStartBitRed                           << "\t"
    << startBitReductionLevel                            << "\t"
    << procTimeBestBitRed                                << "\t"
    << procTimeBestUnBalBits                             << "\t"
    << totalTuningTime                                   << "\t"
    << "\"'" <<bitReduction << "-" << unBalBits <<  "\"" << "\t"
    << unBalFeatures                                     << "\t"
    << trainTime                                         << "\t"
    << (totalTuningTime + trainTime)                     << "\t"
    << testTime                                          << "\t"
    << accuracy << "%"                                   << "\t"
    << redRatio                                          << "\t"
    << randomSeed
    << endl;

  r.flush ();
}  /* ReportTuningResults */





BitReductionJob&  BitReductionJob::operator= (const BitReductionJob&  r)
{
  log                     = r.log;
  version                 = r.version;
  jobId                   = r.jobId;
  status                  = r.status;
  kernalType              = r.kernalType;
  selectionMethod         = r.selectionMethod;
  encodingMethod          = r.encodingMethod;
  compMethod              = r.compMethod;
  weighted                = r.weighted;
  cParam                  = r.cParam;
  gamma                   = r.gamma;
  bitReduction            = r.bitReduction;
  unBalBits               = r.unBalBits;
  jobType                 = r.jobType;
  numOfFeatures           = r.numOfFeatures;
  numFeatursAfterEncoding = r.numFeatursAfterEncoding;

  unBalFeatures           = r.unBalFeatures;
  accuracy                = r.accuracy;
  supportVectors          = r.supportVectors;
  redTime                 = r.redTime;
  trainTime               = r.trainTime;
  testTime                = r.testTime;
  redPreCount             = r.redPreCount;
  redPostCount            = r.redPostCount;
  redRatio                = r.redRatio;
  testStat                = r.testStat;
  randSampAccuracy        = r.randSampAccuracy;
  runTime                 = r.runTime;


  procTimeBaseLineAccuracy = r.procTimeBaseLineAccuracy;
  procTimeSampleData       = r.procTimeSampleData;
  procTimeFindStartBitRed  = r.procTimeFindStartBitRed;
  procTimeBestBitRed       = r.procTimeBestBitRed;
  procTimeBestUnBalBits    = r.procTimeBestUnBalBits;
  baseLineAccuracy         = r.baseLineAccuracy;
  startBitReductionLevel   = r.startBitReductionLevel;
  totalTuningTime          = r.totalTuningTime;
  sampledTrainingSetSize   = r.sampledTrainingSetSize;
  randomSeed               = r.randomSeed;

  learningAlgorithm        = r.learningAlgorithm;
  return  *this;
}  /* operator= */







void  BitReductionJob::ExtractRandomSamplingForTrainAndTest (FeatureVectorListPtr   fullTrainData,
                                                             FeatureVectorListPtr&  sampleTrainData,
                                                             FeatureVectorListPtr&  sampleTestData,
                                                             float                  samplePercentage
                                                            )
{
  delete  sampleTrainData;  sampleTrainData = NULL;
  delete  sampleTestData;   sampleTestData  = NULL;

  sampleTrainData = new FeatureVectorList (fullTrainData->FileDesc (), true, log);
  sampleTestData  = new FeatureVectorList (fullTrainData->FileDesc (), true, log);

  MLClassListPtr  classes = fullTrainData->ExtractListOfClasses ();
  MLClassList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    FeatureVectorListPtr  examplesThisClass = fullTrainData->ExtractImagesForAGivenClass (ic);
    examplesThisClass->RandomizeOrder ();

    int  numInClass = examplesThisClass->QueueSize ();
    int  numToKeep = (int)(0.5f + (float)samplePercentage * (float)numInClass / 100.0f);
    if  (numToKeep < 2)
      numToKeep = 2;

    int  numToKeepInTestSet = numToKeep * 1;

    FeatureVectorList::iterator  idx2;

    int  numAddedToTrainSet = 0;
    int  numAddedToTestSet  = 0;

    for  (idx2 = examplesThisClass->begin ();  idx2 != examplesThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;
      if  (numAddedToTrainSet < numToKeep)
      {
        sampleTrainData->PushOnBack (new FeatureVector (*example));
        numAddedToTrainSet++;
      }

      else if  (numAddedToTestSet < (numToKeepInTestSet))
      {
        sampleTestData->PushOnBack  (new FeatureVector (*example));
        numAddedToTestSet++;
      }

      else
      {
        break;
      }
    }

    delete  examplesThisClass;  examplesThisClass = NULL;
  }

  delete  classes;  classes = NULL;


  return;
}  /* ExtractRandomSamplingForTrainAndTest */



bool  BitReductionJob::DoseUsfCasCorExist ()
{
#ifdef WIN32
  KKStr  fileName = "usfcascor.exe";
#else
  KKStr  fileName = "usfcascor";
#endif

  return  osFileExists (fileName);
}  /* DoseUsfCasCorExist */






void  BitReductionJob::TestWithUSFCASCOR  (TrainingConfigurationPtr   config,
                                           FeatureVectorListPtr       normTrainData,
                                           FeatureVectorListPtr       normTestData,
                                           float&                     testAccuracy,
                                           double&                    trainTime,
                                           double&                    testTime,
                                           CompressionStats&          compressionStats
                                          )
{
  testAccuracy = 0.0f;
  trainTime    = 0.0;
  testTime     = 0.0;

  RunLog&  log (normTrainData->Log ());

  MLClassListPtr  mlClasses = normTrainData->ExtractListOfClasses ();

  FileDescPtr  fileDesc = normTrainData->FileDesc ();

  KKStr  fileSystemName = "USFcascorWork_" + 
                           StrFormatInt (config->BitsToReduceBy (), "00")     + "-" +
                           StrFormatInt (config->UnBalancedBits (), "00")     + "-" +
                           StrFormatInt (osGetProcessId (),         "00000");

  KKStr  compressedTrainFileName = fileSystemName + ".data";
  KKStr  compressedTestFileName  = fileSystemName + ".test";
  KKStr  compressedNamesFileName = fileSystemName + ".names";
  KKStr  compressedWeightsName   = fileSystemName + ".weights";
  KKStr  resultsFileName         = fileSystemName + ".results";
  KKStr  predFileName            = fileSystemName + ".pred";
  KKStr  predWeightFileName      = fileSystemName + ".weighted_pred";

  SVMparam  param (config->SVMparamREF ());

  FeatureFileIOPtr  c45Format = FeatureFileIOC45::Driver ();
  bool  cancelFlag = false;
  bool  successful = false;

  ClassAssignments  classAssignments (*mlClasses, normTrainData->Log ());

  MLL::AttributeTypeVector  attributeTypes   = normTrainData->FileDesc ()->CreateAttributeTypeTable ();
  VectorInt                 cardinalityTable = normTrainData->FileDesc ()->CreateCardinalityTable ();
  FeatureEncoder encoder (param, normTrainData->FileDesc (), attributeTypes, cardinalityTable, NULL, NULL, log);

  {
    FeatureVectorListPtr  compTrainExamples = new FeatureVectorList (config->FileDesc (), true, log, 1000);
    ::BitReduction br (param, fileDesc, normTrainData->AllFeatures ());
    compressionStats =  br.compress (*normTrainData, compTrainExamples, classAssignments);

    {
      FeatureVectorListPtr  encodedCompTrainExamples = encoder.CreateEncodedFeatureVector (*compTrainExamples);
      uint  numExamplesWritten = 0;
      c45Format->SaveFeatureFile (compressedTrainFileName, 
                                  encodedCompTrainExamples->AllFeatures (), 
                                  *encodedCompTrainExamples, 
                                  numExamplesWritten,
                                  cancelFlag,
                                  successful,
                                  log
                                 );
      delete  encodedCompTrainExamples;
    }


    if  (config->WeightBitReduction ())
    {
      FeatureVectorList::iterator  idx;

      // Find the heighest weight;  so we can weith all weights from 0 -> 1;
      float   highestWeight = -9999.99f;
      for  (idx = compTrainExamples->begin ();  idx != compTrainExamples->end ();  idx++)
      {
        FeatureVectorPtr  i = *idx;
        if  (i->TrainWeight () > highestWeight)
          highestWeight = i->TrainWeight ();
      } 

      ofstream weightFile (compressedWeightsName.Str ());
      for  (idx = compTrainExamples->begin ();  idx != compTrainExamples->end ();  idx++)
      {
        FeatureVectorPtr  i = *idx;
        weightFile << (float)i->TrainWeight () / highestWeight  << endl;
      } 
      weightFile.close ();
    }

    delete  compTrainExamples;  compTrainExamples = NULL;
  }

  {
    uint  numExamplesWritten = 0;
    FeatureVectorListPtr  encodedNormTestData = encoder.CreateEncodedFeatureVector (*normTestData);
    c45Format->SaveFeatureFile (compressedTestFileName, 
                                encodedNormTestData->AllFeatures (), 
                                *encodedNormTestData, 
                                numExamplesWritten,
                                cancelFlag,
                                successful,
                                log
                               );
    delete   encodedNormTestData;
  }


  {
    // Lets run usfcascor

#ifdef WIN32
    KKStr  cmdLine = "usfcascor -f " + fileSystemName;
#else
    KKStr  cmdLine = "./usfcascor -f " + fileSystemName;
#endif

    if  (config->WeightBitReduction ())
      cmdLine << "  --use-example-weights";

    cmdLine << "  -r 30";

    system (cmdLine.Str ());

    FILE*  in = fopen (resultsFileName.Str (), "r");
    if  (in == NULL)
    {
      // Something went wrong;  No result file was generated.
      log.Level (-1) << endl << endl << endl
                     << "BitReductionJob::TestWithUSFCASCOR    ***ERROR***" << endl
                     << endl
                     << "                    Results File[" << resultsFileName << "] was not created." << endl
                     << "                     can not finish processing." << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
    else
    {
      char  buff[10240];
      while  (fgets (buff, sizeof (buff), in) != NULL)
      {
        KKStr line (buff);
        if  (line.Len () < 3)
          continue;

        if  (line.SubStrPart (0, 1) == "//")
          continue;

        KKStr  fieldName  = line.ExtractToken2 ("\t");
        KKStr  fieldValue = line.ExtractToken2 ("\t");

        fieldName.Upper ();
        if  (fieldName == "TESTACCURACY")
          testAccuracy = (float)atof (fieldValue.Str ());

        else if  (fieldName == "TRAINTIME")
          trainTime = atof (fieldValue.Str ());

        else if  (fieldName == "TESTTIME")
          testTime =  atof (fieldValue.Str ());
      }
      
      fclose (in);
    }
  }
  
  {
    // Lets clean up the files we created to run 'usfcascor'.
    //osDeleteFile (compressedTrainFileName);
    //osDeleteFile (compressedTestFileName);
    //osDeleteFile (compressedNamesFileName);
    //osDeleteFile (compressedWeightsName);
    osDeleteFile (resultsFileName);
    osDeleteFile (predFileName);
    osDeleteFile (predWeightFileName);
  }

  delete  mlClasses;   mlClasses = NULL;
}  /* TestWithUSFCASCOR */



void  BitReductionJob::TestWithSVM  (TrainingConfigurationPtr   config,
                                     FeatureVectorListPtr       normTrainData,
                                     FeatureVectorListPtr       normTestData,
                                     float&                     testAccuracy,
                                     double&                    trainTime,
                                     double&                    testTime,
                                     bool*                      classedCorrectly,
                                     CompressionStats&          compressionStats,
                                     double&                    numSupportVectors
                                    )
{
  MLClassListPtr  mlClasses = normTrainData->ExtractListOfClasses ();
  FileDescPtr        fileDesc = config->FileDesc ();

  bool  cancelFlag = false;

  CrossValidationPtr cv = 
        new CrossValidation(config, 
                            normTrainData,
                            mlClasses, 
                            10, 
                            true,     // true = Features are already normalized
                            fileDesc,
                            normTrainData->Log (),
                            cancelFlag
                           );

  cv->RunValidationOnly (normTestData, classedCorrectly);
  testAccuracy     = cv->AccuracyMean ();
  trainTime        = cv->TrainTimeMean ();
  testTime         = cv->TestTimeMean ();
  compressionStats = cv->CompStats ();

  numSupportVectors = cv->SupportPointsMean ();

  delete  cv;  cv = NULL;
} /* TestWithSVM */
 


void  BitReductionJob::TestASpecificConfiguraion  (laLearningAlgorithms       learningAlgorithm,
                                                   TrainingConfigurationPtr   config,
                                                   bool                       dataAlreadyNormalized,
                                                   FeatureVectorListPtr       trainData,
                                                   FeatureVectorListPtr       testData,
                                                   float&                     testAccuracy,
                                                   double&                    trainTime,
                                                   double&                    testTime,
                                                   bool*                      classedCorrectly,
                                                   CompressionStats&          compressionStats,
                                                   double&                    numSupportVectors
                                                  )
{
  RunLog&  log (trainData->Log ());
  FileDescPtr  fileDesc = trainData->FileDesc ();

  FeatureVectorListPtr  normalizedTrainData = trainData;
  FeatureVectorListPtr  normalizedTestData  = testData;
  if  (!dataAlreadyNormalized)
  {
    normalizedTrainData = trainData->DuplicateListAndContents ();
    normalizedTestData  = testData->DuplicateListAndContents  ();

    NormalizationParms  normParms (config, *normalizedTrainData, log);
    normParms.NormalizeImages (normalizedTrainData);
    normParms.NormalizeImages (normalizedTestData);
  }


  testAccuracy   = 0.0f;
  trainTime      = 0.0;
  testTime       = 0.0;
  numSupportVectors = 0.0;


  if  (learningAlgorithm == laSupportVectorMachine)
  {
    TestWithSVM  (config,
                  normalizedTrainData,
                  normalizedTestData,
                  testAccuracy,
                  trainTime,
                  testTime,
                  classedCorrectly,
                  compressionStats,
                  numSupportVectors
                 );
  }

  else if (learningAlgorithm == laUSFcascor)
  {
    TestWithUSFCASCOR  (config,
                        normalizedTrainData,
                        normalizedTestData,
                        testAccuracy,
                        trainTime,
                        testTime,
                        compressionStats
                       );
  }

  if  (!dataAlreadyNormalized)
  {
    delete  normalizedTrainData;  normalizedTrainData = NULL;
    delete  normalizedTestData;   normalizedTestData  = NULL;
  }

}  /* TestASpecificConfiguraion */





void  BitReductionJob::FindStartingBitReductionLevel (TrainingConfigurationPtr  config,
                                                      FeatureVectorListPtr      trainData,      // Needs to be already normalized
                                                      int&                      bitReduction,
                                                      double&                   processingTime,
                                                      ostream&                  report
                                                     )
{
  log.Level (10) << "BitReductionJob::FindStartingBitReductionLevel" << endl;

  //srand (2186667 + 924 * jobId);

  FileDescPtr  fileDesc = trainData->FileDesc ();

  bitReduction   = 0;
  processingTime = 0.0;

  MLClassListPtr  classes = trainData->ExtractListOfClasses ();
  ClassAssignments   classAssignments (*classes, log);

  report << endl << endl << endl
         << "Searching For Strating Bit Reduction " << endl << endl
         << "Bit" << "\t" << "Comp"  << "\t" << "Red"  << endl
         << "Red" << "\t" << "Ratio" << "\t" << "Time" << endl;


  double  startTime = osGetSystemTimeUsed ();


  SVMparam   svmParams (*(config->Model3Parameters ()));
  svmParams.CompressionMethod (BRcompressionPost);


  for  (bitReduction = 0;  bitReduction < 14;  bitReduction++)
  {
    report << bitReduction;

    //  We will perform a compression to get the stats for this bitReduction level
    svmParams.BitsToReduceBy (bitReduction);

    //  Lets Compress Data to get some stats
    ::BitReduction  bitRed (svmParams, fileDesc, trainData->AllFeatures ());

    FeatureVectorListPtr  compExamples = new FeatureVectorList (fileDesc, true, log, 1000);

    CompressionStats  stats = bitRed.compress (*trainData, compExamples, classAssignments);

    report << "\t" << stats.compression_ratio
           << "\t" << stats.elapsed_cpu_time
           << endl;

    delete   compExamples;  compExamples = NULL;

    if  (stats.compression_ratio < 0.99f)
    {
      // We found our ideal Bitreduction Level
      bitReduction--;
      break;
    }
  }


  double  endTime = osGetSystemTimeUsed ();

  processingTime = endTime - startTime;

  delete  classes;  classes = NULL;
}  /* FindStartingBitReductionLevel */






void  BitReductionJob::RunTuningExample1 (TrainingConfigurationPtr  config,
                                          FeatureVectorListPtr      trainData,
                                          FeatureVectorListPtr      testData,
                                          MLClassListPtr         mlClasses,
                                          float                     samplePercentage,
                                          ostream&                  report,
                                          float                     accLossTolerance
                                         )

{
  randomSeed = lrand48 ();
  srand48 (randomSeed);

  FileDescPtr  fileDesc = trainData->FileDesc ();

  FeatureVectorListPtr  sampleTrainData = NULL;
  FeatureVectorListPtr  sampleTestData  = NULL;


  baseLineAccuracy         = 0.0f;
  procTimeBaseLineAccuracy = 0.0;
  procTimeSampleData       = 0.0;
  procTimeFindStartBitRed  = 0.0;
  procTimeBestBitRed       = 0.0;
  procTimeBestUnBalBits    = 0.0;


  config->KernalType         (kernalType);
  config->EncodingMethod     (encodingMethod);
  config->WeightBitReduction (true);
  config->C_Param            (cParam);
  config->Gamma              (gamma);

  FeatureVectorListPtr  trainDataNorm = trainData->DuplicateListAndContents ();
  FeatureVectorListPtr  testDataNorm  = testData->DuplicateListAndContents ();

  NormalizationParms normParms (config, *trainData, log);
  normParms.NormalizeImages (trainDataNorm);
  normParms.NormalizeImages (testDataNorm);

  bitReduction = 0;

  {
    // Step One;   Random Sample Full dataset
    double  startTime = osGetSystemTimeUsed ();
    ExtractRandomSamplingForTrainAndTest (trainDataNorm, sampleTrainData, sampleTestData, samplePercentage);
    sampledTrainingSetSize = sampleTrainData->QueueSize ();
    double  endTime = osGetSystemTimeUsed ();
    procTimeSampleData  = endTime - startTime;
  }

  int     sampleTestDataSize = sampleTestData->QueueSize ();
  bool*   baseLineClassedCorrectly = new bool[sampleTestDataSize];
  bool*   classedCorrectly         = new bool[sampleTestDataSize];


  {
    // Step Two;  get base line accuracy
    double  startTime = osGetSystemTimeUsed ();

    config->BitsToReduceBy (0);
    config->CompressionMethod (BRnoCompression);

    double                timeToTrain = 0.0;
    double                timeToTest = 0.0;
    CompressionStats      compressionStats;
    double                numSupportVectors;

    TestASpecificConfiguraion  (learningAlgorithm, 
                                config, 
                                true,  // true = data is already normalized.
                                sampleTrainData, 
                                sampleTestData, 
                                baseLineAccuracy, 
                                timeToTrain, 
                                timeToTest, 
                                baseLineClassedCorrectly, 
                                compressionStats, 
                                numSupportVectors
                               );

    double  endTime = osGetSystemTimeUsed ();
    procTimeBaseLineAccuracy = endTime - startTime;

    report << endl
           << "BaseLine Accuracy" << "\t"  << baseLineAccuracy << "%" << "\t"
           << "Processing Time"   << "\t"  << procTimeBaseLineAccuracy
           << endl
           << endl;

    report.flush ();
  }


  {
    // Step Three  Find Starting Bit reduction Level
    FindStartingBitReductionLevel (config,
                                   sampleTrainData,
                                   startBitReductionLevel,
                                   procTimeFindStartBitRed,
                                   report
                                  );
  }


  {
    // Step Four  Search for bst Bit Reduction Level.
    double  startTime = osGetSystemTimeUsed ();
    report << endl
           << endl
           << "Bit"  << "\t"  << "UnBal" << "\t" << "UnBal"  << "\t" << "Red"    << "\t" << "Test"     << "\t" << "Acc"  << "\t" << "McNemars" << "\t" << "Train" << "\t" << "Test" << "\t" << "TrainTest" << endl
           << "Red"  << "\t"  << "Bits"  << "\t" << "Bits"   << "\t" << "Ratio"  << "\t" << "Accuracy" << "\t" << "Loss" << "\t" << "Test"     << "\t" << "Time"  << "\t" << "Time" << "\t" << "Time"      << endl;
    report.flush ();

    for  (bitReduction = startBitReductionLevel;  bitReduction < 14;  bitReduction++)
    {
      config->CompressionMethod (BRcompressionPost);
      config->BitsToReduceBy (bitReduction);
      config->UnBalancedBits (0);


      double  timeToTrain = 0.0;
      double  timeToTest = 0.0;
      float   accuracyMean = 0.0f;
      CompressionStats      compressionStats;
      double                numSupportVectors = 0.0;

      TestASpecificConfiguraion  (learningAlgorithm, 
                                  config, 
                                  true,     // true = Data is already normalized.
                                  sampleTrainData, 
                                  sampleTestData, 
                                  accuracyMean, 
                                  timeToTrain, 
                                  timeToTest, 
                                  classedCorrectly, 
                                  compressionStats, 
                                  numSupportVectors
                                 );

      float  mcNemarsTest = McNemarsTest (sampleTestDataSize, baseLineClassedCorrectly, classedCorrectly);
      float  accLoss = 100.0f * (baseLineAccuracy - accuracyMean) / baseLineAccuracy;
      double procTime  = timeToTrain + timeToTest;

      report << bitReduction                        << "\t" 
             << "-"                                 << "\t"   // No Unbalanced Bits
             << "-"                                 << "\t"
             << compressionStats.compression_ratio  << "\t"
             << accuracyMean                        << "%" << "\t"
             << accLoss                             << "%" << "\t"
             << mcNemarsTest                        << "\t"
             << timeToTrain                         << "\t"
             << timeToTest                          << "\t"
             << procTime
             << endl;

      report.flush ();


      if  (accLoss > accLossTolerance)
      {
        bitReduction--;
        break;
      }
    }
      
    double  endTime = osGetSystemTimeUsed ();
    procTimeBestBitRed = endTime - startTime;

    // end of Step Four
  }




  {
    // Step Five;  Determine optimal unbalanced bits
    KKStr  lastUnBalFeatures = "";
    unBalFeatures = "";

    double  startTime = osGetSystemTimeUsed ();

    config->CompressionMethod (BRunBalancedVariance);
    config->BitsToReduceBy (bitReduction);

    int  numNumericFeatures = BitReductionJobList::DeriveNumNumericFeatures (config);

    int  nextUnBalBits = 0;
    for  (nextUnBalBits = 0;  nextUnBalBits < numNumericFeatures;  nextUnBalBits++)
    {
      config->UnBalancedBits (nextUnBalBits);

      double  timeToTrain       = 0.0;
      double  timeToTest        = 0.0;
      float   accuracyMean      = 0.0f;
      double  numSupportVectors = 0.0;
      CompressionStats      compressionStats;
      TestASpecificConfiguraion  (learningAlgorithm, 
                                  config, 
                                  true,            // true = data is already normalized
                                  sampleTrainData, 
                                  sampleTestData, 
                                  accuracyMean, 
                                  timeToTrain, 
                                  timeToTest, 
                                  classedCorrectly, 
                                  compressionStats, 
                                  numSupportVectors
                                 );

      float  mcNemarsTest = McNemarsTest (sampleTestDataSize, baseLineClassedCorrectly, classedCorrectly);
      float  accLoss = 100.0f * (baseLineAccuracy - accuracyMean) / baseLineAccuracy;

      double procTime  = timeToTrain + timeToTest;

      report << bitReduction                       << "\t" 
             << nextUnBalBits                      << "\t"
             << compressionStats.UnBalFeatures ()  << "\t"
             << compressionStats.compression_ratio << "\t"
             << accuracyMean   << "%"              << "\t"
             << accLoss        << "%"              << "\t"
             << mcNemarsTest                       << "\t"
             << timeToTrain                        << "\t"
             << timeToTest                         << "\t"
             << procTime                           << "\t"
             << endl;

      report.flush ();

      KKStr  unBalFeaturesThisLevel = compressionStats.UnBalFeatures ();

      if  (accLoss > accLossTolerance)
        break;

      unBalFeatures = unBalFeaturesThisLevel;
      unBalBits = nextUnBalBits;
    }

    double  endTime = osGetSystemTimeUsed ();
    procTimeBestUnBalBits = endTime - startTime;
  }


  {
    // Lets see how well these parameters work on the full dataset.
    config->CompressionMethod (BRunBalancedSpecified);
    config->BitsToReduceBy    (bitReduction);
    config->UnBalancedBits    (unBalBits);
    config->UnBalancedBitsStr (unBalFeatures);


    double  timeToTrain       = 0.0;
    double  timeToTest        = 0.0;
    float   accuracyMean      = 0.0f;
    double  numSupportVectors = 0.0;
    CompressionStats      compressionStats;
    TestASpecificConfiguraion  (learningAlgorithm, 
                                config,
                                true,     // true = data is already normalized.
                                trainDataNorm, 
                                testDataNorm, 
                                accuracyMean, 
                                timeToTrain, 
                                timeToTest, 
                                NULL, 
                                compressionStats, 
                                numSupportVectors
                               );

    report  << endl
            << "Full Data Set" << endl
            << endl;

    float  accLoss  = 100.0f * (baseLineAccuracy - accuracyMean) / baseLineAccuracy;
    double procTime = timeToTrain + timeToTest;

    report << bitReduction                        << "\t" 
           << unBalBits                           << "\t"
           << compressionStats.UnBalFeatures ()   << "\t"
           << compressionStats.compression_ratio  << "\t"
           << accuracyMean   << "%"               << "\t"
           << accLoss        << "%"               << "\t"
           << "***"                               << "\t"
           << timeToTrain                         << "\t"
           << timeToTest                          << "\t"
           << procTime                            << "\t"
           << endl;

    report.flush ();

    accuracy       = accuracyMean;
    supportVectors = float (numSupportVectors);
    redTime        = compressionStats.elapsed_cpu_time;
    trainTime      = timeToTrain;
    testTime       = timeToTest;
    redPreCount    = compressionStats.num_images_before;
    redPostCount   = compressionStats.num_images_after;
    redRatio       = compressionStats.compression_ratio;
  }


  {
    // Print out timing results

    totalTuningTime = procTimeBaseLineAccuracy + 
                      procTimeSampleData       +
                      procTimeFindStartBitRed  +
                      procTimeBestBitRed       +
                      procTimeBestUnBalBits;

    double  totalTuneAndTrainTime = totalTuningTime + trainTime;

    report << endl << endl 
           << "Timing Results" << endl
           << "Base Line Accuracy"     << "\t"  << procTimeBaseLineAccuracy  << endl
           << "Extract Sample Data"    << "\t"  << procTimeSampleData        << endl
           << "Find Starting BitRed"   << "\t"  << procTimeFindStartBitRed   << endl
           << "Find Best BitRed"       << "\t"  << procTimeBestBitRed        << endl
           << "Find Best Unbal Bits"   << "\t"  << procTimeBestUnBalBits     << endl
           << "Total Tuning Time"      << "\t"  << totalTuningTime           << endl
           << "TrainTime on Full Set"  << "\t"  << trainTime                 << endl
           << "Toatl Tune and Train"   << "\t"  << totalTuneAndTrainTime     << endl
           << endl
           << endl;

    report.flush ();
  }


  delete  baseLineClassedCorrectly;  baseLineClassedCorrectly = NULL;
  delete  classedCorrectly;          classedCorrectly         = NULL;

  delete  sampleTrainData;   sampleTrainData = NULL;
  delete  sampleTestData;    sampleTestData  = NULL;

  delete  trainDataNorm;     trainDataNorm = NULL;
  delete  testDataNorm;      testDataNorm  = NULL;
}  /* RunTuningExample1 */










void  BitReductionJob::RunTuningExample2 (TrainingConfigurationPtr  config,
                                          FeatureVectorListPtr      trainData,
                                          FeatureVectorListPtr      testData,
                                          MLClassListPtr         mlClasses,
                                          float                     samplePercentage,
                                          ostream&                  report,
                                          float                     accLossTolerance
                                         )

{
  randomSeed = lrand48 ();
  srand48 (randomSeed);

  bool  cancelFlag = false;

  FileDescPtr  fileDesc = trainData->FileDesc ();

  FeatureVectorListPtr  sampleTrainData = NULL;
  FeatureVectorListPtr  sampleTestData  = NULL;


  baseLineAccuracy         = 0.0f;
  procTimeBaseLineAccuracy = 0.0;
  procTimeSampleData       = 0.0;
  procTimeFindStartBitRed  = 0.0;
  procTimeBestBitRed       = 0.0;
  procTimeBestUnBalBits    = 0.0;


  config->KernalType         (kernalType);
  config->EncodingMethod     (encodingMethod);
  config->WeightBitReduction (true);
  config->C_Param            (cParam);
  config->Gamma              (gamma);

  bitReduction = 0;

  {
    // Step One;   Random Sample Full dadaset
    double  startTime = osGetSystemTimeUsed ();
    ExtractRandomSamplingForTrainAndTest (trainData, sampleTrainData, sampleTestData, samplePercentage);
    sampledTrainingSetSize = sampleTrainData->QueueSize ();
    double  endTime = osGetSystemTimeUsed ();
    procTimeSampleData  = endTime - startTime;
  }

  int     sampleTestDataSize = sampleTestData->QueueSize ();
  bool*   baseLineClassedCorrectly = new bool[sampleTestDataSize];
  bool*   classedCorrectly         = new bool[sampleTestDataSize];

  {
    // Step Two;  get base line accuracy
    double  startTime = osGetSystemTimeUsed ();

    bool  cancelFlag = false;

    config->BitsToReduceBy (0);
    config->CompressionMethod (BRnoCompression);

    CrossValidationPtr  cv = 
      new CrossValidation(config, 
                          sampleTrainData,
                          mlClasses, 
                          10, 
                          false,     // false = Features are NOT alrady normalized
                          fileDesc,
                          log,
                          cancelFlag
                         );

    cv->RunValidationOnly (sampleTestData, baseLineClassedCorrectly);

    baseLineAccuracy = cv->AccuracyMean ();

    double  endTime = osGetSystemTimeUsed ();
    procTimeBaseLineAccuracy = endTime - startTime;

    report << endl
           << "BaseLine Accuracy" << "\t"  << baseLineAccuracy << "%" << "\t"
           << "Processing Time"   << "\t"  << procTimeBaseLineAccuracy
           << endl
           << endl;

    report.flush ();
    delete  cv;  cv = NULL;
  }


  {
    // Step Four  Search for bst Bit Reduction Level.
    double  startTime = osGetSystemTimeUsed ();
    report << endl
           << endl
           << "Bit"  << "\t"  << "UnBal" << "\t" << "UnBal"  << "\t" << "Red"    << "\t" << "Test"     << "\t" << "Acc"  << "\t" << "McNemars" << "\t" << "Train" << "\t" << "Test" << "\t" << "TrainTest" << endl
           << "Red"  << "\t"  << "Bits"  << "\t" << "Bits"   << "\t" << "Ratio"  << "\t" << "Accuracy" << "\t" << "Loss" << "\t" << "Test"     << "\t" << "Time"  << "\t" << "Time" << "\t" << "Time"      << endl;
    report.flush ();

    for  (bitReduction = 13;  bitReduction > 0;  bitReduction--)
    {
      config->CompressionMethod (BRcompressionPost);
      config->BitsToReduceBy (bitReduction);
      config->UnBalancedBits (0);

      CrossValidationPtr cv = 
           new CrossValidation (config, 
                                sampleTrainData,
                                mlClasses, 
                                10, 
                                false,     // false = Features are NOT already normalized
                                fileDesc,
                                log,
                                cancelFlag
                               );

      cv->RunValidationOnly (sampleTestData, classedCorrectly);

      float  mcNemarsTest = McNemarsTest (sampleTestDataSize, baseLineClassedCorrectly, classedCorrectly);
      float  accLoss = 100.0f * (baseLineAccuracy - cv->AccuracyMean ()) / baseLineAccuracy;
      double procTime  = cv->TrainTimeMean () + cv->TestTimeMean ();

      report << bitReduction               << "\t" 
             << "-"                        << "\t"   // No Unbalanced Bits
             << "-"                        << "\t"
             << cv->ReductionRatio ()      << "\t"
             << cv->AccuracyMean () << "%" << "\t"
             << accLoss             << "%" << "\t"
             << mcNemarsTest               << "\t"
             << cv->TrainTimeMean ()       << "\t"
             << cv->TestTimeMean ()        << "\t"
             << procTime
             << endl;

      report.flush ();

      delete  cv;  cv = NULL;

      if  (accLoss < accLossTolerance)
        break;
    }
      
    double  endTime = osGetSystemTimeUsed ();
    procTimeBestBitRed = endTime - startTime;

    // end of Step Four
  }

  {
    // Step Five;  Determine optimal unbalanced bits
    unBalFeatures = "";

    double  startTime = osGetSystemTimeUsed ();

    config->CompressionMethod (BRunBalancedVariance);
    config->BitsToReduceBy (bitReduction);

    int  numNumericFeatures = BitReductionJobList::DeriveNumNumericFeatures (config);

    for  (unBalBits = numNumericFeatures - 1;  unBalBits > 0;  unBalBits--)
    {
      config->UnBalancedBits (unBalBits);
      CrossValidationPtr cv = 
        new CrossValidation(config, 
                            sampleTrainData,
                            mlClasses, 
                            10, 
                            false,     // false = Features are NOT already normalized
                            fileDesc,
                            log,
                            cancelFlag
                           );

      cv->RunValidationOnly (sampleTestData, classedCorrectly);
      float  mcNemarsTest = McNemarsTest (sampleTestDataSize, baseLineClassedCorrectly, classedCorrectly);
      float  accLoss = 100.0f * (baseLineAccuracy - cv->AccuracyMean ()) / baseLineAccuracy;

      double procTime  = cv->TrainTimeMean () + cv->TestTimeMean ();

      report << bitReduction                      << "\t" 
             << unBalBits                         << "\t"
             << cv->CompStats ().UnBalFeatures () << "\t"
             << cv->ReductionRatio ()             << "\t"
             << cv->AccuracyMean () << "%"        << "\t"
             << accLoss             << "%"        << "\t"
             << mcNemarsTest                      << "\t"
             << cv->TrainTimeMean ()              << "\t"
             << cv->TestTimeMean ()               << "\t"
             << procTime                          << "\t"
             << endl;

      report.flush ();

      KKStr  lastUnBalFeatures = cv->CompStats ().UnBalFeatures ();

      delete  cv;  cv = NULL;

      if  (accLoss <= accLossTolerance)
      {
        unBalFeatures = lastUnBalFeatures;
        break;
      }
    }

    double  endTime = osGetSystemTimeUsed ();
    procTimeBestUnBalBits = endTime - startTime;
  }


  {
    // Lets see how well these parameters work on the full dataset.
    config->CompressionMethod (BRunBalancedSpecified);
    config->BitsToReduceBy    (bitReduction);
    config->UnBalancedBits    (unBalBits);
    config->UnBalancedBitsStr (unBalFeatures);
    CrossValidationPtr  cv =
      new CrossValidation (config, 
                           trainData,  
                           mlClasses, 
                           10, 
                           false,     // false = features are NOT already Normalized
                           fileDesc,
                           log,
                           cancelFlag
                          );

    cv->RunValidationOnly (testData, NULL);

    report  << endl
            << "Full Data Set" << endl
            << endl;

    float  accLoss  = 100.0f * (baseLineAccuracy - cv->AccuracyMean ()) / baseLineAccuracy;
    double procTime = cv->TrainTimeMean () + cv->TestTimeMean ();

    report << bitReduction                      << "\t" 
           << unBalBits                         << "\t"
           << cv->CompStats ().UnBalFeatures () << "\t"
           << cv->ReductionRatio ()             << "\t"
           << cv->AccuracyMean () << "%"        << "\t"
           << accLoss             << "%"        << "\t"
           << "***"                             << "\t"
           << cv->TrainTimeMean ()              << "\t"
           << cv->TestTimeMean ()               << "\t"
           << procTime                          << "\t"
           << endl;

    report.flush ();

    accuracy       = cv->AccuracyMean              ();
    supportVectors = float (cv->SupportPointsMean ());
    redTime        = cv->ReductionTimeMean         ();
    trainTime      = cv->TrainTimeMean             ();
    testTime       = cv->TestTimeMean              ();
    redPreCount    = cv->ReductionPreExampleCount  ();
    redPostCount   = cv->ReductionPostExampleCount ();
    redRatio       = cv->ReductionRatio            ();

    delete  cv;  cv = NULL;
  }


  {
    // Print out timing results

    totalTuningTime = procTimeBaseLineAccuracy + 
                      procTimeSampleData       +
                      procTimeFindStartBitRed  +
                      procTimeBestBitRed       +
                      procTimeBestUnBalBits;

    double  totalTuneAndTrainTime = totalTuningTime + trainTime;

    report << endl << endl 
           << "Timing Results" << endl
           << "Base Line Accuracy"     << "\t"  << procTimeBaseLineAccuracy  << endl
           << "Extract Sample Data"    << "\t"  << procTimeSampleData        << endl
           << "Find Starting BitRed"   << "\t"  << procTimeFindStartBitRed   << endl
           << "Find Best BitRed"       << "\t"  << procTimeBestBitRed        << endl
           << "Find Best Unbal Bits"   << "\t"  << procTimeBestUnBalBits     << endl
           << "Total Tuning Time"      << "\t"  << totalTuningTime           << endl
           << "TrainTime on Full Set"  << "\t"  << trainTime                 << endl
           << "Toatl Tune and Train"   << "\t"  << totalTuneAndTrainTime     << endl
           << endl
           << endl;

    report.flush ();
  }


  delete  baseLineClassedCorrectly;  baseLineClassedCorrectly = NULL;
  delete  classedCorrectly;          classedCorrectly         = NULL;

  delete  sampleTrainData;   sampleTrainData = NULL;
  delete  sampleTestData;    sampleTestData  = NULL;

}  /* RunTuningExample2 */






void   BitReductionJob::ZeroOut ()
{
  bitReduction              = 0;
  unBalBits                 = 0;
  accuracy                  = 0.0f;
  supportVectors            = 0.0f;
  redTime                   = 0;
  trainTime                 = 0;
  testTime                  = 0;
  redPreCount               = 0;
  redPostCount              = 0;
  redRatio                  = 0;
  testStat                  = 0;
  randSampAccuracy          = 0;
  procTimeBaseLineAccuracy  = 0;
  procTimeSampleData        = 0;
  procTimeFindStartBitRed   = 0;
  procTimeBestBitRed        = 0;
  procTimeBestUnBalBits     = 0;
  baseLineAccuracy          = 0;
  sampledTrainingSetSize    = 0;
  startBitReductionLevel    = 0;
  totalTuningTime           = 0;
  unBalFeatures             = "";
  }  /* ZeroOut */
