#include "FirstIncludes.h"

#ifdef  WIN32
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#else
  #include <limits.h>
  #include <stdlib.h>
#endif

#include <stdio.h>

#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


#ifdef  WIN32
#include <ostream>
#endif

#include "MemoryDebug.h"
#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

using namespace std;


#include "BasicTypes.h"
#include "HTMLReport.h"
#include "OSservices.h"
#include "RunLog.h"
#include "StatisticalFunctions.h"
using namespace KKU;


#include "Instrument.h"
#include "InstrumentDataList.h"
#include "InstrumentDataFileManager.h"
using namespace SipperHardware;


#include "BinaryClassParms.h"
#include "ConfusionMatrix2.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "NormalizationParms.h"
using namespace MLL;


#include "AccByFeatureSel.h"
#include "BinaryClass.h"
#include "BinaryJob.h"
#include "FeatureImpact.h"
#include "FinalResults.h"
#include "FeatureSelection.h"
#include "Processor.h"

#include "FeatureSelection.h"

// C:\users\kkramer\GradSchool\Thesis\TestSet9Classes\300_ImagesPerClass\UsingOld5ContourFeatures\BinaryFeatureSearch2
// -C Thesis9Classes.cfg  -DataFile NineClasses_TestData.data  

// K:\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinaryFeatureSearch2
//  -C Thesis9Classes.cfg  -DataFile NineClasses_TestData.data  

// 2005-04-19   Feature selction run with PunishmentFactor = 0.01
// K:\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures\BinarFeatureSearch_Using_01-Punishment
//  -C Thesis9Classes.cfg  -DataFile NineClasses_TestData.data  

// -C AllTrainingImages_Trimmed.cfg  -DataFile AllTrainingImages_Trimmed.data

// Feature Selection for Andrews latest Training set 2005-12-09
// K:\v1\Plankton\FeatureSelections\2005-12-09
// -C Spain.cfg  -DataFile TrainingImages_2005-12-09.data  -BeamSize 5


// C:\Temp\FeatureSearch_BinaryClasses
// -c etpB_training.cfg  -DataFile C:\Temp\FeatureSearch_BinaryClasses\etpB_training.data  -BeamSize 4  -RESULTCONFIGFILENAME etpB_BinaryTuned.cfg
// -c etpC_training.cfg  -DataFile etpC_training.data  -BeamSize 1  

// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\Plankton\
// -BYALLCLASSES  -c etpC_training.cfg  -DataFile etpC_training_shuffled.data  -BeamSize 1
// -BYALLCLASSES  -c Simple.cfg  -DataFile Simple.data -BeamSize 1  -numJobsAtATime 1   -ProcPerCombo 9999
// -BYALLCLASSES  -c Simple.cfg  -DataFile Simple.data -BeamSize 1  -numJobsAtATime 1   -ProcPerCombo 9999


// D:\Users\kkramer\GradSchool\Plankton\Papers\BinaryFeatureSelection\FeatureSelectionExperiments\etpC_training
// -c etpC_training_Tuned.cfg  -TrainData etpC_training_shuffled.data  -folds 5  -BeamSize 1  -numJobsAtATime 1  -ProcPerCombo 10

// C:\Users\kkramer\GradSchool\Plankton\Papers\BinaryFeatureSelection\Experiments\ETP-c
// -c ETP-c.cfg  -TrainData ETP-c_Training.data  -TestData ETP-c_Test.data   -folds 5  -BeamSize 1  -numJobsAtATime 1  -ProcPerCombo 64

// -c Vic.cfg  -TrainData svm_d7_127_30_shuffled.data -format sparse  -folds 5 -BeamSize 1  -numJobsAtATime 1  -ProcPerCombo 64

// D:\Users\kkramer\GradSchool\Plankton\Papers\BinaryFeatureSelection\Experiments\Candamo
// -c  -TrainData Street_Train.data  -TestData Street_Test.data -folds 5 -format c45  -numJobsAtATime 1  -ProcPerCombo 64  


// C:\Users\kkramer\Plankton\Papers\BinaryFeatureSelection\Experiments\Candamo
// C:\Users\kkramer2\GradSchool\Papers\BinaryFeatureSelection\Experiments\Candamo
// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\Candamo
// -c Street.cfg   -TrainData Street_Train.data  -ValidationData Street_Test.data -folds 5 -format c45  -numJobsAtATime 1  -ProcPerCombo 64  -ParamSelCriteria  ma
// -FinalReport


// C:\Users\kkramer\Plankton\Papers\BinaryFeatureSelection\Experiments\WFS
// -c WFS  -TrainData Wfs_Train.data  -TestData Wfs_Test.data -ValidationData Wfs_Validation.data   -folds 5 -numJobsAtATime 1  -ProcPerCombo 64  


// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\Candamo
// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Candamo
// -Config Street.cfg  -TrainingData Street_Train.data  -ValidationData Street_Test.data  -Format C45 -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64 
// -Config Street.cfg  -TrainingData Street_Train.data  -ValidationData Street_Test.data  -Format C45 -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64 -featureCriteria  BestTestSet  -GradingMethod AccuracyNorm
// -GenerateFinalReports


//C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Station1_100Min
// -Config  Station1_100Min.cfg  -TrainingData  Station1_100Min_Train.data  -ValidationData Station1_100Min_Validation.data  -Folds 2 -NumJobsAtATime  1  -ProcPerCombo 64 -ParamSelCriteria MostAccurate

// C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\Candamo
// -c Street.cfg   -TrainData Street_Train.data  -ValidationData Street_Test.data -folds 5 -format c45  -numJobsAtATime 1  -ProcPerCombo 64  

// BinaryClassLine from, FeatureSelection.Status
// Run.0.a.log
// majorstep  ALLCLASSESFEATURESELECTION
// BinaryClass	AllClasses	Status	NotStarted	NumProcessors	0

// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Candamo
// -Config Street.cfg  -TrainingData Street_Train.data  -ValidationData Street_Test.data  -Format C45 -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64 -featureCriteria  BestTestSet  -GradingMethod AccuracyNorm  -FCPS  -MTE 50

//C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\Yuhua
// -c Yuhua.cfg  -TrainData Yuhua_Train.data    -folds 5 -numJobsAtATime 1  -ProcPerCombo 64  
//  -config Yuhua.cfg -TrainingData Yuhua_Train.data   -Format Sparse -folds 5  -NumJobsAtATime 1  -ProcPerCombo 64 -ParamSelCriteria MostAccurate

// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Yuhua

// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\Candamo
// C:\Pices\DataFiles\FeatureSelection\ThreeClasses
// -c ThreeClasses.cfg   -TrainData ThreeClasses_Train.data  -ValidationData ThreeClasses_Test.data -folds 5 -format Pices  -numJobsAtATime 1  -ProcPerCombo 64  -ParamSelCriteria MostAccurate   -RandomSplitsNum 0  -EL 5

// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Stereologer
// -c Cell.cfg  -TrainData CellFeatures_Stratified.data  -folds 5 -format Sparse  -numJobsAtATime 1  -ProcPerCombo 64  

// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\Gulf_2011\Merge3Best
// -Config ../Gulf_2011.cfg  -TrainingData ../Gulf_2011_Train.data  -TestData ../Gulf_2011_Test.data -ValidationData ../Gulf_2011_Validation.data  -Format pices -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64  -MinTrainExamples 50 -FeatureCriteria Merge3Best  -GradingMethod AccuracyNorm 


// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\GulfOilBroad2_Discreate\UsfCasCor
// -Config GulfOilBroad2_Discreate_UsfCasCor.cfg  -TrainingData ../GulfOilBroad2_Discreate_Train.data  -ValidationData ../GulfOilBroad2_Discreate_Validation.data  -Format pices -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64  -MinTrainExamples 100 -FeatureCriteria Merge3Best  -GradingMethod AccuracyNorm 


// E:\Pices\DataFiles\FeatureSelection\KK01
// -Config PGA_PL30-38_MostlyClear.cfg  -TrainingData ./PGA_PL30-38_MostlyClear.data  -Format pices -NumJobsAtATime 1  -ParamSelCriteria MostAccurate   -ProcPerCombo 64  -MinTrainExamples 3 -FeatureCriteria Merge3Best  -GradingMethod AccuracyNorm 



FeatureSelection::FeatureSelection (int     argc,
                                    char**  argv
                                   ):
  
  addingAClass           (NULL),
  alreadyNormalized      (false),
  beamSize               (1),
  byBinaryClasses        (false),             // We would normally start with All classes parameter search
  cancelFlag             (false),  
  config                 (NULL),
  driver                 (FeatureFileIOPices::Driver ()),
  expansionLimit         (50),
  featureCriteria        (fcBestTestSet),
  featureCountPrefSmall  (true),
  fileDesc               (NULL),
  gradingMethod          (gmAccuracy),
  hostName               (""),
  mlClasses           (NULL),
  initialFeatures        (NULL),
  initialFeaturesStr     (),
  justGettingStats       (false),
  lockFile               (-1),
  log                    (),
  majorStep              (msMfsParmTuningPre),  //This is the default MajorStep,  works with 'byBinaryClasses' and 'searchType'
  minTrainExamples       (1),
  nextProcessorId        (0),
  normalizationParms     (NULL),
  numJobsAtATime         (-1),
  numOfFolds             (5),
  numProcPerCombo        (-1),
  paramSelCriteria       (jscMostAccurate),
  positiveClass          (NULL),
  processingOrder        (poStandard),
  randomSplits           (NULL),
  randomSplitsNum        (30),
  randomSplitsIndexName  (),
  quitRunning            (false),
  searchType             (stParameterSearch),   // We would normally start with All classes parameter search
  testData               (NULL),
  testDataFileName       (),
  trainingData           (NULL),
  trainingDataFileName   (),
  trainingTestData       (NULL),
  validationData         (NULL),
  validationDataFileName ()

{
  //log.SetLevel (20);
  log.Level (10) << "FeatureSelection::FeatureSelection" << endl
                 << endl
                 << "Current Directory[" << osGetCurrentDirectory () << "]" << endl
                 << endl;

  procId = osGetProcessId ();
  hostName = osGetHostName ();
  log.Level (10) << "FeatureSelection::FeatureSelection   HostName[" << hostName << "]  ProcessId[" << procId << "]" << endl;

  lockFileName    = "FeatureSelection.Lock";
  statusFileName  = "FeatureSelection.Status";
  mlClasses    = new MLClassConstList ();
  ProcessStatusFile (argc, argv);
}





FeatureSelection::~FeatureSelection ()
{
  delete  config;
  delete  initialFeatures;
  delete  mlClasses;
  delete  normalizationParms;
  delete  randomSplits;
  delete  trainingData;
  delete  trainingTestData;
  delete  testData;
  delete  validationData;
}


FeatureVectorListPtr  FeatureSelection::ValidationData ()
{
  if  (!validationData)
    LoadValidationData ();
  return validationData;
}



/**
 * Will try to load a Random Splits of the combined Train, Test, and Validation data from an existing Orderings file
 * if none exists will create one that can be used by otrher instancs or future reruns.
 */
void  FeatureSelection::RandomSplitsBuild ()
{
  if  (randomSplits)
    delete  randomSplits;
  randomSplits = NULL;

  if  (randomSplitsNum < 1)
    return;

  bool  cancelFlag = false;
  bool  successful = true;

  FeatureVectorList  combinedData (fileDesc, false, log);
  if  (trainingData)
    combinedData.AddQueue (*trainingData);

  if  (testData)
    combinedData.AddQueue (*testData);

  if  (!validationData)
  {
    // Maybe because of poor program design I ave not loaded it yet;  so lets try loading it.
    LoadValidationData ();
  }

  if  (validationData)
    combinedData.AddQueue (*validationData);

  randomSplitsIndexName = osGetRootName (configFileNameSpecified) + "_RandomSplits" + StrFormatInt (randomSplitsNum, "000") + ".idx";
  randomSplits = new Orderings (&combinedData, randomSplitsIndexName, randomSplitsNum, 10);

  return;
}  /* RandomSplitsBuild */




FeatureVectorListPtr  FeatureSelection::RandomSplit (int splitNum)
{
  if  (!randomSplits)
  {
    RandomSplitsBuild ();
  }


  if  (!randomSplits)
  {
    log.Level (-1) << endl
                   << "FeatureSelection::RandomSplit   ***ERROR***    'randomSplits'  is not defined."  << endl
                   << endl;
    return  NULL;
  }

  if  ((splitNum < 0)  ||  (splitNum >= randomSplitsNum))
  {
    log.Level (-1) << endl
                   << "FeatureSelection::RandomSplit   ***ERROR***    SplitNum[" << splitNum << "]  is out of range." << endl
                   << endl;
    return  NULL;
  }

  return  randomSplits->Ordering (splitNum);
}  /* RandomSplit */





const FeatureNumList&  FeatureSelection::InitialFeatures ()  const
{
  if  (initialFeatures == NULL)
  {
    // This should never ever happen;  but if it does I am going to create an instance
    // using the Feature List form the config file.

    cerr << endl << endl << endl
         << "FeatureSelection::InitialFeatures         ***WARNING***" << endl
         << endl
         << "                                  InitialFeatures is set to 'NULL'." << endl
         << endl;

    FeatureNumListPtr  tempInitialFeatures = new FeatureNumList (config->GetFeatureNums ());
    return  *tempInitialFeatures;
  }

  return *initialFeatures;
}  /* InitialFeatures */





bool  AllBinaryClassesAreDone (BinaryClassListPtr  binaryClasses)
{
  BinaryClassList::iterator  idx;
  for  (idx = binaryClasses->begin ();  idx != binaryClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;
    if  ((bc->Status () != cpDone)  ||  (bc->NumProcessors () > 0))
      return false;
  }

  return  true;
}  /* AllBinaryClassesAreDone */




