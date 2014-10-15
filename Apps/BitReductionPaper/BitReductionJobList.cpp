#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>

#include  <fstream>
#include  <iostream>
#include  <vector>

#include <fcntl.h>
#include <sys/types.h>

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
#include  "BitReductionJobList.h"

#include  "BitReducedExample.h"

#include  "BitReduction.h"
#include  "CrossValidation.h"
#include  "DuplicateImages.h"
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






BitReductionJobList::BitReductionJobList (RunLog&                   _log,
                                          const KKStr&             _rootDirName,
                                          TrainingConfigurationPtr  _config,
                                          const DataSetParameters&  _dsp,
                                          brjJobTypes               _jobTypes,
                                          ostream&                  _report,
                                          float                     _samplePercentage,
                                          laLearningAlgorithms      _learningAlgorithm,
                                          float                     _accLossTolerance
                                         ):
  KKQueue<BitReductionJob> (true, 500),
  accLossTolerance       (_accLossTolerance),
  classedCorrectly       (NULL),
  config                 (NULL),
  configFileName         (),
  dataSetDirName         (),
  dsp                    (_dsp),
  fileDesc               (NULL),
  mlClasses           (NULL),
  learningAlgorithm      (_learningAlgorithm),
  jobFileName            (),
  lockFile               (-1),
  lockFileName           (),
  lockFileOpened         (false),
  log                    (_log),
  nextJobId              (0),
  noCompClassedCorrectly (NULL),
  noCompFileName         (),
  jobTypes               (_jobTypes),
  report                 (_report),
  rootDirName            (_rootDirName),
  samplePercentage       (_samplePercentage),
  splitOrderigsFileName  (),
  testData               (NULL),
  testDataFileName       (),
  trainData              (NULL),
  trainDataFileName      (),
  valid                  (true)

{
  dataSetDirName = osAddSlash (rootDirName) + dsp.dataSetSubDir;

  KKStr  percentageStr       = StrFormatInt ((int)(samplePercentage + 0.5f), "00");
  KKStr  accLossToleranceStr = StrFormatInt ((int)(accLossTolerance * 100.0f), "000");
  KKStr  jobTypeStr = "";
  
  switch  (jobTypes)
  {
  case brjRandomSampling:  jobTypeStr = "_Random-Job";
                           break;
  
  case brjNormalJob:       jobTypeStr = "_Job";
                           break;
  
  case brjTuningTest:      jobTypeStr = "_Tuning3-Job-" + percentageStr + "-" + accLossToleranceStr;
                           break;

  case brjSmallSample:     jobTypeStr = "_SmallSample-Job-" + percentageStr;
                           break;

  default:                 jobTypeStr = "_Random-Split";
                           break;
  }

  KKStr  baseName = osAddSlash (dataSetDirName) 
                     + osRemoveExtension (dsp.trainDataFileName) 
                     + jobTypeStr + "_" 
                     + LearningAlgorithmToShortStr (learningAlgorithm);

  jobFileName  = baseName + "_Job.status";
  lockFileName = baseName + "_Job.lock";
  splitOrderigsFileName = baseName + "_Random-Split.idx";


  Block ();

  InitVariables ();
 
  if  (osFileExists (jobFileName))
  {
    Load ();
  }
  else
  {
    switch  (jobTypes)
    {
    case  brjNormalJob:
    case  brjSmallSample:
    case  brjRandomSampling:
      CreateJobs ();
      break;

    case  brjTuningTest:
      CreateTuningJobs ();
      break;


    case  brjRandomSplits:
      CreateRandomSplitJobs ();
      break;
    }
    Save ();
  }

  EndBlock ();
}




BitReductionJobList::~BitReductionJobList ()
{
  delete[]  classedCorrectly;
  delete    config;
  delete    mlClasses;
  delete[]  noCompClassedCorrectly;
  delete    testData;
  delete    trainData;
}



void  BitReductionJobList::AnalizeDistributionOfFeatureData (const FeatureVectorList&  examples)
{
  // Want to see the distribution of feature data after normalization,  i suspect that
  // during bit reduction after multiplying a normalized feature by 1000, that -1000 -> +2000 
  // contains 95% of the data, so ater 10 bits of reduction (2^10 = 1025) there is only 1 but of data
  // left over 

  MLClassListPtr  mlClasses = examples.ExtractListOfClasses ();
  ClassAssignments   classAssignments (*mlClasses, log);

  int  bitReduction = 0;

  for  (bitReduction = 7;  bitReduction < 13;  bitReduction++)
  {
    FeatureVectorListPtr  dupExamples = new FeatureVectorList (examples, true);

    NormalizationParms normParms (config, *dupExamples, log);
    normParms.NormalizeImages (dupExamples);

    VectorInt  withIn1000 (dupExamples->NumOfFeatures (), 0);
    VectorInt  withIn2000 (dupExamples->NumOfFeatures (), 0);

    int  x;

    FeatureVectorList::const_iterator  idx;
    for  (idx = dupExamples->begin ();  idx != dupExamples->end ();  idx++)
    {
      FeatureVectorPtr example = *idx;
      const FFLOAT*  data = example->FeatureData ();

      for  (x = 0;  x < dupExamples->NumOfFeatures ();  x++)
      {
        int z = int (data[x] * 1000.0 + 0.5);
        if  ((z >= -1000)  &&  (z <= 1000))
          withIn1000[x]++;

        if  ((z >= -2000)  &&  (z <= 2000))
          withIn2000[x]++;
      }
    }

    report << endl << endl << endl
           << "Feature Data Distribution Analysis" << endl
           << endl
           << "Ftur#" << "\t" << "With1000" << "\t" << "Frac" << endl;

    double  numExamples = double (dupExamples->QueueSize ());
    for  (x = 0;  x < dupExamples->NumOfFeatures ();  x++)
    {
      double  frac1000 = double (withIn1000[x]) / numExamples;
      double  frac2000 = double (withIn2000[x]) / numExamples;
      cout << x              << "\t" 
           << withIn1000[x]  << "\t"  
           << frac1000       << "\t"
           << withIn2000[x]  << "\t"  
           << frac2000
           << endl;

      report << x              << "\t" 
             << withIn1000[x]  << "\t" 
             << frac1000       << "\t"
             << withIn2000[x]  << "\t" 
             << frac2000 
             << endl;
    }
  
    report << endl << endl;


    TrainingConfigurationPtr  tempConfig = new   TrainingConfiguration (*config);
    tempConfig->CompressionMethod (BRcompressionPost);
    tempConfig->BitsToReduceBy    (bitReduction);

    BitReduction br (tempConfig->SVMparamREF (), 
                     fileDesc,
                     dupExamples->AllFeatures ()
                    );

    FeatureVectorListPtr  compExamples = new FeatureVectorList (fileDesc, true, log, 1000);
    br.compress (*dupExamples, compExamples, classAssignments);

    report << "Duplicate Report for Bitreduction [" << bitReduction << "]" << endl;

    DuplicateImages dupDetector (compExamples, log);
    dupDetector.ReportDuplicates (report);
    report.flush ();

    delete  compExamples;

    delete  tempConfig;
    delete  dupExamples;
  }

  delete  mlClasses;
}  /* AnalizeDistributionOfFeatureData */




