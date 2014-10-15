#ifndef  _TRAININGPROCESS2_
#define  _TRAININGPROCESS2_


#include "Model.h"
#include "SVMModel.h"
#include "TrainingConfiguration2.h"



namespace MLL 
{
  #if  !defined(_FEATUREVECTOR_)
  class  FeatureVector;
  typedef  FeatureVector*  FeatureVectorPtr;
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif


  #ifndef _FileDescDefined_
  class  FileDesc;
  typedef  FileDesc*  FileDescPtr;
  #endif


  #if  !defined(_MLCLASS_)
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


  #if  !defined(_MODELOLDSVM_)
  class  ModelOldSVM;
  typedef  ModelOldSVM*  ModelOldSVMPtr;
  #endif

  #if  !defined(_TRAININGCONFIGURATION2_)
  class  TrainingConfiguration2;
  typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;
  #endif


  class  TrainingProcess2
  {
  public:
    typedef  TrainingProcess2*  TrainingProcess2Ptr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::ulong   ulong;

    /**
     *@brief  Constructor that is driven by contents of configuration file.
     *@details  If no changes to config file or training data, will utilize an existing built model
     *          that was saved to disk earlier;  otherwise will train from data in training library
     *          and save resultant training classifier to disk.
     *
     *@todo  I need to make a stucture that would contain variables required to perform prediction 
     *       The idea is that we should only need to create one instance of "TrainingProcess2" for 
     *       use by multiple threads.  Since diferent classifiers will require different structures
     *       "TrainingProcess2" wil have a factory method for creating instances of these structures.
     *       These structures woul dthen have their own predict methods.
     *
     *@param[in]  _configFileName  Configuration file name where classes and SVM parameters are
     *                             specified.  The directories where example images for each class
     *                             will be specified.
     *
     *@param[in]  _excludeList  List of Feature Vectors that are to be excluded from the TrainingData.
     *                          If will check by both ImageFileName and FeatureValues. If this param-
     *                          is not equal to NULL then will not save the Training model.  Will
     *                          NOT take ownership of list or its contents.  Caller still owns it.
     *
     *@param[in]  _fileDesc Description of the Feature Data,  how many features, types, etc.
     *
     *@param[in,out]  _log   Logging file.
     *
     *@param[in] _report  if not set to NULL will write statistics of the training process to stream.
     *
     *@param[in] _forceRebuild  If set to true will ignore existing training model and rebuild and
     *                          save new one.
     * 
     *@param[in] _checkForDuplicates  If set to true will look for duplicates in the training data. Two
     *                                FeatureVectors will be considered duplicate if they have the Same
     *                                ImageFileName or the save Feature Values.  If duplicates are in 
     *                                the same class then all all but one will be removes.  If they are
     *                                in more then one class then they will both be removed.
     *
     *@param[in] _cancelFlag  Will be monitored by training process.  If this flag turns true will return
     *                        to caller as soon as convenient.
     *
     *@param[out] _statusMessage  Caller can monitor this field for messages that can be displayed to 
     *                            the user as a way of letting them know what is happening.
     */
    TrainingProcess2 (const KKStr&         _configFileName,
                      FeatureVectorListPtr _excludeList,
                      FileDescPtr          _fileDesc,
                      RunLog&              _log,
                      std::ostream*        _report,
                      bool                 _forceRebuild,
                      bool                 _checkForDuplicates,
                      volatile const bool& _cancelFlag,
                      KKStr&               _statusMessage
                     );


     /**
      *@brief  Build a new model from scratch for the specified class level. Will also remove duplicates.
      *
      *@details  Using the parameter '_level' will construct a classifier that groups classes together
      *          by group.  Underscore characters in the class name will be used to deliminate group
      *          levels.  Ex:  Crustacean_Copepod_Calanoid has three levels of grouping where 'Crustacean'
      *          belongs to level 1.
      *
      *@param[in]  _configFileName  Configuration file name where classes and SVM parameters are
      *                             specified.  The directories where example images for each class
      *                             will be specified.
      *
      *@param[in]  _excludeList  List of Feature Vectors that are to be excluded from the TrainingData.
      *                          If will check by both ImageFileName and FeatureValues. If this param-
      *                          is not equal to NULL then will not save the Training model.  Will
      *                          NOT take ownership of list or its contents.  Caller still owns it.
      *
      *@param[in]  _fileDesc Description of the Feature Data,  how many features, types, etc.
      *
      *@param[in,out]  _log   Logging file.
      *
      *@param[in] _level  The grouping level to build a classifier for.  Ex: if _level = 2 is specified
      *                   and referring to the class name "Crustacean_Copepod_Calanoid" above all classes
      *                   that start with "Crustacean_Copepod_" will be combined as one logical class.
      *
      *@param[in] _cancelFlag  Will be monitored by training process.  If this flag turns true will return
      *                        to caller as soon as convenient.
      *
      *@param[out] _statusMessage  Caller can monitor this field for messages that can be displayed to 
      *                            the user as a way of letting them know what is happening.
      */
     TrainingProcess2 (const KKStr&         _configFileName,
                       FeatureVectorListPtr _excludeList,
                       FileDescPtr          _fileDesc,
                       RunLog&              _log,
                       uint32               _level,            // Class hierarchy level to train at.
                       volatile const bool& _cancelFlag, 
                       KKStr&               _statusMessage
                      );

     
     
