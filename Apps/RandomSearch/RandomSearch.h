#ifndef  _RANDOMSEARCH_
#define  _RANDOMSEARCH_

#include "RunLog.h"
#include "Str.h"

#include  "Application.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "ResultLine.h"
#include  "TrainingConfiguration2.h"


#define  OtherOrderings 5


class  RandomSearch: public Application
{
public:
  RandomSearch (int     argc,
                char**  argv
               );
  
  ~RandomSearch ();



  bool                       AlreadyNormalized ()  const {return alreadyNormalized;}
  KKStr                      ConfigFileName    ()  const {return configFileName;}
  TrainingConfiguration2Ptr  Config            ()  const {return config;}
  FeatureVectorListPtr       Data              ()  const {return data;}
  MLClassListPtr          MLClasses      ()  const {return mlClasses;}
  int                        NumOfFolds        ()  const {return numOfFolds;}
  KKStr                      ResultFileName    ()  const {return resultFileName;}


  void    Block ();

  KKStr  CommandLineStr  ();
 
  void    DisplayCommandUsage ();

  void    EndBlock ();

  void    Run ();

  void    Run0 ();
  void    Run1 ();
  void    Run2 ();
  void    Run3 ();

  void    LoadFeatureData ();



private:
  float Accuracy (ResultLinePtr  result);

  void  AddToCombinations (ResultLineVector&  resultsToCombine,
                           ResultLinePtr      result
                          );

  void  AddToCombinationsIfNotRelated (int                deapth,   // How many famliies to search back
                                       ResultLineVector&  results,
                                       VectorInt&         families,
                                       ResultLinePtr      result
                                      );

  virtual const char*  ApplicationName () {return  "RandomSearch";}

  ResultLineListPtr  BuildComponentParts (ResultLinePtr  result);

  void  BuildOtherOrderings ();


  ResultLinePairVector  BuildPairsFromList (ResultLineVector&  resultsToCombine);


  void  CheckAgainstOtherOrderings (ResultLinePtr  result);


  ResultLineList  CombineSomeOfTheBestJobs (int  numToCombine,
                                              int  maxSize
                                             );

  ResultLinePtr  EvaluateAFeatureSet (FeatureNumListPtr  features,
                                       ResultLinePtr      parent1,
                                       ResultLinePtr      parent2,
                                       ResultLinePtr      family
                                      );


  ResultLineList  EvaluateCombinations (ResultLinePairVector&  combinations);

  void  ExpandedUnSetAll ();


  void  GetTestAccuracy (ResultLinePtr  result,
                         float&         accuracy,
                         float&         accuracyWeighted
                        );

  ResultLinePairVector  GetUnrelatedPairs (ResultLineVector&  resultsToCombine,
                                           int                deapth
                                          );


  ResultLineVector  LookForUsedUpInAllCombinations (ResultLineVector&  onesToTry,
                                                    int&               numNotCombined
                                                   );

  void   PerformWrapper (ResultLinePtr  seedResult);
 

  virtual  bool    ProcessCmdLineParameter (char    parmSwitchCode, 
                                            KKStr  parmSwitch, 
                                            KKStr  parmValue
                                           );

  void             ReportStats ();

  ResultLinePtr    RunARandomlySelectedSetOfFeatures ();


  ResultLinePtr    TrimAFeatureSet (ResultLinePtr  resultToExpand,
                                    float          highAccuracy
                                   );


  ResultLinePtr    TrimAFeatureSetInDepth (int            depth,
                                           int            width,
                                           ResultLinePtr  resultToExpand,
                                           float          highAccuracy,
                                           float*         highs         // The high accuracies by feature count
                                          );

  ResultLinePtr    TrimBestNotExpanded (int  numToTrim,
                                        int  maxSize
                                       );


  ResultLinePtr    TrimComponentsOfAFeatureSet (ResultLinePtr  resultToExpand,
                                                float          highAccuracy
                                               );

  ResultLineList   TrySomeCombinationsOfTheBetterOnesFoundSofar (int  maxSize);



  bool                      accuracyWeighted;   // Use AccuracyClassWeightedEqually rather than just Acuracy

  FeatureNumListPtr         allFeatures;

  bool                      alreadyNormalized;  // If set to true means that the featuer daya in 
                                                // 'dataFileName' is already normalized.

  bool                      cancelFlag;

  KKStr                     configFileName;
  TrainingConfiguration2Ptr config;
  
  MLClassListPtr         configClasses;

  FeatureVectorListPtr      data;               // Feature data as loaded from data file specified
                                                // in command line.

  FeatureVectorListPtr      dataOther[OtherOrderings];     // Feature data as loaded from data file specified

  KKStr                     dataFileName;       // Data file containing feature data to use to drive 
                                                // search.

  FileDescPtr               fileDesc;           // Structure taht describes feature data loaded from 
                                                // 'dataFileName'.

  FeatureFileIOPtr          fileFormat;         // Format that 'dataFileName' will be in.

  float                     highAccuracy;       // Highest Accuracy encounbtered during search.

  MLClassListPtr         mlClasses;       // List of all classes.
 
  KKStr                     lockFileName;
  int                       lockFile;
  bool                      lockFileOpened;

  int                       maxNumOfFeatures;  // Will start out at INT_MAX and reduce to level that
                                               // that is being expanded, then after a number of 
                                               // expansions will reduce by 1.  

  int                       numOfFolds;        // Num to be used by 'x-Fold-Cross-Validation' 

  int                       randomFeatures;    // How many features to select randomly.

  KKStr                     resultFileName;    // Name of file to store results in,  if not 
                                               // specified in the command line will default
                                               // to 'dataFileName' with extension 'result'

  ofstream*                 resultFile;

  ResultLineTreePtr         results;           // A data structure that will contain all feature
                                               // combinations calculated.  See 'ResultLine.h' for
                                               // definition of this structure.  It is based of 
                                               // 'RBTree.h' Red Blacvk tree whose primary index
                                               // will be the features selected.  This will 
                                               // allow us to look up previously computed total's
                                               // quickly,  also has secondary indesx using 
                                               // 'resultId' and 'parentId'

  bool                      stratify;


  KKStr                    testFileName;
  FeatureVectorListPtr      testData;

  bool                      verifyAccuracy;
};


typedef  RandomSearch*  FeatureSelectionPtr;



#endif
