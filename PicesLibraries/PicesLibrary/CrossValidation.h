#ifndef  _CROSSVALIDATION_
#define  _CROSSVALIDATION_

/**
 @class  MLL::CrossValidation
 @brief  A class that is meant to manage a n-Fold Cross Validation.
 @author  Kurt Kramer
 @details
 @code
 *********************************************************************
 *                           CrossValidation                         *
 *                                                                   *
 *                                                                   *
 *                                                                   *
 *-------------------------------------------------------------------*
 *  History                                                          *
 *                                                                   *
 *    Date     Programmer   Description                              *
 *  ---------- -----------  -----------------------------------------*
 *  2004       Kurt Kramer  Origanol Development.                    *
 *                                                                   *
 *                                                                   *
 *  2005-01-07 Kurt Kramer  Added classedCorrectly parameter to      *
 *                          CrossValidate.  If not null it should    *
 *                          point to an array of bool that has as    *
 *                          many elemenst as there are in the        *
 *                          testImages list.  Each element reprsents *
 *                          weather the coresponding element in      *
 *                          testImages was classified correctly.     *
 *                                                                   *
 *********************************************************************
 @endcode
 */


#include "BasicTypes.h"
#include "RunLog.h"

#include "BitReduction.h"

namespace  MLL  
{
  #ifndef  _FeatureVectorDefined_
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif


  #ifndef  _MLCLASS_
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass const  MLClassConst;
  typedef  MLClassConst*  MLClassConstPtr;
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif


  #if  !defined(_MLClassConstListDefined_)
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassConstListPtr;
  #endif


  #ifndef  _ConfussionMatrix2_
  class  ConfusionMatrix2;
  typedef  ConfusionMatrix2*  ConfusionMatrix2Ptr;
  #endif


  #ifndef  _TrainingConfiguration2Defined_
  class  TrainingConfiguration2;
  typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;
  #endif


