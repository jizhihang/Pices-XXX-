#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory>
#include  <math.h>
#include  <limits.h>
#include  <float.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <algorithm>
#include  <functional>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;

#include "BasicTypes.h"

#include "ClassGroupTotals.h"


#include "ClassFocus.h"
#include "ClassGroupHistory.h"
#include "MLClass.h"
#include "OSservices.h"
#include "RunLog.h"




ClassGroupTotals::ClassGroupTotals (KKStr         _fileName,
                                    MLClassList&  _mlClasses,
                                    int           _iipc,
                                    int           _ipr,
                                    RunLog&       _log
                                   ):
                                    
         log                       (_log),
         numOfRetrainings          (-1),
         numOfRandomPasses         (-1),
         mlClasses                 (),
         maxTrainingImagesPerRetraining (0),
         totalCounts               (0.0),
         totalCorrect              (0.0),
         collectHistory            (false),
         sortOrder                 (NULL_SortOrder),
         iipc                      (_iipc),
         ipr                       (_ipr),
         fileName                  (_fileName)
                    
{
  MLClassListPtr  classesInFile = NULL;

  ClassGroupTotals::GetClassGroupTotalsFileStats (_fileName, 
                                                  _mlClasses, 
                                                  log, 
                                                  classesInFile, 
                                                  sortOrder, 
                                                  numOfRetrainings, 
                                                  numOfRandomPasses, 
                                                  streamBlockSize
                                                 );
  if  (!classesInFile)
  {
    _log.Level (-1) << "ClassGroupTotals::ClassGroupTotals  Error gettimng Stats From File[" << _fileName << "]." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  {
    // Copy Images classes returned in classesInFile to mlClasses */
    MLClassListIterator  icIDX (*classesInFile);
    MLClassPtr mlClass = NULL;
    for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
      mlClasses.PushOnBack (mlClass);
    delete  classesInFile;
    classesInFile = NULL;
  }


  FILE*  in = osFOPEN (_fileName.Str (), "r");
  if  (!in)
  {
    _log.Level (-1) << "ClassGroupTotals::ClassGroupTotals  Error Opening File[" << _fileName << "]." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  InitializeVectors ();

  rewind (in);
  ReadInHistory (in);

  fclose (in);
}



ClassGroupTotals::ClassGroupTotals (SortOrderType  _sortOrder,
                                    int            _numOfretrainings,
                                    int            _numOfPasses,
                                    int            _iipc,
                                    int            _ipr,
                                    int            _streamBlockSize,
                                    RunLog&        _log,
                                    MLClassList&   _mlClasses,
                                    bool           _collectHistory
                                   ):

         sortOrder                      (_sortOrder),
         log                            (_log),
         numOfRetrainings               (_numOfretrainings),
         numOfRandomPasses              (_numOfPasses),
         mlClasses                      (_mlClasses),
         maxTrainingImagesPerRetraining (0),
         totalCounts                    (0.0),
         totalCorrect                   (0.0),
         collectHistory                 (_collectHistory),
         iipc                           (_iipc),
         ipr                            (_ipr),
         streamBlockSize                (_streamBlockSize),
         fileName                       ()
{
  InitializeVectors ();
}





ClassGroupTotals::~ClassGroupTotals ()
{
  if  (collectHistory)
  {
    for  (int retraining = 0;  retraining < numOfRetrainings;  retraining++)
    {
      for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
      {
        delete  predictions   [retraining][randomPass];
        delete  probabilities [retraining][randomPass];
        delete  sortMethods   [retraining][randomPass];
      }

      delete  predictions   [retraining];
      delete  probabilities [retraining];
      delete  sortMethods   [retraining];
    }

    delete  predictions;
    delete  probabilities;
    delete  sortMethods;
  }
  
  if  (totals)
  {
    for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      for  (int retraining = 0;  retraining < numOfRetrainings;  retraining++)
      {
        delete  totals[randomPass][retraining];
        totals[randomPass][retraining] = NULL;
      }

      delete  totals[randomPass];
      totals[randomPass] = NULL;
    }

    delete  totals;
    totals = NULL;
  }
}





void  ClassGroupTotals::InitializeVectors ()
{
  int retraining;
  int randomPass;

  int  numOfClasses = mlClasses.QueueSize ();

  if  (collectHistory)
  {
    totals = new ClassFocusPtr*[numOfRandomPasses];
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      totals[randomPass] = new ClassFocusPtr[numOfRetrainings];
      for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
      {
        totals[randomPass][retraining] = new ClassFocus (numOfClasses);
      }
    }

    predictions   = new HistoryBitListPtr*[numOfRetrainings];
    probabilities = new HistoryBitListPtr*[numOfRetrainings];
    sortMethods   = new HistoryBitListPtr*[numOfRetrainings];

    for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
    {
      predictions   [retraining] = new HistoryBitListPtr[numOfRandomPasses];
      probabilities [retraining] = new HistoryBitListPtr[numOfRandomPasses];
      sortMethods   [retraining] = new HistoryBitListPtr[numOfRandomPasses];
  
      for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
      {
        predictions   [retraining][randomPass] = new HistoryBitList (true, 100);
        probabilities [retraining][randomPass] = new HistoryBitList (true, 10);
        sortMethods   [retraining][randomPass] = new HistoryBitList (true, 6);
      }
    }
  }

  else
  {
    predictions   = NULL;
    probabilities = NULL;
    sortMethods   = NULL;

    totals = new ClassFocusPtr*[numOfRandomPasses];
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      totals[randomPass] = new ClassFocusPtr[numOfRetrainings];
      for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
      {
        totals[randomPass][retraining] = new ClassFocus (numOfClasses);
      }
    }
  }
}  /* InitializeVectors */




void   ClassGroupTotals::ReadInDataFile (KKStr   _fileName,
                                         bool&   _successful
                                        )
{
  _successful = true;

  fileName = _fileName;

  MLClassListPtr  classesInFile = NULL;


  ClassGroupTotals::GetClassGroupTotalsFileStats (fileName, 
                                                  mlClasses, 
                                                  log, 
                                                  classesInFile, 
                                                  sortOrder, 
                                                  numOfRetrainings, 
                                                  numOfRandomPasses, 
                                                  streamBlockSize
                                                 );
  if  (!classesInFile)
  {
    log.Level (-1) << endl
                   << endl
                   << "ClassGroupTotals::ReadInDataFile   Error getting stats on File[" << _fileName << "]." << endl
                   << endl;
    _successful = false;
  }


  FILE*  in = osFOPEN (_fileName.Str (), "r");

  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "ClassGroupTotals::ReadInDataFile Error Opening File[" << _fileName << "]." << endl
                   << endl;
    _successful = false;
  }


  rewind (in);
  ReadInHistory (in);
  fclose (in);
}  /* ReadInDataFile */






inline
int  ClassGroupTotals::MLClassIDX (const char*  funcName,
                                   MLClassPtr   mlClass
                                  )   const
{
  int  idx = mlClasses.PtrToIdx (mlClass);

  if  (idx < 0)
  {
    cerr <<                                                                     endl
         << "****  ERROR  ****     ClassGroupTotals::" << funcName           << endl
         <<                                                                     endl
         << "                 Invalid Class[" << mlClass->Name () << "]"  << endl
         << "                 Is not one of the ones in our List."           << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  return  idx;
}




void  ClassGroupTotals::ValidateClassIDX (const char*  funcName, 
                                          int          classIDX
                                         )   const
{
#ifdef  KKDEBUG

  if  ((classIDX < 0)  ||  (classIDX >= mlClasses.QueueSize ()))
  {
    cerr <<                                                                     endl
         << "****  ERROR  ****     ClassGroupTotals::" << funcName           << endl
         <<                                                                     endl
         << "                 ClassIDX[" << classIDX << "]  out of range."   << endl
         << "                 Num Classes Defined are [" << mlClasses.QueueSize () << "]"  << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }

  return;

#endif
}



inline
void  ClassGroupTotals::ValidateRetraining (const char*  funcName, 
                                            int          retraining
                                           )  const
{
#ifdef  KKDEBUG
  if  (retraining >= numOfRetrainings)
  {
    cerr <<                                                                          endl
         << "****  ERROR  ****     ClassGroupTotals::"  << funcName                    << endl
         <<                                                                          endl
         << "                 retraining["        << retraining       << "]  out of range." << endl
         << "                 numOfRetrainings [" << numOfRetrainings << "]"                << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }
#endif
}



inline
void  ClassGroupTotals::ValidatePass (const char*  funcName, 
                                      int          randomPass
                                     )  const
{
#ifdef  KKDEBUG

  if  (randomPass >= numOfRandomPasses)
  {
    cerr <<                                                                           endl
         << "****  ERROR  ****     ClassGroupTotals::"  << funcName                << endl
         <<                                                                           endl
         << "                 randomPass["         << randomPass         << "]  out of range." << endl
         << "                 numOfRandomPasses [" << numOfRandomPasses  << "]"                << endl
         << endl;
    osWaitForEnter ();
    exit (-1);
  }
#endif
}





float  ClassGroupTotals::RetrainingCounts (int retraining)  const
{
  ValidateRetraining ("RetrainingCounts", retraining);

  float  totalRetrainingCounts = 0.0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    totalRetrainingCounts += totals[randomPass][retraining]->Count ();
  }

  return  totalRetrainingCounts;
}  /* RetrainingCounts */




