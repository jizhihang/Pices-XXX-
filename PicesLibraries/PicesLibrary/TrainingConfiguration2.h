#if  !defined(_TRAININGCONFIGURATION2_)
#define  _TRAININGCONFIGURATION2_

#include  "DateTime.h"
#include  "Str.h"


#include  "Configuration.h"
#include  "Model.h"
#include  "ModelParam.h"
#include  "ModelParamOldSVM.h"
#include  "SVMparam.h"
#include  "TrainingClass.h"

namespace MLL 
{
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


  #if  !defined(_NormalizationParmsDefined_)
  class  NormalizationParms;
  typedef  NormalizationParms*  NormalizationParmsPtr;
  #endif


  class  TrainingConfiguration2:  public  Configuration
  {
  public:
    typedef  Model::ModelTypes   ModelTypes;

    typedef  TrainingConfiguration2*  TrainingConfiguration2Ptr;


    TrainingConfiguration2 (FileDescPtr   _fileDesc,
                            const KKStr&  _configFileName, 
                            RunLog&       _log,
                            bool          validateDirectories = true
                           );


    TrainingConfiguration2 (const TrainingConfiguration2&  tc);

    TrainingConfiguration2 (FileDescPtr             _fileDesc,       // Use this one if you want to
                            MLClassConstListPtr  _mlClasses,   // create a default Configuration 
                            KKStr                   _parameterStr,   // object.
                            RunLog&                 _log             
                           );

    TrainingConfiguration2 (FileDescPtr             _fileDesc,          // Use this one if you want to
                            MLClassConstListPtr  _mlClasses,      // create a default Configuration 
                            ModelParamPtr           _modelParameters,   /**< Will take ownership of this instance. */
                            RunLog&                 _log
                           );


    ~TrainingConfiguration2 ();


    static
    TrainingConfiguration2*  CreateFromDirectoryStructure 
                                            (FileDescPtr        _fileDesc,
                                             const KKStr&       _existingConfigFileName,
                                             const KKStr&       _subDir,
                                             RunLog&            _log,
                                             bool&              _successful,
                                             KKStr&             _errorMessage
                                            );

    static
    TrainingConfiguration2*  CreateFromFeatureVectorList
                                            (FeatureVectorList&  _examples,
                                             const KKStr&        _parameterStr, 
                                             RunLog&             _log
                                            );

    static KKStr       ModelTypeToStr   (ModelTypes    _modelType)    {return  Model::ModelTypeToStr   (_modelType);}
    static ModelTypes  ModelTypeFromStr (const KKStr&  _modelTypeStr) {return  Model::ModelTypeFromStr (_modelTypeStr);}


    //  Access Methods.

    float                    A_Param                 () const;
    float                    AvgNumOfFeatures        () const;
    int32                    BitsToReduceBy          () const;
    double                   C_Param                 () const;
    SVM_CompressionMethod    CompressionMethod       () const;
    const KKStr&             ConfigFileNameSpecified () const  {return configFileNameSpecified;}
    SVM_EncodingMethod       EncodingMethod          () const;
    int32                    ExamplesPerClass        () const;
    FileDescPtr              FileDesc                () const {return  fileDesc;}
    double                   Gamma                   () const;
    int32                    ImagesPerClass          () const {return  ExamplesPerClass ();};
    SVM_KernalType           KernalType              () const;
    SVM_MachineType          MachineType             () const;
    Model::ModelTypes        ModelingMethod          () const {return  modelingMethod;}
    Model::ModelTypes        ModelType               () const {return  modelingMethod;}
    KKStr                    ModelTypeStr            () const {return  Model::ModelTypeToStr (modelingMethod);}
    MLClassConstPtr          NoiseMLClass            () const {return  noiseMLClass;}
    const TrainingClassPtr   NoiseTrainingClass      () const {return  noiseTrainingClass;}
    int32                    NoiseGuaranteedSize     () const {return  noiseGuaranteedSize;}
    int32                    NumOfRounds             () const {return  Number_of_rounds ();}
    int32                    Number_of_rounds        () const;
    MLClassConstPtr          OtherClass              () const {return  otherClass;}
    const KKStr&             RootDir                 () const {return  rootDir;}
    const TrainingClassList& TrainingClasses         () const {return  trainingClasses;}
    ModelParamPtr            ModelParameters         () const {return  modelParameters;}
    KKStr                    ModelParameterCmdLine   () const;
    uint32                   NumHierarchialLevels    () const;  /**<  returns back the number of hierarchail levels there are in the trainingClass that has the most. */
    SVM_SelectionMethod      SelectionMethod         () const;
    const SVMparam&          SVMparamREF             () const;
    int32                    UnBalancedBits          () const;
    KKStr                    UnBalancedBitsStr       () const;
    bool                     WeightBitReduction      () const;

