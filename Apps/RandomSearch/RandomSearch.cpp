#include  "FirstIncludes.h"

//**********************************************************************************
//*                                                                                *
//*                                                                                *
//*  Author: Kurt Kramer                                                           *
//*  Date:   2005-July-16                                                          *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*--Description-------------------------------------------------------------------*
//*                                                                                *
//* Performs a random search of Featuer Space, using a support vector machine(SVM) *
//* and a n-cross-fold-validation to grade a particular feature selection.         *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*                                                                                *
//*********************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include  <ctype.h>
#include  <time.h>

#include  <fcntl.h>

#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;




#include <sys/types.h>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif


#define  NumOfRandomOnesToCreate  500

#include  "RandomSearch.h"

#include  "CrossValidation.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "FeatureStats.h"
#include  "RunLog.h"
#include  "NormalizationParms.h"
#include  "OSservices.h"
#include  "StatisticalFunctions.h"

// 2005-07-16
// K:\v1\Adult
// -c dukes.cfg  -df dukes.data  -Format C45  -Stratify -RandomWidth 12

// 2005-07-17,  
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFfeatures
// -c Thesis9Classes.cfg  -df NineClasses_TestData.data  -Report TestReport.txt


// -c Thesis9Classes.cfg  -aw -df NineClasses_ValidationData.data  -RW 10 -tf NineClasses_TestData.data  

// -c survival.cfg  -df survival.data  - RW 25

//  -c survival_Stratified.CFG  -df survival_Stratified.data  -Format C45  -RandomWidth 25  -aw

// -c survival.CFG  -df survival_TestData.data  -Format C45  -RandomWidth 25  -aw  -TF survival_ValidationData.data

// 2005-08-09
//K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures
// -c Thesis9Classes.cfg  -df NineClasses_ValidationData.data  -RW  5  -tf  NineClasses_TestData.data  

RandomSearch::RandomSearch (int     argc,
                            char**  argv
                           ):

  Application (argc, argv),
  
  accuracyWeighted  (false),
  allFeatures       (NULL),
  alreadyNormalized (false),
  cancelFlag        (false),
  configFileName    (),
  config            (NULL),
  configClasses     (NULL),
  dataFileName      (),
  fileDesc          (NULL),
  fileFormat        (FeatureFileIOPices::Driver ()),
  mlClasses      (NULL),
  data              (NULL),
  highAccuracy      (0.0f),
  lockFile          (-1),
  lockFileName      ("RandomSearch.lock"),
  lockFileOpened    (false),
  maxNumOfFeatures  (INT_MAX),
  numOfFolds        (5),
  randomFeatures    (5),
  resultFileName    (),
  results           (NULL),
  stratify          (false),
  testFileName      (),
  testData          (NULL),
  verifyAccuracy    (false)

