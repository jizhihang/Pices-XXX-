#ifndef  SINGLERUN
#define  SINGLERUN


#include  <iomanip>
#include  <iostream>
#include  <fstream>

#ifdef  WIN32
#include  <ostream>
#endif

using namespace std;


#include  <time.h>

#include  "BasicTypes.h"
#include  "Str.h"
#include  "RunLog.h"
#include  "FeatureNumList.h"
#include  "ImageFeatures.h"
#include  "ImageClass.h"
#include  "ConfussionMatrix2.h"
#include  "TrainingConfiguration.h"

#define  MAXaccuracyHistory  5
#define  SingleRunCurVersion 1

typedef  enum  {faNULL, faAddFeature, faDeleteFeature, faSpecifyFeatures}  FeatureAction;


typedef  enum  {SVM, C45}  Algorithm;

class  SingleRunList;

typedef  SingleRunList*  SingleRunListPtr;



class  SingleRun 
{

public:
  SingleRun (FileDescPtr    _fileDesc,
             RunLog&        _log,
             int            _jobId,
             int            _parentJobId,
             Algorithm      _algorithm
            );



  SingleRun (FileDescPtr     _fileDesc,
             RunLog&         _log,
             KKStr           _desc,  //  Creates a Single run using string from status file.
             Algorithm       _algorithm,
             SingleRunList&  _existingJobs
            );  



  SingleRun (const SingleRun&  singleRun);

  ~SingleRun ();

  void    AddFeatureNum (int  featureNum);

  void    BuildSubSets (SingleRunListPtr  singleRuns,
                        int&              nextJobId,
                        int&              numJobsSelected,
                        SingleRunListPtr  doneJobs
                       );


  void    BuildSubSetsOut (SingleRunListPtr   singleRuns,
                           int&               nextJobId,
                           int&               numJobsSelected,
                           FeatureNumListPtr  initialFeatures,
                           SingleRunListPtr   doneJobs
                          );


  void    RunCrossValidation (ImageFeaturesList&        images,
                              ImageClassList&           imageClasses,
                              TrainingConfigurationPtr  config,
                              int                       numOfFolds,
                              ostream*                  errorFile
                             );


  void    RunCrossValidation (ImageFeaturesList&        images,
                              ImageFeaturesList&        validationData,
                              ImageClassList&           imageClasses,
                              TrainingConfigurationPtr  config,
                              ostream*                  errorFile
                             );



  void    Accuracy (double _accuracy);
  double  Accuracy ()      const        {return  accuracy;}

  double  AccuracyHistory (int idx);


  FeatureAction  ChgFeatureAction ()  const  {return chgFeatureAction;}

  void    ChgFeatureAction (FeatureAction  _chgFeatureAction)  {chgFeatureAction = _chgFeatureAction;}
 
  void    ChgFeatureNum (int            _chgFeatureNum,
                         FeatureAction  _chgFeatureAction
                        );

  int     ChgFeatureNum () const          {return   chgFeatureNum;}

  double  ChgInAccuracy ()  const;

  void    CurStatus    (char  _curStatus);
  char    CurStatus    ()  const          {return  curStatus;}

  int     FeatureCount ()  const   {return  numOfFeatures;}

  void    FeatureNums (KKStr  featureNumStr);

  void    FeatureNums (const FeatureNumList&  _featureNums);

  void    FeatureNums (const BitString&       _featureNums);

  KKStr  FeaturesIncluded ()  const;

  KKStr  FeaturesExcluded (int maxFeatureNum)  const;


  //const 
  //FeatureNumList&  FeatureNums () const   {return  featureNums;}
  //FeatureNumList    FeatureNum () const   {return FeatureNumList (fileDesc, featureNums);}

  const BitString&  FeatureNums () const   {return featureNums;}

  int     JobId ()         const           {return  jobId;}

  int     ParentJobId ()  const            {return  parentJobId;}

  ConfussionMatrix2Ptr  Results ()  const  {return  results;}
  void                  Results (ConfussionMatrix2Ptr  _results)  {results = _results;}

  KKStr  RunStatusStr ();

  void    SetFeatureNums (const FeatureNumList&  _featureNums);

  double  SupportPoints () const {return supportPoints;}
  void    SupportPoints (double _supportPoints)  {supportPoints = _supportPoints;}

  double  TestTimeMean () const {return testTimeMean;}
  void    TestTimeMean (double _testTime) {testTimeMean = _testTime;}

  double  TestTimeTotal () const {return testTimeTotal;}
  void    TestTimeTotal (double _testTimeTotal) {testTimeTotal = _testTimeTotal;}

  double  TrainTimeMean () const {return trainTimeMean;}
  void    TrainTimeMean (double _trainTimeMean) {trainTimeMean = _trainTimeMean;}

  double  TrainTimeTotal () const {return trainTimeTotal;}
  void    TrainTimeTotal (double _trainTimeTotal) {trainTimeTotal = _trainTimeTotal;}

  bool    Updated ()  const       {return updated;}
  void    Updated (bool _updated) {updated = _updated;} 

  void    UpdateFromJob (const SingleRun&  j);


  static
  void  DisplayLeftOvers ();

  void  DeleteResults ();

private:
  void  PerformC45Algorithm (ImageClassList&       imageClasses,
                             ImageFeaturesListPtr  trainingImages, 
                             ImageFeaturesListPtr  testImages,
                             int                   foldNum,
                             int&                  correct,
                             int&                  total
                            );


  KKStr  HistoryToStr ();


  double         accuracy;

  double         accuracyHistory[MAXaccuracyHistory];  // Accuracy of parents

  Algorithm      algorithm;

  bool           cancelFlag;

  char           curStatus; //  '0' - Scheduled.
                            //  '1' - Selecfted for Processing.
                            //  '2' - Processed.
                            //  '3' - Has been searched already, or  search has started.
                            //        Used for BestFirstSearch.
                            //  '4' - Flagged as Don't Process.  Probably because
                            //        Termination condition for whole run was reaced.

  //FeatureNumList featureNums;
  BitString      featureNums;

  FileDescPtr    fileDesc;

  int            jobId;

  RunLog&        log;

  int            chgFeatureNum; // The number of the feature that was added or subtracted
                                // during Feature Search. 

  FeatureAction  chgFeatureAction;


  int            maxNumOfFeatures;

  uint           numOfFeatures;

  int            numOfParents;  // Works with accuracyHistory; indicates how many
                                // parents there were.

  int            parentJobId;

  ConfussionMatrix2Ptr  results;  // Will not be stored in status file,  juist used temporily
                                  // when we run a Cross Validation.  Will be deleted when we 
                                  // are deleted.,  meaning we own it.


  double         supportPoints;

  time_t         startTime;

  double         testTimeMean;
  double         testTimeTotal;
  double         trainTimeMean;
  double         trainTimeTotal;

  bool           updated;

  uchar          version;

  friend class  SingleRunList;
};



typedef  SingleRun*  SingleRunPtr;

KKStr         FeatureActionToStr   (FeatureAction  fa);
FeatureAction  FeatureActionFromStr (const KKStr&  faStr);




#endif