     /**
      *@brief  Constructor Will use existing built model; will not check to see if it is up-to-date.
      *@details  If no changes to config file or training data, will utilize an existing built model
      *          that was saved to disk earlier;  otherwise will train from data in training library
      *          and save resultant training classifier to disk.
      *
      *@param[in]  _configFileName  Configuration file name where classes and SVM parameters are
      *                             specified.  The directories where example images for each class
      *                             will be specified.
      *
      *@param[in]  _fileDesc Description of the Feature Data,  how many features, types, etc.
      *
      *@param[in,out]  _log   Logging file.
      *
      *@param[in] _featuresAlreadyNormalized  If set to true will assume that all features in the
      *                                       training data are normalized.
      *
      *@param[in] _cancelFlag  Will be monitored by training process.  If this flag turns true will return
      *                        to caller as soon as convenient.
      *
      *@param[out] _statusMessage  Caller can monitor this field for messages that can be displayed to 
      *                            the user as a way of letting them know what is happening.
      */
     TrainingProcess2 (const KKStr&          _configFileName,
                       FileDescPtr           _fileDesc,
                       RunLog&               _log,
                       bool                  _featuresAlreadyNormalized,
                       volatile const bool&  _cancelFlag,
                       KKStr&                _statusMessage
                      );




     /**
      *@brief  Constructor Will read existing built model from provided input stream.
      *
      *@param[in]  _in  Input stream that contains trainingProcess instance;  will process until 
      *                 first line containing "</TrainingProcess2>" is encountered.
      *
      *@param[in]  _fileDesc Description of the Feature Data,  how many features, types, etc.
      *
      *@param[in,out]  _log   Logging file.
      *
      *@param[in] _featuresAlreadyNormalized  If set to true will assume that all features in the
      *                                       training data are normalized.
      *
      *@param[in] _cancelFlag  Will be monitored by training process.  If this flag turns true will return
      *                        to caller as soon as convenient.
      *
      *@param[out] _statusMessage  Caller can monitor this field for messages that can be displayed to 
      *                            the user as a way of letting them know what is happening.
      */
     TrainingProcess2 (istream&              _in,
                       FileDescPtr           _fileDesc,
                       RunLog&               _log,
                       bool                  _featuresAlreadyNormalized,
                       volatile const bool&  _cancelFlag,
                       KKStr&                _statusMessage
                      );


     /**
      *@brief  Constructor that gets its training data from a list of examples provided in one of the parameters.
      *@param[in]  _config  A configuration that is already loaded in memory.
      *@param[in]  _trainingExamples  Training data to train classifier with.
      *@param[in]  _mlClasses  Class list.
      *@param[in]  _reportFile  if not set to NULL will write statistics of the training process to stream.
      *@param[in]  _fileDesc Description of the Feature Data,  how many features, types, etc.
      *@param[in,out]  _log   Logging file.
      *@param[in] _featuresAlreadyNormalized  If set to true will assume that all features in the
      *                                       training data are normalized.
      *@param[in] _cancelFlag  Will be monitored by training process.  If this flag turns true will return
      *                        to caller as soon as convenient.
      *@param[out] _statusMessage  Caller can monitor this field for messages that can be displayed to 
      *                            the user as a way of letting them know what is happening.
      */
     TrainingProcess2 (TrainingConfiguration2Ptr  _config, 
                       FeatureVectorListPtr       _trainingExamples,
                       std::ostream*              _reportFile,
                       FileDescPtr                _fileDesc,
                       RunLog&                    _log,
                       bool                       _featuresAlreadyNormalized,
                       volatile const bool&       _cancelFlag,
                       KKStr&                     _statusMessage
                      );


     ~TrainingProcess2 ();

    void    CreateModelsFromTrainingData ();

    void    LoadPrevTrainedOtherwiseRebuild (bool  _forceRebuild,
                                             bool  _checkForDuplicates
                                            );

    void    ExtractTrainingClassFeatures (KKU::DateTime&  latestImageTimeStamp,
                                          bool&           changesMadeToTrainingLibraries
                                         );

    void    ReportTraningClassStatistics (std::ostream&  report);

    void    SaveResults ();

    void    ValidateConfiguration ();


    // Access Members
    bool                      Abort              () const  {return abort;}

    const KKU::DateTime&      BuildDateTime      () const  {return  buildDateTime;}

    const CompressionStats&   CompStats          () const;

    TrainingConfiguration2Ptr Config             ()        {return  config;}    

    const KKStr&              ConfigFileName     () const  {return configFileName;}

