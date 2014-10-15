#ifndef  _FEATURESELECTION_
#define  _FEATURESELECTION_
//*********************************************************************************************************
//*                                                                                                       *
//*                                                                                                       *
//*  Developer:   Kurt Kramer                                                                             *
//*               University of South Florida                                                             *
//*               2007 thru 2009                                                                          *
//*                                                                                                       *
//*                                                                                                       *
//*  Description:                                                                                         *
//*  ------------                                                                                         *
//*  Developed this application to test out the idea of using Feature Selection for each combinatio of    *
//*  binary classes.  First started to play with this idea back in 2003 with a provious version of this   *
//*  program called BeasmSearchSVM.                                                                       *
//*                                                                                                       *
//*  Four Phases:                                                                                         *
//*  1) Parameter Tune for all Features.                                                                  *
//*  2) Feature Selection for all classes combined.                                                       *
//*                                                                                                       *
//*  3) Feature Selection for each binary class combination. (Parameters selected in phase 1 will be used)*
//*  4) Parameter Tuning  for each binary class combinaion.                                               *
//*                                                                                                       *
//*  Each phase must be completed before the next one can be started.                                     *
//*                                                                                                       *
//*  Once all phases are completed a Validation run will be performed for each of the phases.  That is    *
//*  he optimal parameters found by a given phase will be used to train a classifier.                     *
//*                                                                                                       *
//*                                                                                                       *
//*  FeatureSelection:   This module;  main control.                                                      *
//*                                                                                                       *
//*  Processor:          Created to perform a specific phase.  Several processors wil be created; one     *
//*                      for each CPU that you want to utilize.  Teh processor will schedule in a         *
//*                      cooperetive fasion all the jobs nessasary to perform a given phase.  A processor *
//*                      can be for either All-Classes or for a given binary combination of classes.      *
//*                                                                                                       *
//*  BinaryJob:          Used to control one single evaluation of parameters or features. When fearure    *
//*                      feature selection is started it will create a single 'BinaryJob' instance that   *
//*                      will represent all features selected.  Each exansion will then create more       *
//*                      'BinaryJob' instances.                                                           *
//*                                                                                                       *
//*  FeatureImpact:                                                                                       *
//*                                                                                                       *
//* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = *
//*                                                                                                       *
//*  1) the 1st Cmd Line parm is checked for certain commands.  if one of them then it processes          *
//*     that command only.                                                                                *
//*     a. -restart                                                                                       *
//*     b. -quit                                                                                          *
//*     c. -FinalReport                                                                                  *
//*     d. -Lock                                                                                          *
//*                                                                                                       *
//*                                                                                                       *
//*  2) If no 'FeatureSelection.Status' file exists then a new one is created and initialized to          *
//*     starting configuration.  If one does exists then loads the curent status of from it.  When        *
//*     one does exist there is no need to provide any command lie parameters.                            *
//*                                                                                                       *
//*                                                                                                       *
//*  3) Feature Files are loaded and data is normalized.                                                  *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*                                                                                                       *
//*********************************************************************************************************
#include "RunLog.h"
#include "Str.h"

#include "FeatureFileIO.h"
#include "FeatureVector.h"
#include "FeatureSelectionTypes.h"
#include "FinalResults.h"
#include "MLClass.h"
#include "Orderings.h"
#include "TrainingConfiguration2.h"


#ifndef  _PROCESSOR_
class  Processor;
typedef  Processor*  ProcessorPtr;
#endif

#if  !defined(_ACCBYFEATURESEL_)
class  AccByFeatureSel;
typedef  AccByFeatureSel*  AccByFeatureSelPtr;
class  AccByFeatureSelList;
typedef  AccByFeatureSelList*  AccByFeatureSelListPtr;
#endif



#include  "BinaryClass.h"




class  FeatureSelection
{
public:
  typedef  KKU::uint  uint;

  FeatureSelection (int     argc,
                    char**  argv
                   );
  
  ~FeatureSelection ();


