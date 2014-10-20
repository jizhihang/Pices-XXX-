#include  "FirstIncludes.h"

//*******************************************************************
//*                    CrossValidationThreeWay                      *
//*                                                                 *
//*   This is a special CrossValidation that is meant to help       *
//* determine a good feature set for a Binary classifier.  It will  *
//* use the two specified classes for training and testing while    *
//* using all other classes for testing only.                       *
//*                                                                 *
//*******************************************************************
#include  <stdio.h>
#include  <math.h>

#include  <iomanip>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "RunLog.h"
using namespace KKU;



#include  "CrossValidationThreeWay.h"

#include  "Classifier2.h"
#include  "ConfusionMatrix2.h"
#include  "Histogram.h"
#include  "MLClass.h"
#include  "FeatureVector.h"
#include  "StatisticalFunctions.h"
#include  "TrainingClass.h"
#include  "TrainingConfiguration2.h"
#include  "TrainingProcess2.h"
using namespace  MLL;




CrossValidationThreeWay::CrossValidationThreeWay (TrainingConfiguration2Ptr  _config,
                                                  bool                       useConfigAsIs,
                                                  FeatureVectorListPtr       _examples,
                                                  MLClassConstListPtr     _mlClasses,
                                                  MLClassConstPtr         _class1,
                                                  MLClassConstPtr         _class2,
                                                  int32                      _numOfFolds,
                                                  bool                       _featuresAlreadyNormalized,
                                                  bool                       _stratify,
                                                  float                      _punishmentFactor,
                                                  RunLog&                    _log
                                                 ):

   accuracy                     (0.0),
   class1                       (_class1),
   class2                       (_class2),
   classificationTime           (0.0),
   config                       (_config),
   confusionMatrix              (NULL),
   featuresAlreadyNormalized    (_featuresAlreadyNormalized),
   foldAccuracies               (NULL),
   grade                        (0.0),
   mlClasses                 (_mlClasses),
   examples                     (_examples),
   log                          (_log),
   maxProb                      (-999.99),
   minProb                      (999.99),
   numClassedCorrectly          (0),
   numInTwoClasses              (0),
   numOfFolds                   (_numOfFolds),
   numOfSupportVectors          (0.0f),
   ourClasses                   (NULL),
   punishmentFactor             (_punishmentFactor),
   stratify                     (_stratify),
   thirdClassTotalDecisiveCount (0),
   thirdClassTotalDecisiveProb  (0.0f),
   trainingTime                 (0.0),
   twoClassConfig               (NULL),
   twoClassList                 (NULL),
   weOwnTwoClassConfig          (false)
{
  twoClassList = new MLClassConstList;
  twoClassList->PushOnBack (class1);
  twoClassList->PushOnBack (class2);

  confusionMatrix = new ConfusionMatrix2 (*mlClasses);

  if  (!useConfigAsIs)
  {
    weOwnTwoClassConfig = true;
    twoClassConfig = new TrainingConfiguration2 (*config);
    twoClassConfig->EmptyTrainingClasses ();
    twoClassConfig->AddATrainingClass (new TrainingClass ("", class1->Name (), "F", 1.0f, *twoClassList)); 
    twoClassConfig->AddATrainingClass (new TrainingClass ("", class2->Name (), "F", 1.0f, *twoClassList)); 
  }
  else
  {
    weOwnTwoClassConfig = false;
    twoClassConfig = config;
  }

  if  (numOfFolds > 0)
    foldAccuracies = new float[numOfFolds];
}
 



CrossValidationThreeWay::~CrossValidationThreeWay (void)
{
  delete  twoClassList;

  delete  confusionMatrix;
  delete  ourClasses;

  delete  foldAccuracies;

  if  (weOwnTwoClassConfig)
    delete  twoClassConfig;

}




