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


#include  "RandomSampleJob.h"



#include  "CrossValidation.h"
#include  "Orderings.h"
#include  "OSservices.h"
#include  "KKQueue.h"
#include  "RunLog.h"
#include  "StatisticalFunctions.h"
#include  "Str.h"
#include  "TrainingProcess.h"









RandomSampleJob::RandomSampleJob (TrainingConfigurationPtr  _config,
                                  OrderingsPtr              _orderings,
                                  int                       _jobId,
                                  int                       _orderingNum,
                                  int                       _numExamplesToKeep
                                 ):

  accuracy             (-1.0f),
  c                    (_config->C_Param ()),
  compMethod           (BRNull),
  config               (new TrainingConfiguration (*_config)),
  crossValidation      (NULL),
  encodingMethod       (_config->EncodingMethod ()),
  gamma                (_config->Gamma ()),
  jobId                (_jobId),
  kernelType           (_config->KernalType ()),
  log                  (_orderings->Log ()),
  numExamplesToKeep    (_numExamplesToKeep),
  orderings            (_orderings),
  orderingNum          (_orderingNum),
  status               (rjOpen),
  supportVectors       (0.0),
  testTime             (0.0),
  trainTime            (0.0)

{}



RandomSampleJob::RandomSampleJob (TrainingConfigurationPtr  _config,
                                  OrderingsPtr              _orderings,
                                  int                       _numExamplesToKeep,
                                  const KKStr&             statusLine
                                 ):

  accuracy                   (-1.0f),
  c                          (-1),
  compMethod                 (BRNull),
  config                     (new TrainingConfiguration (*_config)),
  crossValidation            (NULL),
  encodingMethod             (Encoding_NULL),
  gamma                      (-1.0),
  log                        (_orderings->Log ()),
  jobId                      (-1),
  kernelType                 (KT_NULL),
  numExamplesToKeep          (_numExamplesToKeep),
  orderings                  (_orderings),
  orderingNum                (-1),
  testTime                   (0.0),
  status                     (rjNULL),
  supportVectors             (0.0),
  trainTime                  (0.0)

{
  ProcessStatusStr (statusLine);
}



RandomSampleJob::~RandomSampleJob ()
{
  delete  crossValidation;
  delete  config;
}




KKStr  RandomSampleJob::JobStatusToStr (rjJobStatus  status)
{
  if  (status == rjOpen)
    return  "Open";

  if  (status == rjStarted)
    return "Started";

  if  (status == rjDone)
    return "Done";

  return  "";
}  /* JobStatusToStr */



rjJobStatus  RandomSampleJob::JobStatusFromStr (KKStr  statusStr)
{
  statusStr.Upper ();

  if  (statusStr == "OPEN")
    return  rjOpen;

  if  (statusStr == "STARTED")
    return rjStarted;

  if  (statusStr == "DONE")
    return rjDone;


  return rjNULL;
}  /* JobStatusFromStr */



KKStr  RandomSampleJob::ToStatusStr ()
{
  KKStr  statusStr (200);  // Preallocating 200 bytes.

  statusStr << "JobId"          << "\t" << jobId                                << "\t"
            << "Status"         << "\t" << JobStatusToStr (status)              << "\t"
            << "Kernel"         << "\t" << KernalTypeToStr (kernelType)         << "\t"
            << "Encoding"       << "\t" << EncodingMethodToStr (encodingMethod) << "\t"
            << "CompMethod"     << "\t" << CompressionMethodStr (compMethod)    << "\t"
            << "C_Param"        << "\t" << c                                    << "\t"
            << "Gamma"          << "\t" << gamma                                << "\t"
            << "OrderingNum"    << "\t" << orderingNum                          << "\t"
            << "ExamplesToKeep" << "\t" << numExamplesToKeep                    << "\t"
            << "Accuracy"       << "\t" << accuracy                             << "\t"
            << "TrainTime"      << "\t" << trainTime                            << "\t"
            << "PredTime"       << "\t" << testTime                             << "\t"
            << "SupportVectors" << "\t" << supportVectors;

  return  statusStr;
}  /* ToStatusStr */



