#include "FirstIncludes.h"

#include <stdlib.h>
#include <fstream>
#include <memory>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include <strstream>
#include <string>
#include <vector>
#include <windows.h>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "BMPImage.h"
#include "HTMLReport.h"
#include "OSservices.h"
#include "Raster.h"
#include "Str.h"
using namespace  KKU;

#include "SipperVariables.h"
using namespace SipperHardware;

#include "Classifier2.h"
#include "ConfusionMatrix2.h"
#include "CrossValidation.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingProcess2.h"
#include "TrainingConfiguration2.h"
using namespace  MLL;


#include  "ClassCountSearch.h"



class  ClassCountSearch::ClassStats
{
public:
  ClassStats (MLClassConstPtr  _mlClass,
              double           _truePositives,
              double           _trueNegatives,
              double           _falsePositives,
              double           _falseNegatives
             )
  {
    mlClass     = _mlClass;
    truePositives  = _truePositives;
    trueNegatives  = _trueNegatives;
    falsePositives = _falsePositives;
    falseNegatives = _falseNegatives;
  }

  // http://www.cs.rpi.edu/~leen/misc-publications/SomeStatDefs.html


  double  FalsePositiveRatio ()  const
  {
    double  denominator = truePositives + falseNegatives;
    if  (denominator== 0.0)
      return 0.0;
    else
      return  falsePositives / denominator;
  }


  double  ClassCount ()  const
  {
    return  truePositives + falseNegatives;
  }



  double  Sensitivity ()  const
  {
    double  denominator = truePositives + falseNegatives;
    if  (denominator == 0.0)
      return 0.0;
    else
      return truePositives / denominator;
  }


  double  Type1Error ()  const  
  {
    double  denominator = falsePositives + trueNegatives;
    if  (denominator == 0.0)
      return 0.0;
    else
      return  falsePositives / denominator;
  }


  double  Type2Error ()  const  
  {
    double  denominator = truePositives + falseNegatives;
    if  (denominator == 0.0)
      return 0.0;
    else
      return  falseNegatives / denominator;
  }

  MLClassConstPtr  mlClass;

  double  truePositives;
  double  trueNegatives;
  double  falsePositives;
  double  falseNegatives;
};

typedef  ClassCountSearch::ClassStats*  ClassStatsPtr;



class  ClassCountSearch::ClassStatsList: KKQueue <ClassStats>
{
public:
  ClassStatsList (): KKQueue <ClassStats> ()
  {}


  void  Push (MLClassConstPtr  _mlClass,
              double         _truePositives,
              double         _trueNegatives,
              double         _falsePositives,
              double         _falseNegatives
             )
  {
    PushOnBack (new ClassStats (_mlClass, _truePositives, _trueNegatives, _falsePositives, _falseNegatives));
  }



  ClassStatsPtr  PopLargestType1Error ()
  {
    if  (size () < 1)
      return NULL;

    ClassStatsPtr  largestValue = NULL;

    iterator  idx;
    iterator  largestIdx;

    for  (idx = begin ();  idx != end ();  ++idx)
    {
      ClassStatsPtr  p = *idx;
      if  (largestValue == NULL)
      {
        largestValue = p;
        largestIdx = idx;
      }

      else if  (p->Type1Error () > largestValue->Type1Error ())
      {
        largestIdx = idx;
        largestValue = p;
      }
    }

    erase (largestIdx);
    return  largestValue;
  }  /* PopLargestType1Error */



  ClassStatsPtr  PopLargestFalsePositives ()
  {
    if  (size () < 1)
      return NULL;

    ClassStatsPtr  largestValue = NULL;

    iterator  idx;
    iterator  largestIdx;

    for  (idx = begin ();  idx != end ();  ++idx)
    {
      ClassStatsPtr  p = *idx;
      if  (largestValue == NULL)
      {
        largestValue = p;
        largestIdx = idx;
      }

      else if  (p->falsePositives > largestValue->falsePositives)
      {
        largestIdx = idx;
        largestValue = p;
      }
    }

    erase (largestIdx);
    return  largestValue;
  }  /* PopLargestFalsePositives */



