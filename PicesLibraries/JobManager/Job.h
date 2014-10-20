#ifndef  _JOB_
#define  _JOB_

//***************************************************************************
//*  Kurt Kramer                                                            *
//*  2009-10-10                                                             *
//*                                                                         *
//*   This code is currently not in use.  I am toying with the idea to make *
//*   a general Job Management Frame Work.  The Idea is that 'Job' and      *
//*   'JobManager' would be used to build specific Job Management code      *
//*   arround.  In the case of 'FeatureSeletion'  we would detrive sub-     *
//*   classes to manage the Major Speps and Jobs which would then in turn   *
//*   call a different set of Derved classes that would manage Binary,      * 
//*   MultiClass,  Parameter Tuning, and Feature Selection tasks.           *
//*                                                                         *
//*   ex:  JobManager would be replaced by.                                  *
//*      ParameterManager:: public JobManager                               *
//*      FeatureSelectionManager:: public JobManager                        *
//*                                                                         *
//*   Would also need to create a set of classes to manage the RandomSplits *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//***************************************************************************






#include  "KKQueue.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"




namespace  JobManagment
{

  class   JobList;
  typedef JobList* JobListPtr;


  #if  !defined(_JobManagerDefined_)
  class  JobManager;
  typedef  JobManager*  JobManagerPtr;
  #endif



  class  Job
  {
  public:
    typedef  enum   {jsNULL, jsOpen, jsStarted, jsDone, jsExpanded}  JobStatus;

    typedef  enum   {jtNULL, jtBinaryCombo, jtMultiClass, jtRandomSplit}  JobTypes;

    typedef  Job*  JobPtr;

    Job (const Job&  j);

    //  To create a brand new job that has not been proceesed yet.  
    Job (JobManagerPtr  _manager,
         int            _jobId,
         int            _parentId,
         int            _numPorcessesAllowed,
         RunLog&        _log
        );


    Job (JobManagerPtr  _manager);


    ~Job ();



    //*************************************************************************************************************************
    typedef  JobPtr (*ConstructorPtr)(JobManagerPtr _manager);

    typedef  map<KKStr, ConstructorPtr>  ConstructorIndex;

    static  ConstructorIndex  registeredConstructors;  // Track all the different constructors.  Will be used to know wich constructor to
                                                       // use when reading the status file.

    static  void  RegisterConstructor (const KKStr&    _name,
                                       ConstructorPtr  _constructor
                                      );

    static  JobPtr  CallAppropriateConstructor (JobManagerPtr  _manager,
                                                const KKStr&   _jobTypeName,
                                                const KKStr&   _statusStr
                                               );
    //*************************************************************************************************************************




    //***************************************** Access Methods **********************************************
    int                JobId         () const  {return jobId;}
    int                ParentId      () const  {return parentId;}
    JobManagerPtr      Manager       () const  {return manager;}
    const VectorInt&   Prerequisites () const  {return prerequisites;}
    RunLog&            Log           ()        {return log;}
    JobStatus          Status        () const  {return status;}
    KKStr              StatusStr     () const;

    void   JobId     (int        _jobId)   {jobId   = _jobId;}
    void   Status    (JobStatus  _status)  {status  = _status;}



    //**************************  Methods that derived classes must implement  ******************************
    virtual  JobPtr  Duplicate ()  const = 0;  // Create a duplicate instance.


    virtual  const char*   JobType ()  const;     // This will allow us to know which specific implementaion
                                                  // of 'Job'  an instance really is.

    virtual  void       ProcessNode () = 0;

    virtual  void       ReFresh (Job&  j);

    virtual  KKStr      ToStatusStr ();      // Any SubClass that implements this method must call its base class version 1st.

    // Any SubClass of Job  needs to define this method.  Whenever the 'ProcessStatusStr' method can not identify a field
    // it will call this method to let the child Class process the field.
    virtual
      void  ProcessStatusField (const KKStr&  fieldName,
                                const KKStr&  fieldValue
                               );


    //                   *** CompletedJobData ***
    // This method will get called right after the "Job" status line gets written when a Job is completed.  
    // See 'JobManager::GetNextSetOfJobs'.  If a job needs to write more data to the Status file then you want to 
    // put on a single status line this is where you would do it.   You write all the text in a format that you 
    // want to support.  'JobManager' will bracket it with <Job JobType=Job::JobType, JobId=####>   and </Job>
    // ex:
    // <RandomSplitJob>
    //    Job Data;  
    //    Job Data,
    //    etc. etc. etc.
    // </RandomSplitJob>
    virtual  
      void  CompletedJobDataWrite (ostream& o);


    // Works with 'WriteCompletedJobData';  You use this to load in data written by 'WriteCompletedJobData'
    virtual  
      void  CompletedJobDataRead (istream& i);



    //*******************************************************************************************************


    void  AddPrerequisites (int        _prerequisite);
    void  AddPrerequisites (VectorInt  _prerequisites);

    bool  InPrerequisites (int _jobId);   // Retuens true if  '_jobId' is in 'prerequisites'.


    static   KKStr      JobStatusToStr   (JobStatus  status);
    static   JobStatus  JobStatusFromStr (const KKStr&  statusStr);

    static   KKStr      JobTypeToStr   (JobTypes jt);
    static   void       JobTypeFromStr (const KKStr&  s);


    RunLog&         log;


  protected:
    KKStr  PrerequisitesToStr ()  const;
    void   PrerequisitesFromStr (const KKStr&  s);

    void   ProcessStatusStr (const KKStr&  statusStr);

    int             jobId;
    int             parentId;
    JobManagerPtr   manager;
    int             numProcessors;         // Number of CPU's that are currently processing this node;  That is 
                                           // the num ber that are currently calling the 'ProcessNode' method.

    int             numPorcessesAllowed;   // The number of Processes that are allowd to process at same time.
                                           // that is ('numPorcessesAllowed' <= 'numActiveProcessors').

    VectorInt       prerequisites;         // List of JobId's that must complete (jobStatus == jsDone) before this 
                                           // process may be started.

    JobStatus       status;
  };  /* Job */

  typedef  Job::JobPtr  JobPtr;

  #define  _JobDefined_





  class  JobList: public KKQueue<Job>
  {
  public:
    typedef  JobList*  JobListPtr;

    JobList (const JobList& jobs);

    JobList (JobManagerPtr  _manager);

    JobList (JobManagerPtr _manager,
             KKStr         _fileName
            );

    bool              AllPrequisitesDone (JobPtr job);

    bool              AreAllJobsDone ();

    bool              JobsStillRunning (); 

    JobPtr            LocateOpenJob ();

    RunLog&           Log ()  {return log;}

    JobPtr            LookUpByJobId (int  jobId);

    void              PushOnBack (JobPtr  j);



  private:
    RunLog&          log;
    JobManagerPtr    manager;

    map<int,JobPtr>              jobIdLookUpTable;
    map<int,JobPtr>::iterator    jobIdLookUpTableIdx;
    typedef pair <int, JobPtr>   JobIdLookUpTablePair;

  };  /* JobList */

  typedef  JobList::JobListPtr  JobListPtr;

  #define  _JobListDefined_
}  /* JobManagment */

#endif
