#ifndef  __CLASSCOUNTSEARCH__
#define  __CLASSCOUNTSEARCH__

#include "Application.h"
#include "HTMLReport.h"
using namespace  KKU;

#include "ConfusionMatrix2.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingConfiguration2.h"
using  namespace  MLL;

class  ClassCountSearch: public Application
{
public:
  typedef  KKU::uint  uint;

  class  ClassStats;
  typedef  ClassStats*  ClassStatsPtr;

  class  ClassStatsList;
  typedef  ClassStatsList*  ClassStatsListPtr;


  ClassCountSearch (int argc, char**  argv);
  ~ClassCountSearch ();

  void  Main ();

  virtual const char* ApplicationName ()  {return  "ClassCountSearch";}

  void  DisplayCommandLineParameters ();

  virtual bool ProcessCmdLineParameter (char    parmSwitchCode, 
                                        KKStr   parmSwitch, 
                                        KKStr   parmValue
                                       );

private:
  MLClassConstPtr  SelectNextClassToRemove (ConfusionMatrix2Ptr  cm);

  ConfusionMatrix2Ptr  GradeClassList (MLClassConstListPtr  classes,
                                       int32&             numExamples, 
                                       int32&             numIgnored
                                      );

  ClassStatsListPtr  ComputeClassStatsList (ConfusionMatrix2Ptr  cm);

  void       ProcessClassCombo (MLClassConstListPtr  classes);

  void       ReportResults ();



  bool                        cancelFlag;

  TrainingConfiguration2Ptr   config;
  KKStr                       configFileName;

  FileDescPtr                 fileDesc;

  KKStr                       groundTruthDirName;
  FeatureVectorListPtr        groundTruth;
  MLClassConstListPtr      groundTruthClasses;

  MLClassConstListPtr      mlClasses;

  ofstream*                   report;
  ofstream*                   report1Type1Errors;
  KKStr                       reportFileName;

  FeatureVectorListPtr        trainExamples;
  MLClassConstListPtr           trainExamplesClasses;

  queue<MLClassConstListPtr>    combinationsToTry;           

};

#endif