void  FeatureSelection::SetMajorStep (MajorSteps  step)
{
  log.Level (10) << "FeatureSelection::SetMajorStep [" << MajorStepToStr (step) << "]" << endl;

  majorStep = step;


  switch  (step)
  {
  case  msMfsParmTuningPre:
    byBinaryClasses = false;
    searchType      = stParameterSearch;
    break;


  case  msMfsFeatureSelection:
    byBinaryClasses = false;
    searchType      = stFeatureSelectionSearch;
    break;


  case  msMfsParmTuningPost:
    byBinaryClasses = false;
    searchType      = stParameterSearch;
    break;


  case  msBfsParmTuningPre:
    byBinaryClasses = true;
    searchType      = stParameterSearch;
    break;


  case  msBfsFeatureSelection:
    byBinaryClasses = true;
    searchType      = stFeatureSelectionSearch;
    break;


  case  msBfsParmTuningPost:
    byBinaryClasses = true;
    searchType      = stParameterSearch;
    break;


  case  msGenerateFinalResults:
    byBinaryClasses = false;
    searchType      = stNULL;
    break;


  case  msGenerateBruitSvmSearch:
    byBinaryClasses = false;
    searchType      = stParameterSearch;
    break;


  case  msDone:
    break;

  default:
    {
      log.Level (-1) << endl 
                     << "FeatureSelection::SetMajorStep          ***ERROR***           Invalid MajorStep"  << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

  }
}  /* SetMajorStep */





void  FeatureSelection::Block ()
{
  log.Level (20) << "FeatureSelection::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName.Str (), O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (10) << "FeatureSelection::Block - We are locked out[" << count << "]."  << endl;
      osSleep (2.0f + ((procId + rand ()) % 10 ));
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "FeatureSelection::Block - Lock is Established." << endl;
}



void  FeatureSelection::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "FeatureSelection::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "FeatureSelection::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "FeatureSelection::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName.Str ()))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "FeatureSelection::EndBlock - Error["  << (int32)lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName.Str ());
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */




void  FeatureSelection::DisplayCommandUsage ()
{
  cout << "-AddAClass         <Class Name that is being added>"                           << endl
       << "-BeamSize          <Beam Size,  defaults to 1>"                                << endl
       << "-Config            <Configuration File>"                                       << endl
       << "-TrainingData      <Training data feature file>"                               << endl
       << "-TestData          <Test data feature file>"                                   << endl
       << "-ValidationData    <Validation data feature file>"                             << endl
       << "-ExpansionLimit    <Num of Expansions  before switch to Beam Search>"          << endl
       << "-Format            <" << FeatureFileIO::FileFormatsReadOptionsStr () << ">"    << endl
       << "-Folds             <Num of Folds, defaults to 5>"                              << endl
       << "-MinTrainExamples  <Minimum num Training examples for BFS; otherwise all"      << endl
       << "-NumJobsAtATime    <Num Jobs that a processor can grab from the queue>"        << endl
       << "-ProcPerCombo      <Num processors per binary combo>"                          << endl
       << "-RandomSplitsNum   <Number of Random Splits,  0=none>"                         << endl
       << "-Results           <File Name To Save Results in>"                             << endl
       << "-InitialFeatures   <Initial Features to search down from>"                     << endl
       << "-GradingMethod     <Accuracy | AccuracyNorm | F-Measure>"                      << endl
       << "-ParamSelCriteria  <MostAccurate | FastestFromBest>"                           << endl
       << "-FeatureCriteria   <Merge2Best | Merge3Best | Merge4Best | Merge5Best | "      << endl
       << "                    Merge6Best | Merge7Best | BestTestSet | RemoveHurtful>"    << endl
       << "-ProcessingOrder   <Standard, Hall>"                                           << endl
       << endl
       << "-FeatureImpactSummary"                                                         << endl
       << "        - Will load all feature selection status files and generate a tab-del" << endl
       << "          report indicating Accuracy Gain by Feature for all binary class"     << endl
       << "          pairs."                                                              << endl
       << endl
       << "-GeneratFinalReports"                                                          << endl
       << "        - Configures to generate the finalreports.  You would use this"        << endl
       << "          if the Search Process is already completed and you want to re-run"   << endl
       << "          the Final Reports"                                                   << endl
       << endl
       << "-GenerateBruitSvmSearch"                                                       << endl
       << "        - Configures to generate a bruit force SVM Search. You would use this" << endl
       << "          if the Search Process is already completed and you want to plot"     << endl
       << "          response to SVM paqrameters."                                        << endl
       << endl
       << "-Restart - Sets up status file to allow you to restart the serach process."    << endl
       << "           make sure to processes are still running when you use this option." << endl
       << endl
       << "-ProcessingStats"                                                              << endl
       << "         - Get Processing Stats by Processor and Class Combinations."          << endl  
       << endl
       << "-quit    - When running processes next refresh from there status files they"   << endl
       << "           will see this flag set and terminate gracefully."                   << endl
       << endl;
}  /* DisplayCommandUssage */




BinaryClassListPtr  FeatureSelection::StatusFileLoad ()
{
  log.Level (10) << "FeatureSelection::StatusFileLoad." << endl;

  char  buff[20480];

  FILE*  statusFile;

  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl
                   << "FeatureSelection::LoadCurrentStatusFile - *** ERROR ***  Can not open Status File[" 
                   << statusFileName << "]." 
                   << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }

  BinaryClassListPtr  binClasses = new BinaryClassList ();

  while  (fgets (buff, sizeof (buff), statusFile))
  {
    if  ((buff[0] == '/')  &&  (buff[1] == '/'))
      continue;

    KKStr  buffStr (buff);
    KKStr  fieldName = buffStr.ExtractToken ("\t\n\r");
    fieldName.Upper ();

    buffStr.TrimLeft  ("\n\r\t ");
    buffStr.TrimRight ("\n\r\t ");

    if  (fieldName.EqualIgnoreCase ("addingAClass")  ||  fieldName.EqualIgnoreCase ("addAClass")  ||  fieldName.EqualIgnoreCase ("AAC"))
      addingAClass = MLClass::CreateNewMLClass (buffStr);

    if  (fieldName == "BEAMSIZE")
      beamSize = atoi (buffStr.Str ());

    if  ((fieldName == "ALLCLASSES")  ||  (fieldName == "BYALLCLASSES"))
      byBinaryClasses = false;

    else if  (fieldName == "BYBINARYCLASSES")
      byBinaryClasses = true;

    else if  ((fieldName == "CONFIG")  ||  (fieldName == "CONFIGFILENAME"))
      configFileName = buffStr;


    else if  (fieldName.EqualIgnoreCase ("ConfigFileNameMfsFeaturesSel"))
      configFileNameMfsFeaturesSel = buffStr;

    else if  (fieldName.EqualIgnoreCase ("ConfigFileNameMfsFeatureSelTuned"))
      configFileNameMfsFeatureSelTuned = buffStr;

    else if  (fieldName.EqualIgnoreCase ("CONFIGALLCLASSESTUNEDFILENAME")  ||  fieldName.EqualIgnoreCase ("ConfigFileNameMfsTuned"))
      configFileNameMfsTuned = buffStr;

    else if  (fieldName.EqualIgnoreCase ("CONFIGALLCLASSESTUNEDFEATURESSELFILENAME")  ||  fieldName.EqualIgnoreCase ("ConfigFileNameMfsTunedFeaturesSel"))
      configFileNameMfsTunedFeaturesSel = buffStr;

    else if  (fieldName.EqualIgnoreCase ("configFileNameBfsTuned"))
      configFileNameBfsTuned = buffStr;


    else if  (fieldName.EqualIgnoreCase ("CONFIGBINARYCLASSESFEATURESSELFILENAME")  ||  fieldName.EqualIgnoreCase ("ConfigFileNameBfsFeaturesSel"))
      configFileNameBfsFeaturesSel = buffStr;

    else if  (fieldName.EqualIgnoreCase ("ConfigFileNameBfsTunedFeaturesSelTuned"))
      configFileNameBfsTunedFeaturesSelTuned = buffStr;

    else if  (fieldName.EqualIgnoreCase ("ConfigFileNameBfsTunedFeaturesSel"))
      configFileNameBfsTunedFeaturesSel = buffStr;

    else if  (fieldName.EqualIgnoreCase ("CONFIGBINARYCLASSESFEATURESSELTUNEDFILENAME")  ||  fieldName.EqualIgnoreCase ("ConfigFileNameBfsFeaturesSelTuned"))
      configFileNameBfsFeaturesSelTuned = buffStr;

    else if  (fieldName == "CONFIGFILENAMESPECIFIED")
      configFileNameSpecified = buffStr;

    else if  (fieldName.EqualIgnoreCase ("EXPANSIONLIMIT")  ||  fieldName.EqualIgnoreCase ("EL"))
      expansionLimit = atoi (buffStr.Str ());

    else if  (fieldName == "FEATURECRITERIA")
      featureCriteria = FeatureCriteriaFromStr (buffStr);

    else if  (fieldName.EqualIgnoreCase ("FeatureCountPrefSmall"))
      featureCountPrefSmall = buffStr.ToBool ();

    else if  (fieldName == "FILEFORMAT")
      driver = FeatureFileIO::FileFormatFromStr (buffStr);

    else if  (fieldName == "GRADINGMETHOD")
      gradingMethod = GradingMethodFromStr (buffStr);

    else if  (fieldName == "LOCKFILENAME")
      lockFileName = buffStr;

    else if  (fieldName == "MAJORSTEP")
    {
      majorStep = MajorStepFromStr (buffStr);
      if  (majorStep == msNULL)
      {
        log.Level (-1) << endl
                       << "FeatureSelection::StatusFileLoad       *** ERROR ***" << endl
                       << endl
                       << "                    Invlalid MajorStep[" << buffStr << "]" << endl
                       << endl;
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }

      SetMajorStep (majorStep);
    }

    else if  (fieldName.EqualIgnoreCase ("MinTrainExamples"))
      minTrainExamples = buffStr.ToInt ();

    else if  (fieldName == "NEXTPROCESSORID")
      nextProcessorId = atoi (buffStr.Str ());

    else if  (fieldName == "NUMJOBSATATIME")
      numJobsAtATime = buffStr.ToInt ();

    else if  (fieldName == "NUMOFFOLDS")
      numOfFolds = atoi (buffStr.Str ());

    else if  (fieldName == "NUMPROCPERCOMBO")
      numProcPerCombo = atoi (buffStr.Str ());

    else if  (fieldName == "PARAMSELCRITERIA")
    {
      paramSelCriteria = JobSelectionCriteriaFromStr (buffStr);
      if  (paramSelCriteria == jscNULL)
      {
        log.Level (-1) << endl
                       << "FeatureSelection::StatusFileLoad       *** ERROR ***" << endl
                       << endl
                       << "                    Invlaid ParamSelCriteria[" << buffStr << "]" << endl
                       << endl;
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (fieldName.EqualIgnoreCase ("ProcessingOrder"))
    {
      processingOrder = ProcessingOrderFromStr (buffStr);
      if  (processingOrder == poNULL)
      {
        log.Level (-1) << endl << endl
          << "FeatureSelection::StatusFileLoad   ***ERROR***   Invalid ProcessingOrder[" << buffStr << "] Encountered." << endl
          << endl;
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (fieldName == "QUITRUNNING")
      quitRunning = true;
    
    else if  (fieldName.EqualIgnoreCase ("RandomSplitsNum"))
      randomSplitsNum = buffStr.ToInt ();
    
    else if  (fieldName.EqualIgnoreCase ("RandomSplitsFileName"))
      randomSplitsFileName = buffStr;
    
    else if  (fieldName == "RESTART")
      quitRunning = false;

    else if  (fieldName == "SEARCHTYPE")
      searchType = SearchTypeFromStr (buffStr);

    else if  (fieldName == "TESTDATAFILENAME")
      testDataFileName = buffStr;

    else if  ((fieldName == "FEATUREFILENAME")  ||  (fieldName == "TRAININGDATAFILENAME"))
      trainingDataFileName = buffStr;

    else if  (fieldName == "VALIDATIONDATAFILENAME")
      validationDataFileName = buffStr;

    else if  (fieldName == "INITIALFEATURES")
      initialFeaturesStr = buffStr;

    else if  (fieldName == "BINARYCLASS")
    {
      BinaryClassPtr  bc = new BinaryClass (buffStr, mlClasses);
      binClasses->PushOnBack (bc);
    }
  }

  if  (majorStep == msNULL)
  {
    log.Level (-1) << endl << endl
          << "FeatureSelection::StatusFileLoad   ***ERROR***   MajorStep was not specified." << endl
          << endl;
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }

  SetMajorStep (majorStep);

  fclose (statusFile);
  

  return  binClasses;
}  /* StatusFileLoad */




void  FeatureSelection::StatusFileUpdate (BinaryClassListPtr  binClasses)
{
  log.Level (10) << "FeatureSelection::StatusFileUpdate" << endl;

  {
    // Lets rename first
    KKStr  toName = osRemoveExtension (statusFileName);
    KKStr  ext = osGetFileExtension (statusFileName);
    KKStr  toName1 = toName + "_BackUp1." + ext;
    KKStr  toName2 = toName + "_BackUp2." + ext;

    osDeleteFile (toName2);
    osRenameFile (toName1, toName2);
    osRenameFile (statusFileName, toName1);
  }


  bool  statusFileWritten = false;
  while  (!statusFileWritten)
  {
    int  openAttempts = 0;

    bool  writeErrorOccured = false;
    int   errorCode         = -1;

    ofstream  statusFile  (statusFileName.Str (), ios::out);

    while  ((!statusFile.is_open ()) &&  (openAttempts < 10))
    {
      openAttempts++;
      osSleep (3.0f);
      log.Level (50) << "UpdateStatusFile Open Attempt[" << openAttempts << "]." << endl;
      statusFile.open (statusFileName.Str (), ios::out);
      if  (!statusFile.is_open ())
      {
        log.Level (0) << "UpdateStatusFile - ***WARNING***  Can Not Open Status File[" 
                      << statusFileName << "]   -Will Retry-." 
                      << endl;
      }
    }

    if  (!statusFile.is_open ())
    {
      log.Level (-1) << "UpdateStatusFile - ***ERROR***  Can not Create Status File[" 
                     << statusFileName << "]." 
                     << endl;
      writeErrorOccured = true;
      errorCode = -1;
      EndBlock ();
      exit (-1);
    }

    statusFile << "BeamSize"                                    << "\t" << beamSize                                     << endl
               << "ConfigFileName"                              << "\t" << configFileName                               << endl
               << "ConfigFileNameSpecified"                     << "\t" << configFileNameSpecified                      << endl
               << "ConfigFileNameMfsFeaturesSel"           << "\t" << configFileNameMfsFeaturesSel                 << endl
               << "ConfigFileNameMfsFeatureSelTuned"       << "\t" << configFileNameMfsFeatureSelTuned             << endl
               << "ConfigFileNameMfsTuned"                 << "\t" << configFileNameMfsTuned                       << endl
               << "ConfigFileNameMfsTunedFeaturesSel"      << "\t" << configFileNameMfsTunedFeaturesSel            << endl
               << "ConfigFileNameBfsTuned"                 << "\t" << configFileNameBfsTuned                       << endl
               << "ConfigFileNameBfsFeaturesSel"           << "\t" << configFileNameBfsFeaturesSel                 << endl
               << "ConfigFileNameBfsTunedFeaturesSel"      << "\t" << configFileNameBfsTunedFeaturesSel            << endl
               << "ConfigFileNameBfsTunedFeaturesSelTuned" << "\t" << configFileNameBfsTunedFeaturesSelTuned       << endl
               << "ConfigFileNameBfsFeaturesSelTuned"      << "\t" << configFileNameBfsFeaturesSelTuned            << endl
               << "FeatureCriteria"                             << "\t" << FeatureCriteriaToStr (featureCriteria)       << endl
               << "FeatureCountPrefSmall"                  << "\t" << (featureCountPrefSmall ? "Yes" : "No")       << endl
               << "FileFormat"                                  << "\t" << driver->DriverName ()                        << endl
               << "GradingMethod"                               << "\t" << GradingMethodToStr (gradingMethod)           << endl
               << "TrainingDataFileName"                        << "\t" << trainingDataFileName                         << endl
               << "TestDataFileName"                            << "\t" << testDataFileName                             << endl
               << "ValidationDataFileName"                      << "\t" << validationDataFileName                       << endl
               << "InitialFeatures"                             << "\t" << initialFeaturesStr                           << endl
               << "RandomSplitsNum"                        << "\t" << randomSplitsNum                              << endl
               << "RandomSplitsFileName"                   << "\t" << randomSplitsFileName                         << endl
               << "NextProcessorId"                             << "\t" << nextProcessorId                              << endl
               << "LockFileName"                                << "\t" << lockFileName                                 << endl
               << "NumJobsAtATime"                              << "\t" << numJobsAtATime                               << endl
               << "NumOfFolds"                                  << "\t" << numOfFolds                                   << endl
               << "NumProcPerCombo"                             << "\t" << numProcPerCombo                              << endl
               << "ExpansionLimit"                              << "\t" << expansionLimit                               << endl
               << "MinTrainExamples"                       << "\t" << minTrainExamples                             << endl
               << "ParamSelCriteria"                            << "\t" << JobSelectionCriteriaToStr (paramSelCriteria) << endl
               << (byBinaryClasses ? "ByBinaryClasses" : "ByAllClasses")                                                << endl
               << "SearchType"                                  << "\t" << SearchTypeToStr (searchType)                 << endl
               << "ProcessingOrder"                        << "\t" << ProcessingOrderToStr (processingOrder)       << endl
               << "MajorStep"                                   << "\t" << MajorStepToStr (majorStep)                   << endl;

    if  (addingAClass)
      statusFile << "AddingAClass" << "\t" << addingAClass->Name () << endl;

    statusFile.flush ();

    if  (statusFile.fail ())
    {
      writeErrorOccured = true;
    }
    else
    {
      if  (quitRunning)
        statusFile << "QuitRunning" << endl;

      int  x;

      for  (x = 0;  x < binClasses->QueueSize ();  x++)
      {
        BinaryClassPtr  bc = binClasses->IdxToPtr (x);
        statusFile << "BinaryClass" << "\t" << bc->ToStr () << endl;
        if  (statusFile.fail ())
        {
          writeErrorOccured = true;
          break;
        }
      }
    }

    statusFile.flush ();
    if  (statusFile.fail ())
      writeErrorOccured = true;

    statusFile.close ();
    if  (statusFile.fail ())
      writeErrorOccured = true;


    if  (writeErrorOccured)
    {
      log.Level (-1) << endl 
                     << "StatusFileUpdate     ***ERROR***     Error occured trying to write[" << statusFileName << "]" << endl
                     << endl
                     << "                     Error Code[" << errorCode << "]" << endl
                     << endl;
      osSleep (3.0f);
    }
    else
    {
      statusFileWritten = true;
    }

  }  /*  while (!statusFileWritten); */


  log.Level (50) << "StatusFileUpdate     Exiting." << endl;
}  /* StatusFileUpdate */






void   FeatureSelection::ProcessCmdLine (int    argc, 
                                         char **argv
                                        )
{
  int i = 1;

  while  (i < argc)
  {
    KKStr  switchStr (argv[i]);
    switchStr.Upper ();

    KKStr  field = "";
    if  (argc > (i + 1))
    {
      if  (argv[i + 1][0] != '-')
      {
        i++;
        field = argv[i];
      }
    }

    if  (switchStr.EqualIgnoreCase ("-addingAClass")  ||  switchStr.EqualIgnoreCase ("-addAClass")  ||  switchStr.EqualIgnoreCase ("-AAC"))
    {
      if  (field.Empty ())
      {
        log.Level (-1) << "ProcessCmdLine   **** ERROR ****"  << endl
                       << "                   -AddAClass requires the name of a Class that is being added."
                       << endl
                       << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
      addingAClass = MLClass::CreateNewMLClass (field);
    }

    else if  (switchStr == "-BEAMSIZE") 
    {
      beamSize = atoi (field.Str ());
    }

    else if  ((switchStr == "-ALLCLASSES")  ||  (switchStr == "-BYALLCLASSES"))
    {
      byBinaryClasses = false;
    }

    else if  ((switchStr == "-BINARYCLASSES")  ||  (switchStr == "-BYBINARYCLASSES"))
    {
      byBinaryClasses = true;
    }

    else if  ((switchStr == "-CONFIG")  ||  (switchStr == "-C"))
    {
      configFileNameSpecified = field;
      configFileName = TrainingConfiguration2::GetEffectiveConfigFileName (field);
      if  (configFileName.Empty ())
      {
        log.Level (-1) << "ProcessCmdLine   **** ERROR ****"  << endl
                       << "                   -Config requires parameter"
                       << endl
                       << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }

      if  (!osFileExists (configFileName))
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLine   ConfigFileName[" + configFileNameSpecified + "] does not exist" << endl
                       << endl << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr.EqualIgnoreCase ("-ExpansionLimit")  ||  switchStr.EqualIgnoreCase ("-EL"))
    {
      expansionLimit = field.ToInt ();
      if  (expansionLimit < 1)
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLine   ExpansionLimit[" << field << "]  Must be greter than zero." << endl
                       << endl << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr == "-FILEFORMAT")  ||  (switchStr == "-FORMAT"))
    {
      driver = FeatureFileIO::FileFormatFromStr (field, true, false);  // Has to beable to read
      if  (driver == NULL)
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLine   Invalid File Format[" << field << "]" << endl
                       << endl
                       << "         Valid Formats[" << FeatureFileIO::FileFormatsReadOptionsStr () << "]"  << endl
                       << endl << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr == "-GM")  ||  (switchStr == "-GRADINGMETHOD"))
    {
      gradingMethod = GradingMethodFromStr (field);
      if  (gradingMethod == gmNULL)
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLine   Invalid Grading Method Specified[" << field << "]" << endl
                       << endl << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr == "-FEATURECRITERIA")  ||  (switchStr == "-FC"))
    {
      featureCriteria = FeatureCriteriaFromStr (field);
      if  (featureCriteria == fcNULL)
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLine   Invalid Feature Criteria Specified[" << field << "]" << endl
                       << endl << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr.EqualIgnoreCase ("-FeatureCountPrefSmall")  ||  switchStr.EqualIgnoreCase ("-FCPS"))
    {
      featureCountPrefSmall = true;
      if  (!field.Empty ())
        featureCountPrefSmall = field.ToBool ();
    }

    else if  (switchStr == "-FOLDS") 
    {
      numOfFolds = atoi (field.Str ());
    }


    else if  ((switchStr == "-INITIALFEATURES")  ||
              (switchStr == "-IF")
             )
    {
      initialFeaturesStr = field;
    }

    else if  (switchStr.EqualIgnoreCase ("-MinTrainExamples")  ||  switchStr.EqualIgnoreCase ("-MTE"))
    {
      minTrainExamples = field.ToInt ();
    }


    else if  ((switchStr == "-NUMJOBSATATIME")  ||
              (switchStr == "-JOBSATATIME")
             )
    {
      numJobsAtATime = field.ToInt ();
    }

    else if  ((switchStr == "-NUMPROCPERCOMBO")  ||
              (switchStr == "-NUMPROC")          ||
              (switchStr == "-NUMPROCESSORS")    ||
              (switchStr == "-PROCPERCOMBO")
             )
    {
      numProcPerCombo = field.ToInt ();
    }



    else if  ((switchStr == "-PARAMSELCRITERIA")           ||
              (switchStr == "-PSC")                        ||
              (switchStr == "-PARAMETERSELCRITERIA")       ||
              (switchStr == "-PARAMETERSELECTIONCRITERIA")
             )
    {
      paramSelCriteria = JobSelectionCriteriaFromStr (field);
      if  (paramSelCriteria == jscNULL)
      {
        log.Level (-1) << "ProcessCmdLine   ***ERROR***"  << endl
                       << endl
                       << "             Invalid ParamSelCriteria[" << field << "]" << endl
                       << endl
                       << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr.EqualIgnoreCase ("-ProcessingOrder"))  ||  (switchStr.EqualIgnoreCase ("-PO")))
    {
      processingOrder = ProcessingOrderFromStr (field);
      if  (processingOrder == poNULL)
      {
        log.Level (-1) << endl << endl
          << "ProcessCmdLine   ***ERROR***      Invalid ProcessingOrder[" << field << "]" << endl
          << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    
    else if  (switchStr.EqualIgnoreCase ("-RandomSplitsNum")  ||  
              switchStr.EqualIgnoreCase ("-RandomSplits")     ||
              switchStr.EqualIgnoreCase ("-RSN")              ||
              switchStr.EqualIgnoreCase ("-RS")
             )
    {
      randomSplitsNum = field.ToInt ();
      if  ((randomSplitsNum < 2)  &&  (randomSplitsNum != 0))
      {
        log.Level (-1) << endl << endl
          << "ProcessCmdLine   ***ERROR***      -RandomSplitsNum[" << field << "]  must be a integer 2 or greater." << endl
          << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }


    else if  ((switchStr == "-RESULTCONFIG")          ||
              (switchStr == "-RESULTCONFIGFILENAME")  ||  
              (switchStr == "-RESULTFILENAME")  
             )
    {
      resultConfigFileName = field;
    }

    else if  (switchStr == "-SEARCHTYPE")
    {
      searchType = SearchTypeFromStr (field);
      if  (searchType == stNULL)
      {
        log.Level (-1) << endl << endl
          << "ProcessCmdLine   ***ERROR***    Invalid SearchType[" << field << "]" << endl
                       << endl;

        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  (switchStr == "-TESTDATA")
    {
      testDataFileName = field;
      if  (!osFileExists (testDataFileName))
      {
        log.Level (-1) << "ProcessCmdLine   **** ERROR ****"  << endl
                       << endl
                       << "         Can not find Test Data File[" << testDataFileName << "]" << endl
                       << endl
                       << endl;
        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }


    else if  ((switchStr == "-TRAINDATA")     ||
              (switchStr == "-TRAININGDATA")  ||
              (switchStr == "-DATAFILE")  
             )
    {
      trainingDataFileName = field;
      if  (!osFileExists (trainingDataFileName))
      {
        log.Level (-1) << "ProcessCmdLine   **** ERROR ****"  << endl
                       << endl
                       << "         Can not find Training Data File[" << trainingDataFileName << "]" << endl
                       << endl
                       << endl;
        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else if  ((switchStr == "=VALIDATION")     ||
              (switchStr == "-VALIDATIONDATA")  ||
              (switchStr == "-VD")
             )
    {
      validationDataFileName = field;
      if  (!osFileExists (validationDataFileName))
      {
        log.Level (-1) << "ProcessCmdLine   **** ERROR ****"  << endl
                       << endl
                       << "         Can not find Validation Data File[" << validationDataFileName << "]" << endl
                       << endl
                       << endl;
        DisplayCommandUsage ();
        EndBlock ();
        osWaitForEnter ();
        exit (-1);
      }
    }

    else
    {
      log.Level (-1) << endl << endl << endl
                     << "ProcessCmdLine     **** ERROR ****"  << endl
                     << endl
                     << "              Unrecognized Parameter[" << switchStr << "]" << endl
                     << endl;

      DisplayCommandUsage ();
      EndBlock ();
      osWaitForEnter ();
      exit (-1);
    }

    i++;
  } /* End of While */


  if  (processingOrder == poStandard)
    SetMajorStep (msMfsParmTuningPre);

  else if  (processingOrder == poHall)
    SetMajorStep (msMfsFeatureSelection);

  else
  {
    log.Level (-1) << endl << endl  << "ProcessCmdLine     ***ERROR***     Processing Order was not Specified."  << endl << endl;
    DisplayCommandUsage ();
    EndBlock ();
    osWaitForEnter ();
    exit (-1);
  }

  if  (numProcPerCombo < 1)
  {
    if  (byBinaryClasses)
      numProcPerCombo = 1;
    else
      numProcPerCombo = 9999;
  }


  if  (numJobsAtATime < 1)
  {
    if  (byBinaryClasses)
      numJobsAtATime = 9999;
    else
      numProcPerCombo = 1;
  }
}  /* ProcessCmdLine */



void  FeatureSelection::SetUpConfigFileNames ()
{
  if  (configFileName.Empty ())
  {
    if  (trainingDataFileName.Empty ())
    {
      log.Level (-1) << endl << endl
        << "FeatureSelection::SetUpConfigFileNames   ***ERROR***    TrainingDataFileName is blank." << endl
        << endl;
      EndBlock ();
      exit (-1);
    }

    configFileNameSpecified = osGetRootName (trainingDataFileName) + ".cfg";
    configFileName = TrainingConfiguration2::GetEffectiveConfigFileName (configFileNameSpecified);
    if  (!osFileExists (configFileName))
      configFileName = osGetRootName (trainingDataFileName) + ".cfg";
  }

    KKStr  rootName = osRemoveExtension (configFileName);
  if  (processingOrder == poStandard)
  {
    configFileNameMfsFeatureSelTuned       = "";
    configFileNameBfsTuned                 = "";
    configFileNameBfsTunedFeaturesSelTuned = "";

    configFileNameMfsTuned                       = rootName + "_AllClasses-Tuned.cfg";
    configFileNameMfsFeaturesSel                 = rootName + "_AllClasses-FeaturesSel.cfg";
    configFileNameMfsTunedFeaturesSel            = rootName + "_AllClasses-TunedFeaturesSel.cfg";

    configFileNameBfsFeaturesSel                 = rootName + "_BinaryClasses-FeaturesSel.cfg";
    configFileNameBfsFeaturesSelTuned            = rootName + "_BinaryClasses-FeaturesSelTuned.cfg";
  }
  else if  (processingOrder == poHall)
  {
    configFileNameMfsTuned                       = "";
    configFileNameMfsTunedFeaturesSel            = "";
    configFileNameBfsFeaturesSel                 = "";
    configFileNameBfsFeaturesSelTuned            = "";

    configFileNameMfsFeaturesSel                 = rootName + "_MFS-FeaturesSel.cfg";
    configFileNameMfsFeatureSelTuned             = rootName + "_MFS-FeaturesSelTuned.cfg";

    configFileNameBfsTuned                       = rootName + "_BFS-Tuned.cfg";
    configFileNameBfsTunedFeaturesSel            = rootName + "_BFS-TunedFeaturesSel.cfg";
    configFileNameBfsTunedFeaturesSelTuned       = rootName + "_BFS-TunedFeaturesSelTuned.cfg";
  }
}  /* SetUpConfigFileNames */





void  FeatureSelection::LoadRunTimeData ()
{
  delete  mlClasses;  mlClasses = NULL;

  LoadFeatureData ();
  
  // KK 2010-07-11
  // We want to use the Config File names from the FeatureSelection.Status file so we only want to call 'SetUpConfigFileNames' once during initial setup.
  // SetUpConfigFileNames ();

  if  (configFileName.Empty ())
  {
    log.Level (-1) << endl << endl 
      << "FeatureSelection::LoadRunTimeData    ***ERROR***    'configFileName' is blank." << endl
      << endl;
    EndBlock ();
    exit (-1);
  }

  if  (!osFileExists (configFileName))
  {
    delete  mlClasses;  mlClasses = NULL;
    mlClasses = trainingData->ExtractMLClassConstList ();
    config = TrainingConfiguration2::CreateFromFeatureVectorList (*trainingData, "", log);
    if  (initialFeatures)
      config->SetFeatureNums (*initialFeatures);
    config->Save (configFileName);
  }

  if  (!config)
    config = OpenConfig (configFileName);

  if  ((!config)  ||  (!config->FormatGood ()))
  {
    log.Level (-1) << endl
                   << endl
                   << "FeatureSelection::LoadRunTimeData    ***ERROR***" << endl
                   << endl
                   << "   Could not load Config File[" << configFileName << "]" << endl
                   << endl;
    EndBlock ();
    exit (-1);
  }

  delete  mlClasses;  mlClasses = NULL;
  mlClasses = config->ExtractClassList ();


  if  (addingAClass)
  {
    // Since we are adding a class we need to make sure that the class beeing added exists in the configuration file.
    if  (mlClasses->PtrToIdx (addingAClass) < 0)
    {
      log.Level (-1) << endl
                     << endl
                     << "FeatureSelection::LoadRunTimeData    ***ERROR***" << endl
                     << endl
                     << "   The new Class[" << addingAClass->Name () << "] is not in the config file." << endl
                     << endl;
      EndBlock ();
      exit (-1);
    }
  }


  {
    // Check for agreement of class list between Train Data and Config File.
    MLClassConstListPtr  trainDataClassList = trainingData->ExtractMLClassConstList ();
    if  ((*trainDataClassList) !=  (*mlClasses))
    {
      MLClassConstList  classesInConfigNotInTrain;
      MLClassConstList  classesInTrainNotInConfig;
      classesInConfigNotInTrain = (*mlClasses) - (*trainDataClassList);
      classesInTrainNotInConfig = (*trainDataClassList) - (*mlClasses);

      log.Level (-1) << endl << endl
                     << "FeatureSelection::LoadRunTimeData         ***ERROR***"
                     << endl
                     << " The Training data ClassList does nit agree with what was specified in the config file."
                     << endl
                     << "Classes in Config not in train[" << classesInConfigNotInTrain.ToString () << "]" << endl
                     << "Classes in Train not in Config[" << classesInTrainNotInConfig.ToString () << "]" << endl
                     << endl;
      EndBlock ();
      exit (-1);
    }

    delete  trainDataClassList;  trainDataClassList = NULL;
  }

  positiveClass = mlClasses->LookUpByName ("0");
  if  (!positiveClass)
    positiveClass = mlClasses->BackOfQueue ();

  delete  initialFeatures;  initialFeatures = NULL;
  if  (initialFeaturesStr.Empty ())
  {
    initialFeatures = new FeatureNumList (config->GetFeatureNums ());
    initialFeaturesStr = initialFeatures->ToString ();
  }
  else
  {
    bool valid = false;
    initialFeatures = new FeatureNumList (config->FileDesc (), initialFeaturesStr, valid);
    if  (!valid)
    {
      log.Level (-1) << endl << endl
                     << "FeatureSelection::LoadRunTimeData    *** ERROR ***" << endl
                     << endl
                     << "   Invalid InitialFeatures [" << initialFeaturesStr << "]" << endl
                     << endl;
      EndBlock ();
      exit (-1);
    }
  }

  {
    normalizationParms = new NormalizationParms (config, *trainingData, log);
    normalizationParms->NormalizeImages (trainingData);
    alreadyNormalized = true;

    if  (testData)
      normalizationParms->NormalizeImages (testData);

    if  (validationData)
      normalizationParms->NormalizeImages (validationData);
  }
}  /* LoadRunTimeData */




BinaryClassListPtr   FeatureSelection::CreateInitialBinaryClassListBruitSvmSearch ()
{
  log.Level (10) << "FeatureSelection::CreateInitialBinaryClassListBruitSvmSearch"  << endl;
  BinaryClassListPtr  binClasses = new BinaryClassList ();
  BinaryClassPtr  bc = new BinaryClass (NULL, NULL, 0);
  binClasses->PushOnBack (bc);
  return  binClasses;
}  /* CreateInitialBinaryClassListBruitSvmSearch */




BinaryClassListPtr   FeatureSelection::CreateInitialBinaryClassListFinalResultJobs ()
{
  log.Level (10) << "FeatureSelection::CreateInitialBinaryClassListFinalResultJobs"  << endl;

  BinaryClassListPtr  binClasses = new BinaryClassList ();

  if  (this->processingOrder == poStandard)
  {
    binClasses->PushOnBack (new BinaryClass (0, frtNoTuningAllFeatures,       SelectByVoting,      "MFS-NoTuningAllFeaturesVote.txt",     configFileNameSpecified));
    binClasses->PushOnBack (new BinaryClass (0, frtNoTuningAllFeatures,       SelectByProbability, "MFS-NoTuningAllFeaturesProb.txt",     configFileNameSpecified));

    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTuned,             SelectByVoting,      "FeaturesAllParmsTunedVote.txt",       configFileNameMfsTuned));
    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTuned,             SelectByProbability, "FeaturesAllParmsTunedProb.txt",       configFileNameMfsTuned));

    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTunedFeaturesSel,  SelectByVoting,      "FeaturesSelParmsTunedVote.txt",       configFileNameMfsTunedFeaturesSel));
    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTunedFeaturesSel,  SelectByProbability, "FeaturesSelParmsTunedProb.txt",       configFileNameMfsTunedFeaturesSel));

    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSel,            SelectByVoting,      "BinaryFeaturesSelVote.txt",           configFileNameBfsFeaturesSel));
    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSel,            SelectByProbability, "BinaryFeaturesSelProb.txt",           configFileNameBfsFeaturesSel));

    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByVoting,      "BinaryFeaturesSelParmsTunedVote.txt", configFileNameBfsFeaturesSelTuned));
    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByProbability, "BinaryFeaturesSelParmsTunedProb.txt", configFileNameBfsFeaturesSelTuned));
  }

  else if  (this->processingOrder == poHall)
  {
    binClasses->PushOnBack (new BinaryClass (0, frtNoTuningAllFeatures,       SelectByVoting,      "MFS-NoTuningAllFeaturesVote.txt",     configFileNameSpecified));
    binClasses->PushOnBack (new BinaryClass (0, frtNoTuningAllFeatures,       SelectByProbability, "MFS-NoTuningAllFeaturesProb.txt",     configFileNameSpecified));

    binClasses->PushOnBack (new BinaryClass (0, frtMfsFeaturesSel,            SelectByVoting,      "MFS-FeaturesSelVote.txt",             configFileNameMfsFeaturesSel));
    binClasses->PushOnBack (new BinaryClass (0, frtMfsFeaturesSel,            SelectByProbability, "MFS-FeaturesSelProb.txt",             configFileNameMfsFeaturesSel));

    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTunedFeaturesSel,  SelectByVoting,      "MFS-FeaturesSelParmsTunedVote.txt",   configFileNameMfsFeatureSelTuned));
    binClasses->PushOnBack (new BinaryClass (0, frtMfsParmsTunedFeaturesSel,  SelectByProbability, "MFS-FeaturesSelParmsTunedProb.txt",   configFileNameMfsFeatureSelTuned));


    binClasses->PushOnBack (new BinaryClass (0, frtBfsParmsTuned,             SelectByVoting,      "BFS-ParmsTunedVote.txt",              configFileNameBfsTuned));
    binClasses->PushOnBack (new BinaryClass (0, frtBfsParmsTuned,             SelectByProbability, "BFS-ParmsTunedProb.txt",              configFileNameBfsTuned));

    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByVoting,      "BFS-ParmsTunedFeaturesSelVote.txt",   configFileNameBfsTunedFeaturesSel));
    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByProbability, "BFS-ParmsTunedFeaturesSelProb.txt",   configFileNameBfsTunedFeaturesSel));

    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByVoting,      "BFS-ParmsTunedFeaturesSelParmsTunedVote.txt",   configFileNameBfsTunedFeaturesSelTuned));
    binClasses->PushOnBack (new BinaryClass (0, frtBfsFeaturesSelParmsTuned,  SelectByProbability, "BFS-ParmsTunedFeaturesSelParmsTunedProb.txt",   configFileNameBfsTunedFeaturesSelTuned));
  }

  return  binClasses;
}  /* CreateInitialBinaryClassListFinalResultJobs */