  MLClassConstPtr        AddingAClass           ()  const {return addingAClass;}
  bool                      AlreadyNormalized      ()  const {return alreadyNormalized;}
  int                       BeamSize               ()  const {return beamSize;}
  const KKStr&              ConfigFileName         ()  const {return configFileName;}
  TrainingConfiguration2Ptr Config                 ()  const {return config;}
  int                       ExpansionLimit         ()  const {return expansionLimit;}
  fcFeatureCriteria         FeatureCriteria        ()  const {return  featureCriteria;}
  bool                      FeatureCountPrefSmall  ()  const {return featureCountPrefSmall;}
  GradingMethodType         GradingMethod          ()  const {return gradingMethod;}
  bool                      JustGettingStats       ()  const {return justGettingStats;}
  RunLog&                   Log                    ()        {return log;}
  const KKStr&              HostName               ()  const {return hostName;}
  MLClassConstListPtr    MLClasses           ()  const {return mlClasses;}
  const FeatureNumList&     InitialFeatures        ()  const;
  MajorSteps                MajorStep              ()  const {return majorStep;}
  int                       MinTrainExamples       ()  const {return minTrainExamples;}
  int                       NextProcessorId        ()  const {return nextProcessorId;}
  int                       NumJobsAtATime         ()  const {return numJobsAtATime;}
  int                       NumProcPerCombo        ()  const {return numProcPerCombo;}
  int                       NumOfFolds             ()  const {return numOfFolds;}
  JobSelectionCriteriaType  ParamSelCriteria       ()  const {return paramSelCriteria;}
  MLClassConstPtr        PositiveClass          ()  const {return positiveClass;}
  poProcessingOrder         ProcessingOrder        ()  const {return processingOrder;}
  int                       RandomSplitsNum        ()  const {return randomSplitsNum;}
  stSearchTypes             SearchType             ()  const {return searchType;}
  FeatureVectorListPtr      TestData               ()  const {return testData;}
  const KKStr&              TestDataFileName       ()  const {return testDataFileName;}
  FeatureVectorListPtr      TrainingData           ()  const {return trainingData;}
  const KKStr&              TrainingDataFileName   ()  const {return trainingDataFileName;}
  const KKStr&              ValidationDataFileName ()  const {return validationDataFileName;}
  FeatureVectorListPtr      ValidationData         ();
  
  void    Block ();

  void    BuildResultConfigFile (const KKStr&       prevConfigFileName,
                                 double&            totalCpuTimeUsed,
                                 fcFeatureCriteria  featureCriteria
                                );

  KKStr   CommandLineStr  ();
 
  void    DisplayCommandUsage ();

  void    EndBlock ();


  void    LoadFeatureData ();

  void    ProcessCmdLine (int    argc, 
                          char **argv
                         );

  void    ProcessStatusFile (int     argc,  
                             char**  argv
                            );

  FeatureVectorListPtr  RandomSplit (int splitNum);


  const   KKStr&  ResultConfigFileName ()  const  {return  resultConfigFileName;}

  void    Run ();


private:
  void                CheckForNextStep ();

  BinaryClassListPtr  CreateInitialBinaryClassList ();

  BinaryClassListPtr  CreateInitialBinaryClassListBruitSvmSearch ();

  BinaryClassListPtr  CreateInitialBinaryClassListFinalResultJobs ();

  BinaryClassListPtr  CreateInitialBinaryClassListBinaryComboSearchJobs ();

  ProcessorPtr        GetNextBinaryClassComboToRun ();

  void                FeatureImpactSummary ();
  void                FeatureImpactSummary (AccByFeatureSelListPtr  fsSumList,
                                            BinaryClassListPtr      binaryClasses
                                           );

  void                FinalReport ();

  void                GetProcessingStats ();

  /**
   *  Given a list of Binary classes will load the original Status files for each one and return 
   *  the total number of Cpu Seconds.  This can be usefull to determine the total number of CPU
   *  seconds used to prcess a Major Step.
   */
  double              GetTotalCpuSecs (BinaryClassListPtr  binClasses,
                                       double&             totalProcessingTime,
                                       double&             elapsedClockTime,
                                       double&             longestPath,
                                       int&                jobCount
                                      );
  
  double              GetTotalCpuSecs (MajorSteps  step,
                                       double&     totalProcessingTime,
                                       double&     elapsedClockTime,
                                       double&     longestPath,
                                       int&        jobCount
                                     );

  void                LoadRunTimeData ();

  void                LoadValidationData ();

  TrainingConfiguration2Ptr  OpenConfig (const KKStr&  fn);

  void                PerformInitialSetup (int argc, char** argv);

  void                ProcessRestart ();

  void                RandomSplitsBuild ();  // Will create the Random Splits of the combined Train, Test, and Validation data.

  void                ReduceProcessorCount (ProcessorPtr     processor,
                                            ClassPairStatus  status
                                           );

  FinalResultsPtr    RunAValidation (const KKStr&  summaryResultsFileName,
                                      double        totalCpuSecsSearching,
                                      double        totalProcessingTime,
                                      double        elapsedTime,
                                      double        longestPath, 
                                      int           jobCount
                                     );

