#ifndef  _CROSSVALIDATIONTHREEWAY_
#define  _CROSSVALIDATIONTHREEWAY_
//*******************************************************************
//* Written by: Kurt Kramer                                         *
//*        For: Research Work, Plankton recognition System          *
//*                                                                 *
//*-----------------------------------------------------------------*
//*                    CrossValidationThreeWay                      *
//*                                                                 *
//*   This is a special CrossValidation that is meant to help       *
//* determine a good feature set for a Binary classifier.  It will  *
//* use the two specified classes for training and testing while    *
//* using all other classes for testing only.                       *
//*                                                                 *
//*******************************************************************

#include "RunLog.h"


namespace MLL
{
  #ifndef  _ConfussionMatrix2_
  class  ConfusionMatrix2;
  typedef  ConfusionMatrix2*  ConfusionMatrix2Ptr;
  #endif


  #ifndef  _FeatureVectorDefined_
  class  FeatureVector;
  typedef  FeatureVector*  FeatureVectorPtr;
  #endif

  #ifndef  _FeatureVectorListDefined_
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif

  #ifndef  _CLASSSTATISTIC_
  class  ClassStatisticList;
  typedef  ClassStatisticList*  ClassStatisticListPtr;
  #endif


  #if  !defined(_MLClassDefined_)
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass const *  MLClassConstPtr;
  #endif

  #if  !defined(_MLClassListDefined_)
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif

  #if  !defined(_MLClassConstListDefined_)
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassConstListPtr;
  #endif


  #ifndef  _TrainingConfiguration2Defined_
  class  TrainingConfiguration2;
  typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;
  #endif

                                  
  class CrossValidationThreeWay
  {
  public:
    CrossValidationThreeWay (TrainingConfiguration2Ptr  _config,
                             bool                       useConfigAsIs,
                             FeatureVectorListPtr       _examples,
                             MLClassConstListPtr        _mlClasses,
                             MLClassConstPtr            _class1,
                             MLClassConstPtr            _class2,
                             int32                      _numOfFolds,
                             bool                       _featuresAlreadyNormalized,
                             bool                       _stratify,
                             float                      _punishmentFactor,
                             RunLog&                    _log
                            );

    ~CrossValidationThreeWay (void);

    float      Accuracy ()  {return accuracy;}

    KKStr     AccuracyStr ();

    void       CalculateThirdClassDecisiveProbability (float&  mean,
                                                       float&  stdDev
                                                      );


    //  You would use this when when you perform a ValidationOnly operation.
    //  because we will not know th etest data used at this time.
    void       CalculateThirdClassDecisiveProbability (FeatureVectorListPtr testData, 
                                                       float&               mean,
                                                       float&               stdDev
                                                      );

    void       CalculateAccuracyAndGrade ();

    double     ClassificationTime () const {return classificationTime;}

    void       CreateDistanceReport (ostream&  r);

    void       CreateProbabilityReport (ostream&  r);

    float      FoldAccuracy       (int32 foldNum);

    KKStr     FoldStr ()  const;

    float      Grade              () const {return grade;}

    float      NumOfSupportVectors() const {return  numOfSupportVectors;}

    void       RunValidation (void);

    void       RunValidationOnly (FeatureVectorListPtr  validationData,
                                  bool*                 classedCorrectly
                                 );

    double     TrainingTime       () const {return  trainingTime;}

  private:
    void  BuildOurClasses ();

    void  CrossValidate (FeatureVectorListPtr   testImages, 
                         FeatureVectorListPtr   trainingImages,
                         int32                    foldNum,
                         bool*                  classedCorrectly
                        );

    float                     accuracy;
    MLClassConstPtr        class1;
    MLClassConstPtr        class2;
    double                    classificationTime;
    TrainingConfiguration2Ptr config;
    ConfusionMatrix2Ptr       confusionMatrix;
    bool                      featuresAlreadyNormalized;
    float*                    foldAccuracies;
    float                     grade;
    MLClassConstListPtr    mlClasses;
    FeatureVectorListPtr      examples;
    RunLog                    log;
    double                    maxProb;
    double                    minProb;
    int32                     numClassedCorrectly;
    int32                     numInTwoClasses;
    int32                     numOfFolds;
    float                     numOfSupportVectors;
    MLClassConstListPtr    ourClasses;                   // Class list where class1, class2 are in front
    float                     punishmentFactor;
    bool                      stratify;
    int32                     thirdClassTotalDecisiveCount;
    float                     thirdClassTotalDecisiveProb;  // sum(0.5 - Imagefeatures.Probability ())
    double                    trainingTime;
    TrainingConfiguration2Ptr twoClassConfig;
    MLClassConstListPtr    twoClassList;
    bool                      weOwnTwoClassConfig;
  };

  typedef  CrossValidationThreeWay*  CrossValidationThreeWayPtr;

}  /* namespace MLL */


#endif
