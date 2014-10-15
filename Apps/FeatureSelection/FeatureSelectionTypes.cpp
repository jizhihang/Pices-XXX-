#include "FirstIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "MemoryDebug.h"

using namespace std;


#include <sys/types.h>
#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif


#include "BasicTypes.h"
#include "StatisticalFunctions.h"
#include "Str.h"
using namespace KKU;

#include "MLClass.h"
using namespace MLL;


#include "FeatureSelectionTypes.h"



poProcessingOrder  ProcessingOrderFromStr (const KKStr&  procOrderStr)
{
  if  (procOrderStr.EqualIgnoreCase ("StandardOrder")  ||  procOrderStr.EqualIgnoreCase ("Standard"))
    return  poStandard;

  if  (procOrderStr.EqualIgnoreCase ("HallOrder")      ||   procOrderStr.EqualIgnoreCase ("Hall"))
    return  poHall;

  return  poNULL;
}



KKStr  ProcessingOrderToStr (poProcessingOrder  order)
{
  if  (order == poStandard)
    return "StandardOrder";

  if  (order == poHall)
    return "HallOrder";

  return "";
}




MajorSteps  MajorStepFromStr (KKStr  majorStepStr)
{
  majorStepStr.Upper ();

  if  (majorStepStr.EqualIgnoreCase ("ALLCLASSESPARMTUNING")  ||  majorStepStr.EqualIgnoreCase ("MfsParmTuningPre"))
    return  msMfsParmTuningPre;

  if  (majorStepStr.EqualIgnoreCase ("ALLCLASSESFEATURESELECTION")  ||  majorStepStr.EqualIgnoreCase ("MfsFeatureSelection"))
    return  msMfsFeatureSelection;

  if  (majorStepStr.EqualIgnoreCase ("MfsParmTuningPost"))
    return msMfsParmTuningPost;

  if  (majorStepStr.EqualIgnoreCase ("BINARYCLASSESFEATURESELECTION")  ||  majorStepStr.EqualIgnoreCase ("BfsFeatureSelection"))
    return  msBfsFeatureSelection;

  if  (majorStepStr.EqualIgnoreCase ("BfsParmTuningPre"))
    return msBfsParmTuningPre;

  if  ((majorStepStr == "BINARYCLASSESPARMTUNING")  ||  majorStepStr.EqualIgnoreCase ("BfsParmTuningPost"))
    return  msBfsParmTuningPost;

  if  ((majorStepStr == "GENERATEFINALERESULTS")  ||  (majorStepStr == "GENERATEFINALRESULTS"))
    return  msGenerateFinalResults;

  if  ((majorStepStr == "FINALEREPORT")  ||  (majorStepStr == "FINALREPORT"))
    return  msFinalReport;

  if  (majorStepStr.EqualIgnoreCase ("GenerateBruitSvmSearch"))
    return  msGenerateBruitSvmSearch;

  if  (majorStepStr == "ALLDONE")
    return  msDone;

  return  msNULL;
}  /* MajorStepFromStr */




KKStr  MajorStepToStr (MajorSteps  step)
{
  if  (step == msMfsParmTuningPre)
    return  "MfsParmTuningPre";

  if  (step == msMfsFeatureSelection)
    return  "MfsFeatureSelection";

  if  (step == msMfsParmTuningPost)
    return "MfsParmTuningPost";

  if  (step == msBfsFeatureSelection)
    return  "BfsFeatureSelection";

  if  (step == msBfsParmTuningPre)
    return  "BfsParmTuningPre";

  if  (step == msBfsParmTuningPost)
    return  "BfsParmTuningPost";

  if  (step == msGenerateFinalResults)
    return  "GenerateFinalResults";

  if  (step == msGenerateBruitSvmSearch)
    return  "GenerateBruitSvmSearch";

  if  (step == msFinalReport)
    return  "FinalReport";

  if  (step == msDone)
    return  "AllDone";

  return  "NULL";
}  /* MajorStepFromStr */



