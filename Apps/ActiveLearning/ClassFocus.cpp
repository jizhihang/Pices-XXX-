#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory>
#include  <math.h>
#include  <limits.h>
#include  <float.h>

#include  <iostream>
#include  <string>
#include  <vector>
#include  <algorithm>
#include  <functional>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include  "ClassFocus.h"

#include  "OSservices.h"
#include  "TrainingImage.h"





ClassFocus::ClassFocus (int _numOfClasses):

   counts              (new uint[_numOfClasses]),
   numOfClasses        (_numOfClasses),
   numOfSupportVectors (0),
   predicted           (new uint*[_numOfClasses]),
   trainingImages      (true, 5),
   probabilities       (NULL),
//   imageFileNames      (NULL),
//   nonStreamPlaces     (NULL),
//   percentiles         (NULL),
   sortMethods         (new SortOrderType[_numOfClasses])
{
  int knownIDX;
  int predIDX;

  probabilities   = new VectorFloat*  [_numOfClasses];
//  imageFileNames  = new VectorKKStr;
//  nonStreamPlaces = new VectorInt;
//  percentiles     = new VectorFloat;

  for  (knownIDX = 0;  knownIDX < numOfClasses;  knownIDX++)
  {
    predicted       [knownIDX] = new uint[numOfClasses];
    sortMethods     [knownIDX] = NULL_SortOrder;
    counts          [knownIDX] = 0;
    probabilities   [knownIDX] = new VectorFloat;

    for  (predIDX = 0;  predIDX < numOfClasses;  predIDX++)
    {
      predicted [knownIDX][predIDX] = 0;
    }
  }
}




ClassFocus::~ClassFocus ()
{
  for  (int classIDX = 0;  classIDX < numOfClasses;  classIDX++)
  {
    delete  predicted[classIDX];
    predicted[classIDX] = NULL;
    delete  probabilities   [classIDX];
  }

  delete  predicted;
  delete  counts;
  delete  sortMethods;
  delete  probabilities;
//  delete  imageFileNames;
//  delete  nonStreamPlaces;
//  delete  percentiles;
}




void  ClassFocus::NumOfSupportVectors (int    _numOfSupportVectors,
                                       float  _trainingTime,
                                       float  _testingTime
                                      )
{
  numOfSupportVectors = _numOfSupportVectors;
  trainingTime        = _trainingTime;
  testingTime         = _testingTime;
}




void  ClassFocus::ValidateClassIDX (KKStr fn, int  classIDX)
{
  if  ((classIDX < 0)  ||  (classIDX >= numOfClasses))
  {
    cerr << endl;
    cerr << "**** ERROR ****   " << fn << "IDX[" << classIDX << "] out of Range(0.." << numOfClasses << ")." << endl;
    cerr << endl;
    osWaitForEnter ();
    exit (-1);
  }
}



uint  ClassFocus::Count ()
{
  uint  count = 0;

  for  (int classIDX = 0;  classIDX < numOfClasses;  classIDX++)
  {
    count += counts[classIDX];
  }

  return  count;
}




uint  ClassFocus::Count (int  classIDX)
{
  ValidateClassIDX ("Count", classIDX);
  return  counts[classIDX];
}




uint  ClassFocus::Correct ()
{
  uint  correct = 0;

  for  (int classIDX = 0;  classIDX < numOfClasses;  classIDX++)
  {
    correct += predicted[classIDX][classIDX];
  }

  return  correct;
}




uint   ClassFocus::NumOfProbabilities ()      
{
  uint  numOfProbabilities = 0;

  for  (int idx = 0;  idx < numOfClasses;  idx++)
  {
    numOfProbabilities += (uint)(probabilities[idx]->size ());
  }

  return  numOfProbabilities;
}



uint   ClassFocus::NumOfProbabilities (int classIDX)
{
  ValidateClassIDX ("NumOfProbabilities", classIDX);
  return  (uint)(probabilities[classIDX]->size ());
}



float  ClassFocus::Probabilities (int  classIDX,
                                  uint idx
                                 )
{
  ValidateClassIDX ("Probabilities", classIDX);

  if  (idx >= probabilities[classIDX]->size ())
  {
    cerr << endl;
    cerr << "**** ERROR ****   Probabilities IDX[" << idx << "] out of Range(0.." << (int)probabilities[classIDX]->size () << ")." << endl;
    cerr << endl;
    osWaitForEnter ();
    exit (-1);
  }

  return  (*probabilities[classIDX])[idx];
}




void  ClassFocus::AddProbability (int            _classIDX,
                                  MLClassPtr  _mlClass,
                                  const KKStr&  _imageFileName,
                                  float          _probability,
                                  int            _nonStreamPlace,
                                  float          _percentile
                                 )
{
  ValidateClassIDX ("AddProbability", _classIDX);
  probabilities  [_classIDX]->push_back (_probability);
  trainingImages.PushOnBack (new TrainingImage (_imageFileName, 
                                                _mlClass,
                                                _probability, 
                                                _nonStreamPlace, 
                                                _percentile
                                               )
                            );
}



int  ClassFocus::GetNumNewImagesInClass (int _classIDX)
{
  ValidateClassIDX ("GetNumNewImagesInClass", _classIDX);
  return  (int)probabilities[_classIDX]->size ();
}



SortOrderType  ClassFocus::SortMethodUsed   (int classIDX)
{
  ValidateClassIDX ("SetSortMethodUsed",  classIDX);
  return  sortMethods[classIDX];
}




void  ClassFocus::SetSortMethodUsed   (int            classIDX, 
                                       SortOrderType  sortMethod
                                      )
{
  ValidateClassIDX ("SetSortMethodUsed",  classIDX);
  sortMethods[classIDX] = sortMethod;
}




void  ClassFocus::Prediction (int    knownIDX,
                              int    predIDX,
                              float  probability
                             )
{
  counts[knownIDX]++;
  predicted[knownIDX][predIDX]++;
}



void  ClassFocus::AddIn (ClassFocus&  cf)
{
  uint  x;

  for  (int knownIDX = 0;  knownIDX < numOfClasses;  knownIDX++)
  {
    counts[knownIDX] += cf.counts[knownIDX];

    for  (int predIDX = 0;  predIDX < numOfClasses;  predIDX++)
    {
      predicted [knownIDX][predIDX] += cf.predicted [knownIDX][predIDX];
    }

    for  (x = 0; x < cf.probabilities[knownIDX]->size ();  x++)
    {
      probabilities[knownIDX]->push_back ((*cf.probabilities[knownIDX])[x]);
    }
  }

  numOfSupportVectors += cf.numOfSupportVectors;

  for  (TrainingImageList::iterator i = cf.trainingImages.begin ();  i != cf.trainingImages.end ();  i++)
  {
    trainingImages.PushOnBack (new TrainingImage (**i));
  }
}  /* AddIn */





void   ClassFocus::CalcAvgProb (int     classIDX, 
                                uint&   numOfProbs,
                                float&  avgProbability
                               )
{
  ValidateClassIDX ("CalcAvgProb", classIDX);

  numOfProbs = 0;
  avgProbability = (float)0.0;

  VectorFloat*  p  = probabilities[classIDX];

  for  (uint idx = 0;  idx < p->size ();  idx++)
  {
    avgProbability += (*p)[idx];
    numOfProbs++;
  }

  if  (numOfProbs > 0)
    avgProbability = avgProbability / numOfProbs;

  return;
}  /* CalcAvgProb */
