#ifndef  _FEATURESELECTIONTYPES_
#define  _FEATURESELECTIONTYPES_

#include  "Str.h"




typedef  enum  
{
  gmNULL, 
  gmAccuracy,
  gmAccuracyNorm,
  gmFMeasure
}  
  GradingMethodType;




typedef  enum  
{
  jscNULL,
  jscMostAccurate,      // Select the job that has teh highest Accuracy all other criteria is secondary.
  jscFastestFromBest,   // Select the Fastest Job from the group of jobs that are most accurate, 
  jscBruitForce         // Perform a plain bruit force Grid Search
}  
  JobSelectionCriteriaType;


typedef  enum
{
  poNULL,
  poStandard,  // AllClasses (Parameter,  FeatureSelection)  Binary (AllParam, FeatureSel, Parameter);
  poHall       // AllClasses (FeatureSel, ParamTuning)       Binary (ParamTuning, FeatureSel, ParamTuning);
}  poProcessingOrder;




/**
 @brief  MajorSteps that need to be processed.
 @details Depening on the Processing Order the FeatureSelection experiment will follow a specific order of major steps.
 @code
   Standard:   msMfsParmTuningPre,    msMfsFeatureSelection, msBfsFeatureSelection, msBfsParmTuningPost
   Hall:       msMfsFeatureSelection, msMfsParmTuningPost,   msBfsParmTuningPre,    msBfsFeatureSelection, msBfsParmTuningPost
 @endcode
 */
typedef  enum  
{
  msNULL,

  msMfsParmTuningPre,
  msMfsFeatureSelection,
  msMfsParmTuningPost,

  msBfsParmTuningPre,
  msBfsFeatureSelection,
  msBfsParmTuningPost,

  msGenerateFinalResults,
  msFinalReport,
  msGenerateBruitSvmSearch,
  msDone
} MajorSteps;



typedef  enum
{
  frtNULL,
  frtNoTuningAllFeatures,
  frtMfsFeaturesSel,
  frtMfsParmsTuned,
  frtMfsParmsTunedFeaturesSel,
  frtBfsFeaturesSel,
  frtBfsParmsTuned,
  frtBfsFeaturesSelParmsTuned,
}  FinalResultType;



typedef  enum  
{
  stNULL,
  stFeatureSelectionSearch,
  stParameterSearch,
  stGeneratingFinalResults
}
  stSearchTypes;



typedef  enum  
{
  fsNULL, 
  fsNotStarted, 
  fsStarted, 
  fsDone
}  
  fsProcessorStatus;


typedef  enum  
{
  smNULL, 
  smBestCaseNext,     //   
  smBeam,             //  > Feature Search
  smTestResults,      //    
  smPickTheBest,      //
  smGrid,             // Used by Parameter Search only
  smOnePassOnly       // Means there are no expansions;  just process the inital Jobs.
} 
  smSearchMethod;



typedef  enum
{
  fcNULL,
  fcMerge2Best,
  fcMerge3Best,
  fcMerge4Best,
  fcMerge5Best,
  fcMerge6Best,
  fcMerge7Best,
  fcBestTestSet,
  fcRemoveHurtful
}
  fcFeatureCriteria;






class  ProcessorCycles: public  map<int, double>
{
public:
  ProcessorCycles ();
  
  ProcessorCycles (const ProcessorCycles&  _cycles);

  void  AddCycles (int     _procId,
                   double  _cycles
                  );

  void  AddCycles (const ProcessorCycles&  _cycles);

  double  LongestPath ()  const;  /**< returns the Number of CPU seconds that the  longets process used. */

  double  TotalCycles ()  const;

  VectorInt     ProcessorIds   ()  const;
  VectorDouble  CyclesByProcId ()  const;
  double        totalCycles;

  void  Report (ostream&  r);
};





poProcessingOrder  ProcessingOrderFromStr (const KKStr&  procOrderStr);
KKStr              ProcessingOrderToStr   (poProcessingOrder  order);




MajorSteps   MajorStepFromStr  (KKStr  majorStepStr);
KKStr        MajorStepToStr    (MajorSteps  step);



FinalResultType  FinalResultTypeFromStr (const KKStr&  s);
KKStr             FinalResultTypeToStr   (FinalResultType  frt);


stSearchTypes  SearchTypeFromStr (KKStr  searchTypeStr);
KKStr          SearchTypeToStr   (stSearchTypes  _searchType);



KKStr  GradingMethodToStr (GradingMethodType  gm);
GradingMethodType  GradingMethodFromStr (const KKStr&  str);



KKStr  JobSelectionCriteriaToStr (JobSelectionCriteriaType  gm);
JobSelectionCriteriaType  JobSelectionCriteriaFromStr (const KKStr&  str);



KKStr              SearchMethodToStr      (smSearchMethod     searchMethod);
smSearchMethod     SearchMethodFromStr    (KKStr              searchMethodStr);



KKStr              ProcessorStatusToStr   (fsProcessorStatus  processorStatus);
fsProcessorStatus  ProcessorStatusFromStr (const KKStr&       processorStatusStr);



KKStr              FeatureCriteriaToStr   (fcFeatureCriteria  featureCriteria);
fcFeatureCriteria  FeatureCriteriaFromStr (KKStr              featureCriteriaStr);




#endif

