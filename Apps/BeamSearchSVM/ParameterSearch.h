#ifndef  _PARAMETERSEARCH_
#define  _PARAMETERSEARCH_

#include "Str.h"
#include "RunLog.h"
#include "svm.h"
#include "SVMparam.h"
#include "ImageClass.h"
#include "ImageFeatures.h"
#include "ParameterJob.h"


class  ParameterSearch
{
public:
  ParameterSearch ();
  
  ~ParameterSearch ();

  void  Block ();

  KKStr  CommandLineStr  ();
 
 
  void    EndBlock ();

  void    exit_with_help ();

  void    LetsDoIt ();

  #ifndef  WIN32
  double  LoadAverage ();
  #endif;


  void    ExtractGoodParamters (KKStr  resultFileName,
                                KKStr  reportFileName,
                                KKStr  configFileName
                               );

  void    LoadCurrentStatusFile ();

  void    MakeGammaCostReport (KKStr  resultFileName,
                               KKStr  reportFileName
                              );

  void    ParseCommandLine (int    argc, 
                            char **argv
                           );

  void    ProcessCmdLineStr  (KKStr  cmdLine);

  void    ProcessStatusFile (int argc,  char **argv);

  void    ReadProblem ();

  void    UpdateCurrentStatusFile ();


private:
  void  GetGoodParametersForTwoClasses (ImageClassPtr                     class1,
                                        ImageClassPtr                     class2,
                                        ParameterSearchReportLineListPtr  results,
                                        ostream&                          r,
                                        ostream&                          c
                                       );

  void  ProcessJobBinaryClasses ();

  void    ProcessNextJobAllClassesCombined ();


  bool                      cancelFlag;
  TrainingConfigurationPtr  config;
  KKStr                     configFileName;
  ParameterJobPtr           curJob;
  int                       currentJobId;
  FileFormat                fileFormat;
  double                    highestAccuracySoFar;
  ImageClassList            imageClasses;
  ImageFeaturesListPtr      images;
  KKStr                     inputFileName;
  int                       lockFile;
  RunLog                    log;
  int                       maxIndex;
  bool                      multiClassClassifiers;
  int                       nextJobId;
  int                       nr_fold;
  int                       numOfProcesses;
  int                       ourProcessNum;
  ParameterJobListPtr       processes;
  KKStr                     resultFileName;
  FILE*                     statusFile;
  SVMparamPtr               svmParamaters;
  KKStr                     testFileName;
  ImageFeaturesListPtr      testExamples;

  int                  nextC;
  double               nextGamma;
  float                nextA;

  int                  currentC;
  double               currentGamma;
  int                  currentA;


  #ifndef  WIN32
  bool                 weWereSleeping;
  #endif;
};
#endif