float  ClassGroupTotals::NumOfSupportVectors (int retraining)  const
{
  ValidateRetraining ("NumOfSupportVectors", retraining);

  int  totalNumOfSupportVectors = 0;

  int  numSupportPointsEncounterd = 0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    int  nosp = totals [randomPass][retraining]->NumOfSupportVectors ();

    if  (nosp != 0)
    {
      totalNumOfSupportVectors += nosp;
      numSupportPointsEncounterd++;
    }
  }

  if  (numSupportPointsEncounterd == 0)
    return 0.0f;
  else
    return  (float)totalNumOfSupportVectors / (float)numSupportPointsEncounterd;
}  /* NumOfSupportVectors */





float  ClassGroupTotals::TrainingTimeMean (int retraining)  const
{
  ValidateRetraining ("TrainingTimeMean", retraining);

  float  totalTrainngTime = 0.0f;

  int    numTrainingTimeEncountered = 0;
  
  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    float tt = totals [randomPass][retraining]->TrainingTime ();
    if  (tt != 0.0f)
    {  
      totalTrainngTime += tt;
      numTrainingTimeEncountered++;
    }
  }

  if  (numTrainingTimeEncountered > 0)
    return  totalTrainngTime / (float)numTrainingTimeEncountered;
  else
    return  0.0f;
}  /* TrainingTime */




float  ClassGroupTotals::TestingTimeMean (int retraining)  const
{
  ValidateRetraining ("TestingTime", retraining);

  float  totalTestingTime = 0.0f;
  int    numTestingTimeEncountered = 0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    float tt = totals [randomPass][retraining]->TestingTime ();
    if  (tt != 0.0f)
    {  
      totalTestingTime += tt;
      numTestingTimeEncountered++;
    }
  }

  if  (numTestingTimeEncountered > 0)
    return  totalTestingTime / (float)numTestingTimeEncountered;
  else
    return 0.0f;

}  /* TestingTimeMean */





void  ClassGroupTotals::NumOfSupportVectors (int    retraining, 
                                             int    randomPass,  
                                             int    _numOfSupportVectors,
                                             float  _trainingTime,
                                             float  _testingTime
                                            )  const
{
  ValidateRetraining ("NumOfSupportVectors", retraining);
  ValidatePass  ("NumOfSupportVectors", randomPass);

  totals[randomPass][retraining]->NumOfSupportVectors (_numOfSupportVectors, 
                                                       _trainingTime,
                                                       _testingTime
                                                      );
}



const TrainingImageList&  ClassGroupTotals::TrainingImages  (int  retraining,
                                                             int  randomPass
                                                            )
{
  ValidateRetraining ("TrainingImages", retraining);
  ValidatePass  ("TraiingImages", randomPass);
  return  totals[randomPass][retraining]->TrainingImages ();
}



TrainingImageListPtr  ClassGroupTotals::TrainingImages (int  retraining)
{
  ValidateRetraining ("TrainingImages", retraining);

  TrainingImageListPtr  trainingImages = new TrainingImageList (false, 5);

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    trainingImages->AddQueue (totals[randomPass][retraining]->TrainingImages ());
  }
  
  return  trainingImages;
}  /* TrainingImages */



VectorFloat  ClassGroupTotals::MeanNonStreamPlaces (int  retraining)
{
  ValidateRetraining ("NonStreamPlaces", retraining);

  int  randomPass;
  int  x;
  
  int  ipr = 0;

  VectorFloat  nonStreamPlaces;


  for  (randomPass = 0;  randomPass < NumOfRandomPasses ();  randomPass++)
  {
    ipr = Max (ipr, (int)(totals[randomPass][retraining]->TrainingImages ().size ()));
  }

  for  (x = 0;  x < ipr;  x++)
  {
    int  numImages = 0;
    int  total     = 0;

    for  (randomPass = 0;  randomPass < NumOfRandomPasses ();  randomPass++)
    {
      const TrainingImageList  trainingImages = totals[randomPass][retraining]->TrainingImages ();
      // const VectorInt*  nonStreamPlaces = totals[randomPass][retraining]->NonStreamPlaces ();
      if  (x < (int)trainingImages.size ())
      {
        total += trainingImages[x].NonStreamPlace ();
        numImages++;
      }
    }

    float  mean = 0.0f;
    if  (numImages > 0)
      mean = (float)total / (float)numImages;

    nonStreamPlaces.push_back (mean);
  }

  return  nonStreamPlaces;
}  /* MeanNonStreamPlaces */







uint  ClassGroupTotals::MaxNumOfProbs (int  retraining,
                                       int  classIDX
                                      )  const
{
  ValidateClassIDX ("MaxNumOfProbs", classIDX);
  ValidateRetraining    ("MaxNumOfProbs", retraining);

  uint maxNumOfProbs = 0;

  for  (int  randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    maxNumOfProbs = Max (totals[randomPass][retraining]->NumOfProbabilities (classIDX), maxNumOfProbs);
  }

  return  maxNumOfProbs;
}






uint  ClassGroupTotals::MaxNumOfProbs  (int            retraining,
                                        MLClassPtr  mlClass
                                       )    const
{
  return MaxNumOfProbs (retraining,  MLClassIDX ("MaxNumOfProbs", mlClass));
}





float  ClassGroupTotals::AvgProb (int  retrainingIDX,
                                   int  classIDX,
                                   uint idx
                                  )    const
{
  ValidateClassIDX ("AvgProb", classIDX);
  ValidateRetraining    ("AvgProb", retrainingIDX);

  float  totProb    = 0.0;
  int    totNumProb = 0;

  for  (int randomPass = 0; randomPass < numOfRandomPasses; randomPass++)
  {
    if  (idx < totals [randomPass][retrainingIDX]->NumOfProbabilities (classIDX))
    {
      totNumProb++;
      totProb += totals [randomPass][retrainingIDX]->Probabilities (classIDX, idx);
    }
  }

  if  (totNumProb <= 0)
    return 0.0;
  else
    return  totProb / (float)totNumProb;
}  /* AvgProb */





float   ClassGroupTotals::AvgProb (int            retraining, 
                                    MLClassPtr  mlClass,  
                                    uint           idx
                                   )      const
{
  int  classIDX = MLClassIDX ("ClassAvgProb", mlClass);
  
  return  AvgProb (retraining, classIDX, idx);
}




void   ClassGroupTotals::CalcAvgProb (int            retraining, 
                                      MLClassPtr  mlClass,
                                      float&        numOfProbabilities,
                                      float&        avgProbability
                                     )  const
{
  int  classIDX = MLClassIDX ("ClassAvgProb", mlClass);

  avgProbability = (float)0.0;

  numOfProbabilities = (float)0.0;

  for (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    uint   numOfProbsThisPass;
    float  avgProbThisPass;

    totals[randomPass][retraining]->CalcAvgProb (classIDX, numOfProbsThisPass, avgProbThisPass);
    avgProbability += (float)numOfProbsThisPass * avgProbThisPass;
    numOfProbabilities += numOfProbsThisPass;
  }


  if  (numOfProbabilities > 0)
    avgProbability = avgProbability / numOfProbabilities;


  numOfProbabilities = numOfProbabilities / (float)numOfRandomPasses;
  return;
}  /* AvgProb */




VectorDouble  ClassGroupTotals::AvgProbVector (int  retraining,
                                               int  classIDX
                                              )     const
{
  ValidateClassIDX ("AvgProbVector",  classIDX);
  ValidateRetraining    ("AvgProbVector",  retraining);

  VectorDouble  avgVector;

  int  maxNumProbs = MaxNumOfProbs (retraining, classIDX);

  for  (int idx = 0; idx < maxNumProbs; idx++)
  {
    avgVector.push_back (AvgProb (retraining, classIDX, idx));
  }

  return  avgVector;
}  /* AvgProbVector */




VectorDouble  ClassGroupTotals::AvgProbVector (int            retraining,
                                               MLClassPtr  mlClass
                                              )     const
{
  return  AvgProbVector (retraining,  MLClassIDX ("ClassAvgProbVector", mlClass));
}