void  CrossValidationThreeWay::BuildOurClasses ()
{
  delete  ourClasses;
  ourClasses = new MLClassConstList,
  ourClasses->PushOnBack (class1);
  ourClasses->PushOnBack (class2);

  ClassStatisticListPtr  classStats = examples->GetClassStatistics ();

  ClassStatisticList::iterator csIDX = classStats->begin ();
  for  (csIDX = classStats->begin ();  csIDX != classStats->end ();  csIDX++)
  {
    ClassStatisticPtr cs = *csIDX;
    if  ((cs->MLClass () != class1)  &&  (cs->MLClass () != class2))
    {
      ourClasses->PushOnBack ((MLClassPtr)(cs->MLClass ()));
    }
  }

  delete  classStats;
}  /* BuildOurClasses */





void   CrossValidationThreeWay::RunValidation (void)
{
  int32  foldNum;

  int32  rowsPerFold = (int32)((float)(examples->QueueSize () / numOfFolds) + 0.5f);
  int32  startingRowNum = 0;
  int32  endingRowNum   = -1;

  thirdClassTotalDecisiveCount = 0;
  thirdClassTotalDecisiveProb  = 0.0f;

  for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    FeatureVectorListPtr trainingImages = new FeatureVectorList (examples->FileDesc (), false, log, rowsPerFold + 1);
    FeatureVectorListPtr testImages     = new FeatureVectorList (examples->FileDesc (), false, log, examples->QueueSize ());

    startingRowNum = endingRowNum + 1;
    if  (foldNum < (numOfFolds - 1))
      endingRowNum = startingRowNum + rowsPerFold - 1;
    else
      endingRowNum = examples->QueueSize () - 1;

    int32  row = 0;  
    FeatureVectorPtr  image = NULL;
    FeatureVectorList::iterator  iIDX = examples->begin ();
    for  (iIDX = examples->begin ();  iIDX != examples->end ();  iIDX++)
    {
      image = *iIDX;
      if  ((row >= startingRowNum)  &&  (row <= endingRowNum))
      {
        testImages->PushOnBack (image);
      }
      else
      {
        if  ((image->MLClass () == class1)  ||  (image->MLClass () == class2))
          trainingImages->PushOnBack (image);
      }
      row++;
    }

    {
      FeatureVectorListPtr  dupTrainImages = trainingImages->DuplicateListAndContents ();
      CrossValidate (testImages, dupTrainImages, foldNum, NULL);
      delete  dupTrainImages;
    }

    delete  testImages;
    delete  trainingImages;
  }

  CalculateAccuracyAndGrade ();

}  /* RunValidation */





void  CrossValidationThreeWay::CalculateAccuracyAndGrade ()
{
  accuracy = (float)numClassedCorrectly / (float)(numInTwoClasses);

  float meanOtherClassProbs = 0.0f;
  if  (thirdClassTotalDecisiveCount > 0)
     meanOtherClassProbs = thirdClassTotalDecisiveProb / (float)thirdClassTotalDecisiveCount;

  grade = accuracy - punishmentFactor * meanOtherClassProbs;
}  /* CalculateAccuracyAndGrade */