  #ifndef  _FILEDESC_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  class  CrossValidation
  {
  public:
    CrossValidation (TrainingConfiguration2Ptr _config,
                     FeatureVectorListPtr      _examples,
                     MLClassConstListPtr    _mlClasses,
                     int32                     _numOfFolds,
                     bool                      _featuresAreAlreadyNormalized,
                     FileDescPtr               _fileDesc,
                     RunLog&                   _log,
                     bool&                     _cancelFlag
                    );

    ~CrossValidation ();
    
    void  RunCrossValidation ();

    void  RunValidationOnly (FeatureVectorListPtr validationData,
                             bool*                classedCorrectly = NULL
                            );

    const
    ConfusionMatrix2Ptr    ConfussionMatrix () const  {return  confusionMatrix;}

    float         Accuracy     ();
    float         AccuracyNorm ();
    int32         DuplicateTrainDataCount () const {return  duplicateTrainDataCount;}
    float         FoldAccuracy (int32 foldNum) const;

    void          NumOfFolds (int32 _numOfFolds)  {numOfFolds = _numOfFolds;}

    const
    VectorFloat&  FoldAccuracies          () const {return  foldAccuracies;}

    KKStr         FoldAccuracysToStr      () const;

    ConfusionMatrix2Ptr  GiveMeOwnershipOfConfusionMatrix ();

    int32         NumOfSupportVectors       () const {return  numSVs;}
    int32         NumSVs                    () const {return  numSVs;}
    int32         TotalNumSVs               () const {return  totalNumSVs;}

    int32         ReductionPreExampleCount  () const {return  reductionPreImageCount;}
    int32         ReductionPostExampleCount () const {return  reductionPostImageCount;}
    float         ReductionRatio            () const;
    int32         SupportPointsTotal        () const {return  numSVs;}
    
    const CompressionStats&  CompStats    ()  const  {return  compStats;}

    const VectorFloat&   Accuracies           () const {return foldAccuracies;}
    float                AccuracyMean         () const {return accuracyMean;}
    float                AccuracyStdDev       () const {return accuracyStdDev;}

    double               AvgPredProb          () const {return avgPredProb;}

    const VectorFloat&   ReductionRatios      () const {return reductionRatios;}
    float                ReductionRatioMean   () const {return reductionRatioMean;}
    float                ReductionRatioStdDev () const {return reductionRatioStdDev;}

    const VectorDouble&  ReductionTimes       () const {return reductionTimes;}
    double               ReductionTimeMean    () const {return reductionTimeMean;}
    double               ReductionTimeStdDev  () const {return reductionTimeStdDev;}
    double               ReductionTime        () const {return  reductionTime;}

    const VectorFloat&   SupportPoints        () const {return supportPoints;}
    double               SupportPointsMean    () const {return supportPointsMean;}
    double               SupportPointsStdDev  () const {return supportPointsStdDev;}

    const VectorDouble&  TestTimes       () const {return testTimes;}
    double               TestTimeMean    () const {return testTimeMean;}
    double               TestTimeStdDev  () const {return testTimeStdDev;}
    double               TestTimeTotal   () const {return testTime;}

    const VectorDouble&  TrainTimes      () const {return trainTimes;}
    double               TrainTimeMean   () const {return trainTimeMean;}
    double               TrainTimeStdDev () const {return trainTimeStdDev;}
    double               TrainTimeTotal  () const {return  trainingTime;}

  private:
    void  AllocateMemory ();

    void  CrossValidate (FeatureVectorListPtr   testImages, 
                         FeatureVectorListPtr   trainingImages,
                         int32                    foldNum,
                         bool*                  classedCorrectly = NULL
                        );

    void  DeleteAllocatedMemory ();

    //void  DistributesImagesRandomlyFromEachWithInFolds ();


    bool                      cancelFlag;
    TrainingConfiguration2Ptr config;
    int32                     duplicateTrainDataCount;
    bool                      featuresAreAlreadyNormalized;
    FileDescPtr               fileDesc;
    VectorFloat               foldAccuracies;
    VectorInt                 foldCounts;
    ConfusionMatrix2Ptr       confusionMatrix;
    ConfusionMatrix2Ptr*      cmByNumOfConflicts;
    FeatureVectorListPtr      examples;
    MLClassConstListPtr    mlClasses;
    int32                     imagesPerClass;
    RunLog&                   log;
    int32                     maxNumOfConflicts;  // Will indicate the number confusionMatrices created in
                                                  // table in cmByNumOfConflicts;
    int32                     numOfFolds;

    int32                     numSVs;               // Total Support Vectors Detected.

    int32                     totalNumSVs;          // This is different from 'numOfSupportVectors' it will reflect all the Support Vectors
                                                    // that are created in a Multi Class SVM.  That is if a given example is used in three
                                                    // different binary classifiers it will be counted three times.

    int32*                    numOfWinnersCounts;
    int32*                    numOfWinnersCorrects;
    int32*                    numOfWinnersOneOfTheWinners;

    double                    testTime;
    double                    reductionTime;
    int32                     reductionPreImageCount;
    int32                     reductionPostImageCount;
    double                    trainingTime;


    CompressionStats          compStats;   // Compression Stats returned form a Validation Run;

    float                     accuracyMean;
    float                     accuracyStdDev;

    double                    avgPredProb;
    double                    totalPredProb;

    float                     reductionRatioMean;
    float                     reductionRatioStdDev;
    VectorFloat               reductionRatios;

    double                    reductionTimeMean;
    double                    reductionTimeStdDev;
    VectorDouble              reductionTimes;

    float                     supportPointsMean;
    float                     supportPointsStdDev;
    VectorFloat               supportPoints;

    double                    testTimeMean;
    double                    testTimeStdDev;
    VectorDouble              testTimes;

    double                    trainTimeMean;
    double                    trainTimeStdDev;
    VectorDouble              trainTimes;

    bool                      weOwnConfusionMatrix;
  };


  typedef  CrossValidation*  CrossValidationPtr;

}  /* namespace  MLL */

#endif

