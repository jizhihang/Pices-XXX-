#ifndef  _JOBMANAGER_
#define  _JOBMANAGER_

namespace  JobManagment
{
}


#include  "Job.h"


#include  "RunLog.h"


/*!
 \namespace  JobManagment
 \brief  NameSpace for the JobManagement Library.
 */

namespace  JobManagment
{
  /*!
  \class  JobManager
  \brief  Responsable for keeping track of a list of jobs.
  \details See the application called RandomSplitsJobManager.  It is the first alplication to use 
           this library.  any class that is derived from this class are required to implement these
           methid  "StatusFileProcessLine",  "StatusFileProcessLineJobStatusChange", "ToStatusStr"
  */
  class  JobManager: public  Job
  {
  public:
    typedef  JobManager*  JobManagerPtr;

    JobManager (const JobManager& j);

    JobManager (JobManagerPtr _manager,               /*!< Ptr to job that is managing this 'JobManager'  */
                int           _jobId,
                int           _parentId,
                int           _numPorcessesAllowed,
                const KKStr&  _managerName,           /*!< Name of this 'JobManager' ; status and lock file will be based on it.                */
                int           _numJobsAtATime,        /*!< The number of jobs that can be allocatd at one time for a single process to execute. */
                RunLog&       _log
               );

    ~JobManager ();


    /*!
     \brief  Initialize the JobManager object.
     \details  This is the first Method that needs to be called right after the object is constructed.
     */
    void  InitilizeJobManager (bool&  successful);


    virtual  JobPtr  Duplicate ()  const = 0;  /*!< Create a duplicate instance.  */

    virtual  const char*   JobType ()  const;   /*!< Allows us to know which specific implementaion of 'Job'  an instance really is. */


    bool        SupportCompletedJobData () const  {return  supportCompletedJobData;}

    void        SupportCompletedJobData (bool  _supportCompletedJobData)  {supportCompletedJobData = _supportCompletedJobData;}


    double        CpuTimeTotalUsed  () const  {return  cpuTimeTotalUsed;}
    int           ExpansionCount    () const  {return  expansionCount;}
    JobListPtr    Jobs              () const  {return  jobs;}
    const KKStr&  ManagerName       () const  {return  managerName;}
    int           NextJobId         () const  {return  nextJobId;}
    bool          QuitRunning       () const  {return  quitRunning;}


    int         AllocateNextJobId ();  // Used to get next available Job Id

    bool        AreAllJobsDone ();

    void        Block ();
 
    void        EndBlock ();

    int         GetNextJobId ();   // Returns 'nextJobId' then increments by '1'.
  
    void        Restart ();

    void        Run ();
  
    void        SetQuitRunningFlag ();

    void        Update (JobManagerPtr  p);



  private:
    virtual
      JobListPtr  JobsCreateInitialSet () = 0;   // Derived classes use this method to sed the initial set of jobs.


    // JobsExpandNextSetOfJobs
    // Derived classes need to implement this method.  It will be called when all jobs have been completed.  The 
    // variable 'jobs' will contain a list of all jobs.
    // jobsJustCompletd -  Contains a list of jobs that have been completed since the last time this 
    //                     method was called or 'JobsCreateInitialSet' was called.
    // Return - List of new jobs that are to be processed.  A NULL signifies that no more jobs to process.
    virtual  JobListPtr  JobsExpandNextSetOfJobs (const JobListPtr  jobsJustCompletd) = 0;

    
    virtual  void  GenerateFinalResultsReport () = 0;


    virtual  void  LoadRunTimeData () = 0;  // Load any run time data that will be needed.  'JobManager' will 
                                            // call this method just before it calls 'StatusFileInitialize' if
                                            // no existing StausFile otherwise it will call it just after it
                                            // loads the StatusFile.


    // In one (Block - EndBlock)  sequesnce will update completd jobs and select next set of jobs to
    // process.
    JobListPtr  GetNextSetOfJobs (JobListPtr  completedJobs);



    //****************************************************************************************
    //*   Status File Routines.
    void    ReportCpuTimeUsed (ofstream* statusFile);

    void    StatusFileInitialize ();

    virtual 
      void  StatusFileInitialize (ostream& o) = 0;


    ofstream*  StatusFileOpen (ios::openmode  openMode);

    void       StatusFileLoad    ();  /*!< To load initial contents of status file.  */
  
    void       StatusFileRefresh ();  /*!< Will read in any changes from status file since last call.  */

    void       StatusFileWrite   ();  /*!< Will rewrite the status file from scratch;  the old one will be deleted.  */


  protected:
    // The next 3 methods need to be implemented by any Derived classes.  Those derived class need to also
    // call the Base class version of this method.
    virtual void    StatusFileProcessLine (const KKStr&  ln,
                                           istream&      statusFile
                                          );

    virtual void    StatusFileProcessLineJobStatusChange (KKStr&  statusLineStr);

    virtual KKStr   ToStatusStr ();  // derived classes should implement this method.  They need to return a tab
                                     // delimietd string with all there parameters
                                     // <FieldName1> <\t> <FieldValue1> <\t> <fieldName2> <\t> <FieldValue2>  etc etc etc ...
                                     // They should call the base Class version of this method and return that string first.
                                     // ex:  return JobManager::ToStatusStr () + "\t" + "FieldName1" + "\t" + "Value1" + "\t" + "FieldName2" + "\t" + "Value2";


  private:
    //****************************************************************************************
    void  ProcessNextExpansion (ostream&  o);

    void  ProcessRestart ();

    void  ProcessJobXmlBlockOfText (const KKStr&  startStr,
                                    istream&      i
                                   );


    double          cpuTimeLastReported;
    double          cpuTimeTotalUsed;
    KKU::DateTime   dateTimeStarted;
    KKU::DateTime   dateTimeEnded;
    bool            dateTimeFirstOneFound;  // Flag that indicates weather we have read the first 
                                            // instance of a 'CurrentDateTime' in the Status file.


    JobListPtr      jobs;                   // List of jobs that we are working with.  We
                                            // will be maintaining consistance with other
                                            // parallel running processes through the 
                                            // 'status' file.  As we make changes to each 
                                            // individule 'BinaryJob' we write them out to
                                            // the status file.  We periodically read from 
                                            // the status file to see if there have been
                                            // changes made by other processors.
  
    int             blockLevel;             // Starts at 0;  Will increments with each Call to 'Block'  and  decrementes with 'EndBlock'
    int             lockFile;
    KKStr           lockFileName;
    bool            lockFileOpened;

    KKStr           managerName;
    bool            supportCompletedJobData;  // If set to 'true' will call 'WriteCompletedJobData' after each job is completed.

    int             expansionCount;
    int             expansionFirstJobId;    // jobId of first job in the current expansion.
    int             nextJobId;
    int             numJobsAtATime;
    int             procId;                 // Processor Id  that O/S assigns to this thread.
    bool            quitRunning;            // If this flag is set true we are to terminate processing as soon as possible.
  
    bool            restart;
  
    KKStr           statusFileName;
    long            statusFileNextByte;     // Byte offset of next bye to read from status file.
  
  };  /* JobManager */
  

  typedef  JobManager::JobManagerPtr  JobManagerPtr;

  #define  _JobManagerDefined_



  class  JobManagerList: public  KKQueue<JobManager>  
  {
  public:
    JobManagerList (const KKStr& _rootDir);
    ~JobManagerList ();

  private:
  };

}  /* JobManagment */



#endif