void  BitReductionJobList::ExtractRandomSamplingForTrainAndTest (const KKStr&          fileName,
                                                                 FeatureVectorListPtr&  sampleTrainData,
                                                                 FeatureVectorListPtr&  sampleTestData
                                                                )
{
  KKStr  trainRandomFileName = osRemoveExtension (fileName) + 
                                "_Train-SmallSamp-" + 
                                StrFormatDouble (samplePercentage, "000") +
                                "." + 
                                osGetFileExtension (fileName);


  KKStr  testRandomFileName = osRemoveExtension (fileName) + 
                               "_Test-SmallSamp-" + 
                               StrFormatDouble (samplePercentage, "000") +
                               "." + 
                               osGetFileExtension (fileName);

  bool  trainDataValid = false;
  bool  changesMade    = false;
  bool  cancelFlag     = false;

  sampleTrainData = dsp.fileFormat->LoadFeatureFile (trainRandomFileName, *mlClasses, -1, cancelFlag, trainDataValid, changesMade, log);

  bool  testDataValid = false;
  sampleTestData = dsp.fileFormat->LoadFeatureFile  (testRandomFileName,  *mlClasses, -1, cancelFlag, testDataValid, changesMade, log);

 
  if  (trainDataValid  &&  testDataValid)
    return;

  delete  sampleTrainData;  sampleTrainData = NULL;
  delete  sampleTestData;   sampleTestData  = NULL;


  bool  valid = false;
  FeatureVectorListPtr  data = dsp.fileFormat->LoadFeatureFile (fileName, *mlClasses, -1, cancelFlag, valid, changesMade, log);
  if  (!valid)
  {
    log.Level (-1) << endl
                   << "ProcessOneDataSet    ***Error***    loading data[" << fileName << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  sampleTrainData = new FeatureVectorList (data->FileDesc (), true, log);
  sampleTestData  = new FeatureVectorList (data->FileDesc (), true, log);

  data->RemoveEntriesWithMissingFeatures ();

  MLClassListPtr  classes = data->ExtractListOfClasses ();
  MLClassList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassPtr  ic = *idx;
    FeatureVectorListPtr  examplesThisClass = data->ExtractImagesForAGivenClass (ic);
    examplesThisClass->RandomizeOrder ();

    int  numInClass = examplesThisClass->QueueSize ();
    int  numToKeep = (int)(0.5f + (float)samplePercentage * (float)numInClass / 100.0f);

    FeatureVectorList::iterator  idx2;
    for  (idx2 = examplesThisClass->begin ();  idx2 != examplesThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;
      if  (sampleTrainData->QueueSize () < numToKeep)
        sampleTrainData->PushOnBack (new ImageFeatures (*example));

      else if  (sampleTestData->QueueSize () < (5 * numToKeep))
        sampleTestData->PushOnBack  (new ImageFeatures (*example));

      else
        break;
    }

    delete  examplesThisClass;  examplesThisClass = NULL;
  }

  delete  classes;  classes = NULL;
  delete  data;     data    = NULL;


  return;
}  /* ExtractRandomSamplingForTrainAndTest */






