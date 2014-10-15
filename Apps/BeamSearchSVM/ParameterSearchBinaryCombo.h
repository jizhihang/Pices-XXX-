#ifndef  _PARAMETERSEARCHBINARYCOMBO_
#define  _PARAMETERSEARCHBINARYCOMBO_

#include "ImageFeatures.h"
#include "ParameterProcess.h"
#include "RunLog.h"
#include "Str.h"
#include "svm.h"


#ifndef  _SVMPARAM_
class  SVMparam;
typedef  SVMparam*  SVMparamPtr;
#endif

#ifndef  _PARAMETERJOB_
class  ParameterJob;
typedef  ParameterJob*  ParameterJobPtr;
#endif


class  ParameterSearchBinaryCombo
{
public:
  ParameterSearchBinaryCombo ();
  
  ~ParameterSearchBinaryCombo ();

  void  Block ();

  String  CommandLineStr  ();
 
 
  void    EndBlock ();

  void    exit_with_help ();

  void    LetsDoIt ();

  #ifndef  WIN32
  double  LoadAverage ();
  #endif;


  void    LoadCurrentStatusFile ();

  void    ParseCommandLine (int    argc, 
                            char **argv
                           );

  void    ProcessCmdLineStr  (String  cmdLine);

  void    ProcessNextJob (ParameterProcessPtr   curProcess,
                          ParameterJobPtr       curJob,
                          ImageFeaturesListPtr  twoClassImages,
                          ImageClassListPtr     twoClasses
                         );

  void    ProcessTwoClassCombo (ParameterProcessPtr  curProcess);

  void    ProcessStatusFile (int argc,  char **argv);

  void    ReadProblem ();

  void    UpdateCurrentStatusFile ();


private:

  ImageClassList       imageClasses;


  int                  numOfClasses;
  ImageClassPtr*       masterClassList;   // This list will containg the same imageClass objects
                                          // as  imageClasses, but will always be in the same order
                                          // as the one indicated in the status file.

  ImageFeaturesList    images;

  String               inputFileName;
  int                  lockFile;
  RunLog               log;
  int                  maxIndex;
  int                  nr_fold;
  SVMparamPtr          svmParamaters;

  String               resultFileName;
  FILE*                statusFile;

  ParameterProcessListPtr  processes;

  double               highestAccuracySoFar;

  int                  ourProcessNum;

  int                  numOfProcesses;

  int                  nextClass0IDX;
  int                  nextClass1IDX;


  #ifndef  WIN32
  bool                 weWereSleeping;
  #endif;
};
#endif