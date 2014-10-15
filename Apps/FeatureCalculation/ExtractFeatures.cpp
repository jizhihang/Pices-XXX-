#include  "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


#include "MemoryDebug.h"


using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "Str.h"
using namespace  KKU;

#include "InstrumentDataFileManager.h"
using namespace  SipperHardware;

#include "Classifier2.h"
#include "DuplicateImages.h"
#include "FileDesc.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "TrainingConfiguration2.h"
#include "TrainingProcess2.h"
using namespace  MLL;



#include  "ExtractFeatures.h"



// Used on 2004-09-19
// -c "C:\TrainingApp\DataFiles\TrainingModels\working model.cfg"   -S K:\v1\Plankton\ExtractedImages   -n K:\v1\Plankton\ExtractedImages\ExtractedImages.data  

// Used on 2004-09-21
// -c "C:\TrainingApp\DataFiles\TrainingModels\ECOHAB test.cfg"   -S K:\v1\Plankton\ExtractedImages   -n K:\v1\Plankton\ExtractedImages\ExtractedImages.data    -r K:\v1\Plankton\ExtractedImages\ExtractedImagesReport.txt  -l K:\v1\Plankton\ExtractedImages\ExtractedImagesLog.txt  -d K:\v1\Plankton\ClassifiedImages

// 2004-09-23
// Just running agains Training Libraries,  to make sure Featue Data files are up to date,  and get Class Stats
// -S C:\TrainingApp\TrainingImages  -n C:\TrainingApp\TrainingImages\AllTrainingImages.data  -r C:\TrainingApp\TrainingImages\AllTrainingImagesReport.txt

// -s C:\TrainingApp\ClassifiedImages\hrs020607  -U
// -s D:\TrainingApp\etpC_training  -n D:\TrainingApp\etpC_training\etpC_training.data

// -s D:\Pices\TrainingLibraries\SMP751001  -n D:\Pices\TrainingLibraries\SMP751001\Oil.data  -u
// -s D:\Pices\TrainingLibraries\Oil_SMP751001037_22  -n D:\Pices\TrainingLibraries\Oil_SMP751001037_22\Oil_SMP751001037_22.data
// -s I:\Pices\SMP751001\  -n I:\Pices\may_BP_2010\SMP751001_34_35_37.data -u
// 2011-02-21: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\temp\SMP751001 -n I:\Pices\TrainingLibraries\temp\all_but_34_35_37.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\binary_testset\SMP751001 -n I:\Pices\TrainingLibraries\binary_testset\binary_all_but_34_35_37.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\binary_testset\SMP751001_SMP_binary -n I:\Pices\TrainingLibraries\binary_testset\binary_all_but_34_35_37.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\binary_trainset\SMP751001 -n I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\SMP751001_SMP_NOT34_35_37 -n I:\Pices\TrainingLibraries\36ClassSMPNot34_35_37.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20 -n I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20.data -u
// 2011-03-01: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\SMP751001_44Classes_NOT34_35_37 -n I:\Pices\TrainingLibraries\44ClassNOT_34_35_37.data -u
// 2011-03-08: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset -n I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset.data -u
// 2011-03-08: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset -n I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset.data -u

// 2011-03-15: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\NON_SMP751001_BINARY -n I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -u

// 2011-03-15: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\2011-03-15-two-stageEXP-1stStage -n I:\Pices\TrainingLibraries\2011-03-15-two-stageEXP-1stStage-missclassified_other.data -u
// 2011-03-15: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_expanded -n I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_expanded.data -u

// 2011-03-22: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\2ndStage-2011-03-22 -n I:\Pices\TrainingLibraries\2ndStage-2011-03-22.data -u

// 2011-03-22: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\binary_trainset\SMP751001 -n I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -u
// 2011-03-28: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\NON_SMP751001_BINARY -n I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -u
// 2011-03-28: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20 -n I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -u

// 2011-03-28: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES -n I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -u

// 2011-04-05: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\2ndStage-2011-04-05\ -n I:\Pices\TrainingLibraries\2ndStage-2011-04-05.data -u
// 2011-04-05: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\2ndStage-TESTSET-2011-04-05\ -n I:\Pices\TrainingLibraries\2ndStage-TESTSET-2011-04-05.data -u