BinaryClassListPtr  FeatureSelection::CreateInitialBinaryClassListBinaryComboSearchJobs ()
{
  log.Level (10) << "FeatureSelection::CreateInitialBinaryClassListBinaryComboSearchJobs"  << endl;

  BinaryClassListPtr  binClasses = new BinaryClassList ();

  MLClassConstListPtr  classes = config->ExtractClassList ();
  classes->SortByName ();

  MLClassConstPtr  class1 = NULL;
  MLClassConstPtr  class2 = NULL;

  int  idx1, idx2;
  for  (idx1 = 0;  idx1 < (classes->QueueSize () - 1);  idx1++)
  {
    class1 = classes->IdxToPtr (idx1);
    for  (idx2 = idx1 + 1;  idx2 < classes->QueueSize ();  idx2++)
    {
      class2 = classes->IdxToPtr (idx2);

      if  ((addingAClass == NULL)    ||  
           (addingAClass == class1)  ||  (addingAClass == class2))  
      {
        // We only want to add a BinaryClass if one of the classes in the Class being added.
        BinaryClassPtr  bc = new BinaryClass (class1, class2, 0);
        binClasses->PushOnBack (bc);
      }
    }
  }

  delete  classes;  classes = NULL;
  
  return  binClasses;
}  /* CreateInitialBinaryClassListBinaryComboSearchJobs */





