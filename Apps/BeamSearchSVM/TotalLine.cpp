#include  "FirstIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include  <fstream>
#ifdef  WIN32
#include  <ostream>
#endif
#include  <iostream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "BasicTypes.h"
#include  "ResultLine.h"


#include  "TotalLine.h"

#include  "CrossValidation.h"
#include  "CrossValidationThreeWay.h"
#include  "FeatureNumList.h"
#include  "ImageClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"
#include  "TrainingConfiguration.h"
#include  "OSservices.h"



TotalLine::TotalLine (FileDescPtr               _fileDesc,
                      ImageClassListPtr         _classes,
                      RunLog&                   _log,
                      TrainingConfigurationPtr  _config,
                      ImageFeaturesListPtr      _testData,
                      ImageFeaturesListPtr      _validationData,
                      bool                      _useGrading
                     ):
  
  classes                    (new ImageClassList (*_classes)),
  config                     (_config),
  fileDesc                   (_fileDesc),
  log                        (_log),
  maxNumOfFeatures           (-1),
  numOfResults               (0),
  statisticallySameJob       (NULL),
  featureCountWithStatisticlySame (0),
  testDataInOurClasses       (NULL),
  validationDataInOurClasses (NULL),
  jobsWithHighestGrade       (NULL),
  gradesByNumOfFeatuers      (NULL),
  useGrading                 (_useGrading)

{
  maxNumOfFeatures = fileDesc->NumOfFields ();

  ussageInTop100          = new int           [maxNumOfFeatures];
  ussageInTop200          = new int           [maxNumOfFeatures];
  ussageInTop100Grades    = new int           [maxNumOfFeatures];
  ussageInTop200Grades    = new int           [maxNumOfFeatures];
  accuracyByNumOfFeatuers = new FFLOAT        [maxNumOfFeatures + 1];
  jobsWithHighestAccuracy = new ResultLinePtr [maxNumOfFeatures + 1];
  jobsWithHighestGrade    = new ResultLinePtr [maxNumOfFeatures + 1];
  gradesByNumOfFeatuers   = new float         [maxNumOfFeatures + 1];
  chgInAccuracies         = new FFLOATVector  [maxNumOfFeatures];

  for  (int featureNum = 0;  featureNum < maxNumOfFeatures;  featureNum++)
  {
    ussageInTop100          [featureNum] = 0;
    ussageInTop200          [featureNum] = 0;
    ussageInTop100Grades    [featureNum] = 0;
    ussageInTop200Grades    [featureNum] = 0;
    accuracyByNumOfFeatuers [featureNum] = (FFLOAT)0.0;
    jobsWithHighestAccuracy [featureNum] = NULL;
    jobsWithHighestGrade    [featureNum] = NULL;
    gradesByNumOfFeatuers   [featureNum] = NULL;
  }

  accuracyByNumOfFeatuers [maxNumOfFeatures] = (FFLOAT)0.0;
  gradesByNumOfFeatuers   [maxNumOfFeatures] = (FFLOAT)0.0;
  jobsWithHighestAccuracy [maxNumOfFeatures] = NULL;
  jobsWithHighestGrade    [maxNumOfFeatures] = NULL;


  if  (_testData)
  {
    testDataInOurClasses = new ImageFeaturesList (*classes, *_testData, log);

    if  (_validationData)
    {
      if  (useGrading)
        validationDataInOurClasses = new ImageFeaturesList (*_validationData, false);
      else
        validationDataInOurClasses = new ImageFeaturesList (*classes, *_validationData, log);
    }
  }
}





TotalLine::~TotalLine ()
{
  delete[]  ussageInTop100;
  delete[]  ussageInTop200;
  delete[]  ussageInTop100Grades;
  delete[]  ussageInTop200Grades;
  delete[]  accuracyByNumOfFeatuers;
  delete[]  gradesByNumOfFeatuers;

  for  (int x = 0;  x < maxNumOfFeatures;  x++)
  {
    delete  jobsWithHighestAccuracy[x];
    delete  jobsWithHighestGrade[x];
  }

  delete  statisticallySameJob;

  delete[]  jobsWithHighestAccuracy;
  delete[]  jobsWithHighestGrade;
  delete[]  chgInAccuracies;

  delete  testDataInOurClasses;
  delete  validationDataInOurClasses;

  delete  classes;
}



KKStr  TotalLine::emptyStr = "";



const
ImageClassPtr  TotalLine::Class1 ()   const
{
  if  (classes->QueueSize () < 1)
    return NULL;
  else
    return  classes->IdxToPtr (0);
}



const
ImageClassPtr  TotalLine::Class2 ()  const
{
  if  (classes->QueueSize () < 2)
    return NULL;
  else
    return  classes->IdxToPtr (1);
}



void  TotalLine::StatisticallySameJob (int                  _featureCount,
                                       const ResultLinePtr  _statisticallySameJob
                                      )
{
  delete  statisticallySameJob;
  featureCountWithStatisticlySame = _featureCount;
  statisticallySameJob = new ResultLine (*_statisticallySameJob, log);
}  /* StatisticallySameJob */





