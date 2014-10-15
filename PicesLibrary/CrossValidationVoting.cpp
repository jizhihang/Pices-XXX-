#include "FirstIncludes.h"

#include <stdio.h>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace  KKU;



#include "CrossValidationVoting.h"
#include "Classifier2.h"
#include "ConfusionMatrix2.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "MLClassConstList.h"
#include "FeatureVector.h"
#include "TrainingConfiguration2.h"
#include "TrainingProcess2.h"
using namespace  MLL;



CrossValidationVoting::CrossValidationVoting (TrainingConfigurationList2Ptr  _configs,
                                              FeatureVectorListPtr           _examples,
                                              MLClassConstListPtr         _mlClasses,
                                              int32                          _numOfFolds,
                                              bool                           _featuresAreAlreadyNormalized,
                                              FileDescPtr                    _fileDesc,
                                              RunLog&                        _log
                                             ):

   configs                      (_configs),
   featuresAreAlreadyNormalized (_featuresAreAlreadyNormalized),
   fileDesc                     (_fileDesc),
   foldAccuracies               (NULL),
   foldCounts                   (NULL),
   confusionMatrix              (NULL),
   cmByNumOfConflicts           (NULL),
   examples                     (new FeatureVectorList (*_mlClasses, *_examples, _log)),
   mlClasses                 (_mlClasses),
   examplesPerClass             (0),
   log                          (_log),
   maxNumOfConflicts            (0),
   numOfFolds                   (_numOfFolds),
   numOfSupportVectors          (0),
   numOfWinnersCounts           (NULL),
   numOfWinnersCorrects         (NULL),
   numOfWinnersOneOfTheWinners  (NULL),
   classificationTime           (0.0),
   reductionTime                (0.0),
   reductionPreExampleCount     (0),
   reductionPostExampleCount    (0),
   trainingTime                 (0.0)
 
{
  examplesPerClass = 999999;
}



CrossValidationVoting::~CrossValidationVoting ()
{
  DeleteAllocatedMemory ();
  delete  examples;  examples = NULL;
}
  
  


void  CrossValidationVoting::AllocateMemory ()
{
  maxNumOfConflicts = mlClasses->QueueSize () + 1;

  confusionMatrix    = new ConfusionMatrix2 (*mlClasses);
  cmByNumOfConflicts = new ConfusionMatrix2Ptr[maxNumOfConflicts];

  numOfWinnersCounts          = new int32[maxNumOfConflicts];
  numOfWinnersCorrects        = new int32[maxNumOfConflicts];
  numOfWinnersOneOfTheWinners = new int32[maxNumOfConflicts];

  int32  conflictIDX;

  for  (conflictIDX = 0;  conflictIDX < maxNumOfConflicts;  conflictIDX++)
  {
    cmByNumOfConflicts          [conflictIDX] = new ConfusionMatrix2 (*mlClasses);
    numOfWinnersCounts          [conflictIDX] = 0;
    numOfWinnersCorrects        [conflictIDX] = 0;
    numOfWinnersOneOfTheWinners [conflictIDX] = 0;
  }

  foldAccuracies  = new float[numOfFolds];
  foldCounts      = new int32[numOfFolds];

  int32  foldNum;

  for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    foldAccuracies [foldNum] = 0.0;
    foldCounts     [foldNum] = 0;
  }
}  /* AllocateMemory */




void  CrossValidationVoting::DeleteAllocatedMemory ()
{
  maxNumOfConflicts = mlClasses->QueueSize () + 1;
  if  (confusionMatrix)
  {
    delete confusionMatrix;
    confusionMatrix = NULL;
  }

  int32  conflictIDX;

  if  (cmByNumOfConflicts)
  {
    for  (conflictIDX = 0;  conflictIDX < maxNumOfConflicts;  conflictIDX++)
    {
      delete  cmByNumOfConflicts[conflictIDX];
      cmByNumOfConflicts[conflictIDX] = NULL;
    }
 
    delete  cmByNumOfConflicts;
    cmByNumOfConflicts = NULL;
  }

  delete  foldAccuracies;               foldAccuracies = NULL;
  delete  foldCounts;                   foldCounts     = NULL;
  
  delete  numOfWinnersCounts;           numOfWinnersCounts          = NULL;
  delete  numOfWinnersCorrects;         numOfWinnersCorrects        = NULL;
  delete  numOfWinnersOneOfTheWinners;  numOfWinnersOneOfTheWinners = NULL;

}  /* DeleteAllocatedMemory */




