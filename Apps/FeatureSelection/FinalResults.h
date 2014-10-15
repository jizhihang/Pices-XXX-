#ifndef  _FINALRESULTS_
#define  _FINALRESULTS_
//*********************************************************************************************************
//*                                                                                                       *
//*                                                                                                       *
//*  Developer:   Kurt Kramer                                                                             *
//*               University of South Florida                                                             *
//*               2007 thru 2009                                                                          *
//*                                                                                                       *
//*  One instance for each Final randomSplitResults that we want to generate as part of the overall      *
//*  Feature Selection process.  Example there will be one instance to represent the performnace of the   *
//*  classifier using all features with SVM parameters tuned;  another for Binary Combo's and paraneters  *
//*  tuned by binary combo's; etc.                                                                        *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*********************************************************************************************************
#include "RunLog.h"
#include "Str.h"

#include "ConfusionMatrix2.h"
#include "FeatureSelectionTypes.h"
#include "HTMLReport.h"
#include "RandomSplitResults.h"
#include "ValidationResults.h"



class  FinalResults
{
public:
  typedef  KKU::uint  uint;

  typedef  FinalResults*  FinalResultsPtr;

  FinalResults (const KKStr&  _summaryResultsFileName,
                 double        _totalSearchTime,
                 double        _totalProcessingTime,
                 double        _elapsedClockTime,
                 double        _longestPath,
                 int           _jobCount,
                 RunLog&       _log
                );

  ~FinalResults ();

  
  ConfusionMatrix2Ptr  ConfusionMatrix ();

  KKStr   Description ()  const;

  static  KKStr  HeaderLineHTML1 ();
  static  KKStr  HeaderLineHTML2 ();

  MLClassConstListPtr  MLClasses ()  const;

  KKStr  LinkName () const;

  KKStr  ToHTMLStr () const;

  void   PrintConfigParametersRandomSplitsHTML (HTMLReport& r);
  void   PrintConfigParametersValidationHTML   (HTMLReport& r);

  static
  void   PrintAccuraciesByClassSummary (vector<FinalResultsPtr>&  results,
                                        HTMLReport&                o,
                                        bool                       validationData
                                       );

  VectorFloat  RandomSplitClassCounts ()  const;

  VectorFloat  RandomSplitAccuracies ()  const;

  VectorFloat  RandomSplitAccuracyByClass  ()  const;

  KKStr        SelectionMethodStr    ()  const;

  const bool*   ValidationClassedCorrectly     () const;
  int           ValidationClassedCorrectlySize () const;

  VectorFloat   ValidationAccuracyByClass  ()  const;

  VectorFloat   ValidationClassCounts ()  const;

  ConfusionMatrix2Ptr  ValidationConfusionMatrix ();


private:
  void  PrintConfigParametersHTML (HTMLReport&  r,
                                   ResultsPtr   results
                                  );

  void  ReadInResultsFile (const KKStr&  fileName);

  KKStr                      configFileName;
  double                     elapsedClockTime;
  int                        jobCount;
  double                     longestPath;
  RandomSplitResultsListPtr  randomSplitResults;
  RandomSplitResultsPtr      randomSplitsAvg;
  KKStr                      svmParameters;
  double                     totalSearchTime;
  double                     totalProcessingTime;
  ValidationResultsPtr       validationResults;
  RunLog&                    log;
};
typedef  FinalResults::FinalResultsPtr  FinalResultsPtr;





#endif