FeatureVectorListPtr  BitReductionJobList::LoadDataFileForRandomSampling (const KKStr&  fileName)
{
  KKStr  randomFileName = osRemoveExtension (fileName) + 
                           "_SmallSamp-" + 
                           StrFormatDouble (samplePercentage, "000") +
                           "." + 
                           osGetFileExtension (fileName);

  bool  cancelFlag  = false;
  bool  changesMade = false;
  bool  valid       = false;
  bool  successful  = false;

  FeatureVectorListPtr  randomSampling = dsp.fileFormat->LoadFeatureFile (randomFileName, *mlClasses, -1, cancelFlag, valid, changesMade, log);
  if  (valid)
  {
    return  randomSampling;
  }
  else
  {
    FeatureVectorListPtr  data = dsp.fileFormat->LoadFeatureFile (fileName, *mlClasses, -1, cancelFlag, valid, changesMade, log);
    if  (!valid)
    {
      log.Level (-1) << endl
                     << "ProcessOneDataSet    ***Error***    loading data[" << fileName << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    data->RemoveEntriesWithMissingFeatures ();

    FeatureVectorListPtr  sampledData = data->ExtractRandomSampling (samplePercentage, 100);
    FeatureVectorListPtr  temp = data;
    data = sampledData->DuplicateListAndContents ();
    delete  temp;         temp        = NULL;
    delete  sampledData;  sampledData = NULL;

    uint  numExamplesWritten = 0;
    dsp.fileFormat->SaveFeatureFile (randomFileName, data->AllFeatures (), *data, numExamplesWritten, cancelFlag, successful, log);

    return  data;
  }
}  /* LoadDataFileForRandomSampling */




FeatureVectorListPtr  BitReductionJobList::CreateListWithNoMissingData (FeatureVectorListPtr  examples)
{
  FeatureVectorListPtr  result = new FeatureVectorList (examples->FileDesc (), true, log, examples->QueueSize ());

  FeatureVectorList::iterator  idx;
  for  (idx = examples->begin ();  idx != examples->end ();  idx++)
  {
    FeatureVectorPtr  example = *idx;
    if  (!(example->MissingData ()))
      result->PushOnBack (new ImageFeatures (*example));
  }

  return  result;
}  /* CreateListWithNoMissingData */




void  BitReductionJobList::InitVariables ()
{
  mlClasses = new MLClassList ();

  bool  changesMade = false;
  bool  cancelFlag  = false;

  trainDataFileName = osAddSlash (dataSetDirName) + dsp.trainDataFileName;
  if  (jobTypes == brjSmallSample)
  {
    ExtractRandomSamplingForTrainAndTest (trainDataFileName, trainData, testData);
  }
  else
  {
    trainData = dsp.fileFormat->LoadFeatureFile (trainDataFileName, *mlClasses, -1, cancelFlag, valid, changesMade, log);
    if  (!valid)
    {
      log.Level (-1) << endl
                     << "ProcessOneDataSet    Error loading training data[" << trainDataFileName << "]" << endl
                     << endl;
      return;
    }
  }

  report << endl << endl
         << "Training File[" << dsp.trainDataFileName << "]  TimeStamp[" << osGetFileDateTime (trainDataFileName) << "]" << endl
         << endl;

  trainData->PrintClassStatistics (report);

  if  (trainData->MissingData ())
  {
      log.Level (0) << endl 
                    << "Test File[" << dsp.trainDataFileName << "] Incudes missing data." << endl
                    << endl;

      report << "Test File[" << dsp.trainDataFileName << "] Incudes missing data." << endl;

      FeatureVectorListPtr  newTrainData = CreateListWithNoMissingData (trainData);
      delete  trainData;
      trainData = newTrainData;
      newTrainData = NULL;
  }


  fileDesc = trainData->FileDesc ();

  if  (!dsp.testDataFileName.Empty ())
  {
    testDataFileName = osAddSlash (dataSetDirName) + dsp.testDataFileName;

    if  (jobTypes != brjSmallSample)
    {
      testData = dsp.fileFormat->LoadFeatureFile (testDataFileName, 
                                                  *mlClasses,
                                                  -1,
                                                  cancelFlag,
                                                  valid,
                                                  changesMade,
                                                  log
                                                 );

      if  ((!valid)  ||  (!testData))
      {
        log.Level (-1) << endl
                       << "ProcessOneDataSet    Error loading test data[" << dsp.testDataFileName << "]" << endl
                       << endl;
        return;
      }
    }

    report << endl << endl << endl
           << "Test File[" << dsp.testDataFileName << "]  TimeStamp[" << osGetFileDateTime (testDataFileName) << "]" << endl
           << endl;

    testData->PrintClassStatistics (report);

    if  (testData->MissingData ())
    {
      log.Level (0) << endl 
                    << "Test File[" << dsp.testDataFileName << "] Incudes missing data." << endl
                    << endl;

      report << "Test File[" << dsp.testDataFileName << "] Incudes missing data." << endl;

      FeatureVectorListPtr  newTestData = CreateListWithNoMissingData (testData);
      delete  testData;
      testData = newTestData;
      newTestData = NULL;
    }
  }

  {
    configFileName = osAddSlash (dataSetDirName) + dsp.configFileName;
    config = new TrainingConfiguration (fileDesc, configFileName, log, false);
    if  (!config->FormatGood ())
    {
      log.Level (-1) << endl
                     << "BitReductionJobList::InitVariables **** Invalid Configuration File[" << configFileName << "] ****" << endl
                     << endl;
      return;
    }
  }

  noCompClassedCorrectly = NULL;
  classedCorrectly       = NULL;

  if  (trainData  &&  testData)
  {
    classedCorrectly = new bool[testData->QueueSize ()];
    noCompClassedCorrectly = new bool[testData->QueueSize ()];

    int  x;
    for  (x = 0;  x < testData->QueueSize ();  x++)
    {
      classedCorrectly[x]       = true;
      noCompClassedCorrectly[x] = false;
    }
  }

  //CompressionStatistics (config, trainData);

}  /* InitVariables */




void  BitReductionJobList::CompressionStatistics (TrainingConfigurationPtr  config,
                                                  FeatureVectorListPtr      trainData
                                                 )
{
  log.Level (10) << "BitReductionJobList::CompressionStatistics" << endl;
  MLClassListPtr  classes = trainData->ExtractListOfClasses ();

  int  numOfClasses = classes->QueueSize ();

  ClassAssignments   classAssignments (*mlClasses, log);

  int  x;

  report << endl << endl << endl
         << "Cross Class Duplicate Counts" << endl << endl
         << "Bit" << "\t" << "Comp"  << "\t" << "Red" << "\t" << endl
         << "Red" << "\t" << "Ratio" << "\t" << "Time";

  for  (x = 0;  x < numOfClasses;  x++)
    report << "\t" << (x + 1);

  for  (x = 0;  x < numOfClasses;  x++)
    report << "\t" << (x + 1);

  report << endl;

  int  bitReduction = 0;
  for  (bitReduction = 0;  bitReduction < 12;  bitReduction++)
  {
    log.Level (10) << "CompressionStatistics  BitRed[" << bitReduction << "]" << endl;

    report << bitReduction;

    FeatureVectorListPtr  dupData = new FeatureVectorList (*trainData, true);

    NormalizationParms normParms (config, *dupData, log);
    normParms.NormalizeImages (dupData);

    {
      //  We will perform a compression to get the stats for this bitReduction level
      SVMparam   svmParams (*(config->Model3Parameters ()));
      svmParams.CompressionMethod (BRcompressionPost);
      svmParams.BitsToReduceBy (bitReduction);

      //  Lets Compress Data to get some stats
      BitReduction  bitRed (svmParams, fileDesc, dupData->AllFeatures ());
      FeatureVectorListPtr  compExamples = new FeatureVectorList (fileDesc, true, log, 1000);

      CompressionStats  stats = bitRed.compress (*dupData, compExamples, classAssignments);

      report << "\t" << stats.compression_ratio
             << "\t" << stats.elapsed_cpu_time;

      delete   compExamples;  compExamples = NULL;
    }


    log.Level (10) << "CompressionStatistics  Building DupTree" << endl;
    BitReducedExampleTree  bitRedTree (*dupData, bitReduction, fileDesc, log);
    log.Level (10) << "CompressionStatistics  * DONE *   Building DupTree" << endl;

    VectorInt  crossClassCounts;
    VectorInt  crossClassCountImages;

    bitRedTree.DeriveDuplicateCounts  (crossClassCounts, crossClassCountImages);

    for  (x = 0;  x < numOfClasses;  x++)
      report << "\t" << crossClassCounts[x];

    for  (x = 0;  x < numOfClasses;  x++)
      report << "\t" << crossClassCountImages[x];
    report << endl;

    log.Level (10) << "CompressionStatistics  - Deleting dupData" << endl;
    delete  dupData;
    log.Level (10) << "CompressionStatistics  - * DONE *    Deleting dupData" << endl;
  }

  delete  classes;
}  /* CompressionStatistics */

                                          



void  BitReductionJobList::SetNoCompFileName (const  BitReductionJobPtr  j)
{
  noCompFileName = osAddSlash (dataSetDirName) + osRemoveExtension (dsp.trainDataFileName);

  switch  (j->KernalType ())
  {
  case      KT_Linear:  noCompFileName << "_Linear"; break;
  case  KT_Polynomial:  noCompFileName << "_Poly";   break;
  case         KT_RBF:  noCompFileName << "_RBF";    break;
  }

  switch  (j->EncodingMethod ())
  {
  case      NoEncoding:  noCompFileName << "_NoEnc";    break;
  case  BinaryEncoding:  noCompFileName << "_BinEnc";   break;
  case  ScaledEncoding:  noCompFileName << "_ScaleEnc"; break;
  }

  if  (jobTypes == brjSmallSample)
  {
    noCompFileName << "_SmallSamp-" + StrFormatDouble (samplePercentage, "000");
  }

  noCompFileName << "_C-" << j->CParam ()
                 << "_ClassedCorrectly.data";
}  /* SetNoCompFileName */



int  BitReductionJobList::DeriveNumNumericFeatures (TrainingConfigurationPtr config)
{
  FeatureNumList  selFeatures = config->GetFeatureNums ();
  int idx                = 0;
  int numNumericFeatures = 0;

  for  (idx = 0;  idx < selFeatures.NumSelFeatures ();  idx++)
  {
    if  (selFeatures.FeatureAttributeType (idx) == MLL::NumericAttribute)
      numNumericFeatures++;
  }

  return  numNumericFeatures;
}  /* DeriveNumNumericFeatures */





void  BitReductionJobList::CreateTuningJobs ()
{
  bool  allFieldsAteNumeric = trainData->AllFieldsAreNumeric ();

  SVM_KernalType  svmKernalType;
  int  kernalIdx = 0;
  for  (kernalIdx = 0;  (svmKernalType = dsp.kernalTypes[kernalIdx]) != KT_NULL;  kernalIdx++)
  {
    config->KernalType (svmKernalType);

    const int defaultCValues[] = {1, 3, 9, 12, 15, -1};
    const int*  cValues = defaultCValues;

    cValues = dsp.c_params;

    int cIDX = 0;

    for  (cIDX = 0; cValues[cIDX] > 0; cIDX++)
    {
      int  cParam = cValues[cIDX];

      config->C_Param (cParam);


      SVM_EncodingMethod  encodingMethod = BinaryEncoding;
      int  encodingIdx = 0;
      for  (encodingIdx = 0;  dsp.encodingMethods[encodingIdx] != Encoding_NULL;  encodingIdx++)
      {
        encodingMethod = dsp.encodingMethods[encodingIdx];

        config->EncodingMethod (encodingMethod);

        if  (allFieldsAteNumeric  &&  (encodingMethod != NoEncoding))
          continue;

        bool weightBitReduction = true;   

        const int*  bitReductionTable = dsp.bitReductions;


        for  (int randomJobNum = 0;  randomJobNum < 30;  randomJobNum++)
        {
          BitReductionJobPtr  bitReductionJob 
            = new BitReductionJob (log, 
                                   nextJobId,
                                   learningAlgorithm,
                                   svmKernalType,
                                   config->SelectionMethod (),
                                   encodingMethod,
                                   BRcompressionPost,
                                   true,            // Weighted = true
                                   cParam,
                                   config->Gamma (),
                                   0,                // Bits to reduce By,
                                   0,                // UnBalBits,
                                   jobTypes,
                                   0,                // orderings
                                   int (config->AvgMumOfFeatures () + 0.5),
                                   config->NumOfFeaturesAfterEncoding ()
                                  );
          nextJobId++;
          PushOnBack (bitReductionJob);
        }
      }  /* encodingIdx */
    }  /* cIDX */
  } /* KernalIDX */
}  /* CreateTuningJobs */





void  BitReductionJobList::CreateJobs ()
{
  bool  allFieldsAteNumeric = trainData->AllFieldsAreNumeric ();

  SVM_KernalType  svmKernalType;
  int  kernalIdx = 0;
  for  (kernalIdx = 0;  (svmKernalType = dsp.kernalTypes[kernalIdx]) != KT_NULL;  kernalIdx++)
  {
    config->KernalType (svmKernalType);

    const int defaultCValues[] = {1, 3, 9, 12, 15, -1};
    const int*  cValues = defaultCValues;

    cValues = dsp.c_params;

    int cIDX = 0;

    for  (cIDX = 0; cValues[cIDX] > 0; cIDX++)
    {
      int  cParam = cValues[cIDX];

      config->C_Param (cParam);


      SVM_EncodingMethod  encodingMethod = BinaryEncoding;
      int  encodingIdx = 0;
      for  (encodingIdx = 0;  dsp.encodingMethods[encodingIdx] != Encoding_NULL;  encodingIdx++)
      {
        encodingMethod = dsp.encodingMethods[encodingIdx];

        config->EncodingMethod (encodingMethod);

        if  (allFieldsAteNumeric  &&  (encodingMethod != NoEncoding))
          continue;

        bool weightBitReduction = true;   

        int  wIDX = 0;
        for  (wIDX = 0;  wIDX < 2;  wIDX++)
        {
          if  (wIDX == 0)
          {
            weightBitReduction = false;
            if  (!dsp.unWeighted)
              continue;
          }
          else
          {
            weightBitReduction = true;
          }

          const int*  bitReductionTable = dsp.bitReductions;

          if  ((jobTypes == brjNormalJob)  ||  (jobTypes == brjSmallSample))
          {
            BitReductionJobPtr  baseJob 
              = new BitReductionJob (log, 
                                     nextJobId,
                                     learningAlgorithm,
                                     svmKernalType,
                                     config->SelectionMethod (),
                                     encodingMethod,
                                     BRnoCompression,
                                     weightBitReduction,            // Weighted = true
                                     cParam,
                                     config->Gamma (),
                                     0,               // Bits to reduce By,
                                     0,               // UnBalBits,
                                     jobTypes,
                                     0,               // ordering,  only used with jobTypes == RandomSplits
                                     int (config->AvgMumOfFeatures () + 0.5),
                                     config->NumOfFeaturesAfterEncoding ()
                                    );

            nextJobId++;

            PushOnBack (baseJob);
          }

          for  (int  bitReductionIdx = 0;  bitReductionTable[bitReductionIdx] >= 0;  bitReductionIdx++)
          {
            int  bitReduction = bitReductionTable[bitReductionIdx];
  
            BitReductionJobPtr  bitReductionJob 
              = new BitReductionJob (log, 
                                     nextJobId,
                                     learningAlgorithm,
                                     svmKernalType,
                                     config->SelectionMethod (),
                                     encodingMethod,
                                     BRcompressionPost,
                                     weightBitReduction,            // Weighted = true
                                     cParam,
                                     config->Gamma (),
                                     bitReduction,    // Bits to reduce By,
                                     0,               // UnBalBits,
                                     jobTypes,
                                     0,               // orderings
                                     int (config->AvgMumOfFeatures () + 0.5),
                                     config->NumOfFeaturesAfterEncoding ()
                                    );
            nextJobId++;
            PushOnBack (bitReductionJob);

            config->CompressionMethod (BRunBalancedVariance);
         
            if  (bitReduction >= dsp.rsBrBits)
            {
              int  numNumericFeatures = DeriveNumNumericFeatures (config);

              int  unBalBits =0;
              for  (unBalBits = 1;  unBalBits < numNumericFeatures;  unBalBits++)
              {
                BitReductionJobPtr  unBalBitsJob 
                   = new BitReductionJob (log, 
                                          nextJobId,
                                          learningAlgorithm,
                                          svmKernalType,
                                          config->SelectionMethod (),
                                          encodingMethod,
                                          BRunBalancedVariance,
                                          weightBitReduction,     // Weighted = true
                                          cParam,
                                          config->Gamma (),
                                          bitReduction,    // Bits to reduce By,
                                          unBalBits,       // UnBalBits,
                                          jobTypes,
                                          0,               // orderings
                                          int (config->AvgMumOfFeatures () + 0.5),
                                          config->NumOfFeaturesAfterEncoding ()
                                         );

                nextJobId++;
                PushOnBack (unBalBitsJob);
              }
            }
          }  /* bitReductionIdx */
        }  /* WeightedIdx */
      }  /* encodingIdx */
    }  /* cIDX */
  } /* KernalIDX */
}  /* CreateJobs */




void  BitReductionJobList::CreateRandomSplitJobs ()
{
  bool  allFieldsAteNumeric = trainData->AllFieldsAreNumeric ();

  SVM_KernalType  svmKernalType;
  int  kernalIdx = 0;
  for  (kernalIdx = 0;  (svmKernalType = dsp.kernalTypes[kernalIdx]) != KT_NULL;  kernalIdx++)
  {
    config->KernalType (svmKernalType);

    const int defaultCValues[] = {1, 3, 9, 12, 15, -1};
    const int*  cValues = defaultCValues;

    cValues = dsp.c_params;

    int cIDX = 0;

    for  (cIDX = 0; cValues[cIDX] > 0; cIDX++)
    {
      int  cParam = cValues[cIDX];

      config->C_Param (cParam);


      SVM_EncodingMethod  encodingMethod = BinaryEncoding;
      int  encodingIdx = 0;
      for  (encodingIdx = 0;  dsp.encodingMethods[encodingIdx] != Encoding_NULL;  encodingIdx++)
      {
        encodingMethod = dsp.encodingMethods[encodingIdx];

        config->EncodingMethod (encodingMethod);

        if  (allFieldsAteNumeric  &&  (encodingMethod != NoEncoding))
          continue;

        bool weightBitReduction = true;   

        const int*  bitReductionTable = dsp.bitReductions;

        for  (int  bitReductionIdx = 0;  bitReductionTable[bitReductionIdx] >= 0;  bitReductionIdx++)
        {
          int  bitReduction = bitReductionTable[bitReductionIdx];

          int  orderingIDX = 0;

          for  (orderingIDX = 0;  orderingIDX < MaxNumOfRandomSplits;  orderingIDX++)
          {
            BitReductionJobPtr  bitReductionJob 
              = new BitReductionJob (log, 
                                     nextJobId,
                                     learningAlgorithm,
                                     svmKernalType,
                                     config->SelectionMethod (),
                                     encodingMethod,
                                     BRcompressionPost,
                                     true,            // Weighted = true
                                     cParam,
                                     config->Gamma (),
                                     bitReduction,    // Bits to reduce By,
                                     0,               // UnBalBits,
                                     jobTypes,
                                     orderingIDX,     // orderings
                                     int (config->AvgMumOfFeatures () + 0.5),
                                     config->NumOfFeaturesAfterEncoding ()
                                    );
            nextJobId++;
            PushOnBack (bitReductionJob);
          }

          config->CompressionMethod (BRunBalancedVariance);
         
          if  (bitReduction >= dsp.rsBrBits)
          {
            int  numNumericFeatures = DeriveNumNumericFeatures (config);

            int  unBalBits =0;
            for  (unBalBits = 1;  unBalBits < numNumericFeatures;  unBalBits++)
            {

              for  (orderingIDX = 0;  orderingIDX < MaxNumOfRandomSplits;  orderingIDX++)
              {
                BitReductionJobPtr  unBalBitsJob 
                   = new BitReductionJob (log, 
                                          nextJobId,
                                          learningAlgorithm,
                                          svmKernalType,
                                          config->SelectionMethod (),
                                          encodingMethod,
                                          BRunBalancedVariance,
                                          true,            // Weighted = true
                                          cParam,
                                          config->Gamma (),
                                          bitReduction,    // Bits to reduce By,
                                          unBalBits,       // UnBalBits,
                                          jobTypes,
                                          orderingIDX,     // orderings
                                          int (config->AvgMumOfFeatures () + 0.5),
                                          config->NumOfFeaturesAfterEncoding ()
                                         );

                nextJobId++;
                PushOnBack (unBalBitsJob);
              }
            }
          }
        }  /* bitReductionIdx */
      }  /* encodingIdx */
    }  /* cIDX */
  } /* KernalIDX */
}  /* CreateRandomSplitJobs */










void  BitReductionJobList::Save ()
{
  KKStr  dirPath, rootName, ext;

  osParseFileName (jobFileName, dirPath, rootName, ext);

  KKStr name1 = osAddSlash (dirPath) + rootName + "_Prev1" + "." + ext;
  KKStr name2 = osAddSlash (dirPath) + rootName + "_Prev2" + "." + ext;

  osDeleteFile (name2);
  osRenameFile (name1, name2);
  osRenameFile (jobFileName, name1);

  ofstream  jobFile (jobFileName.Str ());

  if  (!jobFile.is_open ())
  {
    log.Level (-1) << endl
                   << endl
                   << "BitReductionJobList::Save    *** ERROR ***" << endl
                   << endl
                   << "Error Saving to file[" << jobFileName << "]" << endl
                   << endl;

    osWaitForEnter ();
    exit (-1);
  }

  jobFile << "// DateWritten  [" << osGetLocalDateTime () << "]" << endl
          << "//" << endl;

  jobFile << "NextJobId"        << "\t" << nextJobId        << endl;
  jobFile << "AccLossTolerance" << "\t" << accLossTolerance << endl;
          
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    jobFile << "Job" << "\t" << IdxToPtr (x)->ToTabDelStr ().QuotedStr () << endl;
  }
  
  jobFile.close ();
}  /* Save */