  ClassStatsPtr  PopSmallestCount ()
  {
    if  (size () < 1)
      return NULL;

    ClassStatsPtr  s = NULL;

    iterator  idx;
    iterator  smallestIdx;

    ClassStatsPtr  smallestValue = NULL;

    for  (idx = begin ();  idx != end ();  ++idx)
    {
      ClassStatsPtr  p = *idx;
      if  (smallestValue == NULL)
      {
        smallestValue = p;
        smallestIdx = idx;
      }

      else if  (p->ClassCount () < smallestValue->ClassCount ())
      {
        smallestIdx = idx;
        smallestValue = p;
      }
    }

    erase (smallestIdx);
    return  smallestValue;
  }  /* PopLargestType1Error */



  ClassStatsPtr  PopLeastSensitive ()
  {
    if  (size () < 1)
      return NULL;

    ClassStatsPtr  s = NULL;

    iterator  idx;
    iterator  smallestIdx;

    ClassStatsPtr  smallestValue = NULL;

    for  (idx = begin ();  idx != end ();  ++idx)
    {
      ClassStatsPtr  p = *idx;
      if  (smallestValue == NULL)
      {
        smallestValue = p;
        smallestIdx = idx;
      }

      else if  (p->Sensitivity () < smallestValue->Sensitivity ())
      {
        smallestIdx = idx;
        smallestValue = p;
      }
    }

    erase (smallestIdx);
    return  smallestValue;
  }  /* PopLargestType1Error */









};  /* ClassStatsList */




typedef  ClassCountSearch::ClassStatsList*  ClassStatsListPtr;



// -s C:\TrainingApp\ClassifiedImages\hrs020409  -r Gradereport.txt

/******************************************************************************
 * command line parameters that have been used for testing purposes
 ******************************************************************************/

// -s "C:\TrainingApp\ClassifiedImages" -r "report.txt"

// -s C:\Temp\sennai_sorted   -gt C:\Temp\GroundTruth  -report  Sennai.txt

//  -s C:\Temp\hrs020503 -r c:\hrs020503_GradeReport.txt

// -s C:\temp\hrs020503_auto  -gt C:\temp\hrs020503_GroundTruth

// -s D:\TrainingApp\ExtractedImages\hrs020505_auto  -gt D:\TrainingApp\ExtractedImages\hrs020505_man

// -config WFS.cfg  -gt C:\PICES\ExtractedImages\hrs020505_man

// -config WFS_BinaryTuned -gt D:\TrainingApp\ExtractedImages\hrs020501_man

// -c Oil.cfg    -gt C:\Pices\TrainingLibraries\oil-test  -R c:\Temp\GradeReport.txt

// ClassCountSearch  -c ETP08_BFS  -gt <Specify Directory where ground truth oimages are>  -R <Name of report file>

// -c GulfOilBroad2_Discreate_Dual.cfg  -gt E:\Pices\TrainingLibraries\GulfOilBroad2_Validation -r C:\Temp\GulfOilBroad2_Validation_Graded.txt

// -c USF2_MFS



ClassCountSearch::ClassCountSearch (int argc, 
                                    char**  argv
                                   ):
  Application            (argc, argv),
  cancelFlag             (false),
  config                 (NULL),
  configFileName         (),
  fileDesc               (NULL),
  groundTruthDirName     (),
  groundTruth            (NULL),
  groundTruthClasses     (NULL),
  mlClasses           (new MLClassConstList ()),
  report                 (NULL),
  report1Type1Errors     (NULL),
  reportFileName         (),
  trainExamples          (NULL),
  trainExamplesClasses   (NULL)