void  RandomSampleJob::ProcessStatusStr (KKStr  statusStr)
{
  log.Level (30) << "RandomSampleJob::ProcessStatusStr[" << statusStr << "]" << endl;
  KKStr  fieldName;
  KKStr  fieldValue;

  compMethod = BRNull;

  fieldName = statusStr.ExtractToken ("\t\n\r");

  while  (!fieldName.Empty ())
  {
    fieldName.Upper ();

    fieldValue = statusStr.ExtractToken ("\t\n\r");
    fieldValue.TrimLeft ("\n\r\t ");
    fieldValue.TrimRight ("\n\r\t ");

    if  (fieldName == "JOBID")
      jobId = atoi (fieldValue.Str ());

    else if  (fieldName == "STATUS")
      status = JobStatusFromStr (fieldValue);

    else if  (fieldName == "KERNEL")
      kernelType = KernalTypeFromStr (fieldValue);

    else if  (fieldName == "ENCODING")
      encodingMethod = EncodingMethodFromStr (fieldValue);

    else if  (fieldName == "COMPMETHOD")
      compMethod = CompressionMethodFromStr (fieldValue);

    else if  (fieldName == "C_PARAM")
      c = atoi (fieldValue.Str ());

    else if  (fieldName == "GAMMA")
      gamma = atof (fieldValue.Str ());

    else if  (fieldName == "ORDERINGNUM")
      orderingNum = atoi (fieldValue.Str ());

    else if  (fieldName == "EXAMPLESTOKEEP")
      numExamplesToKeep = atoi (fieldValue.Str ());

    else if  (fieldName == "ACCURACY")
      accuracy = (float)atof (fieldValue.Str ());

    else if  (fieldName == "TRAINTIME")
    {
      trainTime = atof (fieldValue.Str ());
    }

    else  if  (fieldName == "PREDTIME")
      testTime = (float)atof (fieldValue.Str ());

    else if  (fieldName == "SUPPORTVECTORS")
      supportVectors = (float)atof (fieldValue.Str ());

    else
    {
      log.Level (-1) << "RandomSampleJob::ProcessStatusStr  Invalid Field Name[" << fieldName << "]." << endl;
    }

    fieldName = statusStr.ExtractToken ("\t\n\r");
  }
}  /* ProcessStatusStr */




void  RandomSampleJob::EvaluteNode (FeatureVectorListPtr  validationData,
                                    MLClassListPtr     classes
                                   )
{
  log.Level (9) << "  " << endl;
  log.Level (9) << "  " << endl;
  log.Level (9) << "RandomSampleJob::EvaluteNode JobId[" << jobId << "] Ordering[" << orderingNum << "]" << endl;

  status = rjStarted;

  config->CompressionMethod (BRnoCompression);
  config->KernalType        (kernelType);
  config->EncodingMethod    (encodingMethod);
  config->C_Param           (c);
  config->Gamma             (gamma);

  FileDescPtr fileDesc = config->FileDesc ();


  const FeatureVectorListPtr  srcExamples = orderings->Ordering (orderingNum);

  if  (numExamplesToKeep > srcExamples->QueueSize ())
  {
    log.Level (-1) << endl << endl << endl
                   << "RandomSampleJob::EvaluteNode     *** ERROR ***    RandomExamples to large" << endl
                   << endl
                   << "                     RandomExamples > num in Training set." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }



  FeatureVectorListPtr  trainingData = new FeatureVectorList (srcExamples->FileDesc (), false, log, 10000);
  for  (int x = 0;  x < numExamplesToKeep;  x++)
  {
    trainingData->PushOnBack (srcExamples->IdxToPtr (x));
  }

  bool  allClassesRepresented = true;
  {
    MLClassListPtr  classesInRandomSample = trainingData->ExtractListOfClasses ();
    if  (*classesInRandomSample != (*classes))
    {
      log.Level (-1) << endl << endl
                     << "RandomSampling    *** ERROR ***" << endl
                     << endl
                     << "                  Missing Classes From Random Sample." << endl
                     << endl
                     << "MLClasses[" << classes->ToCommaDelimitedStr               () << "]" << endl
                     << "Found       [" << classesInRandomSample->ToCommaDelimitedStr () << "]" << endl
                     << endl;

       allClassesRepresented = false;

    }

    delete  classesInRandomSample;  classesInRandomSample = NULL;
  }


  //if  (!allClassesRepresented)
  //{
  //  accuracy  = 0.0;
  //  trainTime = 0.0;
  //  testTime  = 0.0;
  //}
  //else
  {
    delete  crossValidation;  crossValidation = NULL;

    compMethod = config->CompressionMethod ();

    bool  cancelFlag = false;

    crossValidation = new CrossValidation 
                              (config,
                               trainingData,
                               classes,
                               10,
                               false,   //  False = Features are not normalized already.
                               trainingData->FileDesc (),
                               log,
                               cancelFlag
                              );

    crossValidation->RunValidationOnly (validationData, NULL);

    accuracy  = crossValidation->Accuracy ();
    trainTime = crossValidation->TrainTimeMean ();
    testTime  = crossValidation->TestTimeMean ();
    supportVectors = crossValidation->SupportPointsMean ();
  }

  delete  trainingData;

  status = rjDone;
}  /* EvaluteNode */




