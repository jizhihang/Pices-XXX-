#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


#include "Classifier2.h"
#include "ImageFeatures.h"
#include "PicesFeatureVector.h"
#include "PicesRunLog.h"
#include "TrainingProcess2.h"

// Pices Interface Headers
#include "PicesClass.h"
#include "PicesClassList.h"
#include "PicesPrediction.h"
#include "PicesPredictionList.h"
#include "PicesTrainingConfiguration.h"




namespace PicesInterface 
{
  public ref class  ProbNamePair
  {
  public:
    ProbNamePair (String^  _name,
                  double   _probability
                 ):
        name (_name),  probability (_probability)
        {}

    ProbNamePair (const KKStr&  _name,
                  double        _probability
                 ):
        name (nullptr),  probability (0.0f)
    {
      name = PicesKKStr::KKStrToSystenStr (_name);
      probability = _probability;
    }

    String^  name;
    double   probability;
  };



	public ref class TrainingModel2 
	{
	public:
    enum class ModelTypes   {Null = 0, OldSVM = 1, SvmBase = 2, KNN = 3, UsfCasCor = 4, Dual = 5};

    TrainingModel2 (PicesRunLog^     _picesRunLog,
                    System::String^  _modelName
                   );

    TrainingModel2 (PicesRunLog^                _picesRunLog,
                    System::IO::DirectoryInfo^  _directoryInfo
                   );

    TrainingModel2 (PicesRunLog^                 _picesRunLog,
                    PicesTrainingConfiguration^  _config
                   );

    /** 
     *@brief  Will creae an instance to be used with an already existing instance of 'TrainingProcess2'.
     *@details I created this to help get a break down of a prediction on a Dual class classifier.
     */
    TrainingModel2 (PicesRunLog^         _picesRunLog,
                    TrainingProcess2Ptr  _trainer
                   );

    void  CleanUp ();

  protected:
    !TrainingModel2 ();

  private:
    ~TrainingModel2 ();

    void  CleanUpUnmanagedResources ();


  public:
    property  System::DateTime  BuildDateTime            {System::DateTime  get ();}
    property  bool              CancelFlag               {bool              get ()  {return *cancelFlag;}}
    property  String^           ConfigFileName           {String^           get ();}
    property  List<String^>^    ErrorMsgs                {List<String^>^    get ()  {return  errorMsgs;}}
    property  int               ImagesPerClass           {int               get ();}
    property  String^           ModelName                {String^           get ()  {return  modelName;}}
    property  ModelTypes        ModelType                {ModelTypes        get ();}
    property  uint              NumHierarchialLevels     {uint              get ();}     // returns back the number of hierarchail levels there
    property  PicesClass^       OtherClass               {PicesClass^       get ();}
    property  String^           ParameterStr             {String^           get ();}
    property  String^           RootDir                  {String^           get ();}
    //property  PicesRunLog^  PicesRunLog              {PicesRunLog^ get ()  {return picesRunLog;}}
    property  String^           RunLogFileName           {String^           get ();}
//    property  String^           SvmParamStr              {String^           get ();}
    property  bool              Valid                    {bool              get ()  {return  *valid;}}


    /**@brief When using a Dual classifier will return an instance of the first/left classifier.  */
    TrainingModel2^   CreateTrainingModelLeft ();

    /**@brief When using a Dual classifier will return an instance of the second/right classifier.  */
    TrainingModel2^   CreateTrainingModelRight ();



    void  AddClass (PicesClass^  newClass);

    void  AddImageToTrainingLibray (String^       imageFileName,
                                    PicesRaster^  raster, 
                                    PicesClass^   mlClass,
                                    bool          onLine  
                                   );

    void  BuildTrainingModel (PicesFeatureVectorList^  picesTrainingData);


    void  CancelLoad ();   // Sets cancel flag to terminate loading of training model.

    PicesClassList^  MLClasses ();  // Will return a list of classes that belong to this model. 
                                       // It will be created from "classList".  So the the caller 
                                       // can do with it as they want.

    String^  DirectoryPathForClass (PicesClass^  mlClass);


    static
    array<String^>^  GetListOfTrainingModels ();

    bool  IncludesClass (PicesClass^  mlClass); 

    String^  LastRunLogTextLine ();

    void  LoadExistingTrainedModel ();

    void  LoadExistingModelOtherwiseBuild (PicesMsgQueue^  msgQueue  /**< Will attach message queue to runLog */
                                          );

    void  LoadTrainigLibrary (bool  forceRebuild);


    void  LoadTrainingModelForGivenLevel (uint            level,
                                          PicesMsgQueue^  msgQueue  /**< Will attach message queue to runLog */
                                         );

    PicesPrediction^  PredictClass (PicesFeatureVector^  example);

    void  PredictClass (PicesFeatureVector^  featureVector,
                        PicesPrediction^     prediction1,
                        PicesPrediction^     prediction2
                       );

    void  PredictClass (System::String^   imageFileName,
                        System::Array^    raster,
                        System::Single^   depth,
                        System::Single^   salinity, 
                        System::Single^   oxygen, 
                        System::Single^   florences,
                        PicesPrediction^  prediction1,
                        PicesPrediction^  prediction2
                       );


    // This call assumes that there was a sucessfull call to PredictProbabilities before.
    PicesPredictionList^   BinaryProbailitiesForClass (PicesClass^  leftClass);

    array<PicesInterface::ProbNamePair^>^  
                           FindWorstSupportVectors (PicesFeatureVector^  picesFeatureVector,
                                                    int                  numToFind,
                                                    PicesClass^          c1,
                                                    PicesClass^          c2
                                                   );

    array<PicesInterface::ProbNamePair^>^  
                           FindWorstSupportVectors2 (PicesFeatureVector^  picesFeatureVector,
                                                     int                  numToFind,
                                                     PicesClass^          c1,
                                                     PicesClass^          c2
                                                    );

    PicesPredictionList^   PredictProbabilities (System::String^  imageFileName,
                                                 System::Array^   raster,
                                                 System::Single^  depth,
                                                 System::Single^  salinity, 
                                                 System::Single^  oxygen, 
                                                 System::Single^  florences
                                                );

    PicesPredictionList^   PredictProbabilities (System::String^   imageFileName,
                                                 PicesRasterList^  saveCalcImages   /**< If Not NULL intermediate instances created during feature calculation will be appended to this list. */
                                                );

    PicesPredictionList^   PredictProbabilities (PicesFeatureVector^  picesFeatureVector);


    void   ProbabilitiesByClassDual (PicesFeatureVector^    picesFeatureVector,
                                     String^%               classifier1Desc,
                                     String^%               classifier2Desc,
                                     PicesPredictionList^%  classifier1Results,
                                     PicesPredictionList^%  classifier2Results
                                    );

    void  SaveConfiguration ();

    array<String^>^   SupportVectorNames (PicesClass^ c1,
                                          PicesClass^ c2
                                         );

	private:

    Bitmap^  BuildBitmapFromRasterData (uchar**  r,
                                        int      height,
                                        int      width
                                       );

    void     CreateRunLog ();

    void     ErrorMsgsClear ();
    void     ErrorMsgsAdd (String^ errorMsg);
    void     ErrorMsgsAdd (const VectorKKStr&  _errorMsgs);


    TrainingConfiguration2Ptr   GetConfigToUse ();

    void  PopulateCSharpClassList ();

    void  UpdateMemoryPressure ();


    bool*                     cancelFlag;
    Classifier2Ptr            classifier;
    MLClassConstListPtr    classes;
    TrainingConfiguration2Ptr config;
    double**                  crossProbTable;
    int                       crossProbTableNumClasses;
    int                       curMemoryPressure;
    List<String^>^            errorMsgs;     /**< Error messages for later recall are added to this list. */
    bool                      loadTrainingLibraryRunning;
    System::String^           modelName;
    PicesRunLog^              picesRunLog;
    double*                   probabilities;
    RunLogPtr                 runLog;
    String^                   runLogFileName;
    int                       runLogLastLineNum;
    KKStrPtr                  statusMsg;
    TrainingProcess2Ptr       trainer;
    bool                      trainerWeOwn;
    bool*                     valid;     // True if Training Library Valid
    int*                      votes;

    PicesClassList^           classList;
    array<Pen^>^              colorValues;
  };
}