void  BitReductionJobList::SaveNoCompClassedCorrectly ()
{
  if  (!noCompClassedCorrectly)
    return;

  ofstream  o (noCompFileName.Str ());

  int  x;
  for  (x = 0;  x < testData->QueueSize ();  x++)
  {
    o << x << "\t" << (noCompClassedCorrectly[x] ? "Y" : "N") << endl;
  }
  o.close ();
}  /* SaveNoCompClassedCorrectly */





void  BitReductionJobList::Load ()
{
  DeleteContents ();  // Delete all existing jobs.

  FILE*  in = fopen (jobFileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "BitReductionJobList::Load    *** ERROR ***" << endl
                   << endl
                   << "     Error Opening Status File[" << jobFileName << "]" << endl
                   << endl;
    exit (-1);
  }

  char  buff[40960];

  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  ln (buff);
    if  (ln.SubStrPart (0, 1) == "//")
      continue;

    KKStr  field = ln.ExtractQuotedStr ("\t", true);
    field.Upper ();
   
    if  (field == "NEXTJOBID")
    {
      nextJobId = ln.ExtractTokenInt ("\t\n\r");
    }

    else if  (field.EqualIgnoreCase ("AccLossTolerance"))
    {
      accLossTolerance = (float)ln.ExtractTokenDouble ("\t");

    }

    else if  (field == "JOB")
    {
      BitReductionJobPtr j = new BitReductionJob (log, ln.ExtractQuotedStr ("\t", true));
      PushOnBack (j);
    }
  }

  fclose (in);
}  /* Load */




