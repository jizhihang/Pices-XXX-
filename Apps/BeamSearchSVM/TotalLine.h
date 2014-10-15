#ifndef   _TOTALLINE_
#define   _TOTALLINE_

#include  "BasicTypes.h"
#include  "ResultLine.h"

#include  "CrossValidation.h"
#include  "FeatureNumList.h"
#include  "ImageClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"
#include  "TrainingConfiguration.h"

typedef  vector<FFLOAT>  FFLOATVector;


class  TotalLine
{
public:
   TotalLine (FileDescPtr               _fileDesc,
              ImageClassListPtr         _classes,
              RunLog&                   _log,
              TrainingConfigurationPtr  _config,
              ImageFeaturesListPtr      _testData,
              ImageFeaturesListPtr      _validationData,
              bool                      _useGrading
             );

  ~TotalLine ();
  
  FFLOAT   AccuracyByNumOfFeatuers (int  numOfFeatures);
  
  void     CalcFeatureChangeStats (int       featureNum,
                                   FFLOAT&   mean,
                                   FFLOAT&   stdDev
                                  );
  
  const
  ImageClassListPtr  Classes () const {return  classes;}

  const
  ImageClassPtr  Class1 () const;

  const
  ImageClassPtr  Class2 () const;


  const
  KKStr&  Class1Name ()   
    {
      if  (classes->QueueSize () > 0)
        return  classes->IdxToPtr (0)->Name ();
      else
        return  emptyStr;
    }

  const
  KKStr&  Class2Name ()
    {
      if  (classes->QueueSize () > 1)
        return  classes->IdxToPtr (1)->Name ();
      else
        return  emptyStr;
    }


  int   BestFeatureSetFoundFeatureCount ()  const;


  FeatureNumList  BestFeatureSetFound ()  const;

  float  GradeByNumOfFeatuers (int  numOfFeatures) const;

  FeatureNumList  SameAsBestFeatureSetFound (int  upCount) const;


  FeatureNumList  FeatureForBestAccurcayFoundFeatureCount (int  featureCount) const;


  void     CrossValidationsForTop200 (ostream&  o);

  const 
    ResultLinePtr JobWithHighestAccuracy (int featureCount)  const;


  int      NumOfResults ()  const   {return  numOfResults;}

  ResultLinePtr  ResultWithHighestAccuracy ()  const;

  void     PrintHighestAccuraciesByNumOfFeatures (ostream&  o,
                                                  int       upOneCount,
                                                  ostream&  sumResults
                                                  );


  void     FeatureForBestAccurcayFoundFeatureCount ();

  void     PrintHighestGradesByNumOfFeatures (ostream&       o,
                                              ImageClassPtr  class1,
                                              ImageClassPtr  class2,
                                              float          punishmentFactor,
                                              int            upOneCount
                                             );

  void     StatisticallySameJob (int                  _featureCount,
                                 const ResultLinePtr  _statisticallySameJob
                                );

  ResultLinePtr  StatisticallySameJob () const  {return  statisticallySameJob;}

  void     UpdateTotals (int            placeFromTop,
                         ResultLinePtr  resultLine
                        );

  void     UpdateTotalsByGrades (int            placeFromTop,
                                 ResultLinePtr  resultLine
                                );

  int      UssageInTop100 (int x);

  int      UssageInTop200 (int x);

  int      UssageInTop100Grades (int featureNum);

  int      UssageInTop200Grades (int featureNum);


private:
  FeatureNumList  FeatureNumListFromTop200 (int  minNumOfApperances);


  double**  PerformMcNemarsTest (ostream&              o,
                                 bool**                classedCorrectly,
                                 ImageFeaturesListPtr  testDataInOurClasses,
                                 ImageFeaturesListPtr  validationDataInOurClasses
                                );

  float**  PerformTTest (ostream&              o,
                         float**               foldAccuracies,
                         ImageFeaturesListPtr  testDataInOurClasses
                        );



  FFLOAT*                   accuracyByNumOfFeatuers;
  FileDescPtr               fileDesc;
  int                       maxNumOfFeatures;
  FFLOAT*                   gradesByNumOfFeatuers;
  FFLOATVector*             chgInAccuracies;
  ImageClassListPtr         classes;
  TrainingConfigurationPtr  config;
  ResultLinePtr*            jobsWithHighestAccuracy;
  ResultLinePtr*            jobsWithHighestGrade;
  int                       numOfResults;
  RunLog&                   log;
  ResultLinePtr             statisticallySameJob;
  int                       featureCountWithStatisticlySame;
  bool                      useGrading;
  int*                      ussageInTop100;
  int*                      ussageInTop200;
  int*                      ussageInTop100Grades;
  int*                      ussageInTop200Grades;

  ImageFeaturesListPtr      testDataInOurClasses;
  ImageFeaturesListPtr      validationDataInOurClasses;

  static  
  KKStr             emptyStr;
};  /* TotalLine */


typedef  TotalLine*  TotalLinePtr;

typedef  KKQueue<TotalLine>  TotalLineList;

#define  NumOfValidationFolds  10

#endif
