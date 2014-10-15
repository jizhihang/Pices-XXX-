#ifndef  _BINARYJOB_
#define  _BINARYJOB_

#include "KKQueue.h"
#include "KKStrParser.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"

#include "CrossValidation.h"
#include "FeatureNumList.h"
#include "MLClassConstList.h"
#include "ImageFeatures.h"

#define  PunishmentFactor  0.00f


typedef  enum   {jtNULL, jtBinaryCombo, jtRandomSplit, jtValidation}  JobTypes;

typedef  enum   {bjNULL, bjOpen, bjStarted, bjDone, bjExpanded}  bjBinaryJobStatus;


class   BinaryJobList;
typedef BinaryJobList* BinaryJobListPtr;


class  Processor;
typedef  Processor*  ProcessorPtr;



class  BinaryJob
{
public:

  typedef  BinaryJob*  BinaryJobPtr;

  BinaryJob (const BinaryJob&  j);


  BinaryJob (ProcessorPtr    _processor,
             int             _parentId,
             FeatureNumList  _features,
             int             _chgFeatureNum
            );


  BinaryJob (ProcessorPtr    _processor,
             int             _parentId,
             FeatureNumList  _features,
             int             _chgFeatureNum,
             int32           _numOfRounds,
             double          _cParm,
             double          _gammaParm,
             float           _aParm
            );

  BinaryJob (ProcessorPtr  _processor,
             KKStrParser   statusLine
            );

  BinaryJob (RunLog& _log,
             FileDescPtr  _fileDesc
            );

  virtual  
    ~BinaryJob ();


  static
    BinaryJobPtr  CreateDuplicateJob (BinaryJob&  j);

  static
    BinaryJobPtr  CreateTestJob (BinaryJobPtr  sourceJob);


  virtual  JobTypes  JobType ()  const;

  KKStr   JobTypeStr ()  const;


  //***************************************** Access Methods **********************************************

  float               Accuracy           () const {return accuracy;}
  float               AccuracyNorm       () const {return accuracyNorm;}
  float               AParm              () const {return aParm;}
  float               AvgPredProb        () const {return avgPredProb;}
  int                 ChgFeatureNum      () const {return chgFeatureNum;}
  double              CParm              () const {return cParm;}
  float               DecisiveProbMean   () const {return decisiveProbMean;}
  float               DecisiveProbStdDev () const {return decisiveProbStdDev;}
  float               DeltaProbAccuracy  () const;
  float               FMeasure           () const {return fMeasure;}
  double              GammaParm          () const {return gammaParm;}
  float               Grade              () const {return grade;}
  const
  FeatureNumList&     Features              () const {return features;}
  int                 JobId                 () const {return jobId;}
  int32               NumOfRounds        () const {return numOfRounds;}
  int                 ParentId              () const {return parentId;}
  ProcessorPtr        Processor             () const {return processor;}
  double              ProcessingTime        () const {return processingTime;}
  int                 RandomNum             () const {return randomNum;}
  bjBinaryJobStatus   Status                () const {return status;}
  KKStr               StatusStr             () const;
  float               TestAccuracy          () const {return testAccuracy;}
  float               TestAccuracyNorm      () const {return testAccuracyNorm;}
  float               TestAvgPredProb       () const {return testAvgPredProb;}
  float               TestDeltaProbAccuracy () const;
  float               TestGrade             () const {return testGrade;}
  float               TestFMeasure          () const {return testFMeasure;}
  int                 TestNumSVs            () const {return testNumSVs;}
  bool                ValidateOnly          () const {return validateOnly;}


