#ifndef  _PROCESSOR_
#define  _PROCESSOR_



#ifndef  _BINARYJOB_
class  BinaryJob;
class  BinaryJobList;
typedef  BinaryJob*  BinaryJobPtr;
typedef  BinaryJobList*  BinaryJobListPtr;
#endif


#ifndef  _FILEDESC_
class  FileDesc;
typedef  FileDesc*  FileDescPtr;
#endif



#if  !defined(_FEATUREIMPACT_)
class  FeatureImpact;
typedef  FeatureImpact*  FeatureImpactPtr;
#endif


//#ifndef  _FEATURESELECTION_
//class  FeatureSelection;
//typedef  FeatureSelection*  FeatureSelectionPtr;
//#endif

class  Processor;
typedef  Processor*  ProcessorPtr;

#include "BinaryClass.h"
#include "FeatureSelection.h"
#include "KKStrParser.h"
#include "RunLog.h"
#include "Str.h"

#include "MLClassConstList.h"
#include "TrainingConfiguration2.h"


#include "FeatureSelection.h"


//*************************************************************************
//*    Responsable for keep track of which set of Binmary Classes a given *
//*  Processor is responsable for.                                        *
//*                                                                       *
//*************************************************************************
class  Processor
{
public:   
  Processor (FeatureSelectionPtr  _featureSelection,
             bool                 _byBinaryClasses,
             int                  _processorId,
             FileDescPtr          _fileDesc,
             stSearchTypes        _searchType,
             MLClassConstPtr   _class1,
             MLClassConstPtr   _class2,
             smSearchMethod       _searchMethod,
             fsProcessorStatus    _status
            );


  // RandomSplit process.  Use this constructor when you want to manage RandmSplit prcessing
  // suct that is done when getting the FinalResults

  // Make sure the the _summaryResultsFileName is deleted before we start processing.
  Processor (FeatureSelectionPtr  _featureSelection,
             int                  _processorId,
             BinaryClassPtr       _bnaryClass,
             FileDescPtr          _fileDesc,
             fsProcessorStatus    _status
            );



  ~Processor ();

  bool                      AlreadyNormalized      () const  {return  alreadyNormalized;}
  int                       BeamSearchFeatureCount () const  {return  beamSearchFeatureCount;}
  float                     BestA                  () const  {return  bestA;}
  double                    BestC                  () const  {return  bestC;}
  double                    BestGamma              () const  {return  bestGamma;}
  int32                     BestNumOfRounds        () const  {return  bestNumOfRounds;}
  bool                      ByBinaryClasses        () const  {return  byBinaryClasses;}
  MLClassConstPtr        Class1                 () const  {return  class1;}
  MLClassConstPtr        Class2                 () const  {return  class2;}
  TrainingConfiguration2Ptr Config                 () const  {return  config;}
  const ProcessorCycles&    CpuCyclesByProcId      () const  {return  cpuCyclesByProcId;}
  const DateTime&           DateTimeEnded          () const  {return  dateTimeEnded;}
  const DateTime&           DateTimeStarted        () const  {return  dateTimeStarted;}
  FeatureSelectionPtr       FeatureSelection       () const  {return  featureSelection;}
  FileDescPtr               FileDesc               () const  {return  fileDesc;}
  GradingMethodType         GradingMethod          () const  {return  featureSelection->GradingMethod ();}
  MLClassConstListPtr    MLClasses           () const  {return  mlClasses;}
  MLClassConstListPtr    ClassesThisProcess     () const  {return classesThisProcess;}
  const FeatureNumList&     InitialFeatures        () const;
  RunLog&                   Log                    () const  {return  log;}
  int                       NextJobId              () const  {return  nextJobId;}
  int                       NumOfFolds             () const  {return  numOfFolds;}
  MLClassConstPtr        PositiveClass          () const;
  int                       ProcessorId            () const  {return  processorId;}
  bool                      QuitRunning            () const  {return  quitRunning;}
  const KKStr&              ResultFileName         () const  {return  resultFileName;}
  FinalResultType          ResultType             () const  {return  resultType;}
  SVM_SelectionMethod       SelectionMethod        () const  {return  selectionMethod;}
  fsProcessorStatus         Status                 () const  {return  status;}
  const KKStr&              SummaryResultsFileName () const  {return  summaryResultsFileName;}
  double                    TotalCpuTimeUsed       () const  {return  totalCpuTimeUsed;}
  FeatureVectorListPtr      TrainingData           () const  {return  trainingData;}
  FeatureVectorListPtr      TestData               ();
  FeatureVectorListPtr      ValidationData         ();