FinalResultType  FinalResultTypeFromStr (const KKStr&  s)
{
  if  (s.EqualIgnoreCase ("NoTuningAllFeatures"))
    return frtNoTuningAllFeatures;

  if  (s.EqualIgnoreCase ("MfsFeaturesSel"))
    return frtMfsFeaturesSel;

  if  (s.EqualIgnoreCase ("FeaturesAllParmsTuned")  ||  s.EqualIgnoreCase ("MfsParmsTuned"))
    return frtMfsParmsTuned;

  if  (s.EqualIgnoreCase ("FeaturesSelParmsTuned")  ||  s.EqualIgnoreCase ("MfsParmsTunedFeaturesSel"))
    return frtMfsParmsTunedFeaturesSel;

  if  (s.EqualIgnoreCase ("BinaryFeaturesSel")  ||   s.EqualIgnoreCase ("BfsFeaturesSel"))
    return frtBfsFeaturesSel;

  if  (s.EqualIgnoreCase ("BfsParmsTuned"))
    return frtBfsParmsTuned;

  if  (s.EqualIgnoreCase ("BinaryFeaturesSelParmsTuned")  ||  s.EqualIgnoreCase ("BfsFeaturesSelParmsTuned"))
    return frtBfsFeaturesSelParmsTuned;

  return  frtNULL;
}



KKStr  FinalResultTypeToStr (FinalResultType  frt)
{
  switch  (frt)
  {
  case  frtNULL:                        return "";                             break;
  case  frtNoTuningAllFeatures:         return "NoTuningAllFeatures";          break;
  case  frtMfsFeaturesSel:              return "MfsFeaturesSel";               break;
  case  frtMfsParmsTuned:               return "MfsParmsTuned";                break;
  case  frtMfsParmsTunedFeaturesSel:    return "MfsParmsTunedFeaturesSel";     break;
  case  frtBfsFeaturesSel:              return "BfsFeaturesSel";               break;
  case  frtBfsParmsTuned:               return "BfsParmsTuned";                break;
  case  frtBfsFeaturesSelParmsTuned:    return "BfsFeaturesSelParmsTuned";     break;
  }
  return "";
}



stSearchTypes  SearchTypeFromStr (KKStr  searchTypeStr)
{
  searchTypeStr.Upper ();

  if  ((searchTypeStr == "PARM")       ||
       (searchTypeStr == "PARAM")      ||
       (searchTypeStr == "PARAMETER")  ||
       (searchTypeStr == "P")
      )
    return  stParameterSearch;

  if  ((searchTypeStr == "FEATURE")   ||
       (searchTypeStr == "F")
      )
    return  stFeatureSelectionSearch;

  if  ((searchTypeStr == "FINALEREPORTS")                         ||
       (searchTypeStr == "GENERATINGFINALEREPORTS")               ||
       (searchTypeStr == "FINALEREPORTS")                         ||
       (searchTypeStr.EqualIgnoreCase ("GenerateFinaleResults"))  ||
       (searchTypeStr.EqualIgnoreCase ("GenerateFinalResults"))
      )
    return  stGeneratingFinalResults;

  return stNULL;
}  /* SerachTypeFromStr */




KKStr  SearchTypeToStr (stSearchTypes  _searchType)
{
  if  (_searchType == stFeatureSelectionSearch)
    return  "Feature";

  if  (_searchType == stParameterSearch)
    return  "Parameter";

  if  (_searchType == stGeneratingFinalResults)
    return  "GenerateFinalResults";

  return  "NULL";
}  /* SearchTypeToStr */




KKStr  GradingMethodToStr (GradingMethodType  gm)
{
  if  (gm == gmAccuracy)
    return  "gmAccuracy";
  
  else if  (gm == gmAccuracyNorm)
    return  "gmAccuracyNorm";
  
  else if  (gm == gmFMeasure)
    return  "gmFMeasure";

  else 
    return  "gmNULL";
}  /* GradingMethodFromStr */




GradingMethodType  GradingMethodFromStr (const KKStr&  str)
{
  if  ((str.CompareIgnoreCase ("gmAccuracy") == 0)  ||  (str.CompareIgnoreCase ("Accuracy") == 0)  ||  (str.CompareIgnoreCase ("AccuracyMethod") == 0))
    return  gmAccuracy;

  if  ((str.CompareIgnoreCase ("gmAccuracyNorm") == 0)  ||  (str.CompareIgnoreCase ("AccuracyNorm") == 0)  ||  (str.CompareIgnoreCase ("AccuracyNormMethod") == 0))
    return  gmAccuracyNorm;

  if  ((str.CompareIgnoreCase ("F-Measure") == 0)  ||  (str.CompareIgnoreCase ("gmFMeasure") == 0)  ||  (str.CompareIgnoreCase ("FMeasure") == 0))
    return  gmFMeasure;

  return gmNULL;
}  /* GradingMethodFromStr */