BinaryClassListPtr  FeatureSelection::CreateInitialBinaryClassList ()
{
  log.Level (10) << "FeatureSelection::CreateInitialBinaryClassList  " 
                 << "MajorStemp["  << MajorStepToStr (majorStep) << "]  "
                 << "MachineType[" << (byBinaryClasses ? "Binary Classes" : "All Classes") << "]" 
                 << endl;

  BinaryClassListPtr  binClasses = NULL;

  if  (majorStep == msGenerateBruitSvmSearch)
  {
    binClasses = CreateInitialBinaryClassListBruitSvmSearch ();
  }

  else if  (majorStep == msGenerateFinalResults)
  {
    binClasses = CreateInitialBinaryClassListFinalResultJobs ();
  }

  else if  (byBinaryClasses)
  {
    binClasses = CreateInitialBinaryClassListBinaryComboSearchJobs ();
  }

  else
  {
    binClasses = new BinaryClassList ();
    BinaryClassPtr  bc = new BinaryClass (NULL, NULL, 0);
    binClasses->PushOnBack (bc);
  }

  return  binClasses;
}  /* CreateInitialBinaryClassList */




void  FeatureSelection::PerformInitialSetup (int     argc, 
                                             char**  argv
                                            )
{
  ProcessCmdLine (argc, argv);

  SetUpConfigFileNames ();

  LoadRunTimeData ();

  BinaryClassListPtr  binClasses = CreateInitialBinaryClassList ();

  StatusFileUpdate (binClasses);

  delete  binClasses;
}  /* PerformInitialSetup */






// read in a problem (in svmlight format)
void FeatureSelection::LoadFeatureData ()
{
  bool  successful;
  bool  changesMade = false;

  log.Level (10) << "FeatureSelection::LoadFeatureData  Load Data Files" << endl;

  if  (mlClasses == NULL)
    mlClasses = new MLClassConstList ();

  trainingData = driver->LoadFeatureFile (trainingDataFileName, 
                                          *mlClasses, 
                                          -1, 
                                          cancelFlag, 
                                          successful, 
                                          changesMade,
                                          log
                                         );
  if  (!successful)
  {
    log.Level (-1) << endl
                   << " *****  Can not open input file[" << trainingDataFileName << "]  *****" 
                   << endl;
    EndBlock ();
  	osWaitForEnter ();
    exit (-1);
  }

  fileDesc = trainingData->FileDesc ();

  if  (!testDataFileName.Empty ())
  {
    testData = driver->LoadFeatureFile (testDataFileName, 
                                        *mlClasses, 
                                        -1, 
                                        cancelFlag, 
                                        successful, 
                                        changesMade,
                                        log
                                       );
    if (!successful)
    {
      log.Level (-1) << endl
                     << " *****  Can not load test file[" << testDataFileName << "]  *****" 
                     << endl;
      EndBlock ();
    	osWaitForEnter ();
      exit (-1);
    }

    if  (trainingData->FileDesc () != testData->FileDesc ())
    {
      log.Level (-1) << endl << endl
                     << "FeatureSelection::LoadFeatureData          *** Error ***" << endl
                     << endl
                     << "         the TrainingData and TestData  files are not the same format." << endl
                     << endl;
      EndBlock ();
    	osWaitForEnter ();
      exit (-1);
    }
  }

  if  (trainingData  &&  testData)
  {
    trainingTestData = new FeatureVectorList (*trainingData, false);
    trainingTestData->AddQueue (*testData);
    trainingTestData = trainingTestData->StratifyAmoungstClasses (10);
  }
  else if  (trainingData)
  {
    trainingTestData = new FeatureVectorList (*trainingData, false);
  }

  mlClasses->SortByName ();
}  /* LoadFeatureData */




void  FeatureSelection::SetQuitRunningFlag ()
{
  log.Level (10) << "FeatureSelection::SetQuitRunningFlag" << endl;
  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl << endl << endl 
                   << "FeatureSelection::SetQuitRunningFlag      *** QUIT specified but not Status file exists ***" << endl
                   << endl;
    exit (0);
  }

  fclose (statusFile);

  BinaryClassListPtr  binClasses = StatusFileLoad ();

  LoadRunTimeData ();

  quitRunning = true;
  StatusFileUpdate (binClasses);
  EndBlock ();

  {
    BinaryClassList::iterator  idx;
    for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
    {
      BinaryClassPtr  bc = *idx;
      if  (bc->Status () == cpStarted)
      {
        ProcessorPtr  p = NULL;

        if  (bc->ResultType () == frtNULL)
        {
          // We are creating a process that will be doing searching.
          p = new Processor (this,
                             byBinaryClasses,
                             -1,
                             fileDesc,
                             searchType,
                             bc->Class1 (),
                             bc->Class2 (),
                             smBestCaseNext,
                             fsStarted
                           );

        }

        else
        {
          p = new Processor (this,
                             -1,
                             bc,
                             fileDesc,
                             fsStarted
                            );
        }

        p->SetQuitRunningFlag ();
      }
    }
  }
  delete  binClasses;  binClasses = NULL;

  log.Level (10) << endl << endl << endl
                 << "FeatureSelection::SetQuitRunningFlag      *** QUIT FLAG SET ***" << endl
                 << endl;
  exit (0);
}  /* SetQuitRunningFlag */



void  CalcClassPairWeighted (VectorDouble& counts, 
                             VectorInt&    exampleCountPerClassPair, 
                             double&       weightedCount
                            )
{
  double  grandTotal = 0.0;
  double  countsTotal = 0.0;
  for  (KKU::uint x = 0;  x < counts.size ();  x++)
  {
    grandTotal = exampleCountPerClassPair[x] * counts[x];
    countsTotal += counts[x];
  }

  weightedCount = grandTotal / countsTotal;
}  /* CalcClassPairWeighted */