// 2011-04-12: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSampling\ -n I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSampling.data -u
// 2011-04-12: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10\ -n I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -u
// 2011-04-12: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingBinary\ -n I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingBinary.data -u

// 2011-04-12: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES\ -n I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -u
// 2011-04-12: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\NON_SMP751001_BINARY -n I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -u

//////////----not valid// 2011-05-16: c:\Pices\exe>ExtractFeaturesApp.exe -s I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20 -n I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_100percent.data -u

// 2011-05-22: -s I:\Pices\TrainingLibraries\RandomSampling27-36Classes -n I:\Pices\TrainingLibraries\RandomSampling27-36Classes.data -u
// 2011-05-22: -s I:\Pices\TrainingLibraries\NON_SMP751001_24-36CLASSES -n I:\Pices\TrainingLibraries\NON_SMP751001_24-36CLASSES.data -u
// 2011-05-22: -s I:\Pices\TrainingLibraries\RandomSampling2ndStage -n I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -u

// 2011-05-23: -s I:\Pices\TrainingLibraries\MissclassNonOil -n I:\Pices\TrainingLibraries\MissclassNonOil.data -u

// -s C:\Pices\TrainingLibraries\Gulf_2011 -n C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Gulf_2011\Gulf_2011.data  -u -r C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\Gulf_2011\Gulf_2011_ExtractionReport.txt

ExtractFeatures::ExtractFeatures (int     argc,
                                  char**  argv
                                 ):
  Application (argc, argv),

  cancelFlag                   (false),
  classifier                   (NULL),
  fileDesc                     (FeatureFileIOPices::NewPlanktonFile (log)),
  mlClass                      (MLClass::CreateNewMLClass ("UnKnown")),
  mlClasses                    (new MLClassConstList ()),
  images                       (NULL),
  report                       (NULL),
  sourceRootDirPath            (),
  statusMessage                (),
  successful                   (true),
  trainer                      (NULL),
  trainingConfiguration        (NULL),
  useDirectoryNameForClassName (false)

{
  ProcessCmdLineParameters (argc, argv);

  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }

  if  (sourceRootDirPath.Empty ())
  {
    sourceRootDirPath = osGetCurrentDirectory ();
  }

  if  (featureFileName.Empty ())
  {
    KKStr  path, root, extension;
    osParseFileName (sourceRootDirPath, path, root, extension);
    featureFileName = path;
    osAddLastSlash (featureFileName);
    featureFileName << root + DS + root + ".data";
  }

  {
    KKStr  driveLetter, path, root, extension;

    osParseFileSpec (featureFileName, driveLetter, path, root, extension);

    rootFeatureFileName = root;
    if  (extension.Empty ())
      rootFeatureFileName << ".data";
    else
      rootFeatureFileName << "." << extension;
  }


  if  (reportFileName.Empty ())
  {
    reportFile = NULL;
    report = &cout;
  }
  else
  {
    reportFile = new ofstream (reportFileName.Str ());
    report = reportFile;
  }

  *report << "Extract Traning Feature Data from Images." << endl;

  *report << "Configuration File         [" << configFileName      << "]."  << endl;
  *report << "Destination Directory      [" << destDirectory       << "]."  << endl;
  *report << "Feature File Name          [" << featureFileName     << "]."  << endl;
  *report << "Report File Name           [" << reportFileName      << "]."  << endl;
  *report << "Source Root Directory      [" << sourceRootDirPath   << "]."  << endl;
  *report << "Use Dir Name as Class Name [" <<   (useDirectoryNameForClassName ? "Yes" : "No") << "]." << endl;


  if  (!configFileName.Empty ())
  {
    // Will need Training Configuration if doing Active Learning Testing.
    trainingConfiguration = new TrainingConfiguration2 (fileDesc, configFileName, log);
    delete  mlClasses;
    mlClasses = trainingConfiguration->ExtractClassList ();

    trainer = new TrainingProcess2 (configFileName, 
                                    NULL,
                                    fileDesc,
                                    log, 
                                    report, 
                                    false,             // false = DONT force model rebuild.
                                    false,
                                     cancelFlag,
                                    statusMessage
                                   );

    if  (trainer->Abort ())
    {
      log.Level (-1) << "ExtractFeatures    *** ERROR ***" << endl;
      log.Level (-1) << "          Error Creating Training Models" << endl << endl;
      delete  trainer;
      trainer = NULL;
      return;
    }

    trainer->ReportTraningClassStatistics (*report);

    log.Level (10) << "ExtractFeatures   Done with CreateModelsFromTrainingData" << endl;

    classifier = new Classifier2 (trainer, log);
    if  (classifier->Abort ())
    {
      log.Level (-1) << endl;
      log.Level (-1) << "ExtractFeatures    *** Error Creating Classifier ***" << endl << endl;

      delete  classifier;
      delete  trainer;
      classifier = NULL;
      trainer    = NULL;
      return;
    }
  }
}  /* ExtractFeatures */