KKStr  JobSelectionCriteriaToStr (JobSelectionCriteriaType  jsc)
{
  switch  (jsc)
  {
    case  jscMostAccurate:     return  "MostAccurate";
    case  jscFastestFromBest:  return  "FastestFromBest";
    case  jscBruitForce:       return  "BruitForce";
  }

  return  "NULL";
}




JobSelectionCriteriaType  JobSelectionCriteriaFromStr (const KKStr&  str)
{
  KKStr  s = str.ToUpper ();
  if  ((s == "MOSTACCURATE")  ||  (s == "MA"))
    return  jscMostAccurate;

  if  ((s == "FASTESTFROMBEST")  ||  (s == "FFB"))
    return  jscFastestFromBest;

  if  ((s == "BRUITFORCE")       ||  (s == "BF"))
    return  jscBruitForce;

  return  jscNULL;
}










//*************************************************************************
//*    Responsable for keep track of which set of Binmary Classes a given *
//*  Processor is responsable for.                                        *
//*                                                                       *
//*************************************************************************
KKStr  SearchMethodToStr (smSearchMethod  searchMethod)
{
  if  (searchMethod == smBestCaseNext)
    return "BestCaseNext";

  if  (searchMethod == smBeam)
    return "Beam";

  if  (searchMethod == smTestResults)
    return  "TestResults";

  if  (searchMethod == smPickTheBest)
    return  "PickTheBest";

  if  (searchMethod == smGrid)
    return "Grid";

  if  (smOnePassOnly == smOnePassOnly)
    return "OnePassOnly";

  return "";
}  /* SearchMethodToStr */





smSearchMethod  SearchMethodFromStr (KKStr  searchMethodStr)
{
  searchMethodStr.Upper ();

  if  (searchMethodStr      == "BESTCASENEXT")
    return smBestCaseNext;

  else if  (searchMethodStr == "BEAM")
    return smBeam;

  else if  (searchMethodStr == "TESTRESULTS")
    return smTestResults;

  else if  (searchMethodStr == "PICKTHEBEST")
    return smPickTheBest;

  else if  (searchMethodStr == "GRID")
    return smGrid;

  else if  (searchMethodStr == "ONEPASSONLY")
    return smOnePassOnly;

  else
    return smNULL;
}  /* SearchMethodToStr */





KKStr  ProcessorStatusToStr (fsProcessorStatus  processorStatus)
{
  if  (processorStatus == fsNotStarted)
    return "NotStarted";

  if  (processorStatus == fsStarted)
    return "Started";

  if  (processorStatus == fsDone)
    return "Done";

  return "";
}  /* ProcessorStatusToStr */





fsProcessorStatus  ProcessorStatusFromStr (const KKStr&  processorStatusStr)
{
  if  (processorStatusStr.EqualIgnoreCase ("NOTSTARTED"))
    return  fsNotStarted;

  if  (processorStatusStr.EqualIgnoreCase ("STARTED"))
    return fsStarted;
  
  if  (processorStatusStr.EqualIgnoreCase ("DONE"))
    return fsDone;

  return  fsNULL;
}  /* ProcessorStatusFromStr */



KKStr  FeatureCriteriaToStr (fcFeatureCriteria  featureCriteria)
{
  switch  (featureCriteria)
  {
  case  fcMerge2Best:     return  "Merge2Best";
  case  fcMerge3Best:     return  "Merge3Best";
  case  fcMerge4Best:     return  "Merge4Best";
  case  fcMerge5Best:     return  "Merge5Best";
  case  fcMerge6Best:     return  "Merge6Best";
  case  fcMerge7Best:     return  "Merge7Best";
  case  fcBestTestSet:    return  "BestTestSet";
  case  fcRemoveHurtful:  return  "RemoveHurtful";
  }

  return  "";
}  /* FeatureCriteriaToStr */