void  CrossValidationThreeWay::CrossValidate (FeatureVectorListPtr   testImages, 
                                              FeatureVectorListPtr   trainingImages,
                                              int32                  foldNum,
                                              bool*                  classedCorrectly
                                             )
{
  log.Level (20) << "CrossValidationThreeWay::CrossValidate   FoldNum[" << foldNum  << "]." << endl;

  bool   cancelFlag = false;
  KKStr  statusMessage;

  TrainingProcess2  trainer (twoClassConfig, 
                             trainingImages, 
                             NULL,
                             testImages->FileDesc (),
                             log,
                             featuresAlreadyNormalized,
                             cancelFlag,
                             statusMessage
                            );

  double  startTrainingTime = osGetSystemTimeUsed ();
  trainer.CreateModelsFromTrainingData ();
  double  endTrainingTime   = osGetSystemTimeUsed ();
  trainingTime += (endTrainingTime - startTrainingTime);
  numOfSupportVectors += trainer.NumOfSupportVectors ();

  log.Level (20) << "CrossValidate   Creating Classification Object" << endl;

  Classifier2  classifier (&trainer, log);

  FeatureVectorList::iterator  exampleIDX;

  double   probability;

  int32  foldCorrect   = 0;
  int32  foldCount     = 0;
  int32  testItemCount = 0;

  log.Level (20) << "CrossValidate   Classifying Test Images." << endl;

  double  startClassificationTime = osGetSystemTimeUsed ();

  FeatureVectorPtr  image = NULL;

  for  (exampleIDX = testImages->begin ();   exampleIDX != testImages->end ();   ++exampleIDX)
  {
    image = *exampleIDX;
    MLClassConstPtr  knownClass = image->MLClass ();

    int32   numOfWinners              = 0;
    bool    knownClassOneOfTheWinners = false;
    double  breakTie                  = 0.0;

    bool  correctPredictionMade = false;

    FeatureVectorPtr  dupImage  = new FeatureVector (*image);

    MLClassConstPtr  predictedClass 
      =  classifier.ClassifyAImage (*dupImage,
                                    probability, 
                                    numOfWinners,
                                    knownClassOneOfTheWinners,
                                    breakTie
                                   );

    MLClassConstPtr  ourPredictedClass = mlClasses->LookUpByName (predictedClass->Name ());


    confusionMatrix->Increment (knownClass, 
                                ourPredictedClass, 
                                (int32)(*exampleIDX)->OrigSize (), 
                                probability,
                                log
                               );

    if  (probability > maxProb)
      maxProb = probability;

    if  (probability < minProb)
      minProb = probability;

    image->Probability    ((float)probability);
    image->BreakTie       ((float)breakTie);
    image->PredictedClass (ourPredictedClass);

    if  ((image->MLClass () == class1)  ||  (image->MLClass () == class2))
    {
      // We are lookinga a test item that belongs to one of th etwo classes that we
      // are trained for.
      if  (knownClass->UpperName () == predictedClass->UpperName ())
      {
        numClassedCorrectly++;
        foldCorrect++;
        correctPredictionMade = true;
      }
      
      numInTwoClasses++;

      foldCount++;
    }
    else
    {
      // We are now looking at a test item that is -NOT- one of the
      // two classes that we are trained for
      thirdClassTotalDecisiveProb  += fabs ((float)probability - 0.5f);
      thirdClassTotalDecisiveCount++;
    }

    if  (classedCorrectly)
      classedCorrectly[testItemCount] = correctPredictionMade;

    delete  dupImage;
    testItemCount++;
  }

  double  endClassificationTime = osGetSystemTimeUsed ();
  classificationTime += (endClassificationTime - startClassificationTime);

  double  foldAccuracy = 0.0;
  if  (foldCount > 0)
    foldAccuracy = 100.0 * (double)foldCorrect / (double)foldCount;

  foldAccuracies[foldNum] = (float)foldAccuracy;

  log.Level (20) << "CrossValidation::CrossValidate - Done." << endl;
}  /* CrossValidate */








