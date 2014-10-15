#ifndef  _FEATURESTATS_
#define  _FEATURESTATS_

#include  "FileDesc.h"
#include  "ResultLine.h"
#include  "RunLog.h"


class  OneFeatureStat;
typedef  OneFeatureStat*  OneFeatureStatPtr;


class FeatureStats
{
public:
  FeatureStats (FileDescPtr      _fileDesc,
                ResultLineTree&  _results,
                RunLog&          _log
               );

  FeatureStats (FileDescPtr      _fileDesc,
                ResultLineList&  _results,
                RunLog&          _log
               );

  ~FeatureStats ();

  float  AccuracyDeltaCount  (int featureNum);

  float  AccuracyDeltaMean   (int featureNum);

  float  AccuracyDeltaStdDev (int featureNum);

  int    NumOfFeatures ()  const  {return numOfFeatures;}

  FeatureNumList  RemoveHarmfulFeatures (FeatureNumList&  features);



private:
  //**********************************************************************
  //*  The following 4 methods are used to calculate statistics for      *
  //*  each feature.                                                     *
  //*                                                                    *
  //*  AddToAccumulators   - Call this for each ResultLinem, Totals up   *
  //*                        accuracy change for each feature.           *
  //*                                                                    *
  //*  CalculateMeans      - After you have called AddToAccumulatots you *
  //*                        call this method to get means.              *
  //*                                                                    *
  //*  AddToVariances      - After means have been calculated, call this *
  //*                        method for each ResultLine.                 *
  //*                                                                    *
  //*  CalculateVariances  - Last thing is to call this methiod to calc  *
  //*                        Variances and StdDev for each feature.      *
  //**********************************************************************
  void  AddToAccumulators (ResultLine&  result); 

  void  AddToVariances (ResultLine&  result);

  void  CalculateMeans ();

  void  CalculateVariances ();
  //**********************************************************************


  void   ValidateFeatureNum (const char*  funcName,
                             int          featureNum
                            );


  FileDescPtr        fileDesc;
  int                numOfFeatures;
  RunLog&            log;
  OneFeatureStatPtr  stats;
};  /* FeatureStats */


               
#endif