void  BitReductionJobList::LoadNoCompClassedCorrectly ()
{
  if  (!testData)
    return;

  FILE* in = fopen (noCompFileName.Str (), "r");
  if  (!in)
  {
    delete[]  noCompClassedCorrectly;
    noCompClassedCorrectly = new bool[testData->QueueSize ()];
    int x;
    for  (x = 0; x < testData->QueueSize (); x++)
      noCompClassedCorrectly[x] = false;
    return;
  }

  char  buff[10240];

  delete[]  noCompClassedCorrectly;
  noCompClassedCorrectly = new bool[testData->QueueSize ()];

  int  linesRead = 0;
  
  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  ln (buff);
    int  lineNum = ln.ExtractTokenInt ("\t\n\r");
    if  ((lineNum < 0)  ||  (lineNum >= testData->QueueSize ()))
    {
      fclose (in);
      log.Level (-1) << endl << endl << endl
                     << "LoadNoCompClassedCorrectly      *** ERROR ***    Invalid Line Num[" << lineNum << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    KKStr  nextField = ln.ExtractToken ("\t\n\r");
    nextField.Upper ();

    noCompClassedCorrectly[lineNum] = ((nextField == "YES")  ||  
                                       (nextField == "Y")    ||
                                       (nextField == "TRUE") ||
                                       (nextField == "T")
                                      );
  }

  fclose (in);
}  /* LoadNoCompClassedCorrectly */




BitReductionJobPtr  BitReductionJobList::LookUpByJobId (int  jobId)
{
  int  x;

  for  (x = 0;  x < QueueSize (); x++)
  {
    BitReductionJobPtr  j = IdxToPtr (x);
    if  (j->JobId () == jobId)
      return  j;
  }

  return  NULL;
}  /* LookUpByJobId */




BitReductionJobPtr  BitReductionJobList::LocateOpenJob ()
{
  bool  jobHasBeenFound = false;
  bool  stillJobsToRun  = true;
  BitReductionJobPtr  j = NULL;
  int   numTimesWentToSleep = 0;


  while  ((!jobHasBeenFound)  &&  (stillJobsToRun))
  {
    Block ();
    Load ();

    stillJobsToRun = false;

    int  x;
    for  (x = 0;  x < QueueSize (); x++)
    {
      j = IdxToPtr (x);
      if  (j->Status () == brOpen)
      {
        jobHasBeenFound = true;
        stillJobsToRun  = true;
        break;
      }

      else if  (j->Status () == brStarted)
      {
        if  ((j->CompMethod () == BRnoCompression)  &&  (jobTypes == brjNormalJob))
        {
           // If the job that is runnig is the first job for the paired t-test 
           // data, see  'SetNoCompFileName'  Then we are not ready to run.
           // We must wait for that job to finish.
          j = NULL;
          stillJobsToRun = true;
          jobHasBeenFound = false;
          break;
        }
      }
    }

    if  (jobHasBeenFound)
    {
      j->Status (brStarted);
      Save ();
      
      if  (jobTypes != brjTuningTest)
      {
        // Lets set up 'noCompFileName'  for current job
        SetNoCompFileName (j);
        LoadNoCompClassedCorrectly ();
      }
    }
    else
    {
      j = NULL;
    }

    EndBlock ();

    if  ((!jobHasBeenFound)  &&  (stillJobsToRun))
    {
      log.Level (10) << "BitReductionJobList::LocateOpenJob  Sleeping" << endl;

      int  secsToSleep = 30;
      if  (numTimesWentToSleep > 1)
        secsToSleep = 120;


      // We are going to sleep for a bit and try again later.
      #ifdef  WIN32
      Sleep (secsToSleep * 1000);
      #else
      sleep (secsToSleep);
      #endif

      numTimesWentToSleep++;
    }
  }

  if  (j)
    return  new BitReductionJob (*j);
  else
    return NULL;
}  /* LocateOpenJob */





