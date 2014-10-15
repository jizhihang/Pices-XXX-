#ifndef  _BINARYJOBLIST_
#define  _BINARYJOBLIST_

#include  "KKQueue.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"

#include  "BinaryJob.h"



class   BinaryJobList;
typedef BinaryJobList* BinaryJobListPtr;


class  Processor;
typedef  Processor*  ProcessorPtr;





class  BinaryJobList: public KKQueue<BinaryJob>
{
public:
  typedef  KKU::uint  uint;

  typedef  BinaryJobList*  BinaryJobListPtr;


  BinaryJobList (ProcessorPtr  _processor);

  BinaryJobList (ProcessorPtr _processor,
                 KKStr        _fileName
                );

  virtual
    ~BinaryJobList ();


  void   ComputeMinAndMaxParmValues (double&  cMin,  
                                     double&  cMax,
                                     double&  gammaMin,
                                     double&  gammaMax,
                                     float&   aMin,
                                     float&   aMax
                                    );

  bool              AreAllJobsDone ();

  //void            CleanUpCrossValidations ();
  BinaryJobListPtr  CreateTestJobsForHighetsGradePerFeatureCount ();


  // Used to extract all jobs that are with in a given factor of the fastst job in the list
  BinaryJobListPtr  ExtractFastestJobs (double  minTime,       // will extract all jobs that are less than 'minTime'  or 
                                        float   topPercentage  // fastestJob * factor
                                       );

  BinaryJobListPtr  ExtractHighestGrade (int  numToExtract);

  BinaryJobListPtr  ExtractHighestGrade (float  topPercentage,   // if HighestGrade is 90% and topPercentage = 1.0%  then all jobs
                                         int    minNumOfJobs     // whos accuracy is >= (90% * (100 - topPercentage) /100).  If the 
                                        );                       // number of jobs that meet this criteria are lessthan 'minNumJobs'
                                                                 // then the 'minNumJobs' most accurate will be selected.

  
  BinaryJobListPtr  ExtractHighestGrade (float  percentageFromTop);  // Get all jobs who's grade is within 'percentageFromTop' 
                                                                     // of the highest grade.


  BinaryJobListPtr  ExtractHighestTestGrade (float  acceptableDelta,
                                             bool   featureCountPrefSmall
                                            );

  BinaryJobListPtr  ExtractLowDeltaProbAccuarcy (float topPercentage);

  BinaryJobListPtr  ExtractJobsForAGivenFeatureCount (int featureCount);

  BinaryJobListPtr  ExtractByMinimumProcessintTime (double  maxProcessingTime);

  BinaryJobListPtr  ExtractJobsWithSmallestDeltaAccProbForEachSvmParameterSet ();

  BinaryJobListPtr  ExtractTestJobs ();

  BinaryJobPtr      FastestJob ();

  FileDescPtr       FileDesc ();

  void              FindNeighboringParameterValues (BinaryJobPtr  targetJob,
                                                    double&       cMin,
                                                    double&       cMax,
                                                    float         cMaxGrowthFactor,
                                                    double&       gammaMin,
                                                    double&       gammaMax,
                                                    float         gammaMaxGrowthFactor,
                                                    float&        aMin,
                                                    float&        aMax,
                                                    float         aMaxGrowthFactor
                                                   );

  void              GenrateSvmResponseSheet (ostream&  r);

  map<double, int>* GetListOfUniqueCValues ();

  map<double, int>* GetListOfUniqueGammaValues ();


  BinaryJobPtr      HighestGradedJob ();

  BinaryJobPtr      HighestGradedTestJob ();

  bool              JobsStillRunning (); 

  void              Load ();

  BinaryJobPtr      LocateOpenJob ();

  RunLog&           Log ()  {return log;}

  BinaryJobPtr      LookUpByFeatureNums (const FeatureNumList&  _features);

  BinaryJobPtr      LookUpByJobId (int  jobId);


  BinaryJobListPtr  LookUpByParameters (double  minC,
                                        double  maxC,
                                        double  minGamma,
                                        double  maxGamma
                                       );