FeatureNumList  TotalLine::FeatureNumListFromTop200 (int  minNumOfApperances)
{
  FeatureNumList  features (fileDesc);

  int  featureNum = 1;
  for  (featureNum = 0;  featureNum < maxNumOfFeatures;  featureNum++)
  {
    if  (UssageInTop200 (featureNum) >= minNumOfApperances)
    {
      features.AddFeature (featureNum);
    }
  }

  return  features;
}  /* FeatureNumListFromTop200 */




//*****************************************************************************************************************
//*  classesCorrectly   - two dimensional array [maxNumOfFeatures x validationDataInOurClasses->QueueSize ()]     *
//*                       Each row represents the classification results for a given feature count.               *
//*                       each element in a row coresponds to a example in validationDataInOurClasses             *
//*                                                                                                               *
//*  We have the results of MaxNumOfFeatures test, one for each number of features 1..MaxNumOfFeatures            *
//*  A McNemars test will be performed between each combination.  Ex if MaxNumOfFeatures = 4 then there will be   *
//*  a mcNemars test done for (1,2), (1,3), (1,4), (2,3), (2,4), and (3,4).                                       *
//*                                                                                                               *
//*  The results will be returned in a 2 dimensional array such that results[x,y] will represent the mcNemar's    *
//*  test value between classifers for x numOfFeatures and y numOfFeatures.                                       *
//*                                                                                                               *
//*                                                                                                               *
//*                                                                                                               *
//*****************************************************************************************************************

double**  TotalLine::PerformMcNemarsTest (ostream&              o,
                                          bool**                classedCorrectly,
                                          ImageFeaturesListPtr  testDataInOurClasses,
                                          ImageFeaturesListPtr  validationDataInOurClasses
                                         )
{
  log.Level (10) << "TotalLine::PerformMcNemarsTest" << endl;
  
  int  fc1, fc2;
  int  x;


  // Build a mask that will indicate which validation images are of a class
  // that is in the testData.  This is because the testData was used to train 
  // with.  And in the case if binary feature selection, we can posibly
  // have classes in the vcalidation data that does not belong.
  bool*  validationMask = new bool[validationDataInOurClasses->QueueSize ()];

  {
    ImageClassListPtr  trainingClasses = testDataInOurClasses->ExtractListOfClasses ();

    for  (x = 0;  x < validationDataInOurClasses->QueueSize ();  x++)
    {
      if  (classes->PtrToIdx (validationDataInOurClasses->IdxToPtr (x)->ImageClass ()) < 0)
        validationMask[x] = false;
      else
        validationMask[x] = true;
    }
    delete  trainingClasses;
  }


  double**  mcNemarsTable = new double*[maxNumOfFeatures + 1];
  mcNemarsTable[0] = NULL;
  for  (fc1 = 1; fc1 <= maxNumOfFeatures; fc1++)
  {
    mcNemarsTable[fc1] = new double[maxNumOfFeatures + 1];
    for  (fc2 = 1; fc2 <= maxNumOfFeatures; fc2++)
      mcNemarsTable[fc1][fc2] = 0.0;
  }

  o << endl;

  for  (x = 1;  x <=  maxNumOfFeatures;  x++)
    o << "\t" << x;
  o << endl;


  for  (fc1 = 1;  fc1 <=  maxNumOfFeatures;  fc1++)
  {
    if (!classedCorrectly[fc1])
      continue;

    o << fc1;

    for  (fc2 = 1;  fc2 <=  maxNumOfFeatures;  fc2++)
    {
      if (!classedCorrectly[fc2])
        continue;

      if  (fc1 == fc2)
      {
        o << "\t";
        continue;
      }

      int  n00 = 0;
      int  n01 = 0;
      int  n10 = 0;
      int  n11 = 0;

      for  (x = 0;  x < validationDataInOurClasses->QueueSize ();  x++)
      {
        if  (!validationMask[x])
          continue;

        bool  ca = classedCorrectly[fc1][x];
        bool  cb = classedCorrectly[fc2][x];

        if  (!ca && !cb)
          n00++;   // true neg.

        else if  (!ca && cb)
          n01++;   // false pos.

        else if  (ca && !cb)
          n10++;   // false neg.

        else if  (ca && cb)
          n11++;   // true pos
      }

      double y = fabs ((double)(n01 - n10)) - 1.0;    // (false pos - false neg) - 1.0;

      double  mcNemars = 0.0;
      double  divisor = n01 + n10;

      if  (divisor != 0.0)
        mcNemars =  y * y / divisor;

      o << "\t" << mcNemars;
      mcNemarsTable[fc1][fc2] = mcNemars;
    }

    o << endl;
  }

  o << endl;

  delete  validationMask;
  return  mcNemarsTable;
}  /* PerformMcNemarsTest */





