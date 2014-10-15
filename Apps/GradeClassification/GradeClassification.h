#ifndef  __GRADE_CLASSIFICATION_H__
#define  __GRADE_CLASSIFICATION_H__

#include "Application.h"
#include "HTMLReport.h"
using namespace  KKU;

#include "ConfusionMatrix2.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingConfiguration2.h"
using  namespace  MLL;




class  GradeClassification: public Application
{
public:
  typedef  KKU::uint  uint;

  GradeClassification (int argc, char**  argv);
  ~GradeClassification ();

  void  Grade();

  virtual const char* ApplicationName ()  {return  "GradeClassification";}

  void  DisplayCommandLineParameters ();

  virtual bool ProcessCmdLineParameter (char    parmSwitchCode, 
                                        KKStr   parmSwitch, 
                                        KKStr   parmValue
                                       );

private:

  void       GradeSourceImagesAgainstGroundTruth ();

  void       GradeExamplesAgainstGroundTruth (FeatureVectorListPtr  examplesToGrade,
                                              FeatureVectorListPtr  groundTruth
                                             );

  void       GradeUsingTrainingConfiguration ();

 
  void       ReportResults ();


  void       ValidateThatBothListsHaveSameEntries (FeatureVectorList&  groundTruth, 
                                                   FeatureVectorList&  examplesToGrade,
                                                   bool&               theyAreTheSame
                                                  );

  bool                      cancelFlag;


  TrainingConfiguration2Ptr  config;
  KKStr                     configFileName;

  FileDescPtr               fileDesc;

  KKStr                     sourceRootDirPath;
  KKStr                     groundTruthDirName;
  ImageFeaturesListPtr      groundTruth;

  MLClassConstPtr        mlClass;
  MLClassConstListPtr    mlClasses;

  ostream*                  report;
  ofstream*                 reportFile;
  HTMLReportPtr             html;
  KKStr                     reportFileName;
  KKStr                     htmlFileName;



  struct  SummaryRec
  {
    ConfusionMatrix2Ptr  confussionMatrix;
    KKStr                 name;
    uint                  predLevel;
    uint                  tranLevel;

    SummaryRec (uint                  _tranLevel,
                uint                  _predLevel,
                ConfusionMatrix2Ptr  _confussionMatrix
                ):
     confussionMatrix (_confussionMatrix),
     predLevel        (_predLevel),
     tranLevel        (_tranLevel)
     {
       name = "CM" + StrFormatInt (tranLevel, "00") + "-" + StrFormatInt (predLevel, "00");
     }
  };

  typedef  vector<SummaryRec> SummaryList;
  SummaryList  resultsSummary;
};

#endif