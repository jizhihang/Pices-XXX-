#include "FirstIncludes.h"

#include <stdlib.h>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"

using namespace  std;


#include "BasicTypes.h"
#include "BMPImage.h"
#include "OSservices.h"
#include "Raster.h"
#include "Str.h"
using namespace  KKU;

#include "DataBase.h"
#include "DataBaseServer.h"
#include "FeatureFileIOC45.h"
#include "FeatureFileIOPices.h"
#include "HashTable.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "NormalizationParms.h"
#include "TrainingProcess2.h"
#include "Classifier2.h"
using namespace  MLL;


#include  "ExtractFeatures.h"


// K:\v1\Plankton\SipperImages2001Cruise\D01_D03_11a_c1  UnKnown  K:\v1\Plankton\SipperImages2001Cruise\D01_D03_11a_c1\D01_D03_11a_c1.data

// -c GrayScaleExtended.cfg -s K:\v1\Plankton\SipperImages2002Cruise -n C:\Release_2\Apps\CrossValidation\SipperImages2002Cruise2.data  -d K:\v1\Plankton\GrayScaleMachineClassified

// -c GrayScaleExtended.cfg -s K:\v1\Plankton\SipperImages2002Cruise\hrs020408  -n C:\Release_2\Apps\CrossValidation\SipperImages2002Cruise2.data  -d K:\v1\Plankton\GrayScaleMachineClassified  -f

// -c GrayScaleExtended.cfg -s K:\v1\Plankton\SipperImages2002Cruise\hrs0206  -n K:\v1\Plankton\SipperImages2002Cruise\hrs0206\hrs0206.data  -d K:\v1\Plankton\GrayScaleMachineClassified_hrs0206 

//  -c GrayScaleExtended.cfg -s K:\v1\Plankton\SipperImages2002Cruise\hrs0206  -n K:\v1\Plankton\SipperImages2002Cruise\hrs0206\hrs0206.data  -d K:\v1\Plankton\GrayScaleMachineClassified\hrs0206  -f

//  -c OverAll.cfg -s    C:\Temp\hrs020601_300_C_drive  -n K:\v1\Plankton\GrayScaleMachineClassified\hrs0206\hrs0206.data  -d K:\v1\Plankton\GrayScaleMachineClassified\hrs0206  -l K:\v1\Plankton\GrayScaleMachineClassified\hrs0206\hrs0206_FeatureExtraction.log

//  -c OverAll.cfg -s    K:\v1\Plankton\SipperImages2002Cruise\hrs0202  -n K:\v1\Plankton\GrayScaleMachineClassified\hrs0202\hrs0202.data  -d K:\v1\Plankton\GrayScaleMachineClassified\hrs0202  -l K:\v1\Plankton\GrayScaleMachineClassified\hrs0202\hrs0202_FeatureExtraction.log -f

//  -s    K:\v1\Plankton\SipperImages2002Cruise\hrs0202  -n K:\v1\Plankton\GrayScaleMachineClassified\hrs0202\hrs0202.data    -f

//  -c C:\Release_2\Apps\CrossValidation\AndrewGradedImagesSum.cfg  -s K:\v1\Plankton\SipperImages2002Cruise\hrs0207\hrs020701  -n K:\v1\Plankton\GrayScaleMachineClassified\hrs0202\hrs0202.data  -f  -d c:\temp\ClassifiedImages


// -c TestActiveLearning.cfg  -s C:\Test\SourceImages    -r  K:\Plankton\StatusReports\2003-Jun-26\ActiveLearnPass1.txt  -d C:\Test\ClassifiedImages  -l Test.log   -t C:\Test\TestImages  -f

// -c TestActiveLearning.cfg  -s C:\Test\SourceImages    -r  K:\Plankton\StatusReports\2003-Jun-26\ActiveLearnPass3.txt  -d C:\Test\ClassifiedImages  -l Test.log   -t C:\Test\TestImages 


// -c TestActiveLearning.cfg  -s C:\T`est\SourceImages    -r  K:\Plankton\StatusReports\2003-Jun-26\ActiveLearnPass2.txt  -d C:\Test\ClassifiedImages  -l Test.log   -t C:\Test\TestImages -f

// -s C:\users\kkramer\GradSchool\Plankton\ExtractedImages -r ImagesForPhore.txt  -n DataForPhore.data



