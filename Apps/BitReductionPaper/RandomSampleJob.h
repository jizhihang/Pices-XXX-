#ifndef  _RANDOMSAMPLEJOB_
#define  _RANDOMSAMPLEJOB_


#ifndef  _CROSSVALIDATION_
class  CrossValidation;
typedef  CrossValidation*  CrossValidationPtr;
#endif


#if  !defined(_ORDERINGS_)
class  Orderings;
typedef  Orderings* OrderingsPtr;
#endif


#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"
#include  "SVMparam.h"
#include  "TrainingConfiguration.h"

typedef  enum   {rjNULL, rjOpen, rjStarted, rjDone}  rjJobStatus;






class  RandomSampleJob
{
public:
  RandomSampleJob (TrainingConfigurationPtr  _config,
                   OrderingsPtr              _orderings,
                   int                       _jobId,
                   int                       _orderingNum,
                   int                       _numExamplesToKeep
                  );

  RandomSampleJob (TrainingConfigurationPtr  _config,
                   OrderingsPtr              _orderings,
                   int                       _numExamplesToKeep,
                   const KKStr&              statusLine
                  );


  ~RandomSampleJob ();


  float               Accuracy () const {return accuracy;}


  void                DeleteCrossValidation ();

  void                EvaluteNode (FeatureVectorListPtr  validationData,
                                   MLClassListPtr     classes
                                  );

  int                 JobId    () const {return jobId;}

  rjJobStatus         Status   () const {return status;}

  void                Status   (rjJobStatus _status) {status = _status;}

  double              SupportVectors ()  const {return supportVectors;}

  double              TrainTime () const  {return trainTime;}
  double              TestTime  () const  {return testTime;}

  void    ProcessStatusStr (KKStr  statusStr);

  static
    void    ReportResultsHeader (ostream&   r);

  void    ReportResults (ostream&  r);

  KKStr  ToStatusStr ();

  void    ToStatusStr (KKStr  statusStr);

  static  KKStr       JobStatusToStr   (rjJobStatus   status);
  static  rjJobStatus  JobStatusFromStr (KKStr        statusStr);



private:
  float                     accuracy;
  double                    c;
  SVM_CompressionMethod     compMethod;       // If NULL means was not stred when ran
  TrainingConfigurationPtr  config;
  CrossValidationPtr        crossValidation;
  SVM_EncodingMethod        encodingMethod;
  double                    gamma;
  RunLog&                   log;
  int                       jobId;
  SVM_KernalType            kernelType;
  int                       numExamplesToKeep;
  OrderingsPtr              orderings;
  int                       orderingNum;
  double                    supportVectors;
  double                    testTime;
  double                    trainTime;
  rjJobStatus               status;
}; /* RandomSampleJob */



typedef  RandomSampleJob*  RandomSampleJobPtr;




class  RandomSampleJobList: public KKQueue<RandomSampleJob>
{
public:
  RandomSampleJobList (RunLog&  _log);

  RandomSampleJobList (RunLog&                   _log,
                       TrainingConfigurationPtr  _config,
                       OrderingsPtr              _orderings,
                       int                       _numExamplesToKeep,
                       const KKStr&             _fileName
                      );



  RandomSampleJobList (RunLog&                   _log,
                       TrainingConfigurationPtr  _config,
                       KKStr                     trainDataFileName,
                       FeatureVectorListPtr      trainData,
                       OrderingsPtr              orderings
                      );

  bool          AreAllJobsAreDone ();

  void          CleanUpCrossValidations ();

  int           DetermineCompressedImageCount (FeatureVectorListPtr       trainData,
                                               TrainingConfigurationPtr   config
                                              );

  void          Load (TrainingConfigurationPtr  _config,
                      OrderingsPtr              _orderings,
                      int                       _numExamplesToKeep
                     );

  RandomSampleJobPtr  LocateOpenJob ();

  RandomSampleJobPtr  LookUpByJobId (int  jobId);

  void                Save ();

  void                Save (const KKStr&  _fileName);


  static
  RandomSampleJobList*    CreateNewList (RunLog&                   _log,
                                         TrainingConfigurationPtr  _config,
                                         OrderingsPtr              _orderings,
                                         int                       _numExamplesToKeep,
                                         const KKStr&             _fileName
                                        );


  void  CalcAccuracyStats (double&  accuracyMean, 
                           double&  accuracyStdDev,
                           double&  trainTimeMean,
                           double&  testTimeMean,
                           double&  supportVectorsMean
                          )  const;



private:


  void  CreateRandomJobs (TrainingConfigurationPtr  _config,
                          OrderingsPtr              _orderings,
                          int                       _numExamplesToKeep
                         );

  KKStr        jobFileName;
  RunLog&       log;


};  /* RandomSampleJobList */



typedef  RandomSampleJobList*  RandomSampleJobListPtr;



#endif