bool  BitReductionJobList::JobsHaveSameSvmParms (const BitReductionJob&  j1, 
                                                 const BitReductionJob&  j2
                                                )  const
{
  if  ((j1.CParam          () != j2.CParam          ())  ||
       (j1.EncodingMethod  () != j2.EncodingMethod  ())  ||
       (j1.Gamma           () != j2.Gamma           ())  ||
       (j1.KernalType      () != j2.KernalType      ())  ||
       (j1.SelectionMethod () != j2.SelectionMethod ())  ||
       (j1.Weighted        () != j2.Weighted        ())
      )
    return false;

  return true;
}  /* JobsHaveSameSvmParms */
                                                 









VectorBitReductionJobsPtr  BitReductionJobList::LocateOpenJobs (uint  numJobsToFetch,
                                                                int&  minBitRedLevel,
                                                                int&  minUnBalLevel
                                                               )
{
  bool  jobsHaveBeenFound = false;
  bool  stillJobsToRun    = true;
  VectorBitReductionJobsPtr  jobs = NULL;

  int   numTimesWentToSleep = 0;

  while  ((!jobsHaveBeenFound)  &&  (stillJobsToRun))
  {
    BitReductionJobPtr  firstJobFound = NULL;
    Block ();

    Load ();
    
    jobs = new VectorBitReductionJobs (false);

    stillJobsToRun = false;

    int  x;
    for  (x = 0;  x < QueueSize (); x++)
    {
      BitReductionJobPtr j = IdxToPtr (x);
      if  (j->Status () == brOpen)
      {
        jobsHaveBeenFound = true;
        stillJobsToRun    = true;

        if  (!firstJobFound)
        {
          firstJobFound = j;
        }
        else
        {
          if  ((j->BitReduction () * 10000 + j->UnBalBits ()) < (minBitRedLevel * 10000 + minUnBalLevel))
          {
            // We just ran into a job hat will run longer than what we were willing to accept
            break;
          }


          if  (!JobsHaveSameSvmParms (*j, *firstJobFound))
          {
            break;
          }
        }

        jobs->PushOnBack (j);

        if  ((j->BitReduction () * 10000 + j->UnBalBits ()) > (minBitRedLevel * 10000 + minUnBalLevel))
        {
          // We just found a job that will run quicker than our current expectations, so we will now
          // update our expections.

          minBitRedLevel = j->BitReduction ();
          minUnBalLevel  = j->UnBalBits ();
        }

        if  (jobs->size () >= numJobsToFetch)
          break;
      }

      else if  (j->Status () == brStarted)
      {
        if  ((j->CompMethod () == BRnoCompression)  &&  (jobTypes == brjNormalJob))
        {
          // If the job that is runnig is the first job for the paired t-test 
          // data, see  'SetNoCompFileName'  Then we are not ready to run.
          // We must wait for that job to finish.

          if  (jobs->size () > 0)
          {
            // But there are jobs that we have already found prior to this job,
            // then we can go and run them.
            break;
          }

          j = NULL;
          stillJobsToRun = true;
          jobsHaveBeenFound = false;
          break;
        }
      }
    }  /* end of for (x) */

    if  (jobsHaveBeenFound)
    {
      // Flag selected jobs as started.

      BitReductionJobList::iterator  idx;
      for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
      {
        BitReductionJobPtr  j = *idx;
        j->Status (brStarted);
      }

      Save ();
      // Lets set up 'noCompFileName'  for current job
      SetNoCompFileName (firstJobFound);
      LoadNoCompClassedCorrectly ();
    }
    else
    {
      delete  jobs;  jobs = NULL;
    }

    EndBlock ();

    if  ((!jobsHaveBeenFound)  &&  (stillJobsToRun))
    {
      log.Level (10) << "BitReductionJobList::LocateOpenJob  Sleeping" << endl;

      int  secsToSleep = 30;
      if  (numTimesWentToSleep > 1)
        secsToSleep = 120;


      // We are going to sleep for a bit and try again later.
      #ifdef  WIN32
      Sleep (secsToSleep * 1000);
      #else
      sleep (secsToSleep);
      #endif

      numTimesWentToSleep++;
    }
  }

  VectorBitReductionJobsPtr  jobsToRun = NULL;
  if  (jobs)
  {
    jobsToRun = new VectorBitReductionJobs (true);
    VectorBitReductionJobs::iterator  idx;
    for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
    {
      BitReductionJobPtr j = *idx;
      jobsToRun->PushOnBack (new BitReductionJob (*j));
    }

    delete  jobs;  jobs = NULL;
  }

  return  jobsToRun;
}  /* LocateOpenJobs */







bool  BitReductionJobList::AreAllJobsAreDone ()
{
  int  x;
  for  (x = 0;  x < QueueSize (); x++)
  {
    BitReductionJobPtr  j = IdxToPtr (x);
    if  (j->Status () != brDone)
      return false;
  }

  return  true;
}  /* AreAllJobsAreDone */





void  BitReductionJobList::ProcessJobQueue (ostream& r)
{
  BitReductionJob::ReportResultsHeader (r);

  
  // Get the next Job to run,  we will own the object returned, so we 
  // will need to delete it when we are done with it.
  BitReductionJobPtr  job = LocateOpenJob ();
  while  (job)
  {
    job->EvaluteNode (config, 
                      trainDataFileName, 
                      trainData,
                      testData,
                      mlClasses,
                      noCompClassedCorrectly,
                      classedCorrectly,
                      r
                     );

    Block ();

    Load ();
    
    BitReductionJobPtr origJob = LookUpByJobId (job->JobId ());
    if  (!origJob)
    {
      EndBlock ();

      r << endl << endl << endl << endl
        << "BitReductionJobList::ProcessJobQueue    *** ERROR ***" << endl
        << endl
        << "     Could not locate Original Job[" << job->JobId () << "]" << endl
        << endl;

      log.Level (-1) << endl << endl << endl << endl
                     << "BitReductionJobList::ProcessJobQueue    *** ERROR ***" << endl
                     << endl
                     << "     Could not locate Original Job[" << job->JobId () << "]" << endl
                     << endl;
      exit (-1);
    }

    *origJob = *job;
    origJob->Status (brDone);
    
    Save ();

    if  (job->CompMethod () == BRnoCompression)
      SaveNoCompClassedCorrectly ();

    job->ReportResults (r, NULL);
    r.flush ();

    EndBlock ();


    delete  job;  job = NULL;

    job = LocateOpenJob ();
  }

  if  (AreAllJobsAreDone ())
  {
    r << endl << endl << endl << endl << endl
      << "Summary of Job Runs for dataset[" << dsp.name << "]" << endl 
      << endl;

    BitReductionJob::ReportResultsHeader (r);

    // Lets print out summary of all jobs
    BitReductionJobList::const_iterator  idx;

    BitReductionJobPtr  unCompressedJob = NULL;
  
    for  (idx = begin ();  idx != end ();  idx++)
    {
      BitReductionJobPtr  job = *idx;
     
      if  (job->CompMethod () == BRnoCompression)
      {
        // This is a job that all other jobs that follow must be compared with
        unCompressedJob = job;
      }

      job->ReportResults (r, unCompressedJob);
    }

    r.flush ();
  }
}  /* ProcessJobQueue */