{
  log.Level (10) << "RandomSearch::RandomSearch" << endl;
  lockFileName    = "RandomSearch.lock";
  mlClasses    = new MLClassList ();

  ProcessCmdLineParameters (argc, argv);

  log.Level (10);

  if  (Abort ())
  {
    log.Level (-1) << endl
                   << "RandomSearch::RandomSearch   *** ERROR *** " << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  (dataFileName.Empty ())
  {
    log.Level (-1) << endl
                   << "No data file specified." << endl
                   << endl;
    DisplayCommandUsage ();
    osWaitForEnter ();
    exit (-1);
  }

  bool  successful;
  bool  changesMade = false;
  data = fileFormat->LoadFeatureFile (dataFileName, *mlClasses, -1, cancelFlag, successful, changesMade, log);
  if  (!successful)
  {
    log.Level (-1) << endl
                   << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                   << "                Error loading file[" << dataFileName << "]" << endl
                   << endl;
    DisplayCommandUsage ();
    osWaitForEnter ();
    exit (-1);
  }

  fileDesc = data->FileDesc ();

  if  (!testFileName.Empty ())
  {
    successful = false;
    testData = fileFormat->LoadFeatureFile (testFileName, *mlClasses, -1, cancelFlag, successful, changesMade, log);
    if  (!successful)
    {
      delete  testData;
      log.Level (-1) << endl
                     << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                     << "              Error loading test file[" << testFileName << "]" << endl
                     << endl;
      DisplayCommandUsage ();
      osWaitForEnter ();
      exit (-1);
    }

    if  (testData->FileDesc () != fileDesc)
    {
      log.Level (-1) << endl
                     << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                     << "              The data and test files have differnet FileDesc formats." << endl
                     << endl;
      DisplayCommandUsage ();
      osWaitForEnter ();
      exit (-1);
    }
  }


  allFeatures = new FeatureNumList (FeatureNumList::AllFeatures (fileDesc));

  config = new TrainingConfiguration2 (fileDesc, configFileName, log, false);
  if  (!config->FormatGood ())
  {
    log.Level (-1) << endl
                   << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                   << "                Invalid Configuration File[" << configFileName << "]" << endl
                   << endl;
    DisplayCommandUsage ();
    osWaitForEnter ();
    exit (-1);
  }
  configClasses = config->ExtractClassList ();
  (*mlClasses) += (*configClasses);


  //allFeatures = new FeatureNumList (FeatureNumList::AllFeatures (fileDesc));
  allFeatures = new FeatureNumList (config->GetFeatureNums ());

  if  ((randomFeatures < 1)  ||  (randomFeatures >= fileDesc->NumOfFields ()))
  {
    log.Level (-1) << endl
                   << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                   << "                Invalid Random Features[" << randomFeatures << "]" << endl
                   << endl;
    DisplayCommandUsage ();
    osWaitForEnter ();
    exit (-1);
  }

  if  (stratify)
  {
    KKStr  stratifiedFileName = osRemoveExtension (dataFileName) + 
                                 "_Stratified" + "." +
                                 osGetFileExtension (dataFileName);

    if  (osFileExists (stratifiedFileName))
    {
      log.Level (-1) << endl
                     << "RandomSearch          *** ERROR ***" << endl
                     << "            Stratified File[" << stratifiedFileName << "] already exists." << endl
                     << "            Erase it first or turn off the '-Stratify' option." << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    FeatureVectorListPtr  newData = data->StratifyAmoungstClasses (mlClasses, -1, numOfFolds);
    data->Owner (false);
    delete  data;
    data = newData;
    data->Owner (true);

    log.Level (10) << endl
                   << endl
                   << endl
                   << "Saving Stratified file[" << stratifiedFileName << "]" << endl
                   << endl;
    uint  numExamplesWritten = 0;
    fileFormat->SaveFeatureFile (stratifiedFileName, data->AllFeatures (), *data, numExamplesWritten, cancelFlag, successful, log);
  }

  if  (resultFileName.Empty ())
  {
    resultFileName = osRemoveExtension (dataFileName) + "_" +
                     StrFormatInt (randomFeatures, "Z000")  +
                     ".result";
  }

  if  (osFileExists (resultFileName))
  {
    results = new ResultLineTree (resultFileName, fileDesc, *mlClasses, successful, log);
    if  (accuracyWeighted)
      highAccuracy = results->HighestAccuracyWeighted ();
    else
      highAccuracy = results->HighestAccuracy ();

    cout << endl << endl << endl << endl << endl << endl
         << endl
         << "RandomSearch::RandomSearch     Highest Accuracy Loaded[" 
         << StrFormatDouble (highAccuracy, "ZZZ0.000") << "%" << "]." 
         << endl
         << endl;

    
    #ifdef  WIN32
    Sleep (4000);
    #else
    sleep (4);
    #endif
  }
  else
  {
    results = new ResultLineTree (true, log);
  }


  resultFile = new ofstream (resultFileName.Str (),  ios::app);
  if  (!resultFile->is_open ())
  {
    log.Level (-1) << endl
                   << "RandomSearch::RandomSearch   *** ERROR ***" << endl
                   << "             Error opening ResultFile[" << resultFileName << "]" << endl
                   << endl;
    DisplayCommandUsage ();
    osWaitForEnter ();
    exit (-1);
  }

  *resultFile << "//" << endl
              << "//" << endl
              << "// Date and Time      [" << osGetLocalDateTime ()                        << "]" << endl
              << "// Data File Name     [" << dataFileName                                 << "]" << endl
              << "// Test File Name     [" << testFileName                                 << "]" << endl
              << "// FileFormat         [" << fileFormat->DriverName ()                    << "]" << endl
              << "// Already Normalized [" << (alreadyNormalized ? "Yes" : "No")           << "]" << endl
              << "// Stratify           [" << (stratify ? "Yes" : "No")                    << "]" << endl
              << "// ConfigFileName     [" << configFileName                               << "]" << endl
              << "// Parameters         [" << config->ModelParameterCmdLine ()             << "]" << endl
              << "// Accuracy Measure   [" << (accuracyWeighted ? "ClassEqual" : "Normal") << "]" << endl
              << "// NumRandomFeatures  [" << randomFeatures                               << "]" << endl
              << "// NumOfFolds         [" << numOfFolds                                   << "]" << endl
              << "// ResultFileName     [" << resultFileName                               << "]" << endl
              << "// NumDataPoints      [" << data->QueueSize ()                           << "]" << endl
              << "// VerifyAccuracy     [" << (verifyAccuracy ? "Yes" : "No")              << "]" << endl
              << "//" << endl
              << "//" << endl
              << "// NumOfClasses       [" << mlClasses->QueueSize ()                   << "]" << endl
              << "// Name" << "\t" << "Count" << endl;

  ClassStatisticListPtr stats = data->GetClassStatistics ();
  for  (int x = 0;  x < stats->QueueSize ();  x++)
  {
    ClassStatisticPtr  stat = stats->IdxToPtr (x);
    *resultFile << "// " << stat->Name () << "\t" << stat->Count () << endl;
  }

  *resultFile << "//" << endl
              << "//" << endl
              << "// " << ResultLine::TitleLine (*mlClasses) << endl;
  resultFile->flush ();


  BuildOtherOrderings ();
}





RandomSearch::~RandomSearch ()
{
  delete results;
  delete resultFile;
  delete data;
  delete mlClasses;
  delete config;
}




void  RandomSearch::BuildOtherOrderings ()
{
  bool  successful;
  int   x;

  for  (x = 0;  x < OtherOrderings;  x++)
  {
    dataOther[x] = NULL;
    KKStr  otherOrderingFileName = osRemoveExtension (dataFileName) + "_" + StrFormatInt (x, "00") + ".data";
    if  (osFileExists (otherOrderingFileName))
      dataOther[x] = data->OrderUsingNamesFromAFile (otherOrderingFileName);

    if  (!dataOther[x])
    {
      dataOther[x] = data->StratifyAmoungstClasses (mlClasses, -1, numOfFolds);
      dataOther[x]->SaveOrderingOfImages (otherOrderingFileName, successful);
      if  (!successful)
      {
        log.Level (-1) << endl
                       << "RandomSearch::BuildOtherOrderings    *** ERROR ***" << endl
                       << "                  Could not save OtherOdering File[" << otherOrderingFileName << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }
    }
  }
}  /* BuildOtherOrderings */




float  RandomSearch::Accuracy (ResultLinePtr  result)
{
  if  (accuracyWeighted)
    return result->AccuracyWeighted ();
  else
    return result->Accuracy ();
}  /* Accuracy */




void  RandomSearch::Block ()
{
  log.Level (20) << "RandomSearch::Block - Entering." << endl;
  
  int  count = 0;

  do  {
    lockFile = open (lockFileName.Str (), O_WRONLY | O_CREAT | O_EXCL);

    if  (lockFile < 0)
    {
      count++;
      log.Level (20) << "RandomSearch::Block - We are locked out[" << count << "]."  << endl;
      #ifdef  WIN32
      Sleep (5000);
      #else
      sleep (5);
      #endif
    }
  }  while  (lockFile < 0);

  lockFileOpened = true;

  log.Level (20) << "RandomSearch::Block - Lock is Established." << endl;
}



void  RandomSearch::EndBlock ()
{              
  #ifndef  WIN32
  int  returnCd;
  #endif
  
  log.Level (20) << "RandomSearch::EndBlock - Ending Block" << endl;

  if  (!lockFileOpened)
  {
    log.Level (-1) << "RandomSearch::EndBlock - Lock file is not opened." << endl;
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
       log.Level (-1) << "RandomSearch::EndBlock - *** ERROR *** Could not set Lock File  to  Normal." << endl;
     }
     else
     {
       if  (!DeleteFile (lockFileName.Str ()))
       {
         DWORD  lastErrorNum = GetLastError ();
         log.Level (-1) << "RandomSearch::EndBlock - Error["  << lastErrorNum << "] deleting Lock File." << endl;
       }
     }
  }
  #else
  returnCd = unlink (lockFileName.Str ());
  #endif  

  log.Level (20) << "EndBlock - Unlocking" << endl;
  return;
}  /* EndBlock */



void  RandomSearch::DisplayCommandUsage ()
{
  cout << "-AW           Weight Accuracy for each class equally"                 << endl
       << "-Config       <Configuration File>"                                   << endl
       << "-DataFile     <Feature Data File>"                                    << endl
       << "-Format      " << FeatureFileIO::FileFormatsReadAndWriteOptionsStr () << endl
       << "-Folds        <Num of Folds, defaults to 5>"                          << endl
       << "-RandomWidth  <Num of Features to Keep>"                              << endl
       << "-Result       <Result File Name>"                                     << endl
       << "-Stratify"                                                            << endl
       << "-TestFile     <Test data file name>  optional"                        << endl
       << endl;
}  /* DisplayCommandUssage */




bool  RandomSearch::ProcessCmdLineParameter (char    parmSwitchCode, 
                                             KKStr  parmSwitch, 
                                             KKStr  parmValue
                                            )

{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-ACCURACYWEIGHTED")  ||  (parmSwitch == "-AW"))
  {
    accuracyWeighted = true;
  }

  else if  ((parmSwitch == "-CONFIG") ||  (parmSwitch == "-C"))
  {
    configFileName = parmValue;
  }

  else if  ((parmSwitch == "-DATAFILE") ||  (parmSwitch == "-DF"))
  {
    dataFileName = parmValue;
  }

  else if  ((parmSwitch == "-FORMAT"))
  {
    fileFormat = FeatureFileIO::FileFormatFromStr (parmValue, true, true);
    if  (fileFormat == NULL)
    {
      log.Level (-1) << endl
                     << endl
                     << "Invalid File Format Specified[" << parmValue << "]." << endl
                     << endl;
      DisplayCommandUsage ();
      osWaitForEnter ();
      exit (-1);
    }
  }


  else if  (parmSwitch == "-FOLDS")
  {
    numOfFolds = atoi (parmValue.Str ());
  }

  else if  ((parmSwitch == "-RANDOMWIDTH") ||  (parmSwitch == "-RW"))
  {
    randomFeatures = atoi (parmValue.Str ());
  }

  else if  ((parmSwitch == "-RESULT") ||  (parmSwitch == "-RF"))
  {
    resultFileName = parmValue;
  }

  else if  ((parmSwitch == "-STRATIFY"))
  {
    stratify = true;
  }

  else if  ((parmSwitch == "-TESTFILE") ||  (parmSwitch == "-TF"))
  {
    testFileName = parmValue;
  }

  else if  ((parmSwitch == "-VerifyAccuracy") ||  (parmSwitch == "-VA"))
  {
    verifyAccuracy = true;
  }

  else
  {
    log.Level (-1) << endl
                   << endl
                   << "RandomSearch::ProcessCmdLineParameter   *** ERROR ***" << endl
                   << "              Invalid Parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */





ResultLineListPtr  RandomSearch::BuildComponentParts (ResultLinePtr result)
{
  ResultLineListPtr  components = new ResultLineList (false, 20);

  while  (true)
  {
    if  ((result->Parent1Id () >= 0)  &&  ((result->Parent2Id () < 0)))
      result = results->LookUpById (result->Parent1Id ());
    else
      break;
  }

  if  (result)
  {
    if  (result->Parent2Id () < 0)
    {
      components->PushOnBack (result);
    }

    else
    {
      ResultLineListPtr  leftComponents  = BuildComponentParts (results->LookUpById (result->Parent1Id ()));
      ResultLineListPtr  rightComponents = BuildComponentParts (results->LookUpById (result->Parent2Id ()));
      components->AddQueue (*leftComponents);
      components->AddQueue (*rightComponents);
    }
  }

  return  components;
}  /* BuildComponentParts */





void  RandomSearch::GetTestAccuracy (ResultLinePtr  result,
                                     float&         accuracy,
                                     float&         accuracyWeighted
                                    )
{
  log.Level (10) << "RandomSearch::GetTestAccuracy  Result[" << result->Id () << "]." << endl;

  accuracy = 0.0f;
  accuracyWeighted = 0.0f;
  if  (!testData)
    return;

  config->SetFeatureNums (*result->Features ());
  bool  cancelFlag = false;
  CrossValidation  cv (config, data, mlClasses, numOfFolds, alreadyNormalized, fileDesc, log, cancelFlag);
  cv.RunValidationOnly (testData, NULL);

  ConfusionMatrix2Ptr  cm = cv.ConfussionMatrix ();
  accuracy         = float (cm->Accuracy ());
  accuracyWeighted = cm->AccuracyClassWeightedEqually ();

  log.Level (50) << "GetTestAccuracy     Exiting" << endl;

  return;
}  /* GetTestAccuracy */




ResultLinePtr  RandomSearch::EvaluateAFeatureSet (FeatureNumListPtr  features,
                                                  ResultLinePtr      parent1,
                                                  ResultLinePtr      parent2,
                                                  ResultLinePtr      family
                                                 )
{
  log.Level (10) << "RandomSearch::EvaluateAFeatureSet [" << features->NumOfFeatures () << "]" << endl;

  ResultLinePtr  existingResult = results->GetEqual (*features);
  if  (existingResult)
  {
    log.Level (50) << "EvaluateAFeatureSet   ** Already Existed **     Returning." << endl;
    return existingResult;
  }

  config->SetFeatureNums (*features);
  bool  cancelFlag = false;
  CrossValidation  cv (config, data, mlClasses, numOfFolds, alreadyNormalized, fileDesc, log, cancelFlag);
  cv.RunCrossValidation ();

  ResultLinePtr  result = new ResultLine (results->NextId (),
                                          parent1,
                                          parent2,
                                          family,
                                          cv,
                                          features, 
                                          log
                                         );


  if  (!verifyAccuracy)
    result->Verified (true);

  else if  (Accuracy (result) > (highAccuracy - 5.0f))
    CheckAgainstOtherOrderings (result);

  results->RBInsert (result);

  *resultFile << result->ToString () << endl;
  resultFile->flush ();

  if  (Accuracy (result) > highAccuracy)
  {
    // We have a new high, lets get the test accuracy for it.
    float  testAccuracy         = 0.0f;
    float  testAccuracyWeighted = 0.0f;

    GetTestAccuracy (result, testAccuracy, testAccuracyWeighted);

    highAccuracy = Accuracy (result);
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl
         << endl
         << "    ****************************************************" << endl
         << "    ****************  NEW HIGH ACCURACY  ***************" << endl
         << "    ****************************************************" << endl
         << endl
         << "    Train     Accuracy " << StrFormatDouble (result->Accuracy (),         "##0.000") << "%" << endl
         << "    Train Wtd Accuracy " << StrFormatDouble (result->AccuracyWeighted (), "##0.000") << "%" << endl
         << "    Test      Accuracy " << StrFormatDouble (testAccuracy,                "##0.000") << "%" << endl
         << "    Test  Wtd Accuracy " << StrFormatDouble (testAccuracyWeighted,        "##0.000") << "%" << endl
         << endl
         << endl;

    #ifdef  WIN32
    Sleep (2000);
    #else
    sleep (2);
    #endif

    *resultFile << "//" << endl
                << "//********************** NEW HIGH ACCURACY *********************" << endl
                << "//*     Train     Accuracy[" << StrFormatDouble (result->Accuracy (),         "ZZ0.000") << "%]" << endl
                << "//*     Train Wtd Accuracy[" << StrFormatDouble (result->AccuracyWeighted (), "ZZ0.000") << "%]" << endl
                << "//*     Test      Accuracy[" << StrFormatDouble (testAccuracy,                "ZZ0.000") << "%]" << endl
                << "//*     Test  Wtd Accuracy[" << StrFormatDouble (testAccuracyWeighted,        "ZZ0.000") << "%]" << endl
                << "//**************************************************************" << endl
                << "//" << endl;

    resultFile->flush ();
  }


  log.Level (50) << "EvaluateAFeatureSet     Exiting" << endl;

  return  result;
}  /* EvaluateAFeatureSet */





void  RandomSearch::CheckAgainstOtherOrderings (ResultLinePtr  result)
{
  float  curAccuracy  = Accuracy (result);
  float  tempAccuracy = curAccuracy;

  float  accuracy    = result->Accuracy ();
  float  accuracyWtd = result->AccuracyWeighted ();
  int    idx = 0;

  config->SetFeatureNums (*(result->Features ()));

  cout << endl << endl << endl << "RandomSearch::CheckAgainstOtherOrderings[" << result->Id () << "]  Features[" << *(result->Features ()) << "]" << endl << endl << endl << endl;

  while  ((curAccuracy > 50.0f)  &&  (idx < OtherOrderings))
  {
    //config->SetFeatureNums (*features);
    bool  cancelFlag = false;
    CrossValidation  cv (config, dataOther[idx], mlClasses, numOfFolds, alreadyNormalized, fileDesc, log, cancelFlag);
    cv.RunCrossValidation ();

    accuracyWtd = cv.ConfussionMatrix ()->AccuracyClassWeightedEqually ();
    accuracy    = (float)cv.ConfussionMatrix ()->Accuracy ();

    if  (accuracyWeighted)
      tempAccuracy = accuracyWtd;
    else
      tempAccuracy = accuracy;

    if  (tempAccuracy < curAccuracy)
    {
      curAccuracy = tempAccuracy;
      result->Accuracy (accuracy);
      result->AccuracyWeighted (accuracyWtd);
    }

    idx++;
  }

  result->Verified (true);
}  /* CheckAgainstOtherOrderings */






ResultLinePtr  RandomSearch::TrimAFeatureSet (ResultLinePtr  resultToExpand,
                                              float          highAccuracy
                                             )
{
  FeatureNumList features = *(resultToExpand->Features ());

  ResultLinePtr  resultWithHighestAccuracy = NULL;
  float  highestAccuracyFound = 0.0f;

  log.Level (10) << "RandomSearch::TrimAFeatureSet   FeatureSet[" << features.ToString () << "]" << endl;

  int x;

  ResultLineVector  evaluatedresults;

  
  ResultLinePtr  family = resultToExpand->Family ();
  if  (!family)
    family = resultToExpand;

  for  (x = 0; x < features.NumOfFeatures ();  x++)
  {
    FeatureNumList trimmedFeatureSet = features - features[x];
    ResultLinePtr trimmedResult = EvaluateAFeatureSet (&trimmedFeatureSet,
                                                        resultToExpand,
                                                        NULL,
                                                        family
                                                       );
    if  (trimmedResult)
    {
      evaluatedresults.push_back (trimmedResult);

      float accuracy = Accuracy (trimmedResult);
      if  (accuracy > highestAccuracyFound)
      {
        highestAccuracyFound      = accuracy;
        resultWithHighestAccuracy = trimmedResult;
      }
    }
  }
  
  resultToExpand->Expanded (true);
  *resultFile << resultToExpand->ToString () << endl;
  
  return  resultWithHighestAccuracy;
}  /* TrimAFeatureSet */






ResultLinePtr  RandomSearch::TrimAFeatureSetInDepth (int            depth,
                                                     int            width,
                                                     ResultLinePtr  resultToExpand,
                                                     float          highAccuracy,
                                                     float*         highs         // The high accuracies by feature count
                                                    )
{
  FeatureNumList features = *(resultToExpand->Features ());

  log.Level (10) << "RandomSearch::TrimAFeatureSetInDepth[" << features.NumOfFeatures () << "]  FeatureSet[" << features.ToString () << "]" << endl;

  bool            createdHighs = false;
  ResultLineList  evaluatedResults (false, 50);
  ResultLinePtr   family = NULL;
  int             featureCount = features.NumOfFeatures ();
  float           highestAccuracyFound = 0.0f;
  int             numWithHighestAccuracy = 0;
  ResultLinePtr   resultWithHighestAccuracy = NULL;
  int x;
  
  family = resultToExpand->Family ();
  if  (!family)
    family = resultToExpand;


  createdHighs = false;
  if  (!highs)
  {
    highs = new float[featureCount];
    createdHighs = true;
    for  (x = 0;  x < featureCount; x++)
      highs[x] = 0.0f;
  }


  for  (x = 0; x < featureCount;  x++)
  {
    log.Level (50) << "TrimAFeatureSetInDepth   tOP of loop[" << x << "] of [" << featureCount << "]" << endl;

    FeatureNumList trimmedFeatureSet = features - features[x];
    int  trimmedFeatureCount = trimmedFeatureSet.NumOfFeatures ();


    

    ResultLinePtr trimmedResult = EvaluateAFeatureSet (&trimmedFeatureSet,
                                                        resultToExpand,
                                                        NULL,
                                                        family
                                                       );

    log.Level  (50) << "TrimAFeatureSetInDepth   returned from EvaluateAFeatureSet" << endl;

    if  (trimmedResult)
    {
      evaluatedResults.PushOnBack (trimmedResult);

      float accuracy = Accuracy (trimmedResult);
      if  (accuracy > highestAccuracyFound)
      {
        highestAccuracyFound      = accuracy;
        resultWithHighestAccuracy = trimmedResult;
        numWithHighestAccuracy    = 1;
      }
      else if  (accuracy == highestAccuracyFound)
      {
        numWithHighestAccuracy++;
      }
    }

    log.Level (50) << "TrimAFeatureSetInDepth   Bottom of loop[" << x << "] of [" << featureCount << "]" << endl;
  }

  resultToExpand->Expanded (true);
  *resultFile << resultToExpand->ToString () << endl;

  if  (numWithHighestAccuracy > 1)
    log.Level (50) << "TrimAFeatureSetInDepth  NumWithHighAccuracy[" << numWithHighestAccuracy << "]" << endl;
  
  for  (x = featureCount - 1;  x > 1;  x--)
  {
    highs[x] = Max (highs[x], highestAccuracyFound);
  }

  if  ((depth                                        <=  1)  ||  
       ((highAccuracy        - highestAccuracyFound) > 0.8)  ||  
       ((highs[featureCount] - highestAccuracyFound) > 0.8)  ||  
       (featureCount                                 <   3)
      )
  {
    log.Level (50) << endl
                   << "TrimAFeatureSetInDepth     Reached Termination Condition" << endl
                   << "                   depth[" << depth                        << "]" << endl
                   << "                   highAccuracy[" << highAccuracy          << "]" << endl
                   << "            highs[featureCount][" << highs[featureCount]   << "]" << endl
                   << "           highestAccuracyFound[" << highestAccuracyFound  << "]" << endl
                   << endl;

    return  resultWithHighestAccuracy;
  }

  evaluatedResults.SortByAccuracy (false, accuracyWeighted);

  highAccuracy = Max (highAccuracy, highestAccuracyFound);
  ResultLineList  trimmendResults (false, width);

  int  idx = 0;
  int  numEvaluated = 0;
  
  //int  numToEvaluate = Max (numWithHighestAccuracy, width);
  int  numToEvaluate = width;

  while  ((idx < evaluatedResults.QueueSize ())  &&  (numEvaluated < numToEvaluate))
  {
    ResultLine&  result = evaluatedResults[idx];
    if  ((highAccuracy - Accuracy (&result)) > 0.8)
      break;

    ResultLinePtr  trimmedResult = TrimAFeatureSetInDepth (depth - 1, width, &result, highAccuracy, highs);
    trimmendResults.PushOnBack (trimmedResult);
    idx++;
    numEvaluated++;
  }

  if  (trimmendResults.QueueSize () < 1)
  {
    log.Level (30) << endl
                   << "TrimAFeatureSetInDepth     ** (trimmendResults.QueueSize () < 1)  **" << endl
                   << endl;
    return  resultWithHighestAccuracy;
  }

  if  (createdHighs)
    delete  highs;

  trimmendResults.SortByAccuracy (false, accuracyWeighted);
  log.Level (30) << "TrimAFeatureSetInDepth      exited" << endl;

  return  trimmendResults.IdxToPtr (0);
}  /* TrimAFeatureSetInDepth */








ResultLinePtr  RandomSearch::TrimComponentsOfAFeatureSet (ResultLinePtr  resultToExpand,
                                                          float          highAccuracy
                                                         )
{
  if  (resultToExpand->Parent2Id () < 0)
  {
    // There are no sub-components, 
    return  TrimAFeatureSet (resultToExpand, Accuracy (resultToExpand));
  }

  ResultLineListPtr  components = BuildComponentParts (resultToExpand);

  FeatureNumList  aggragateTrimmedFeatures (fileDesc);

  ResultLineList::iterator  idx;

  for  (idx = components->begin ();  idx != components->end ();  idx++)
  {
    ResultLinePtr  component = *idx;
    ResultLinePtr  trimmendComponent = TrimAFeatureSet (component, Accuracy (component));
    if  (trimmendComponent)
      aggragateTrimmedFeatures = aggragateTrimmedFeatures + *trimmendComponent->Features ();
  }

  ResultLinePtr  aggragateResult  = EvaluateAFeatureSet (&aggragateTrimmedFeatures, NULL, NULL, NULL);

  ResultLinePtr  aggragateTrimmedResult = TrimAFeatureSet (aggragateResult, Accuracy (aggragateResult));

  ResultLinePtr fullResultTrimmed = TrimAFeatureSet (resultToExpand, Accuracy (resultToExpand));

  // Return back the result that got the highest accuracy.
  if  ((Accuracy (fullResultTrimmed) > Accuracy (aggragateTrimmedResult))  &&
       (Accuracy (fullResultTrimmed) > Accuracy (aggragateResult))
      )
    return  fullResultTrimmed;

  else if  (Accuracy (aggragateTrimmedResult) > Accuracy (aggragateResult))
    return  aggragateTrimmedResult;

  else
    return  aggragateResult;
}  /* TrimComponentsOfAFeatureSet */





VectorInt  UniteTwoVectors (VectorInt& v1,
                            VectorInt& v2
                            )
{
  VectorInt  result;

  VectorInt::size_type  x1 = 0;
  VectorInt::size_type  x2 = 0;

  while  ((x1 < v1.size ())  &&  (x2 < v2.size ()))
  {
    if  (v1[x1] < v2[x2])
    {
      result.push_back (v1[x1]);
      x1++;
    }

    else if  (v1[x1] > v2[x2])
    {
      result.push_back (v2[x2]);
      x2++;
    }

    else
    {
      result.push_back (v1[x1]);
      x1++;
      x2++;
    }
  }

  while  (x1 < v1.size ())
  {
    result.push_back (v1[x1]);
    x1++;
  }

  while  (x2 < v2.size ())
  {
    result.push_back (v2[x2]);
    x2++;
  }

  return  result;
}  /* UniteTwoVectors */






bool  FamiliesAreRelated (VectorInt&  family1,  
                          VectorInt&  family2
                         )
{
  // it is assumed that famil1 && family2 are sorted
  VectorInt::size_type  x1 = 0; 
  VectorInt::size_type  x2 = 0;

  while  ((x1 < family1.size ())  &&  (x2 < family2.size ()))
  {
    if  (family1[x1] < family2[x2])
      x1++;

    else if  (family1[x1] > family2[x2])
      x2++;

    else
      return true;
  }

  return  false;
}  /* FamiliesAreRelated */



ResultLinePairVector  RandomSearch::GetUnrelatedPairs (ResultLineVector&  resultsToCombine,
                                                       int                deapth
                                                      )
{
  int  x, y;

  ResultLinePairVector  unrelatedPairs;

  int  numOfResults = (int)resultsToCombine.size ();
  if  (numOfResults < 2)
  {
   return unrelatedPairs;
  }


  vector<VectorInt>  extendedFamilies;

  for  (x = 0;  x < numOfResults;  x++)
  {
    VectorInt  extendedFamily;
    resultsToCombine[x]->BuildExtendedFamilyList (deapth, extendedFamily);
    sort (extendedFamily.begin (), extendedFamily.end ());
    extendedFamilies.push_back (extendedFamily);
  }


  for  (x = 0;  (x < numOfResults - 1);  x++)
  {
    VectorInt&  extendedFamily1 = extendedFamilies[x]; 
    ResultLinePtr  result1 = resultsToCombine[x];

    for  (y = x + 1;  y < numOfResults;  y++)
    {
      if  (x == y)  
        continue;

      VectorInt&  extendedFamily2 = extendedFamilies[y]; 
      ResultLinePtr  result2 = resultsToCombine[y];

      FeatureNumList  combinedFeatures = *(result1->Features ()) + *(result2->Features ());
      ResultLinePtr alreadyExistingEvaluation = results->GetEqual (combinedFeatures);
      if  (!alreadyExistingEvaluation)
      {
        if  (!FamiliesAreRelated (extendedFamily1, extendedFamily2))
        {
          unrelatedPairs.push_back (ResultLinePair (result1, result2));
        }
      }
    }
  }

  return unrelatedPairs; 
}  /* GetUnrelatedPairs */





ResultLinePairVector  RandomSearch::BuildPairsFromList (ResultLineVector&  resultsToCombine)
{
  int  x, y;

  ResultLinePairVector  pairs;

  int  numOfResults = (int)resultsToCombine.size ();
  if  (numOfResults < 2)
  {
   return pairs;
  }


  for  (x = 0;  (x < numOfResults - 1);  x++)
  {
    ResultLinePtr  result1 = resultsToCombine[x];

    for  (y = x + 1;  y < numOfResults;  y++)
    {
      ResultLinePtr  result2 = resultsToCombine[y];
      pairs.push_back (ResultLinePair (result1, result2));
    }
  }

  return pairs; 
}  /* GetUnrelatedPairs */





void  RandomSearch::AddToCombinationsIfNotRelated (int                deapth,   // How many famliies to search back
                                                   ResultLineVector&  results,
                                                   VectorInt&         families,
                                                   ResultLinePtr      result
                                                  )
{
  VectorInt  extendedFamily;
  result->BuildExtendedFamilyList (deapth, extendedFamily);
  sort (extendedFamily.begin (), extendedFamily.end ());

  // Make sure not related to any families in 'families'
  if  (FamiliesAreRelated (extendedFamily, families))
  {
    // This result is related to one that already exists in 'results'
    return;
  }
  
  families = UniteTwoVectors (extendedFamily, families);
  results.push_back (result);
}  /* AddToCombinationsIfNotRelated */




void  RandomSearch::AddToCombinations (ResultLineVector&  resultsToCombine,
                                       ResultLinePtr      result
                                      )
{
  bool  insest = false;

  int  y;   
  for  (y = 0; y < (int)resultsToCombine.size ();  y++)
  {
    if  (resultsToCombine[y]->FamilyId () == result->FamilyId ())
    {
      insest = true;
      if  (Accuracy (resultsToCombine[y]) < Accuracy (result))
      {
        // Use the family member with the greater accuracy, 
        resultsToCombine[y] = result;
      }

      else if  (Accuracy (resultsToCombine[y]) == Accuracy (result))
      {
        if  (resultsToCombine[y]->NumOfFeatures () > result->NumOfFeatures ())
          resultsToCombine[y] = result;
      }

      break;
    }
  }

  if  (!insest)
    resultsToCombine.push_back (result);
}  /* AddToCombinations */




ResultLineVector  RandomSearch::LookForUsedUpInAllCombinations (ResultLineVector&  onesToTry,
                                                                     int&                    combinationsNotUsed
                                                                    )
{
  combinationsNotUsed = 0;

  int  x, y;

  ResultLineVector  usedUpResults;

  for  (x = 0; x < ((int)onesToTry.size () - 1); x++)
  {
    ResultLinePtr  r1 = onesToTry[x];

    bool  usedUp = true;

    for (y = 0; y < (int)onesToTry.size  (); y++)
    {
      ResultLinePtr  r2 = onesToTry[y];
      if  (r1 == r2)
        continue;

      ResultLinePtr  existingCombination = results->LookUpByParents (r1, r2);
      if  (!existingCombination)
      {
        usedUp = false;
        combinationsNotUsed++;
      }
    }
    if  (usedUp)
      usedUpResults.push_back (r1);
  }

  return  usedUpResults;
}  /* LookForUsedUpInAllCombinations */





ResultLineList  RandomSearch::EvaluateCombinations (ResultLinePairVector&  combinations)
{
  float  highestAccuracy = 0.0f;
  ResultLinePtr  resultWithHighestAccuracy = NULL;

  ResultLineList  evaluatedCombos;

  int  x;

  for  (x = 0; x < (int)combinations.size (); x++)
  {
    ResultLinePair& pair = combinations[x];
    ResultLinePtr  r1 = pair.first;
    ResultLinePtr  r2 = pair.second;

    FeatureNumList combined = (*(r1->Features ())) +  (*(r2->Features ()));
    ResultLinePtr  combinedResult = EvaluateAFeatureSet (&combined, r1, r2, NULL);

    if  (combinedResult)
    {
      evaluatedCombos.PushOnBack (combinedResult);
      if  (Accuracy (combinedResult) > highestAccuracy)
      {
        highestAccuracy = Accuracy (combinedResult);
        resultWithHighestAccuracy = combinedResult;
      }
    }
  }

  evaluatedCombos.SortByAccuracy (false, accuracyWeighted);

  return  evaluatedCombos;
} /* EvaluateCombinations */






ResultLineList  RandomSearch::TrySomeCombinationsOfTheBetterOnesFoundSofar (int  maxSize)
{
  ResultLineList sortedResults (results, maxSize);
  sortedResults.SortByAccuracy (false, accuracyWeighted);

  int  size = sortedResults.QueueSize ();

  ResultLinePairVector  unrelatedPairs;
  ResultLineVector      onesToTry;

  if  (size < 5)
    return ResultLineList ();

  int  idx = 0;
  while  ((idx < sortedResults.QueueSize ())  &&  ((int)unrelatedPairs.size () < 3))
  {
    AddToCombinations (onesToTry, sortedResults.IdxToPtr (idx));
    if  (onesToTry.size () > 2)
      unrelatedPairs = GetUnrelatedPairs (onesToTry, 2);
    idx++;
  }

  // Select three at random from top 10 Percent
  int  top10PercIdx = (int)((float)size * 0.10f + 0.5f);

  int  numOfTries = 0;
  while  ((unrelatedPairs.size () < 15)  &&  (numOfTries < 100))
  {
    int randomIdx = rand () % top10PercIdx + 3;
    if  (randomIdx < size)
    {
      AddToCombinations (onesToTry, sortedResults.IdxToPtr (randomIdx));
      if  (numOfTries > 3)
         unrelatedPairs = GetUnrelatedPairs (onesToTry, 2);
    }
    numOfTries++;
  }


  //  Get four original random randomly from top 10%
  numOfTries = 0;
  while  ((unrelatedPairs.size () < 36)  &&  (numOfTries < 100))
  {
    int randomIdx = rand () % top10PercIdx + 4;
    ResultLinePtr  result = sortedResults.IdxToPtr (randomIdx);
    if  ((result->Parent1Id () < 0)  &&  (result->Parent2Id () < 0))
    {
      AddToCombinations (onesToTry, result);
      if  (numOfTries > 3)
        unrelatedPairs = GetUnrelatedPairs (onesToTry, 2);
    }
    numOfTries++;
  }

  return  EvaluateCombinations (unrelatedPairs);
}  /* TrySomeCombinationsOfTheBetterOnesFoundSofar */





ResultLineList  RandomSearch::CombineSomeOfTheBestJobs (int  numToCombine,
                                                        int  maxSize
                                                       )
{
  ResultLineList sortedResults (results, maxSize);
  sortedResults.SortByAccuracy (false, accuracyWeighted);

  int  totalNumWanted = numToCombine * (numToCombine - 1) / 2;

  ResultLinePairVector  combinations;
  ResultLineVector      resultsToCombine;

  int  idx = 0;
  int  oldResultsToCombine = 0;
  while  (((int)resultsToCombine.size () <  numToCombine) || ((int)combinations.size () < totalNumWanted))
  {
    if  (idx >= (int)sortedResults.QueueSize ())
      break;

    ResultLinePtr  result = sortedResults.IdxToPtr (idx);
    AddToCombinations (resultsToCombine, result);
    if  (((int)resultsToCombine.size () >= numToCombine)  &&  (oldResultsToCombine != resultsToCombine.size ()))
    {
      combinations = GetUnrelatedPairs (resultsToCombine, 3);  // Go back 3 whole families.
      oldResultsToCombine = (int)resultsToCombine.size ();
    }
    idx++;
  }

  return  EvaluateCombinations (combinations);
}  /* CombineSomeOfTheBestJobs */





ResultLinePtr  RandomSearch::TrimBestNotExpanded (int  numToTrim,
                                                  int  maxSize
                                                 )
{
  ResultLineList sortedResults (results, maxSize);
  sortedResults.SortByAccuracy (false, accuracyWeighted);
  
  int  idx = 0;
  ResultLineVector resultsToTrim;
  VectorInt        resultsToTrimFamilies;

  while  (((int)resultsToTrim.size () < numToTrim)  &&  (idx < sortedResults.QueueSize ()))
  {
    ResultLinePtr  result = sortedResults.IdxToPtr (idx);
    if  (!result->Expanded ())
    {
      // To avoid insest we use 'AddToCombinationsIfNotRelated' to build list.
      AddToCombinationsIfNotRelated (3, // Go back three whole family generations
                                     resultsToTrim,
                                     resultsToTrimFamilies,
                                     result
                                    );
    }
    idx++;
  }

  float  highestAccuracy = 0.0f;
  ResultLinePtr  resultWithHighestAccuracy = NULL;

  for  (idx = 0;  idx < (int)resultsToTrim.size ();  idx++)
  {
    ResultLinePtr  result = resultsToTrim[idx];
    ResultLinePtr trimmedResult = TrimAFeatureSet  (result, Accuracy (result));
    if  (Accuracy (trimmedResult) > highestAccuracy)
    {
      highestAccuracy = Accuracy (trimmedResult) ;
      resultWithHighestAccuracy  = trimmedResult;
    }
  }

  return  resultWithHighestAccuracy;
}  /* TrimBestNotExpanded */





// Select sets of features at random until a combination that has not
// been processed before, then evaluate that set of features.
ResultLinePtr  RandomSearch::RunARandomlySelectedSetOfFeatures ()
{
  ResultLinePtr existingResult = NULL;
  FeatureNumListPtr  randomSelFeatures = NULL;

  do {
    delete randomSelFeatures;
    randomSelFeatures = allFeatures->RandomlySelectFeatures (randomFeatures);
    existingResult = results->GetEqual (*randomSelFeatures);
  }  while  (existingResult);

 ResultLinePtr randomResults = EvaluateAFeatureSet (randomSelFeatures, NULL, NULL, NULL);
 delete  randomSelFeatures;
 return  randomResults;
}  /* RunARandomlySelectedSetOfFeatures */



void  RandomSearch::ExpandedUnSetAll ()
{
  ResultLinePtr  result = results->GetFirst ();
  while  (result)
  {
    result->Expanded (false);
    result = results->GetNext ();
  }
}  /* ExpandedUnSetAll */



void  RandomSearch::ReportStats ()
{
  int  featureNum;
  FeatureStats fs (fileDesc, *results, log);

  *resultFile << "//" << endl;
  *resultFile << "//" << endl;
  *resultFile << "//FeatureNums";
  for  (featureNum = 0;  featureNum < fs.NumOfFeatures ();  featureNum++)
    *resultFile << "\t" << featureNum;
  *resultFile << endl;

  *resultFile << "//Count";
  for  (featureNum = 0;  featureNum < fs.NumOfFeatures ();  featureNum++)
    *resultFile << "\t" << fs.AccuracyDeltaCount (featureNum);;
  *resultFile << endl;

  *resultFile << "//Mean";
  for  (featureNum = 0;  featureNum < fs.NumOfFeatures ();  featureNum++)
    *resultFile << "\t" << fs.AccuracyDeltaMean (featureNum);
  *resultFile << endl;

  *resultFile << "//StdDev";
  for  (featureNum = 0;  featureNum < fs.NumOfFeatures ();  featureNum++)
    *resultFile << "\t" << fs.AccuracyDeltaStdDev (featureNum);
  *resultFile << endl;
  *resultFile << "//" << endl;
  *resultFile << "//" << endl;
}  /* ReportStats */




void  RandomSearch::PerformWrapper (ResultLinePtr  seedResult)
{
  float  highAccuracy = 0.0f;
  ResultLineList  pool (false, 10000);
  pool.PushOnBack (seedResult);

  maxNumOfFeatures = seedResult->Features ()->NumOfFeatures ();
  int  numExpansionsSinceLastReduction = 0;

  while  (true)
  {
    //ReportStats ();

    // First, make sure that the 1st one we are picking has been verified.
    pool.SortByAccuracy (false, accuracyWeighted);
    int  idx = 0;
    bool  exitLoop = false;
    while  (!exitLoop)
    {
      if  (pool[idx].Features ()->NumOfFeatures () > maxNumOfFeatures)
      {
        idx++;
      }

      else if  (!pool[idx].Verified ())
      {
        CheckAgainstOtherOrderings (&pool[idx]);
        *resultFile << pool[idx].ToString () << endl;
        pool.SortByAccuracy (false, accuracyWeighted);
        idx = 0;
      }

      else if  (pool[idx].Expanded ())
      {
        idx++;
      }

      else
      {
        exitLoop = true;
      }
    }
       
    // The sort will place non expanded first,
    ResultLinePtr  nextOneToExpand = pool.IdxToPtr (idx);

    if  (nextOneToExpand->Expanded ())
    {
      // There must be nothing left to expand;  This is not likely
      // to have happend but it did so lets terminate search.
      *resultFile << "//"                               << endl
                  << "// No more nodes left to expand." << endl
                  << "// Terminating"                   << endl
                  << "// "                              << endl;
       break;
    }

    numExpansionsSinceLastReduction++;
    vector<FeatureNumList>  featureCombosToTry;
    FeatureNumList& features = *(nextOneToExpand->Features ());
    for  (int x = 0;  x < features.NumOfFeatures (); x++)
    {
      FeatureNumList  trimmedFeatures = features - features[x];
      featureCombosToTry.push_back (trimmedFeatures);
    }

    //FeatureStats  fs (fileDesc,  *results, log);
    //FeatureNumList  harmfullFreeFeatures =  fs.RemoveHarmfulFeatures (*(nextOneToExpand->Features ()));
    //featureCombosToTry.push_back (harmfullFreeFeatures);

    if  ((nextOneToExpand->Parent1 ())  &&  (!nextOneToExpand->Parent2 ()))
    {
      // Lets chck to see other siblings that did as well and remove there featuer also.

      int  numCandidates = 0;
      FeatureNumList  reducedFeatures = *(nextOneToExpand->Features ());
      ResultLinePtr   parent          = nextOneToExpand->Parent1 ();
      FeatureNumList  parentsFeatures = *(parent->Features ());

      ResultLineListPtr  siblings = results->GetChildrenOfAParent (nextOneToExpand->Parent1 ());

      ResultLineList::iterator  idx;
      for  (idx = siblings->begin ();  idx != siblings->end ();  idx++)
      {
        ResultLinePtr  sibling = *idx;
        if  (sibling == nextOneToExpand)
          continue;

        if  (sibling->Accuracy () >= nextOneToExpand->Accuracy ())
        {
          // We don't want his feature change either.
          numCandidates++;
          FeatureNumList  featuresThatChanged = parentsFeatures - *(sibling->Features ());
          reducedFeatures = reducedFeatures - featuresThatChanged;
        }
      }

      if  (numCandidates > 1)
      {
        FeatureNumList featuresReduced = *(nextOneToExpand->Features ()) - reducedFeatures;
        *resultFile << "//" << endl
                    << "// Mutiple Feature Reduction[" << featuresReduced << "]" << endl
                    << "//" << endl;
        featureCombosToTry.push_back (reducedFeatures);
      }
    }

    *resultFile << "//" << endl
                << "// Expanding Id[" << nextOneToExpand->Id () << "]" << endl;


    for  (int x = 0;  x < (int)featureCombosToTry.size (); x++)
    {
      FeatureNumList  trimmedFeatures = featureCombosToTry[x];
      ResultLinePtr trimmedResult 
            = EvaluateAFeatureSet (&trimmedFeatures, 
                                   nextOneToExpand,
                                   NULL,
                                   nextOneToExpand->Family ()
                                  );

      pool.PushOnBack (trimmedResult);

      if  (Accuracy (trimmedResult) > highAccuracy)
      {
        numExpansionsSinceLastReduction = 0;
        highAccuracy = Accuracy (trimmedResult);

        float  trainAccuracy         = trimmedResult->Accuracy         ();
        float  trainAccuracyWeighted = trimmedResult->AccuracyWeighted ();
        float  testAccuracy          = 0.0f;
        float  testAccuracyWeighted  = 0.0f;

        if  (testData)
          GetTestAccuracy (trimmedResult, testAccuracy, testAccuracyWeighted);

        *resultFile << "//" << endl
                    << "//****************** NEW HIGH ACCURACY ******************" << endl
                    << "//*     Train Accuracy     [" << StrFormatDouble (trainAccuracy,         "ZZ0.000") << "%]" << endl
                    << "//*     Train Accuracy Wtd [" << StrFormatDouble (trainAccuracyWeighted, "ZZ0.000") << "%]" << endl
                    << "//*     Test  Accuracy     [" << StrFormatDouble (testAccuracy,          "ZZ0.000") << "%]" << endl
                    << "//*     Test  Accuracy Wtd [" << StrFormatDouble (testAccuracyWeighted,  "ZZ0.000") << "%]" << endl
                    << "//*******************************************************" << endl
                    << "//" << endl;

        resultFile->flush ();
      } 
      
      else if  (Accuracy (trimmedResult) == highAccuracy)
      {
        numExpansionsSinceLastReduction = 0;
      }
    }


    nextOneToExpand->Expanded (true);
    *resultFile << nextOneToExpand->ToString () << endl;

    if  (numExpansionsSinceLastReduction > 10)
    {
      numExpansionsSinceLastReduction = 0;
      maxNumOfFeatures = nextOneToExpand->Features ()->NumOfFeatures () - 1;
      *resultFile << "//" << endl
                  << "// Reducing maxNumOfFeatures to [" << maxNumOfFeatures << "]" << endl
                  << "//" << endl;
    }
  }
}  /* PerformWrapper */





void  RandomSearch::Run0 ()
{
  if  (results->Size () < 10000)
  {
    while  (results->Size () < 10000)
    {
      RunARandomlySelectedSetOfFeatures ();
    }
  }


  if  (results->Size () < 11600)
  {
    while  (results->Size () < 11600)
    {
      // Only interested in combining the originally radomly selected results, which 
      // at this point is all that there should be abailable.
      CombineSomeOfTheBestJobs (40,              // Combine 40 of the best that have no
                                randomFeatures   // more than 'randomFeatures' features.
                               );
    }
  }


  if  (results->Size () < 12000)
  {
    while  (results->Size () < 12000)
    {
      // Will now combine 20 of the best results that have no more
      // than twice(2) the 'randomFeatures' features.  There is a 
      // restriction in that no two results can be related going back
      // two whole families.  At this point here should be none taht 
      // are more than one family past.
      CombineSomeOfTheBestJobs (20, 
                                randomFeatures * 2
                               );
    }
  }


  // Will now go into a loop that we will stay in until the 
  // program is killed.
  while  (true)
  {
    int x;
    CombineSomeOfTheBestJobs (3, randomFeatures * 10);
    //TrySomeCombinationsOfTheBetterOnesFoundSofar (randomFeatures * 10);
    for  (x = 0;  x < 3;  x++)
    {
      // Trim th eresults of the three best results in the system
      // that are not related to each other for at least 3
      // families.
      TrimBestNotExpanded (3, randomFeatures * 10);
    }
  }

}  /* Run0 */








void  RandomSearch::Run1 ()
{
  if  (results->Size () < NumOfRandomOnesToCreate)
  {
    while  (results->Size () < NumOfRandomOnesToCreate)
    {
      RunARandomlySelectedSetOfFeatures ();
    }
  }

  if  (results->Size () < (NumOfRandomOnesToCreate + 100))
  {
    CombineSomeOfTheBestJobs (5,                 // Combine 10 of the best that have no
                              randomFeatures      // more than 'randomFeatures' features.
                             );

    CombineSomeOfTheBestJobs (10,                 // Combine 10 of the best that have no
                              randomFeatures      // more than 'randomFeatures' features.
                             );

    CombineSomeOfTheBestJobs (10,                 // Combine 10 of the best that have no
                              2 * randomFeatures  // more than 'randomFeatures' features.
                             );

    CombineSomeOfTheBestJobs (10,                 // Combine 10 of the best that have no
                              4 * randomFeatures  // more than 'randomFeatures' features.
                             );
  }

  {
    ResultLineList    sortedResults (results, 10 * randomFeatures, 11225);
    sortedResults.SortByAccuracy (false, accuracyWeighted);

    ResultLineVector  topResults;
    int idx = 0;
    while  ((idx < sortedResults.QueueSize ())  &&  (topResults.size () < 10))
    {
      ResultLinePtr  result = sortedResults.IdxToPtr (idx);
      topResults.push_back (result);
      idx++;
    }

    ResultLineVector  bestUnrelatedResults;
    VectorInt         families;
    idx = 0;
    while  ((idx < sortedResults.QueueSize ())  &&  (bestUnrelatedResults.size () < 10))
    {
      ResultLinePtr  result = sortedResults.IdxToPtr (idx);
      AddToCombinationsIfNotRelated (2,  bestUnrelatedResults, families, result);
      idx++;
    }

    ResultLineVector resultsToProcess = topResults + bestUnrelatedResults;

    ResultLineList bestTrimmedResults (false, 20);
    for  (idx = 0;  idx < (int)resultsToProcess.size ();  idx++)
    {
      ResultLinePtr  result = resultsToProcess[idx];
      int featureCount = result->Features ()->NumOfFeatures ();

      int  x; 
      float*  highs = new float[featureCount];
      for  (x = 0;  x < featureCount;  x++)
        highs[x] = 0.0f;

      ResultLinePtr  trimmedResult = TrimAFeatureSetInDepth 
                    (20, // depth
                     1,   // width
                     result,
                     Accuracy (result),
                     highs
                    );

      bestTrimmedResults.PushOnBack (trimmedResult);

      log.Level (10) << "RandomSearch::Run1   Done Trimming  [" << result->Features ()->ToString () << endl;

      *resultFile << "//"  << endl
                  << "//";

      for  (x = 0;  x < featureCount;  x++)
        *resultFile << "\t" << x;
      *resultFile << endl;

      *resultFile << "//";
      for  (x = 0;  x < featureCount;  x++)
        *resultFile << "\t" << highs[x];
      *resultFile <<         endl
                  << "//" << endl;

      delete  highs;

      idx++;
    }

    bestTrimmedResults.SortByAccuracy (false, accuracyWeighted);

    ResultLinePairVector  pairs = BuildPairsFromList (bestTrimmedResults);
    ResultLineList  combinations = EvaluateCombinations (pairs);

    for  (idx = 0;  idx < (int)combinations.size ();  idx++)
    {
      ResultLine& result = combinations[idx];

      ResultLinePtr  trimmedResult = TrimAFeatureSetInDepth (200, // depth
                                                             1,   // width
                                                             &result,
                                                             Accuracy (&result),
                                                             NULL
                                                            );
    }
  }
}  /* Run1 */




void  RandomSearch::Run2 ()
{
  if  (results->Size () < NumOfRandomOnesToCreate)
  {
    while  (results->Size () < NumOfRandomOnesToCreate)
    {
      RunARandomlySelectedSetOfFeatures ();
    }
  }

  if  (results->Size () < (NumOfRandomOnesToCreate + 20))
  {
    while  (results->Size () < (NumOfRandomOnesToCreate + 20))
    {
      // Only interested in combining the originally radomly selected results, which 
      // at this point is all that there should be abailable.
      CombineSomeOfTheBestJobs (10,              // Combine 10 of the best that have no
                                randomFeatures   // more than 'randomFeatures' features.
                               );
    }
  }

  {
    ResultLineList    sortedResults (results, 10 * randomFeatures, 41770);
    sortedResults.SortByAccuracy (false, accuracyWeighted);

    ResultLineVector  topResults;
    int idx = 0;
    while  ((idx < sortedResults.QueueSize ())  &&  (topResults.size () < 10))
    {
      ResultLinePtr  result = sortedResults.IdxToPtr (idx);
      topResults.push_back (result);
      idx++;
    }

    ResultLineVector  bestUnrelatedResults;
    VectorInt         families;
    idx = 0;
    while  ((idx < sortedResults.QueueSize ())  &&  (bestUnrelatedResults.size () < 10))
    {
      ResultLinePtr  result = sortedResults.IdxToPtr (idx);
      AddToCombinationsIfNotRelated (2,  bestUnrelatedResults, families, result);
      idx++;
    }

    ResultLineVector resultsToProcess = topResults + bestUnrelatedResults;

    ResultLineList bestTrimmedResults (false, 20);
    for  (idx = 0;  idx < (int)resultsToProcess.size ();  idx++)
    {
      ResultLinePtr  result = resultsToProcess[idx];
      int featureCount = result->Features ()->NumOfFeatures ();

      int  x; 
      float*  highs = new float[featureCount];
      for  (x = 0;  x < featureCount;  x++)
        highs[x] = 0.0f;

      ResultLinePtr  trimmedResult = TrimAFeatureSetInDepth 
                    (200, // depth
                     1,   // width
                     result,
                     Accuracy (result),
                     highs
                    );

      bestTrimmedResults.PushOnBack (trimmedResult);

      log.Level (10) << "RandomSearch::Run1   Done Trimming  [" << result->Features ()->ToString () << endl;

      *resultFile << "//"  << endl
                  << "//";

      for  (x = 0;  x < featureCount;  x++)
        *resultFile << "\t" << x;
      *resultFile << endl;

      *resultFile << "//";
      for  (x = 0;  x < featureCount;  x++)
        *resultFile << "\t" << highs[x];
      *resultFile <<         endl
                  << "//" << endl;

      delete  highs;
    }

    bestTrimmedResults.SortByAccuracy (false, accuracyWeighted);

    ResultLinePairVector  pairs = BuildPairsFromList (bestTrimmedResults);
    ResultLineList  combinations = EvaluateCombinations (pairs);

    for  (idx = 0;  idx < (int)combinations.size ();  idx++)
    {
      ResultLine& result = combinations[idx];

      ResultLinePtr  trimmedResult = TrimAFeatureSetInDepth (200, // depth
                                                             1,   // width
                                                             &result,
                                                             Accuracy (&result),
                                                             NULL
                                                            );
    }
  }
}  /* Run2 */




void  RandomSearch::Run3 ()
{
  ExpandedUnSetAll ();

  if  (results->Size () < NumOfRandomOnesToCreate)
  {
    while  (results->Size () < NumOfRandomOnesToCreate)
    {
      RunARandomlySelectedSetOfFeatures ();
    }
  }

  {
    int  topNumToCombine = 10;
    // Combine the best 10 and make a supper FeatureSet;

    {
      // First, make sure that the best have all been verified.
      bool  allHaveBeenVerified = false;

      while  (!allHaveBeenVerified)
      {
        allHaveBeenVerified = true;

        ResultLineList  bestResults (results, randomFeatures, NumOfRandomOnesToCreate);
        bestResults.SortByAccuracy (false, accuracyWeighted);
        for  (int x = 0;  x < (topNumToCombine * 10);  x++)
        {
          if  (!bestResults[x].Verified ())
          {
            allHaveBeenVerified = false;
            CheckAgainstOtherOrderings (&(bestResults[x]));
            *resultFile << bestResults[x].ToString () << endl;
          }
        }
      }
    }
   
    FeatureNumList  featuresOfBest20 (fileDesc);

    ResultLineList  bestResults (results, randomFeatures, NumOfRandomOnesToCreate);
    bestResults.SortByAccuracy (false, accuracyWeighted);

    for  (int x = 0;  ((x < topNumToCombine)  &&  (x < bestResults.QueueSize ())); x++)
    {
      ResultLine&  result = bestResults[x];
      featuresOfBest20 += *(result.Features ());
    }

    ResultLinePtr  best20Result = EvaluateAFeatureSet (&featuresOfBest20, NULL, NULL, NULL);

    PerformWrapper (best20Result);
  }
}  /* Run3 */









void  RandomSearch::Run ()
{
  Run1 ();


  // Will now go into a loop that we will stay in until the 
  // program is killed.
  while  (true)
  {
    int x;
    CombineSomeOfTheBestJobs (3, randomFeatures * 10);
    //TrySomeCombinationsOfTheBetterOnesFoundSofar (randomFeatures * 10);
    for  (x = 0;  x < 3;  x++)
    {
      // Trim th eresults of the three best results in the system
      // that are not related to each other for at least 3
      // families.
      TrimBestNotExpanded (3, randomFeatures * 10);
    }
  }
}  /* Run */






// -c survival.cfg -aw -df survival_TestData.data -format c45 -folds 5 -RW 100 -TF survival_ValidationData.data

int  main (int argc, char **argv)
{
  RandomSearch*  featureSelection = new RandomSearch (argc, argv);
  featureSelection->Run ();
  delete  featureSelection;
  return 0;
}
