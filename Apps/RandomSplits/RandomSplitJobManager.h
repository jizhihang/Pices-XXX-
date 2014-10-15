#if  !defined(_RANDOMSPLITJOBMANAGER_)
#define  _RANDOMSPLITJOBMANAGER_


#include  "Application.h"
#include  "FeatureVector.h"
#include  "JobManager.h"
#include  "Orderings.h"
#include  "TrainingConfiguration2.h"

using namespace  KKU;

using namespace  JobManagment;


class  RandomSplitJobManager: public  JobManagment::JobManager
{
public:
  typedef  RandomSplitJobManager*  RandomSplitJobManagerPtr;

  RandomSplitJobManager (const RandomSplitJobManager&    j);

  RandomSplitJobManager (const KKStr&      _configFileName,
                         FeatureFileIOPtr  _format,
                         const KKStr&      _dataFileName,
                         int               _numSplits,
                         float             _splitFraction,
                         int               _numFolds,
                         RunLog&           _log
                        );

  ~RandomSplitJobManager ();


  TrainingConfiguration2Ptr  Config       ()  const {return  config;}
  MLClassConstListPtr     MLClasses ()  const {return  mlClasses;}
  int                        NumFolds     ()  const {return  numFolds;}



  /*!                       RetrieveRandomSplit
   * \brief Will get the 'train' and 'test' datasets for the given split.  The lists will point to the original data
   *        so caller should not make any changes to its contents.
   * \param[int], splitNum   Split that we want data for.
   * \param[out], trainData  Training Data.
   * \param[out], testData   Test Data.
   */
  void   RetrieveRandomSplit (int                    splitNum,
                              FeatureVectorListPtr&  trainData,
                              FeatureVectorListPtr&  testData
                             );


  virtual  const char*   JobType ()  const;

  

  /*  Pure virtual functions from 'Job' that need to be implemented.  */
  virtual  JobPtr  Duplicate ()  const;  // Create a duplicate instance.
  virtual  void    ProcessNode ();


private:
  /***********************************************************************************************************************/
  /*                                         Base Class Methods Overriden                                                */
  /***********************************************************************************************************************/

  /*! \brief Seed the initial set of jobs. */
  virtual  JobListPtr  JobsCreateInitialSet ();


  /*!
   \brief  Load any run time data that will be needed.  
   \details 'JobManager' will call this method just before it calls 'StatusFileInitialize' if
            no existing StausFile otherwise it will call it just after it loads the StatusFile.
  */
  virtual  void  LoadRunTimeData ();


  /*!
   \brief  Injitilize a new StatusFile. 
   \details  JobManager::InitilizeJobManager  Will call this method if no Status file exists yet.
   */
  virtual  void  StatusFileInitialize (ostream& o);


  /*
   \brief  Process a status file line, if not meant fo rus then call the base classes method.
   \details Every new text line read in from the Status File by 'JobManager' will be passed to this routine.
            If the text line is not meant for us then call the Base classes version of this method.  
            JobManager::StatusFileProcessLine ().
   */
  virtual void    StatusFileProcessLine (const KKStr&  ln,
                                         istream&      statusFile
                                        );

  virtual  JobListPtr  JobsExpandNextSetOfJobs (const JobListPtr  jobsJustCompletd);
  virtual  void        JobCompleted            (ostream& o, JobPtr   j);

  virtual  void  GenerateFinalResultsReport ();
  /***********************************************************************************************************************/




  KKStr                     configFileName;
  TrainingConfiguration2Ptr config;
  FeatureVectorListPtr      data;             //  Master Data List
  KKStr                     dataFileName;
  KKStr                     dataIndexFileName;
  FileDescPtr               fileDesc;
  FeatureFileIOPtr          format;
  MLClassConstListPtr    mlClasses;
  int                       numFolds;
  int                       numSplits;
  float                     splitFraction;

  OrderingsPtr              splits;   //  The randomly selected orderings for each split stored hear.
};


typedef  RandomSplitJobManager::RandomSplitJobManagerPtr  RandomSplitJobManagerPtr;

#define  _RandomSplitJobManagerDefined_

#endif