void  BitReductionJobList::SaveCompressedData  (BitReductionJobPtr    j,
                                                FeatureVectorListPtr  normTrainData,
                                                FeatureVectorListPtr  normTestData
                                               )
{
  bool  cancelFlag = false;
  bool  successful = false;

  FeatureFileIOPtr  c45Driver = FeatureFileIOC45::Driver ();

  KKStr  fileSystemName = osRemoveExtension (trainDataFileName) + 
                           "_Compressed-" + 
                           StrFormatInt (j->BitReduction (), "0") + "-" +
                           StrFormatInt (j->UnBalBits    (), "0");

  KKStr  compressedTrainFileName = fileSystemName + ".data";
  KKStr  compressedTestFileName  = fileSystemName + ".test";
  KKStr  compressedWeightsName   = fileSystemName + ".weights";

  SVMparam  param (config->SVMparamREF ());
  param.CompressionMethod (BRunBalancedSpecified);
  param.BitsToReduceBy (j->BitReduction ());
  param.UnBalancedBits (j->UnBalBits    ());

  ClassAssignments  classAssignments (*mlClasses, log);


  {
    FeatureVectorListPtr  compTrainExamples = new FeatureVectorList (fileDesc, true, log, 1000);
    BitReduction br (param, fileDesc, normTrainData->AllFeatures ());
    br.compress (*normTrainData, compTrainExamples, classAssignments);
    uint  numExamplesWritten = 0;
    c45Driver->SaveFeatureFile (compressedTrainFileName, compTrainExamples->AllFeatures (), *compTrainExamples, numExamplesWritten, cancelFlag, successful, log);

    {
      ofstream weightFile (compressedWeightsName.Str ());
      FeatureVectorList::iterator  idx;
      for  (idx = compTrainExamples->begin ();  idx != compTrainExamples->end ();  idx++)
      {
        FeatureVectorPtr  i = *idx;
        weightFile << i->TrainWeight () << endl;
      } 
      weightFile.close ();
    }

    delete  compTrainExamples;  compTrainExamples = NULL;
  }

  {
    uint  numExamplesWritten = 0;
    c45Driver->SaveFeatureFile (compressedTestFileName, normTestData->AllFeatures (), *normTestData, numExamplesWritten, cancelFlag, successful, log);
  }


}  /* SaveCompressedData */




bool  BitRedJobMediumPredicate (BitReductionJobPtr j1,
                                BitReductionJobPtr j2
                               )
{
  if  (j1->BitReduction () < j2->BitReduction ())
    return true;

  else if  (j1->BitReduction () > j2->BitReduction ())
    return false;

  if  (j1->UnBalBits () < j2->UnBalBits ())
    return true;

  else if  (j1->UnBalBits () > j2->UnBalBits ())
    return false;

  return  j1->Accuracy () < j2->Accuracy ();
}  /* BitRedJobMediumPredicate */





void  BitReductionJobList::ProcessTuningJobQueue (ostream& r)
{
  BitReductionJob::ReportResultsHeader (r);

  
  // Get the next Job to run,  we will own the object returned, so we 
  // will need to delete it when we are done with it.
  BitReductionJobPtr  job = LocateOpenJob ();
  while  (job)
  {
    job->RunTuningExample1 (config, trainData, testData, mlClasses, samplePercentage, r, accLossTolerance);

    Block ();

    Load ();
    
    BitReductionJobPtr origJob = LookUpByJobId (job->JobId ());
    if  (!origJob)
    {
      EndBlock ();

      r << endl << endl << endl << endl
        << "ProcessTuningJobQueue    *** ERROR ***" << endl
        << endl
        << "     Could not locate Original Job[" << job->JobId () << "]" << endl
        << endl;

      log.Level (-1) << endl << endl << endl << endl
                     << "BitReductionJobList::ProcessTuningJobQueue    *** ERROR ***" << endl
                     << endl
                     << "     Could not locate Original Job[" << job->JobId () << "]" << endl
                     << endl;
      exit (-1);
    }

    *origJob = *job;
    origJob->Status (brDone);
    
    Save ();

    job->ReportTuningResults (r);
    r.flush ();

    EndBlock ();


    delete  job;  job = NULL;

    job = LocateOpenJob ();
  }


  if  (AreAllJobsAreDone ())
  {
    r << endl << endl << endl << endl << endl
      << "Summary of Job Runs for dataset[" << dsp.name << "]" << endl 
      << endl;

    BitReductionJob::ReportTuningResultsHeader (r);

    BitReductionJobList::const_iterator  idx;
    idx = begin ();
    BitReductionJobPtr  job = NULL;
    if  (idx != end ())
      job = *idx;

    while  (job)
    {
      BitReductionJobPtr  subTotal = new BitReductionJob (*job);
      int  subTotalCount = 0;
      subTotal->ZeroOut ();

      vector<BitReductionJobPtr>  jobsInSubTotal; 

      config->KernalType      (subTotal->KernalType      ());
      config->SelectionMethod (subTotal->SelectionMethod ());
      config->EncodingMethod  (subTotal->EncodingMethod  ());

      FeatureVectorListPtr  normTrainData = trainData->DuplicateListAndContents ();
      FeatureVectorListPtr  normTestData  = testData->DuplicateListAndContents ();
      NormalizationParms (config, *normTrainData, log);
      NormalizationParms (config, *normTestData,  log);

      while  ((idx != end ())             &&  
              (subTotal->KernalType      () == job->KernalType      ())  &&
              (subTotal->SelectionMethod () == job->SelectionMethod ())  &&
              (subTotal->EncodingMethod  () == job->EncodingMethod  ())  &&
              (subTotal->Weighted        () == job->Weighted        ())  &&
              (subTotal->CParam          () == job->CParam          ())  &&
              (subTotal->Gamma           () == job->Gamma           ())
             )
      {
        job->ReportTuningResults (r);
        r.flush ();

        (*subTotal) += (*job);

        jobsInSubTotal.push_back (job);
        subTotalCount++;

        idx++;
        if  (idx != end ())
          job = *idx;
        else
          job = NULL;
      }

      sort (jobsInSubTotal.begin (), jobsInSubTotal.end (), BitRedJobMediumPredicate);
      BitReductionJobPtr  mediumJob = jobsInSubTotal[(int)(subTotalCount / 2)];

      (*subTotal) /= (float)subTotalCount;
   
      r << endl
        << "SubTotal" << endl;
      subTotal->ReportTuningResults (r);
      r << endl 
        << endl;

      r.flush ();

      //SaveCompressedData (mediumJob, normTrainData, normTestData);

      {
        // Run a Pure SVM to get base stats
        config->BitsToReduceBy (0);
        config->KernalType         (mediumJob->KernalType     ());
        config->EncodingMethod     (mediumJob->EncodingMethod ());
        config->CompressionMethod  (BRnoCompression);
        config->WeightBitReduction (true);
        config->C_Param            (mediumJob->CParam ());
        config->Gamma              (mediumJob->Gamma ());


        float                 testAccuracy = 0.0f;
        double                timeToTrain = 0.0;
        double                timeToTest = 0.0;
        CompressionStats      compressionStats;
        double                numSupportVectors;

        BitReductionJob::TestASpecificConfiguraion  (learningAlgorithm,
                                                     config,
                                                     false,      // false = data is NOT already normalized.
                                                     trainData,
                                                     testData,
                                                     testAccuracy,
                                                     timeToTrain,
                                                     timeToTest,
                                                     NULL,
                                                     compressionStats,
                                                     numSupportVectors
                                                    );

        double  totalTuneAndTrainTime = subTotal->TotalTuningTime () + subTotal->TrainTime ();
        float  accLoss             = 100.0f * (testAccuracy - subTotal->Accuracy ()) / testAccuracy;
        double trainTuneSpeadUp    = timeToTrain / totalTuneAndTrainTime;
        double trainNoTuneSpeadUp  = timeToTrain / subTotal->TrainTime ();
        double testSpeedUp         = timeToTest  / subTotal->TestTime  ();

        KKStr  bitRedParms (20);
        bitRedParms << mediumJob->BitReduction () << "-" << mediumJob->UnBalBits ();


        r << endl << endl;
        r << "Train DataSet Size"  << "\t" << trainData->QueueSize    ()          << endl;
        r << "Test  DataSet Size"  << "\t" << testData->QueueSize     ()          << endl;
        r << "Sample Percentage"   << "\t" << samplePercentage                    << endl;
        r << "Sample Train Size"   << "\t" << subTotal->SampledTrainingSetSize () << endl;
        r << "Number Classes"      << "\t" << mlClasses->QueueSize ()          << endl;
        r << "Number Features"     << "\t" << trainData->FeatureCount ()          << endl;
        r << endl << endl;

        r << ""        << "\t" << ""       << "\t" << ""       << "\t" << ""      << "\t" << ""      << "\t" << ""         << "\t" << "Tune"    << "\t" << "NoTune"  << "\t" << ""        << "\t" << ""         << endl;
        r << ""        << "\t" << "Tuning" << "\t" << "BitRed" << "\t" << "Train" << "\t" << "Test"  << "\t" << "Test"     << "\t" << "Train"   << "\t" << "Train"   << "\t" << "Test"    << "\t" << "Accurcay" << endl;
        r << "Desc"    << "\t" << "Time"   << "\t" << "Parms"  << "\t" << "Time"  << "\t" << "Time"  << "\t" << "Accuracy" << "\t" << "SpeedUp" << "\t" << "SpeedUp" << "\t" << "SpeedUp" << "\t" << "Loss"     << endl;

        r << "PureSvm"       << "\t" 
          << ""              << "\t"   // Tuning Time
          << "--"            << "\t"   // Bit Red Parms
          << timeToTrain     << "\t"
          << timeToTest      << "\t"
          << testAccuracy    << "%" << "\t"
          << endl;

        r << "UBRSVM"                       << "\t"
          << subTotal->TotalTuningTime ()   << "\t"
          << "'" << bitRedParms             << "\t"
          << subTotal->TrainTime ()         << "\t"
          << subTotal->TestTime  ()         << "\t"
          << subTotal->Accuracy  () << "%"  << "\t"
          << trainTuneSpeadUp               << "\t"
          << trainNoTuneSpeadUp             << "\t"
          << testSpeedUp                    << "\t"
          << accLoss << "%"
          << endl;

        r.flush ();
      }

      delete  normTestData;   normTestData  = NULL;
      delete  normTrainData;  normTrainData = NULL;
      delete  subTotal;       subTotal = NULL;
    }


    r.flush ();
  }
}  /* ProcessTuningJobQueue */







