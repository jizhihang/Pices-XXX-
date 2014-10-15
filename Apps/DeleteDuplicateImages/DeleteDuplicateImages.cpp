#include "FirstIncludes.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <sys/types.h>

#include "MemoryDebug.h"


using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "RunLog.h"
using namespace KKU;

#include "Instrument.h"
#include "InstrumentDataFileManager.h"
using namespace  SipperHardware;

#include "Application.h"
#include "Classifier2.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "MLClass.h"
#include "ImageFeatures.h"
using namespace MLL;



#include  "DeleteDuplicateImages.h"

// -ROOTDIR C:\TrainingApp\TrainingImages





DeleteDuplicateImages::DeleteDuplicateImages (int     argc,
                                              char**  argv
                                             ):
  Application  (argc, argv),

  cancelFlag   (false),
  mlClasses (),
  r            (NULL)
{
  ProcessCmdLineParameters (argc, argv);
  
  if  (rootDir.Empty ())
  {
    rootDir = osGetCurrentDirectory ();
  }

  if  (reportFileName.Empty ())
  {
    reportFileName = osAddSlash (rootDir) + osGetRootName (rootDir) + "_DupImagesReport.txt";
  }

  r = new ofstream (reportFileName.Str ());

  duplicateImagesDir = "C:\\Temp\\DuplicateImages\\";
  duplicateImagesDir << osGetRootName (rootDir);
  osCreateDirectoryPath (duplicateImagesDir);
}  /* DeleteDuplicateImages */



DeleteDuplicateImages::~DeleteDuplicateImages ()
{
  r->close ();
  delete  r;
}



