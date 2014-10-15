#if  !defined(_MODELOLDSVM_)
#define  _MODELOLDSVM_


/**
 *@class MLL::ModelOldSVM
 *@browse
 *@details
 *@code
 ************************************************************************
 **                           ModelOldSVM                               *
 **                                                                     *
 **  This is a specialization of 'Model'.  It is meant to Wrap the      *
 **  original version of 'SvmModel'.  This class will allow us to use   *
 **  the original implementation using version 2.39 of LibSVM.          *
 **                                                                     *
 ************************************************************************
 *@endcode
 */


#include  "RunLog.h"
#include  "Str.h"

#include  "svm.h"
#include  "Model.h"
#include  "SVMModel.h"


namespace MLL
{
  #if  !defined(_CLASSASSIGNMENTS_)
  class  ClassAssignments;
  typedef  ClassAssignments*  ClassAssignmentsPtr;
  #endif


  #if  !defined(_MODELPARAMOLDSVM_)
  class  ModelParamOldSVM;
  typedef  ModelParamOldSVM*  ModelParamOldSVMPtr;
  #endif


  class  ModelOldSVM:  public Model
  {
  public:
    typedef  ModelOldSVM*  ModelOldSVMPtr;

    ModelOldSVM (FileDescPtr           _fileDesc,
                 volatile const bool&  _cancelFlag,
                 RunLog&               _log
                );

    ModelOldSVM (const KKStr&            _name,
                 const ModelParamOldSVM& _param,         // Create new model from
                 FileDescPtr             _fileDesc,
                 volatile const bool&    _cancelFlag,
                 RunLog&                 _log
                );
  
    ModelOldSVM (const ModelOldSVM&   _madel);
  
    virtual ~ModelOldSVM ();

    virtual
    ModelOldSVMPtr  Duplicate ()  const;

    virtual ModelTypes       ModelType () const  {return mtOldSVM;}

    const ClassAssignments&  Assignments () const;

    virtual
    KKStr                    Description ()  const;  /**< Return short user readable description of model. */

    const FeatureNumList&    GetFeatureNums ()  const;

    FeatureNumList           GetFeatureNums (MLClassConstPtr  class1,
                                             MLClassConstPtr  class2
                                            );

    virtual
    KKU::int32               MemoryConsumedEstimated ()  const;

    virtual
    bool                     NormalizeNominalAttributes ()  const;

    int32                    NumOfSupportVectors  () const;

    void                     SupportVectorStatistics (int32&  numSVs,
                                                      int32&  totalNumSVs
                                                     );

    ModelParamOldSVMPtr      Param                () const;

    SVM_SelectionMethod      SelectionMethod      () const;

    SVMModelPtr              SvmModel             () const {return svmModel;}


    virtual
    MLClassConstPtr       Predict (FeatureVectorPtr  image);
  
    virtual
    void               Predict (FeatureVectorPtr     example,
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
    void  ProbabilitiesByClass (FeatureVectorPtr            example,
                                const MLClassConstList&  _mlClasses,
                                int32*                      _votes,
                                double*                    _probabilities
                               );


    virtual
    void  ProbabilitiesByClass (FeatureVectorPtr            _example,
                                const MLClassConstList&  _mlClasses,
                                double*                     _probabilities
                               );


    vector<ProbNamePair>  FindWorstSupportVectors (FeatureVectorPtr    example,
                                                   int32               numToFind,
                                                   MLClassConstPtr  c1,
                                                   MLClassConstPtr  c2
                                                  );


    vector<ProbNamePair>  FindWorstSupportVectors2 (FeatureVectorPtr    example,
                                                    int32               numToFind,
                                                    MLClassConstPtr  c1,
                                                    MLClassConstPtr  c2
                                                   );


    virtual
    FeatureVectorPtr   PrepExampleForPrediction (FeatureVectorPtr  fv,
                                                 bool&             newExampleCreated
                                                );    
    
    vector<KKStr>  SupportVectorNames ()  const;


    vector<KKStr>  SupportVectorNames (MLClassConstPtr  c1,
                                       MLClassConstPtr  c2
                                      )  const;


    void  RetrieveCrossProbTable (MLClassConstList&  classes,
                                  double**              crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                 );


    virtual  void  ReadSpecificImplementationXML (istream&  i,
                                                  bool&     _successful
                                                 );

    virtual  void  TrainModel (FeatureVectorListPtr  _trainExamples,
                               bool                  _alreadyNormalized,
                               bool                  _takeOwnership  /*!< Model will take ownership of these examples */
                              );


    virtual  void  WriteSpecificImplementationXML (ostream&  o);



  private:
    ClassAssignmentsPtr  assignments;
    SVMModelPtr          svmModel;
  };


  typedef  ModelOldSVM::ModelOldSVMPtr  ModelOldSVMPtr;
} /* namespace MLL */



#endif