float  CrossValidationVoting::ReductionRatio () const
{
  if  (reductionPreExampleCount == 0)
    return  0.0;

  return  (float)reductionPostExampleCount / (float)reductionPreExampleCount;
}  /* ReductionRatio */





void  CrossValidationVoting::RunCrossValidation ()
{
  log.Level (10) << "CrossValidationVoting::RunCrossValidationVoting" << endl;

  if  (numOfFolds < 1)
  {
    log.Level (-1) << endl
                   << endl
                   << "CrossValidationVoting::RunCrossValidationVoting     **** ERROR ****" << endl
                   << endl
                   << "                                  Invalid  numOfFolds[" << numOfFolds << "]." << endl
                   << endl;
    return;
  }

  DeleteAllocatedMemory ();
  AllocateMemory ();

  int32  imageCount       = examples->QueueSize ();
  int32  numImagesPerFold = (imageCount + numOfFolds - 1) / numOfFolds;
  int32  firstInGroup     = 0;

  int32  foldNum;

  for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    int32  lastInGroup;

    // If We are doing the last Fold Make sure that we are including all the examples 
    // that have not been tested.
    if  (foldNum == (numOfFolds - 1))
      lastInGroup = imageCount;
    else
      lastInGroup = firstInGroup + numImagesPerFold - 1;


    cout << "Fold [" << (foldNum + 1) << "]  of  [" << numOfFolds << "]" << endl;

    FeatureVectorListPtr  trainingImages = new FeatureVectorList (fileDesc, true, log);

    FeatureVectorListPtr  testImages     = new FeatureVectorList (fileDesc, true, log);

    log.Level (30) << "Fold Num["        << foldNum        << "]   "
                   << "FirstTestImage["  << firstInGroup   << "]   "
                   << "LastInGroup["     << lastInGroup    << "]."
                   << endl;

    for  (int32  x = 0; x < imageCount; x++)
    {
      FeatureVectorPtr  newImage = new FeatureVector (*(examples->IdxToPtr (x)));

      if  ((x >= firstInGroup)  &&  (x <= lastInGroup))
      {
        testImages->PushOnBack (newImage);
      }
      else
      {
        trainingImages->PushOnBack (newImage);
      }
    }

    cout << "Number Of Training Images : " << trainingImages->QueueSize () << endl;
    cout << "Number Of Test Images     : " << testImages->QueueSize ()     << endl;

    CrossValidate (testImages, trainingImages, foldNum);

    delete  trainingImages;
    delete  testImages;
    firstInGroup = firstInGroup + numImagesPerFold;
  }
}  /* RunCrossValidationVoting */





void  CrossValidationVoting::RunValidationOnly (FeatureVectorListPtr validationData,
                                          bool*                classedCorrectly
                                         )
{
  log.Level (10) << "CrossValidationVoting::RunValidationOnly" << endl;
  DeleteAllocatedMemory ();
  AllocateMemory ();


  // We need to get a duplicate copy of each image data because the traininer and classofier
  // will nor,alize the data.
  FeatureVectorListPtr  trainingImages = examples->DuplicateListAndContents ();
  FeatureVectorListPtr  testImages     = validationData->DuplicateListAndContents ();

  CrossValidate (testImages, trainingImages, 0, classedCorrectly);

  delete  trainingImages;
  delete  testImages;
}  /* RunValidationOnly */