void  CrossValidationThreeWay::CreateDistanceReport (ostream&  r)
{
  r << endl
    << endl
    << "Distance to Margin Report" << endl
    << "Class1[" << class1->Name () << "]   Class2[" << class2->Name () << "]" << endl
    << endl
    << "Feature Nums[" << config->GetFeatureNums () << "]" << endl
    << endl
    << "Accuracy[" << StrFormatDouble (accuracy * 100, "zzz0.00") << "%" << "]"  << endl
    << endl;

    
  KKStr title1, title2, title3;

  BuildOurClasses ();

  ourClasses->ExtractThreeTitleLines (title1, title2, title3);

  r << "\t" << title1 << endl;
  r << "\t" << title2 << endl;
  r << "\t" << title3 << endl;

  int32  x;
  int32  largestClassList = -1;

  int32  numHistBuckets = 100;
  float bucketSize = (float)(maxProb - minProb) / (float)numHistBuckets;

  double*               totalProbabilityByClass = new double[ourClasses->QueueSize ()];
  FeatureVectorListPtr* imagesByClass           = new FeatureVectorListPtr[ourClasses->QueueSize ()];
  HistogramPtr*         histograms              = new HistogramPtr[ourClasses->QueueSize ()];

  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    imagesByClass[x] = examples->ExtractImagesForAGivenClass (ourClasses->IdxToPtr (x));
    largestClassList = Max (largestClassList, (int32)imagesByClass[x]->QueueSize ());
    totalProbabilityByClass[x] = 0.0;
    histograms[x] = new Histogram ((float)minProb, numHistBuckets, bucketSize, false);
  }

  int32  row = 0; 
  for  (row = 0;  row < largestClassList;  row++)
  {
    for  (x = 0;  x < ourClasses->QueueSize (); x++)
    {
      FeatureVectorListPtr  imgaesInClass = imagesByClass[x];
      if  (row < imgaesInClass->QueueSize ())
      {
        FeatureVectorPtr  image = imgaesInClass->IdxToPtr (row);
        totalProbabilityByClass[x] += image->Probability ();
        histograms[x]->Increment (image->Probability ());
      }
      else
      {
      }
    }
  }

  KKStr  meanLine   ("Mean");
  KKStr  stdDevLine ("StdDev");
  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    FeatureVectorListPtr  imagesInClass = imagesByClass[x];
    double  meanDistance = totalProbabilityByClass[x] / (double)(imagesInClass->QueueSize ());
    double  totalSquareError = 0.0;
    for  (row = 0;  row < imagesInClass->QueueSize ();  row++)
    {
      FeatureVectorPtr  image = imagesInClass->IdxToPtr (row);
      double  delta = image->Probability () - meanDistance;
      totalSquareError +=  delta * delta;
    }

    double  varianmce = totalSquareError / (double)(imagesInClass->QueueSize ());
    double  stdDev    = sqrt (varianmce);
    meanLine   << "\t" << meanDistance;
    stdDevLine << "\t" << stdDev;
  }

  r << meanLine   << endl;
  r << stdDevLine << endl;
  r << endl;
  r << endl;


  float  rowValue = histograms[0]->MinValue ();
  r << "\t" << "\t" << title1 << endl;
  r << "\t" << "\t" << title2 << endl;
  r << "\t" << "\t" << title3 << endl;
  for  (row = 0;  row < numHistBuckets;  row++)
  {
    r << row << "\t" << rowValue;
    for  (x = 0; x < ourClasses->QueueSize (); x++)
    {
      HistogramPtr h = histograms[x];
      r << "\t" << h->Bucket (row);
    }
    r << endl;
    rowValue += histograms[0]->BucketSize ();
  }

   r << endl
     << endl
     << endl;

  delete  totalProbabilityByClass;
  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    delete  imagesByClass[x];
    delete  histograms[x];
  }
  delete  histograms;
  delete  imagesByClass;
}  /* CreateDistanceReport */







