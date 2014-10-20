#ifndef  _CLASSIFIER2_
#define  _CLASSIFIER2_

#include "RunLog.h"

#include "Application.h"
#include "Model.h"
#include "ModelOldSVM.h"
#include "ModelParamOldSVM.h"
#include "ModelParam.h"
#include "SVMModel.h"


namespace  MLL
{
#if  !defined(_CLASSPROB_)
class  ClassProb;
typedef  ClassProb*  ClassProbPtr;
class  ClassProbList;
typedef  ClassProbList*  ClassProbListPtr;
#endif


#ifndef  _FeatureVectorDefined_
class  FeatureVector;
typedef  FeatureVector*  FeatureVectorPtr;
#endif

#ifndef  _FeatureVectorListDefined_
class  FeatureVectorList;
typedef  FeatureVectorList*  FeatureVectorListPtr;
#endif


#ifndef  _MLCLASS_
class  MLClass;
typedef  MLClass*  MLClassPtr;
typedef  MLClass const *  MLClassConstPtr;

class  MLClassList;
typedef  MLClassList*  MLClassListPtr;
#endif


#if  !defined(_MLClassConstListDefined_)
class  MLClassConstList;
typedef  MLClassConstList*  MLClassConstListPtr;
#endif


#ifndef  _TRAININGCONFIGURATION2_
class  TrainingConfiguration2;
typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;
#endif


#ifndef  _TRAININGPROCESS2_
class  TrainingProcess2;
typedef  TrainingProcess2*  TrainingProcess2Ptr;
#endif



  class  Classifier2
  {
  public:
    Classifier2 (TrainingProcess2Ptr  _trainer,
                 RunLog&              _log
                );

    virtual  ~Classifier2 ();

    SVM_SelectionMethod   SelectionMethod ()  const;

    bool                 Abort ()  {return abort;}

    MLClassConstPtr   ClassifyAImage    (FeatureVector&  example);

    void  ClassifyAImage (FeatureVector&       example,
                          MLClassConstPtr&  predClass1,
                          MLClassConstPtr&  predClass2,
                          int32&               predClass1Votes,
                          int32&               predClass2Votes,
                          double&              knownClassProb,
                          double&              predClass1Prob,
                          double&              predClass2Prob,
                          int32&               numOfWinners,
                          double&              breakTie,
                          double&              compact
                         );
 
    MLClassConstPtr   ClassifyAImage (FeatureVector&  example,
                                         int32&          numOfWinners,
                                         bool&           knownClassOneOfTheWinners
                                        );

    MLClassConstPtr   ClassifyAImage (FeatureVector&  example,
                                         double&         probability,
                                         int32&          numOfWinners,
                                         bool&           knownClassOneOfTheWinners,
                                         double&         breakTie
                                        );

    /**
     *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
     *@details  This method will iterate through all the S/V's removing them one at a 
     *          time and recompute the decision boundary and probability.  It will then
     *          return the S/V's that when removed improve the probability in 'c1's 
     *          the most.
     *@param[in]  example  Example that was classified incorrectly.
     *@param[in]  numToFind  The number of the worst examples you are looking for.
     *@param[in]  c1  Class that the 'example; parameter should have been classed as.
     *@param[in]  c2  Class that it was classified as.
     */
    vector<ProbNamePair>  FindWorstSupportVectors (FeatureVectorPtr    example,
                                                   int32               numToFind,
                                                   MLClassConstPtr  c1,
                                                   MLClassConstPtr  c2
                                                  );



    /**
     *@brief  For a given two class pair return the names of the 'numToFind' worst S/V's.
     *@details  This method will iterate through all the S/V's removing them one at a 
     *          time and rebuild a new SVM then submit example for testing.
     *@param[in]  example  Example that was classified incorrectly.
     *@param[in]  numToFind  The number of the worst examples you are looking for.
     *@param[in]  c1  Class that the 'example; parameter should have been classed as.
     *@param[in]  c2  Class that it was classified as.
     */
    vector<ProbNamePair>  FindWorstSupportVectors2 (FeatureVectorPtr    example,
                                                    int32               numToFind,
                                                    MLClassConstPtr  c1,
                                                    MLClassConstPtr  c2
                                                   );


    virtual
    int32   MemoryConsumedEstimated ()  const;

    /**
     *@brief  For a given feature vector return back the probabilities and votes for each class.
     *@details
     *@param classes       [in]  List of classes that we can be predicted for  The ordering of 'votes' and 'probabilities' will be dictated by this list.
     *@param example       [in]  Feature Vector to make prediction on.
     *@param votes         [out] Pointer to list of ints,  must be as large as 'classes'  The number of votes for each corresponding class will be stored hear.
     *@param probabilities [out] Pointer to list of double's,  must be as large as 'classes'  The probability for each corresponding class will be stored hear.
     */
    void                 ProbabilitiesByClass (const MLClassConstList&  classes,
                                               FeatureVectorPtr            example,
                                               int32*                      votes,
                                               double*                     probabilities
                                              );

    
    ClassProbListPtr     ProbabilitiesByClass (FeatureVectorPtr  example);


    void                 ProbabilitiesByClassDual (FeatureVectorPtr   example,
                                                   KKStr&             classifier1Desc,
                                                   KKStr&             classifier2Desc,
                                                   ClassProbListPtr&  classifier1Results,
                                                   ClassProbListPtr&  classifier2Results
                                                  );


    void                 RetrieveCrossProbTable (MLClassConstList&  classes,
                                                 double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                                );


    vector<KKStr>        SupportVectorNames (MLClassConstPtr   c1,
                                             MLClassConstPtr   c2
                                            );



  private:
    MLClassConstPtr  ClassifyAImageOneLevel (FeatureVector&  example);
 
    MLClassConstPtr  ClassifyAImageOneLevel (FeatureVector&  example,
                                                int32&          numOfWinners,
                                                bool&           knownClassOneOfTheWinners
                                               );


    MLClassConstPtr  ClassifyAImageOneLevel (FeatureVector&  example,
                                                double&         probability,
                                                int32&          numOfWinners, 
                                                bool&           knownClassOneOfTheWinners,
                                                double&         breakTie
                                               );


    //************************************************************
    // Variables that are Global to Classifier2 application. *
    //************************************************************

    bool                   abort;

    bool                   featuresAlreadyNormalized;

    MLClassConstListPtr    mlClasses;       /**< We will own the MLClass objects in this
                                             *   list.  Will be originally populated by
                                             *   TrainingConfiguration2 construction.
                                             */
    RunLog&                log;
  
    MLClassConstPtr        noiseMLClass;    /**< Point to class that represents Noise Images
                                                *  The object pointed to will also be included 
                                                *  in mlClasses.
                                                */
    TrainingProcess2Ptr    trainingProcess;   

    ModelPtr               trainedModel;

    ModelOldSVMPtr         trainedModelOldSVM;

    SVMModelPtr            trainedModelSVMModel;

    MLClassConstPtr        unKnownMLClass;
  };
  typedef  Classifier2*  Classifier2Ptr;
}  /* namespace  MLL */


#define  _Classifier2Defined_

#endif