bool  DeleteDuplicateImages::ProcessCmdLineParameter (char    parmSwitchCode, 
                                                      KKStr  parmSwitch, 
                                                      KKStr  parmValue
                                                     )
{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-ROOTDIR")    ||  
       (parmSwitch == "-ROOT")       ||
       (parmSwitch == "-SOURCEDIR")  ||
       (parmSwitch == "-RD")
      )
  {
    rootDir = parmValue;
    if  (!osValidDirectory (rootDir))
    {
      cerr << endl
           << endl
           << "*** ERROR ***  Invalid Root Dir[" << rootDir << "]." << endl;
      DisplayCmdLine ();
      Abort (true);
    }

  }

  else if  ((parmSwitch == "-R")      ||  
            (parmSwitch == "-REPORT") ||
            (parmSwitch == "-ReportFile")
           )
  {
    reportFileName = parmValue;
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */



void    DeleteDuplicateImages::DisplayCmdLine ()
{
  cout << endl
       << endl
       << "DeleteDuplicateImages"                                                         << endl
       << "           -RootDir  <Sub Directory Tree where images are located.>"           << endl
       << "           -ReportFile  <File to write report to/ defaults to root dir name.>" << endl
       << endl;
}  /* DisplayCmdLine */




void  DeleteDuplicateImages::DeleteImages ()
{
  *r << "Root Directory [" << rootDir               << "]" << endl;
  *r << "Report File    [" << reportFileName        << "]" << endl;
  *r << "Date           [" << osGetLocalDateTime () << "]" << endl;
  *r << endl;

  // We will first Load Images
  ImageFeaturesListPtr images = FeatureFileIOPices::LoadInSubDirectoryTree 
                                     (rootDir,
                                      mlClasses,
                                      false,
                                      cancelFlag,
                                      false,
                                      log
                                     );

  KKStr  rootFeatureFileName = osAddSlash (rootDir) + osGetRootName (rootDir) + ".data";
  //images->WriteImageFeaturesToFile (rootFeatureFileName, RawFormat, FeatureNumList::AllFeatures ());

  bool  successful = false;
  uint  numExamplesWritten = 0;
  FeatureFileIOPices::Driver ()->SaveFeatureFile (rootFeatureFileName, images->AllFeatures (), *images, numExamplesWritten, cancelFlag, successful, log);

  *r << "Class Statistics" << endl;
  *r << images->ClassStatisticsStr ();
  *r << endl;

  
  // We can now look for duplicates in list.
  DuplicateImagesPtr  dupLocator = new DuplicateImages (images, log);

  DuplicateImageListPtr  setsOfDupImages = dupLocator->DupImages ();

  *r << "Number of Duplicate Sets [" << setsOfDupImages->QueueSize () << "]" << endl;
  *r << endl;

  int  dupsDeleted        = 0;
  int  dupsFailedToDelete = 0;

  DuplicateImageList::iterator  dsIDX;
  for  (dsIDX = setsOfDupImages->begin ();  dsIDX != setsOfDupImages->end ();  ++dsIDX)
  {
    DuplicateImagePtr  dupSet = *dsIDX;

    ImageFeaturesListPtr  dups = new ImageFeaturesList (*(dupSet->DuplicatedImages ()), false);

    ImageFeaturesPtr      imageToKeep = (ImageFeaturesPtr)dupSet->ImageWithSmallestScanLine ();
    ImageFeaturesPtr      firstImage  = NULL;
    bool                  allTheSameClass = dupSet->AllTheSameClass ();
    if  (!allTheSameClass)
    {
      imageToKeep = NULL;
      *r << endl
         << "Images in this set of duplicates are of different classes, will delete all of them." << endl;
    }
    else
    {
      *r << endl
         << "Duplicate Class[" << imageToKeep->ClassName () << "]  FileName[" << imageToKeep->ImageFileName () << "]  Keeping." << endl;
    }

    for  (ImageFeaturesList::iterator iIDX = dups->begin ();  iIDX != dups->end ();  iIDX++)
    {
      ImageFeaturesPtr i = *iIDX;

      if  (!firstImage)
        firstImage = i;

      if  (i != imageToKeep)
      {
        KKStr  fullFileName = osAddSlash (rootDir) + i->ImageFileName ();

        *r << "          Class[" << i->ClassName () << "]  FileName[" << i->ImageFileName () << "]  ";

        if  (!duplicateImagesDir.Empty ())
        {
          // We will move image to duplicates directory
          int  dupCount = 0;
          KKStr newFileName;
          do {
            if  (imageToKeep)
            {
              newFileName = osAddSlash (duplicateImagesDir) + 
                            osGetRootName (imageToKeep->ImageFileName ()) 
                            + "-" +
                            osGetRootName (i->ImageFileName ());
            }
            else
            {
              newFileName = osAddSlash (duplicateImagesDir) + 
                            osGetRootName (firstImage->ImageFileName ()) 
                            + "-" +
                            osGetRootName (i->ImageFileName ());
            }
            if  (dupCount > 0)
              newFileName << "-" << dupCount;
            newFileName << ".bmp";
            dupCount++;
          }  while  (osFileExists (newFileName));

          osCopyFile (fullFileName, newFileName);
          *r << "moved to[" << newFileName << "]  ";
          cout << "Coping [" << fullFileName << "] to Duplicates List" << endl;
        }

        bool  deleted = osDeleteFile (fullFileName);

        if  (deleted)
        {
          *r << "* DELETED *" << endl;
          dupsDeleted++;
          images->DeleteEntry (i);
        }
        else
        {
          *r << "--- Failed To Delete ---" << endl;
          dupsFailedToDelete++;
        }
      }
    }
  }

  *r << endl;
  *r << "Duplicates Successfully Deleted[" << dupsDeleted        << "]" << endl;
  *r << "Duplicates Failed to Delete    [" << dupsFailedToDelete << "]" << endl;
  *r << endl;

  // We will write out the feature file again with the dup images removed.
  //images->WriteImageFeaturesToFile (rootFeatureFileName, RawFormat, FeatureNumList::AllFeatures ());
  numExamplesWritten= 0;
  FeatureFileIOPices::Driver ()->SaveFeatureFile (rootFeatureFileName, images->AllFeatures (), *images, numExamplesWritten, cancelFlag, successful, log);

  delete  dupLocator;  dupLocator = NULL;
  delete  images;      images     = NULL;
}  /* DeleteDuplicateImages */



#include  "TrainingConfiguration2.h"
#include  "TrainingProcess2.h"

int  main (int  argc,  char**  argv)
{
  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

    
  {
    /*

    RunLog  rl ("RunLog.txt");

    FileDescPtr fd = FileDesc::NewPlanktonFile (rl);

    KKU::KKStr  dirPath = "C:\\TrainingApp\\etpC_training_heirarchy";
    KKU::KKStr  configFileName = osGetRootNameOfDirectory (dirPath) + ".cfg";

    KKU::KKStr  statusMsg;

    KKU::KKStr  specificConfigFileName = TrainingConfiguration2::GetEffectiveConfigFileName (configFileName);

    bool    cancelFlag   = false;
    KKStr   errorMessage = "";
    bool    successful   = false;
    MLClassConstList  classes (rl);

    TrainingConfiguration2Ptr  config = 
      TrainingConfiguration2::CreateFromDirectoryStructure 
                                          (fd,
                                           &classes,
                                           specificConfigFileName,
                                           dirPath,
                                           rl,
                                           successful,
                                           errorMessage
                                          );

    delete  config;  config = NULL;

    TrainingProcessPtr trainer = 
            new TrainingProcess (configFileName, 
                                 fd, 
                                 rl, 
                                 false, 
                                 cancelFlag, 
                                 statusMsg
                                );

    if  (!trainer->Abort ())
    {
      MLClassConstList  classes (*(trainer->MLClasses ()));

      Classifier  classifier (trainer);

      bool  successful = false;

      MLClassPtr  uc = MLClass::GetUnKnownClass (rl);
      float*  probabilities = new float[1000];

      ImageFeaturesPtr  i = new ImageFeatures ("D:\\TrainingApp\\etpB_training\\chaetognath\\Station8A_02_00008888_2305.bmp", uc, successful);
      if  (successful)
      {
        classifier.ProbabilitiesByClass (classes, i, probabilities);

        for  (int x = 0;  x < classes.QueueSize (); x++)
          cout << classes[x].Name () << "\t" << probabilities[x] << endl;
        cout << endl;
      }

      cout << endl << endl;

      ImageFeaturesPtr  i2 = new ImageFeatures ("D:\\TrainingApp\\etpB_training\\gelatinous_globes\\Station8B_05_00863139_1161.bmp", uc, successful);
      if  (successful)
      {
        classifier.ProbabilitiesByClass (classes, i2, probabilities);

        for  (int x = 0;  x < classes.QueueSize (); x++)
          cout << classes[x].Name () << "\t" << probabilities[x] << endl;
        cout << endl;
      }
 
    }

    delete  trainer;  trainer = NULL;
    */

  }
  

  {
    DeleteDuplicateImages*  app = new DeleteDuplicateImages (argc, argv);
    if  (!(app->Abort ()))
      app->DeleteImages ();
    delete  app;  app = NULL;
  }
  
}  /* main */
