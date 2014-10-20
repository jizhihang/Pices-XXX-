#ifndef  _MODEL_
#define  _MODEL_
/**
 @class  MLL::Model
 @brief Base class to all Learning Algorithms.
 @author  Kurt Kramer
 @details
   Base class to be used by all Learning Algorithm Models. The idea is that all learning algorithms 
   all follow the same basic flow.  The two obvious functions that they all support are Training from 
   supplied labeled examples(List of FeatureVector objects),  Prediction of an unlabeled example.
 */

#include "BasicTypes.h"
#include "RunLog.h"
#include "Str.h"

#include "BitReduction.h"
#include "ModelParam.h"


namespace MLL
{
  #if  !defined(_CLASSPROB_)
  class  ClassProb;
  typedef  ClassProb*  ClassProbPtr;
  class  ClassProbList;
  typedef  ClassProbList*  ClassProbListPtr;
  #endif

  #if  !defined(_FEATUREENCODER2_)
  class  FeatureEncoder2;
  typedef  FeatureEncoder2*  FeatureEncoder2Ptr;
  #endif


  #ifndef  _FeatureNumListDefined_
  class  FeatureNumList;
  typedef  FeatureNumList*  FeatureNumListPtr;
  #endif


  #if  !defined(_FEATUREVECTOR_)
  class  FeatureVector;
  typedef  FeatureVector*  FeatureVectorPtr;
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif


  #if  !defined(_FileDescDefined_)
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  #if  !defined(_MLCLASS_)
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass const *  MLClassConstPtr;
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassListPtr;
  #endif

  #if  !defined(_MLClassConstListDefined_)
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassConstListPtr;
  #endif


  #if  !defined(_NORMALIZATIONPARMS_)
  class  NormalizationParms;
  typedef  NormalizationParms*  NormalizationParmsPtr;
  #endif



  class  Model
  {
  public:
    typedef  Model*  ModelPtr;

    enum  ModelTypes {mtNULL = 0, mtOldSVM = 1, mtSvmBase = 2 , mtKNN= 3, mtUsfCasCor = 4, mtDual = 5};
    static KKStr       ModelTypeToStr   (ModelTypes    _modelingType);
    static ModelTypes  ModelTypeFromStr (const KKStr&  _modelingTypeStr);

 
    /**
     *@brief  Use this when you are planning on creating a empty model without parameters.
     */
    Model (FileDescPtr           _fileDesc,
           volatile const bool&  _cancelFlag,
           RunLog&               _log
          );


    /**
     *@brief  Construct a instance of 'Model' using the parameters specified in '_param'.
     *@param[in]  _param  Parameters for learning algorithm; we will create a duplicate copy.
     *@param[in]  _fileDesc Description of the dataset that will be used to train the classifier and examples that will be classified.
     *@param[in]  _cancelFlag  Will monitor; if at any point it turns true this instance is to terminate and return to caller.
     *@param[in,out]  _log  Logging file.
     */
    Model (const KKStr&           _name,
           const ModelParam&      _param,         // Create new model from
           FileDescPtr            _fileDesc,
           volatile  const bool&  _cancelFlag,
           RunLog&                _log
          );

  
    /**
     *@brief Copy Constructor.
     */
    Model (const Model&   _madel);

    virtual  ~Model ();


    /**
     *@brief  A factory method that will instantiate the appropriate class of training model based off '_modelType'.
     *@details  This method is used to construct a model that is going to be built from training data.
     *@param[in] _modelType  Type of model to be created; ex: mtOldSVM, mtSvmBase, or  mtKNN.
     *@param[in] _param  Parameters used to drive the creating of the model.
     *@param[in] _fileDesc Description of the dataset that will be used to train the classifier and examples that will be classified.
     *@param[in]  _cancelFlag  Will monitor; if at any point it turns true this instance is to terminate and return to caller.
     *@param[in,out]  _log  Logging file.
     */
    static 
      ModelPtr  CreateAModel (ModelTypes            _modelType,
                              const KKStr&          _name,
                              const ModelParam&     _param,      /**< Will make a duplicate copy of */
                              FileDescPtr           _fileDesc,
                              volatile const bool&  _cancelFlag,
                              RunLog&               _log
                             );
  