void  FeatureSelection::GetProcessingStats ()
{
  log.Level (10) << "FeatureSelection::GetProcessingStats" << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl << endl << endl 
                   << "FeatureSelection::GetProcessingStats      *** There is no Status File ***" << endl
                   << endl;
    return;
  }

  fclose (statusFile);

  BinaryClassListPtr  binClasses = StatusFileLoad ();

  resultConfigFileName = configFileNameSpecified;
  configFileName       = configFileNameSpecified;

  LoadFeatureData ();

  delete  config;
  config = new TrainingConfiguration2 (fileDesc, configFileName, log, false);

  if  (!config->FormatGood ())
  {
    log.Level (-1) << endl
                   << endl
                   << "FeatureSelection::LoadRunTimeData    ***ERROR***" << endl
                   << endl
                   << "   Could not load Config File[" << configFileName << "]" << endl
                   << endl;
    EndBlock ();
    exit (-1);
  }


  delete  mlClasses;  mlClasses = NULL;
  mlClasses = config->ExtractClassList ();


  KKStr  hdrLine1 (1024);
  KKStr  hdrLine2 (1024);

  VectorInt     zeroCounts       (fileDesc->NumOfFields () + 1, 0);
  VectorDouble  zeroCountsDouble (fileDesc->NumOfFields () + 1, 0.0);

  VectorInt             jobIdsByFeatureCountAllClasses (fileDesc->NumOfFields () + 1, 0);
  vector<VectorInt>     jobIdsByFeatureCountBinaryClasses;

  VectorInt             exampleCountPerClassPair;
  int                   exampleCountTotal = 0;

  int                   featureCountBeamSearchStartMFS = INT_MAX;
  VectorDouble          featureCountBeamSearchStartBFS;

  vector<VectorDouble>  cpuCyclesByFeatureCountBinaryClasses;
  VectorDouble          cpuCyclesByFeatureCountAllClasses (fileDesc->NumOfFields () + 1, 0.0);

  VectorDouble  fsCpuCyclesByClassPair;
  VectorDouble  parnCParamByClassPairBFS;
  VectorDouble  parnGammaParamByClassPairBFS;
  VectorDouble  parnProbParamByClassPairBFS;
  VectorDouble  fsFeatureCountByClassPair;
  double        fsCpuCyclesMFS = 0.0;
  int           fsFeatureCountMFS = 0;

  ProcessorCycles  fsCpuCyclesByProcBFS;
  ProcessorCycles  fsCpuCyclesByProcMFA;

  double  cParamMFS        = 0.0;
  double  gammaParamMFS    = 0.0;
  double  probParamMFS     = 0.0;


  {
    // Lets get the ALL Classes Count 
    SetMajorStep (msMfsFeatureSelection);

    delete  binClasses;
    binClasses  = CreateInitialBinaryClassList ();

    resultConfigFileName = configFileNameMfsTunedFeaturesSel;
    delete  config;
    config = new TrainingConfiguration2 (fileDesc, resultConfigFileName, log, false);

    fsFeatureCountMFS = config->GetFeatureNums ().NumSelFeatures ();
    cParamMFS         = config->C_Param ();
    gammaParamMFS     = config->Gamma   ();
    probParamMFS      = config->A_Param ();

    BinaryClassList::iterator  idx;
    for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
    {
      BinaryClassPtr  bc = *idx;

      ProcessorPtr  p = new Processor (this, false, 999999, fileDesc, stFeatureSelectionSearch, bc->Class1 (), bc->Class2 (), smNULL, fsDone);
      if  (p != NULL)
      {
        jobIdsByFeatureCountAllClasses = p->GetSmallestJobIdByFeatureCount ();
        cpuCyclesByFeatureCountAllClasses = p->GetCpuCyclesByFeatureCount ();
        fsCpuCyclesMFS = p->TotalCpuTimeUsed ();
        fsCpuCyclesByProcMFA = p->CpuCyclesByProcId ();
        featureCountBeamSearchStartMFS = p->BeamSearchFeatureCount ();
      }
      delete  p;
      p = NULL;
    }
  }

  resultConfigFileName = configFileNameBfsFeaturesSelTuned;
  delete  config;
  config = new TrainingConfiguration2 (fileDesc, resultConfigFileName, log, false);

  SetMajorStep (msBfsParmTuningPost);

  delete  binClasses;
  binClasses  = CreateInitialBinaryClassList ();

  double  cParam     = 0.0;
  double  gammaParam = 0.0;
  double  probParam  = 0.0;
  int     featureCount = 0;

  BinaryClassList::iterator  idx;
  for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;

    MLClassConstPtr  class1 = bc->Class1 ();
    MLClassConstPtr  class2 = bc->Class2 ();

    hdrLine1 << "\t" << bc->Class1Name ();
    hdrLine2 << "\t" << bc->Class2Name ();

    {
      // Get Class Counts.
      int  class1Count = trainingData->GetClassCount (class1);
      int  class2Count = trainingData->GetClassCount (class1);
      int  totalPairCount = class1Count + class2Count;
      exampleCountPerClassPair.push_back (totalPairCount);
      exampleCountTotal += totalPairCount;
    }


    BinaryClassParmsPtr  bcp = config->BinaryClassParms (bc->Class1 (), bc->Class2 ());
    if  (bcp)
    {
      cParam       = bcp->C ();
      gammaParam   = bcp->Param ().Gamma ();
      probParam    = bcp->AParam ();
      featureCount = bcp->SelectedFeatures ().NumSelFeatures ();
    }
    else
    {
      cParam     = 0.0;
      gammaParam = 0.0;
      probParam  = 0.0;
      featureCount = 0;
    }

    ProcessorPtr  p = new Processor (this, true, 999999, fileDesc, stFeatureSelectionSearch, bc->Class1 (), bc->Class2 (), smNULL, fsDone);
    if  (p == NULL)
    {
      jobIdsByFeatureCountBinaryClasses.push_back    (zeroCounts);
      cpuCyclesByFeatureCountBinaryClasses.push_back (zeroCountsDouble);
      fsCpuCyclesByClassPair.push_back    (0.0);
      featureCountBeamSearchStartBFS.push_back       (0.0);
    }
    else
    {
      jobIdsByFeatureCountBinaryClasses.push_back    (p->GetSmallestJobIdByFeatureCount ());
      cpuCyclesByFeatureCountBinaryClasses.push_back (p->GetCpuCyclesByFeatureCount  ());
      fsCpuCyclesByClassPair.push_back    (p->TotalCpuTimeUsed ());
      fsCpuCyclesByProcBFS.AddCycles (p->CpuCyclesByProcId ());
      featureCountBeamSearchStartBFS.push_back       (p->BeamSearchFeatureCount ());
    }


    fsFeatureCountByClassPair.push_back    ((double)featureCount);
    parnCParamByClassPairBFS.push_back     (cParam);
    parnGammaParamByClassPairBFS.push_back (gammaParam);
    parnProbParamByClassPairBFS.push_back  (probParam);

    delete  p;
    p = NULL;

    cout  << jobIdsByFeatureCountBinaryClasses.size () << "\t" << bc->Class1Name () << "\t" << bc->Class2Name ();
  }
  


  //************************************************
  //*   Get CPU Stats for parameter Serach Jobs.   *
  //************************************************
  double  parmCpuCyclesMFS = 0.0;
  double  parmJobsMFS      = 0.0;
  ProcessorCycles  parmCpuCyclesByProcMFS;
    
  {
    // Lets get the ALL Classes Count 
    SetMajorStep (msMfsParmTuningPre);
    delete  binClasses;
    binClasses  = CreateInitialBinaryClassList ();
    BinaryClassList::iterator  idx;
    for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
    {
      BinaryClassPtr  bc = *idx;

      ProcessorPtr  p = new Processor (this, false, 999999, fileDesc, stParameterSearch, bc->Class1 (), bc->Class2 (), smNULL, fsDone);
      if  (p != NULL)
      {
        parmCpuCyclesMFS = p->TotalCpuTimeUsed ();
        parmJobsMFS      = p->NextJobId ();
        parmCpuCyclesByProcMFS = p->CpuCyclesByProcId ();
      }

      delete  p;
      p = NULL;
    }
  }

  ProcessorCycles  parmCpuCyclesByProcBFS;
  VectorDouble  parnCpuCyclesByClassPairBFS;
  VectorDouble  parmJobsByClassPairBFS;


  resultConfigFileName = configFileNameBfsFeaturesSelTuned;
  SetMajorStep (msBfsParmTuningPost);

  delete  binClasses;
  binClasses  = CreateInitialBinaryClassList ();


  for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;
    ProcessorPtr  p = new Processor (this, true, 999999, fileDesc, stParameterSearch, bc->Class1 (), bc->Class2 (), smNULL, fsDone);
    if  (p == NULL)
    {
      parnCpuCyclesByClassPairBFS.push_back    (0.0);
      parmJobsByClassPairBFS.push_back (0.0);
    }
    else
    {
      parnCpuCyclesByClassPairBFS.push_back  (p->TotalCpuTimeUsed ());
      parmJobsByClassPairBFS.push_back (p->NextJobId ());
      parmCpuCyclesByProcBFS.AddCycles (p->CpuCyclesByProcId ());
    }

    delete  p;
    p = NULL;

    cout  << "Binary Parameters" << jobIdsByFeatureCountBinaryClasses.size () << "\t" << bc->Class1Name () << "\t" << bc->Class2Name ();
  }




  ofstream  r ("ProcessorUsageAnalysis.txt");


  {
    //***********************************************
    //*           Feature Count Results             *
    //***********************************************

    r << endl
      << "Jobs Processed By Feature Count" << endl
      << endl;

    r << "Feature" << "\t" << "All"      << hdrLine1 << endl;
    r << "Count"   << "\t" << "Classes"  << hdrLine2 << endl;

    for  (uint fc = 1;   fc <= (uint)fileDesc->NumOfFields ();  fc++)
    {
      r << fc
        << "\t" << jobIdsByFeatureCountAllClasses[fc];

      for  (uint comboIDX = 0; comboIDX < jobIdsByFeatureCountBinaryClasses.size ();  comboIDX++)
      {
        VectorInt&  counts = jobIdsByFeatureCountBinaryClasses[comboIDX];
        r << "\t";  
        if  (fc < counts.size ())
          r << counts[fc];
        else
          r << "***";
      }
      r << endl;
    }
  }

  {
    //*************************************
    //*    CPU Cycles by Feature Count    *
    //*************************************
    r << endl
      << endl
      << "Feature Selection  CPU Cycles by Feature Count" << endl
      << endl;

    r << "Feature" << "\t" << "All"      << hdrLine1 << endl;
    r << "Count"   << "\t" << "Classes"  << hdrLine2 << endl;

    for  (uint fc = 1;   fc <= (uint)fileDesc->NumOfFields ();  fc++)
    {
      r << fc
        << "\t" << cpuCyclesByFeatureCountAllClasses[fc];

      for  (uint comboIDX = 0; comboIDX < cpuCyclesByFeatureCountBinaryClasses.size ();  comboIDX++)
      {
        VectorDouble&  cpuCycles = cpuCyclesByFeatureCountBinaryClasses[comboIDX];
        r << "\t";
        if  (fc < cpuCycles.size ())
          r << cpuCycles[fc];
        else
          r << "***";
      }
      r << endl;
    }
  }

  

  double  total  = 0.0;
  double  mean   = 0.0;
  double  min    = 0.0;
  double  max    = 0.0;
  double  stdDev = 0.0;

  {
    //**********************************************************
    //*      Parameter Tuning    cycles by class pairs         *
    //**********************************************************

    r << endl << endl
      << "Parameter CPU Cycles." << endl
      << endl;
    r << "" << "\t" << "All"      << "\t"  << ""      << "\t" << "Std"  << "\t" << ""    << "\t" << ""    << hdrLine1  << endl;
    r << "" << "\t" << "Classes"  << "\t"  << "Mean"  << "\t" << "Dev"  << "\t" << "Min" << "\t" << "Max" << hdrLine2  << endl;

    CalcVectorStats (parnCpuCyclesByClassPairBFS, total, mean, stdDev, min, max);
    r << "CPU Secs" << "\t" << parmCpuCyclesMFS << "\t" << mean << "\t" << stdDev << "\t" << min << "\t" << max;
    for  (uint z = 0;  z < parnCpuCyclesByClassPairBFS.size ();  z++)
      r << "\t" << parnCpuCyclesByClassPairBFS[z];
    r << endl;

    CalcVectorStats (parmJobsByClassPairBFS, total, mean, stdDev, min, max);
    r << "NumOfJobs" << "\t" << parmJobsMFS << "\t" << mean << "\t" << stdDev << "\t" << min << "\t" << max;
    for  (uint z = 0;  z < parmJobsByClassPairBFS.size ();  z++)
      r << "\t" << parmJobsByClassPairBFS[z];
    r << endl;

      
    r << "C Param" << "\t" << cParamMFS << "\t" << "" << "\t" << "" << "\t" << "" << "\t" << "";
    for  (uint z = 0;  z < parnCParamByClassPairBFS.size ();  z++)
      r << "\t" << parnCParamByClassPairBFS[z];
    r << endl;

    r << "Gamma Param" << "\t" << gammaParamMFS << "\t" << "" << "\t" << "" << "\t" << "" << "\t" << "";
    for  (uint z = 0;  z < parnGammaParamByClassPairBFS.size ();  z++)
      r << "\t" << parnGammaParamByClassPairBFS[z];
    r << endl;

    r << "Prob Param" << "\t" << probParamMFS << "\t" << "" << "\t" << "" << "\t" << "" << "\t" << "";
    for  (uint z = 0;  z < parnProbParamByClassPairBFS.size ();  z++)
      r << "\t" << parnProbParamByClassPairBFS[z];
    r << endl;

    r << endl;
    r << endl;
  }


  {
    //*******************************************************
    //*    Feature Selection CPU Ussage by Class Pairs    *
    //*******************************************************

    r << endl 
      << endl
      << "Feature Selection  CPU Ussage by Class Pairs" << endl
      << endl;
    r << "" << "\t" << "All"      << "\t"  << ""      << "\t" << "Std"  << "\t" << ""    << "\t" << ""    << "\t" << "WtdClass" << hdrLine1  << endl;
    r << "" << "\t" << "Classes"  << "\t"  << "Mean"  << "\t" << "Dev"  << "\t" << "Min" << "\t" << "Max" << "\t" << "PairSize" << hdrLine2  << endl;

    CalcVectorStats (fsCpuCyclesByClassPair, total, mean, stdDev, min, max);
    r << "CPU Secs" << "\t" << fsCpuCyclesMFS << "\t" << mean << "\t" << stdDev << "\t" << min << "\t" << max << "\t" << "";
    for  (uint z = 0;  z < fsCpuCyclesByClassPair.size ();  z++)
      r << "\t" << fsCpuCyclesByClassPair[z];
    r << endl;
    
    double  featureCountWeighted = 0.0;
    CalcVectorStats (fsFeatureCountByClassPair, total, mean, stdDev, min, max);
    CalcClassPairWeighted (fsFeatureCountByClassPair, exampleCountPerClassPair, featureCountWeighted);
    r << "Feature Count" << "\t" << fsFeatureCountMFS << "\t" << mean << "\t" << stdDev << "\t" << min << "\t" << max <<  "\t" << featureCountWeighted;
    for  (uint z = 0;  z < fsFeatureCountByClassPair.size ();  z++)
      r << "\t" << fsFeatureCountByClassPair[z];
    r << endl;


    double  beamSearchStartedWeightedBFS = 0.0;
    CalcVectorStats (featureCountBeamSearchStartBFS, total, mean, stdDev, min, max);
    CalcClassPairWeighted (featureCountBeamSearchStartBFS, exampleCountPerClassPair, beamSearchStartedWeightedBFS);
    r << "BeamSearchStart" << "\t" << featureCountBeamSearchStartMFS << "\t" << mean << "\t" << stdDev << "\t" << min << "\t" << max << "\t" << beamSearchStartedWeightedBFS;
    for  (uint z = 0;  z < featureCountBeamSearchStartBFS.size ();  z++)
      r << "\t" << featureCountBeamSearchStartBFS[z];
    r << endl;

    r << endl;
    r << endl;
  }


  {
    //*****************************************************
    //*      Parameter Tuning CPU Usage by processor      *
    //*****************************************************

    r << endl << endl << endl
      << "MFS Parameter All Classes  Cycles by ProcessorID"  << endl
      << endl;
    parmCpuCyclesByProcMFS.Report (r);

    r << endl << endl;

    r << endl << endl << endl
      << "BFS  Parameter Binary Class Combos  Cycles by ProcessorID"  << endl
      << endl;
    parmCpuCyclesByProcBFS.Report (r);
  }


  {
    r << endl << endl << endl
      << "MFS  Feature Selection All Classes  Cycles by ProcessorID"  << endl
      << endl;
    fsCpuCyclesByProcMFA.Report (r);

    r << endl << endl;

    r << endl << endl << endl
      << "BFS  Feature Selection Binary Class Combos  Cycles by ProcessorID"  << endl
      << endl;
    fsCpuCyclesByProcBFS.Report (r);
    r << endl << endl;
  }


  r.close ();

  delete  binClasses;  binClasses = NULL;

  log.Level (10) << endl << endl << endl
                 << "FeatureSelection::GetProcessingStats      *** ready to generate Final Reports ***" << endl
                 << endl;
  return;
}  /* GetProcessingStats */





void  FeatureSelection::ProcessRestart ()
{
  log.Level (10) << "FeatureSelection::ProcessRestart" << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl << endl << endl 
                   << "FeatureSelection::ProcessRestart      *** QUIT specified but not Status file exists ***" << endl
                   << endl;
    exit (0);
  }

  fclose (statusFile);

  BinaryClassListPtr  binClasses = StatusFileLoad ();
  
  LoadRunTimeData ();

  quitRunning = false;

  BinaryClassList::iterator  idx;
  for  (idx = binClasses->begin ();   idx != binClasses->end ();  idx++)
  {
    BinaryClassPtr bc = *idx;
    {
      bc->Status (cpNotStarted);
      bc->NumProcessors (0);
      ProcessorPtr  p = NULL;

      if  (bc->ResultType () == frtNULL)
      {
        p = new Processor (this,
                           byBinaryClasses,
                           -1,
                           fileDesc,
                           searchType,
                           bc->Class1 (),
                           bc->Class2 (),
                           smBestCaseNext,
                           fsStarted
                          );
      }
      else
      {
        p = new Processor (this,
                           -1,
                           bc,
                           fileDesc,
                           fsStarted
                          );
      }
          
      if  ((p->Status () == fsStarted)  ||  (p->QuitRunning ())  ||  (p->Status () == fsNotStarted))
        p->ProcessRestart ();
      delete  p;  p = NULL;
    }
  }

  StatusFileUpdate (binClasses);

  EndBlock ();
  delete  binClasses;  binClasses = NULL;

  log.Level (10) << endl << endl << endl
                 << "FeatureSelection::ProcessRestart      *** All Combos Set For Restart ***" << endl
                 << endl;
  exit (0);
}  /* ProcessRestart */




void  FeatureSelection::SetUpToBruitSvmSearch ()
{
  log.Level (10) << "FeatureSelection::SetUpToBruitSvmSearch" << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl << endl << endl 
                   << "FeatureSelection::SetUpToBruitSvmSearch      *** There is no Status File ***" << endl
                   << endl;
    return;
  }

  fclose (statusFile);

  BinaryClassListPtr  binClasses = StatusFileLoad ();

  resultConfigFileName = configFileNameSpecified;
  SetMajorStep (msGenerateBruitSvmSearch);

  delete  binClasses;
  binClasses = NULL;

  binClasses  = CreateInitialBinaryClassList ();

  configFileName = resultConfigFileName;
  numJobsAtATime  = 1;
  numProcPerCombo = 30;
  quitRunning = false;

  StatusFileUpdate (binClasses);

  delete  binClasses;  binClasses = NULL;

  log.Level (10) << endl << endl << endl
                 << "SetUpToBruitSvmSearch      *** Ready to perform bruit SVM search ***" << endl
                 << endl;
  return;
}  /* SetUpToBruitSvmSearch */






void  FeatureSelection::SetUpToGenerateFinalReports ()
{
  log.Level (10) << "FeatureSelection::SetUpToGenerateFinalReports" << endl;

  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    log.Level (-1) << endl << endl << endl 
                   << "FeatureSelection::SetUpToGenerateFinalReports      *** There is no Status File ***" << endl
                   << endl;
    return;
  }

  fclose (statusFile);

  BinaryClassListPtr  binClasses = StatusFileLoad ();

  resultConfigFileName = configFileNameBfsFeaturesSelTuned;
  SetMajorStep (msGenerateFinalResults);

  delete  binClasses;
  binClasses = NULL;

  binClasses  = CreateInitialBinaryClassList ();

  BinaryClassList::iterator  idx;
  for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;
    KKStr  summaryResultsFileName = bc->FinalResultsFileName ();
    KKStr  lockFileName   = osGetRootName (summaryResultsFileName) + ".Lock";
    KKStr  statusFileName = osGetRootName (summaryResultsFileName) + ".Status";

    osDeleteFile (summaryResultsFileName);
    osDeleteFile (lockFileName);
    osDeleteFile (statusFileName);
  }
  

  configFileName = resultConfigFileName;
  numJobsAtATime  = 1;
  numProcPerCombo = 30;
  quitRunning = false;

  StatusFileUpdate (binClasses);

  delete  binClasses;  binClasses = NULL;

  log.Level (10) << endl << endl << endl
                 << "FeatureSelection::SetUpToGenerateFinalReports      *** ready to generate Final Reports ***" << endl
                 << endl;
  return;
}  /* SetUpToGenerateFinalReports */





