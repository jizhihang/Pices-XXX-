#ifndef  _SINGLERUNLIST_
#define  _SINGLERUNLIST_

#include  "KKQueue.h"

#include  "FileDesc.h"
#include  "SingleRun.h"





class  StatusKey
{
public:
  StatusKey (char  _curStatus,
             int   _jobId
             ):
     
      curStatus (_curStatus),
      jobId     (_jobId)
  {
  }


  StatusKey (const StatusKey&  _statusKey):
      curStatus (_statusKey.curStatus),
      jobId     (_statusKey.jobId)
  {
  }


  bool  operator< (const StatusKey& rightSide)  const
  {
    if  (curStatus < rightSide.curStatus)
      return true;

    else if  (curStatus > rightSide.curStatus)
      return false;

    else
      return  jobId < rightSide.jobId;
  }

  char  curStatus;
  int   jobId;
};  /* StatusKey */


typedef  map<StatusKey,SingleRunPtr>  StatusKeyIndex;
typedef  StatusKeyIndex*  StatusKeyIndexPtr;




class  FeatureNumListOperator
{
public:
   FeatureNumListOperator (){}

   bool  operator() (const BitStringPtr  p1,
                     const BitStringPtr  p2
                    )  const
   {
     return  ((*p1) < (*p2));
   }
};  /* FeatureNumListOperator */



class  AccuracyKey
{
public:
  AccuracyKey (int   _featureCount,
               float _accuracy,
               int   _jobId
               ):
     
      featureCount (_featureCount),
      accuracy     (_accuracy),
      jobId        (_jobId)
  {
  }

  AccuracyKey (const AccuracyKey&  _accuracyKey):
      featureCount (_accuracyKey.featureCount),
      accuracy     (_accuracyKey.accuracy),
      jobId        (_accuracyKey.jobId)
  {
  }


  AccuracyKey (const SingleRunPtr  s):
      featureCount (s->FeatureCount ()),
      accuracy     (float (s->Accuracy ())),
      jobId        (s->JobId ())
  {
  }

  bool  operator< (const AccuracyKey& rightSide)  const
  {
    if  (accuracy > rightSide.accuracy)
      return true;

    else if  (accuracy < rightSide.accuracy)
      return false;

    else if  (featureCount < rightSide.featureCount)
      return true;

    else if  (featureCount > rightSide.featureCount)
      return false;

    else
      return  jobId < rightSide.jobId;
  }

  char   featureCount;
  float  accuracy;
  int    jobId;
};  /* AccuracyKey */





typedef  map<const BitStringPtr, const SingleRunPtr, FeatureNumListOperator>  FeatureIndex;
typedef  FeatureIndex*  FeatureIndexPtr;

typedef  map<AccuracyKey, const SingleRunPtr>  NextBestJobIndex;
typedef  NextBestJobIndex*  NextBestJobIndexPtr;

typedef  pair<AccuracyKey,const SingleRunPtr>  NextBestJobPair;



class  SingleRunList:  public KKQueue<SingleRun>
{
public:
  typedef  SingleRunList*  SingleRunListPtr;

  SingleRunList (bool  _owner);


  SingleRunList (const KKStr&  _fileName);

  ~SingleRunList ();

  //void              AddSingleRun (SingleRunPtr  singleRun);

  bool              AllJobsEvaluated ()  const;

  void              DeleteEntry (SingleRunPtr _entry);


  SingleRunListPtr  GetJobsForNumOfFeatures (int numOfFeatures);

  SingleRunListPtr  GetNextBestJobs ();

  SingleRunPtr      GetNextOpenJobUsingStatusIndex ();

  void  LoadStatusFile (Algorithm    algorithm,
                        FileDescPtr  fileDesc,
                        RunLog&      log
                       );

  SingleRunPtr      LookUpByJobId (int  jobId);

  SingleRunPtr      LookUpRun (const FeatureNumList&  featuresIncluded);

  SingleRunPtr      LookUpRun (const BitString&  featuresIncluded);

  SingleRunPtr      NextUnStartedJob ();

  void              PushOnBack (SingleRunPtr _entry);
 
  void              PushOnFront (SingleRunPtr _entry);

  void              SortByAccuracy ();

  void              SortByJobId ();

  void              UpdateStatusIndex (SingleRunPtr  j,
                                       char          oldStatus,
                                       char          newStatus
                                      );

#ifdef  KKDEBUG
  static  void   DisplayLeftOvers ()  {memoryTracker.DisplayLeftOvers ();}
  static  MemoryTracker<SingleRunList>   memoryTracker;
  #endif

private:
  void  BuildStatusIndex ();

  void  BuildNextBestJobIndex ();

  FeatureIndex              featureIndex;
  KKStr                    fileName;
  map<int, SingleRunPtr>    idIndex;
  fpos_t                    lastReadPos;
  bool                      lastReadPosValid;
  NextBestJobIndexPtr       nextBestJobIndex;
  StatusKeyIndexPtr         statusIndex;
};




typedef  SingleRunList::SingleRunListPtr  SingleRunListPtr;




class  SingleRunListIterator: public  QueueIterator<SingleRun>
{
public:
  SingleRunListIterator (SingleRunList&  runs);
};

#endif