void  CrossValidationThreeWay::CreateProbabilityReport (ostream&  r)
{
  r << endl
    << endl
    << "Probability Report" << endl
    << "Class1[" << class1->Name () << "]   Class2[" << class2->Name () << "]" << endl
    << endl
    << "Feature Nums[" << config->GetFeatureNums () << "]" << endl
    << endl
    << "Accuracy[" << StrFormatDouble (accuracy * 100, "zzz0.00") << "%" << "]"  << endl
    << endl;

    
  KKStr title1, title2, title3;

  BuildOurClasses ();

  ourClasses->ExtractThreeTitleLines (title1, title2, title3);

  r << "\t" << title1 << endl;
  r << "\t" << title2 << endl;
  r << "\t" << title3 << endl;

  int32  x;
  int32  largestClassList = -1;


  float*                totalProbabilitiesByClass = new float[ourClasses->QueueSize ()];
  FeatureVectorListPtr* imagesByClass             = new FeatureVectorListPtr[ourClasses->QueueSize ()];
  HistogramPtr*         histograms                = new HistogramPtr[ourClasses->QueueSize ()];

  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    imagesByClass[x] = examples->ExtractImagesForAGivenClass (ourClasses->IdxToPtr (x));
    largestClassList = Max (largestClassList, (int32)(imagesByClass[x]->QueueSize ()));
    totalProbabilitiesByClass[x] = 0.0;
    histograms[x] = new Histogram (0.0f, 100, 0.01f, false);
  }

  int32  row = 0; 
  for  (row = 0;  row < largestClassList;  row++)
  {
    for  (x = 0;  x < ourClasses->QueueSize (); x++)
    {
      FeatureVectorListPtr  imgaesInClass = imagesByClass[x];
      if  (row < imgaesInClass->QueueSize ())
      {
        FeatureVectorPtr  image = imgaesInClass->IdxToPtr (row);
        float  probability = image->Probability ();
        if  (image->PredictedClass() == class1)
          probability = 1.0f - (float)(image->Probability ());

        totalProbabilitiesByClass[x] += probability;
        histograms[x]->Increment (probability);
      }
    }
  }

  KKStr  meanLine   ("Mean");
  KKStr  stdDevLine ("StdDev");
  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    FeatureVectorListPtr  imagesInClass = imagesByClass[x];
    float  probMean = totalProbabilitiesByClass[x] / (float)(imagesInClass->QueueSize ());
    float  totalSquareError = 0.0;
    for  (row = 0;  row < imagesInClass->QueueSize ();  row++)
    {
      FeatureVectorPtr  image = imagesInClass->IdxToPtr (row);
      float  probability = image->Probability ();
      if  (image->PredictedClass() == class1)
        probability = 1.0f - (float)(image->Probability ());

      float  delta = probability - probMean;
      totalSquareError +=  delta * delta;
    }

    float  varianmce = totalSquareError / (float)(imagesInClass->QueueSize ());
    float  stdDev    = (float)sqrt (varianmce);
    meanLine   << "\t" << probMean;
    stdDevLine << "\t" << stdDev;
  }

  r << meanLine   << endl;
  r << stdDevLine << endl;
  r << endl;
  r << endl;

  float  rowValue = histograms[0]->MinValue ();
  r << "\t" << "\t" << title1 << endl;
  r << "\t" << "\t" << title2 << endl;
  r << "\t" << "\t" << title3 << endl;
  for  (row = 0;  row < histograms[0]->NumOfBuckets ();  row++)
  {
    r << row << "\t" << rowValue;
    for  (x = 0; x < ourClasses->QueueSize (); x++)
    {
      HistogramPtr h = histograms[x];
      r << "\t" << h->Bucket (row);
    }
    r << endl;
    rowValue += histograms[0]->BucketSize ();
  }

   r << endl
     << endl
     << endl;

  delete  totalProbabilitiesByClass;
  for  (x = 0;  x < ourClasses->QueueSize (); x++)
  {
    delete  imagesByClass[x];
    delete  histograms[x];
  }
  delete  histograms;
  delete  imagesByClass;
}  /* CreateProbabilityReport */








KKStr  CrossValidationThreeWay::AccuracyStr ()
{
  return  confusionMatrix->AccuracyStr ();
}  /* AccuracyStr */







void  CrossValidationThreeWay::CalculateThirdClassDecisiveProbability (float&  mean,
                                                                       float&  stdDev
                                                                      )
{
  FeatureVectorList::iterator idx;
  FeatureVectorPtr  i = NULL;

  float  delta;
  int32    count = 0;
  float  totalSquared = 0.0f;

  if  (thirdClassTotalDecisiveCount < 1)
    mean = 0.0f;
  else
    mean = thirdClassTotalDecisiveProb / thirdClassTotalDecisiveCount;

  for  (idx = examples->begin ();  idx != examples->end ();  idx++)
  {
    i = *idx; 
    if  ((i->MLClass () == class1)  ||  (i->MLClass () == class2))
      continue;

    delta = (mean - fabs (i->Probability () - 0.50f));
    totalSquared += delta * delta;
    count++;
  }

  if  (count == 0)
  {
    stdDev = 0.0f;
  }
  else
  {
    float var = totalSquared / (float)count;
    stdDev = sqrt (var);
  }
}  /* CalculateThirdClassDecisiveProbability */