{
  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  osRunAsABackGroundProcess ();

  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

  if  (argc < 2)
  {
    log.Level (-1) << endl << endl << endl
                   << "No parameters specified." << endl
                   << endl;
    Abort (true);
    DisplayCommandLineParameters ();
    return;
  }

  if  (configFileName.Empty ())
  {
    log.Level (-1) << endl << endl << endl
                   << "Configuration file was not specified."  << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  else 
  {
    config = new TrainingConfiguration2 (fileDesc, configFileName, log, true);
    if  (!(config->FormatGood ()))
    {
      log.Level (-1) << endl << endl << endl
                     << "Configuration file[" << configFileName << "] is not valid." << endl
                     << endl;
      DisplayCommandLineParameters ();
      Abort (true);
      return;
    }
  }

  if  (!groundTruthDirName.Empty ()  &&  (!osValidDirectory (groundTruthDirName)))
  {
    log.Level (-1) << endl << endl << endl
                   << "-GroundTruth directory[" <<  groundTruthDirName << "] is not a valid directory."  << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  fileDesc = config->FileDesc ();

  if  (reportFileName.Empty ())
  {
    KKStr  reportDir = osAddSlash (SipperVariables::PicesReportDir ()) + "ClassCountSearch";
    KKU::osCreateDirectoryPath (reportDir);
    reportFileName = osAddSlash (reportDir) + osGetRootNameOfDirectory (configFileName) + "_ClassCountSearch.txt";
  }

  reportFileName  = osCreateUniqueFileName (reportFileName);
  report = new ofstream (reportFileName.Str ());

  KKStr  type1ErrorReportName = KKU::osRemoveExtension (reportFileName) + "_Sensitivity.txt";
  report1Type1Errors = new ofstream (type1ErrorReportName.Str ());
  
  *report << endl;
  *report << "------------------------------------------------------------------------" << endl;
  *report << "Run Date                [" << osGetLocalDateTime () << "]."  << endl;
  *report << "Report File Name        [" << reportFileName        << "]."  << endl;
  *report << "Config File Name        [" << configFileName        << "]."  << endl;
  *report << "Ground Truch Directory  [" << groundTruthDirName    << "]."  << endl;
  if  (config)
  {
    *report << "Model Type              [" << config->ModelTypeStr ()           << "]." << endl;
    *report << "SVM Parameters          [" << config->ModelParameterCmdLine ()  << "]." << endl;
    if  (config->OtherClass ())
      *report << "Other Class             [" << config->OtherClass ()->Name ()    << "]." << endl;
  }

  *report << "------------------------------------------------------------------------" << endl;
  *report << endl;

  *report1Type1Errors << endl;
  *report1Type1Errors << "------------------------------------------------------------------------" << endl;
  *report1Type1Errors << "Run Date                [" << osGetLocalDateTime () << "]."  << endl;
  *report1Type1Errors << "Report File Name        [" << reportFileName        << "]."  << endl;
  *report1Type1Errors << "Config File Name        [" << configFileName        << "]."  << endl;
  *report1Type1Errors << "Ground Truch Directory  [" << groundTruthDirName    << "]."  << endl;
  if  (config)
  {
    *report1Type1Errors << "Model Type              [" << config->ModelTypeStr ()           << "]." << endl;
    *report1Type1Errors << "SVM Parameters          [" << config->ModelParameterCmdLine ()  << "]." << endl;
    if  (config->OtherClass ())
      *report1Type1Errors << "Other Class             [" << config->OtherClass ()->Name ()    << "]." << endl;
  }
  *report1Type1Errors << "------------------------------------------------------------------------" << endl;
  *report1Type1Errors << endl;
}




/******************************************************************************
 * Destructor
 ******************************************************************************/
ClassCountSearch::~ClassCountSearch ()
{
  if  (report)
  {
    report->close ();
    delete  report;
    report = NULL;
  }

  if  (report1Type1Errors)
  {
    report1Type1Errors->close ();
    delete  report1Type1Errors;
    report1Type1Errors = NULL;
  }

  delete  groundTruth;           groundTruth          = NULL;
  delete  groundTruthClasses;    groundTruthClasses   = NULL;
  delete  config;                config               = NULL;
  delete  trainExamples;         trainExamples        = NULL;
  delete  trainExamplesClasses;  trainExamplesClasses = NULL;
  //delete  mlClass;
}




/******************************************************************************
 * ProcessCmdLineParamters
 * DESC: Extracts parameters from the command line
 ******************************************************************************/
bool  ClassCountSearch::ProcessCmdLineParameter (char   parmSwitchCode, 
                                                 KKStr  parmSwitch, 
                                                 KKStr  parmValue
                                                )
{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-R")  ||  (parmSwitch == "-REPORT"))
  {
    reportFileName = parmValue;
  }

  else if  ((parmSwitch == "-C")  ||  (parmSwitch == "-CONFIG")  ||  (parmSwitch == "-CONFIGFILE"))
  {
    configFileName = parmValue;
  }

  else if  ((parmSwitch == "-GROUNDTRUTH") || (parmSwitch == "-GT"))
  {
    groundTruthDirName = parmValue;
  }

  else
  {
    log.Level (-1) << endl  << endl
                   << "Unrecognized parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */




/******************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   ClassCountSearch::DisplayCommandLineParameters ()
{ 
  log.Level (0) << "ClassCountSearch  -r <xxx>  -gt <xxxx>  -c <config file>"                << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -config  <Configuration File Name.>"                                 << endl;
  log.Level (0) << "             Works in conjunction with '-GroundTruth'. It will build"    << endl;
  log.Level (0) << "             a classifier using the training data specified in the"      << endl;
  log.Level (0) << "             configuration file.   It will then classify all the images" << endl;
  log.Level (0) << "             in the ground truth and grade.  It will build and grade"    << endl;
  log.Level (0) << "             for each level of hierarchy."                               << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -r       <Report File Name>,  Defaults to Command Line."             << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -GroundTruth  <Sub-Directory where ground truth images are>"         << endl;
}

// ClassCountSearch -gt D:\Pices\TrainingLibraries\oil  -s C:\temp\oil -r C:\temp\oil\OilGradingReport.txt





/******************************************************************************
 ******************************************************************************/
void  ClassCountSearch::Main ()
{
  log.Level (10) << "ClassCountSearch::Grade   Loading Ground Truth"  << endl;

  if  (!groundTruthDirName.Empty ())
  {
    groundTruth = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
                        (groundTruthDirName, 
                         *mlClasses, 
                         true,             // true = useDirectoryNameForClassName
                         cancelFlag, 
                         false,            // false = don't rewiteRootFeatureFile 
                         log
                        );

    {
      DuplicateImages  dupDetector (groundTruth, log);
      if  (dupDetector.DuplicatesFound ())
        dupDetector.ReportDuplicates (*report);
    }

    groundTruthClasses = groundTruth->ExtractMLClassConstList ();
  }

  DateTime  latestImageTimeStamp;
  bool      changesMadeToTrainingLibraries = false;

  {
    KKU::DateTime  latestImageTimeStamp;
    trainExamples = config->LoadFeatureDataFromTrainingLibraries (latestImageTimeStamp, changesMadeToTrainingLibraries, cancelFlag);
    if  (!trainExamples)
    {
      log.Level (-1) << endl
        << "ClassCountSearch::Main   ***ERROR***   Failed to load trainig examples for model: " << config->FileName () << endl << endl;
      exit (-1);
    }
  }

  trainExamplesClasses = trainExamples->ExtractMLClassConstList ();
  trainExamplesClasses->SortByName ();


  KKStr  l1, l2, l3;
  trainExamplesClasses->ExtractThreeTitleLines (l1, l2, l3);

  *report1Type1Errors << ""       << "\t" << ""      << "\t" << ""         << "\t" << ""        << "\t" << ""         << "\t" << "Class"   << "\t" << "Class"      << "\t" << l1 << endl;
  *report1Type1Errors << "Error"  << "\t" << "Class" << "\t" << "Number"   << "\t" << "Number"  << "\t" << ""         << "\t" << "Wtd"     << "\t" << "With >"     << "\t" << l2 << endl;
  *report1Type1Errors << "Type"   << "\t" << "Count" << "\t" << "Examples" << "\t" << "Ignored" << "\t" << "Accuracy" << "\t" << "Equally" << "\t" << "Type1Error" << "\t" << l3 << endl;

  combinationsToTry.push (new MLClassConstList (*trainExamplesClasses));

  while  (combinationsToTry.size () > 0)
  {
    MLClassConstListPtr  nextClassCombo = combinationsToTry.front ();
    combinationsToTry.pop ();
    ProcessClassCombo (nextClassCombo);
    delete  nextClassCombo;
    nextClassCombo = NULL;
  }

}  /* Main */




ClassStatsListPtr  ClassCountSearch::ComputeClassStatsList (ConfusionMatrix2Ptr  cm)
{
  ClassStatsListPtr  stats = new ClassStatsList ();

  const MLClassConstList&  classes = cm->MLClasses ();

  MLClassConstList::const_iterator  idx;
  for  (idx = classes.begin ();  idx != classes.end ();  ++idx)
  {
    MLClassConstPtr  ic = *idx;

    double  truePositives  = 0.0;
    double  trueNegatives  = 0.0;
    double  falsePositives = 0.0;
    double  falseNegatives = 0.0;

    cm->ComputeFundamentalStats (ic, truePositives, trueNegatives, falsePositives, falseNegatives);

    // http://www.cs.rpi.edu/~leen/misc-publications/SomeStatDefs.html
    
    stats->Push (ic, truePositives, trueNegatives, falsePositives, falseNegatives);
  }

  return   stats;
}  /* ComputeClassStatsList */



void  ClassCountSearch::ProcessClassCombo (MLClassConstListPtr  classes)
{
  log.Level (10) << "ClassCountSearch::ProcessClassCombo" << endl;

  classes->SortByName ();
 
  *report << endl << endl << endl
          << "*****************************************************************************************************************************************************************" << endl
          << "*****************************************************************************************************************************************************************" << endl;

  *report << "ClassCount" << "\t" << classes->QueueSize ()         << endl;
  *report << "Classes"    << "\t" << classes->ToTabDelimitedStr () << endl;

  int32 numExamples = 0;
  int32 numIgnored  = 0;

  ConfusionMatrix2Ptr cm = GradeClassList (classes, numExamples, numIgnored);
  cm->PrintConfusionMatrixTabDelimited (*report);
  cm->PrintTrueFalsePositivesTabDelimited (*report);
  *report << endl;

  ClassStatsListPtr  stats =  ComputeClassStatsList (cm);

  //ClassStatsPtr  nextCandidate = stats->PopLargestFalsePositives ();
  //ClassStatsPtr  nextCandidate = stats->PopSmallestCount ();
  ClassStatsPtr  nextCandidate = stats->PopLeastSensitive ();

  ostringstream	 type2Errors;
  ostringstream	 falsePositivesStr;
  ostringstream	 sensitivityStr;

  *report1Type1Errors << "Type1Error"     << "\t" << classes->QueueSize () << "\t" << numExamples << "\t" << numIgnored << "\t" << cm->Accuracy () << "\t" << cm->AccuracyNorm () << "\t" << nextCandidate->mlClass->Name ();
  type2Errors         << "Type2Error"     << "\t" << classes->QueueSize () << "\t" << numExamples << "\t" << numIgnored << "\t" << cm->Accuracy () << "\t" << cm->AccuracyNorm () << "\t" << nextCandidate->mlClass->Name ();
  falsePositivesStr   << "FalsePositives" << "\t" << classes->QueueSize () << "\t" << numExamples << "\t" << numIgnored << "\t" << cm->Accuracy () << "\t" << cm->AccuracyNorm () << "\t" << nextCandidate->mlClass->Name ();
  sensitivityStr      << "FalsePositives" << "\t" << classes->QueueSize () << "\t" << numExamples << "\t" << numIgnored << "\t" << cm->Accuracy () << "\t" << cm->AccuracyNorm () << "\t" << nextCandidate->mlClass->Name ();

  MLClassConstList::iterator  idx;
  for  (idx = trainExamplesClasses->begin ();  idx != trainExamplesClasses->end ();  ++idx)
  {
    MLClassConstPtr  ic = *idx;
    if  (classes->PtrToIdx (ic) < 0)
    {
      *report1Type1Errors << "\t" << "********";
      type2Errors         << "\t" << "********";
      falsePositivesStr   << "\t" << "********";
      sensitivityStr      << "\t" << "********";
    }
    else
    {
      double  tp, tn, fp, fn;
      cm->ComputeFundamentalStats (ic, tp, tn, fp, fn);

      double  type1Error = 0.0;
      double  denominator = fp + tn;
      if  (denominator == 0.0)
        type1Error = 0.0;
      else
        type1Error = fp / denominator;

      double  type2Error = 0;
      denominator = (fn + tp);
      if  (denominator != 0.0)
        type2Error = fn / denominator;

      double  sensitivity = 0.0;
      denominator = tp + fn;
      if  (denominator != 0.0)
        sensitivity = tp / denominator;

      *report1Type1Errors << "\t" << type1Error;
      type2Errors         << "\t" << type2Error;
      falsePositivesStr   << "\t" << fp;
      sensitivityStr      << "\t" << sensitivity;
    }
  }

  *report1Type1Errors << endl;
  type2Errors         << endl;
  falsePositivesStr   << endl;
  *report1Type1Errors << type2Errors.str ();
  *report1Type1Errors << falsePositivesStr.str ();
  *report1Type1Errors << sensitivityStr.str ();

  if  (classes->QueueSize () > 3)
  {
    MLClassConstListPtr  nextClassList = new MLClassConstList (*classes);
    nextClassList->DeleteEntry (nextCandidate->mlClass);
    combinationsToTry.push (nextClassList);
  }

  delete  nextCandidate;  nextCandidate = NULL;
  delete  stats;          stats = NULL;
  delete  cm;             cm    = NULL;
}  /* ProcessClassCombo */



ConfusionMatrix2Ptr  ClassCountSearch::GradeClassList (MLClassConstListPtr  classes,
                                                       int32&             numExamples, 
                                                       int32&             numIgnored
                                                       )
{
  log.Level (10) << "ClassCountSearch::GradeClassList" << endl;

  numExamples = 0;
  numIgnored  = 0;

  classes->SortByName ();
  
  TrainingConfiguration2Ptr  specificConfig = new TrainingConfiguration2 (*config);

  specificConfig->EmptyTrainingClasses ();

  FeatureVectorListPtr  specificTrainExamples = new FeatureVectorList (fileDesc, false, log, 1000);
  FeatureVectorListPtr  specificGroundTruthExamples = NULL;

  if  (groundTruth)
    specificGroundTruthExamples = new FeatureVectorList (fileDesc, false, log, 1000);
  
  MLClassConstList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  ++idx)
  {
    MLClassConstPtr  ic = *idx;
    specificConfig->AddATrainingClass (ic);

    {
      FeatureVectorListPtr  trainExamplesThisClass = trainExamples->ExtractImagesForAGivenClass (ic);
      specificTrainExamples->AddQueue (*trainExamplesThisClass);
      delete  trainExamplesThisClass;  trainExamplesThisClass = NULL;
    }

    if  (groundTruth)
    {
      FeatureVectorListPtr  groundTruthExamplesThisClass = groundTruth->ExtractImagesForAGivenClass (ic);
      specificGroundTruthExamples->AddQueue (*groundTruthExamplesThisClass);
      delete  groundTruthExamplesThisClass;
      groundTruthExamplesThisClass = NULL;
    }
  }


  if  (!specificGroundTruthExamples)
  {
    FeatureVectorListPtr  stratified = specificTrainExamples->StratifyAmoungstClasses (10);
    delete  specificTrainExamples;
    specificTrainExamples = stratified;
    stratified = NULL;
  }

  CrossValidation  cv (specificConfig, 
                       specificTrainExamples,
                       classes,
                       10,                     // _numOfFolds,
                       false,                  // false = Not already normalized.
                       fileDesc,
                       log,
                       cancelFlag
                      );

  if  (specificGroundTruthExamples)
  {
    numExamples = specificGroundTruthExamples->QueueSize ();
    numIgnored  = groundTruth->QueueSize () - numExamples;
    cv.RunValidationOnly (specificGroundTruthExamples, NULL);
  }
  else
  {
    numExamples = specificTrainExamples->QueueSize ();
    numIgnored  = trainExamples->QueueSize () - numExamples;
    cv.RunCrossValidation ();
  }

  ConfusionMatrix2Ptr  cm = cv.GiveMeOwnershipOfConfusionMatrix ();

  delete  specificTrainExamples;        specificTrainExamples       = NULL;
  delete  specificGroundTruthExamples;  specificGroundTruthExamples = NULL;
  delete  specificConfig;               specificConfig              = NULL;
  
  return cm;
}  /* GradeUsingTrainingConfiguration */




void  main (int     argc,
            char**  argv
           )
{
  ClassCountSearch  app (argc, argv);
  if  (!app.Abort ())
    app.Main ();
}