float**  TotalLine::PerformTTest (ostream&              o,
                                  float**               foldAccuracies,
                                  ImageFeaturesListPtr  testDataInOurClasses
                                 )
{
  log.Level (10) << "TotalLine::PerformMcNemarsTest" << endl;

  int  fc1, fc2;
  int  x;


  float**  tTestTable = new float*[maxNumOfFeatures + 1];
  tTestTable[0] = NULL;
  for  (fc1 = 1; fc1 <= maxNumOfFeatures; fc1++)
  {
    tTestTable[fc1] = new float[maxNumOfFeatures + 1];
    for  (fc2 = 1; fc2 <= maxNumOfFeatures; fc2++)
      tTestTable[fc1][fc2] = 0.0;
  }

  o << endl;

  for  (x = 1;  x <=  maxNumOfFeatures;  x++)
    o << "\t" << x;
  o << endl;


  for  (fc1 = 1;  fc1 <=  maxNumOfFeatures;  fc1++)
  {
    if (!foldAccuracies[fc1])
      continue;

    o << fc1;

    float*  xFolds = foldAccuracies[fc1];

    for  (fc2 = 1;  fc2 <=  maxNumOfFeatures;  fc2++)
    {
      if (!foldAccuracies[fc2])
        continue;

      float*  yFolds = foldAccuracies[fc2];

      if  (fc1 == fc2)
      {
        o << "\t";
        continue;
      }

      float  xTotal = 0.0f;
      float  yTotal = 0.0f;

      int  foldNum = 0;
      for  (foldNum = 0;  foldNum < NumOfValidationFolds;  foldNum++)
      {
        xTotal += xFolds[foldNum];
        yTotal += yFolds[foldNum];
      }
      float  xMean = xTotal / NumOfValidationFolds;
      float  yMean = yTotal / NumOfValidationFolds;

      float  totalDeltaDeltaSquared = 0.0f;

      for  (foldNum = 0;  foldNum < NumOfValidationFolds;  foldNum++)
      {
        float  xDelta = xFolds[foldNum] - xMean;
        float  yDelta = yFolds[foldNum] - yMean;

        float  deltaDelta = xDelta - yDelta;
        totalDeltaDeltaSquared += (deltaDelta * deltaDelta);
      }

      float tValue = 0.0f;
      if  (totalDeltaDeltaSquared != 0)
        tValue = fabs ((xMean - yMean) * sqrt ((NumOfValidationFolds * (NumOfValidationFolds - 1)) / totalDeltaDeltaSquared));

      o << "\t" << tValue;
      tTestTable[fc1][fc2] = tValue;
    }

    o << endl;
  }

  o << endl;

  return  tTestTable;
}  /* PerformTTest */




void  TotalLine::CrossValidationsForTop200 (ostream&  o)
{
  log.Level (10) << "CrossValidationsForTop200" << endl;

  if  (!testDataInOurClasses)
    return;

  int  minAppearances[] = {50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, -1};
  
  vector<double>          accuracies;
  vector<double>          trainingTimes;
  vector<double>          classificationTimes;
  vector<int>             supportPoints;
  vector<FeatureNumList>  featureNumLists;

  uint  runNum = 0;

  bool  cancelFlag = false;


  while  ((minAppearances[runNum] > 0)  &&  (minAppearances[runNum] <= 200))
  {
    int  minAppearance  = minAppearances[runNum];
    FeatureNumList  features = FeatureNumListFromTop200 (minAppearance);

    log.Level (10) << "CrossValidationsForTop200  Appearances[" << minAppearance << "]  "
                   << "FeatureCount[" << features.NumOfFeatures () << "]  "
                   << "Features[" << features.ToString () << "]" 
                   << endl;

    config->SetFeatureNums (features);

    CrossValidationPtr  cv = NULL;
    if  (validationDataInOurClasses)
    {
      cv = new CrossValidation (config, testDataInOurClasses, classes, 1, false, fileDesc, log, cancelFlag);
      cv->RunValidationOnly (validationDataInOurClasses, NULL);
    }
    else
    {
      cv = new CrossValidation (config, testDataInOurClasses, classes, 10, false, fileDesc, log, cancelFlag);
      cv->RunCrossValidation ();
    }

    //o << endl
    //  << endl
    //  << "In Top 200 [" << minAppearance << "]   Features[" << features.ToString () << "]" << endl
    //  << endl;
    //cv->ConfussionMatrix ()->PrintConfusionMatrixTabDelimited (o);
    
    accuracies.push_back          (cv->Accuracy ());
    trainingTimes.push_back       (cv->TrainTimeTotal  ());
    classificationTimes.push_back (cv->TestTimeTotal ());
    supportPoints.push_back       (cv->NumOfSupportVectors ());
    featureNumLists.push_back     (features);

    delete  cv;  cv = NULL;

    runNum++;
  }

  o << endl
    << endl
    << endl;

  o << "MinimumIn200"        << "\t"
    << "Accuracy"            << "\t"
    << "TrainingTime"        << "\t"
    << "ClassificationTime"  << "\t"
    << "SupportPoints"       << "\t"
    << "FeatureCount"        << "\t"
    << "FeatureNums"       
    << endl;

  for  (runNum = 0;  runNum < accuracies.size ();  runNum++)
  {
    o << minAppearances      [runNum]                  << "\t"
      << accuracies          [runNum] << "%"           << "\t"
      << trainingTimes       [runNum]                  << "\t"
      << classificationTimes [runNum]                  << "\t"
      << supportPoints       [runNum]                  << "\t"
      << featureNumLists     [runNum].NumOfFeatures () << "\t"
      << featureNumLists     [runNum].ToString ()
      << endl;
  }

  o << endl << endl;
}  /* CrossValidationsForTop200 */