void  FeatureSelection::ProcessStatusFile (int     argc,  
                                           char**  argv
                                          )
{
  Block ();

  if  (argc == 2)
  {
    KKStr  parm1 (argv[1]);
    parm1.Upper ();

    if  (parm1.EqualIgnoreCase ("-GenerateFinalReports")  ||  parm1.EqualIgnoreCase ("-GFR"))
    {
      SetUpToGenerateFinalReports ();
      EndBlock ();
      exit (0);
    }
    
    if  (parm1.EqualIgnoreCase ("-FeatureImpactSummary")  ||  parm1.EqualIgnoreCase ("-FIS"))
    {
      FeatureImpactSummary ();
      EndBlock ();
      exit (0);
    }
    
    if  (parm1.EqualIgnoreCase ("-GenerateBruitSvmSearch"))
    {
      SetUpToBruitSvmSearch ();
      EndBlock ();
      exit (0);
    }

    if  (parm1.EqualIgnoreCase ("-FINALREPORT"))
    {
      BinaryClassListPtr  binClasses = StatusFileLoad ();
      LoadRunTimeData ();
      FinalReport ();
      EndBlock ();
      delete  binClasses;
      exit (-1);
    }

    if  (parm1 == "-LOCK")
    {
      cout << endl
           << endl
           << endl
           << "   *** Lock File Created. ***" << endl
           << endl;
      exit (0);
    }

    if  ((parm1 == "-QUIT")  ||  (parm1 == "-STOP"))
    {
      SetQuitRunningFlag ();  // We will not return fron this call
      EndBlock ();
      exit (0);
    }

    if  (parm1 == "-RESTART")
    { 
      ProcessRestart ();     // We will not return fron this call
      EndBlock ();
      exit (0);
    }

    if  (parm1.EqualIgnoreCase ("-FEAATURECOUNT")  ||  parm1.EqualIgnoreCase ("-ProcessingStats"))
    {
      justGettingStats = true;
      GetProcessingStats ();     // We will not return fron this call
      EndBlock ();
      exit (0);
    }



    if  (parm1 == "-RESULTCONFIG")
    {
      BinaryClassListPtr  binClasses = StatusFileLoad ();
      if  (!AllBinaryClassesAreDone (binClasses))
      {
        log.Level (-1) << endl << endl << endl
                       << "FeatureSelection::ProcessStatusFile      *** error ***" << endl
                       << endl
                       << "             *** Not all Binary Combos have been processed." << endl
                       << endl;
      }
      else
      {
        resultConfigFileName =  osRemoveExtension (configFileName) + "_Tuned.cfg";
        StatusFileUpdate (binClasses);
      }

      delete  binClasses;  binClasses = NULL; 
      EndBlock ();
      exit (0);
    }

    log.Level (-1) << "FeatureSelection::ProcessStatusFile    Did not recognize command[" << argv[1] << "]" << endl; 
    DisplayCommandUsage ();
    EndBlock ();
    exit (0);
  }


  FILE*  statusFile = osFOPEN (statusFileName.Str (), "r");
  if  (!statusFile)
  {
    //  We are doing our initial StartUp.  So lets set up the Status file.
    PerformInitialSetup (argc, argv);
  }

  else
  {
    fclose (statusFile);
    BinaryClassListPtr  binClasses = StatusFileLoad ();
    delete  binClasses;
    binClasses = NULL;
    LoadRunTimeData ();
  }  


  //resultConfigFileName = "etpB_TunedParameters.cfg";
  EndBlock ();
}  /* ProcessStatusFile */