  void   Accuracy           (float              _accuracy)          {accuracy         = _accuracy;}
  void   AccuracyNorm       (float              _accuracyNorm)      {accuracyNorm     = _accuracyNorm;}
  void   AParm              (float              _aParm)             {aParm            = _aParm;}
  void   AvgPredProb        (float              _avgPredProb)       {avgPredProb      = _avgPredProb;}
  void   CParm              (double             _cParm)             {cParm            = _cParm;}
  void   DecisiveProbMean   (float              _decisiveProbMean)  {decisiveProbMean = _decisiveProbMean;}
  void   DecisiveProbStdDev (float              _decisiveProbStdDev) {decisiveProbStdDev = _decisiveProbStdDev;}
  void   Features           (const FeatureNumList& _features)       {features         = _features;}
  void   FMeasure           (float              _fMeasure)          {fMeasure         = _fMeasure;}
  void   GammaParm          (double             _gammaParm)         {gammaParm        = _gammaParm;}
  void   Grade              (float              _grade)             {grade            = _grade;}
  void   JobId              (int                _jobId)             {jobId            = _jobId;}
  void   NumOfRounds        (int32              _numOfRounds)       {numOfRounds      = _numOfRounds;}
  void   ProcessingTime     (double             _processingTime)    {processingTime   = _processingTime;}
  void   Status             (bjBinaryJobStatus  _status)            {status           = _status;}
  void   TestAccuracy       (float              _testAccuracy)      {testAccuracy     = _testAccuracy;}
  void   TestAccuracyNorm   (float              _testAccuracyNorm)  {testAccuracyNorm = _testAccuracyNorm;}
  void   TestAvgPredProb    (float              _testAvgPredProb)   {testAvgPredProb  = _testAvgPredProb;}
  void   TestGrade          (float              _testGrade)         {testGrade        = _testGrade;}
  void   TestFMeasure       (float              _testFMeasure)      {testFMeasure     = _testFMeasure;}
  void   TestNumSVs         (int                _testNumSVs)        {testNumSVs       = _testNumSVs;}
  void   ValidateOnly       (bool               _validateOnly)      {validateOnly     = _validateOnly;}


  BinaryJobPtr        CheckTwoListsForExitingJob (FeatureNumList&   featuresToLookFor,
                                                  BinaryJobListPtr  exitingJobs1,
                                                  BinaryJobListPtr  exitingJobs2
                                                 );  
  
  virtual  void       EvaluateNode ();


  BinaryJobListPtr    ExpandBestCaseBackward (BinaryJobListPtr  exitingJobs1,
                                              BinaryJobListPtr  exitingJobs2
                                             );

  BinaryJobListPtr    ExpandBestCaseForward (BinaryJobListPtr  exitingJobs1,
                                             BinaryJobListPtr  exitingJobs2
                                            );

  BinaryJobListPtr    ExpandBestCaseForward  (BinaryJobListPtr  exitingJobs);

  int                 FeatureCount () const  {return features.NumOfFeatures ();}

  virtual
  void    ReFresh (BinaryJob&  j);


  virtual
  KKStr   ToStatusStr ();

  static  KKStr              BinaryJobStatusToStr   (bjBinaryJobStatus  status);
  static  bjBinaryJobStatus  BinaryJobStatusFromStr (KKStr  statusStr);


  double   CParm     () {return  cParm;}
  double   GammaParm () {return  gammaParm;}
  float    AParm     () {return  aParm;}

  static
    KKStr  ReportHeaderLine1 ();

  static
    KKStr  ReportHeaderLine2 ();

  static
    KKStr  ReportHeaderLineHTML ();


  KKStr  ReportLine ()  const;

  KKStr  ReportLineHTML ()  const;


protected:
  void   ProcessStatusStr (KKStrParser&  statusStr);


  void   ProcessStatusField (const KKStr&  fieldName,
                             const KKStr&  fieldValue
                            );



  float                accuracy;
  float                accuracyNorm;
  float                avgPredProb;         // Average predictited probability
  int                  chgFeatureNum;
  float                decisiveProbMean;
  float                decisiveProbStdDev;
  FeatureNumList       features;
  float                fMeasure;
  float                grade;
  RunLog&              log;
  int                  jobId;
  int                  parentId;
  double               processingTime;
  ProcessorPtr         processor;
  int                  randomNum;
  bjBinaryJobStatus    status;
  float                testAccuracy;
  float                testAccuracyNorm;
  float                testAvgPredProb;
  float                testFMeasure;
  float                testGrade;
  int                  testNumSVs;      // Number of Suport Vectors created during Test


  // next three are only used during parameter search
  double               cParm;
  double               gammaParm;
  float                aParm;
  int32                numOfRounds;

  bool                 validateOnly;
};




typedef  BinaryJob::BinaryJobPtr  BinaryJobPtr;





#endif