//  This version was made specifically for usew with ParseFeatureSlection results
void  CrossValidationThreeWay::CalculateThirdClassDecisiveProbability (FeatureVectorListPtr testData, 
                                                                       float&               mean,
                                                                       float&               stdDev
                                                                      )
{
  FeatureVectorList::iterator idx;
  FeatureVectorPtr  i = NULL;

  float  delta;
  int32    count = 0;
  float  totalSquared = 0.0f;

  if  (thirdClassTotalDecisiveCount < 1)
    mean = 0.0f;
  else
    mean = thirdClassTotalDecisiveProb / thirdClassTotalDecisiveCount;

  for  (idx = testData->begin ();  idx != testData->end ();  idx++)
  {
    i = *idx; 
    if  ((i->MLClass () == class1)  ||  (i->MLClass () == class2))
      continue;

    delta = (mean - fabs (i->Probability () - 0.50f));
    totalSquared += delta * delta;
    count++;
  }

  if  (count == 0)
  {
    stdDev = 0.0f;
  }
  else
  {
    float var = totalSquared / (float)count;
    stdDev = sqrt (var);
  }
}  /* CalculateThirdClassDecisiveProbability */








void   CrossValidationThreeWay::RunValidationOnly (FeatureVectorListPtr  validationData,
                                                   bool*                 classedCorrectly
                                                  )
{
  log.Level (10) << "CrossValidationThreeWay::RunValidationOnly" << endl;
  
  // We need to get a duplicate copy of each image data because the traininer and classofier
  // will normalize the data.

  FeatureVectorListPtr  trainingImages = new FeatureVectorList (examples->FileDesc (), true, log, examples->QueueSize ());

  {
    FeatureVectorListPtr class1Images     = examples->ExtractImagesForAGivenClass (class1);
    FeatureVectorListPtr class1ImagesDup = class1Images->DuplicateListAndContents ();
    trainingImages->AddQueue (*class1ImagesDup);
    class1ImagesDup->Owner (false);
    delete  class1Images;     class1Images    = NULL;
    delete  class1ImagesDup;  class1ImagesDup = NULL;
  }

  {
    FeatureVectorListPtr class2Images     = examples->ExtractImagesForAGivenClass (class2);
    FeatureVectorListPtr class2ImagesDup = class2Images->DuplicateListAndContents ();
    trainingImages->AddQueue (*class2ImagesDup);
    class2ImagesDup->Owner (false);
    delete  class2Images;     class2Images    = NULL;
    delete  class2ImagesDup;  class2ImagesDup = NULL;
  }

  // FeatureVectorListPtr testImages = validationData->DuplicateListAndContents ();

  //CrossValidate (testImages, trainingImages, 0, classedCorrectly);
  CrossValidate (validationData, trainingImages, 0, classedCorrectly);

  delete  trainingImages;
  // delete  testImages;

  CalculateAccuracyAndGrade ();
}  /* RunValidationOnly */



float  CrossValidationThreeWay::FoldAccuracy (int32 foldNum)
{
    if  (!foldAccuracies)
  {
    log.Level (-1) << endl
                   << endl
                   << "CrossValidationThreeWay::FoldAccuracy      *** ERROR ***" << endl
                   << endl
                   << "                                   FoldAccuracies not defined." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  ((foldNum < 0)  ||  (foldNum >= numOfFolds))
  {
    log.Level (-1) << endl
                   << endl
                   << "CrossValidationThreeWay::FoldAccuracy      *** ERROR ***" << endl
                   << endl
                   << "                                   FoldNum[" << foldNum << "] Is Out Of Range." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  return  foldAccuracies[foldNum];
}  /* FoldAccuracy */



KKStr  CrossValidationThreeWay::FoldStr ()  const
{
  if  ((numOfFolds <= 0)  ||  (!foldAccuracies))
    return  "";

  int32  x;

  KKStr  result (numOfFolds + 10);

  for  (x = 0;  x < numOfFolds;  x++)
  {
    if  (x > 0)
      result << ",";
    result << foldAccuracies[x];
  }

  return  result;
}  /* FoldStr */
