#ifndef  _BITREDUCTIONPAPER_
#define  _BITREDUCTIONPAPER_


#include  "BitReduction.h"
#include  "ConfusionMatrix2.h"
#include  "CrossValidation.h"
#include  "DateTime.h"
#include  "FeatureNumList.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "Orderings.h"
#include  "RunLog.h"
#include  "Str.h"
#include  "TrainingConfiguration.h"

#include  "DataSetParameters.h"
#include  "BitReductionJob.h"


class  BitReductionPaper
{
public:
  BitReductionPaper ();

  ~BitReductionPaper ();

  float                      accLossTolerance;

  TrainingConfigurationPtr   config;
  KKStr                     configFileName;

  KKStr                     dataSetRootDir;

  bool                       featuresAreNormalizedAlready;

  FileDescPtr                fileDesc;

  RunLog                     runLog;
  FeatureVectorListPtr       images;
  MLClassListPtr          mlClasses;

  FeatureFileIOPtr           inputFormat;

  laLearningAlgorithms       learningAlgorithm;

  int                        numOfFolds;

  bool                       performRandomSampleAll;   // If set to true then we want to perform Random Sampling on
                                                       // all data sets. 

  //bool                       performRandomSampling;    // When true perform random sampling on data set specified.
  brjJobTypes                jobTypes;


  KKStr                      reportFileName;

  ostream*                   reportFile;

  ofstream*                  reportFileStream;

  KKStr                      rootDirName;

  float                      samplePercentage;

  DateTime                   startTime;

  bool                       testBitReductionOptions;

  KKStr                      trainDataFileName;

  FeatureVectorListPtr       testData;

  KKStr                      testDataFileName;




  void   CmdLineHelp ();

  int    DetermineCompressedImageCount ();

  void   ProcessAllJobsForASingleDataSet (DataSetParameters&  dsp);


  void   ProcessCmdLineParameter (KKStr  parmSwitch, 
                                  KKStr  parmValue 
                                 );

  void   ProcessCmdLineParameters (int     argc, 
                                   char**  argv
                                  );

  void   ReportCmdLineParameters ();

  void   RunRandomSamplingPasses (TrainingConfigurationPtr  config, 
                                  SVM_KernalType            kernalType,
                                  SVM_EncodingMethod        encodinfMethiod,
                                  int                       C,
                                  int                       bitReduction,
                                  int                       unBalBits,
                                  int                       numCompressedImages, 
                                  OrderingsPtr              orderings,
                                  double&                   accuracyMean,
                                  double&                   accuracyStdDev,
                                  double&                   trainTimeMean,
                                  double&                   testTimeMean
                                 );

  void  RunRandomSamplingOnAllDataSets ();

  void  RunRandomSamplingOnOneDataSet (DataSetParameters&  dsp,
                                       double&             accuracyMean,
                                       double&             accuracyStdDev,
                                       double&             trainTimeMean,
                                       double&             testTimeMean,
                                       float&              compRatio,
                                       float&              majarityClassFrac
                                      );

  void  SetDataSetRootDirName ();



  int   Main (int  argc,  char** argv);

};  /* BitReductionPaper */




typedef   BitReductionPaper*  BitReductionPaperPtr;
#endif