    /**
     *@brief  A factory method that will instantiate the appropriate class of training model based off the contents of the istream "i".
     *@details  This method is used to construct a model that has already been built and saved to disk.
     *@param[in] i  Input stream where previously built model has been saved.
     *@param[in] _param  Parameter used to drive the creating of the model.
     *@param[in] _fileDesc Description of the dataset that will be used to train the classifier and examples that will be classified.
     *@param[in]  _cancelFlag  Will monitor; if at any point it turns true this instance is to terminate and return to caller.
     *@param[in,out]  _log  Logging file.
     */
    static
      ModelPtr  CreateFromStream (istream&              i,
                                  FileDescPtr           _fileDesc,
                                  volatile const bool&  _cancelFlag,
                                  RunLog&               _log
                                 );
    virtual
    ModelPtr                 Duplicate () const = 0;


    // Access Methods

    bool                     AlreadyNormalized () const {return alreadyNormalized;}

    int32                    BitsToReduceBy    () const;

    const CompressionStats&  Compression_Stats         () const {return compression_stats;}
    const KKStr&             BitReductionByFeature     () const {return compression_stats.bitReductionByFeature;}
    double                   ReductionTime             () const {return compression_stats.elapsed_cpu_time;  }
    int32                    ReductionPreExampleCount  () const {return compression_stats.num_images_before; }
    int32                    ReductionPostExampleCount () const {return compression_stats.num_images_after;  }
    float                    ReductionRatio            () const {return compression_stats.compression_ratio; }

    virtual
    KKStr                    Description ()  const;  /**< Return short user readable description of model. */

    const FeatureEncoder2&   Encoder () const;

    virtual
    const FeatureNumList&    GetFeatureNums    () const;

    virtual
    KKU::int32               MemoryConsumedEstimated ()  const;

    virtual ModelTypes       ModelType ()  const = 0;
  
    virtual KKStr            ModelTypeStr () const  {return ModelTypeToStr (ModelType ());}

    const KKStr&             Name () const  {return name;}
    void                     Name (const KKStr&  _name)  {name = _name;}

    virtual
    bool                     NormalizeNominalAttributes ()  const; /**< Return true, if nominal fields need to be normalized. */
  
    ModelParamPtr            Param ()  const  {return  param;}

    virtual
    const FeatureNumList&    SelectedFeatures () const;

    const KKStr&             RootFileName            () const {return rootFileName;}

    const KKU::DateTime&     TimeSaved               () const {return timeSaved;}

    double                   TrainingTime            () const {return trainingTime;}

    double                   TrianingPrepTime        () const {return trianingPrepTime;}  //*< Time ins secs spent preparining training data in Model::TrainModel */
  
    bool                     ValidModel              () const {return validModel;}



    // Access Update Methods
    void  RootFileName (const KKStr&  _rootFileName)  {rootFileName = _rootFileName;}
  

    /**
     @brief  Derived classes call this method to start the clock for 'trainingTime'.
     */
    void  TrainingTimeStart ();

    /**
     @brief  Derived classes call this method to stop the clock for 'trainingTime'.
     */
    void  TrainingTimeEnd ();



    void  Load (const KKStr& _rootFileName,
                bool&        _successful
               );
  
    virtual
    FeatureVectorPtr         PrepExampleForPrediction (FeatureVectorPtr  fv,
                                                       bool&             newExampleCreated
                                                      );


    /**
     *@brief  Expects to read in the entire contents of a previously trainied model into
     *        this instance. One of the first lines to be read will contain the specific
     *        type of model to be read.  To update the fields taht are particular to
     *        the specioalized class the method 'ReadSpecificImplementationXML' will be 
     *        called.
     */
    virtual  
    void  ReadXML (istream&  i,
                   bool&     _successful
                  ); 

    virtual  
    void  WriteXML (ostream&  o);


    void  Save (const KKStr& _rootFileName,
                bool&        _successful
               );
  

    virtual  
    void  WriteSpecificImplementationXML (ostream&  o) = 0;





    //*********************************************************************
    //*     Routines that should be implemented by dependent classes.     *
    //*********************************************************************

    virtual
    MLClassConstPtr  Predict (FeatureVectorPtr  image) = 0;
  