void  TotalLine::PrintHighestAccuraciesByNumOfFeatures (ostream&  o,
                                                        int       upOneCount,
                                                        ostream&  sumResults
                                                       )
{
  log.Level (10) << "TotalLine::PrintHighestAccuraciesByNumOfFeatures [" << classes->ToString () << "]." << endl;

  int  x;

  double  highestAccuracy = -99999.99;
  int  featureCountWithHighestAccuracy = 0;

  
  bool**    classedCorrect = NULL;
  double**  mcNemarsTable  = NULL;
  float**   folds          = NULL;
  float**   tTestTable     = NULL;

  bool      cancelFlag = false;

  ImageFeaturesListPtr  validationDataToUse = NULL;
  if  (validationDataInOurClasses)
    validationDataToUse = new ImageFeaturesList (*classes, *validationDataInOurClasses, log);

  x = classes->QueueSize ();

  if  (config  &&  testDataInOurClasses)
  {
    int  numOfTestImages = testDataInOurClasses->QueueSize ();
    if  (validationDataToUse)
      numOfTestImages = validationDataToUse->QueueSize ();

    if  (testDataInOurClasses)
    {
      if  (validationDataToUse)
      {
        classedCorrect = new bool*[maxNumOfFeatures + 1];
        for  (x = 0;  x <= maxNumOfFeatures;  x++)
          classedCorrect[x] = NULL;
      }
      else
      {
        folds = new float*[maxNumOfFeatures + 1];
        for  (x = 0;  x <= maxNumOfFeatures;  x++)
          folds[x] = NULL;
      }
    }

    for  (x = 1;  x <= maxNumOfFeatures;  x++)
    {
      ResultLinePtr  j = jobsWithHighestAccuracy[x];

      if  (j)
      {
        log.Level (10) << "PrintHighestAccuraciesByNumOfFeatures" 
                       << "  NumOfFeatures[" << x << "]  "
                       << "  Features[" << j->GetFeatureNums ().ToString () << "]"
                       << endl;
                          

        config->SetFeatureNums (j->GetFeatureNums ());

        CrossValidationPtr cv = NULL;
        if  (validationDataToUse)
        {
          classedCorrect[x] = new bool[numOfTestImages];
          cv = new CrossValidation (config, testDataInOurClasses, classes, 1, false, fileDesc, log, cancelFlag);
          cv->RunValidationOnly (validationDataToUse, classedCorrect[x]);
        }
        else
        {
          cv = new CrossValidation (config, testDataInOurClasses, classes, NumOfValidationFolds, false, fileDesc, log, cancelFlag);
          cv->RunCrossValidation ();
          folds[x] = new float[NumOfValidationFolds];
          int  y;
          for  (y = 0;  y < NumOfValidationFolds;  y++)
            folds[x][y] = cv->FoldAccuracy (y);
        }


        float  accuracy = cv->Accuracy ();

        j->TestAccuracy  (accuracy);
        j->TrainTime     (cv->TrainTimeTotal ());
        j->TestTime      (cv->TestTimeTotal ());
        j->SupportPoints ((float)cv->NumOfSupportVectors ());

        if  (accuracy > highestAccuracy)
        {
          highestAccuracy = accuracy;
          featureCountWithHighestAccuracy = x;
        }

        ImageClassPtr  class1 = classes->IdxToPtr (0);
        ImageClassPtr  class2 = classes->IdxToPtr (1);
        
        int  numOfClass1 = testDataInOurClasses->NumEntriesOfAGivenClass (class1);
        int  numOfClass2 = testDataInOurClasses->NumEntriesOfAGivenClass (class2);

        sumResults << "Class1"       << "\t" << class1->Name           () << "\t"
                   << "Class2"       << "\t" << class2->Name           () << "\t"
                   << "Class1Count"  << "\t" << numOfClass1               << "\t"
                   << "Class2Count"  << "\t" << numOfClass2               << "\t"
                   << "JobId"        << "\t" << j->JobId               () << "\t"
                   << "TrainAcc"     << "\t" << j->Accuracy            () << "\t"
                   << "TestAcc"      << "\t" << j->TestAccuracy        () << "\t"
                   << "SP"           << "\t" << j->SupportPoints       () << "\t"
                   << "Folds"        << "\t" << cv->FoldAccuracysToStr () << "\t"
                   << "FC"           << "\t" << x                         << "\t"
                   << "Features"     << "\t" << j->GetFeatureNums ().ToString ()
                   << endl;

        delete  cv;  cv = NULL;
      }
    }
    

    if  (classedCorrect)
    {
      mcNemarsTable = PerformMcNemarsTest (o, classedCorrect, testDataInOurClasses, validationDataToUse);
    }

    else if  (folds)
    {
      tTestTable = PerformTTest (o, folds, testDataInOurClasses);
    }
  }


  o << endl;
  o << "NumOf" << "\t" << "JobId" 
               << "\t" << "BeamResult"
               << "\t" << "Accuracy" 
               << "\t" << "TestAccuracy"
               << "\t" << "AccuracyFlag"
               << "\t" << "MnNemars"
               << "\t" << "TestTrainTime"
               << "\t" << "TestClassTime"
               << "\t" << "SupportPoints"
               << "\t" << "SelectedFeatures" 
               << endl;

  int  smallestFeatureCountToUse = featureCountWithHighestAccuracy;

  for  (int x = 1;  x <= maxNumOfFeatures;  x++)
  {
    double  mcNemarsValue = 0.0;
    float   tTestValue    = 0.0f;
    if  (mcNemarsTable)
      mcNemarsValue = mcNemarsTable[x][featureCountWithHighestAccuracy];

    else if  (tTestTable)
      tTestValue = tTestTable[x][featureCountWithHighestAccuracy];

    ResultLinePtr  j = jobsWithHighestAccuracy[x];
    if  (j)
    {
      KKStr  accuracyFlag = "";

      if  (x == featureCountWithHighestAccuracy)
      {
        accuracyFlag = "*HIGHEST*";
      }

      else
      {
        if  (smallestFeatureCountToUse >= featureCountWithHighestAccuracy)
        {
          if  ((mcNemarsTable)  &&  (mcNemarsValue < 3.841459))
          {
            smallestFeatureCountToUse = x;
            accuracyFlag = "*The-Same*";
            StatisticallySameJob (x, j);
          }

          else if  ((tTestTable)  &&  (tTestValue < 1.37218))
          {
            smallestFeatureCountToUse = x;
            accuracyFlag = "*The-Same*";
            StatisticallySameJob (x, j);
          }
        }
      }

      KKStr  beamResult = "";
      if  (j->BeamSearch ())
        beamResult = "BeamResult";

      // kkkkkk
      o << x << "\t" 
        << j->JobId () << "\t"
        << beamResult  << "\t"
        << StrFormatDouble (j->Accuracy (),      "ZZZZ0.000") << "%" << "\t"
        << StrFormatDouble (j->TestAccuracy (),  "ZZZZ0.000") << "%" << "\t"
        << accuracyFlag                                              << "\t"
        << mcNemarsValue                                             << "\t"
        << StrFormatDouble (j->TrainTime     (), "ZZZZ0.000")        << "\t"
        << StrFormatDouble (j->TestTime      (), "ZZZZ0.000")        << "\t"
        << j->SupportPoints ()                                       << "\t"
        << j->GetFeatureNums ().ToString ()
        << endl;
    }
  }

  o << endl
    << "Highest Accuracy  Feature Count  [" << featureCountWithHighestAccuracy << "]  Features[" 
    << jobsWithHighestAccuracy[featureCountWithHighestAccuracy]->GetFeatureNums ().ToString () << "]" << endl
    << "Statistically Same Feature Count [" << smallestFeatureCountToUse       << "]  Features[" 
    << jobsWithHighestAccuracy[smallestFeatureCountToUse]->GetFeatureNums ().ToString () << "]" << endl
    << endl
    << endl;


  if  (classedCorrect)
  {
    for  (int fc = 0;  fc <= maxNumOfFeatures;  fc++)
    {
      delete  classedCorrect[fc];
      delete  mcNemarsTable[fc];
    }
    delete  classedCorrect;
    delete  mcNemarsTable;
  }

  sumResults << endl;

  if  (folds)
  {
    for  (int fc = 0;  fc <= maxNumOfFeatures;  fc++)
    {
      delete  folds[fc];
      delete  tTestTable[fc];
    }
    delete  tTestTable;
    delete  folds;
  }

  delete  validationDataToUse;
}  /* PrintHighestAccuraciesByNumOfFeatures */