// Call this method when adding an image to Training Library
void  ClassGroupTotals::AddAProbability (int              retraining,       
                                         int              randomPass,          
                                         MLClassPtr    mlClass,
                                         float           probability,
                                         const KKStr&    imageFileName,
                                         int              position,
                                         float            percentile
                                        )
{
  ValidateRetraining    ("AvgProbVector",  retraining);
  ValidatePass     ("AvgProbVector",  randomPass);

  if  (probabilities)
  {
    probabilities [retraining][randomPass]->PushOnBack (new ProbabilityHistoryBit (mlClass, probability, imageFileName, position, percentile));
    return;
  }

  int  classIDX = mlClasses.PtrToIdx (mlClass);

  if  (classIDX < 0)
  {
     cerr << endl;
     cerr << "**** ERROR ****,   AddAProbability,  Image Class[" << mlClass->Name () << "] Undefined." << endl;
     osWaitForEnter ();
     exit (-1);
  }


  ClassFocusPtr cf = totals[randomPass][retraining];

  cf->AddProbability (classIDX, mlClass, imageFileName, probability, position, percentile);

  maxTrainingImagesPerRetraining = Max (maxTrainingImagesPerRetraining, cf->NumOfProbabilities ());
}  /* AddAProbability */




VectorFloat  ClassGroupTotals::GetNewTrainingImagesThisRetraining (int  retraining)
{
  VectorFloat  classCounts;

  for  (int  classIDX = 0;  classIDX < mlClasses.QueueSize ();  classIDX++)
  {
    int  totalImagesAddedThisRetraning = 0;
    for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
      totalImagesAddedThisRetraning += totals[randomPass][retraining]->GetNumNewImagesInClass (classIDX);

    float  imagesAddedThisRetraining = (float)totalImagesAddedThisRetraning / (float)numOfRandomPasses;
    classCounts.push_back (imagesAddedThisRetraining);
  }

  return  classCounts;
}  /* GetNewTrainingImagesAddedPerClassPerRetraining */




void  ClassGroupTotals::SetSortMethodUsed (int            retraining,
                                           MLClassPtr  mlClass, 
                                           int            randomPass,
                                           SortOrderType  sortMethodUsed
                                          )
{
  ValidateRetraining    ("SetSortMethodUsed",  retraining);
  ValidatePass     ("SetSortMethodUsed",  randomPass);


  if  (sortMethods)
  {
    sortMethods [retraining][randomPass]->PushOnBack (new SortMethodHistoryBit (mlClass, sortMethodUsed));
    return;
  }

  int  classIDX = mlClasses.PtrToIdx (mlClass);
  if  (classIDX < 0)
  {
     cerr << endl;
     cerr << "**** ERROR ****,   SetSortMethodUsed,  Image Class[" << mlClass->Name () << "] Undefined." << endl;
     osWaitForEnter ();
     exit (-1);
  }
  
  totals[randomPass][retraining]->SetSortMethodUsed (classIDX, sortMethodUsed);
}  /*  SetSortMethodUsed */






KKStr   ClassGroupTotals::SortMethodsUsed (int  retraining)  const
{
  int     classIDX;
  int     randomPass;
  KKStr  result;
  int     sortMethodsCount[10];
  int     x;

  ValidateRetraining    ("SortMethodsUsed",  retraining);

  for  (x = 0;  x < 10;  x++)
    sortMethodsCount[x] = 0;
   

  for  (classIDX = 0;  classIDX < mlClasses.QueueSize ();  classIDX++)
  {
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      SortOrderType  methodUsed = totals[randomPass][retraining]->SortMethodUsed (classIDX);
      sortMethodsCount[methodUsed] += totals[randomPass][retraining]->NumOfProbabilities (classIDX);
    }
  }

  for  (x = 0; x < 10;  x++)
  {
    if  (sortMethodsCount[x] > 0)
    {
      if  (!result.Empty ())
        result << ", ";

      result << SortOrderDescr ((SortOrderType)x) << "(" << sortMethodsCount[x] << ")";
    }
  }

  return  result;
}  /* SortMethodsUsed */




KKStr   ClassGroupTotals::SortMethodsUsed (int            retraining,
                                            MLClassPtr  mlClass
                                           )  const
{

  int     classIDX;
  int     randomPass;
  KKStr  result;
  int     sortMethodsCount[10];
  int     x;

  ValidateRetraining    ("SortMethodsUsed",  retraining);


  classIDX = MLClassIDX ("SortMethodsUsed", mlClass);

  for  (x = 0;  x < 10;  x++)
    sortMethodsCount[x] = 0;
   

  for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    SortOrderType  methodUsed = totals[randomPass][retraining]->SortMethodUsed (classIDX);
    sortMethodsCount[methodUsed] += totals[randomPass][retraining]->NumOfProbabilities (classIDX);
  }

  for  (x = 0; x < 10;  x++)
  {
    if  (sortMethodsCount[x] > 0)
    {
      if  (!result.Empty ())
        result << ", ";

      result << SortOrderDescr ((SortOrderType)x) << "(" << sortMethodsCount[x] << ")";
    }
  }

  return  result;
}  /* SortMethodsUsed */





void  ClassGroupTotals::Prediction (int             retraining,
                                    int             randomPass,
                                    MLClassPtr   knownClass,
                                    MLClassPtr   predictedClass,
                                    float          probability
                                   )
{
#ifdef  KKDEBUG
  ValidateRetraining    ("Prediction",  retraining);
  ValidatePass     ("Prediction",  randomPass);
#endif

  if  (predictions)
  {
    predictions [retraining][randomPass]->PushOnBack (new PredictionHistoryBit (knownClass, predictedClass, probability));
    return;
  }

  int  knownClassIDX     = MLClassIDX ("Prediction: known", knownClass);

  int  predictedClassIDX = MLClassIDX ("Prediction: predicted", predictedClass);


  totals[randomPass][retraining]->Prediction (knownClassIDX, predictedClassIDX, probability);

  totalCounts++;

  if  (knownClassIDX == predictedClassIDX)
  {
    totalCorrect++;
  }
}  /* Prediction */







float   ClassGroupTotals::AvgNumOfProbs (int  retraining,
                                          int  classIDX
                                         )   const
{
  ValidateClassIDX ("AvgNumOfProbs", classIDX);
  ValidateRetraining    ("AvgNumOfProbs", retraining);


  int totalNumOfProbs = 0;

  for  (int  randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    totalNumOfProbs += totals[randomPass][retraining]->NumOfProbabilities (classIDX);
  }

  return  (float)totalNumOfProbs / (float)numOfRandomPasses;
}  /* ClassAvgNumOfProbs */





float   ClassGroupTotals::AvgNumOfProbs (int            retraining,
                                          MLClassPtr  mlClass
                                         )    const
{
  return AvgNumOfProbs (retraining,  MLClassIDX ("AvgNumOfProbs", mlClass));
}





float  ClassGroupTotals::Count (int  retraining,
                                 int  classIDX
                                )   const
{
  ValidateClassIDX ("Count", classIDX);
  ValidateRetraining    ("Count", retraining);

  
  
  int  count = 0;
  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    count += totals[randomPass][retraining]->Count ();
  }
  
  return  (float)count / (float)numOfRandomPasses;
}





float   ClassGroupTotals::Count (int            retraining,
                                  MLClassPtr  mlClass
                                 )  const
{
  return  Count (retraining, MLClassIDX ("Count", mlClass));
}






float   ClassGroupTotals::Correct (int  retraining,
                                    int  classIDX
                                   )   const
{
  ValidateClassIDX ("Correct", classIDX);
  ValidateRetraining    ("Correct", retraining);

  int  correct = 0;
  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    correct += totals[randomPass][retraining]->Predicted (classIDX, classIDX);
  }
  
  return  (float)correct / (float)numOfRandomPasses;
}




float   ClassGroupTotals::Correct (int            retraining,
                                    MLClassPtr  mlClass
                                   )   const
{
  return  Correct (retraining, MLClassIDX ("Count", mlClass));
}





KKStr  ClassGroupTotals::ProbabilityStr (VectorDouble&  v)  const
{
  KKStr  str;

  for  (uint idx = 0; idx < v.size (); idx++)
  {
    if  (idx > 0)
      str << "\t";

    str << StrFormatDouble ((100.0 * v[idx]), "zz0.00") << "%";
  }

  return str;
}