void  CrossValidationVoting::CrossValidate (FeatureVectorListPtr   testImages, 
                                            FeatureVectorListPtr   trainingImages,
                                            int32                  foldNum,
                                            bool*                  classedCorrectly
                                           )
{
  log.Level (20) << "CrossValidationVoting::CrossValidate   FoldNum[" << foldNum  << "]." << endl;

  int32  numOfClasses = mlClasses->QueueSize ();

  bool   cancelFlag = false;
  KKStr  statusMessage;

  vector<TrainingProcess2Ptr>  trainers;
  vector<Classifier2Ptr>       classifiers;

  int32  idx;
  for  (idx = 0;  idx < configs->QueueSize ();  idx++)
  {
    TrainingConfiguration2Ptr  config = configs->IdxToPtr (idx);
    
    TrainingProcess2Ptr  trainer = new TrainingProcess2 (config, 
                                                         trainingImages, 
                                                         NULL,
                                                         fileDesc,
                                                         log,
                                                         featuresAreAlreadyNormalized,
                                                         cancelFlag,
                                                         statusMessage
                                                        );

    trainer->CreateModelsFromTrainingData ();

    reductionTime             += trainer->ReductionTime ();
    reductionPreExampleCount  += trainer->ReductionPreExampleCount ();
    reductionPostExampleCount += trainer->ReductionPostExampleCount ();
    trainingTime              += trainer->TrainingTime ();
    numOfSupportVectors       += trainer->NumOfSupportVectors ();

    log.Level (20) << "CrossValidate   Creating Classification Object" << endl;

    Classifier2Ptr   classifier = new Classifier2 (trainer, log);

    trainers.push_back (trainer);
    classifiers.push_back (classifier);
  }

  {
    // Force the creation of a noise class
    MLClassConstPtr  noiseMLClass = mlClasses->GetNoiseClass ();
    noiseMLClass = NULL;
  }

  FeatureVectorList::iterator  imageIDX = testImages->begin ();

  double   probability;

  int32  foldCorrect = 0;

  int32  foldCount = 0;

  log.Level (20) << "CrossValidate   Classifying Test Images." << endl;

  double  startClassificationTime = osGetSystemTimeUsed ();

  for  (imageIDX = testImages->begin (); imageIDX != testImages->end (); imageIDX++)
  {
    MLClassConstPtr  knownClass = (*imageIDX)->MLClass ();

    int32               numOfWinners              = 0;
    bool                knownClassOneOfTheWinners = false;
    MLClassConstPtr  predictedClass            = NULL;
    double              breakTie                  = 0.0;

    Classifier2Ptr  classifier = NULL;

    vector<int32>  voteTable (numOfClasses, 0);
    vector<double> probTable (numOfClasses, 0.0f);
    
    for  (idx = 0;  idx < (int32)classifiers.size ();  idx++)
    {
      classifier = classifiers[idx];
      predictedClass =  classifier->ClassifyAImage (*(*imageIDX), 
                                                    probability, 
                                                    numOfWinners,
                                                    knownClassOneOfTheWinners,
                                                    breakTie
                                                   );
      int32  predictedIdx = mlClasses->PtrToIdx (predictedClass);

      if  ((predictedIdx < 0)  ||  (predictedIdx >= mlClasses->QueueSize ()))
      {
        // We are screwed,  don't know what class was predicted.
        log.Level (-1) << endl
                       << endl
                       << "CrossValidationVoting::CrossValidate    *** ERROR ***" << endl
                       << endl
                       << "UnKnown Class was returned[" << predictedClass->Name () << "]" << endl
                       << "predictedIdx[" << predictedIdx << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }

      voteTable[predictedIdx]++;

      if  (probTable[predictedIdx] == 0.0f)
        probTable[predictedIdx] = probability;
      else
        probTable[predictedIdx] *= probability;
    }

    {
      // Normalize Probability
      int32 x = 0;
      double  probTotal = 0.0f;
      for  (x = 0;  x < numOfClasses;  x++)
        probTotal += probTable[x];

      for  (x = 0;  x < numOfClasses;  x++)
        probTable[x] = probTable[x] / probTotal;
    }


    {
      // Determine winning vote
      int32  highVote = 0;
      numOfWinners = 0;
      int32  idxWithHighVote = -1;
      int32  winnerIdx       = -1;
      int32  x;

      for  (x = 0;  x < numOfClasses;  x++)
      {
        if  (voteTable[x] > highVote)
        {
          highVote = voteTable[x];
          numOfWinners    = 1;
          idxWithHighVote = x;
          winnerIdx       = x;
        }
        else if  (voteTable[x] == highVote)
        {
          numOfWinners++;
        }
      }

      if  (numOfWinners > 1)
      {
        // Select winner by high probability
        double  highProbability = 0.0f;

        for  (x = 0;  x < numOfClasses;  x++)
        {
          if  (voteTable[x] >= highVote)
          {
            if  (probTable[x] > highProbability)
            {
              highProbability = probTable[x];
              winnerIdx       = x;
            }
          }
        }
      }

      predictedClass = mlClasses->IdxToPtr (winnerIdx);
      probability    = probTable[winnerIdx];
    }

    confusionMatrix->Increment (knownClass, 
                                predictedClass, 
                                (int32)(*imageIDX)->OrigSize (), 
                                probability,
                                log
                               );

    cmByNumOfConflicts[numOfWinners]->Increment (knownClass, 
                                                 predictedClass, 
                                                 (int32)(*imageIDX)->OrigSize (), 
                                                 probability,
                                                 log
                                                );

    bool  correctClassificationMade = false;
    numOfWinnersCounts[numOfWinners]++;
    if  (knownClass->UpperName () == predictedClass->UpperName ())
    {
      correctClassificationMade = true;
      numOfWinnersCorrects[numOfWinners]++;
      foldCorrect++;
    }

    if  (classedCorrectly)
    {
      classedCorrectly[foldCount] = correctClassificationMade;
    }

    if  (knownClassOneOfTheWinners)
       numOfWinnersOneOfTheWinners[numOfWinners]++;

    log.Level (50) << "CrossValidate - Known Class["      << knownClass->Name ()     << "]  "
                   <<                 "Predicted Class["  << predictedClass->Name () << "]."
                   << endl;

    foldCount++;
  }

  double  endClassificationTime = osGetSystemTimeUsed ();
  classificationTime += (endClassificationTime - startClassificationTime);

  float  foldAccuracy = 0.0;

  if  (foldCount > 0)
    foldAccuracy = 100.0f * (float)foldCorrect / (float)foldCount;

  foldAccuracies [foldNum] = foldAccuracy;
  foldCounts     [foldNum] = foldCount;


  for  (idx = 0;  idx < (int32)trainers.size ();  idx++)
  {delete  trainers[idx];  trainers[idx] = NULL;}

  for  (idx = 0;  idx < (int32)classifiers.size ();  idx++)
  {delete  classifiers[idx];  classifiers[idx] = NULL;}

  log.Level (20) << "CrossValidationVoting::CrossValidate - Done." << endl;
}  /* CrossValidate */







