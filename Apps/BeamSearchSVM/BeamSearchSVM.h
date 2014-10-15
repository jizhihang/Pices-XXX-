#ifndef  _BEAMSEARCHSVM_
#define  _BEAMSEARCHSVM_

#include "Str.h"

#include "svm.h"
#include "SVMparam.h"
#include "ImageFeatures.h"
#include "SingleRun.h"
#include "SingleRunList.h"


typedef  enum {Beam, BestFirst, BestFirstOut}  SearchMethod;

#define  MaxNumPrevResults  10

class  BeamSearchSVM
{
public:
  BeamSearchSVM ();
  
  ~BeamSearchSVM ();

  void    Block ();

  KKStr   CommandLineStr  ();
 
  void    EndBlock ();

  void    exit_with_help ();

  void    LetsDoItInBatches ();   // Meant to work when running standalone,  NOT Multi Processing

  void    parse_command_line (int    argc, 
                              char **argv
                             );

  void    ProcessAllOpenJobsAtOnce (SingleRunListPtr  queue);

  void    ProcessCmdLineStr  (KKStr  cmdLine);

  void    ProcessStatusFile (int argc,  char **argv);

  void    read_problem ();

  void    RunOneJobOnly ();


  SingleRunListPtr  ScheduleNextLevelOfJobs (SingleRunListPtr  queue);

  void    ScheduleMoreJobsBestFirst (SingleRunListPtr  queue);

  void    ScheduleMoreJobsBestFirstOut (SingleRunListPtr  queue);


private:

  void  AddJobAndParentsToQueue (SingleRunList&  masterList,
                                 SingleRunList&  list, 
                                 SingleRunPtr    job
                                );

  
  
  SingleRunListPtr  BuildQueueList     (SingleRunList&  openJobs);  // used when updating only open file.
  SingleRunListPtr  BuildOpenQueueList (SingleRunList&  openJobs);

  void              DetermineIfMoreJobs (bool&  moreJobs);

  KKStr             ExcludedFeaturesStr (SingleRunPtr  j)  const;

  FeatureNumList    ExtractFeatureNumbers (KKStr&  featureListStr);

  SingleRunListPtr  LoadStatusFileOpen (bool forceLoadDone = false);

  ofstream*         OpenDoneStatusFile ();

  void    PerformARestart ();

  KKStr   PrevResultsStr ();

  void    ProcessPrevResultsStr (KKStr  prevResultsStr);


  void    ProcessTerminationSituation (SingleRunListPtr&  queue);


  void    ScheduleAJobRandomly (SingleRunListPtr  queue,
                                ostream&          resultFile
                               );

  void    SwitchOverToBeamSeach (SingleRunListPtr&  queue);

  bool    TerminationConditionReached ();

  void    UpdatePrevResults (double  result);

  void    UpdateStatusFile (SingleRunListPtr  queue);




  // Batch Processing Methods.
  void  DoneJobsReportResults (SingleRunListPtr  jobsJustDone,
                               SingleRunListPtr  jobsOnFile
                              );

  SingleRunListPtr  GetNextBatchOfOpenJobs (bool&             allJobsAreDone,
                                            SingleRunListPtr  jobsOnFile
                                           );

  void  ProcessAllJobsInOpenStatusFile ();

  void  TrySchedulingMoreJobs (bool&  moreJobs);

  SingleRunListPtr  UpdateDoneJobsAndGetNextBatch (bool&             allJobsAreDone,
                                                   SingleRunListPtr  jobsJustDone
                                                  );



  Algorithm                 algorithm;

  int                       batchSize;

  int                       beamSize;

  double                    bestResultThisNode;

  bool                      cancelFlag;

  TrainingConfigurationPtr  config;
  KKStr                     configFileName;

  SingleRunListPtr          doneJobs;

  int                       expansionLimit;  // Maximum # of expansions without hitting a new
                                             // highestAccuracySoFar that will be allowed.
                                             // If this limit is reached, search is to 
                                             // terminate.

  int                       expansionsSinceHighestAccuracy;
  
  FileDescPtr               fileDesc;

  FileFormat                fileFormat;

  double                    highestAccuracySoFar;

  ImageClassList            imageClasses;

  ImageFeaturesListPtr      images;
 
  KKStr                     initialFeaturesStr; // As specified in cmd line.

  FeatureNumListPtr         initialFeatures;    // If specied in command line, indicates the features
                                                // that we are to start the search with.
   
  KKStr                     inputFileName; 

  bool                      lockFileOpened;
  int                       lockFile;

  RunLog                    log;

  int                       nextJobId;

  int                       numOfFolds;

  int                       numOfExpansions;

  int                       numPrevResults;   // Ref 'prevResults'

  bool                      performARestart;

  double                    prevResults[MaxNumPrevResults];

  KKStr                     resultFileName;

  bool                      schedulingEnabled;

  bool                      schedulerStarted;
                                       
  DateTime                  startTime;

  double                    startCpuTime;

  double                    totalCpuTime;

  double                    trainTimeTotal;

  double                    testTimeTotal;

  ImageFeaturesListPtr      validationData;
  KKStr                     validationDataFileName;

  bool                      weAreStillSearching;

  #ifndef  WIN32
  bool                      weWereSleeping;   // Only used in unix world.
  #endif;
};
#endif