ExtractFeatures::~ExtractFeatures ()
{
  if  (reportFile)
  {
    reportFile->close ();
    delete  reportFile;
  }

  delete  trainingConfiguration;
  delete  images;
  //delete  mlClass;
  delete  classifier;
  delete  trainer;
}



// -c "C:\TrainingApp\DataFiles\TrainingModels\working model.cfg"   -S K:\v1\Temp\ExtractedImages  -D K:\v1\Temp\ClassifiedImages  -n c:\Temp\ExtractedData.data

bool  ExtractFeatures::ProcessCmdLineParameter (char   parmSwitchCode, 
                                                KKStr  parmSwitch, 
                                                KKStr  parmValue
                                               )
{
  switch  (parmSwitchCode)
  {
    case  'C':
    case  'c':  // Configuration File Specification.
         if  (parmValue.Empty ())
         {
           log.Level (-1) << "ExtractFeatures::ProcessCmdLineParameter *** ERROR Missing Configuration Parameter ***" 
                          << endl;
           Abort (true);
           return  false;
         }

         configFileName = parmValue;
         break;


    case  'D':  // Destination Directory
    case  'd': 
         destDirectory = parmValue;
         if  (!osValidDirectory (&destDirectory))
         {
           if  (!osCreateDirectory (destDirectory))
           {
             log.Level (-1) << "ProcessCmdLineParameter **** Invalid Destination Directory["
                            << destDirectory << "] ****" 
                            << endl;
             Abort (true);
             return  false;
           }

           if  ((destDirectory.LastChar () != ':')  &&  (destDirectory.LastChar () != DSchar))
           {  
             destDirectory << DS;
           }

         }
         break;


    case  'N':
    case  'n':
         featureFileName = parmValue;
         break;


    case  'R':
    case  'r':
         reportFileName = parmValue;
         break;


    case  'S':
    case  's':
         sourceRootDirPath = parmValue;
         break;

    case  'U':
    case  'u':
         useDirectoryNameForClassName = true;
         break;
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */


// -s D:\Pices\TrainingLibraries\NRDA_test_200  -u


void   ExtractFeatures::DisplayCommandLineParameters ()
{
  log.Level (0) << "ExtractFeatures  -c <xxx> -d <xxx>  -f <xxxx>  -n <xxxx>  -s <xxxx>" << endl;
  log.Level (0) << endl;
  log.Level (0) << "    -c  Specify the Configuration file to use if you want"              << endl; 
  log.Level (0) << "        to classify images."                                            << endl;
  log.Level (0)                                                                             << endl;
  log.Level (0) << "    -d  Destination Diretory Tree,  If specified a copy of Classified." << endl;
  log.Level (0) << "        images will be placed here."                                    << endl;   
  log.Level (0)                                                                             << endl;
  log.Level (0) << "    -n  Name of File to store Calculated Features in."                  << endl;
  log.Level (0)                                                                             << endl;
  log.Level (0) << "    -r  Report File,  Defaults to Command Line."                        << endl;
  log.Level (0)                                                                             << endl;
  log.Level (0) << "    -s  Source Directory Tree to Search for images."                    << endl;
  log.Level (0)                                                                             << endl;
  log.Level (0) << "    -u  Use   Directory Name  as   Class Name"                          << endl;
}  /* DisplayCommandLineParameters */


// -n  C:\TrainingApp\TrainingImages\TrainingIages.data  -r C:\TrainingApp\TrainingImages\TrainingImages.txt  -s C:\TrainingApp\TrainingImages\
// -n  C:\Temp\TestImage\TestImages.data  -r C:\Temp\TestImages\TestImages.txt  -s C:\Temp\TestImages

// -s D:\TrainingApp\etpB_training  




void  ExtractFeatures::ClassifyImage (ImageFeaturesPtr  image)
{
  if  (!classifier)
    return;

  MLClassConstPtr  mlClass;
  double         probability;
  int            numOfWinners;
  bool           knownClassOneOfTheWinners;
  double         breakTie;

  mlClass = classifier->ClassifyAImage (*image, 
                                           probability, 
                                           numOfWinners,
                                           knownClassOneOfTheWinners,
                                           breakTie
                                          );

  MLClassConstPtr  ourClass = mlClasses->GetMLClassPtr (mlClass->Name ());

  if  (mlClass)
  {
    image->MLClass (ourClass);
    image->Probability ((FFLOAT)probability);
  }
}  /* ClassifyImage */



void  ExtractFeatures::ReportDuplicates ()
{
  if  (!images)
  {
    log.Level (-1) << endl << endl
      << "ExtractFeatures::ReportDuplicates    (images == NULL)." << endl
      << endl;
    return;
  }

  DuplicateImages duplicateChecker (images, log);

  if  (!duplicateChecker.DuplicatesFound ())
  {
    *report << endl
            << endl 
            << "***      There were no duplicate File Names found.     ***"  << endl
            << endl
            << endl;
  }
  else
  {
    log.Level (-1) << endl << "   ***ERROR***     Duplicate Images Detected." << endl << endl;
    *report << endl
            << endl 
            << "***  Duplicate Images Detected  ***"  << endl
            << endl;

    duplicateChecker.ReportDuplicates (*report);
    *report << endl;
  }

  duplicateChecker.PurgeDuplicates (images, report);

  
}  /* ReportDuplicates */




void  ExtractFeatures::MoveImagesToDestinationDirectoryByClassName ()
{
  images->SortByClass (false);

  ImageFeaturesList::iterator  iIDX;
  ImageFeaturesPtr  image = NULL;

  iIDX = images->begin ();
  image = *iIDX;  iIDX++;

  while  (image)
  {
    MLClassConstPtr  curClass = image->MLClass ();

    log.Level (20) << "MoveImagesToDestinationDirectoryByClassName   Class[" << curClass->Name () << "]." << endl;

    int  classSubDirectoryNum = 0;

    while  (image  &&  (curClass == image->MLClass ()))
    {
      ImageFeaturesListPtr  imagesInThisSubDir = new ImageFeaturesList (fileDesc, true, log, 1000);

      KKStr  classSubDirName (curClass->Name ());
      classSubDirName << "_" << StrFormatInt (classSubDirectoryNum, "z00");

      KKStr  classSubDirFullPathName (destDirectory);
      osAddLastSlash (classSubDirFullPathName);
      classSubDirFullPathName << classSubDirName;

      osAddLastSlash (classSubDirFullPathName);
      
      if  (!osCreateDirectoryPath (classSubDirFullPathName))
      {
        // For some reason we are unable to create this subdirectory.
        // we will terminate the app at this point.

        log.Level (-1) << endl
                       << endl
                       << "**** ERROR ****   MoveImagesToDestinationDirectoryByClassName" << endl
                       << endl
                       << "Unable to create Sub Directory[" << classSubDirFullPathName << "]." << endl
                       << endl;
        osWaitForEnter ();
        exit (-1);
      }

      while  (image  &&  (curClass == image->MLClass ())  &&  (imagesInThisSubDir->QueueSize () < 1000))
      {
        KKStr  sourceDirName (sourceRootDirPath);
        osAddLastSlash (sourceDirName);
        sourceDirName << osGetPathPartOfFile (image->ImageFileName ());
        KKStr  imageFileName (osGetRootNameWithExtension (image->ImageFileName ()));
        
        osMoveFileBetweenDirectories (imageFileName, sourceDirName, classSubDirFullPathName);

        ImageFeaturesPtr  imageInClassDir = new ImageFeatures (*image);
        imageInClassDir->ImageFileName (imageFileName);

        imagesInThisSubDir->PushOnBack (imageInClassDir);
        image = *iIDX;  iIDX++;
      }

      KKStr  featureFileName (classSubDirFullPathName);
      osAddLastSlash (classSubDirFullPathName);
      classSubDirFullPathName << classSubDirName << ".data";
      
      log.Level (20) << "MoveImagesToDestinationDirectoryByClassName  Writting out Feature File Name[" << classSubDirFullPathName << "]." << endl;

      uint  numExamplesWritten = 0;

      FeatureFileIOPices::Driver ()->SaveFeatureFile (classSubDirFullPathName, 
                                                      imagesInThisSubDir->AllFeatures (), 
                                                      *imagesInThisSubDir,
                                                      numExamplesWritten,
                                                      cancelFlag, 
                                                      successful,
                                                      log
                                                     );

      classSubDirectoryNum++;
      delete  imagesInThisSubDir;  imagesInThisSubDir = NULL;
    }
  }
}  /* MoveImagesToDestinationDirectoryByClassName */




void  ExtractFeatures::MoveOver_SMP751001037_22_ImagesIntoSeparateDir (ImageFeaturesListPtr  examples)
{
  KKStr  lastDestDir;
  ImageFeaturesList::iterator  idx;
  for  (idx = examples->begin ();  idx != examples->end ();  idx++)
  {
    ImageFeaturesPtr  i = *idx;
    //if  (i->ImageFileName ().LocateStr ("SMP751001037_22") >= 0)
    if  ((i->ImageFileName ().LocateStr ("SMP751001034") >= 0) || (i->ImageFileName ().LocateStr ("SMP751001035") >= 0) || (i->ImageFileName ().LocateStr ("SMP751001037") >= 0))
    {
      KKStr  srcFileName   = osAddSlash (sourceRootDirPath) + i->ImageFileName ();
      KKStr  destFileName  = "I:\\Pices\\may_BP_2010\\SMP751001_34_35_37\\" + i->ImageFileName ();
      KKStr  destDir = osGetPathPartOfFile (destFileName);
      if  (destDir != lastDestDir)
      {
        lastDestDir = destDir;
        osCreateDirectoryPath (destDir);
      }

      osCopyFile (srcFileName, destFileName);
      cout << i->ImageFileName () << endl;
    }
  }
}  /* MoveOver_SMP751001037_22_ImagesIntoSeparateDir */



void  ExtractFeatures::ClassifyImages ()
{
  int      c = 0;
  bool     knownClassOneOfTheWinners;
  int      numOfWinners;
  double   probability;
  double   breakTie;

  DateTime  classificationStartTime = osGetLocalDateTime ();
  double    startCPUsecs = osGetSystemTimeUsed ();


  MLClassConstPtr  ourClass       = NULL;
  MLClassConstPtr  predictedClass = NULL;
  ImageFeaturesPtr    image          = NULL;

  ImageFeaturesList::iterator  iIDX;

  for  (iIDX = images->begin ();   iIDX != images->end ();  iIDX++)
  {
    image = *iIDX;
    ImageFeaturesPtr  tempImage = new ImageFeatures (*image);

    predictedClass = classifier->ClassifyAImage (*tempImage, 
                                                 probability,
                                                 numOfWinners,
                                                 knownClassOneOfTheWinners,
                                                 breakTie
                                                );

    delete  tempImage;

    ourClass = mlClasses->GetMLClassPtr (predictedClass->Name ());
    if  (!ourClass)
      ourClass = mlClasses->GetUnKnownClass ();

    image->MLClass     (ourClass);
    image->PredictedClass (ourClass);
    image->Probability ((float)probability);
    image->BreakTie ((float)breakTie);

    c++;
    if  ((c % 1000) == 0)
      cout << "Number of Images Classified[" << c << "]" << endl;
  }

  DateTime  classificationEndTime = osGetLocalDateTime ();
  double    endCPUsecs  = osGetSystemTimeUsed ();
  double    cpuSecsUsed = endCPUsecs - startCPUsecs;

  DateTime  elaspedTime = classificationEndTime - classificationStartTime;
  
  double classificationsPerElaspedSec = 0.0;
  double classificationsPerCPUsec     = 0.0;
  if  (images->QueueSize () > 0)
  {
    classificationsPerElaspedSec = (double)images->QueueSize () / (double)elaspedTime.Seconds ();
    classificationsPerCPUsec     = (double)images->QueueSize () / (double)cpuSecsUsed;
  }

  *report << endl;
  *report << "Classification Stats" << endl;
  *report << "====================" << endl;
  *report << "Start Time    [" << classificationStartTime  << "]" << endl;
  *report << "End Time      [" << classificationEndTime    << "]" << endl;
  *report << "Elasped Time  [" << elaspedTime.Seconds ()   << "]"
          << "  Classifications Per Sec     [" << StrFormatDouble (classificationsPerElaspedSec, "ZZZ0.000") << "]" << endl;
  *report << "CPU Secs Used [" << cpuSecsUsed              << "]" 
          << "  Classifications Per CPU Sec [" << StrFormatDouble (classificationsPerCPUsec,     "ZZZ0.000") << "]" << endl;
  *report << endl;
}  /* ClassifyImages */



void  ExtractFeatures::AddInstrumentData (ImageFeaturesListPtr  data)
{
  ImageFeaturesList::iterator idx;
  bool  cancelFlag = false;

  for  (idx = data->begin ();  idx != data->end ();  idx++)
  {
    ImageFeaturesPtr fd = *idx;
      
    InstrumentDataPtr id = InstrumentDataFileManager::GetClosestInstrumentData (fd->ImageFileName (), cancelFlag, log);
    if  (id)
    {
      fd->FeatureData (2, id->Temperature ());
      fd->Oxygen       (id->Oxygen        ());
      fd->Salinity     (id->Salinity      ());
      fd->Fluorescence (id->Fluorescence  ());
      fd->Depth        (id->Depth         ());
    }
    else
    {
      log.Level (-1) << endl << "AddInstrumentData   ***ERROR***    Could not locate instrument data for Image[" << fd->ImageFileName () << "]." << endl << endl;
    }
  }
}  /* AddInstrumentData */




void  ExtractFeatures::Extract ()
{
  log.Level (10) << "ExtractFeatures::Extract" << endl;

  InstrumentDataFileManager::Initialize ();

  KKStr  relativeDir;

  images = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
                                   (sourceRootDirPath, 
                                    *mlClasses,
                                    useDirectoryNameForClassName,
                                    cancelFlag,
                                    true,    // rewiteRootFeatureFile 
                                    log
                                   );
    
  cout << endl << endl << endl;
  cout << "Done with  LoadInSubDirectoryTree." << endl;
  AddInstrumentData (images);


  {
    // One time routine to copy images that belong to SIPPER file 'SMP751001037_22'
    // into there own directory.
    //MoveOver_SMP751001037_22_ImagesIntoSeparateDir (images);
  }


  if  (classifier)
  {
    //Label each entry in images with predicted class.
    ClassifyImages ();
  }

  ReportDuplicates ();

  //  Report Image Class Stats
  *report << endl;
  *report << images->ClassStatisticsStr ();
  *report << endl;

  if  (!featureFileName.Empty ())
  {
    uint  numExamplesWritten = 0;
    FeatureFileIOPices::Driver ()->SaveFeatureFile (featureFileName, images->AllFeatures (), *images, numExamplesWritten, cancelFlag, successful, log);
  }

  if  ((classifier)  &&  (!destDirectory.Empty ()))
  {
    // Since we have a Classofoer and a Destination Directory, we are going to move the
    // images from there original location to a Destination Directory with sub-dircetories
    // by Class Name.
    MoveImagesToDestinationDirectoryByClassName ();
  }


  InstrumentDataFileManager::FinalCleanUp ();


  log.Level (10) << "ExtractFeatures::Extract    Exiting"  << endl;
}  /* Extract */