float  ClassGroupTotals::Accuracy (int  retraining)  const
{
  ValidateRetraining    ("Accuracy", retraining);

  float  totalAccuracy = 0.0f;


  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    int  c = totals[randomPass][retraining]->Count ();
    if  (c > 0)
    {
      float  accuracy = (float)totals[randomPass][retraining]->Correct () / (float)c;
      totalAccuracy += accuracy;
    }
  }

  return  100.0f * totalAccuracy / (float)numOfRandomPasses;
} /* Accuracy */




float  ClassGroupTotals::AccuracyForRetrainingAndPass (int  retraining,
                                                        int  randomPass
                                                       )  const
{
  ValidateRetraining ("AccuracyForRetrainingAndPass", retraining);
  ValidatePass  ("AccuracyForRetrainingAndPass", randomPass);

  int  c = totals[randomPass][retraining]->Count ();
  if  (c == 0)
    return  (float)0.0;

  return  (float)totals[randomPass][retraining]->Correct () / (float)c;
} /* Accuracy */





//*******************************************************************************
//*                   AccuracyDeltaForRetrainingAndPass                         *
//*                                                                             *
//*  Calcs the change in accuracy occuring at a given retraining and randomPass *
//*  by subtracting the accuracy from a prev retraining from the one for the    *
//*  next retraining.                                                           *
//*******************************************************************************
float  ClassGroupTotals::AccuracyDeltaForRetrainingAndPass (int  retraining,
                                                            int  randomPass
                                                           )  const
{
  ValidateRetraining ("AccuracyDeltaForRetrainingAndPass", retraining);
  ValidatePass       ("AccuracyDeltaForRetrainingAndPass", randomPass);

  int  retrainingIdx0 = retraining - 1;
  int  retrainingIdx1 = retraining + 1;

  if  (retrainingIdx0 < 0)
    retrainingIdx0 = 0;

  if  (retrainingIdx1 >= numOfRetrainings)
    retrainingIdx1 = numOfRetrainings - 1;


  float  accuracyDelta0 = 0.0f;
  float  accuracyDelta1 = 0.0f;

  int  c = totals[randomPass][retrainingIdx0]->Count ();
  if  (c > 0)
    accuracyDelta0 =  (float)totals[randomPass][retrainingIdx0]->Correct () / (float)c;

  c = totals[randomPass][retrainingIdx1]->Count ();
  if  (c > 0)
    accuracyDelta1 =  (float)totals[randomPass][retrainingIdx1]->Correct () / (float)c;

  return  (accuracyDelta1 - accuracyDelta0) / (float)(retrainingIdx1 - retrainingIdx0);
} /* AccuracyDeltaForRetrainingAndPass */






void  ClassGroupTotals::CalcRetrainingAccuracyStats (int     retraining,
                                                     float&  mean,
                                                     float&  variance,
                                                     float&  stdDev,
                                                     float&  min,
                                                     float&  max,
                                                     float&  accuracyDelta
                                                    )  const
{
  ValidateRetraining    ("CalcRetrainingAccuracyStats", retraining);

  int  randomPass;

  min = FFLOAT_MAX;
  max = FFLOAT_MIN;

  float  totalAccuracy      = 0.0f;
  float  accuracyDeltaTotal = 0.0f;

  float*  accuracys = new float[numOfRandomPasses];

  //  2004-12-08  KK
  //  We want the stats calculated to refect the number of actual results files 
  //  encountered.  This way we can get results before all active leaning jobs have completed.
  int  numPassesEncountered = 0;

  for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    float  accuracy = 0.0;

    int  count   = totals[randomPass][retraining]->Count ();
    int  correct = totals[randomPass][retraining]->Correct ();

    if  (count > 0)
    {
      accuracy = (float)((float)correct / (float)count);
      numPassesEncountered++;
      if  (accuracy > max)
        max = accuracy;

      if  (accuracy < min)
        min = accuracy;
    }

    accuracys[randomPass] = accuracy;
    totalAccuracy += accuracy;

    accuracyDeltaTotal += AccuracyDeltaForRetrainingAndPass (retraining, randomPass);
  }

  mean          = totalAccuracy      / (float)numPassesEncountered;
  accuracyDelta = accuracyDeltaTotal / (float)numPassesEncountered;

  float   y = 0.0f;
  float   deletaAccuracy;
  for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    if  (accuracys[randomPass] != 0.0f)
    {
      deletaAccuracy = (mean - accuracys[randomPass]);
      y += deletaAccuracy * deletaAccuracy;
    }
  }

  variance = y / (float)numPassesEncountered;

  stdDev = (float)sqrt (variance);
  
  delete  accuracys;

  return;
}  /* CalcRetrainingAccuracyStats */






vector<float>  ClassGroupTotals::CalcAccuraciesByClass (int retraining)  const
{
  vector<float>  classAccuracies;

  for  (int x = 0;  x < mlClasses.QueueSize ();  x++)
  {
    MLClassPtr  mlClass = mlClasses.IdxToPtr (x);

    float  totalAccuracy = 0.0f;
    int numPassesEncountered = 0;

    for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      float accuracy = 0.0;

      int  count   = totals[randomPass][retraining]->Count (x);
      int  correct = totals[randomPass][retraining]->Predicted (x, x);

      if  (count > 0)
      {
        accuracy = (float)((float)correct / (float)count);
        totalAccuracy += accuracy;
        numPassesEncountered++;
      }
    }

    float  accurcayMean = totalAccuracy / (float)numPassesEncountered;
    classAccuracies.push_back (accurcayMean);
  }

  return  classAccuracies;
}  /* CalcAccuraciesByClass */




void  ClassGroupTotals::CalcRetrainingSupportPointStats (int     retraining,
                                                         float&  mean,
                                                         float&  variance,
                                                         float&  stdDev,
                                                         float&  min,
                                                         float&  max,
                                                         float&  trainingTimeMean,
                                                         float&  testingTimeMean
                                                        )  const
{
  ValidateRetraining ("CalcRetrainingSupportPointStats", retraining);

  FFLOAT  totalNumOfSupportPoints = 0.0f;

  min = FFLOAT_MAX;
  max = FFLOAT_MIN;


  float  totalTrainingTime = 0.0f;
  float  totalTestingTime  = 0.0f;

  //  2004-12-08  KK
  //  We want the stats calculated tp refect the number of actual results files 
  //  encountered.  This way we can get results before all active leaning jobs have completed.
  int  numPassesEncountered = 0;

  int  numTrainingTimesEncounterd = 0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    float  numOfSupportPoints = (float)totals [randomPass][retraining]->NumOfSupportVectors ();

    if  (numOfSupportPoints > 0.0f)
    {
      numPassesEncountered++;

      if  (numOfSupportPoints > max)
        max = numOfSupportPoints;

      if  (numOfSupportPoints < min)
        min = numOfSupportPoints;
    }

    totalNumOfSupportPoints += numOfSupportPoints;

    float  tt = totals [randomPass][retraining]->TrainingTime ();

    if  (tt != 0.0f)
    {
      numTrainingTimesEncounterd++;
      totalTrainingTime += tt;
      totalTestingTime  += totals [randomPass][retraining]->TestingTime ();
    }
  }

  if  (numPassesEncountered == 0)
    mean = 0.0;
  else
    mean = totalNumOfSupportPoints / numPassesEncountered;


  if  (numTrainingTimesEncounterd == 0)
  {
    trainingTimeMean = 0.0;
    testingTimeMean  = 0.0;
  }
  else
  {
    trainingTimeMean = totalTrainingTime / numTrainingTimesEncounterd;
    testingTimeMean  = totalTestingTime  / numTrainingTimesEncounterd;
  }

  variance = 0.0f;
  float  y = 0.0f;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    float  numOfSupportPoints = (float)totals [randomPass][retraining]->NumOfSupportVectors ();
    if  (numOfSupportPoints > 0.0f)
    {
      float  deltaSupportPoints = mean - totals [randomPass][retraining]->NumOfSupportVectors ();
      y += deltaSupportPoints * deltaSupportPoints;
    }
  }

  if  (numPassesEncountered == 0)
    variance = 0.0;
  else
    variance = y / numPassesEncountered;

  stdDev = (float)sqrt (variance);
  
  return;
}  /* CalcRetrainingSupportPointStats */




float  ClassGroupTotals::Accuracy (int  retraining,
                                    int  classIDX
                                   )  const
{
  ValidateClassIDX ("Accuracy", classIDX);
  ValidateRetraining    ("Accuracy", retraining);

  uint  count   = 0;
  uint  correct = 0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    count   += totals[randomPass][retraining]->Count (classIDX);
    correct += totals[randomPass][retraining]->Predicted (classIDX, classIDX);
  }  
  
  if  (count == 0)
    return 0.0;

  return  (float)100.0 * ((float)correct / (float)count);
}  /* Accuracy */