void  RandomSampleJob::ReportResultsHeader (ostream&   r)
{
  r << "Job"        << "\t"
    << "SVM"        << "\t"
    << "Feature"    << "\t"
    << "Compresion" << "\t"
    << ""           << "\t"   // C
    << ""           << "\t"   // Gamma
    << "Order"      << "\t" 
    << "Num"        << "\t"   // Examples
    << "Sample"     << "\t"   // Sample Accurcay
    << "Support"    << "\t"
    << "Train"      << "\t"
    << "Test"       << "\t"
    << endl;

  r << "Id"         << "\t"
    << "Kernel"     << "\t"
    << "Encoding"   << "\t"
    << "Method"     << "\t"
    << "C"          << "\t"
    << "Gamma"      << "\t"
    << "Num"        << "\t"
    << "Examples"   << "\t"  
    << "Accuracy"   << "\t"   // Sample Accuracy
    << "Points"     << "\t" 
    << "Time"       << "\t"   // Train Time
    << "Time"       << "\t"   // Test Time
    << endl;

  r.flush ();

}  /* ReportResultsHeader */


void  RandomSampleJob::ReportResults (ostream&   r)
{
    
  bool  errorOccured = false;
  do
  {
    errorOccured = false;

    r << jobId                                               << "\t"
      << KernalTypeToStr      (config->KernalType ())        << "\t"
      << EncodingMethodToStr  (config->EncodingMethod ())    << "\t"
      << CompressionMethodStr (compMethod)                   << "\t"
      << config->C_Param ()                                  << "\t"
      << config->Gamma   ()                                  << "\t" 
      << orderingNum                                         << "\t"
      << numExamplesToKeep                                   << "\t" 
      << Accuracy () << "%"                                  << "\t"  
      << SupportVectors ()                                   << "\t"  
      << trainTime                                           << "\t"
      << testTime
      << endl;
  
    r.flush ();

    if  (r.fail ())
    {
      r.clear ();
      errorOccured = true;
      #ifdef  WIN32
      Sleep (30000);
      #else
      sleep (30);
      #endif
    }

  }  while  (errorOccured);

}  /* ReportResults */




void  RandomSampleJob::DeleteCrossValidation ()
{
  delete  crossValidation;
  crossValidation = NULL;
}  /* DeleteCrossValidation */






RandomSampleJobList::RandomSampleJobList (RunLog&  _log):

   KKQueue<RandomSampleJob> (true, 500),
   jobFileName   (),
   log           (_log)

{
}