// -n  C:\TrainingApp\TrainingImages\TrainingIages.data  -r C:\TrainingApp\TrainingImages\TrainingImages.data  -s C:\TrainingApp\TrainingImages\


// -s K:\PolutionParticles  -n K:\PolutionParticles\PolutionParticles  -r K:\PolutionParticles\PolutionParticles.txt

// -s C:\TrainingApp\TrainingImages2  -n C:\TrainingApp\TrainingImages2\AllData.data

// -s C:\Temp\GroundTruth  -n C:\Temp\GroundTruth\GroundTruth.data  -u


// -s C:\TrainingApp\etpB_training  -n C:\TrainingApp\etpB_training\etpB_training.data

// -s C:\TrainingApp\etpC_training_heirarchy  -n C:\TrainingApp\etpC_training_heirarchy\etpC_training_heirarchy.data  -r etpC_training_heirarchy.txt

// -s C:\TrainingApp\Temp  -n  C:\TrainingApp\Temp\Temp.data

// -s C:\Pices\SavedImages\SMP751001  -n C:\Pices\SavedImages\SMP751001\SMP751001_ValidatedExamples.data  -u

// -s D:\Pices\TrainingLibraries\ETP_08_SubSet  -n D:\Pices\TrainingLibraries\ETP_08_SubSet\ETP_08_SubSet.data.c45  -u



void  SetUpEmbeddedServer ()
{
  RunLog  log;
  DataBaseServer  dbs;
  dbs.Description ("Embedded");
  dbs.DataBaseName ("Pices_New");

  DataBasePtr  conn = new DataBase (&dbs, log);

  delete  conn;
  conn = NULL;
  return;
}  /* SetUpEmbeddedServer */


void  RandomlySelectImagesByClass (const KKStr&  srcRootDir,
                                   const KKStr&  destRootDir,
                                   int32         imagesPerClass
                                  )
{
  RunLog  log;
  bool  cancelFlag = false;
  MLClassConstListPtr  mlClasses = new MLClassConstList ();

  ImageFeaturesListPtr  images = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
                                   (srcRootDir, 
                                    *mlClasses,
                                    true,               // true = useDirectoryNameForClassName
                                    cancelFlag,
                                    true,               // rewiteRootFeatureFile 
                                    log
                                   );

  MLClassConstListPtr  classes = images->ExtractMLClassConstList ();

  MLClassConstList::iterator  idx;
  
  for  (idx = classes->begin ();  idx != classes->end ();  ++idx)
  {
    MLClassConstPtr  c = *idx;
    cout << endl << endl << "Class " << c->Name () << endl;

    ImageFeaturesListPtr  imagesThisClass = images->ExtractImagesForAGivenClass (c);
    imagesThisClass->RandomizeOrder ();

    KKStr  srcDirName  = osAddSlash (srcRootDir) + c->Name ();
    KKStr  destDirName = osAddSlash (destRootDir) + c->Name ();
    osCreateDirectoryPath (destDirName);

    ImageFeaturesListPtr  featureDataForThisClass = new ImageFeaturesList (imagesThisClass->FileDesc (), false, log);
    int  zed = Min (imagesPerClass, imagesThisClass->QueueSize ());
    int32  x = 0;
    ImageFeaturesList::iterator  idx2;
    for  (idx2 = imagesThisClass->begin (), x = 0;  ((idx2 != imagesThisClass->end ())  &&  (x < zed));  idx2++, ++x)
    {
      ImageFeaturesPtr i = *idx2;
      KKStr  srcFileName  = osAddSlash (srcDirName)  + osGetRootName (i->ImageFileName ()) + ".bmp";
      KKStr  destFileName = osAddSlash (destDirName) + osGetRootName (i->ImageFileName ()) + ".bmp";
      osCopyFile (srcFileName, destFileName);
      featureDataForThisClass->PushOnBack (i);
    }

    uint32  numWritten = 0;
    bool  cancelFlag = false;
    bool  successful = false;
    KKStr  errMsg;

    KKStr  featureDataFileName = osAddSlash (destDirName) + c->Name ();
    FeatureFileIOPices::Driver ()->SaveFeatureFile 
          (featureDataFileName, 
           featureDataForThisClass->AllFeatures (),
           *featureDataForThisClass,
           numWritten,
           cancelFlag,
           successful,
           log
          );

    delete featureDataForThisClass;
    featureDataForThisClass = NULL;
  }

  delete  classes;       classes      = NULL;
  delete  mlClasses;  mlClasses = NULL;
}  /* RandomlySelectImagesByClass */



