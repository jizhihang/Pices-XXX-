#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "BMPImage.h"
#include "HTMLReport.h"
#include "OSservices.h"
#include "Raster.h"
#include "Str.h"
using namespace  KKU;


#include "Classifier2.h"
#include "ConfusionMatrix2.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingProcess2.h"
#include "TrainingConfiguration2.h"
using namespace  MLL;


#include  "GradeClassification.h"




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

// GradeClassification  -c ETP08_BFS  -gt <Specify Directory where ground truth oimages are>  -R <Name of report file>

// -c GulfOilBroad2_Discreate_Dual.cfg  -gt E:\Pices\TrainingLibraries\GulfOilBroad2_Validation -r C:\Temp\GulfOilBroad2_Validation_Graded.txt

GradeClassification::GradeClassification (int argc, char**  argv) :
  Application              (argc, argv),
  cancelFlag               (false),
  config                   (NULL),
  configFileName           (),
  fileDesc                 (NULL),
  groundTruthDirName       (),
  groundTruth              (NULL),
  html                     (NULL),
  htmlFileName             (),
  sourceRootDirPath        (),
  mlClass               (NULL),
  mlClasses             (new MLClassConstList ()),
  report                   (NULL),
  reportFile               (NULL),
  reportFileName           ()

{
  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

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


  mlClass = MLClass::CreateNewMLClass ("UnKnown");


  if  (groundTruthDirName.Empty ())
  {
    log.Level (-1) << endl << endl << endl
                   << "-GroundTruth directory was not specified.  This is a mandatory parameter." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  if  (!osValidDirectory (groundTruthDirName))
  {
    log.Level (-1) << endl << endl << endl
                   << "-GroundTruth directory[" <<  groundTruthDirName << "] is not a valid directory."  << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  if  ((!configFileName.Empty ())  &&  (!sourceRootDirPath.Empty ()))
  {
    log.Level (-1) << endl << endl << endl
                   << "You can not specify both '-Config' and '-s' parameters."  << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }


  if  (configFileName.Empty ()  &&  sourceRootDirPath.Empty ())
  {
    config = TrainingConfiguration2::PromptForConfigurationFile (log);
    if  (config == NULL)
    {
      Abort (true);
      return;
    }

    configFileName = config->FileName ();
  }

  else if  (!configFileName.Empty ())
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


  if  (reportFileName.Empty ())
    reportFileName = osGetRootNameOfDirectory (groundTruthDirName) + "_GradingReport.txt";
  reportFileName  = osCreateUniqueFileName (reportFileName);
  reportFile = new ofstream (reportFileName.Str ());
  report = reportFile;
  
  if  (htmlFileName.Empty ())
    htmlFileName = osRemoveExtension (reportFileName) + ".html";
  html = new HTMLReport (htmlFileName, "Grading Report", HTMLReport::Center);

  *report << endl;
  *report << "------------------------------------------------------------------------" << endl;
  *report << "Run Date                [" << osGetLocalDateTime () << "]."  << endl;
  *report << "Report File Name        [" << reportFileName        << "]."  << endl;
  *report << "HTML File Name          [" << htmlFileName          << "]."  << endl;
  *report << "Source Root Directory   [" << sourceRootDirPath     << "]."  << endl;
  *report << "Ground Truch Directory  [" << groundTruthDirName    << "]."  << endl;
  *report << "Config File Name        [" << configFileName        << "]."  << endl;
  if  (config)
  {
    *report << "Model Type              [" << config->ModelTypeStr ()           << "]." << endl;
    *report << "SVM Parameters          [" << config->ModelParameterCmdLine ()  << "]." << endl;
    *report << "Num Hierarchial Levels  [" << config->NumHierarchialLevels  ()  << "]." << endl;
    if  (config->OtherClass ())
      *report << "Other Class             [" << config->OtherClass ()->Name ()    << "]." << endl;
  }

  *report << "------------------------------------------------------------------------" << endl;
  *report << endl;


  *html << endl
        << "<h1>Run Time Parameters</h1>" << endl
        << "<table align=\"center\" border=\"2\" cellpadding=\"4\">"                             << endl
        << "<caption>Run Time Parameters</caption>" << endl
        << "<tr><td>Run Time</td>"                << "<td>" << osGetLocalDateTime ()    << "</td></tr>" << endl
        << "<tr><td>Ground Truch Directory</td>"  << "<td>" << groundTruthDirName       << "</td></tr>" << endl
        << "<tr><td>Config File Name</td>"        << "<td>" << configFileName           << "</td></tr>" << endl;
 
  if  (config)
  {
    *html << "Model Type              [" << config->ModelTypeStr ()           << "]." << endl;
    *html << "SVM Parameters          [" << config->ModelParameterCmdLine ()  << "]." << endl;
    *html << "Num Hierarchial Levels  [" << config->NumHierarchialLevels  ()  << "]." << endl;
    if  (config->OtherClass ())
      *html << "Other Class             [" << config->OtherClass ()->Name ()    << "]." << endl;
  }

  *html << "<tr><td>Source Root Directory</td>"   << "<td>" << sourceRootDirPath        << "</td></tr>" << endl
        << "<tr><td>Report File Name</td>"        << "<td>" << reportFileName           << "</td></tr>" << endl
        << "<tr><td>HTML File Name</td>"          << "<td>" << htmlFileName             << "</td></tr>" << endl
        << "</table>" << endl
        << endl 
        << endl;
}




/******************************************************************************
 * Destructor
 ******************************************************************************/
GradeClassification::~GradeClassification ()
{
  if  (reportFile)
  {
    reportFile->close ();
    delete  reportFile;
  }

  delete  html;

  SummaryList::iterator  idx;
  for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
  {
    delete  idx->confussionMatrix;
    idx->confussionMatrix = NULL;
  }

  delete  groundTruth;  groundTruth = NULL;
  delete  config;       config      = NULL;
  //delete  mlClass;
}




/******************************************************************************
 * ProcessCmdLineParamters
 * DESC: Extracts parameters from the command line
 ******************************************************************************/
bool  GradeClassification::ProcessCmdLineParameter (char   parmSwitchCode, 
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

  else if  ((parmSwitch == "-SOURCEDIR")        ||  
            (parmSwitch == "-SOURCEDIRECTORY")  ||
            (parmSwitch == "-SD")               ||
            (parmSwitch == "-S")
           )
  {  
    sourceRootDirPath = parmValue;
  }

  else if  ((parmSwitch == "-HTML") || (parmSwitch == "-H"))
  {
    htmlFileName = parmValue;
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
void   GradeClassification::DisplayCommandLineParameters ()
{ 
  log.Level (0) << "GradeClassification  -r <xxx>  -gt <xxxx>  -c <config file>"             << endl;
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
  log.Level (0) << "    -s       <Sub-Directory of images to grade.>"                        << endl;
  log.Level (0) << "             Specify this instead of a config file.  Images in this"     << endl;
  log.Level (0) << "             directory will be graded against the GroundTruth"           << endl;
  log.Level (0) << "             directory."                                                 << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -H       HTML report file."                                          << endl;
  log.Level (0)                                                                              << endl;
  log.Level (0) << "    -GroundTruth  <Sub-Directory where ground truth images are>"         << endl;
}

// GradeClassification -gt D:\Pices\TrainingLibraries\oil  -s C:\temp\oil -r C:\temp\oil\OilGradingReport.txt





/******************************************************************************
 * Grade()
 * DESC: Builds a new data file in the root directory that reflects the currect 
 * location of all the images. This new data file is then compared to old one
 * and a grading report that indicates the number of classes correctly predicted
 * by the classifier is generated
 ******************************************************************************/
void  GradeClassification::Grade ()
{

  log.Level (10) << "GradeClassification::Grade   Loading Ground Truth"  << endl;

  
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
    {
      *html << "<br /><br /><br />"
            << "<h1>Duplicates Detected in Ground Truth</h1>" << endl
            << "<br />"
            << endl;
      dupDetector.ReportDuplicates (html->OStream ());
      *html << "<br /><br /><br />" << endl;
    }
  }

  if  (!configFileName.Empty ())
  {
    GradeUsingTrainingConfiguration ();
  }
  else
  {
    GradeSourceImagesAgainstGroundTruth ();
  }
}  /* Grade*/





void  GradeClassification::GradeSourceImagesAgainstGroundTruth ()
{
  log.Level (10) << "GradeClassification::GradeSourceImagesAgainstGroundTruth" << endl;

  // We were provided a GroundTruth directory and a SourceDirectory that is to be graded.  We 
  // only need to load in both directories and f=grade the SourceDirectory against the GroundTruth
  // Directory.

  ImageFeaturesListPtr  sourceImages = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
                          (sourceRootDirPath, 
                           *mlClasses, 
                           true,              // true = useDirectoryNameForClassName
                           cancelFlag, 
                           false,             // rewiteRootFeatureFile (Don't update root feature file)
                           log
                          );

  if  (!sourceImages)
  {
    log.Level (-1) << endl << endl << endl
                   << "GradedlClassification::GradeSourceImagesAgainstGroundTruth      ***ERROR***" << endl
                   << endl
                   << "               Could not load SourceImages[" << sourceRootDirPath << "]" << endl
                   << endl;
    Abort (true);
    return;
  }

  {
    DuplicateImages  dupDetector (sourceImages, log);
    if  (dupDetector.DuplicatesFound ())
    {
      *html << "<br /><br /><br />"
            << "<h1>Duplicates Detected in Source Images</h1>" << endl
            << "<br />"
            << endl;
      dupDetector.ReportDuplicates (html->OStream ());
      *html << "<br /><br /><br />" << endl;
    }
  }

  bool theyAreTheSame = true;
  ValidateThatBothListsHaveSameEntries (*groundTruth, *sourceImages, theyAreTheSame);

  GradeExamplesAgainstGroundTruth (sourceImages, groundTruth);
  
  ReportResults ();
  
  delete  sourceImages;
}  /* GradeSourceImagesAgainstGroundTruth */





void  GradeClassification::GradeUsingTrainingConfiguration ()
{
  log.Level (10) << "GradeClassification::GradeUsingTrainingConfiguration" << endl;

  delete  mlClasses;
  mlClasses = config->ExtractClassList ();

  bool  changesMadeToTrainingLibraries = false;

  KKU::DateTime  latestImageTimeStamp;

  log.Level (10) << "GradeUsingTrainingConfiguration  Loading Training Data." << endl;

  FeatureVectorListPtr  trainingData = config->LoadFeatureDataFromTrainingLibraries (latestImageTimeStamp, changesMadeToTrainingLibraries, cancelFlag);
  if  (!trainingData)
  {
    log.Level (-1) << endl << endl << endl
                   << "GradedlClassification::GradeUsingTrainingConfiguration      ***ERROR***" << endl
                   << endl
                   << "               Could not load training data file Configuration File[" << configFileName << "]" << endl
                   << endl
                   << endl;
    Abort (true);
    return;
  }

  uint  maxLevelsOfHiearchy = config->NumHierarchialLevels ();
  uint  hierarchyLevel = 0;

  while  (hierarchyLevel < maxLevelsOfHiearchy)
  {
    log.Level (10) << "GradeUsingTrainingConfiguration    Hierarchy Level[" << hierarchyLevel << "]" << endl;

    TrainingConfiguration2Ptr  configThisLevel = config->GenerateAConfiguraionForAHierarchialLevel (hierarchyLevel);

    FeatureVectorListPtr  trainingDataThisLevel = trainingData->ExtractExamplesForHierarchyLevel (hierarchyLevel);
    FeatureVectorListPtr  groundTruthThisLevel  = groundTruth->ExtractExamplesForHierarchyLevel (hierarchyLevel);
    FeatureVectorListPtr  groundTruthThisLevelClassified  = new FeatureVectorList (*groundTruthThisLevel, true);

    KKStr  statusMessage;

    TrainingProcess2 trainer (configThisLevel,
                              trainingDataThisLevel,
                              NULL,                               // No report file,
                              trainingDataThisLevel->FileDesc (),
                              log,
                              false,                              // false = features are not already normalized.
                              cancelFlag,
                              statusMessage
                             );
    trainer.CreateModelsFromTrainingData ();

    {
      Classifier2 classifier (&trainer, log);
      FeatureVectorList::iterator  idx;

      for  (idx = groundTruthThisLevelClassified->begin ();  idx != groundTruthThisLevelClassified->end ();  idx++)
      {
        FeatureVectorPtr  fv = *idx;
        MLClassConstPtr  ic = classifier.ClassifyAImage (*fv);
        fv->MLClass (ic);
      }
    }

    GradeExamplesAgainstGroundTruth (groundTruthThisLevelClassified, groundTruthThisLevel);

    delete  groundTruthThisLevelClassified;  groundTruthThisLevelClassified = NULL;
    delete  groundTruthThisLevel;            groundTruthThisLevel           = NULL;
    delete  trainingDataThisLevel;           trainingDataThisLevel          = NULL;

    hierarchyLevel++;
  }

  ReportResults ();

  delete  trainingData;
}  /* GradeUsingTrainingConfiguration */



void  GradeClassification::ReportResults ()
{
  *report << endl << endl << endl
          << "Summary by Training and Prediction Hierarchy Leveles" << endl
          << endl
          << "Train"  << "\t" << "Prediction" << "\t" << ""  << endl
          << "Level"  << "\t" << "Level"      << "\t" << "Acuracy" << endl;


  *html << "<h1>Summary of Results.</h1>" << endl
        << "<table  align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Summary of Results\">"     << endl
        << "  <thead>"                    << endl
        << "    <tr><th>Training<br />Level</th><th>Prediction<br />Level</th><th>Accuracy</th></tr>" << endl
        << "  </thead>" << endl
        << "  <tbody style=\"font-weight:normal; text-align:center; font-family:Courier\">"  << endl;


  SummaryList::iterator idx;
  for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
  {
    SummaryRec&  sr = *idx;
    KKStr accStr = StrFormatDouble (sr.confussionMatrix->Accuracy (), "ZZ0.00") + "%";

    *report << sr.tranLevel  << "\t"
            << sr.predLevel  << "\t"
            << accStr        << endl;

    *html << "    <tr>" << "<td>" << sr.tranLevel << "</td>" << "<td>" << sr.predLevel << "</td>" << "<td>" << "<a href=\"#" << sr.name << "\">" << accStr << "</a>" << "</td>" << "</tr>" << endl;
  }

  *html << "  </tbody>" << endl
        << "</table>"   << endl
        << "<br />"     << endl
        << "<br />"     << endl;

  
  
  *html << "<h1>Confusion Matrixes</h1>" << endl;

  for  (idx = resultsSummary.begin ();  idx != resultsSummary.end ();  idx++)
  {
    SummaryRec&  sr = *idx;

    *html << "<br />"     << endl
          << "<br />"     << endl
          << "<br />"     << endl
          << "<br />"     << endl;

    *html << "<h2><a name=\"" << sr.name << "\"/>Training Level[" << sr.tranLevel << "]  Prediction Level[" << sr.predLevel << "]" << "</h2>" << endl
          << "<br />" << endl;

    sr.confussionMatrix->PrintConfusionMatrixHTML (html->OStream ());
    *html << "<br />" << endl;
  }
}  /* ReportResults */



void  GradeClassification::ValidateThatBothListsHaveSameEntries (FeatureVectorList&  groundTruth, 
                                                                 FeatureVectorList&  examplesToGrade,
                                                                 bool&               theyAreTheSame
                                                                )
{
  theyAreTheSame = true;  // We will assume that they are the same to start with.

  int  missingExamplesToGrade = 0;
  int  missingGroundTruthExamples = 0;

  groundTruth.SortByRootName ();
  examplesToGrade.SortByRootName ();

  *report << endl << endl << endl
          << "Missing Examples To Grade" << endl
          << "=========================" << endl;

  ImageFeaturesList::iterator  idx;
  for  (idx = groundTruth.begin ();  idx !=  groundTruth.end ();  idx++)
  {
    FeatureVectorPtr  groundTruthExample = *idx;

    KKStr  rootName = osGetRootName (groundTruthExample->ImageFileName ());

    FeatureVectorPtr  exampleToGrade = examplesToGrade.LookUpByRootName (rootName);

    if  (!exampleToGrade)
    {
      theyAreTheSame = false;
      *report << rootName << "\t" << "*** MISSING ***" << "\t" << groundTruthExample->ImageFileName () << endl;
      missingExamplesToGrade++;
    }
  }

  if  (missingExamplesToGrade == 0)
  {
    *report  << "    *** None ***" << endl;
  }


  *report << endl << endl << endl
          << "Missing Ground Truth Examples" << endl
          << "=============================" << endl;

  for  (idx = examplesToGrade.begin ();  idx !=  examplesToGrade.end ();  idx++)
  {
    FeatureVectorPtr  exampleToGrade = *idx;

    KKStr  rootName = osGetRootName (exampleToGrade->ImageFileName ());

    FeatureVectorPtr  groundTruthExample = groundTruth.LookUpByRootName (rootName);

    if  (!groundTruthExample)
    {
      theyAreTheSame = false;
      *report << rootName << "\t" << "*** MISSING ***" << "\t" << exampleToGrade->ImageFileName () << "\t" << endl;
      missingGroundTruthExamples++;
    }
  }

  if  (missingGroundTruthExamples == 0)
  {
    *report << "   *** None ***" << endl;
  }
} /* ValidateThatBothListsHaveSameEntries */






void  GradeClassification::GradeExamplesAgainstGroundTruth (FeatureVectorListPtr  examplesToGrade,
                                                            FeatureVectorListPtr  groundTruth
                                                           )
{
  log.Level (10) << "GradeClassification::GradeExamplesAgainstGroundTruth" << endl;

  groundTruth->SortByRootName ();

  MLClassConstPtr  unknownClass = mlClasses->GetUnKnownClass ();

  MLClassConstListPtr classes = NULL;
  {
    MLClassConstListPtr examplesToGradeClasses = examplesToGrade->ExtractMLClassConstList ();
    MLClassConstListPtr groundTruthClasses     = groundTruth->ExtractMLClassConstList ();
    classes = MLClassConstList::MergeClassList (*examplesToGradeClasses, *groundTruthClasses);
    delete  examplesToGradeClasses;
    delete  groundTruthClasses;
  }

  uint16  maxHierarchialLevel = 0;
  {
    MLClassConstList::iterator  idx;
    for  (idx = classes->begin ();  idx != classes->end ();  idx++)
    {
      MLClassConstPtr  c = *idx;
      maxHierarchialLevel = Max (maxHierarchialLevel, c->NumHierarchialLevels ());
    }
  }

  // Create ConfusionMatrix objects for each posible level of Hierarchy.  The 'resultsSummary' vector will 
  // end up owning the instances of 'ConfusionMatrix2' and th edestructr will be responable for deleting them.
  uint  curLevel = 0;
  vector<ConfusionMatrix2Ptr>  cmByLevel;
  for  (curLevel = 0;  curLevel < maxHierarchialLevel;  curLevel++)
  {
    MLClassConstListPtr  classesThisLevel = classes->ExtractListOfClassesForAGivenHierarchialLevel (curLevel);
    ConfusionMatrix2Ptr  cm = new ConfusionMatrix2 (*classesThisLevel);
    cmByLevel.push_back (cm);
  }

  ConfusionMatrix2  cm (*classes);


  ImageFeaturesList::iterator  idx;

  for  (idx = examplesToGrade->begin ();  idx !=  examplesToGrade->end ();  idx++)
  {
    ImageFeaturesPtr  exampleToGrade = *idx;
    MLClassConstPtr  predictedClass = exampleToGrade->MLClass ();
    float          origSize       = exampleToGrade->OrigSize ();
    float          probability    = exampleToGrade->Probability ();

    KKStr  rootName = osGetRootName (exampleToGrade->ImageFileName ());
    FeatureVectorPtr  groundTruthExample = groundTruth->LookUpByRootName (rootName);
    MLClassConstPtr  groundTruthClass = unknownClass;
    if  (groundTruthExample)
      groundTruthClass = groundTruthExample->MLClass ();

    cm.Increment (groundTruthClass, predictedClass, (int)origSize, probability, log);

    for  (curLevel = 0;  curLevel < maxHierarchialLevel;  curLevel++)
    {
      MLClassConstPtr  groundTruthClasssThisLevel = groundTruthClass->MLClassForGivenHierarchialLevel (curLevel);
      MLClassConstPtr  predictedClassThisLevel    = predictedClass->MLClassForGivenHierarchialLevel   (curLevel);

      cmByLevel[curLevel]->Increment (groundTruthClasssThisLevel, predictedClassThisLevel, (int)origSize, probability, log);
    }
  }    


  //cm.PrintTrueFalsePositivesTabDelimited (*report);

  {
    // report Hierarchial results
    for  (curLevel = 0;  curLevel < maxHierarchialLevel;  curLevel++)
    {
      log.Level (10) << "GradeClassification::GradeExamplesAgainstGroundTruth   Printing Level[" << curLevel << "]" << endl;
      *report << endl << endl << endl
              << "Confusion Matrix   Training Level[" << maxHierarchialLevel << "]       Preduction Level[" << (curLevel + 1) << "]" << endl
              << endl;
      cmByLevel[curLevel]->PrintConfusionMatrixTabDelimited (*report);
      resultsSummary.push_back (SummaryRec (maxHierarchialLevel, curLevel + 1, cmByLevel[curLevel]));
    }

    *report << endl << endl << endl;
  }

  log.Level (10) << "GradeClassification::GradeExamplesAgainstGroundTruth     Exiting"  << endl;
}  /* GradeExamplesAgainstGroundTruth */