float  ClassGroupTotals::Accuracy (int            retraining,
                                    MLClassPtr  mlClass
                                   )  const
{
  int  classIDX = MLClassIDX ("Accuracy", mlClass);
  return  Accuracy (retraining,  classIDX);
}





KKStr  ClassGroupTotals::ProbabilityStr (int  retraining,
                                          int  classIDX
                                         )   const
{
  ValidateClassIDX ("ProbabilityStr", classIDX);
  ValidateRetraining    ("ProbabilityStr", retraining);

  VectorDouble  v = AvgProbVector (retraining, classIDX);
  return  ProbabilityStr (v);
}




KKStr  ClassGroupTotals::ProbabilityStr (int            retraining,
                                          MLClassPtr  mlClass
                                         )  const
{
  int  classIDX = MLClassIDX ("ProbabilityStr", mlClass);
 
  ValidateClassIDX ("ProbabilityStr", classIDX);
  ValidateRetraining    ("ProbabilityStr", retraining);

  VectorDouble  v = AvgProbVector (retraining, classIDX);
  return  ProbabilityStr (v);
}







int  CountTabs (KKStr&  str)
{
  int  c = 0;

  int  l = str.Len ();


  for  (int x = 0; x < l; x++)
  {
    if  (str[x] == '\t')
      c++;
  }

  return c;
}  /* CountTabs */






void  ClassGroupTotals::PrintGroupBreakDown (ostream&  report)  const
{
  MLClassListIterator  icIDX (mlClasses);

  MLClassPtr  mlClass = NULL;

  report << endl << endl;
  report << "SortOrder" << "\t" << SortOrderDescr (sortOrder) << endl << endl;

  report  << "Grp#\t";

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    report << mlClass->Name () << "\t\t\t\t";

  report  << "Avg" << endl;

  int  classIDX = 0;

  float*  totalNewTrainingImages = new float[mlClasses.QueueSize ()];
  for  (int x = 0; x < mlClasses.QueueSize (); x++)
    totalNewTrainingImages[x] = (float)0.0;


  for  (int  retraining = 0; retraining < numOfRetrainings;  retraining++)
  {
    float  totalCountOnLine = 0.0;
    float  totalAccuracy    = 0.0;

    classIDX = 0;

    report << retraining;
    for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    {
      classIDX = MLClassIDX ("PrintGroupBreakDown", mlClass);

      float  accuracy      = Accuracy      (retraining, classIDX);
      float  count         = Count         (retraining, classIDX);
      float  avgNumOfProbs = AvgNumOfProbs (retraining, classIDX);

      report << "\t" << StrFormatDouble (accuracy,      "zz0.00") << "%";
      report << "\t" << StrFormatDouble (count,         "zz,zz0.0");
      report << "\t" << StrFormatDouble (avgNumOfProbs, "zz,zz0.0");      

      totalCountOnLine = totalCountOnLine + count;
      totalAccuracy    = totalAccuracy    + accuracy * count;

      totalNewTrainingImages[classIDX] += avgNumOfProbs;
      report << "\t" << StrFormatDouble (totalNewTrainingImages[classIDX], "zz,zz0.0");      
    }


    float  avgAccuracy = 0.0;

    if  (totalCountOnLine > 0)
      avgAccuracy = totalAccuracy / totalCountOnLine;

    report << "\t" << StrFormatDouble (avgAccuracy, "zz#.##");


    for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    {
      KKStr  probStr = ProbabilityStr (retraining,  mlClass);

      uint  numOfTabs = CountTabs (probStr);
      while  (numOfTabs < (maxTrainingImagesPerRetraining + 2))
      {
        probStr << "\t";
        numOfTabs++;
      }

      report << "\t" << probStr;
    }

    report << endl;
  }


  report << endl
         << endl;

  
  delete  totalNewTrainingImages;

}  /* PrintGroupBreakDown */






KKStr  ClassGroupTotals::FormatSummaryHeader ()  const
{
  KKStr  formatedStr;


  MLClassPtr  mlClass = NULL;

  MLClassListIterator  icIDX (mlClasses);

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    formatedStr << mlClass->Name () << "\t\t";

  formatedStr  << "Avg\t\t";

  return  formatedStr;

}  /* FormatSummaryHeader */








KKStr  ClassGroupTotals::FormatSummaryLine (int  retraining)  const
{
  ValidateRetraining    ("FormatSummaryLine", retraining);

  KKStr  formatedStr;

  MLClassListIterator  icIDX (mlClasses);

  MLClassPtr  mlClass = NULL;


  int  classIDX = 0;


  float  totalCountOnLine = 0.0;
  float  totalAccuracy    = 0.0;

  float  allClassesNewTrainingImages      = 0.0;
  float  totalAllClassesNewTrainingImages = 0.0;

  classIDX = 0;

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
  {
    classIDX = MLClassIDX ("PrintGroupBreakDown", mlClass);

    float  totalNewTrainingImages = 0.0;
    for  (int x = 0; x < retraining; x++)
    {
      totalNewTrainingImages += AvgNumOfProbs (x, classIDX);
    }

    float  accuracy      = Accuracy      (retraining, classIDX);
    float  count         = Count         (retraining, classIDX);
    float  avgNumOfProbs = AvgNumOfProbs (retraining, classIDX);

    formatedStr << "\t" << StrFormatDouble (accuracy,      "zz0.00") << "%";
    //formatedStr << "\t" << StrFormatDouble (count,         "zz,zz0.0");
    //formatedStr << "\t" << StrFormatDouble (avgNumOfProbs, "zz,zz0.0");      

    totalCountOnLine            += count;
    totalAccuracy               += accuracy * count;
    allClassesNewTrainingImages += avgNumOfProbs;

    formatedStr << "\t" << StrFormatDouble (totalNewTrainingImages, "zz,zz0.0");      

    totalAllClassesNewTrainingImages += totalNewTrainingImages;
  }


  float  avgAccuracy = 0.0;

  if  (totalCountOnLine > 0)
    avgAccuracy = totalAccuracy / totalCountOnLine;

  formatedStr << "\t" << StrFormatDouble (avgAccuracy, "zz0.00") << "%";

  //formatedStr << "\t" << StrFormatDouble (allClassesNewTrainingImages,      "zz,zz0.0");
  formatedStr << "\t" << StrFormatDouble (totalAllClassesNewTrainingImages, "zz,zz0.0");

  return  formatedStr;
}  /* FormatSummaryLine */





KKStr  ClassGroupTotals::OneImageFormatHeaderLine1 ()  const
{
  KKStr  formatedStr;
    
  MLClassPtr  mlClass = NULL;

  MLClassListIterator  icIDX (mlClasses);

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    formatedStr << mlClass->Name () << "\t\t\t\t";

  formatedStr  << "Avg\t\t\t\t\t\t\t\t\t";

  return  formatedStr;
} /* OneImageFormatHeaderLine */




KKStr  ClassGroupTotals::OneImageFormatHeaderLine2 ()  const
{
  KKStr  formatedStr;
    
  MLClassPtr  mlClass = NULL;

  MLClassListIterator  icIDX (mlClasses);

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
    formatedStr << "Accuracy" 
        << "\t" << "Count" 
        << "\t" << "Prob" 
        << "\t" << "Accum" 
        << "\t";

  formatedStr  << "Accuracy" 
               << "\t" << "Min" 
               << "\t" << "Max" 
               << "\t" << "StdDev" 
               << "\t" << "Delta"
               << "\t" << "Count" 
               << "\t" << "Prob" 
               << "\t" << "Accum"
               << "\t" << "num_S/V"
               << "\t" << "TrainingTime"
               << "\t" << "TestingTime";

  return  formatedStr;
}  /* OneImageFormatHeaderLine2 */





