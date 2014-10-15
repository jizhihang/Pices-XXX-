#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <math.h>

#include  <iostream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "FeatureStats.h"


#include  "FileDesc.h"
#include  "ResultLine.h"
#include  "OSservices.h"




class  OneFeatureStat
{
public:
  OneFeatureStat (): 
      accuracyDelta       (0.0f),
      accuracyDeltaCount  (0),
      accuracyDeltaVar    (0.0f),
      accuracyDeltaStdDev (0.0f)
   {}

  float  accuracyDelta;
  uint   accuracyDeltaCount;
  float  accuracyDeltaVar;
  float  accuracyDeltaStdDev;

};  /* OneFeatureStat */






FeatureStats::FeatureStats (FileDescPtr      _fileDesc,
                            ResultLineTree&  _results,
                            RunLog&          _log
                           ):
  fileDesc      (_fileDesc),
  log           (_log),
  numOfFeatures (0),
  stats         (NULL)
{
  numOfFeatures = _fileDesc->NumOfFields ();
  stats = new OneFeatureStat[numOfFeatures];

  ResultLinePtr  result = _results.GetFirst ();
  while  (result)
  {
    AddToAccumulators (*result);
    result = _results.GetNext ();
  }

  CalculateMeans ();

  result = _results.GetFirst ();
  while  (result)
  {
    AddToVariances (*result);
    result = _results.GetNext ();
  }
  
  CalculateVariances ();
}





FeatureStats::FeatureStats (FileDescPtr      _fileDesc,
                            ResultLineList&  _results,
                            RunLog&          _log
                           ):
  fileDesc      (_fileDesc),
  log           (_log),
  numOfFeatures (0),
  stats         (NULL)
{
  numOfFeatures = _fileDesc->NumOfFields ();
  stats = new OneFeatureStat[numOfFeatures];

  ResultLineList::iterator  idx;

  for  (idx = _results.begin ();  idx != _results.end ();  idx++)
    AddToAccumulators (**idx);

  CalculateMeans ();

  for  (idx = _results.begin ();  idx != _results.end ();  idx++)
    AddToVariances (**idx);

  CalculateVariances ();
}




FeatureStats::~FeatureStats ()
{
  delete[] stats;
}



void  FeatureStats::AddToAccumulators (ResultLine&  result)
{
  if  (!result.Parent1 ())
    return;

  if  (result.Parent2 ())
  {
    // Looks like a joining of two results, 
    // notr interestied in these results.
    return;
  }

  float  accuracy = result.Accuracy ();
  int    x;

  FeatureNumList&  resultFeatures = *(result.Features ());

  float  accuracyDelta = result.Parent1 ()->Accuracy () - accuracy;
  FeatureNumList featuresThatChanged = *(result.Parent1 ()->Features ()) - resultFeatures;

  for  (x = 0;  x < featuresThatChanged.NumOfFeatures ();  x++)
  {
    int  featureNum = featuresThatChanged[x];
    OneFeatureStat& stat = stats[featureNum];
    stat.accuracyDeltaCount++;
    stat.accuracyDelta += accuracyDelta;
  }
}  /* AddToAccumulators */




void  FeatureStats::CalculateMeans ()
{
  int  x;

  for  (x = 0;  x < numOfFeatures;  x++)
  {
    OneFeatureStat& stat = stats[x];
    if  (stat.accuracyDeltaCount > 0)
      stat.accuracyDelta = stat.accuracyDelta / stat.accuracyDeltaCount;
  }
}  /* CalculateMeans */
 



void  FeatureStats::AddToVariances (ResultLine&  result)
{
  if  (!result.Parent1 ())
    return;

  if  (result.Parent2 ())
  {
    // Looks like a joining of two results, 
    // notr interestied in these results.
    return;
  }

  float  accuracy = result.Accuracy ();
  float  accuracyDelta = result.Parent1 ()->Accuracy () - accuracy;
  int    x;

  FeatureNumList&  resultFeatures = *(result.Features ());

  FeatureNumList featuresThatChanged = *(result.Parent1 ()->Features ()) - resultFeatures;

  for  (x = 0;  x < featuresThatChanged.NumOfFeatures ();  x++)
  {
    int  featureNum = featuresThatChanged[x];
    OneFeatureStat& stat = stats[featureNum];
    float delta = accuracyDelta - stat.accuracyDelta;
    stat.accuracyDeltaVar += delta * delta;
  }
}  /* AddToVariances */




void  FeatureStats::CalculateVariances ()
{
  int  x;

  for  (x = 0;  x < numOfFeatures;  x++)
  {
    OneFeatureStat& stat = stats[x];
    if  (stat.accuracyDeltaCount > 0)
    {
      stat.accuracyDeltaVar    = stat.accuracyDeltaVar / stat.accuracyDeltaCount;
      stat.accuracyDeltaStdDev = sqrt (stat.accuracyDeltaVar);
    }
  }
}  /* CalculateVariances */



FeatureNumList  FeatureStats::RemoveHarmfulFeatures (FeatureNumList&  features)
{
  FeatureNumList  result (fileDesc);

  int  x;
  for  (x = 0;  x < features.NumOfFeatures ();  x++)
  {
    int featureNum = features[x];
    if  (featureNum >= numOfFeatures)
    {
      // Featur specifued is larger than the number of features that this
      // object was built for.
      log.Level (-1) << endl
                     << "FeatureStats::RemoveHarmfulFeatures    *** ERROR ***" << endl
                     << "                      featureNum[" << featureNum << "] greater than numOfFeaturs[" << numOfFeatures << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }

    OneFeatureStat& stat = stats[featureNum];
    if  (((stat.accuracyDelta + stat.accuracyDeltaStdDev) > 0.0f)  ||  (stat.accuracyDeltaCount < 3))
      result += featureNum;
    else
      cout << endl 
           << "stat.accuracyDelta      [" << stat.accuracyDelta       << "]" << endl
           << "stat.accuracyDeltaStdDev[" << stat.accuracyDeltaStdDev << "]" << endl
           << endl;
  }

  return  result;
}  /* RemoveHarmfulFeatures */




void   FeatureStats::ValidateFeatureNum (const char*  funcName,
                                         int          featureNum
                                        )
{
  if  ((featureNum < 0)  ||  (featureNum >= numOfFeatures))
  {
    log.Level (-1) << endl
                   << "FeatureStats::" << funcName << "   *** ERROR ***"  << endl
                   << "                       Invalid FeatureNum[" << featureNum << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

}  /* ValidateFeatureNum */




float  FeatureStats::AccuracyDeltaMean (int featureNum)
{
  ValidateFeatureNum ("AccuracyDeltaMean", featureNum);
  return  stats[featureNum].accuracyDelta;
}  /* PrintStats */



float  FeatureStats::AccuracyDeltaStdDev (int featureNum)
{
  ValidateFeatureNum ("AccuracyDeltaStdDev", featureNum);
  return  stats[featureNum].accuracyDeltaStdDev;
}  /* PrintStats */



float  FeatureStats::AccuracyDeltaCount (int featureNum)
{
  ValidateFeatureNum ("AccuracyDeltaCount", featureNum);
  return  float (stats[featureNum].accuracyDeltaCount);
}  /* PrintStats */



