#ifndef  _MODELSVMBASE_
#define  _MODELSVMBASE_
//***********************************************************************
//*                           ModelSvmBase                              *
//*                                                                     *
//*  This will be the base model for implementations that utilize       *
//*  the Support Vector Machine.                                        *
//***********************************************************************


#include "RunLog.h"
#include "Str.h"

#include "Model.h"
#include "ModelParam.h"
#include "ModelParamSvmBase.h"
#include "svm2.h"


namespace  MLL  
{
  class  ModelSvmBase: public Model
  {
  public:

    typedef  ModelSvmBase*  ModelSvmBasePtr;


    ModelSvmBase (FileDescPtr           _fileDesc,
                  volatile const bool&  _cancelFlag,
                  RunLog&               _log
                );

    ModelSvmBase (const KKStr&             _name,
                  const ModelParamSvmBase& _param,         // Create new model from
                  FileDescPtr              _fileDesc,
                  volatile const bool&     _cancelFlag,
                  RunLog&                  _log
                 );
  
    ModelSvmBase (const ModelSvmBase&   _model);

    virtual
    ~ModelSvmBase ();

    virtual
    KKStr                 Description ()  const;

    virtual
    ModelSvmBasePtr       Duplicate ()  const;

    virtual
    int32                 MemoryConsumedEstimated ()  const;

    virtual ModelTypes    ModelType () const  {return mtSvmBase;}

    virtual
    int32                 NumOfSupportVectors () const;

    ModelParamSvmBasePtr  Param ();

    virtual
    MLClassConstPtr    Predict (FeatureVectorPtr  image);
  
    virtual
    void                  Predict (FeatureVectorPtr  example,
                                   MLClassConstPtr   knownClass,
                                   MLClassConstPtr&  predClass1,
                                   MLClassConstPtr&  predClass2,
                                   int32&            predClass1Votes,
                                   int32&            predClass2Votes,
                                   double&           probOfKnownClass,
                                   double&           predClass1Prob,
                                   double&           predClass2Prob,
                                   double&           compact,
                                   int32&            numOfWinners,
                                   bool&             knownClassOneOfTheWinners,
                                   double&           breakTie
                                  );


    virtual 
    ClassProbListPtr  ProbabilitiesByClass (FeatureVectorPtr  example);



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
    SVM289_MFS::svm_model*  svmModel;
    ModelParamSvmBasePtr    param;   /*!<   We will NOT own this instance. It will point to same instance defined in parent class Model.  */
  };  /* ModelSvmBase */



typedef  ModelSvmBase::ModelSvmBasePtr  ModelSvmBasePtr;

} /* namespace  MLL */

#endif