KKStr  ClassGroupTotals::OneImageFormatSummaryLine (int  retraining)  const
{
  ValidateRetraining ("OneImageFormatSummaryLine", retraining);

  KKStr  formatedStr (200u);  // Reserving at least 200 chars for KKStr.

  MLClassListIterator  icIDX (mlClasses);

  MLClassPtr  mlClass = NULL;

  int  classIDX = 0;

  float  totalAllClassesNewTrainingImages = 0.0;
  float  totalProbability                 = 0.0;
  float  totalNumOfProbabilities          = 0.0;

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
  {
    classIDX = MLClassIDX ("PrintGroupBreakDown", mlClass);

    float  totalNewTrainingImages = 0.0;
    for  (int x = 0; x < retraining; x++)
    {
      totalNewTrainingImages += AvgNumOfProbs (x, classIDX);
    }

    float  accuracy      = Accuracy      (retraining, classIDX);
    float  count         = Count         (retraining, classIDX);

    formatedStr << StrFormatDouble (accuracy, "zzz0.00") << "%";


    float  numOfProbabilities;
    float  avgProbs;

    CalcAvgProb (retraining, mlClass, numOfProbabilities, avgProbs);

    formatedStr << "\t" << StrFormatDouble (numOfProbabilities, "zzz0.00")
                << "\t" << StrFormatDouble (avgProbs * 100.0,   "zzz0.00");

    totalProbability        += avgProbs * numOfProbabilities;
    totalNumOfProbabilities += numOfProbabilities;

    formatedStr << "\t" << StrFormatDouble (totalNewTrainingImages, "zzz,zz0.0") << "\t";      

    totalAllClassesNewTrainingImages += totalNewTrainingImages;
  }


  float  mean;
  float  variance;
  float  stdDev;
  float  min;
  float  max;
  float  accuracyDelta;

  CalcRetrainingAccuracyStats (retraining, mean, variance, stdDev, min, max, accuracyDelta);

  float  avgProbAllClasses = (float)100.0 * totalProbability / (float)totalNumOfProbabilities;

  formatedStr << StrFormatDouble (mean          * 100, "zzz0.00")  << "%" << "\t";
  formatedStr << StrFormatDouble (min           * 100, "zzz0.00")  << "%" << "\t";
  formatedStr << StrFormatDouble (max           * 100, "zzz0.00")  << "%" << "\t";
  formatedStr << StrFormatDouble (stdDev        * 100, "zzz0.00")  << "%" << "\t";
  formatedStr << StrFormatDouble (accuracyDelta * 100, "zz0.0000") << "%" << "\t";

  formatedStr << StrFormatDouble (totalNumOfProbabilities, "ZZZ0.00") << "\t";
  formatedStr << StrFormatDouble (avgProbAllClasses,       "zZZ0.00")  << "\t";

  formatedStr << StrFormatDouble (totalAllClassesNewTrainingImages, "zzz,zz0.0") << "\t";

  formatedStr << StrFormatDouble (NumOfSupportVectors (retraining), "zzz,zz0.0");
  formatedStr << StrFormatDouble (TrainingTimeMean    (retraining), "zzz,zz0.0");
  formatedStr << StrFormatDouble (TestingTimeMean     (retraining), "zzz,zz0.0");


  return  formatedStr;
}  /* OneImageFormatSummaryLine */





float  ClassGroupTotals::Predicted (int  retraining,
                                     int  knownIDX,
                                     int  predIDX
                                    )  const

{
  ValidateClassIDX ("Predicted - knownIDX", knownIDX);
  ValidateClassIDX ("Predicted - predIDX",  predIDX);

  int  predicted = 0;

  for  (int randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
  {
    predicted += totals[randomPass][retraining]->Predicted (knownIDX, predIDX);
  }


  return  (float)predicted / (float)numOfRandomPasses;
} /* Predicted */





void  ClassGroupTotals::ConfusionMatrix (ostream&  report,
                                         int       retraining
                                        )  const
{
  ValidateRetraining    ("ConfusionMatrix", retraining);

  report << "Class" << endl;

  report << "Name" << "\t" << "Count";

  MLClassListIterator  icIDX (mlClasses);

  uint  numOfClasses = mlClasses.QueueSize ();

  uint           colIDX = 0;
  uint           rowIDX = 0;

  MLClassPtr  colClass;
  MLClassPtr  rowClass;

  float*  classTotals = new float[numOfClasses];
  for  (colIDX = 0;  colIDX < numOfClasses;  colIDX++)
  {
    classTotals[colIDX] = 0.0;
  }


  for  (colIDX = 0;  colIDX < numOfClasses;  colIDX++)
  {
    colClass = mlClasses.IdxToPtr (colIDX);
    report << "\t" << colClass->Name ();
  }
  report << endl;


  float  totalOfClasses = 0.0;

  for  (rowIDX = 0;  rowIDX < numOfClasses;  rowIDX++)
  {
    rowClass = mlClasses.IdxToPtr (rowIDX);

    report << rowClass->Name () << "\t" << StrFormatDouble (Count (retraining, rowIDX), "zzz,zz0.0");

    totalOfClasses = totalOfClasses + Count (retraining, rowIDX);

    for  (colIDX = 0;  colIDX < numOfClasses;  colIDX++)
    {
      float  x = Predicted (retraining, rowIDX, colIDX);
      report << "\t" << StrFormatDouble (x, "zzz,zz0.0");
      classTotals[colIDX] += x;
    }

    report << endl;
  }
  
  report << "Totals" << "\t" <<  StrFormatDouble (totalOfClasses, "zzz,zz0.0");
  for  (colIDX = 0;  colIDX < numOfClasses;  colIDX++)
  {
    report << "\t" << StrFormatDouble (classTotals[colIDX], "zzz,zz0.0");
  }


  report << endl
         << endl;

  for  (rowIDX = 0;  rowIDX < numOfClasses;  rowIDX++)
  {
    rowClass = mlClasses.IdxToPtr (rowIDX);

    float  classCount = Count (retraining, rowIDX);

    float  gc = RetrainingCounts (retraining);
    float  percentOfTotal = 0.0;
    if  (gc > 0.0)
       percentOfTotal = (float)100.0 * classCount / gc;

    report  << rowClass->Name () << "\t" << StrFormatDouble (percentOfTotal, "zzz,zz0.0") << "%";

    for  (colIDX = 0;  colIDX < numOfClasses;  colIDX++)
    {
      float  percent = 0.0;

      if  (classCount > 0)
        percent = (float)100.0 * Predicted (retraining, rowIDX, colIDX) / classCount;

      report << "\t" << StrFormatDouble (percent, "zzz,zz0.00") << "%";
    }

    report << endl;
  }
}  /* ConfusionMatrix */




void  ClassGroupTotals::AddIn (ClassGroupTotals&  ai)
{
  int  retraining;
  int  randomPass;

  if  (collectHistory)
  {
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
      {
        predictions   [retraining][randomPass]->AddQueue (*ai.predictions   [retraining][randomPass]);
        probabilities [retraining][randomPass]->AddQueue (*ai.probabilities [retraining][randomPass]);
        sortMethods   [retraining][randomPass]->AddQueue (*ai.sortMethods   [retraining][randomPass]);
      }
    }
  }
  else
  {
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
      {
        totals[randomPass][retraining]->AddIn (*ai.totals[randomPass][retraining]);
      }
    }
  }

  totalCounts  += ai.totalCounts;
  totalCorrect += ai.totalCorrect;
}  /* AddIn */