    virtual
    void                Predict (FeatureVectorPtr     example,
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
                                ) = 0;


    virtual 
    ClassProbListPtr  ProbabilitiesByClass (FeatureVectorPtr  example) = 0;


    /**@brief  Only applied to ModelDual classifier. */
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
                               ) = 0;

    /**
     *@brief Derives predicted probabilities by class.
     *@details Will get the probabilities assigned to each class by the classifier.  The 
     *        '_mlClasses' parameter dictates the order of the classes. That is the 
     *        probabilities for any given index in '_probabilities' will be for the class
     *        Specified in the same index in '_mlClasses'.
     *@param[in]  _example    FeatureVector object to calculate predicted probabilities for.
     *@param[in]  _mlClasses  List of image classes that caller is aware of.  This should be the
     *            same list that was used when constructing this Model object.  The list must
     *            be the same but not necessarily in the same order as when Model was 1st
     *            constructed.  The ordering of this list will dictate the order that '_probabilities'
     *            will be populated.
     *@param[out] _probabilities An array that must be as big as the number of classes in
     *            '_mlClasses'.  The probability of class in '_mlClasses[x]' will be 
     *            returned in probabilities[x].
     */
    virtual
    void  ProbabilitiesByClass (FeatureVectorPtr         _example,
                                const MLClassConstList&  _mlClasses,
                                double*                  _probabilities
                               ) = 0;
  

    virtual  
    void  ReadSpecificImplementationXML (istream&  i,
                                         bool&     _successful
                                        ) = 0; 


    virtual  
    void  RetrieveCrossProbTable (MLClassConstList&  classes,
                                  double**           crossProbTable  // two dimension matrix that needs to be classes.QueueSize ()  squared.
                                 );


    virtual  
    void  TrainModel (FeatureVectorListPtr  _trainExamples,
                      bool                  _alreadyNormalized,
                      bool                  _takeOwnership  /*!< Model will take ownership of these examples */
                     );



  protected:
    void  AllocatePredictionVariables ();


    void  DeAllocateSpace ();


    void  Read         (istream& i,
                        bool&    _successful
                       );

    void  ReadSkipToSection (istream& i, 
                             KKStr    sectName,
                             bool&    sectionFound
                            );

    void  ReduceTrainExamples ();


    bool                   alreadyNormalized;

    volatile const bool&   cancelFlag;

    MLClassConstListPtr classes;

    ClassIndexListPtr      classesIndex;

    double*                classProbs;
 
    CompressionStats       compression_stats;

    double**               crossClassProbTable;   /*!< Probabilities  between Binary Classes From last Prediction */

    int32                  crossClassProbTableSize;

    FeatureEncoder2Ptr     encoder;

    FileDescPtr            fileDesc;

    RunLog&                log;

    NormalizationParmsPtr  normParms;

    int32                  numOfClasses;          /*!< Number of Classes defined in crossClassProbTable. */

    ModelParamPtr          param;                 /*!< Will own this instance                            */

    KKStr                  rootFileName;   /**< This is the root name to be used by all component objects; such as svm_model,
                                            * mlClasses, and svmParam(including selected features).  Each one will have the
                                            * same rootName with a different suffix.
                                            *@code
                                            *      mlClasses  "<rootName>.image_classes"
                                            *      svmParam      "<rootName>.svm_parm"
                                            *      model         "<rootName>"
                                            *@endcode
                                            */

    FeatureVectorListPtr   trainExamples;

    int32*                 votes;

    bool                   validModel;

    bool                   weOwnTrainExamples;  /**< Indicates if we own the 'trainExamples'.   This does not mean that we own its 
                                                 * contents.  That is determined by 'trainExamples->Owner ()'.
                                                 */
   

  private:
    double                 trianingPrepTime;    /**<  Time that it takes to perform bit compression, normalization, and encoding */
    double                 trainingTime;
    double                 trainingTimeStart;   /**<  Time that the clock for TraininTime was started. */
    KKStr                  name;
    KKU::DateTime          timeSaved;           /**<  Date and Time that this model was saved. */
  };
  
  typedef  Model::ModelPtr  ModelPtr;
  
}  /* namespace MML */

#endif