float  CrossValidationVoting::Accuracy ()
{
  if  (confusionMatrix)
    return  (float)confusionMatrix->Accuracy ();
  else
    return 0.0;
}  /* Accuracy */




KKStr  CrossValidationVoting::FoldAccuracysToStr ()  const
{
  KKStr  foldAccuracyStr (9 * numOfFolds);  // Pre Reserving enough space for all Accuracies.

  for  (int32 foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    if  (foldNum > 0)
      foldAccuracyStr << "\t";
    foldAccuracyStr << StrFormatDouble (foldAccuracies[foldNum], "ZZ,ZZ0.00%");
  }

  return  foldAccuracyStr;
}  /* FoldAccuracysToStr */





float  CrossValidationVoting::FoldAccuracy (int32 foldNum)  const
{
  if  (!foldAccuracies)
  {
    log.Level (-1) << endl
                   << endl
                   << "CrossValidationVoting::FoldAccuracy      *** ERROR ***" << endl
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
                   << "CrossValidationVoting::FoldAccuracy      *** ERROR ***" << endl
                   << endl
                   << "                                   FoldNum[" << foldNum << "] Is Out Of Range." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  return  foldAccuracies[foldNum];
}  /* FoldAccuracy */




KKStr  CrossValidationVoting::FoldStr ()  const
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