TrainingConfiguration2Ptr  FeatureSelection::OpenConfig (const KKStr&  fn)
{
  log.Level (30) << "FeatureSelection::OpenConfig   fn[" << fn << "]" << endl;

  if  (fn.Empty ())
  {
    log.Level (-1) << endl << endl
      << "FeatureSelection::OpenConfig     ***ERROR***     FileName is Blank." << endl
      << endl;
    return NULL;
  }

  KKStr  effectiveName = TrainingConfiguration2::GetEffectiveConfigFileName (fn);
  if  (!osFileExists (effectiveName))
  {
    log.Level (-1) << endl << endl
      << "FeatureSelection::OpenConfig     ***ERROR***     FileName[" << fn << "] does not exist." << endl
      << "                                    or effective FileName[" << effectiveName << "]"      << endl
      << endl;
    return NULL;
  }

  TrainingConfiguration2Ptr   c = new TrainingConfiguration2 (fileDesc, fn, log, false);
  if  (!c->FormatGood ())
  {
    log.Level (-1) << endl
                   << endl
                   << "FeatureSelection::OpenConfig    ***ERROR***" << endl
                   << endl
                   << "   Could not load Config File[" << configFileName << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  delete  mlClasses;  mlClasses = NULL;
  mlClasses = c->ExtractClassList ();

  return  c;
}  /* OpenConfig */





FinalResultsPtr  FeatureSelection::RunAValidation (const KKStr&  summaryResultsFileName,
                                                   double        totalCpuSecsSearching,
                                                   double        totalProcessingTime,
                                                   double        elapsedTime,
                                                   double        longestPath, 
                                                   int           jobCount
                                                  )
{
  log.Level (10) << "FeatureSelection::RunAValidation   Description    [" << summaryResultsFileName << "]" << endl;
  log.Level (10) << "                                   ConfigFileName [" << configFileName         << "]" << endl;

  FinalResultsPtr  fr = NULL;
  try
  {
    fr = new FinalResults (summaryResultsFileName, 
                           totalCpuSecsSearching, 
                           totalProcessingTime, 
                           elapsedTime, 
                           longestPath,
                           jobCount,
                           log
                          ); 
  }
  catch  (exception&  e)
  {
    fr = NULL;
    log.Level (-1) << endl 
      << "FeatureSelection::RunAValidation   ***ERROR***   Exception Thrown: " << e.what () << endl
      << endl;
  }

  return  fr;
}  /* RunAValidation */






void  ReportDataSpecs (ostream&              r, 
                       const KKStr&          fileName, 
                       FeatureVectorListPtr  data
                      )
{
  r << endl;
  r << "File       [" << fileName                      << "]" << endl
    << "TimeStamp  [" << osGetFileDateTime (fileName)  << "]" << endl
    << "FileSize   [" << osGetFileSize     (fileName)  << "]" << endl
    << endl;

  data->PrintClassStatistics (r);
  r << endl 
    << endl;

}  /* ReportDataSpecs */



void  FeatureSelection::LoadValidationData ()
{
  if  (validationData)
  {
    delete  validationData;
    validationData = NULL;
  }

  if  (!validationDataFileName.Empty ())
  {
    bool  successful  = false;
    bool  changesMade = false;

    validationData = driver->LoadFeatureFile (validationDataFileName, *mlClasses, -1, cancelFlag, successful, changesMade, log);
    if (!successful)
    {
      log.Level (-1) << endl
                     << " *****  Can not load Validate Data File [" << validationDataFileName << "]  *****" 
                     << endl;
      EndBlock ();
    	osWaitForEnter ();
      exit (-1);
    }

    if  (normalizationParms)
      normalizationParms->NormalizeImages (validationData);
  }
}  /* LoadValidationData */



void  FeatureSelection::FinalReport ()
{
  log.Level (10) << "FeatureSelection::FinalReport" << endl;


  KKStr  reportFileName = osRemoveExtension (configFileNameSpecified) + "_FinalReport.html";
  HTMLReport r (reportFileName, "Feature Selection - Final Report", HTMLReport::Center);

  r << "<center>" << endl
    << endl;

  r << "<h1>Run Time Parameters</h1>" << endl
    << "<table align=\"center\" border=\"2\" cellpadding=\"4\">"            << endl
    << "  <caption>" << "Run Time Parameters" << "</caption>" << endl
    << "  <tr><td>Run Time</td>"               << "<td>" << osGetLocalDateTime ()                         << "</td></tr>" << endl
    << "  <tr><td>Train File</td>"             << "<td>" << trainingDataFileName                          << "</td></tr>" << endl
    << "  <tr><td>Test  File</td>"             << "<td>" << testDataFileName                              << "</td></tr>" << endl
    << "  <tr><td>Validation File</td>"        << "<td>" << validationDataFileName                        << "</td></tr>" << endl
    << "  <tr><td>Processing Order</td>"       << "<td>" << ProcessingOrderToStr (processingOrder)        << "</td></tr>" << endl
    << "  <tr><td>Number of Folds</td>"        << "<td>" << numOfFolds                                    << "</td></tr>" << endl
    << "  <tr><td>Expansion Limit</td>"        << "<td>" << expansionLimit                                << "</td></tr>" << endl
    << "  <tr><td>Initial Features</td>"       << "<td>" << initialFeaturesStr                            << "</td></tr>" << endl
    << "  <tr><td>Grading Method</td>"         << "<td>" << GradingMethodToStr (gradingMethod)            << "</td></tr>" << endl
    << "  <tr><td>Current Directory</td>"      << "<td>" << osGetCurrentDirectory ()                      << "</td></tr>" << endl
    << "  <tr><td>Parm Sel Criteria</td>"      << "<td>" << JobSelectionCriteriaToStr (paramSelCriteria)  << "</td></tr>" << endl
    << "  <tr><td>Feature Criteria</td>"       << "<td>" << FeatureCriteriaToStr (featureCriteria)        << "</td></tr>" << endl
    << "  <tr><td>FeatureCountPrefSmall</td>"  << "<td>" << (featureCountPrefSmall ? "Yes" : "No")        << "</td></tr>" << endl
    << "  <tr><td>MinTrainExamples</td>"       << "<td>" << minTrainExamples                              << "</td></tr>" << endl
    << "  <tr><td>Random Splits Num</td>"      << "<td>" << randomSplitsNum                               << "</td></tr>" << endl
    << "</table>" << endl;

  r << "<h1>Data File Specs</h1>"                                        << endl
    << "<table align=\"center\" border=\"2\" cellpadding=\"4\">"         << endl
    << "  <caption>" << "Data File Specs" << "</caption>"                << endl
    << "  <thead>"                                                       << endl
    << "    <tr><th>Description</th><th>File Name</th><th>TimeStamp</th><th>File Size</th><th>Num Recs</th></tr>" << endl
    << "  </thead" << endl
    << "  <tbody>" << endl;

  r << "    <tr><td>Training Data" << "</td><td>" << trainingDataFileName << "</td><td>" << osGetFileDateTime (trainingDataFileName) << "</td><td>" << osGetFileSize (trainingDataFileName) << "</td><td>" << trainingData->QueueSize () << "</td>" << "</tr>" << endl;
  if  (testData)
    r << "    <tr><td>Test Data"     << "</td><td>" << testDataFileName     << "</td><td>" << osGetFileDateTime (testDataFileName)     << "</td><td>" << osGetFileSize (trainingDataFileName) << "</td><td>" << testData->QueueSize () << "</td>" << "</tr>" << endl;
  if  (validationData)
    r << "    <tr><td>Validation Data" << "</td><td>" << validationDataFileName << "</td><td>" << osGetFileDateTime (validationDataFileName) << "</td><td>" << osGetFileSize (trainingDataFileName) << "</td><td>" << validationData->QueueSize () << "</td>" << "</tr>" << endl;
  
  r << "  </tbody>" << endl
    << "</table>" << endl;

  r << "<h2>Training Data</h2>" << endl;
  trainingData->PrintClassStatisticsHTML (r.OStream ());
  if  (testData)
  {
    r << "<h2>Test Data</h2>" << endl;
    testData->PrintClassStatisticsHTML (r.OStream ());
  }

  if  (!validationData)
    LoadValidationData ();
  
  if  (validationData)
  {
    r << "<h2>Validation Data<br />Unseen Test Data</h2>" << endl;
    validationData->PrintClassStatisticsHTML (r.OStream ());
  }


  vector<FinalResultsPtr>  resultsSummary;

  double  cpuSecsUsedSearching  = 0.0;  // Total time spent doing search
  double  cpuSecsUsedProcessing = 0.0;  // Time spent evaluating nodes.
  double  elaspedTime           = 0.0;
  double  longestPath           = 0.0;
  int     jobCount              = 0;

  if  (processingOrder == poStandard)
  {
    resultsSummary.push_back (RunAValidation ("MFS-NoTuningAllFeaturesVote.txt", 0.0, 0.0, 0.0, 0.0, 0));
    resultsSummary.push_back (RunAValidation ("MFS-NoTuningAllFeaturesProb.txt", 0.0, 0.0, 0.0, 0.0, 0));

    cpuSecsUsedSearching = GetTotalCpuSecs (msMfsParmTuningPre, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("FeaturesAllParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("FeaturesAllParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msMfsFeatureSelection, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("FeaturesSelParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("FeaturesSelParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msBfsFeatureSelection, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("BinaryFeaturesSelVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("BinaryFeaturesSelProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
 
    cpuSecsUsedSearching = GetTotalCpuSecs (msBfsParmTuningPost, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("BinaryFeaturesSelParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("BinaryFeaturesSelParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
  }
 
  else if  (processingOrder == poHall)
  {
    cpuSecsUsedSearching   = 0.0;
    cpuSecsUsedProcessing  = 0.0;
    elaspedTime            = 0.0;
    longestPath            = 0.0;
    jobCount               = 0;

    resultsSummary.push_back (RunAValidation ("MFS-NoTuningAllFeaturesVote.txt", 0.0, 0.0, 0.0, 0.0, 0));
    resultsSummary.push_back (RunAValidation ("MFS-NoTuningAllFeaturesProb.txt", 0.0, 0.0, 0.0, 0.0, 0));

    cpuSecsUsedSearching = GetTotalCpuSecs (msMfsFeatureSelection, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("MFS-FeaturesSelVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("MFS-FeaturesSelProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msMfsParmTuningPost, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("MFS-FeaturesSelParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("MFS-FeaturesSelParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msBfsParmTuningPre, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msBfsFeatureSelection, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedFeaturesSelVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedFeaturesSelProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));

    cpuSecsUsedSearching = GetTotalCpuSecs (msBfsParmTuningPost, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount);
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedFeaturesSelParmsTunedVote.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
    resultsSummary.push_back (RunAValidation ("BFS-ParmsTunedFeaturesSelParmsTunedProb.txt", cpuSecsUsedSearching, cpuSecsUsedProcessing, elaspedTime, longestPath, jobCount));
  }


  r << "<h1>Results</h1>" << endl;
    
  r << "<table style=\"text-align:center; vertical-align:bottom\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Summary of Results\" >" << endl
    << "<caption>Summary of Results</caption>"  << endl
    << "  <thead>"                              << endl
    << "    <tr>" << FinalResults::HeaderLineHTML1 () << "</tr>" << endl
    << "    <tr>" << FinalResults::HeaderLineHTML2 () << "</tr>" << endl
    << "  </thead>"                             << endl
    << "  <tbody>"                              << endl;

 
  vector<FinalResultsPtr>::iterator  idx, idx2;
  for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
  {
    FinalResultsPtr  fr = *idx;
    if  (fr == NULL)
    {
      r << "    <tr>" << "No Final Results Generated"  << "</tr>" << endl;
    }
    else
    {
    r << "    <tr>" << fr->ToHTMLStr ()  << "</tr>" << endl;
  }
  }

  r << "  </tbody>" << endl
    << "</table>"   << endl; 

  r << "<br />" << endl
    << "<br />" << endl;


  // Test Statistics
  {
    KKStr  captionStr = "McNemars Test";

    r << "<table align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"\" >" << endl
      << "<caption>" << captionStr << "</caption>" << endl
      << "  <thead>"                               << endl;

    r << "    <tr>" << "<th>" << "&nbsp" << "</th>";
    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
      r << "<th>" << (*idx)->Description () << "</th>";
    r << "</tr>" << endl;
    r << "  </thead>"  << endl
      << "  <tbody>"   << endl;

    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
    {
      FinalResultsPtr  results1 = *idx;
      int  count1                  = results1->ValidationClassedCorrectlySize ();
      const bool*  classCorrectly1 = results1->ValidationClassedCorrectly ();

      r << "    <tr>" << "<td>" << results1->Description () << "</td>";
      for  (idx2 = resultsSummary.begin ();  idx2 != resultsSummary.end ();  idx2++)
      {
        FinalResultsPtr  results2 = *idx2;
        if  (results1 == results2)
        {
          r << "<td>&nbsp</td>";
        }
        else
        {
          int  count2                  = results2->ValidationClassedCorrectlySize ();
          const bool*  classCorrectly2 = results2->ValidationClassedCorrectly ();
          KKStr  mcNemarsTestStr = "";

          if  (count1 != count2)
          {
            mcNemarsTestStr << "Count1[" << count1 << "]!=Count2[" << count2 << "]";
          }
          else if  (classCorrectly1 == NULL)
          {
            mcNemarsTestStr = "classCorrectly1==NULL";
          }

          else if  (classCorrectly2 == NULL)
          {
            mcNemarsTestStr = "classCorrectly2==NULL";
          }

          else
          {
            float mcNemarsTest = McNemarsTest (count1, classCorrectly1, classCorrectly2);
            mcNemarsTestStr = StrFormatDouble (mcNemarsTest, "####0.00000");
          }

          r << "<td>" << mcNemarsTestStr << "</td>";
        }
      }
    }

    r << "  </tbody>" << endl;
    r << "</table>"   << endl;
  }

    r << "<br />" << endl
      << "<br />" << endl
      << endl;

  {
    KKStr  captionStr = "Paired T-Test";

    r << "<table align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"\" >" << endl
      << "<caption>" << captionStr << "</caption>" << endl
      << "  <thead>"                               << endl;

    r << "    <tr>" << "<th>" << "&nbsp" << "</th>";
    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
      r << "<th>" << (*idx)->Description () << "</th>";
    r << "</tr>" << endl;
    r << "  </thead>"  << endl
      << "  <tbody>"   << endl;

    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
    {
      FinalResultsPtr  results1 = *idx;

      r << "    <tr>" << "<td>" << results1->Description () << "</td>";
      for  (idx2 = resultsSummary.begin ();  idx2 != resultsSummary.end ();  idx2++)
      {
        FinalResultsPtr  results2 = *idx2;
        if  (results1 == results2)
        {
          r << "<td>&nbsp</td>";
        }
        else
        {
          float pairedTTest = PairedTTest (results1->RandomSplitAccuracies (), results2->RandomSplitAccuracies ());
          r << "<td>" << pairedTTest << "</td>";
        }
      }
    }

    r << "  </tbody>" << endl;
    r << "</table>"   << endl;
  }


  r << "<br />" << endl
    << "<br />" << endl
    << "<h2>" << "Validation Accuracies by Class" << "</h2>" << endl;
  FinalResults::PrintAccuraciesByClassSummary (resultsSummary, r, true);


  r << "<br />" << endl
    << "<br />" << endl
    << "<h2>" << "Random Split Accuracies by Class" << "</h2>" << endl;
  FinalResults::PrintAccuraciesByClassSummary (resultsSummary, r, false);


  {
    // Validation Results
    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
    {
      FinalResultsPtr  fr = *idx;

      ConfusionMatrix2Ptr  cm = fr->ValidationConfusionMatrix ();

      r << "<br />" << endl
        << "<br />" << endl
        << "<br />" << endl
        << "<br />" << endl
        << "<h2>" << "<a name=\"" << fr->LinkName () << "-VAL" << "\" />" << fr->Description () << " Validation" << "</h2>" << endl;

      fr->PrintConfigParametersValidationHTML (r);

      r << "<br />" << endl
        << "<br />" << endl
        << endl;
      if  (cm)
      {
        cm->PrintConfusionMatrixHTML (r.OStream ());

        r << "<br />" << endl
          << "<br />" << endl
          << "Predictied Probability Confusion Matrix" << endl
          << endl;
        cm->PrintConfusionMatrixAvgPredProbHTML (r.OStream ());

         r << "<br />" << endl
           << "<br />" << endl
           << "Accuracy by Probability by Class" << endl
           << endl;
        cm->PrintAccuracyByProbByClassHTML (r.OStream ());
      }
    }
  }


  {
    // random Split results.
    for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
    {
      FinalResultsPtr  fr = *idx;

      ConfusionMatrix2Ptr  cm = fr->ConfusionMatrix ();

      r << "<br />" << endl
        << "<br />" << endl
        << "<br />" << endl
        << "<br />" << endl
        << "<h2>" << "<a name=\"" << fr->LinkName () << "-RS" << "\" />" << fr->Description () << " Random Splits" << "</h2>" << endl;

      fr->PrintConfigParametersRandomSplitsHTML (r);

      r << "<br />" << endl
        << "<br />" << endl
        << endl;
      if  (cm)
      {
        cm->PrintConfusionMatrixHTML (r.OStream ());

        r << "<br />" << endl
          << "<br />" << endl
          << "Predictied Probability Confusion Matrix" << endl
          << endl;
        cm->PrintConfusionMatrixAvgPredProbHTML (r.OStream ());

        r << "<br />" << endl
          << "<br />" << endl
          << "Accuracy by Probability by Class" << endl
          << endl;
        cm->PrintAccuracyByProbByClassHTML (r.OStream ());
      }
    }
  }

  for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
    delete  (*idx);

  r << "</center>" << endl
    << endl;

  r.Close ();
}  /* FinalReport */





void  FeatureSelection::FeatureImpactSummary ()
{
  BinaryClassListPtr  binClasses = StatusFileLoad ();
  LoadRunTimeData ();

  MajorSteps  origMajorStep = majorStep;

  AccByFeatureSelListPtr  fsSumList = new AccByFeatureSelList (true);

  SetMajorStep (msMfsFeatureSelection);
  delete  config;  config = NULL;
  if  (!configFileNameMfsFeatureSelTuned.Empty ())
    config = OpenConfig (configFileNameMfsFeatureSelTuned); 
  else
    config = OpenConfig (configFileNameMfsTunedFeaturesSel);

  BinaryClassListPtr  binaryClasses = CreateInitialBinaryClassList ();
  FeatureImpactSummary (fsSumList, binaryClasses);
  delete  binaryClasses;  binaryClasses = NULL;

  SetMajorStep (msBfsFeatureSelection);
  delete  config;  config = NULL;
  config = OpenConfig (configFileNameBfsFeaturesSelTuned);  
  binaryClasses = CreateInitialBinaryClassList ();
  FeatureImpactSummary (fsSumList, binaryClasses);
  delete  binaryClasses;  binaryClasses = NULL;

  SetMajorStep (origMajorStep);

  fsSumList->SortByFeatureNumThenByClassCombo ();
  ofstream r ("FeatureImpact.txt");

  AccByFeatureSelList::iterator idx;
  for  (idx = fsSumList->begin ();  idx != fsSumList->end ();  ++idx)
  {
    AccByFeatureSelPtr  abfs = *idx;
    r << abfs->FeatureNum ()                       << "\t"
      << fileDesc->FieldName (abfs->FeatureNum ()) << "\t"
      << abfs->Class1Name ()                       << "\t"
      << abfs->Class2Name ()                       << "\t"
      << abfs->AvgAccGain ()                       << "\t"
      << abfs->StdDevAccGain ()                    << endl;
  }

  delete  fsSumList;    fsSumList = NULL;
  r.close ();
}  /* FeatureImpactSummary */




void  FeatureSelection::FeatureImpactSummary (AccByFeatureSelListPtr  fsSumList,
                                              BinaryClassListPtr      binaryClasses
                                             )
{
  BinaryClassList::iterator idx;
  for  (idx = binaryClasses->begin ();  idx != binaryClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;
    ProcessorPtr  processor = NULL;
    if  (bc->ResultType () == frtNULL)
    {
      processor = new Processor (this,
                                 byBinaryClasses,
                                 -1,
                                 fileDesc,
                                 searchType,
                                 bc->Class1 (), 
                                 bc->Class2 (), 
                                 smBestCaseNext, 
                                 fsDone
                                );
    }
    else
    {
      processor = new Processor (this,
                                 -1,
                                 bc,
                                 fileDesc,
                                 fsDone
                                );
    }

    FeatureImpactPtr  fi = processor->GetFeatureImpact ();
    if  (fi)
      fi->UpdateAccByFeatureSelList (fsSumList, bc->Class1 (), bc->Class2 ());
    delete  fi;  fi = NULL;

    delete  processor;  processor = NULL;
  }
}  /* FeatureImpactSummary */





ProcessorPtr  FeatureSelection::GetNextBinaryClassComboToRun ()
{
  log.Level (10) << "FeatureSelection::GetNextBinaryClassComboToRun" << endl;

  // We are going to search for the BinaryClass that has the least number of 
  // processors already assigned to it.  This way if there are more processes 
  // running than there are BinaryClasses to be processed we will first get all
  // of them running first with one processesor before we assign a second one.
  // 
  // This way I can specify a large number of processes per combo with out worrying
  // about one binary combo getting them all.

  int  processIdCreated = -1;

  Block ();

  BinaryClassListPtr  binClasses =  StatusFileLoad ();

  bool  allJobsAreDone = true;

  ProcessorPtr  processor = NULL;

  BinaryClassPtr  candidate = NULL;
  BinaryClassList::iterator  idx;

  for  (idx = binClasses->begin ();  idx != binClasses->end ();  idx++)
  {
    BinaryClassPtr  bc = *idx;
    
    if  (bc->Status () == cpNotStarted)
    {
      candidate = bc;
      break;
    }

    if  ((bc->Status () == cpStarted)  &&  (bc->NumProcessors () < numProcPerCombo))
    {
      if  (!candidate)
        candidate = bc;

      else if  (bc->NumProcessors () < candidate->NumProcessors ())
        candidate = bc;
    }
  }


  if  (candidate)
  {
    log.Level (10) << "GetNextBinaryClassComboToRun  Found Available BinaryClass "
                   << "Class1[" << candidate->Class1Name () <<"]  Class2[" << candidate->Class2Name () <<"]"
                   << "  NumProcessors[" << candidate->NumProcessors () << "]"
                   << endl;

    candidate->Status (cpStarted);
    candidate->IncrementNumProcessors ();

    processIdCreated = nextProcessorId;
    nextProcessorId++;
    StatusFileUpdate (binClasses);
  }

  EndBlock ();

  if  (candidate)
  {
    log.Level (10) << "GetNextBinaryClassComboToRun  Creating Processor " 
                   << "Class1[" << candidate->Class1Name () <<"]  Class2[" << candidate->Class2Name () <<"]"
                   << endl;

    if  (candidate->ResultType () == frtNULL)
    {
      // We are creating a process that will be doing searching.
      processor = new Processor (this,
                                 byBinaryClasses,
                                 processIdCreated,
                                 fileDesc,
                                 searchType,
                                 candidate->Class1 (), 
                                 candidate->Class2 (), 
                                 smBestCaseNext, 
                                 fsNotStarted
                                );
    }

    else
    {
      processor = new Processor (this,
                                 processIdCreated,
                                 candidate,
                                 fileDesc,
                                 fsNotStarted
                                );
    }
  }

  delete  binClasses;  binClasses = NULL;

  return  processor;
}  /* GetNextBinaryClassComboToRun */






void  FeatureSelection::ReduceProcessorCount (ProcessorPtr     processor,
                                              ClassPairStatus  status
                                             )
{
  log.Level (10) << "FeatureSelection::ReduceProcessorCount" << endl;

  Block ();

  BinaryClassListPtr  binClasses =  StatusFileLoad ();

  KKStr  classNames;
  if  (byBinaryClasses)
    classNames = processor->Class1Name () + "-" + processor->Class2Name ();
  else
    classNames = "AllClasses";

  bool  found = false;
  int  idx;
  for  (idx = 0;  idx < binClasses->QueueSize ();  idx++)
  {
    BinaryClassPtr bc = binClasses->IdxToPtr (idx);

    bool bcMatches = false;
    if  (processor ->ResultType () == frtNULL)
      bcMatches = ((bc->Class1 () == processor->Class1 ())  &&  (bc->Class2 () == processor->Class2 ()));
    else
      bcMatches = (bc->FinalResultsFileName () == processor->SummaryResultsFileName ());

    if  (bcMatches)
    {
      log.Level (30) << "ReduceProcessorCount  Found BinmaryClass and Flagging as done." << endl;

      // We found the Biary Class in question
      found = true;
      bc->DecrementNumProcessors ();
      bc->Status (status);
      break;
    }
  }

  if  (!found)
  {
    EndBlock ();
    log.Level (-1) << endl
                   << endl
                   << "FeatureSelection::ReduceProcessorCount    *** ERROR ***"  << endl
                   << endl
                   << "Could not locate Binary Class[" << classNames << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  StatusFileUpdate (binClasses);
  delete  binClasses;
  binClasses = NULL;
  EndBlock ();
  return;
}  /* ReduceProcessorCount */






//  Sets up FeatuerSelection to start processing the next Major Step.
void  FeatureSelection::SetUpForNextMajorStep (BinaryClassListPtr&  binaryClasses,
                                               double&              totalCpuTimeUsed
                                              )
{
  log.Level (10) << "FeatureSelection::SetUpForNextMajorStep" << endl;


  if  (this->processingOrder == poHall)
    SetUpForNextMajorStepHallOrder (binaryClasses, totalCpuTimeUsed);

  else if  (this->processingOrder == poStandard)
    SetUpForNextMajorStepStandardOrder (binaryClasses, totalCpuTimeUsed);

  else
  {
    log.Level (-1) << endl << endl
      << "FeatureSelection::SetUpForNextMajorStep    ***ERROR***    Inalid Processing Order Defined." << endl
      << endl;
    exit (-1);
  }
}  /* SetUpForNextMajorStep */




/**
 @code
 This is called when a major step is cmpleted; and we want to set up for the next Major step.
 1) Build the Configuration file using the results from the current MaorSetp applied to the prev configuraion file.
 2) Set up for the next major step.
 */
void  FeatureSelection::SetUpForNextMajorStepStandardOrder (BinaryClassListPtr&  binaryClasses,
                                                            double&              totalCpuTimeUsed
                                                           )
{
  log.Level (10) << "FeatureSelection::SetUpForNextMajorStepStandardOrder" << endl;


  totalCpuTimeUsed = 0.0;

  switch  (majorStep)
  {
  case  msMfsParmTuningPre:
    {
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msMfsParmTuningPre'" << endl;
      resultConfigFileName = configFileNameMfsTuned;
      BuildResultConfigFile (configFileNameSpecified, totalCpuTimeUsed, featureCriteria);
      delete  binaryClasses;  binaryClasses = NULL;
      SetMajorStep (msMfsFeatureSelection);
      binaryClasses  = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
    }
    break;


  case  msMfsFeatureSelection:
    {
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msMfsFeatureSelection'" << endl;
      resultConfigFileName = configFileNameMfsTunedFeaturesSel;
      BuildResultConfigFile (configFileNameMfsTuned, totalCpuTimeUsed, featureCriteria);
      SetMajorStep (msBfsFeatureSelection);
      delete  binaryClasses;  binaryClasses = NULL;
      binaryClasses = CreateInitialBinaryClassList ();

      // We want the next step to start with the configuration file from 'msMfsParmTuningPre'
      // This way we start with all features selected.
      configFileName = configFileNameMfsTuned;
      numJobsAtATime = 10;
      numProcPerCombo = 20;
    }
    break;


  case  msBfsFeatureSelection:
    {
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msBfsFeatureSelection'" << endl;

      resultConfigFileName = configFileNameBfsFeaturesSel;
      BuildResultConfigFile (configFileNameMfsTuned, totalCpuTimeUsed, featureCriteria);

      delete  binaryClasses;  binaryClasses = NULL;
      SetMajorStep (msBfsParmTuningPost);
      binaryClasses = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
    }
    break;


  case  msBfsParmTuningPost:
    { 
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msBfsParmTuningPost'" << endl;

      resultConfigFileName = configFileNameBfsFeaturesSelTuned;
      BuildResultConfigFile (configFileNameBfsFeaturesSel, totalCpuTimeUsed, featureCriteria);
      SetMajorStep (msGenerateFinalResults);
      delete  binaryClasses;
      binaryClasses  = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
      numJobsAtATime  = 1;
      numProcPerCombo = 30;
    }
    break;


  case  msGenerateFinalResults:
    { 
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msGenerateFinalResults'" << endl;
      resultConfigFileName = configFileNameBfsFeaturesSelTuned;
      // KAK 2010-07-11   I do not believe there is any reason to build a coniguration file at this point.
      //BuildResultConfigFile (totalCpuTimeUsed, featureCriteria, BinaryCombos);
      FinalReport ();
      SetMajorStep (msDone);
      numJobsAtATime  = 1;
      numProcPerCombo = 30;
    }
    break;

  case  msGenerateBruitSvmSearch:
    { 
      log.Level (10) << "SetUpForNextMajorStepStandardOrder    'msGenerateBruitSvmSearch'" << endl;
      SetMajorStep (msDone);
      numJobsAtATime  = 1;
      numProcPerCombo = 30;
    }
    break;
      }
}  /* SetUpForNextMajorStepStandardOrder */






//  Sets up FeatuerSelection to start processing the next Major Step.
void  FeatureSelection::SetUpForNextMajorStepHallOrder (BinaryClassListPtr&  binaryClasses,
                                                        double&              totalCpuTimeUsed
                                                       )
{
  log.Level (10) << "FeatureSelection::SetUpForNextMajorStepHallOrder" << endl;

  totalCpuTimeUsed = 0.0;

  switch  (majorStep)
  {
  case  msMfsFeatureSelection:
    {
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msMfsFeatureSelection'" << endl;
      resultConfigFileName = configFileNameMfsFeaturesSel;
      BuildResultConfigFile (configFileNameSpecified, totalCpuTimeUsed, featureCriteria);
      delete  binaryClasses;  binaryClasses = NULL;
      SetMajorStep (msMfsParmTuningPost);
      binaryClasses = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
    }
    break;


  case  msMfsParmTuningPost:
    { 
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msMfsParmTuningPost'" << endl;
      resultConfigFileName = configFileNameMfsFeatureSelTuned;
      BuildResultConfigFile (configFileNameMfsFeaturesSel, totalCpuTimeUsed, featureCriteria);
      SetMajorStep (msBfsParmTuningPre);
      delete  binaryClasses;  binaryClasses = NULL;
      binaryClasses = CreateInitialBinaryClassList ();

      // We want the next step to start with the original config file
      configFileName = configFileNameSpecified;
      numJobsAtATime = 10;
      numProcPerCombo = 20;
    }
    break;


  case  msBfsParmTuningPre:
      {
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msBfsParmTuningPre'" << endl;

      resultConfigFileName = configFileNameBfsTuned;
      BuildResultConfigFile (configFileNameSpecified, totalCpuTimeUsed, featureCriteria);
      delete  binaryClasses;  binaryClasses = NULL;
      SetMajorStep (msBfsFeatureSelection);
      binaryClasses = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
      }
    break;


  case  msBfsFeatureSelection:
    { 
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msBfsFeatureSelection'" << endl;
      resultConfigFileName = configFileNameBfsTunedFeaturesSel;
      BuildResultConfigFile (configFileNameBfsTuned, totalCpuTimeUsed, featureCriteria);
      SetMajorStep (msBfsParmTuningPost);
      delete  binaryClasses;  binaryClasses = NULL;
      binaryClasses  = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
      numJobsAtATime = 10;
      numProcPerCombo = 20;
    }
    break;



  case  msBfsParmTuningPost:
    {
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msBfsParmTuningPost'" << endl;
      resultConfigFileName = configFileNameBfsTunedFeaturesSelTuned;
      BuildResultConfigFile (configFileNameBfsTunedFeaturesSel, totalCpuTimeUsed, featureCriteria);
      SetMajorStep (msGenerateFinalResults);
      delete  binaryClasses;  binaryClasses = NULL;
      binaryClasses  = CreateInitialBinaryClassList ();
      configFileName = resultConfigFileName;
      numJobsAtATime  = 1;
      numProcPerCombo = 30;
    }
    break;


  case  msGenerateFinalResults:
    { 
      log.Level (10) << "SetUpForNextMajorStepHallOrder    'msGenerateFinalResults'" << endl;
      resultConfigFileName = configFileNameBfsTunedFeaturesSelTuned;
      // KAK 2010-07-11   I do not believe there is ant reason to build a coniguration file at this point.
      //BuildResultConfigFile (totalCpuTimeUsed, featureCriteria, BinaryCombos);
      FinalReport ();
      SetMajorStep (msDone);
      numJobsAtATime  = 1;
      numProcPerCombo = 30;
    }
    break;

  }
}  /* SetUpForNextMajorStepHallOrder */






void  FeatureSelection::CheckForNextStep ()
{ 
  log.Level (10) << "FeatureSelection::CheckForNextStep" << endl;

  MajorSteps  lastStep = majorStep;

  bool  exitLoop       = false;
  bool  sleepForAWhile = false;

  while  (!exitLoop)
  {
    bool  goToSleep = false;

    Block ();
    {
      double  totalCpuTimeUsed = 0.0;
      BinaryClassListPtr  binaryClasses = StatusFileLoad ();
      if  ((quitRunning)  ||  (majorStep != lastStep))
        exitLoop = true;

      else if  (!AllBinaryClassesAreDone (binaryClasses))
      {
        goToSleep = true;
      }

      else 
      {
        SetUpForNextMajorStep (binaryClasses, totalCpuTimeUsed);
        StatusFileUpdate (binaryClasses);
        exitLoop = true;
      }
      delete  binaryClasses;  binaryClasses = NULL;
    }
    EndBlock ();

    if  (goToSleep)
    {
      log.Level (10) << "CheckForNextStep    Will sleep for 30 Seconds." << endl;
      osSleep (30.0);

      goToSleep = false;
    }
  }


  if  ((!quitRunning)  &&  (majorStep != msDone))
  {
    // Lets make sure our variables are set up for a new MajorStep.
    delete  config;  config = NULL;

    config = this->OpenConfig (configFileName);
    if  ((!config)  ||  (!config->FormatGood ()))
    {
      log.Level (-1) << endl
                     << endl
                     << "FeatureSelection::CheckForNextStep    ***ERROR***" << endl
                     << endl
                     << "   Could not load Config File[" << configFileName << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
    delete  mlClasses;  mlClasses = NULL;
    mlClasses = config->ExtractClassList ();
  }

  log.Level (10) << "FeatureSelection::CheckForNextStep    Exiting" << endl;
}  /* CheckForNextStep */




void  FeatureSelection::Run ()
{
  while  ((majorStep != msDone)  &&  (!quitRunning))
  {
    log.Level (10) << "FeatureSelection::Run   Starting Step[" << MajorStepToStr (majorStep) << "]" << endl;

    ProcessorPtr  processor = GetNextBinaryClassComboToRun ();
    while  ((processor)  &&  (!quitRunning)  &&  (majorStep != msDone))
    {
      processor->Run ();

      if  (processor->Status () == fsDone)
        ReduceProcessorCount (processor, cpDone);

      else if  (processor->Status () == fsStarted)
        ReduceProcessorCount (processor, cpStarted);

      else 
        ReduceProcessorCount (processor, cpNotStarted);

      delete  processor;  processor = NULL;
      if  (quitRunning)
        break;

      processor = GetNextBinaryClassComboToRun ();
    }

    if  (!quitRunning)
    {
      CheckForNextStep ();
    }
  }

  if  (quitRunning)
  {
    log.Level (9) << endl 
                  << "FeatureSelection::Run         QuitRunning flag has been set." << endl
                  << endl;
  }


  log.Level (10) << endl << endl << endl
                 << "FeatureSelection::Run    *** Terminating ***" << endl
                 << endl;
}  /* Run */







void  FeatureSelection::BuildResultConfigFile (const KKStr&       prevConfigFileName,
                                               double&            totalCpuTimeUsed,
                                               fcFeatureCriteria  featureCriteria
                                              )
{
  log.Level (10) << "FeatureSelection::BuildResultConfigFile" << endl;
  totalCpuTimeUsed = 0.0;
  BinaryClassListPtr  binClasses = StatusFileLoad ();

  if  (binClasses == NULL)
  {
    log.Level (-1) << endl << endl << endl
                   << "FeatureSelection::BuildResultConfigFile        ***ERROR***"  << endl
                   << endl
                   << "binaryClasses == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  int  idx;


  delete  config;
  config = new TrainingConfiguration2 (fileDesc, prevConfigFileName, log, false);
  if  (this->byBinaryClasses)
    config->MachineType (BinaryCombos);
  else
    config->MachineType (OneVsOne);

  if  ((majorStep == msMfsFeatureSelection)  &&   (processingOrder == poHall))
  {
    int  numOfFeatures = InitialFeatures ().NumSelFeatures ();
    config->C_Param (1.0);
    config->Gamma (1.0 / (double)numOfFeatures);
    config->A_Param (100.0);
  }


  for  (idx = 0;  idx < binClasses->QueueSize ();  idx++)
  {
    BinaryClassPtr bc = binClasses->IdxToPtr (idx);

    ProcessorPtr  processor = NULL;

    if  (bc->ResultType () == frtNULL)
    {
      // We are creating a process that will be doing searching.
      processor = new Processor (this, 
                                 byBinaryClasses,
                                 -1,
                                 fileDesc,
                                 searchType,
                                 bc->Class1 (), 
                                 bc->Class2 (), 
                                 smBestCaseNext, 
                                 fsDone
                                );
    }

    else
    {
      processor = new Processor (this,
                                 -1,
                                 bc,
                                 fileDesc,
                                 fsDone
                                );
    }

    totalCpuTimeUsed += processor->TotalCpuTimeUsed ();

    FeatureNumList  bestFeatures (fileDesc);

    if  (searchType == stFeatureSelectionSearch)
    {
      bestFeatures = processor->SelectBestFeatures ();
      if  (processor->ByBinaryClasses ())
      {
        if  ((trainingData->NumEntriesOfAGivenClass (bc->Class1 ()) < minTrainExamples)  ||
             (trainingData->NumEntriesOfAGivenClass (bc->Class2 ()) < minTrainExamples)
            )
        {
          config->SetFeatureNums (bc->Class1 (), bc->Class2 (), InitialFeatures (), -1);
        }
        else
        {
        config->SetFeatureNums (bc->Class1 (), bc->Class2 (), bestFeatures, -1);
      }
      }
      else
      {
        config->SetFeatureNums (bestFeatures);
    }
    }


    else if  (searchType == stParameterSearch)
    {
      float   aParm     = 0;
      double  cParm     = 0.0;
      double  gammaParm = 0.0;
      processor->GetBestParametersFromTestResults (cParm, gammaParm, aParm);

      if  (processor->ByBinaryClasses ())
      {
        FeatureNumList features = config->GetFeatureNums (bc->Class1 (), bc->Class2 ());

        ModelParamPtr  modelParam = config->ModelParameters ();
        SVMparamPtr  svmParam = NULL;

        svm_parameter  parameters;
        
        if  (modelParam->ModelParamType () == ModelParam::mptOldSVM)
        {
          svmParam = dynamic_cast<ModelParamOldSVMPtr> (modelParam)->SvmParameters ();
          if  (svmParam)
           parameters = svmParam->GetParamtersToUseFor2ClassCombo (bc->Class1 (), bc->Class2 ())->Param ();
        }

        parameters.A     = aParm;
        parameters.gamma = gammaParm;
        parameters.C     = cParm;

        config->SetBinaryClassFields (bc->Class1 (), bc->Class2 (),
                                      parameters,
                                      features,
                                      1.0f
                                     );
      }
      else
      {
        config->C_Param (cParm);
        config->Gamma   (gammaParm);
        config->A_Param (aParm);
      }
    }

    delete  processor;  processor = NULL;
  }

  delete  binClasses;  binClasses = NULL;
  

  config->Save (resultConfigFileName);
  return;
}  /* BuildResultConfigFile */




double   FeatureSelection::GetTotalCpuSecs (MajorSteps  step,
                                            double&     totalProcessingTime,
                                            double&     elapsedClockTime,
                                            double&     longestPath,
                                            int&        jobCount
                                           )
{
  MajorSteps  origMajorStep = majorStep;
  SetMajorStep (step);
  BinaryClassListPtr  binaryClasses = CreateInitialBinaryClassList ();
  double  totalCpuSecsUsed = GetTotalCpuSecs (binaryClasses, totalProcessingTime, elapsedClockTime, longestPath, jobCount);
  delete  binaryClasses;  binaryClasses = NULL;
  SetMajorStep (origMajorStep);
  return  totalCpuSecsUsed;
}  /* GetTotalCpuSecs */




double  FeatureSelection::GetTotalCpuSecs (BinaryClassListPtr  binClasses,
                                           double&             totalProcessingTime,
                                           double&             elapsedClockTime,
                                           double&             longestPath,
                                           int&                jobCount
                                          )
{
  log.Level (10) << "FeatureSelection::GetTotalCpuSecs" << endl;
  if  (binClasses == NULL)
  {
    log.Level (-1) << endl << endl << endl
                   << "FeatureSelection::GetTotalCpuSecs        ***ERROR***"  << endl
                   << endl
                   << "binClasses == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  longestPath = 0.0;
  jobCount    = 0;

  ProcessorCycles  cyclesByProcId;

  DateTime  startClockTime = DateTime (9999, 12, 31, 32, 59, 59);
  DateTime  endClockTime   = DateTime (0,     0,  0,  0,  0,  0);

  ofstream cpuSecReport ("GetTotalCpuSecs", ios::app);


  double  totalCpuTimeUsed = 0.0;
  
  totalProcessingTime = 0.0;

  int  idx;

  for  (idx = 0;  idx < binClasses->QueueSize ();  idx++)
  {
    BinaryClassPtr bc = binClasses->IdxToPtr (idx);

    ProcessorPtr  processor = NULL;

    if  (bc->ResultType () == frtNULL)
    {
      // We are creating a process that will be doing searching.
      processor = new Processor (this, 
                                 byBinaryClasses,
                                 -1,
                                 fileDesc,
                                 searchType,
                                 bc->Class1 (), 
                                 bc->Class2 (), 
                                 smBestCaseNext, 
                                 fsDone
                                );
    }
    else
    {
      processor = new Processor (this,
                                 -1,
                                 bc,
                                 fileDesc,
                                 fsDone
                                );
    }

    cyclesByProcId.AddCycles (processor->CpuCyclesByProcId ());
    jobCount += processor->NextJobId ();

    //KK 2010-06-09
    double  zed = processor->TotalCpuTimeUsed ();
    totalCpuTimeUsed    += zed;
    totalProcessingTime += processor->TotalProcessingTime ();
    cpuSecReport << bc->Class1Name () << "\t" << bc->Class2Name () << "\t" << zed << "\t" << totalCpuTimeUsed << endl;

    if  (processor->DateTimeStarted () < startClockTime)
      startClockTime = processor->DateTimeStarted ();

    if  (processor->DateTimeEnded () > endClockTime)
      endClockTime = processor->DateTimeEnded ();

    delete  processor;  processor = NULL;
  }

  cpuSecReport.close ();

  elapsedClockTime = (double)(endClockTime.Seconds () - startClockTime.Seconds ());

  longestPath = cyclesByProcId.LongestPath ();

  return  totalCpuTimeUsed;
}  /* GetTotalCpuSecs */






int  main (int argc, char **argv)
{
  osRunAsABackGroundProcess ();
   //osRunAsABackGroundProcess ();

  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

  FeatureSelection*  featureSelection = new FeatureSelection (argc, argv);

  double  totalCpuTimeUsed = 0.0;

  featureSelection->Run ();

  delete  featureSelection;
  
  MLClass::FinalCleanUp ();
  FileDesc::FinalCleanUp ();
  Instrument::FinalCleanUp ();
  FileDesc::FinalCleanUp ();
  FeatureFileIO::FinalCleanUp ();
  InstrumentDataFileManager::FinalCleanUp ();
  return 0;
}