    const
    BinaryClassParmsPtr      BinaryClassParms (MLClassConstPtr  class1,
                                               MLClassConstPtr  class2
                                              )  const;


    /** if BinaryClass parms exist for the two specified classes will return otherwise NULL. */
    BinaryClassParmsPtr      GetBinaryClassParms (MLClassConstPtr  class1,
                                                  MLClassConstPtr  class2
                                                 );

    double                   C_Param (MLClassConstPtr  class1,
                                      MLClassConstPtr  class2
                                     )  const;


    void   A_Param             (float                  _aParam);
    void   BitsToReduceBy      (int32                  bitReduction);
    void   C_Param             (double                 _CCC);
    void   CompressionMethod   (SVM_CompressionMethod  _compMethod);
    void   EncodingMethod      (SVM_EncodingMethod     _encodingMethod);
    void   ExamplesPerClass    (int32                  _examplesPerClass);
    void   Gamma               (double                 _gamma);
    void   ImagesPerClass      (int32                  _imagesPerClass)  {ExamplesPerClass (_imagesPerClass);}
    void   KernalType          (SVM_KernalType         _kernalType);
    void   MachineType         (SVM_MachineType        _machineType);
    void   Number_of_rounds    (int32                  _number_of_rounds);
    void   NumOfRounds         (int32                  _numOfRounds)     {Number_of_rounds  (_numOfRounds);}
    void   RootDir             (const KKStr&           _rootDir);
    void   ModelParameters     (ModelParamPtr          _modelParameters);
    void   SelectionMethod     (SVM_SelectionMethod    _selectionMethod);
    void   UnBalancedBits      (int32                  _unBalancedBits);
    void   UnBalancedBitsStr   (const KKStr&           _unBalancedBitsStr);
    void   WeightBitReduction  (bool                   _weightBitReduction);

    void                   C_Param (MLClassConstPtr  class1,
                                    MLClassConstPtr  class2,
                                    double              cParam
                                   );


    void                   AddATrainingClass (TrainingClassPtr  _trainClass);

    void                   AddATrainingClass (MLClassConstPtr  _newClass);  /**<  Will assume that images for this class will be saved off 
                                                                                * the RootDirectory using its own name for the subdirectory name.
                                                                                */

    float                  AvgNumOfFeatures (FeatureVectorListPtr  trainExamples)  const;

    static
      bool                 ConfigFileExists (const KKStr& _configFileName);

    KKStr                  DirectoryPathForClass (MLClassConstPtr  mlClass)  const;

    void                   EmptyTrainingClasses ();

    MLClassConstListPtr    ExtractClassList ()  const;             /**< Constructs new list of classes that caller will own. */

    MLClassConstListPtr    ExtractListOfClassesForAGivenHierarchialLevel (uint32 level)   const;

    TrainingConfiguration2Ptr  GenerateAConfiguraionForAHierarchialLevel (uint32 level);

    static  KKStr          GetEffectiveConfigFileName (const  KKStr&  configFileName);

    FeatureNumList         GetFeatureNums ()  const;

    /**@brief  Returns feature nums that are assigned to the given class pair; If none were specified for that pair will return global feature nums. */
    FeatureNumList         GetFeatureNums (MLClassConstPtr  class1,
                                           MLClassConstPtr  class2
                                          );

    FeatureVectorListPtr   LoadFeatureDataFromTrainingLibraries (KKU::DateTime&  latestImageTimeStamp,
                                                                 bool&           changesMadeToTrainingLibraries,
                                                                 bool&           cancelFlag
                                                                );

    FeatureVectorListPtr   LoadOtherClasssExamples ();

    TrainingClassPtr       LocateByMLClassName (const KKStr&  className);

    bool                   NormalizeNominalFeatures ();

    int32                  NumOfFeaturesAfterEncoding ()  const;

    static
      TrainingConfiguration2Ptr
                           PromptForConfigurationFile (RunLog&  log);

    void                   Save (const KKStr& fileName);

    void                   SetFeatureNums (const  FeatureNumList&  features);

    void                   SetFeatureNums (MLClassConstPtr        class1,
                                           MLClassConstPtr        class2,
                                           const FeatureNumList&  _features,
                                           float                  _weight = -1   //  -1 Indicates - use existing value
                                          );

    void                   SetModelParameters (ModelParamPtr  _svmParanters,
                                               int32          _examplesPerClass
                                              );


