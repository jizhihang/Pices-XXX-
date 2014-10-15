#ifndef  _BITREDUCTIONJOBLIST_
#define  _BITREDUCTIONJOBLIST_




#include  "BitReductionJob.h"
#include  "DataSetParameters.h"
#include  "DateTime.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "Orderings.h"
#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"
#include  "SVMparam.h"
#include  "TrainingConfiguration.h"



#include  "Str.h"

#define  MaxNumOfRandomSplits 50


class  VectorBitReductionJobs;
typedef  VectorBitReductionJobs*  VectorBitReductionJobsPtr;



class  BitReductionJobList: public KKQueue<BitReductionJob>
{
public:
  BitReductionJobList (RunLog&                   _log,
                       const KKStr&              _rootDirName,
                       TrainingConfigurationPtr  _config,
                       const DataSetParameters&  _dsp,
                       brjJobTypes               _jobTypes,
                       ostream&                  _report,
                       float                     _samplePercentage,
                       laLearningAlgorithms      _learningAlgorithm,
                       float                     _accLossTolerance
                      );

  ~BitReductionJobList ();


  bool                AreAllJobsAreDone ();

  void                CreateJobs ();

  void                CreateRandomSplitJobs ();

  void                CreateTuningJobs ();

  static
  int                 DeriveNumNumericFeatures (TrainingConfigurationPtr config);

  void                InitVariables ();

  void                Load ();

  FeatureVectorListPtr  LoadDataFileForRandomSampling (const KKStr&  fileName);

  void                LoadNoCompClassedCorrectly ();

  BitReductionJobPtr  LocateOpenJob ();

  BitReductionJobPtr  LookUpByJobId (int  jobId);

  void                ProcessJobQueue (ostream& r);

  void                ProcessRandomSplitsJobQueue (ostream& r);

  void                ProcessTuningJobQueue (ostream& r);

  void                ReportRandomSplits (ostream& r);

  void                Save ();
  void                SaveNoCompClassedCorrectly ();

  void                SetNoCompFileName (const  BitReductionJobPtr  j);

  bool                Valid ()  const {return valid;}


  static
    void    ReportResultsHeader (ostream&   r);


private:
  void  AnalizeDistributionOfFeatureData (const FeatureVectorList&  examples);

  void  Block ();

  void  CompressionStatistics (TrainingConfigurationPtr  config,
                               FeatureVectorListPtr      trainData
                              );


 
  FeatureVectorListPtr  CreateListWithNoMissingData (FeatureVectorListPtr  examples);

  void  EndBlock ();

  
  void  ExtractRandomSamplingForTrainAndTest (const KKStr&          fileName,
                                              FeatureVectorListPtr&  sampleTrainData,
                                              FeatureVectorListPtr&  sampleTestData
                                             );

  

  bool  JobsHaveSameSvmParms (const BitReductionJob&  j1, 
                              const BitReductionJob&  j2
                             )  const;

  

  VectorBitReductionJobsPtr  LocateOpenJobs (uint  numJobsToFetch,
                                             int&  minBitRedLevel,
                                             int&  minUnBalLevel
                                            );


  void  SaveCompressedData  (BitReductionJobPtr    j,
                             FeatureVectorListPtr  compTrainData,
                             FeatureVectorListPtr  compTestData
                            );


  float                     accLossTolerance;
  bool*                     classedCorrectly;
  TrainingConfigurationPtr  config;
  KKStr                     configFileName;
  KKStr                     dataSetDirName;
  const DataSetParameters&  dsp;
  FileDescPtr               fileDesc;
  MLClassListPtr         mlClasses;
  KKStr                     jobFileName;
  laLearningAlgorithms      learningAlgorithm;
  int                       lockFile;
  KKStr                     lockFileName;
  bool                      lockFileOpened;
  RunLog&                   log;
  int                       nextJobId;
  bool*                     noCompClassedCorrectly;
  KKStr                     noCompFileName;         // Name of file whre noCompClasssedCorrectly will be saved. 
  brjJobTypes               jobTypes;
  ostream&                  report;
  KKStr                     rootDirName;
  float                     samplePercentage;
  KKStr                     splitOrderigsFileName;
  FeatureVectorListPtr      testData;
  KKStr                     testDataFileName;
  FeatureVectorListPtr      trainData;
  KKStr                     trainDataFileName;
  bool                      valid;

};  /* BitReductionJobList */


class  VectorBitReductionJobs: public vector<BitReductionJobPtr>  
{
public:
  VectorBitReductionJobs (bool  _owner):
      owner (_owner)
  {}

  ~VectorBitReductionJobs ()
  {
    if  (owner)
    {
      VectorBitReductionJobs::iterator  idx;
      for  (idx = begin ();  idx != end ();  idx++)
        delete  (*idx);
    }
  }

  void  PushOnBack (BitReductionJobPtr  j)
  {
    push_back (j);
  }

private:
  bool  owner;
};  /* VectorBitReductionJobs */



typedef  VectorBitReductionJobs*  VectorBitReductionJobsPtr;



typedef  BitReductionJobList*  BitReductionJobListPtr;



#endif