void  TotalLine::PrintHighestGradesByNumOfFeatures (ostream&       o,
                                                    ImageClassPtr  class1,
                                                    ImageClassPtr  class2,
                                                    float          punishmentFactor,
                                                    int            upOneCount
                                                   )
{
  log.Level (10) << "TotalLine::PrintHighestGradesByNumOfFeatures [" << classes->ToString () << "]." << endl;

  int  x;


  ImageClassListPtr  allClasses = new ImageClassList ();  // We will not own the contents
  allClasses->PushOnBack (class1);
  allClasses->PushOnBack (class2);

  if  (validationDataInOurClasses)
  {
    // Lets get list of all classes from validationImages,  we neeed to do this because when validating
    // binary classifiers we still test against all classes.  

    ClassStatisticListPtr  classStats = validationDataInOurClasses->GetClassStatistics ();
    ClassStatisticList::iterator  idx;

    for  (idx = classStats->begin ();  idx != classStats->end ();  idx++)
    {
      ImageClassPtr  imageClass = (*idx)->ImageClass ();
      if  (allClasses->PtrToIdx (imageClass) < 0)  
          allClasses->PushOnBack (imageClass);
    }

    delete  classStats;
  }

  else if  (testDataInOurClasses)
  {
    ImageClassListPtr  classesInTestData = testDataInOurClasses->ExtractListOfClasses ();
    ImageClassList::iterator  idx;

    for  (idx = classesInTestData->begin ();  idx != classesInTestData->end ();  idx++)
    {
      if  (allClasses->PtrToIdx (*idx) < 0)  
         allClasses->PushOnBack (*idx);
    }
    delete  classesInTestData;
  }


  double  highestGrade = -99999.99;
  int  featureCountWithHighestGrade = 0;

  bool**  classedCorrect = NULL;
  double** mcNemarsTable = NULL;

  if  (config  &&  testDataInOurClasses)
  {
    int  numOfTestImages = testDataInOurClasses->QueueSize ();
    if  (validationDataInOurClasses)
      numOfTestImages = validationDataInOurClasses->QueueSize ();

    if  (testDataInOurClasses  &&  validationDataInOurClasses)
    {
      classedCorrect = new bool*[maxNumOfFeatures + 1];
      for  (x = 0;  x <= maxNumOfFeatures;  x++)
        classedCorrect[x] = NULL;
    }

    for  (x = 1;  x <= maxNumOfFeatures;  x++)
    {
      ResultLinePtr  j = jobsWithHighestGrade[x];

      if  (j)
      {
        log.Level (10) << "PrintHighestGradessByNumOfFeatures" 
                       << "  NumOfFeatures[" << x << "]  "
                       << "  Features[" << j->GetFeatureNums ().ToString () << "]"
                       << endl;
                          
        config->SetFeatureNums (j->GetFeatureNums ());

        CrossValidationThreeWayPtr cv = NULL;
        if  (validationDataInOurClasses)
        {
          classedCorrect[x] = new bool[numOfTestImages];

          cv = new CrossValidationThreeWay (config,
                                            false,
                                            testDataInOurClasses, 
                                            allClasses, 
                                            class1,
                                            class2,
                                            1,      // One Fold
                                            false,  // Features are not already normalized.
                                            false,  // Don't stratafy
                                            punishmentFactor,
                                            log
                                           );
          cv->RunValidationOnly (validationDataInOurClasses, classedCorrect[x]);

        }
        else
        {
          cv = new CrossValidationThreeWay (config, 
                                            false, 
                                            testDataInOurClasses,
                                            allClasses,
                                            class1,
                                            class2,
                                            10,
                                            false,
                                            false,
                                            punishmentFactor,
                                            log
                                           );
          cv->RunValidation ();

        }

        float  otherClassProbMean = 0.0f;
        float  stdDev = 0.0f;

        if  (validationDataInOurClasses)
          cv->CalculateThirdClassDecisiveProbability (validationDataInOurClasses, otherClassProbMean, stdDev);
        else
          cv->CalculateThirdClassDecisiveProbability (otherClassProbMean, stdDev);

        float  grade = cv->Grade ();

        j->TestGrade          (100.0f * grade);
        j->OtherClassProbMean (100.0f * otherClassProbMean);
        j->TrainTime          (cv->TrainingTime ());
        j->TestTime           (cv->ClassificationTime ());
        j->SupportPoints      ((float)cv->NumOfSupportVectors ());

        if  (grade > highestGrade)
        {
          highestGrade = grade;
          featureCountWithHighestGrade = x;
        }

        delete  cv;
      }
    }
    

    if  (classedCorrect)
    {
      mcNemarsTable = PerformMcNemarsTest (o, classedCorrect, testDataInOurClasses, validationDataInOurClasses);
    }
  }


  o << endl;
  o << "NumOf" << "\t" << "JobId" 
               << "\t" << "BeamResult"
               << "\t" << "Accuracy" 
               << "\t" << "TestAccuracy"
               << "\t" << "TestGrade"
               << "\t" << "AccuracyFlag"
               << "\t" << "MnNemars"
               << "\t" << "TestTrainTime"
               << "\t" << "TestClassTime"
               << "\t" << "SelectedFeatures" 
               << endl;

  int  smallestFeatureCountToUse = featureCountWithHighestGrade;

  for  (int x = 1;  x <= maxNumOfFeatures;  x++)
  {
    double  mcNemarsValue = 0.0;
    if  (mcNemarsTable)
    {
      mcNemarsValue = mcNemarsTable[x][featureCountWithHighestGrade];
    }

    ResultLinePtr  j = jobsWithHighestGrade[x];
    if  (j)
    {
      KKStr  accuracyFlag = "";

      if  (x == featureCountWithHighestGrade)
      {
        accuracyFlag = "*HIGHEST*";
      }

      else
      {
        if  (smallestFeatureCountToUse >= featureCountWithHighestGrade)
        {
          if  (mcNemarsValue < 3.841459)
          {
            smallestFeatureCountToUse = x;   // kkkkkk  Need to chg back to just x  not 'x + upOneCount'
            accuracyFlag = "*The-Same*";
            StatisticallySameJob (x, j);
          }
        }
      }

      KKStr  beamResult = "";
      if  (j->BeamSearch ())
        beamResult = "BeamResult";

      // kkkkkk
      o << x << "\t" 
        << j->JobId () << "\t"
        << beamResult  << "\t"
        << StrFormatDouble (j->Accuracy           (),  "ZZZZ0.000") << "%" << "\t"
        << StrFormatDouble (j->TestAccuracy       (),  "ZZZZ0.000") << "%" << "\t"
        << StrFormatDouble (j->TestGrade          (),  "ZZZZ0.000") << "%" << "\t"
        << StrFormatDouble (j->OtherClassProbMean (),  "ZZZZ0.000") << "%" << "\t"
        << accuracyFlag                                                    << "\t"
        << mcNemarsValue                                                   << "\t"
        << StrFormatDouble (j->TrainTime          (), "ZZZZ0.000")         << "\t"
        << StrFormatDouble (j->TestTime           (), "ZZZZ0.000")         << "\t"
        << j->GetFeatureNums ().ToString ()
        << endl;
    }
  }

  o << endl
    << "Highest Accuracy  Feature Count  [" << featureCountWithHighestGrade << "]  Features[" 
    << jobsWithHighestGrade[featureCountWithHighestGrade]->GetFeatureNums ().ToString () << "]" << endl
    << "Statistically Same Feature Count [" << smallestFeatureCountToUse       << "]  Features[" 
    << jobsWithHighestGrade[smallestFeatureCountToUse]->GetFeatureNums ().ToString () << "]" << endl
    << endl
    << endl;


  if  (classedCorrect)
  {
    for  (int fc = 0;  fc <= maxNumOfFeatures;  fc++)
    {
      delete  classedCorrect[fc];
      delete  mcNemarsTable[fc];
    }
    delete  classedCorrect;
    delete  mcNemarsTable;
  }

  delete  allClasses;
}  /* PrintHighestGradesByNumOfFeatures */