void  BitReductionJobList::ProcessRandomSplitsJobQueue (ostream& r)
{
  Block ();
  FeatureVectorList  allData (*trainData, false);
  allData.AddQueue (*testData);
  Orderings  splitOrderings (&allData, splitOrderigsFileName, MaxNumOfRandomSplits, 1);
  EndBlock ();

  uint  numJobsAtATime = 1;
  int   minBitRed = 0;
  int   minUnBalBits = 0;

  MLClassListPtr  mlClasses = NULL;

  // Get the next Job to run,  we will own the object returned, so we 
  // will need to delete it when we are done with it.
  VectorBitReductionJobsPtr  jobs = LocateOpenJobs (numJobsAtATime, minBitRed, minUnBalBits);
  while  (jobs)
  {
    mlClasses = new MLClassList (*splitOrderings.MLClasses ());

    VectorBitReductionJobs::iterator  idx;

    double  batchStartTime = osGetSystemTimeUsed ();
    
    for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
    {
      BitReductionJobPtr  job = *idx;
      job-> EvaluateRandomSplit (config, 
                                 splitOrderings,
                                 *mlClasses,
                                 r
                                );
    }

    double  batchEndTime = osGetSystemTimeUsed ();
    double  batchTotalTime = batchEndTime - batchStartTime;

    // Now that we know how long it took to process the last bactch of jobs we want to 
    // adjust 'numJobsAtATime' such that the next bactch takes approx. 60 seconds.

    numJobsAtATime = int (0.5 + (double (jobs->size ()) * 60.0) / batchTotalTime);
    if  (numJobsAtATime < 1)
      numJobsAtATime = 1;

    else if  (numJobsAtATime > 10)
      numJobsAtATime = 10;


    Block ();
    Load ();
    
    for  (idx = jobs->begin ();  idx != jobs->end ();  idx++)
    {
      BitReductionJobPtr  job = *idx;
      BitReductionJobPtr origJob = LookUpByJobId (job->JobId ());

      if  (!origJob)
      {
        EndBlock ();
        r << endl << endl << endl << endl
          << "BitReductionJobList::ProcessRandomSplitsJobQueue    *** ERROR ***" << endl
          << endl
          << "     Could not locate Original Job[" << job->JobId () << "]" << endl
          << endl;

          log.Level (-1) << endl << endl << endl << endl
                         << "BitReductionJobList::ProcessRandomSplitsJobQueue    *** ERROR ***" << endl
                         << endl
                         << "     Could not locate Original Job[" << job->JobId () << "]" << endl
                         << endl;
          exit (-1);
      }
 
      *origJob = *job;
      origJob->Status (brDone);
      if  (job->CompMethod () == BRnoCompression)
        SaveNoCompClassedCorrectly ();

      job->ReportResults (r, NULL);
      r.flush ();
    }
    
    Save ();

    EndBlock ();

    delete  jobs;  jobs = NULL;

    delete  mlClasses;  mlClasses = NULL;

    jobs = LocateOpenJobs (numJobsAtATime, minBitRed, minUnBalBits);
  }

  if  (AreAllJobsAreDone ())
  {
    r << endl << endl << endl << endl << endl
      << "Summary of Job Runs for dataset[" << dsp.name << "]" << endl 
      << endl;

    ReportRandomSplits (r);

    r.flush ();
  }
}  /* ProcessRandomSplitsJobQueue */





void  BitReductionJobList::Block ()
{
  log.Level (20) << "BitReductionJobList::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName.Str (), O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (10) << "BitReductionJobList::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "BitReductionJobList::Block - Lock is Established." << endl;
} /* Block */



void  BitReductionJobList::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "BitReductionJobList::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "BitReductionJobList::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "BitReductionJobList::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName.Str ()))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "BitReductionJobList::EndBlock - Error["  << lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName.Str ());
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */




void  BitReductionJobList::ReportRandomSplits (ostream& r)
{
  BitReductionJob::ReportResultsHeader (r);

  BitReductionJobPtr  next = NULL;
  iterator  idx = begin ();

  int  count;

  while (idx != end ())
  {
    next = *idx;
    BitReductionJob  subJob = *next;

    count = 1;

    idx++;
   
    while  (idx!= end ())
    {
      next = *idx;

      if  ((next->KernalType      () != subJob.KernalType      ()) ||
           (next->SelectionMethod () != subJob.SelectionMethod ()) ||
           (next->EncodingMethod  () != subJob.EncodingMethod  ()) ||
           (next->CompMethod      () != subJob.CompMethod      ()) ||
           (next->Weighted        () != subJob.Weighted        ()) ||
           (next->CParam          () != subJob.CParam          ()) ||
           (next->Gamma           () != subJob.Gamma           ()) ||
           (next->BitReduction    () != subJob.BitReduction    ()) ||
           (next->UnBalBits       () != subJob.UnBalBits       ())
          )
      {
        break;
      }

      subJob += (*next);
      count++;
      
      idx++;
    }

    subJob /= float (count);
    subJob.ReportResults (r, NULL);
    r.flush ();
  }


}  /* ReportRandomSplits */