  void                SetQuitRunningFlag ();

  void                SetMajorStep (MajorSteps  steps);

  void                SetUpConfigFileNames ();

  void                SetUpForNextMajorStep (BinaryClassListPtr&  binaryClasses,
                                             double&              totalCpuTimeUsed
                                            );

  void                SetUpForNextMajorStepHallOrder (BinaryClassListPtr&  binaryClasses,
                                                      double&              totalCpuTimeUsed
                                                     );

  void                SetUpForNextMajorStepStandardOrder (BinaryClassListPtr&  binaryClasses,
                                                          double&              totalCpuTimeUsed
                                                         );

  void                SetUpToBruitSvmSearch ();

  void                SetUpToGenerateFinalReports ();  // Used when user wanrts to regenerate Final Results Report.


  BinaryClassListPtr  StatusFileLoad ();

  void                StatusFileUpdate (BinaryClassListPtr  binClasses);

  MLClassConstPtr        addingAClass;              // Used when adding a class to a previously completed feature selection.

  bool                      alreadyNormalized;

  int                       beamSize;

  bool                      byBinaryClasses;           // 'true'  = Search by binary class combinations
                                                       // 'false' = Search for all classes combined.
 
  bool                      cancelFlag;

  KKStr                     configFileName;            // The config file name with directory path where it was located.
  KKStr                     configFileNameSpecified;   // The 'configFileName' as specified on command line.
  TrainingConfiguration2Ptr config;

  KKStr                     configFileNameMfsTuned;
  KKStr                     configFileNameMfsFeaturesSel;
  KKStr                     configFileNameMfsFeatureSelTuned;
  KKStr                     configFileNameMfsTunedFeaturesSel;

  KKStr                     configFileNameBfsTuned;
  KKStr                     configFileNameBfsFeaturesSel;
  KKStr                     configFileNameBfsFeaturesSelTuned;
  KKStr                     configFileNameBfsTunedFeaturesSel;
  KKStr                     configFileNameBfsTunedFeaturesSelTuned;

  int                       expansionLimit;

  FeatureFileIOPtr          driver;      // Format that the Feature Files are in, ex C45

  fcFeatureCriteria         featureCriteria;

  bool                      featureCountPrefSmall;   // true = preference for smallest number of features, else largest number of features.

  FileDescPtr               fileDesc;

  GradingMethodType         gradingMethod;

  KKStr                     hostName;            // Host name of computer we are running on

  MLClassConstListPtr    mlClasses;

  FeatureNumListPtr         initialFeatures;
  KKStr                     initialFeaturesStr;

  bool                      justGettingStats;    // If true means we are not doing any processing we want to collect stats from prev processing.

  KKStr                     lockFileName;
  int                       lockFile;
  bool                      lockFileOpened;

  RunLog                    log;

  MajorSteps                majorStep;

  int                       minTrainExamples;  // 

  int                       nextProcessorId;

  NormalizationParmsPtr     normalizationParms;

  int                       numOfFolds;

  int                       numJobsAtATime;

  int                       numProcPerCombo;  // for BinaryCombos default to 1
                                              // for all classes where there is only one combo
                                              // then as many as we want.


  poProcessingOrder         processingOrder;        // The order of the major steps.

  JobSelectionCriteriaType  paramSelCriteria;       // Criteria used to select next jobs to expand during parameter grid search

  MLClassConstPtr        positiveClass;

  int                       procId;                // Process ID assigned by OS

  bool                      quitRunning;

  OrderingsPtr              randomSplits;

  int                       randomSplitsNum;        // Number opf Random Splits that we will be using 

  KKStr                     randomSplitsFileName;   // Will contain all data that will be used by RandomSplits;  that is all the data from
                                                    // Train, Test, and Validation feature files.

  KKStr                     randomSplitsIndexName;  // Will have the index of all the sperate orderings used by RandmSplits.

  KKStr                     resultFileName;

  KKStr                     resultConfigFileName;

  stSearchTypes             searchType;

  KKStr                     statusFileName;

  FeatureVectorListPtr      testData;

  KKStr                     testDataFileName;

  FeatureVectorListPtr      trainingData;
 
  KKStr                     trainingDataFileName;

  FeatureVectorListPtr      trainingTestData;

  FeatureVectorListPtr      validationData;

  KKStr                     validationDataFileName;
};


typedef  FeatureSelection*  FeatureSelectionPtr;



#endif