SipperFileListPtr  GetListOfSipperFiles (DataBasePtr           dbConn,
                                         ImageFeaturesListPtr  examples,
                                         RunLog&               log
                                        )
{
  map<KKStr,KKStr>  sipperFiles;
  map<KKStr,KKStr>::iterator  sipperFilesIdx;
  SipperFileListPtr files = new SipperFileList (true);

  ImageFeaturesList::iterator  idx;
  for  (idx = examples->begin ();  idx != examples->end ();  idx++)
  {
    ImageFeaturesPtr  i = *idx;
    KKStr  imageFileName =  i->ImageFileName ();
    KKStr   sipperFileName;
    uint32  scanLineNum = 0;
    uint32  scanCol     = 0;

    i->ParseImageFileName (imageFileName, sipperFileName, scanLineNum, scanCol);
    sipperFilesIdx = sipperFiles.find (sipperFileName);
    if  (sipperFilesIdx == sipperFiles.end ())
    {
      sipperFiles.insert (pair<KKStr, KKStr>(sipperFileName, sipperFileName));
      SipperFilePtr sf = dbConn->SipperFileRecLoad (sipperFileName);
      if  (!sf)
      {
        sf = new SipperFile (sipperFileName);
        sf->CtdExt0 ("TRN");
        sf->CtdExt1 ("OXG");
        sf->CtdExt2 ("FLO");
        sf->CtdExt3 ("TUR");

        sf->Sp0 (Instrument::LookUpByShortName ("CTD"));
        sf->Sp1 (Instrument::LookUpByShortName ("P-R"));
        sf->Sp2 (Instrument::LookUpByShortName ("BAY"));

        dbConn->SipperFileInsert (*sf);
      }

      files->PushOnBack (sf);
    }
  }

  return  files;
}  /* GetListOfSipperFiles */



void    UpdateInstrumentDataTables (DataBasePtr        dbConn,
                                    SipperFileListPtr  sipperFiles,
                                    RunLog&            runLog
                                   )
{
  bool  cancelFlag = false;
  SipperFileList::iterator idx;
  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  ++idx)
  {
    SipperFilePtr sf = *idx;
    KKStr  sipperFileName = sf->SipperFileName ();
    dbConn->ImagesEraseSipperFile (sipperFileName);

    InstrumentDataListPtr  instrumentData =  InstrumentDataFileManager::GetInstrumentDataForSipperFile 
          (sipperFileName, sf, false, cancelFlag, runLog);

    if  (instrumentData)
      dbConn->InstrumentDataSaveListForOneSipperFile (sipperFileName, *instrumentData, cancelFlag);
  }
}  /* UpdateInstrumentDataTables */




void  ImportImagesIntoDataBase (const KKStr&  rootDir)