  BinaryJobPtr      LookUpByParameters (const FeatureNumList&  features,
                                        int32                  numOfRounds,
                                        double                 cParm,
                                        double                 gamma,
                                        float                  aParm,
                                        bool                   validationOnly
                                       );

  BinaryJobPtr      LowestDeltaAccuracyProbJob ();


  BinaryJobPtr      LowestDeltaAccuracyProbTestJob ();

  void              PushOnBack (BinaryJobPtr  j);

  void              ReFresh ();   //  Will open data file and load any new entries in data file that have been added.  Uses
                              //  lastFilePostion to determine where to read from.

  void              ReportResults (ostream&  r);

  void              ReportResultsHTML (ostream&  r);

  void              Save ();

  BinaryJobPtr      SelectOneDoneJobAtRandomFromTop10Percent ();

  FeatureNumList    SelectFeaturesByMostFeaturesWithHighestGrade ()  const;

  int               SmallestNumberOfFeaturesExpanded ()  const;

  void              SortByGrade ();
   
  void              SortByAccuracy ();
 
  void              SortByAccuracyNorm ();

  //*******************************************************************
  //* Order of sort is                                                *       
  //* 1) ValidationOnly jobs 1st                                      *
  //* 2) if both are Validation jobs                                  *
  //*    2.a TestAccuracy (High to Low)                               *
  //*    2.b if testAccracy same then treat as non Validation jobs.   *
  //* 3) Non Valdation Jobs                                           *
  //*    3.1 Least Features to more Features                          *
  //*    3.2 Shorter Processingh time to more Processing Time.        *
  //*    3.3 Random.                                                  *
  //*******************************************************************
  void          SortByTestAccuracy (); 

  void          SortByTestAccuracyNorm (); 

  void          SortByTestGrade (bool  featureCountPrefSmall);

  void          SortDeltaProbAccuarcy ();

  void          SortDeltaTestProbAccuarcy ();

  void          SortByProcessingTime ();

  void          SortBySvmParameters ();

  double        TotalProcessingTime ()  const;  // Returns the total processing time from all jobs in the list.

private:
  KKStr                   fileName;
  long                    lastFilePostion;
  RunLog&                 log;
  ProcessorPtr            processor;

  map<int,BinaryJobPtr>            jobIdLookUpTable;
  map<int,BinaryJobPtr>::iterator  jobIdLookUpTableIdx;
  typedef pair <int, BinaryJobPtr> JobIdLookUpTablePair;

  struct  ParmsKey
  {
    ParmsKey ():  job(NULL)                    {}
    ParmsKey (BinaryJobPtr  _job):  job(_job)  {}

    bool  operator()  (const ParmsKey&  parm1,
                       const ParmsKey&  parm2
                      )  const
    {
      BinaryJobPtr j1 = parm1.job;
      BinaryJobPtr j2 = parm2.job;

      if  (j1->NumOfRounds () != j2->NumOfRounds ())
        return (j1->NumOfRounds () < j2->NumOfRounds ());

      else if  (j1->CParm () != j2->CParm ())
        return  (j1->CParm () < j2->CParm ());

      else if  (j1->AParm () != j2->AParm ())
        return  (j1->AParm () < j2->AParm ());

      else if  (j1->GammaParm () != j2->GammaParm ())
        return  (j1->GammaParm () < j2->GammaParm ());

      else if  (j1->ValidateOnly () != j2->ValidateOnly ())
        return  (j1->ValidateOnly () < j2->ValidateOnly ());

      else
        return (j1->Features () < j2->Features ());
    }

    BinaryJobPtr  job;
  };

  map<ParmsKey, BinaryJobPtr, ParmsKey>            parmsLookUpTable;
  map<ParmsKey, BinaryJobPtr, ParmsKey>::iterator  parmsLookUpTableIdx;
  typedef pair <ParmsKey, BinaryJobPtr>            ParmsLookUpTablePair;

  BinaryJob parmsLookUpJob;

};  /* BinaryJobList */
   


typedef  BinaryJobList::BinaryJobListPtr  BinaryJobListPtr;



#endif