    VectorKKStr               ConfigFileFormatErrors ()  const;

    int32                     DuplicateDataCount () const;

    bool                      FeaturesAlreadyNormalized ()  
                                                   {return  featuresAlreadyNormalized;}


    FeatureVectorListPtr      Images             () {return  trainingExamples;}

    MLClassConstListPtr       MLClasses       () {return  mlClasses;}

    RunLog&                   Log                () {return log;}

    int32                     MemoryConsumedEstimated ()  const;

    Model::ModelTypes         ModelType          ()  const;

    KKStr                     ModelTypeStr       ()  const;

    KKStr                     ModelDescription   ()  const;

    SVMModelPtr               Model3 ();
   
    ModelOldSVMPtr            OldSVMModel ()  const;

    void                      Read (istream&  in,
                                    bool&     successful
                                   );

    void                      WriteXml (ostream&  o);

    const KKStr&              BitReductionByFeature     () const;
    double                    ReductionTime             () const;
    int32                     ReductionPreExampleCount  () const;
    int32                     ReductionPostExampleCount () const;
    float                     ReductionRatio            () const;

    ModelParamPtr             Parameters                () const;

    ModelPtr                  TrainedModel              () const  {return model;}

    double                    TrainingTime              () const;   // Comes from SVMModel

    int32                     NumOfSupportVectors       () const;

    void                      SupportVectorStatistics (int32&  numSVs,
                                                       int32&  totalNumSVs
                                                      );

    void                      Abort (bool _abort)  {abort = _abort;}


    /**
      *@brief Returns back pointer to 1st classifier of Dual Classifier.
      *@details If not a Dual classifier will return back NULL. Keep in mind that you will
      *  not won this classifier and that ot can be deleted at any time.
      */
    TrainingProcess2Ptr   TrainingProcessLeft ();

    /**
      *@brief Returns back pointer to 2nd classifier of Dual Classifier.
      *@details If not a Dual classifier will return back NULL.
      */
    TrainingProcess2Ptr   TrainingProcessRight ();




  private:
    void    AddImagesToTrainingLibray (FeatureVectorList&  trainingImages,
                                       FeatureVectorList&  imagesToAdd
                                      );

    void    BuildModel3 ();

    void    CheckForDuplicates ();

    void    RemoveExcludeListFromTrainingData ();



    //************************************************************
    //             Routines for Extracting Features              *
    //************************************************************
    void  ExtractFeatures (const TrainingClassPtr  trainingClass,
                           KKU::DateTime&          latestTimeStamp,
                           bool&                   changesMade
                          );


    //************************************************************
    //       Routines for validating Configuration File.         *
    //************************************************************
    FeatureNumListPtr  DeriveFeaturesSelected (int32  sectionNum);

    TrainingClassPtr   ValidateClassConfig (int32  sectionNum);

    void               ValidateTrainingClassConfig (int32  sectionNum);

    void               ValidateModel3Section (int32 sectionNum);
   



    //************************************************************
    // Variables that are Global to TrainingProcess2 application. *
    //************************************************************

    bool                         abort;       // If problem building a model or loading will be set to True.

    volatile const bool&         cancelFlag;  // A calling app can set this to true
                                              // Training Process will monitor this 
                                              // Flag, if true will terminate.


    TrainingConfiguration2Ptr    config;

    KKStr                        configFileName;           /**< The directory path where this file is actually located will be
                                                            * added to this name.
                                                            */

    KKStr                        configFileNameSpecified;  /**< This will be the ConfigFileName specified by caller before the
                                                            * directory ath is added for actual location of config file.
                                                            */

    KKU::DateTime                buildDateTime;

    int32                        duplicateCount;

    FeatureVectorListPtr         excludeList;  /** If != NULL then list of trainingExamples that 
                                                * need to be eliminated from training data.
                                                * This would be used when classifying trainingExamples 
                                                * that might already contain training data.  If
                                                * you wish to grade the results it would only
                                                * be fare to delete these trainingExamples.
                                                */

    bool                         featuresAlreadyNormalized;

    FileDescPtr                  fileDesc;

    FeatureVectorListPtr         trainingExamples;  /** All Images Loaded.  Own's all trainingExamples.
                                                     * All other ImageList's will only point to 
                                                     * these trainingExamples.
                                                     */

    MLClassConstListPtr       mlClasses; /**< List of all classes that are to be processed.
                                                * There will be one entry for each MLClass,
                                                * Including one for noise trainingExamples(unknown trainingExamples).
                                                */

    RunLog&                      log;

    ModelPtr                     model;

    std::ostream*                report;

    KKStr                        savedModelName;

    KKStr&                       statusMessage; /**< A means of communicating back to calling function in a multi threaded environment. */

    bool                         weOwnConfig;
    bool                         weOwnMLClasses;
    bool                         weOwnTrainingExamples;
  };


  typedef  TrainingProcess2::TrainingProcess2Ptr  TrainingProcess2Ptr;


} /* namespace MLL */

#endif