FeatureNumList  TotalLine::FeatureForBestAccurcayFoundFeatureCount (int  featureCount) const
{
  ResultLinePtr  j = jobsWithHighestAccuracy[featureCount];
  return  j->GetFeatureNums ();
}



int  TotalLine::BestFeatureSetFoundFeatureCount ()  const
{
  int   highestAccuracyFeatureCount = 0;
  float highestAccuracy = 0.0f;

  int  featureCountWithHighestAccuracy = -1;

  for  (int x = 1;  x <= maxNumOfFeatures;  x++)
  {
    ResultLinePtr  j = jobsWithHighestAccuracy[x];
    if  (j)
    {
      if  (j->Accuracy () > highestAccuracy)
      {
        featureCountWithHighestAccuracy = x;
        highestAccuracy = j->Accuracy ();
      }
    }
  }

  return  featureCountWithHighestAccuracy;
} /* BestFeatureSetFoundFeatureCount */



FeatureNumList  TotalLine::BestFeatureSetFound ()  const
{
  float          highestAccuracy = 0.0f;
  ResultLinePtr  resultWithHighestAccuracy = NULL; 

  for  (int x = 1;  x <= maxNumOfFeatures;  x++)
  {
    ResultLinePtr  j = jobsWithHighestAccuracy[x];
    if  (j)
    {
      //if  (j->TestAccuracy () >  highestAccuracy))
      //{
      //  resultWithHighestAccuracy = j;
      //  highestAccuracy = j->TestAccuracy ();
      //}
      if  (j->Accuracy () > highestAccuracy)
      {
        resultWithHighestAccuracy = j;
        highestAccuracy = j->Accuracy ();
      }
    }
  }

  return  resultWithHighestAccuracy->GetFeatureNums ();
} /* BestFeatureSetFound */