{
  RunLog  runLog;

  InstrumentDataFileManager::InitializePush ();

  DataBaseServerPtr  dbs = new  DataBaseServer ();
  dbs->Description ("Embedded");
  dbs->HostName     ("Embedded");
  dbs->UserName     ("root");
  dbs->PassWord     ("dasani30");
  dbs->DataBaseName ("pices_new");
  DataBasePtr  dbConn = new DataBase (dbs, runLog);


  MLClassConstList  classes;
  bool cancelFlag = false;
  ImageFeaturesListPtr  data = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
    (rootDir,
     classes,
     true,               //useDirectoryNameForClassName,
     cancelFlag,
     true,               //_rewiteRootFeatureFile,
     runLog
     );

  if  (!data)
  {
    cerr  << "" << endl;
    return;
  }

  SipperFileListPtr  sipperFiles = GetListOfSipperFiles (dbConn, data, runLog);
  UpdateInstrumentDataTables (dbConn, sipperFiles, runLog);
  
  int32  count = 0;
  int32  totalImages = data->QueueSize ();
  ImageFeaturesList::iterator  idx;
  for  (idx = data->begin ();  idx != data->end ();  idx++)
  {
    ImageFeaturesPtr  i = *idx;

    ++count;

    bool  validFile = true;
    KKStr  imageFileName = osAddSlash (rootDir) + i->ImageFileName ();
    RasterPtr  r = new Raster (imageFileName, validFile);
    if  (!validFile)
    {
      cerr << endl << "Invalid File[" << imageFileName << "]" << endl;
    }
    else
    {
      KKStr   sipperFileName;
      uint32  scanLineNum = 0;
      uint32  scanCol     = 0;

      i->ParseImageFileName (imageFileName, sipperFileName, scanLineNum, scanCol);

      //int32  size;
      //int32  weight;
      //float  rowCenter = 0.0f;
      //float  colCenter = 0.0f;
      //float  rowCenterWeighted;
      //float  colCenterWeighted;

      int64  byteOffset = 0;
      int32  imageId = -1;
      bool  successful = false;
      bool  cancelFlag = false;

      InstrumentDataPtr   id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, cancelFlag, runLog);
      if  (id)
        byteOffset = id->ByteOffset ();

      cout << count << " of " << totalImages << "\t" << imageFileName << endl;
    
      //r->CalcCentroid (size, weight, rowCenter, colCenter, rowCenterWeighted, colCenterWeighted);
      dbConn->ImageInsert (*r, 
                           osGetRootName (imageFileName), 
                           sipperFileName, 
                           byteOffset,
                           scanLineNum, 
                           scanCol,
                           (uint32)i->Length (),
                           (uint32)i->Width (),
                           (uint32)i->OrigSize (),
                           3     ,  // connectedPixelDist
                           0     ,  // extraction Log-Id
                           0     , // classification Log-Id
                           (uint32)i->SfCentroidRow (),
                           (uint32)i->SfCentroidCol (),
                           i->MLClass (),
                           1.0f,
                           NULL,
                           0.0f,
                           NULL,
                           i->Depth (),
                           0.0f,                    // ImageSize
                           NULL,                    // SizeCoordinates.
                           imageId,
                           successful
                          );

      dbConn->FeatureDataInsertRow (sipperFileName, *i);
    }
  }

  InstrumentDataFileManager::InitializePop ();
}  /* ImportImagesIntoDataBase */


void  FilterETP2008 ()
{
  RunLog  runLog;
  MLClassConstListPtr  classes = new MLClassConstList ();

  bool  cancelFlag  = false;
  bool  successful  = false;
  bool  changesMade = false;

  ImageFeaturesListPtr  data = (ImageFeaturesListPtr)FeatureFileIOPices::Driver ()->LoadFeatureFile 
     ("C:\\Pices\\Reports\\FeatureDataFiles\\ETP2008.data",
      *classes,
      -1,
      cancelFlag,
      successful,
      changesMade,
      runLog
     );

  if  (!data)
    return;

  ImageFeaturesListPtr  result = new ImageFeaturesList (data->FileDesc (), false, runLog, 10000);

  delete  classes;
  classes = data->ExtractMLClassConstList ();

  MLClassConstList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  ++idx)
  {
    MLClassConstPtr  c = *idx;

    ImageFeaturesListPtr  examplesInClass = data->ExtractImagesForAGivenClass (c);

    if  ((examplesInClass->QueueSize () > 199)  &&  (c->UpperName ().SubStrPart (0, 4) != "NOISE"))
      result->AddQueue (*examplesInClass);

    delete  examplesInClass;
    examplesInClass = NULL;
  }

  bool  valid = false;

  FeatureNumList  selFeatures (result->FileDesc (), 
                               "1,9,16,18-23,25,26,28,29,31,38-42,44-47,50,51,53,54,56,58-71,73-87",
                               valid
                              );
  uint32  numWritten = 0;

  FeatureFileIOC45::Driver ()->SaveFeatureFile
     ("C:\\Pices\\Reports\\FeatureDataFiles\\ETP2008_Filtered.data",
      selFeatures,
      *result,
      numWritten,
      cancelFlag,
      successful,
      runLog
     );

  NormalizationParms normParms (false, *result, runLog);
  normParms.NormalizeImages (result);

  FeatureFileIOC45::Driver ()->SaveFeatureFile
     ("C:\\Pices\\Reports\\FeatureDataFiles\\ETP2008_Filtered_Norm.data",
      selFeatures,
      *result,
      numWritten,
      cancelFlag,
      successful,
      runLog
     );


  delete  classes;  classes = NULL;
  delete  data;     data    = NULL;

} /* FilterETP2008 */