  int      AllocateNextJobId ();  // Used to get next available Job Id

  bool     AreAllJobsDone ();

  BinaryJobListPtr  BinaryJobs ();

  void    Block ();

  const KKStr&  Class1Name () const;
  const KKStr&  Class2Name () const;

  void    EndBlock ();
  
  void  FlagJobsForTesting (ofstream*         statusFile,
                            BinaryJobListPtr  candidats
                           );

  FeatureNumList  GetFeaturesFromTestResults ();
  

  void            GetBestParametersFromTestResults (double&  cParm, 
                                                    double&  gammaParm, 
                                                    float&   aParm
                                                   );


  FeatureNumList  GetFeaturesRemoveHurtFul ();


  FeatureImpactPtr  GetFeatureImpact ();


  /**
   @brief  Return the smallest Job Id that processed for given featuerCounts.
   */
  VectorInt       GetSmallestJobIdByFeatureCount ();


  /**
   @brief  Return the Numbe CPU processed for each FeatureCount.
   */
  VectorDouble  GetCpuCyclesByFeatureCount ();

  FeatureNumList  MergeKBestFeatureSelections (int  k,
                                               int  minNumOfFeatures
                                              );

  void     ProcessRestart ();

  FeatureVectorListPtr  RandomSplit (int splitNum);

  void     Run ();
  
  FeatureNumList  SelectBestFeatures ();

  void     SetQuitRunningFlag ();

  double   TotalProcessingTime ()  const;  // returns the total processing time from all jobs in binaryJobs.

  void     Update (ProcessorPtr  p);



private:

  FeatureVectorListPtr  BuildOurBinaryFeatureData (FeatureVectorListPtr  srcData);

  void    CreateParameterJobs (ofstream*  statusFile,
                               int        parentJobId,
                               float      cParmGrowthRate,
                               double     cParmMin,
                               double     cParmMax,
                               float      gamaParmGrowthRate,
                               double     gammaParmMin,
                               double     gammaParmMax,
                               float      aParmGrowthRate,
                               float      aParmMin,
                               float      aParmMax
                              );

  void  CreateParameterJobsUsfCasCor (ofstream*  statusFile, 
                                      int32      minNumberOfRoumds,
                                      int32      maxNumOfRounds
                                     );

  void    CreateRandomSplitsJobs (ostream*  statusFile);

  void    CreateValidationJob (ostream*        statusFile,
                               BinaryClassPtr  _binaryClass
                              );

  void    GenerateFinalResultsReport ();

  void    GenrateSvmResponseSheet ();

  void    GenerateFinalResultsReportHTML ();

  void    GenerateFinalResultsReportValidationHTML (ostream&  r);

  BinaryJobListPtr  GetNextSetOfJobs (BinaryJobListPtr  completedJobs);

  void    InitializeStatusFile ();

  void    InitializeStatusFileParameterSearch ();

  void    InitializeStatusFileRandomSplits (BinaryClassPtr  _binaryClass);

  ofstream*  OpenStatusFile (ios::openmode  openMode);

  void    ProcessBeamExpansion (ofstream*  statusFile,
                                int&       numJobsCreated
                               );

  void    ProcessGridSearchExpansionFastestFromBest (ofstream*  statusFile,
                                                     int&       numJobsCreated
                                                    );

  
  void    ProcessGridSearchExpansionBruitForce (ofstream*  statusFile,
                                                int&       numJobsCreated
                                               );


  void    ProcessGridSearchExpansionUsfCasCor (ofstream*  statusFile,
                                               int&       numJobsCreated
                                              );


  void    ProcessGridSearchExpansionMostAccurate (ofstream*  statusFile,
                                                  int&       numJobsCreated
                                                 );


  void    ProcessNextBestCaseExpansion (ofstream*  statusFile,
                                        int&       numJobsCreated
                                       );

  void    ProcessTestResultsExpansion (ofstream*  statusFile,
                                       int&       numJobsCreated
                                      );


  void    ProcessTestJobsAndPickTheBest (ofstream*  statusFile,
                                         int&       numJobsCreated
                                        );

  void    ProcessNextExpansion (ofstream*  statusFile);

  void    ProcessStatusFileLine (KKStrParser&  statusStr);

  void    ProcessStatusFileLineJobStatusChange (KKStrParser&  statusLineStr);

  void    ReportCpuTimeUsed (ofstream* statusFile);

  void    RunFeatureSelection ();

  void    RunParameterGridSearch ();

  void    StatusFileLoad   ();  // To load initial contents of status file.
  
  void    StatusFileRefresh ();  // Will read in any changes to status file.

  KKStr   ToStatusStr ();

