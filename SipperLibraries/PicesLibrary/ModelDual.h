#ifndef  _MODELDUAL_
#define  _MODELDUAL_
/**
 *@class  MLL::ModelDual
 *@brief  Will implement the Dual Classifoer Model.  
 *@details This model will actually load two differenttraining models specified in the parameters 
 * field.  It wil utilize both classifiers for to make  a prediction.  Unknow examples will be
 * submitted to both classifiers.  The returned class will be the common part of the class hierarchy
 * of the two predictions.  If there is nothing in common between th etwo predictions then it wil 
 * return th e"Other class.
 */

#include "RunLog.h"
#include "Str.h"

#include "Model.h"


namespace  MLL  
{
  #if  !defined(_MLCLASS_)
    class  MLClass;
    typedef  MLClass*  MLClassPtr;
    typedef  MLClass const *  MLClassConstPtr;
    class  MLClassList;
    typedef  MLClassList*  MLClassListPtr;
  #endif

  #if  !defined(_FILEDESC_)
    class  FileDesc;
    typedef  FileDesc*  FileDescPtr;
  #endif

  #if  !defined(_FEATUREVECTOR_)
    class  FeatureVector;
    typedef  FeatureVectorPtr;
    class  FeatureVectorList;
    typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif

  #if  !defined(_FEATURENUMLIST_)
    class  FeatureNumList;
    typedef  FeatureNumList*  FeatureNumListPtr;
  #endif

  #if  !defined(_TRAININGPROCESS2_)
    class  TrainingProcess2;
    typedef  TrainingProcess2*  TrainingProcess2Ptr;
  #endif

  #if  !defined(_CLASSIFIER2_)
    class  Classifier2;
    typedef  Classifier2*  Classifier2Ptr;
  #endif

  #if  !defined(_MODELPARAMDUAL_)
    class  ModelParamDual;
    typedef  ModelParamDual*  ModelParamDualPtr;
  #endif

  #if  !defined(_TRAININGCONFIGURATION2_)
    class  TrainingConfiguration2;
    typedef  TrainingConfiguration2* TrainingConfiguration2Ptr;
  #endif


  class  ModelDual: public Model
  {
  public:

    typedef  ModelDual*  ModelDualPtr;


    ModelDual (FileDescPtr           _fileDesc,
               volatile const bool&  _cancelFlag,
               RunLog&               _log
              );

    ModelDual (const KKStr&          _name,
               const ModelParamDual& _param,         // Create new model from
               FileDescPtr           _fileDesc,
               volatile const bool&  _cancelFlag,
               RunLog&               _log
              );
  
    ModelDual (const ModelDual&   _model);

    virtual
    ~ModelDual ();

    virtual
    int32                 MemoryConsumedEstimated ()  const;

    virtual
    KKStr                 Description ()  const;  /**< Return short user readable description of model. */

    virtual ModelTypes    ModelType () const  {return mtDual;}

    virtual
    int32                 NumOfSupportVectors () const;

    TrainingProcess2Ptr   Trainer1 ()  {return trainer1;}

    TrainingProcess2Ptr   Trainer2 ()  {return trainer2;}

    virtual
    ModelDualPtr          Duplicate ()  const;

    ModelParamDualPtr     Param ();

    virtual
    MLClassConstPtr    Predict (FeatureVectorPtr  image);
  
    virtual
    void                  Predict (FeatureVectorPtr     example,
                                   MLClassConstPtr   knownClass,
                                   MLClassConstPtr&  predClass1,
                                   MLClassConstPtr&  predClass2,
                                   int32&               predClass1Votes,
                                   int32&               predClass2Votes,
                                   double&              probOfKnownClass,
                                   double&              predClass1Prob,
                                   double&              predClass2Prob,
                                   double&              compact,
                                   int32&               numOfWinners,
                                   bool&                knownClassOneOfTheWinners,
                                   double&              breakTie
                                  );

    virtual 
    ClassProbListPtr  ProbabilitiesByClass (FeatureVectorPtr  example);


    virtual
    void  ProbabilitiesByClassDual (FeatureVectorPtr   example,
                                    KKStr&             classifier1Desc,
                                    KKStr&             classifier2Desc,
                                    ClassProbListPtr&  classifier1Results,
                                    ClassProbListPtr&  classifier2Results
                                   );

    virtual
    void  ProbabilitiesByClass (FeatureVectorPtr            example,
                                const MLClassConstList&  _mlClasses,
                                int32*                      _votes,
                                double*                     _probabilities
                               );

    /**
     *@brief Derives predicted probabilities by class.
     *@details Will get the probabilities assigned to each class by the classifier.  The 
     *         '_mlClasses' parameter dictates the order of the classes. That is the 
     *         probabilities for any goven index in '_probabilities' will be for the class
     *         specifid in the same index in '_mlClasses'.
     *@param[in]  _example       FeatureVector object to calculate predivcted probabilities for.
     *@param[in]  _mlClasses  List image classes that caller is aware of.  This should be the
     *            same list that was used when consttructing this Model object.  The list must 
     *            be the same but not nessasarily in the same order as when Model was 1st 
     *            constructed.
     *@param[out] _probabilities An array that must be as big as the number of classes as in
     *            mlClasses.  The probability of class in mlClasses[x] will be returned 
     *            in probabilities[x].
    */
    virtual
    void  ProbabilitiesByClass (FeatureVectorPtr            _example,
                                const MLClassConstList&  _mlClasses,
                                double*                     _probabilities
                               );
  
    virtual  void  ReadSpecificImplementationXML (istream&  i,
                                                  bool&     _successful
                                                 );


    virtual  
    void  RetrieveCrossProbTable (MLClassConstList&  classes,
                                  double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                 );



    virtual  void  TrainModel (FeatureVectorListPtr  _trainExamples,
                               bool                  _alreadyNormalized,
                               bool                  _takeOwnership  /*!< Model will take ownership of these examples */
                              );


    virtual  void  WriteSpecificImplementationXML (ostream&  o);



  protected:
    void  DeleteExistingClassifiers ();

    MLClassConstPtr  ReconcilePredictions (MLClassConstPtr  pred1, 
                                              MLClassConstPtr  pred2
                                             );

    void  ReconcileProbAndVotes (Classifier2Ptr      classifier,
                                 MLClassConstPtr  predClass,
                                 FeatureVectorPtr    encodedExample, 
                                 double&             predClassProb,
                                 int32&              predClassVotes
                                );

    TrainingConfiguration2Ptr  config1;
    TrainingConfiguration2Ptr  config2;

    TrainingProcess2Ptr        trainer1;
    TrainingProcess2Ptr        trainer2;

    Classifier2Ptr             classifier1;
    Classifier2Ptr             classifier2;

    KKStr                      trainer1StatusMsg;
    KKStr                      trainer2StatusMsg;

    ModelParamDualPtr          param;   /*!<   We will NOT own this instance. It will point to same instance defined in parent class Model.  */
  };  /* ModelDual */



typedef  ModelDual::ModelDualPtr  ModelDualPtr;

} /* namespace  MLL */

#endif