    void                   SetBinaryClassFields (MLClassConstPtr               class1,
                                                 MLClassConstPtr               class2,
                                                 const SVM233::svm_parameter&  _param,
                                                 const FeatureNumList&         _features,
                                                 float                         _weight
                                                );

    void                   SetTrainingClasses (TrainingClassListPtr  _trainingClasses);


  private:
    void                   BuildTrainingClassListFromDirectoryStructure (const KKStr&  rootDir,
                                                                         const KKStr&  subDir,
                                                                         bool&         successful,
                                                                         KKStr&        errorMessage
                                                                        );


    void                   CreateModelParameters (const KKStr&           _parameterStr,
                                                  const FeatureNumList&  _selFeatures,
                                                  int32                  _sectionLineNum,
                                                  int32                  _parametersLineNum, 
                                                  int32                  _featuresIncludedLineNum
                                                 );


    FeatureNumListPtr      DeriveFeaturesSelected (int32  sectionNum);

    void                   DetermineWhatTheRootDirectoryIs ();

    FeatureVectorListPtr   ExtractFeatures (const TrainingClassPtr  trainingClass,
                                            KKU::DateTime&          latestTimeStamp,
                                            bool&                   changesMade,
                                            bool&                   cancelFlag
                                           );

    SVMparamPtr            SVMparamToUse ()  const;

    void                   SyncronizeMLClassListWithTrainingClassList ();

    TrainingClassPtr       ValidateClassConfig    (int32  sectionNum);

    void                   ValidateConfiguration ();
    
    void                   ValidateGlobalSection (int32  sectionNum);

    void                   ValidateOtherClass (MLClassConstPtr  otherClass,
                                               int32               otherClassLineNum
                                              );

    void                   ValidateTrainingClassConfig (int32  sectionNum);

    void                   ValidateTwoClassParameters (int32  sectionNum);


    ModelParamOldSVMPtr    OldSvmParameters ()  const;


    KKStr                  configFileNameSpecified;   /**< Config file name that was specified by caller before
                                                       * directory path was added by 'GetEffectiveConfigFileName'.
                                                       */

    int32                  examplesPerClass;
    FileDescPtr            fileDesc;
    MLClassConstListPtr    mlClasses;
    bool                   mlClassesWeOwnIt;      /**< If we own it we will delete it in the destructor.  */
    RunLog&                log;
    ModelTypes             modelingMethod;
    ModelParamPtr          modelParameters;

    int32                  noiseGuaranteedSize;   /**< Images smaller than this size will be classified as noise and will 
                                                   * not be used for training purposes.
                                                   */

    MLClassConstPtr        noiseMLClass;

    TrainingClassPtr       noiseTrainingClass;    /**< The specific Training Class that is to be used for noise images.  */

    NormalizationParmsPtr  normalizationParms;

    MLClassConstPtr        otherClass;            /**< class that is to be used for "Other" examples when performing
                                                   * adjustment calculations.  This was done as part of the Dual Class
                                                   * classificatuion program.  When specified the acytual training of 
                                                   * the classifier wil lnot inclusde this class.  It will be used when 
                                                   * a clas can not be determined.
                                                   */

    int32                  otherClassLineNum;     /**< Line where OtherClass in configuratyon was defined. */

    KKStr                  rootDir;               /**< Common directory that all images for this training
                                                   * library come from.  This is determined by iterating 
                                                   * through all the 'trainingClasses' entries and 
                                                   * looking for the common string that they all start by.
                                                   */
                                            
    TrainingClassList      trainingClasses;       /**< List of  'Training_Class' objects.  One for each 'Training_Classe' 
                                                   * section defined in configuration file.  Plus one for the 'Noise_Images' 
                                                   * section.
                                                   */

    bool                   validateDirectories;
  };  /* TrainingConfiguration2 */

  #define  _TrainingConfiguration2Defined_

  typedef  TrainingConfiguration2::TrainingConfiguration2Ptr  TrainingConfiguration2Ptr;




  KKStr&  operator<< (KKStr&                               left,
                      TrainingConfiguration2::ModelTypes   modelingMethod
                     );


  std::ostream& operator<< (std::ostream&                       os,
                            TrainingConfiguration2::ModelTypes  modelingMethod
                           );




  class  TrainingConfiguration2List: public KKQueue<TrainingConfiguration2>
  {
  public:
    TrainingConfiguration2List (bool _owner):
        KKQueue<TrainingConfiguration2> (_owner)
        {}

    ~TrainingConfiguration2List ()
        {}

  };


  typedef  TrainingConfiguration2List*  TrainingConfiguration2ListPtr;

}  /* namespace MLL */

#endif