RandomSampleJobList::RandomSampleJobList (RunLog&                   _log,
                                          TrainingConfigurationPtr  _config,
                                          KKStr                    trainDataFileName,
                                          FeatureVectorListPtr      trainData,
                                          OrderingsPtr              orderings
                                         ):

   KKQueue<RandomSampleJob> (true, 500),
   jobFileName   (),
   log           (_log)

{
  TrainingConfigurationPtr config = new TrainingConfiguration (*_config);

  KKStr  kernmalShortName;
  KKStr  encShortName;


  if  (config->UnBalancedBits () == 0)
    config->CompressionMethod (BRcompressionPost);
  else
    config->CompressionMethod (BRunBalancedVariance);

  log.Level (10) << endl << endl << "RandomSampleJobList" << endl
                 << "BitReduction[" << config->BitsToReduceBy () << "]" << endl
                 << "UnBalBits   [" << config->UnBalancedBits () << "]" << endl
                 << "CompMethd   [" << CompressionMethodStr (config->CompressionMethod ()) << "]" << endl
                 << endl;

  int  numCompressedImages = DetermineCompressedImageCount (trainData, config);

  switch  (config->ModelingMethod ())
  {
  case  NoEncoding:
  case  Encoding_NULL:
     encShortName = "None";
     break;

  case  BinaryEncoding:
     encShortName = "Binary";
     break;

  case  ScaledEncoding:
     encShortName = "Scale";
     break;
  }


  switch  (config->KernalType ())
  {
  case  KT_NULL:
  case  KT_Linear:
     kernmalShortName = "Linear";
     break;

  case  KT_Polynomial:
     kernmalShortName = "Poly";
     break;

  case  KT_RBF:
     kernmalShortName = "RBF";
     break;
  }

  jobFileName = osRemoveExtension (trainDataFileName) + 
                "_" + "RandomSampling"                +
                "_" + kernmalShortName                +
                "_" + encShortName;

  if  (config->C_Param () != 1)
    jobFileName << "_C-" + StrFormatInt (int (config->C_Param ()), "00");

  jobFileName <<  "_" <<  StrFormatInt (numCompressedImages, "000000")
              <<  ".status";
 
  if  (osFileExists (jobFileName))
  {
    Load (config, orderings, numCompressedImages);
  }
  else
  {
    CreateRandomJobs (config, orderings, numCompressedImages);
    Save (jobFileName);
  }

  delete  config;  config = NULL;
}





RandomSampleJobList::RandomSampleJobList (RunLog&                   _log,
                                          TrainingConfigurationPtr  _config,
                                          OrderingsPtr              _orderings,
                                          int                       _numExamplesToKeep,
                                          const KKStr&             _fileName
                                         ):
  KKQueue<RandomSampleJob> (true, 500),
  jobFileName  (_fileName),
  log          (_log)

{
  log.Level (10) << endl << endl;
  log.Level (10) << "RandomSampleJobList::RandomSampleJobList" << endl;
  log << "         numExamplesToKeep[" << _numExamplesToKeep << "]" << endl;
  log << "         fileName         [" << _fileName          << "]" << endl;
  Load (_config, _orderings, _numExamplesToKeep);
}





