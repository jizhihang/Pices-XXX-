#ifndef  _BITREDUCTIONJOB_
#define  _BITREDUCTIONJOB_


#include  "BitReduction.h"
#include  "DataSetParameters.h"
#include  "DateTime.h"
#include  "MLClass.h"
#include  "FeatureVector.h"
#include  "Orderings.h"
#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"
#include  "SVMparam.h"
#include  "TrainingConfiguration.h"


typedef  enum   {brNULL, brOpen, brStarted, brDone}  brJobStatus;

typedef  enum   {brjNULL, 
                 brjRandomSampling, 
                 brjNormalJob, 
                 brjRandomSplits, 
                 brjSmallSample, 
                 brjTuningTest
                }  brjJobTypes;

typedef  enum  {laNULL,
                laSupportVectorMachine,
                laUSFcascor
               }
                 laLearningAlgorithms;


#define  BitRetReductionJobVersion  1


class  BitReductionJob
{
public:
  typedef  BitReductionJob*  BitReductionJobPtr;

  BitReductionJob (RunLog&                _log,
                   int                    _jobId,
                   laLearningAlgorithms   _learningAlgorithm,
                   SVM_KernalType         _kernalType,
                   SVM_SelectionMethod    _selectionMethod,
                   SVM_EncodingMethod     _encodingMethod,
                   SVM_CompressionMethod  _compMethod,
                   bool                   _weighted,
                   int                    _cParam,
                   double                 _gamma,
                   int                    _bitReduction,
                   int                    _unBalBits,
                   brjJobTypes            _jobType,
                   uint                   _ordering,
                   int                    _numOfFeatures,
                   int                    _numFeatursAfterEncoding
                  );

  BitReductionJob (RunLog&       _log,
                   const KKStr&  toString
                  );

  BitReductionJob (const BitReductionJob& brj);

  ~BitReductionJob ();

  float                  Accuracy        () const {return accuracy;}
  int                    BitReduction    () const {return bitReduction;}
  SVM_CompressionMethod  CompMethod      () const {return compMethod;}
  int                    CParam          () const {return cParam;}
  SVM_EncodingMethod     EncodingMethod  () const {return encodingMethod;}
  double                 Gamma           () const {return gamma;}
  int                    JobId           () const {return jobId;}
  brjJobTypes            JobType         () const {return jobType;}
  KKStr                  JobTypeStr      () const {return JobTypeToStr (jobType);}
  SVM_KernalType         KernalType      () const {return kernalType;}
  uint                   Ordering        () const {return ordering;}
  int                    SampledTrainingSetSize () const {return sampledTrainingSetSize;}
  SVM_SelectionMethod    SelectionMethod () const {return selectionMethod;}
  brJobStatus            Status          () const {return status;}
  double                 TestTime        () const {return testTime;}
  double                 TotalTuningTime () const {return totalTuningTime;}
  double                 TrainTime       () const {return trainTime;}
  int                    UnBalBits       () const {return unBalBits;}
  const KKStr&           UnBalFeatures   () const {return unBalFeatures;}
  bool                   Weighted        () const {return weighted;}
  long                   RandomSeed      () const {return randomSeed;}


  
  void                 RunTuningExample1 (TrainingConfigurationPtr  _config,
                                          FeatureVectorListPtr      trainData,
                                          FeatureVectorListPtr      testData,
                                          MLClassListPtr         imaageClasses,
                                          float                     samplePercentage,
                                          ostream&                  report,
                                          float                     accLossTolerance
                                         );



  void                 RunTuningExample2 (TrainingConfigurationPtr  _config,
                                          FeatureVectorListPtr      trainData,
                                          FeatureVectorListPtr      testData,
                                          MLClassListPtr         imaageClasses,
                                          float                     samplePercentage,
                                          ostream&                  report,
                                          float                     accLossTolerance
                                         );



  void                 EvaluteNode (TrainingConfigurationPtr  _config,
                                    const KKStr&             trainDataFileName,
                                    FeatureVectorListPtr      trainData,
                                    FeatureVectorListPtr      testData,
                                    MLClassListPtr         imaageClasses,
                                    bool*                     noCompClassedCorrectly,
                                    bool*                     classedCorrectly,
                                    ostream&                  r
                                   );

 void                  EvaluateRandomSplit (TrainingConfigurationPtr  _config,
                                            Orderings&                splitOrderings,
                                            MLClassList&           mlClasses,
                                            ostream&                  r
                                           );

  void                 Status   (brJobStatus _status) {status = _status;}

  void                 ParseTabDelStr (const KKStr&  statusStr);

  void                 ReportResults (ostream&         r,
                                      BitReductionJob* unCompressedJob
                                     );

  void                 ReportTuningResults (ostream&  r);