FeatureNumList  TotalLine::SameAsBestFeatureSetFound (int  upCount) const
{
  int  z = featureCountWithStatisticlySame + upCount;
  z = Min (z, maxNumOfFeatures);

  ResultLinePtr  sameAsBest = jobsWithHighestGrade[z];
  if  (sameAsBest)
    return  sameAsBest->GetFeatureNums ();
  else
    return FeatureNumList (fileDesc);

  
  /*
  ResultLinePtr  sameAsBest = jobsWithHighestGrade[featureCountWithStatisticlySame + upCount];
    
    StatisticallySameJob ();

  if  (sameAsBest)
    return  *(sameAsBest->FeatureNums ());
  else
    return FeatureNumList ();
    */
} /* BestFeatureSetFound */






FFLOAT  TotalLine::AccuracyByNumOfFeatuers (int  numOfFeatures)
{
  if  ((numOfFeatures < 0)  ||  (numOfFeatures > maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::AccuracyByNumOfFeatuers   *** ERROR ***     NumOfFeatures[" << numOfFeatures << "]  is out of range." << endl
         << endl;
    return  -1.0;
  }

  return  accuracyByNumOfFeatuers[numOfFeatures];

}  /* AccuracyByNumOfFeatuers */




float  TotalLine::GradeByNumOfFeatuers (int  numOfFeatures)  const
{
  if  ((numOfFeatures < 0)  ||  (numOfFeatures > maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::GradeByNumOfFeatuers   *** ERROR ***     NumOfFeatures[" << numOfFeatures << "]  is out of range." << endl
         << endl;
    return  -1.0;
  }

  return  gradesByNumOfFeatuers[numOfFeatures];

}  /* AccuracyByNumOfFeatuers */





int   TotalLine::UssageInTop100 (int featureNum)
{
  if  ((featureNum < 0)  ||  (featureNum >= maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::UssageInTop100   *** ERROR ***     featureNum[" << featureNum << "]  out of range." << endl
         << endl;
    return  -1;
  }

  return  ussageInTop100[featureNum];
}




int   TotalLine::UssageInTop100Grades (int featureNum)
{
  if  ((featureNum < 0)  ||  (featureNum >= maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::UssageInTop100Grades   *** ERROR ***     featureNum[" << featureNum << "]  out of range." << endl
         << endl;
    return  -1;
  }

  return  ussageInTop100Grades[featureNum];
}







int   TotalLine::UssageInTop200 (int featureNum)
{
  if  ((featureNum < 0)  ||  (featureNum >= maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::UssageInTop200   *** ERROR ***     featureNum[" << featureNum << "]  out of range." << endl
         << endl;
    return  -1;
  }

  return  ussageInTop200[featureNum];
}





int   TotalLine::UssageInTop200Grades (int featureNum)
{
  if  ((featureNum < 0)  ||  (featureNum >= maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::UssageInTop200Grades   *** ERROR ***     featureNum[" << featureNum << "]  out of range." << endl
         << endl;
    return  -1;
  }

  return  ussageInTop200Grades[featureNum];
}







void   TotalLine::UpdateTotalsByGrades (int            placeFromTop,
                                        ResultLinePtr  resultLine
                                       ) 
{
  int  numOfFeatures = 0;
  int  x;
  
  numOfResults++;

  FeatureNumList  fn (fileDesc, resultLine->FeatureNums ());

  numOfFeatures = fn.NumOfFeatures ();
    
    
  if  (resultLine->Grade () > gradesByNumOfFeatuers[numOfFeatures])
  {
    gradesByNumOfFeatuers[numOfFeatures] = resultLine->Grade ();
  
    if  (jobsWithHighestGrade[numOfFeatures])
      delete  jobsWithHighestGrade[numOfFeatures];
    jobsWithHighestGrade[numOfFeatures] = new ResultLine (*resultLine, log);
  }

  if  (placeFromTop < 200)
  {
    for  (x = 0;  x < numOfFeatures;  x++)
    {
      int featureNum = fn[x];

      if  (placeFromTop < 100)
        ussageInTop100Grades[featureNum]++;
 
      ussageInTop200Grades[featureNum]++;
    }
  }

  // chgInAccuracies[resultLine->FeatureThatChanged ()].push_back (resultLine->ChgInAccuracy ());
}  /* UpdateTotalsBtGrades */









void   TotalLine::UpdateTotals (int            placeFromTop,
                                ResultLinePtr  resultLine
                               ) 
{
  int  numOfFeatures = 0;
  int  x;
  
  numOfResults++;

  FeatureNumList  fn (fileDesc, resultLine->FeatureNums ());

  numOfFeatures = fn.NumOfFeatures ();
    
    
  if  (resultLine->Accuracy () > accuracyByNumOfFeatuers[numOfFeatures])
  {
    accuracyByNumOfFeatuers[numOfFeatures] = resultLine->Accuracy ();
  
    if  (jobsWithHighestAccuracy[numOfFeatures])
      delete  jobsWithHighestAccuracy[numOfFeatures];
    jobsWithHighestAccuracy[numOfFeatures] = new ResultLine (*resultLine, log);
  }

  if  (placeFromTop < 200)
  {
    for  (x = 0;  x < numOfFeatures;  x++)
    {
      int featureNum = fn[x];
  
      if  (placeFromTop < 100)
        ussageInTop100[featureNum]++;
  
      ussageInTop200[featureNum]++;
    }
  }

  int  featureThatChanged = resultLine->FeatureThatChanged ();

  if  ((featureThatChanged >= 0)  &&  (featureThatChanged < maxNumOfFeatures))
    chgInAccuracies[featureThatChanged].push_back (resultLine->ChgInAccuracy ());
}  /* UpdateTotals */






ResultLinePtr  TotalLine::ResultWithHighestAccuracy ()  const
{
  ResultLinePtr resultWithHighestAccuracy = NULL;

  for  (int numOfFeatures = 0;  numOfFeatures < maxNumOfFeatures; numOfFeatures++)
  {
    ResultLinePtr  r = jobsWithHighestAccuracy[numOfFeatures];
    if  (r)    
    {
      if  (!resultWithHighestAccuracy)
        resultWithHighestAccuracy = r;

      else if  (r->Accuracy () > resultWithHighestAccuracy->Accuracy ())
        resultWithHighestAccuracy = r;
    }
  }

  return  resultWithHighestAccuracy;
}  /* ResultWithHighestAccuracy */







void  TotalLine::CalcFeatureChangeStats (int      featureNum,
                                         FFLOAT&  mean,
                                         FFLOAT&  stdDev
                                        )
{
  uint  x;

  mean   = (FFLOAT)0.0;
  stdDev = (FFLOAT)0.0;


  if  ((featureNum < 0)  ||  (featureNum >= maxNumOfFeatures))
  {
    cerr << endl
         << "TotalLine::UssageInTop100   *** ERROR ***     featureNum[" << featureNum << "]  out of range." << endl
         << endl;
    return;
  }


  float  total = 0.0f;

  for  (x = 0;  x < chgInAccuracies[featureNum].size ();  x++)
  {
    total += chgInAccuracies[featureNum][x];
  }

  mean = total / chgInAccuracies[featureNum].size ();

  double  totalSquareDiff = 0.0;
  double  delta;

  for  (x = 0;  x < chgInAccuracies[featureNum].size ();  x++)
  {
    delta = mean - chgInAccuracies[featureNum][x];
    totalSquareDiff += delta * delta;
  }

  double  variance = totalSquareDiff / chgInAccuracies[featureNum].size ();

  stdDev = (FFLOAT)sqrt (variance);
  
}  /* CalcFeatureChangeStats */




const 
ResultLinePtr  TotalLine::JobWithHighestAccuracy (int featureCount)  const
{
  if  ((featureCount < 0)  ||  (featureCount >= fileDesc->NumOfFields ()))
  {
    log.Level (-1) << endl << endl << endl
                   <<  "TotalLine::JobWithHighestAccuracy   *** ERROR ***" << endl \
                   << endl
                   << "FeatureCount[" << featureCount << "] is invalid." << endl
                   << endl;
    return NULL;
  }
  return  jobsWithHighestAccuracy[featureCount];
}  /* JobWithHighestAccuracy */