  void    UpdateExpandedJobs (ofstream*         statusFile,
                              BinaryJobPtr      expandedJob,
                              BinaryJobListPtr  newJobs
                             );


  void    WriteStatusFile ();
  void    WriteStatusFileGrowthRates (ostream&  statusFile);
  void    WriteStatusFileBestParmValues (ostream&  statusFile);



  bool                      alreadyNormalized;
  int                       beamSize;

  int                       beamSearchFeatureCount;   // The number of features that the search was 
                                                      // at when the switch to the Beam Search was triggered.

  BinaryJobListPtr          binaryJobs;          // List of jobs that we are working with.  We
                                                 // will be maintaining consistance with other
                                                 // parallel running processes through the 
                                                 // 'status' file.  As we make changes to each 
                                                 // individule 'BinaryJob' we write them out to
                                                 // the status file.  We periodically read from 
                                                 // the status file to see if there have been
                                                 // changes made by other processors.

  bool                      byBinaryClasses;

  TrainingConfiguration2Ptr config;              /*!<  We need to own this copy of config */

  int                       expansionLimit;      // The number of expansions with out finding 
                                                 // higher accuracy that are allowed before 
                                                 // switching over to beam search.
  FeatureSelectionPtr       featureSelection;
  FileDescPtr               fileDesc;

  MLClassConstListPtr    mlClasses;        // Comes from FeatureSelection;  represents all classes involved in entire search.  
  MLClassConstListPtr    classesThisProcess;  // Classs that are involved with this Process only;  so if binary class combo 
                                                 // will only consist of those two classes.

  int                       lockFile;
  KKStr                     lockFileName;
  bool                      lockFileOpened;

  RunLog&                   log;

  bool                      noMoreJobsLeftToExpand;   
                                                 // Will be set to true when 'ProcessNextExpansion'  
                                                 // can not find any more jobs to expand.  When this 
                                                 // happens we can terminate the feature Selection 
                                                 // for this Processor.

  int                       numJobsAtATime;
  int                       numOfFolds;
  int                       procId;
  bool                      quitRunning;
  KKStr                     statusFileName;

  double                    lastCpuTimeReported;
  double                    totalCpuTimeUsed;
  KKU::DateTime             dateTimeStarted;
  KKU::DateTime             dateTimeEnded;
  bool                      dateTimeFirstOneFound;  // Flag that indicates weather we have read the first 
                                                    // instance of a 'CurrentDateTime' in the Status file.

  ProcessorCycles           cpuCyclesByProcId;


  long                      statusFileNextByte;     // Byte offset of next bye to read from status file.

  FeatureVectorListPtr      trainingData;           // This will consist of examples that are pertinent 
                                                    // to the classes that we(This particular instance 
                                                    // of Processor is concerned with.  We will not own 
                                                    // these examples.

  FeatureVectorListPtr      testData;               // 'Test' Data that is seperate from trainingData.  
                                                    // we will not own these examples, either.


  //********************************************
  //*    Fields that will be in Status File    *
  //********************************************
  MLClassConstPtr   class1;                 // The two particular classes that this instance of
  MLClassConstPtr   class2;                 // 'Processor' is to be working with.  If they are
                                                    // equal to 'NULL' then we are to work with all 
                                                    // classes.

  int                  expansionsSinceHighGrade;

  int                  firstJobAvailableForExpansion;  
                                                 // Used by 'ProcessBeamExpansion' routine. Will only
                                                 // select from jobs with a equal or higher JobId.
                        
  KKStr                summaryResultsFileName;   // name of file where the RandmSplit results are to be written to.


  float                highGrade;
  int                  nextJobId;
  int                  numOfExpansions;
  int                  processorId;
  KKStr                resultFileName;
  FinalResultType     resultType;               // Used when generating Final Results via RandomSplits (JobRandomSplit)
  smSearchMethod       searchMethod;             // Beam, BestCaseNext, etc
  stSearchTypes        searchType;               // FeatureSelection, stParameterSearch
  fsProcessorStatus    status;
  SVM_SelectionMethod  selectionMethod;          // Only Used during FinalResults phase.

  float                aGrowthRate;
  float                cGrowthRate;
  float                gammaGrowthRate;

  float                bestA;       // Used by the Parameter Search;  when we decide on a Gamma and "C"
  double               bestC;       // these variables will be set;  and after that point we will only be
  double               bestGamma;   // interested in searching for "A".
  int32                bestNumOfRounds;
};




class  ProcessorList: public  KKQueue<Processor>
{
public:
  ProcessorList (const KKStr& _rootDir);
  ~ProcessorList ();

private:
};



#endif