void  ClassGroupTotals::WriteOutHistory (KKStr  fileName)
{
  log.Level (20) << "ClassGroupTotals::WriteOutHistory  FileName[" << fileName << "]" << endl;

  if  (!collectHistory)
  {
    log.Level (-1) << "ClassGroupTotals::WriteOutHistory   *** ERROR ***,  Can not write out history because was not collecting it." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  ofstream  o (fileName.Str (), ios::out);

  o << "///Dimensions"      << "\t" << numOfRetrainings << "\t" << numOfRandomPasses << "\t" << streamBlockSize << endl;
  o << "///New_ClassTotals" << "\t" << mlClasses                          << endl;
  o << "///SortOrder"       << "\t" << SortOrderDescr (sortOrder)            << endl;

  int retraining = 0;
  int randomPass  = 0;

  for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
  {
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      if  (predictions[retraining][randomPass]->QueueSize () > 0)
      {
        HistoryBitListPtr  hl = predictions[retraining][randomPass];

        hl->SortHistory ();

        PredictionHistoryBitPtr  p = NULL;

        int  i = 0;
        p = (PredictionHistoryBitPtr)hl->IdxToPtr (i);
        while  (i <  hl->QueueSize ())
        {
          MLClassPtr  lastPred  = p->PredClass ();
          MLClassPtr  lastKnown = p->KnownClass ();
          float  probTotal = 0.0;
          int    probStartIDX = i;
          int    probLastIDX  = i;
          float  probMin = FLT_MAX;
          float  probMax = FLT_MIN;
 
          while  ((p->PredClass () == lastPred)  &&  (p->KnownClass () == lastKnown)  &&  (i <  hl->QueueSize ()))
          {
            probLastIDX = i;
            float  prob = p->Probability ();
            probTotal += prob;

            if  (prob > probMax)  probMax = prob;
            if  (prob < probMin)  probMin = prob;

            i++;
            if  (i >= hl->QueueSize ())
              break;

            p = (PredictionHistoryBitPtr)hl->IdxToPtr (i);
          }
          
          int  c = (probLastIDX - probStartIDX) + 1;
          float  probMean = probTotal / (float)c;

          double totalDeltaSquare = 0.0;

          for  (int j = probStartIDX;  j <= probLastIDX;  j++)
          {
            PredictionHistoryBitPtr  q = (PredictionHistoryBitPtr)hl->IdxToPtr (j);
            float  probDelta = probMean - q->Probability ();
            totalDeltaSquare += probDelta * probDelta;
          }

          float probVar = (float)(totalDeltaSquare / (double)c);
          float probStdDev = sqrt (probVar);

          o << "//PREDICTION2" << "\t" << retraining 
                               << "\t" << randomPass 
                               << "\t" << lastKnown->Name () 
                               << "\t" << lastPred->Name () 
                               << "\t" << c
                               << "\t" << StrFormatDouble (probMean,   "-zz0.00000")
                               << "\t" << StrFormatDouble (probVar,    "-zz0.00000")
                               << "\t" << StrFormatDouble (probStdDev, "-zz0.00000")
                               << "\t" << StrFormatDouble (probMin,    "-zz0.00000")
                               << "\t" << StrFormatDouble (probMax,    "-zz0.00000")
                               << endl;
        }

        //PREDICTION2	672	7	Echino_645	0.00482	ria_Smallbell_Longarms	16	0.76040	0.03492	0.18686	0.41868	0.99180
        //PREDICTION2	000	0	Echino_Plutei	Cnidaria_Smallbell_Longarms	12	0.92093	0.01132	0.10638	0.71106	0.99998

        //for  (int i = 0;  i <  hl->QueueSize ();  i++)
        //{
        //  p = (PredictionHistoryBitPtr)hl->IdxToPtr (i);
        //
        //  if  ((p->PredClass () != lastPred)  ||  (p->KnownClass () != lastKnown))
        //  {
        //    lastPred  = p->PredClass ();
        //    lastKnown = p->KnownClass ();
        //
        //    o << "//PREDICTION" << "\t" << retraining 
        //                        << "\t" << randomPass 
        //                        << "\t" << lastKnown->Name () 
        //                        << "\t" << lastPred->Name () 
        //                        << endl;
        //  }
        //
        //  o << StrFormatDouble (p->Probability (), "-zz0.00000") << endl;
        //}
      }

      
      if  (probabilities[retraining][randomPass]->QueueSize () > 0)
      {
        o << "//PROBABILITY" << "\t" << retraining << "\t" << randomPass << endl;
        probabilities[retraining][randomPass]->WriteOutContents (o);
      }

      if  (sortMethods[retraining][randomPass]->QueueSize () > 0)
      {
        o << "//SORTMETHODS" << "\t" << retraining << "\t" << randomPass << endl;
        sortMethods[retraining][randomPass]->WriteOutContents (o);
      }
    }
  }


  o << "//SUPPORTVECTORS" << endl;
  for  (retraining = 0;  retraining < numOfRetrainings;  retraining++)
  {
    for  (randomPass = 0;  randomPass < numOfRandomPasses;  randomPass++)
    {
      int  nsv = totals[randomPass][retraining]->NumOfSupportVectors ();
      if  (nsv != 0)
      {
        float  trainingTime = totals[randomPass][retraining]->TrainingTime ();
        float  testingTime  = totals[randomPass][retraining]->TestingTime ();
        o << retraining << "\t" << randomPass 
                   << "\t" << nsv 
                   << "\t" << trainingTime 
                   << "\t" << testingTime 
                   << endl;
      }
    }
  }


  o << "///End_ClassTotals"  << endl;

  o.close ();
}  /* WriteOutHistory */





void  ClassGroupTotals::ReadInHistory (FILE*  in)
{
  char  buff[10240];

  HistoryType  curHistoryType = NULLhist;

  cout << "ClassGroupTotals::ReadInHistory  Starting" << endl;

  int     retraining = 0;
  int     randomPass  = 0;
  float   probability = 0.0;

  MLClassPtr  knownClass = NULL;
  MLClassPtr  predClass  = NULL;

  while  (fgets (buff, sizeof (buff), in))
  {
    KKStr  l (buff);

    if  (l.SubStrPart (0, 2) == "///")
    {
      KKStr  hl = l.SubStrPart (3);
      KKStr  cmd = hl.ExtractToken ();

      cmd.Upper ();

      if  (cmd == "DIMENSIONS")
      {
        int  _numOfretrainings = l.ExtractTokenInt ("\n\r\t ,");
        int  _numOfPasses      = l.ExtractTokenInt ("\n\r\t ,");
        int  _streamBlockSize  = l.ExtractTokenInt ("\n\r\t ,");
        if  (streamBlockSize < 1)
          streamBlockSize = _streamBlockSize;
      }

      else if  (cmd == "NEW_CLASSTOTALS")
      {
      }

      else if  (cmd == "SORTORDER")
      {
        KKStr  sortOrderStr = hl.ExtractToken ();
        sortOrder = SortOrderTypeFromStr (sortOrderStr);
      }

      cout << l;
    }

    else if  (l.SubStrPart (0, 1) == "//")
    {
      KKStr h = l.SubStrPart (2);

      KKStr  histTypeStr = h.ExtractToken ();

      if  (histTypeStr == "PREDICTION")
      {
        curHistoryType = PredictionHist;
      }

      else if  (histTypeStr == "PREDICTION2")
      {
        curHistoryType = PredictionHist2;
      }

      else if  (histTypeStr == "PROBABILITY")
      {
        curHistoryType = ProbabilityHist;
      }

      else if  (histTypeStr == "SORTMETHODS")
      {
        curHistoryType = SortMethodHist;
      }

      else if  (histTypeStr == "SUPPORTVECTORS")
      {
        curHistoryType = SupportVectorsHist;
      }

      else
      {
        cerr << endl;
        cerr << "**** ERROR ****   ClassGroupTotals::ReadInHistory" << endl;
        cerr << endl;
        cerr << "Invalid History Type[" << histTypeStr << "]." << endl;
        cerr << endl;
        osWaitForEnter ();
        exit (-1);
      }

      retraining = h.ExtractTokenInt (" ,\n\t\r");
      randomPass  = h.ExtractTokenInt (" ,\n\t\r");
      if  ((curHistoryType == PredictionHist)  ||  (curHistoryType == PredictionHist2))
      {
        KKStr  knownClassName = h.ExtractToken ();
        knownClass = mlClasses.LookUpByName (knownClassName);
        if  (!knownClass)
        {
          cerr << endl
               << endl
               << "*** ERROR ***   ClassGroupTotals::ReadInHistory" << endl
               << "                KnownClass[" << knownClassName << "]  not in list." << endl
               << endl;
          cerr << "Orig Line[" << buff << "]." << endl
               << endl;
          cerr << "FileName[" << fileName << "]." << endl;
          osWaitForEnter ();
          knownClass = MLClass::CreateNewMLClass (knownClassName);
          mlClasses.AddMLClass (knownClass);
        }

        KKStr  predClassName = h.ExtractToken ();
        predClass = mlClasses.LookUpByName (predClassName);
        if  (!predClass)
        {
          cerr << endl
               << endl
               << "*** ERROR ***   ClassGroupTotals::ReadInHistory" << endl
               << "                Predicted Class[" << predClassName << "]  not in list." << endl
               << endl;
          cerr << "Orig Line[" << buff << "]." << endl
               << endl
               << endl;;
          cerr << "FileName[" << fileName << "]." << endl;
          osWaitForEnter ();
          predClass = MLClass::CreateNewMLClass (predClassName);
          mlClasses.AddMLClass (predClass);
        }


        if  (curHistoryType == PredictionHist2)
        {
          int    c           = h.ExtractTokenInt    (" ,\n\t\r");
          float  probMean    = (float)h.ExtractTokenDouble (" ,\n\t\r");
          float  probVar     = (float)h.ExtractTokenDouble (" ,\n\t\r");
          float  probStdDev  = (float)h.ExtractTokenDouble (" ,\n\t\r");
          float  probMin     = (float)h.ExtractTokenDouble (" ,\n\t\r");
          float  probMax     = (float)h.ExtractTokenDouble (" ,\n\t\r");

          for  (int i = 0;  i < c;  i++)
            Prediction (retraining, randomPass, knownClass, predClass, probMean);
        }
      }
    }

    else
    {

      HistoryBitPtr h = NULL;
      switch  (curHistoryType)  
      {
        case PredictionHist:     probability = (float)l.ExtractTokenDouble (" ,\n\t\r");
                                 h = new PredictionHistoryBit (knownClass, predClass, probability);
                                 break;

        case ProbabilityHist:    h = new ProbabilityHistoryBit    (mlClasses, l);
                                 break;

        
        case SortMethodHist:     h = new SortMethodHistoryBit     (mlClasses, l);
                                 break;


        case SupportVectorsHist: h = new SupportVectorsHistoryBit (l);
                                 break;


        default: 
            cerr << endl;
            cerr << "**** ERROR ****   ClassGroupTotals::ReadInHistory" << endl;
            cerr << endl;
            cerr << "No History Type Defined" << endl;
            cerr << endl;
            osWaitForEnter ();
            exit (-1);
      }  /* end of switch */

      h->UpdateClassTotals (this, retraining, randomPass);
      delete h;
      h = NULL;
    }
  }

  cout << "ClassGroupTotals::ReadInHistory  Exiting" << endl;
}  /* ReadInHistory */