  KKStr               ToTabDelStr ();

  void                 ZeroOut ();



  static  KKStr       JobStatusToStr   (brJobStatus status);
  static  brJobStatus  JobStatusFromStr (KKStr      statusStr);

  static  KKStr       JobTypeToStr     (brjJobTypes  jobType);
  static  brjJobTypes  JobTypeFromStr   (const KKStr&  _str);

  BitReductionJob&  operator+= (const BitReductionJob&  r);
  BitReductionJob&  operator/= (float  r);


  static
    void  ReportResultsHeader (ostream&   r);

  static
    void  ReportTuningResultsHeader (ostream&   r);

   BitReductionJob&  operator= (const BitReductionJob&  r);
 

   static
      bool  DoseUsfCasCorExist ();

   static
      void  TestASpecificConfiguraion  (laLearningAlgorithms       learningAlgorithm,
                                        TrainingConfigurationPtr   config,
                                        bool                       dataAlreadyNormalized,
                                        FeatureVectorListPtr       normTrainData,
                                        FeatureVectorListPtr       normTestData,
                                         float&                     testAccuracy,
                                        double&                    trainTime,
                                        double&                    testTime,
                                        bool*                      classedCorrectly,
                                        CompressionStats&          compressionStats,
                                        double&                    supportVectors
                                       );



private:

 static
    void  TestWithUSFCASCOR  (TrainingConfigurationPtr   config,
                              FeatureVectorListPtr       normTrainData,
                              FeatureVectorListPtr       normTestData,
                              float&                     testAccuracy,
                              double&                    trainTime,
                              double&                    testTime,
                              CompressionStats&          compressionStats
                             );


  static 
     void  TestWithSVM  (TrainingConfigurationPtr   config,
                         FeatureVectorListPtr       normTrainData,
                         FeatureVectorListPtr       normTestData,
                         float&                     testAccuracy,
                         double&                    trainTime,
                         double&                    testTime,
                         bool*                      classedCorrectly,
                         CompressionStats&          compressionStats,
                         double&                    supportVectors
                        );



  void  EvaluateRandomSample (TrainingConfigurationPtr  config,
                              const KKStr&             trainDataFileName,
                              FeatureVectorListPtr      trainData,
                              FeatureVectorListPtr      testData,
                              MLClassListPtr         mlClasses,
                              ostream&                  r
                             );

  void  ExtractRandomSamplingForTrainAndTest (FeatureVectorListPtr   fullTrainData,
                                              FeatureVectorListPtr&  sampleTrainData,
                                              FeatureVectorListPtr&  sampleTestData,
                                              float                  samplePercentage
                                             );



  void  FindStartingBitReductionLevel (TrainingConfigurationPtr  config,
                                       FeatureVectorListPtr      trainData,      // Needs to be already normalized
                                       int&                      bitReduction,
                                       double&                   processingTime,
                                       ostream&                  report
                                      );

  RunLog&                log;
  int                    version;
  int                    jobId;
  brJobStatus            status;
  SVM_KernalType         kernalType;
  SVM_SelectionMethod    selectionMethod;
  SVM_EncodingMethod     encodingMethod;
  SVM_CompressionMethod  compMethod;
  bool                   weighted;
  int                    cParam;
  double                 gamma;
  int                    bitReduction;
  int                    unBalBits;
  KKStr                 unBalFeatures;
  brjJobTypes            jobType;
  int                    numOfFeatures;
  int                    numFeatursAfterEncoding;
  uint                   ordering;   // Used only with randomSplit jobs 
  float                  accuracy;
  float                  supportVectors;
  double                 redTime;
  double                 trainTime;
  double                 testTime;
  int                    redPreCount;
  int                    redPostCount;
  float                  redRatio;
  float                  testStat;
  float                  randSampAccuracy;  // Only used with 'randomSplit' jobs.
  DateTime               runTime;

  // Bit Reduction Tuning Fields.
  double  procTimeBaseLineAccuracy;
  double  procTimeSampleData; 
  double  procTimeFindStartBitRed;
  double  procTimeBestBitRed;
  double  procTimeBestUnBalBits;
  float   baseLineAccuracy;
  int     sampledTrainingSetSize;
  int     startBitReductionLevel;
  double  totalTuningTime;

  long    randomSeed;


  laLearningAlgorithms  learningAlgorithm;


};

typedef  BitReductionJob::BitReductionJobPtr    BitReductionJobPtr;




KKStr  LearningAlgorithmToStr      (laLearningAlgorithms  _learningAlgorithm);
KKStr  LearningAlgorithmToShortStr (laLearningAlgorithms  _learningAlgorithm);

laLearningAlgorithms  LearningAlgorithmFromStr (KKStr  s);

#endif