int   RandomSampleJobList::DetermineCompressedImageCount (FeatureVectorListPtr       trainData,
                                                          TrainingConfigurationPtr   config
                                                         )
{
  FileDescPtr  fileDesc = trainData->FileDesc ();
  FeatureVectorListPtr  srcImages = trainData->DuplicateListAndContents ();
  FeatureVectorListPtr  imagesToTrain = new FeatureVectorList (fileDesc, false, log, 10000);

  MLClassListPtr mlClasses = srcImages->ExtractListOfClasses ();

  {
    MLClassList::const_iterator  idx;

    for  (idx = mlClasses->begin ();  idx != mlClasses->end ();  idx++)
    {
      MLClassPtr  mlClass = *idx;
      FeatureVectorListPtr  imagesThisClass = srcImages->ExtractImagesForAGivenClass (mlClass);
      imagesToTrain->AddQueue (*imagesThisClass);
      delete  imagesThisClass;
    }
  }

  NormalizationParms  normParms (config, *imagesToTrain, log);
  normParms.NormalizeImages (imagesToTrain);

  ClassAssignments  classAssignments (*mlClasses, log);
  FeatureVectorListPtr  compressedImageList = new FeatureVectorList (fileDesc, true, log, 10000);

  BitReduction br (config->SVMparamREF (), fileDesc, trainData->AllFeatures ());

  CompressionStats compressionStats 
              = br.compress (*imagesToTrain, 
                             compressedImageList, 
                             classAssignments
                            );


  int  compressedImageCount = compressionStats.num_images_after;

  log.Level (10) << "DetermineCompressedImageCount  compressedImageCount[" << compressedImageCount << "]" << endl;

  delete  compressedImageList;  compressedImageList = NULL;
  delete  mlClasses;         mlClasses        = NULL;
  delete  imagesToTrain;        imagesToTrain       = NULL;
  delete  srcImages;            srcImages           = NULL;

  return compressedImageCount;
}  /* DetermineCompressedImageCount */








RandomSampleJobListPtr  RandomSampleJobList::CreateNewList (RunLog&                   _log,
                                                            TrainingConfigurationPtr  _config,
                                                            OrderingsPtr              _orderings,
                                                            int                       _numExamplesToKeep,
                                                            const KKStr&             _fileName
                                                            )
{

  _log.Level (10) << endl << endl << endl;
  _log << "RandomSampleJobList::CreateNewList" << endl;
  _log << "            fileName         [" << _fileName          << "]" << endl;
  _log << "            numExamplesToKeep[" << _numExamplesToKeep << "]" << endl;          

  RandomSampleJobListPtr  list = NULL;

  if  (osFileExists (_fileName))
  {
    _log << "Ordering Files already Exists." << endl;
    list = new RandomSampleJobList (_log, _config, _orderings, _numExamplesToKeep, _fileName);
  }
  else
  {
    _log << "Ordering Files DID NOT exists" << endl;
    list = new RandomSampleJobList (_log);

    int  orderingNum = 0;

    for  (orderingNum = 0;  orderingNum < (int)_orderings->NumOfOrderings ();  orderingNum++)
    {
      RandomSampleJobPtr  j = new RandomSampleJob (_config, _orderings, orderingNum, orderingNum, _numExamplesToKeep);
      list->PushOnBack (j);
    }

    list->Save (_fileName);
  }

  return  list;
}  /* CreateNewList */



void  RandomSampleJobList::CreateRandomJobs (TrainingConfigurationPtr  _config,
                                             OrderingsPtr              _orderings,
                                             int                       _numExamplesToKeep
                                            )
{
  log.Level (10) << endl << endl;
  log << "RandomSampleJobList::CreateRandomJobs" << endl;
  log << "                numExamplesToKeep[" << _numExamplesToKeep << "]" << endl;


  int  orderingNum = 0;

  for  (orderingNum = 0;  orderingNum < (int)_orderings->NumOfOrderings ();  orderingNum++)
  {
    RandomSampleJobPtr  j = new RandomSampleJob (_config, _orderings, orderingNum, orderingNum, _numExamplesToKeep);
    PushOnBack (j);
  }
}  /* CreateRandomJobs */




void  RandomSampleJobList::Load (TrainingConfigurationPtr  _config,
                                 OrderingsPtr              _orderings,
                                 int                       _numExamplesToKeep
                                )
{
  FILE*  in = fopen (jobFileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "RandomSampleJobList::Load    *** ERROR ***" << endl
                   << endl
                   << "     Error Opening Status File[" << jobFileName << "]" << endl
                   << endl;
    exit (-1);
  }

  char  buff[10240];

  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  statusStr (buff);
    if  (statusStr.SubStrPart (0, 1) == "//")
    {
      // We have a comment line,  we want to ignore.
    }
    else
    {
      KKStr  field = statusStr.ExtractToken (" \t\n\r");
      field.Upper ();
      if  (field == "JOB")
      {
        RandomSampleJobPtr j = new RandomSampleJob (_config, _orderings, _numExamplesToKeep, statusStr);
        PushOnBack (j);
      }
    }
  }

  fclose (in);
}  /* Load */