void  JohnsonParkerTest ()
{
  RunLog  log;

  MLClassConstList  mlClasses;

  bool  cancelFlag  = false;
  bool  sucessful   = false;
  bool  changesMade = false;

  KKStr  trainDataFileName = "E:\\Pices\\Reports\\FeatureData\\ETP2008_Filtered_Norm.data";
  KKStr  testDataFileName  = "E:\\Pices\\Reports\\FeatureData\\f18.m1.7clusters.data";

  FeatureVectorListPtr  trainData = FeatureFileIOC45::Driver ()->LoadFeatureFile
    (trainDataFileName,
     mlClasses,
     -1,
     cancelFlag,
     sucessful,
     changesMade,
     log
    );

  if  (!trainData)
  {
    log.Level (-1) << endl << endl 
      << "JohnsonParkerTest   ***ERROR***   Could not find file[" << trainDataFileName << "]." << endl
      << endl;
    return;
  }

  FileDescPtr  fileDesc = trainData->FileDesc ();

  KKStr  parameterStr = "-a 1  -c 25.11886  -d 3  -e 0.001  -g 0.01474560 -h 1  -i 0  -j 0  -k 0  -l 1  -m 100  -n 0.5  -p 0.1  -q 0  -r 0  -s 0  -t 2  -u 127.48761  -z 0  -MT OneVsOne  -SM Voting";
  TrainingConfiguration2*  config = TrainingConfiguration2::CreateFromFeatureVectorList (*trainData, parameterStr, log);
  if  (!config)
  {
    log.Level (-1) << endl << endl 
      << "JohnsonParkerTest   ***ERROR***   Error crating 'TrainingConfiguration2' config structure." << endl
      << endl;
    delete  trainData;
    trainData = NULL;
    return;
  }

  config->ImagesPerClass (500);

  bool  valid = false;
  FeatureNumList  selFeatures (fileDesc, "0-56", valid);
  config->SetFeatureNums (selFeatures);

  config->Save ("E:\\Pices\\Reports\\FeatureData\\ETP2008_Filtered_Norm.cfg");

  KKStr  statusMessage = "";

  TrainingProcess2 trainer (config, 
                            trainData, 
                            NULL,                   //_reportFile,
                            fileDesc,
                            log,
                            true,                   // true = _featuresAlreadyNormalized,
                            cancelFlag,
                            statusMessage
                          );

  trainer.CreateModelsFromTrainingData ();



  FeatureVectorListPtr  testData = FeatureFileIOC45::Driver ()->LoadFeatureFile
    (testDataFileName,
     mlClasses,
     -1,
     cancelFlag,
     sucessful,
     changesMade,
     log
    );
  if  (!testData)
  {
    log.Level (-1) << endl << endl 
      << "JohnsonParkerTest   ***ERROR***   Could not find file[" << testDataFileName << "]." << endl
      << endl;
    return;
  }

  delete  trainData;
}  /* JohnsonParkerTest */




void  main (int argc,  char** argv)
{
  //FilterETP2008 ();
  //exit (-1);

  //JohnsonParkerTest ();
  //exit (-1);

  //ImportImagesIntoDataBase ("E:\\Pices\\TrainingLibraries\\NRDA_test_200");
  //exit (-1);

  //RandomlySelectImagesByClass ("D:\\Pices\\TrainingLibraries\\etp_08",
  //                             "D:\\Pices\\TrainingLibraries\\NRDA_test_200",
  //                             200
  //                            );

  //RandomlySelectImagesByClass ("C:\\Pices\\TrainingLibraries\\NRDA_test_large",
  //                             "C:\\Pices\\TrainingLibraries\\NRDA_test_200",
  //                             200
  //                            );

  //RandomlySelectImagesByClass ("D:\\Pices\\TrainingLibraries\\NRDA_test_large",
  //                             "D:\\Pices\\TrainingLibraries\\NRDA_test_200",
  //                             200
  //                            );

  //RandomlySelectImagesByClass ("E:\\Pices\\TrainingLibraries\\NRDA_test_large",
  //                             "E:\\Pices\\TrainingLibraries\\NRDA_test_200",
  //                             200
  //                            );
  //exit (-1);
                               
  ExtractFeatures extractFeatures (argc, argv);
  if  (!extractFeatures.Abort ())
    extractFeatures.Extract ();
}