fcFeatureCriteria  FeatureCriteriaFromStr (KKStr  featureCriteriaStr)
{
  featureCriteriaStr.Upper ();
  if  ((featureCriteriaStr == "MERGE2BEST")  ||  (featureCriteriaStr == "M2B"))
    return  fcMerge2Best;

  if  ((featureCriteriaStr == "MERGE3BEST")  ||  (featureCriteriaStr == "M3B"))
    return  fcMerge3Best;

  if  ((featureCriteriaStr == "MERGE4BEST")  ||  (featureCriteriaStr == "M4B"))
    return  fcMerge4Best;

  if  ((featureCriteriaStr == "MERGE5BEST")  ||  (featureCriteriaStr == "M5B"))
    return  fcMerge5Best;

  if  ((featureCriteriaStr == "MERGE6BEST")  ||  (featureCriteriaStr == "M6B"))
    return  fcMerge6Best;

  if  ((featureCriteriaStr == "MERGE7BEST")  ||  (featureCriteriaStr == "M7B"))
    return  fcMerge7Best;

  if  ((featureCriteriaStr == "BESTTESTSET")  ||  (featureCriteriaStr == "BTS"))
    return  fcBestTestSet;

  if  ((featureCriteriaStr == "REMOVEHURTFUL")  ||  (featureCriteriaStr == "RH"))
    return  fcRemoveHurtful;

  return  fcNULL;
}  /* FeatureCriteriaFromStr */




ProcessorCycles::ProcessorCycles ():
  totalCycles (0.0)
{
}


ProcessorCycles::ProcessorCycles (const ProcessorCycles&  _cycles):
   totalCycles (0.0)
{
  const_iterator  idx;
  for  (idx = _cycles.begin ();  idx != _cycles.end ();  idx++)
    AddCycles (idx->first, idx->second);
}



void  ProcessorCycles::AddCycles (int     _procId,
                                  double  _cycles
                                 )
{
  map<int,double>::iterator  idx = find (_procId);
  if  (idx == end ())
    insert (pair<int, double> (_procId, _cycles));
  else
    idx->second += _cycles;
  totalCycles += _cycles;
}




void  ProcessorCycles::AddCycles (const ProcessorCycles&  _cycles)
{
  const_iterator  idx;
  for  (idx = _cycles.begin ();  idx != _cycles.end ();  idx++)
    AddCycles (idx->first, idx->second);
}



double  ProcessorCycles::TotalCycles ()  const
{
  double  totalCycles = 0.0;
  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    totalCycles += idx->second;
  return  totalCycles;
}



double  ProcessorCycles::LongestPath ()  const
{
  double  longestPath = 0.0;
  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    if  (idx->second > longestPath)
      longestPath = idx->second;
  }

  return  longestPath;
}



VectorInt  ProcessorCycles::ProcessorIds ()  const
{
  VectorInt  procIds;
  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    procIds.push_back (idx->first);
  return  procIds;
}



VectorDouble  ProcessorCycles::CyclesByProcId ()  const
{
  VectorDouble  cycles;
  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
    cycles.push_back (idx->second);
  return  cycles;
}


void  ProcessorCycles::Report (ostream&  r)
{
  uint32  z;

  double        totalCycles = TotalCycles ();
  VectorInt     procIds     = ProcessorIds ();
  VectorDouble  cycles      = CyclesByProcId ();

  double  cyclesTotal  = 0.0;
  double  cyclesMean   = 0.0;
  double  cyclesStdDev = 0.0;
  double  cyclesMin    = 0.0;
  double  cyclesMax    = 0.0;

  CalcVectorStats (cycles, cyclesTotal, cyclesMean, cyclesStdDev, cyclesMin, cyclesMax);

  r << "Total";
  r << "Cycles" << "\t" << "Mean" << "\t" << "StdDev" << "\t" << "Min" << "\t" << "Max";
  for  (z = 0;  z < procIds.size ();  z++)
     r << "\t" << procIds[z];
  r << endl;

  r << cyclesTotal << "\t" << cyclesMean << "\t" << cyclesStdDev << "\t" << cyclesMin << "\t" << cyclesMax;
  for  (z = 0;  z < cycles.size ();  z++)
      r << "\t" << cycles[z];
  r << endl;
}