void  RandomSampleJobList::Save (const KKStr&  _jobFileName)
{
  jobFileName = _jobFileName;
  Save ();
}



void  RandomSampleJobList::Save ()
{
  KKStr  dirPath, rootName, ext;

  osParseFileName (jobFileName, dirPath, rootName, ext);

  KKStr name1 = osAddSlash (dirPath) + rootName + "_Prev1" + "." + ext;
  KKStr name2 = osAddSlash (dirPath) + rootName + "_Prev2" + "." + ext;

  osDeleteFile (name2);
  osRenameFile (name1, name2);
  osRenameFile (jobFileName, name1);

  ofstream  statusFile (jobFileName.Str ());

  if  (!statusFile.is_open ())
  {
    log.Level (-1) << endl
                   << endl
                   << "RandomSampleJobList::Save    *** ERROR ***" << endl
                   << endl
                   << "Error Saving to file[" << jobFileName << "]" << endl
                   << endl;

    osWaitForEnter ();
    exit (-1);
  }

  statusFile << "// DateWritten  [" << osGetLocalDateTime () << "]" << endl
             << "//" << endl;

  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    statusFile << "Job" << "\t" << IdxToPtr (x)->ToStatusStr () << endl;
  }
  
  statusFile.close ();
}  /* Save */




RandomSampleJobPtr  RandomSampleJobList::LookUpByJobId (int  jobId)
{
  int  x;

  for  (x = 0;  x < QueueSize (); x++)
  {
    RandomSampleJobPtr  j = IdxToPtr (x);
    if  (j->JobId () == jobId)
      return  j;
  }

  return  NULL;
}  /* LookUpByJobId */




RandomSampleJobPtr  RandomSampleJobList::LocateOpenJob ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    RandomSampleJobPtr  j = IdxToPtr (x);
    if  (j->Status () == rjOpen)
      return  j;
  }

  return  NULL;
}  /* LocateOpenJob */




void  RandomSampleJobList::CleanUpCrossValidations ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    RandomSampleJobPtr  j = IdxToPtr (x);
    j->DeleteCrossValidation ();
  }
}  /* CleanUpCrossValidations */





bool  RandomSampleJobList::AreAllJobsAreDone ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    RandomSampleJobPtr  j = IdxToPtr (x);
    if  (j->Status () != rjDone)
      return false;
  }

  return  true;
}  /* AreAllJobsAreDone */




void  RandomSampleJobList::CalcAccuracyStats (double&  accuracyMean, 
                                              double&  accuracyStdDev,
                                              double&  trainTimeMean,
                                              double&  testTimeMean,
                                              double&  supportVectorsMean
                                             )  const
{
  VectorDouble  accuracies;
  VectorDouble  trainTimes;
  VectorDouble  testTimes;
  VectorDouble  supportVectors;

  RandomSampleJobList::const_iterator  idx;
  for  (idx = begin();  idx != end ();  idx++)
  {
    RandomSampleJobPtr j = *idx;
    accuracies.push_back (j->Accuracy ());
    trainTimes.push_back (j->TrainTime ());
    testTimes.push_back  (j->TestTime ());
    supportVectors.push_back (j->SupportVectors ());
  }  

  CalcMeanAndStdDev (accuracies, accuracyMean, accuracyStdDev);

  double  stdDev;
  CalcMeanAndStdDev (trainTimes,     trainTimeMean,      stdDev);
  CalcMeanAndStdDev (testTimes,      testTimeMean,       stdDev);
  CalcMeanAndStdDev (supportVectors, supportVectorsMean, stdDev);

}  /* CalcAccuracyStats */