void   ClassGroupTotals::GetClassGroupTotalsFileStats (const KKStr&       fileName,
                                                       MLClassList&     globalClassList,
                                                       RunLog&             log,
                                                       MLClassListPtr&  classesInFile,
                                                       SortOrderType&      sortOrder,
                                                       int&                numOfRetrainings,
                                                       int&                numOfRandomPasses,
                                                       int&                streamBlockSize
                                                      )
{
  classesInFile     = NULL;
  sortOrder         = NULL_SortOrder;
  numOfRetrainings  = -1;
  numOfRandomPasses = -1;
  streamBlockSize   = -1;


  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << endl
                   << "ClassGroupTotals::GetClassGroupTotalsFileStats       Error Opening File[" << fileName << "]." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  char   buff[10240];
  char*  buffPtr;

  buffPtr = fgets (buff, sizeof (buff), in);
  while  (buffPtr)  
  {
    KKStr  l (buff);
    KKStr  lUpper (l);
    lUpper.Upper ();

    if  (lUpper.SubStrPart (0, 2) != "///")
      break;

    KKStr  s = lUpper.SubStrPart (3);
    KKStr parmName = s.ExtractToken ("\n\r\t");
    parmName.Upper ();

    if  (parmName == "DIMENSIONS")
    {
      numOfRetrainings  = s.ExtractTokenInt (" ,\t\n\r");
      numOfRandomPasses = s.ExtractTokenInt (" ,\t\n\r");
      streamBlockSize   = s.ExtractTokenInt (" ,\t\n\r");
    }

    if  (parmName == "SORTORDER")
    {
      KKStr  sortOrderDesc = s.ExtractToken ("\n\r\t");
      sortOrder = SortOrderTypeFromStr (sortOrderDesc);
    }

    else  if  (parmName == "NEW_CLASSTOTALS")
    {
      classesInFile = new MLClassList ();
      KKStr  s = l.SubStrPart (18);

      KKStr  className = s.ExtractToken ("\n\r\t");
      while  (!className.Empty ())
      {
        MLClassPtr  mlClass = globalClassList.GetMLClassPtr (className);  // See if instance caller

        // Add pointer to mlClass to ClassGroupTotals own list.
        classesInFile->AddMLClass (mlClass);
        className = s.ExtractToken ("\n\r\t");
      }
    }

    fgets (buff, sizeof (buff), in);
  }

  fclose (in);
  return;
}  /* GetClassGroupTotalsFileStats */









void  ClassGroupTotals::Extract_IIPC_IPR_FromDirName (const KKStr&    dirName,
                                                      int&             iipc,
                                                      int&             ipr
                                                     )
{
  int x = dirName.LocateLastOccurrence ('_');

  iipc = -1;
  ipr  = -1;

  if  (x <= 5)
    return;

  KKStr leftSide  = dirName.SubStrPart (0, x - 1);
  KKStr rightSide = dirName.SubStrPart (x + 1);

  x = rightSide.LocateCharacter ('-');
  KKStr  IPR = rightSide.SubStrPart (0, x - 1);
  ipr = atoi (IPR.Str ());

  // Now lets determine initialImagesPerClass
  x = leftSide.Len () - 1;
  //  We will scan from the end of the string until we find a Ubderscore(_)
  //  or path seperator (\).

  while  (x >= 0)
  {
    if  ((leftSide[x] == '_')  ||  (leftSide[x] == DSchar))
      break;
    x--;
  }

  if (x >= 0)
  {   
    rightSide = leftSide.SubStrPart (x + 1);
    leftSide  = leftSide.SubStrPart (0, x - 1);
  }
  else
  {
    rightSide = leftSide;
    leftSide = "";
  }
  x = rightSide.LocateCharacter ('-');
  if  (x >= 0)
  {
    KKStr  IIPC = rightSide.SubStrPart (0, x - 1);
    iipc = atoi (IIPC.Str ());
  }
}  /* Extract_IIPC_IPR_FromDirName */




ClassGroupTotalsPtr  ClassGroupTotals::ConsolidateOneDirectoryOneSortOrder (const KKStr&    dirName,
                                                                            MLClassList&  globalClassList,
                                                                            SortOrderType    sortOrder,
                                                                            RunLog&          log
                                                                           )
{
  ClassGroupTotalsPtr  totals = NULL;

  log.Level (10) << "ClassGroupTotals::ConsolidateOneDirectoryOneSortOrder["
                 << SortOrderDescr (sortOrder) 
                 << "]   DirName["  << dirName << "]."  
                 << endl;


  int  iipc = -1;
  int  ipr  = -1;
  
  Extract_IIPC_IPR_FromDirName (dirName, iipc, ipr);

  KKStr  fileSpec (dirName);
  osAddLastSlash (fileSpec);
  fileSpec << "*.*";

  KKStrListPtr   dataFiles = osGetListOfFiles (fileSpec);

  if  (!dataFiles)
    return  NULL;

  dataFiles->Sort (false);
  KKStrPtr  fileName = NULL;

  StringListIterator  fnIDX (*dataFiles);

  KKStr  sortOrderDesc = SortOrderDescr (sortOrder);

  for  (fnIDX.Reset ();  fileName = fnIDX.CurPtr ();  ++fnIDX)
  {
    if  (!(fileName->StrInStr (sortOrderDesc)))
      continue;

    KKStr  fullFileName (dirName);
    osAddLastSlash (fullFileName);
    fullFileName << *fileName;

    if  (!osFileExists (fullFileName))
      continue;

    log.Level (10) << "TryLoadingResultsForASpecificSortOrder  Loading File[" << *fileName << "]." << endl;

    SortOrderType     fileSortOrder         = NULL_SortOrder;
    int               fileNumOfRetrainings  = -1;
    int               fileNonOfRandomPasses = -1;
    MLClassListPtr    fileMLClassList       = NULL;
    int               fileStreamBlockSize   = -1;

    GetClassGroupTotalsFileStats (fullFileName,
                                  globalClassList,
                                  log,
                                  fileMLClassList,
                                  fileSortOrder,
                                  fileNumOfRetrainings,
                                  fileNonOfRandomPasses,
                                  fileStreamBlockSize
                                 );

    if  (!fileMLClassList)
      continue;

    if  (fileSortOrder == NULL_SortOrder)
      continue;

    if  (!totals)
    {
      log.Level (10) << "Creating new 'ClassGroupTotals' object." << endl;
      totals = new ClassGroupTotals (fullFileName, 
                                     *fileMLClassList,
                                     iipc,
                                     ipr,
                                     log
                                    );
    }
    else
    {
      log.Level (10) << "Including in Existing in 'ClassGroupTotals' object." << endl;
      bool successful = false;
      totals->ReadInDataFile (fullFileName, successful);
      if  (!successful)
      {
        log.Level (-1) << endl
                       << endl
                       << "SummaryByIPCandIPR::TryLoadingResultsForASpecificSortOrder     *** ERROR ***" << endl
                       << endl
                       << "Could not Read ClassGroupTotals[" << fullFileName << "]" << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }
    }
  }
  
  if  (!totals)
  {
    log.Level (10) << "TryLoadingResultsForASpecificSortOrder, no Totals found for [" 
                   <<  SortOrderDescr (sortOrder) 
                   << "]." 
                   << endl;
    return NULL;
  }

  log.Level (10) << "SummaryByIPCandIPR::TryLoadingResultsForASpecificSortOrder["
                 << SortOrderDescr (sortOrder) 
                 << "]  Done Loading"  
                 << endl;

  return  totals;
}  /* ConsolidateOneDirectoryOneSortOrder */






ClassGroupTotalsList::ClassGroupTotalsList ():
    KKQueue<ClassGroupTotals> (true, 10)
{
}



    
ClassGroupTotalsPtr  ClassGroupTotalsList::LocateBySortOrder (SortOrderType  sortOrder)  const
{
  ClassGroupTotalsPtr  totals = NULL;

  for  (int x = 0;  totals = IdxToPtr (x);  x++)
  {
    if  (totals->SortOrder () == sortOrder)
      return  totals;
  }

  return  NULL;
}  /* LocateBySortOrder */






