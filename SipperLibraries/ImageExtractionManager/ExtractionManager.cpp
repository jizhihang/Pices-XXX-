/* ExtractionManager.cpp -- Manages the threads that perform the image extraction process.
 * Copyright (C) 2012 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in ImageExtractionManager.h
 */
#include "FirstIncludes.h"
#include <string>
#include <iostream>
#include <vector>
#include "MemoryDebug.h"
using namespace std;

#include "MsgQueue.h"
using namespace KKU;

#include "SipperVariables.h"
using namespace  SipperHardware;

#include "FeatureFileIOPices.h"
using namespace  MLL;

#include "DataBaseUpdateThread.h"
#include "ExtractionManager.h"
#include "ExtractionManagerStats.h"
#include "ExtractedImage.h"
#include "FrameExtractorThread.h"
#include "FrameProcessorThread.h"
#include "ImageExtractionThread.h"
#include "LogicalFrame.h"
#include "LogicalFrameQueue.h"
using  namespace  ImageExtractionManager;

// -s E:\Pices\SipperFiles\SMP7510010\SMP751001034\Part1\SMP751001034_02.spr  -d  E:\Pices\ExtractedImages\SMP7510010\SMP751001034_02\ -co -x -c NRDA_small.cfg  -min 100 -max 350
//  -s E:\Pices\SipperFiles\SMP7510010\SMP751001034\Part1\SMP751001034_01.spr  -d  E:\Pices\ExtractedImages\SMP7510010\SMP751001034_01\ -co -x -c NRDA_VerySmall.cfg
//  -s C:\Pices\SipperFiles\SML751001\SMP751001_035\SMP751001035_04.spr  -d C:\Pices\ExtractedImages\SML751001\SMP751001_035\SMP751001035_04\ -co -x -c Oil_BFS.cfg
//  -s C:\Pices\SipperFiles\SML751001\SMP751001_035\SMP751001035_04.spr  -d C:\Pices\ExtractedImages\SML751001\SMP751001_035\SMP751001035_04\ -co -x -c NRDA_small_BFS.cfg  -min 100 -max 350

// -s E:\Pices\SipperFiles\ETP2008\ETP2008_1D\ETP2008_1D_03.spr  -d C:\Pices\ExtractedImages\ETP2008\ETP2008_1D\ETP2008_1D_03  -co -x -c etp_08_BFS.cfg  -min 200
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\GU1002001S_05.spr  -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\GG1002001S_05\GG1002001S_05_c0.txt  -S F:\Pices\SipperFiles\GG1002\GG1002001S\GU1002001S_05.spr  -SF Sipper3  -C D:\Pices\DataFiles\TrainingModels\GulfOilBroad2_Discreate6_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes 

// 2013-03-26   Kurt
// Re-running the extractions below because of descrencies with andrews PC.
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\SMP751001035_16-ReRun  -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\SMP751001035_16-ReRun\SMP751001035_16_c0.txt  -S F:\Pices\SipperFiles\SMP751001\SMP751001_035\SMP751001035_16.spr  -SF Sipper3  -C GulfOilBroad_130314_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\SMP751001035_13-ReRun  -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\SMP751001035_13-ReRun\SMP751001035_13_c0.txt  -S F:\Pices\SipperFiles\SMP751001\SMP751001_035\SMP751001035_13.spr  -SF Sipper3  -C GulfOilBroad_130314_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\GG1002005S_01-ReRun    -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\GG1002005S_01-ReRun\GG1002005S_01_c0.txt  -S F:\Pices\SipperFiles\GG1002\GG1002005S\GG1002005S_01.spr  -SF Sipper3  -C GulfOilBroad_130314_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\GG1002005S_05-ReRun    -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\GG1002005S_05-ReRun\GG1002005S_05_c0.txt  -S F:\Pices\SipperFiles\GG1002\GG1002005S\GG1002005S_05.spr  -SF Sipper3  -C GulfOilBroad_130314_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\SMP751001035_21-ReRun    -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\SMP751001035_21-ReRun\SMP751001035_21_c0.txt  -S E:\Pices\SipperFiles\OilCruise\SMP751001035_21.spr  -SF Sipper3  -C NRDA_SMP751001_37_dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost

//2013-03-31  Extraction of this file crashed on Andrews PC
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\SMP751001_035-ReRun    -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\SMP751001_035-ReRun\GG1002005S_05_c0.txt  -S F:\Pices\SipperFiles\SMP751001\SMP751001_035\SMP751001035_21.spr  -SF Sipper3  -C NRDA_SMP751001_37_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost

// Sipper File "'SMP751001037_01'"  Instrument Data is all zeroed out,
// -A 0  -CPD 3  -D D:\Pices\ExtractedImages\SMP751001037_01-ReRun    -ipd 1000  -min 150  -max 0  -Pre On  -R D:\Pices\ExtractedImages\SMP751001037_01-ReRun\SMP751001037_01_c0.txt  -S F:\Pices\SipperFiles\SMP751001\SMP751001_037\SMP751001037_01.spr  -SF Sipper3  -C NRDA_SMP751001_37_Dual.cfg  -X  -CountOnly  -MultiThreaded Yes -DataBase LocalHost



ExtractionManager::ExtractionManager (const KKStr&      _applicationName,
                                      ExtractionParms&  _parms,
                                      uint32            _maxNumThreads,
                                      RunLog&           _log
                                     ):
  applicationName        (_applicationName),
  cancelFlag             (false),
  completionStatus       (),
  crashed                (false),
  dbConn                 (NULL),
  doneExecuting          (false),
  fileDesc               (NULL),
  framePool              (NULL),
  frameWidth             (0),
  imagesAwaitingUpdate   (NULL),
  imageManager           (NULL),
  logEntry               (NULL),
  logEntryId             (0),
  maxNumThreads          (_maxNumThreads),
  msgQueue               (NULL),
  parms                  (_parms),
  reportFile             (),
  sipperFileRec          (NULL),
  terminateFlag          (false),
  validationInfo         (NULL),
  validationInfoFileName (),
  validationInfoFileNameHistory (),
  veryLargeImageSize     (10000),

  endTime                (),
  endCPUsecs             (0.0),
  startTime              (),
  startCPUsecs           (0.0),

  frameExtractorThread   (NULL),
  frameProcessors        (NULL),
  dataBaseUpdaterThread  (NULL),
  allThreads             (NULL), 

  log                     (_log)

{
  InstrumentDataFileManager::InitializePush ();

  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);
}




ExtractionManager::~ExtractionManager ()
{
  delete  validationInfo;         validationInfo       = NULL;
  delete  logEntry;               logEntry             = NULL;
  delete  dbConn;                 dbConn               = NULL;
  delete  framePool;              framePool            = NULL;
  delete  imageManager;           imageManager         = NULL;
  delete  imagesAwaitingUpdate;   imagesAwaitingUpdate = NULL;
  delete  allThreads;             allThreads           = NULL;
  delete  msgQueue;               msgQueue             = NULL;
  delete  frameProcessors;        frameProcessors      = NULL;
  delete  sipperFileRec;          sipperFileRec        = NULL;

  frameExtractorThread  = NULL;
  dataBaseUpdaterThread = NULL;

  InstrumentDataFileManager::InitializePop ();
}



void  ExtractionManager::TerminateProcessing (ImageExtractionThreadListPtr  threads)
{
  if  (threads)
  {
    ImageExtractionThreadList::iterator  idx;
    for  (idx = threads->begin ();  idx != threads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  thread = *idx;
      thread->TerminateThread ();
    }
  }
}  /* TerminateProcessing */




void  ExtractionManager::CancelProcessing (int32 miliSecsToWait)
{
  cancelFlag = true;
  if  (allThreads)
  {
    ImageExtractionThreadList::iterator  idx;
    for  (idx = allThreads->begin ();  idx != allThreads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  thread = *idx;
      thread->CancelThread ();
    }
  }

  if  (miliSecsToWait == 0)
    return;

  bool     allThreadsStopped      = true;
  uint32   numThreadsStopped      = 0;
  uint32   numThreadsStillRunning = 0;

  int32   numMiliSecsSoFar = 0;

  while  (true)
  {
    allThreadsStopped      = true;
    numThreadsStopped      = 0;
    numThreadsStillRunning = 0;
    ImageExtractionThreadList::iterator  idx;
    for  (idx = allThreads->begin ();  idx != allThreads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  thread = *idx;
      if  ((thread->Status () == tsStopped)     ||
           (thread->Status () == tsNotStarted)  ||
           (thread->Crashed ())
          )
      {
        numThreadsStopped++;
      }
      else
      {
        numThreadsStillRunning++;
        allThreadsStopped = false;
      }
    }

    if  (allThreadsStopped)
    {
      break;
    }
    else
    {
      osSleep (0.01f);
      numMiliSecsSoFar += 10;
    }

    if  ((miliSecsToWait >= 0)  &&  (numMiliSecsSoFar > miliSecsToWait))
      break;
  }

  if  (numThreadsStillRunning > 0)
  {
    ImageExtractionThreadList::iterator  idx;
    for  (idx = allThreads->begin ();  idx != allThreads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  thread = *idx;
      thread->Stop ();
    }
  }
}  /* CancelProcessing */



void   ExtractionManager::MakeSureSipperCruiseExists ()
{
  SipperCruisePtr  sc = dbConn->SipperCruiseLoad ("UNKNOWN");
  if  (sc == NULL)
  {
    sc = new SipperCruise ();
    sc->CruiseName   ("UnKnown");
    sc->ShipName     ("UnKnown");
    sc->Description  ("UnKnown");
    sc->DateStart    (DateType (2001,  1,  1));
    sc->DateEnd      (DateType (2099, 12, 31));
    sc->Location     ("UnKnown");
    sc->Objective    ("UnKnown");
    sc->Principal    ("UnKnown");
    sc->ResearchOrg  ("UnKnown");
    bool  successful = false;
    dbConn->SipperCruiseInsert (sc, successful);
  }
  delete sc;  sc = NULL;
}  /* MakeSureSipperCruiseExists */


void   ExtractionManager::MakeSureSipperStationExists ()
{
  SipperStationPtr  ss = dbConn->SipperStationLoad ("UNKNOWN", "1");
  if  (ss == NULL)
  {
    ss = new SipperStation ();
    ss->CruiseName     ("UNKNOWN");
    ss->StationName    ("1");
    ss->Description    ("UNKNOWN");
    ss->Latitude       (0.0);
    ss->Longitude      (0.0);
    ss->DateTimeStart  (DateTime (2001,  1,  1, 0, 0, 0));
    bool  successful = false;
    dbConn->SipperStationInsert (*ss);
  }
  delete ss;  ss = NULL;
}  /* MakeSureSipperStationExists */



void   ExtractionManager::MakeSureSipperDeploymentExists ()
{
  SipperDeploymentPtr  sd = dbConn->SipperDeploymentLoad ("UNKNOWN", "1", "A");
  if  (sd == NULL)
  {
    sd = new SipperDeployment ();

    sd->CruiseName     ("UNKNOWN");
    sd->StationName    ("1");
    sd->DeploymentNum  ("A");
    sd->Description    ("UNKNOWN");
    sd->Latitude            (0.0);
    sd->Longitude           (0.0);
    sd->DateTimeStart       (DateTime (2001,  1,  1,  0,  0,  0));
    sd->DateTimeEnd         (DateTime (2099, 12, 31, 11, 59, 59));
    sd->SyncTimeStampActual (DateTime (2001,  1,  1,  0,  0,  0));
    sd->SyncTimeStampCTD    (DateTime (2001,  1,  1,  0,  0,  0));
    sd->SyncTimeStampGPS    (DateTime (2001,  1,  1,  0,  0,  0));
    dbConn->SipperDeploymentInsert (*sd);
  }
  delete sd;  sd = NULL;
}  /* MakeSureSipperDeploymentExists */





SipperFilePtr  ExtractionManager::MakeSureSipperSipperFileExists (const KKStr&  sipperFileName)
{
  SipperFilePtr  sipperFileRec = NULL;
  if  (dbConn != NULL)
     sipperFileRec = dbConn->SipperFileRecLoad (sipperRootName);

  if  (sipperFileRec == NULL)
  {
    if  (dbConn != NULL)
    {
      MakeSureSipperCruiseExists ();
      MakeSureSipperStationExists ();
      MakeSureSipperDeploymentExists ();
    }

    sipperFileRec = new SipperFile (parms.SipperFileName ());
    sipperFileRec->AssignCtdExternalInstruments (sipperRootName);
    sipperFileRec->CruiseName ("UnKnown");
    sipperFileRec->StationName    ("1");
    sipperFileRec->DeploymentNum  ("A");
    sipperFileRec->Description    ("UnKnown");
    sipperFileRec->Latitude       (0.0);
    sipperFileRec->Longitude      (0.0);
    sipperFileRec->Depth          (0.0);

    DateTime  refTimeStamp (2008, 10, 1, 0, 0, 0);
    DateTime  sipperFileTimeStamp  = osGetFileDateTime (parms.SipperFileName ());
    long long  fileSizeInBytes = osGetFileSize (parms.SipperFileName ());
    sipperFileRec->SizeInBytes (fileSizeInBytes);
    sipperFileRec->DateTimeStart (sipperFileTimeStamp);
    sipperFileRec->ScanRate (30000.0f);
    if  (parms.UpdateDataBase ())
      dbConn->SipperFileInsert (*sipperFileRec);
  }

  return  sipperFileRec;
}  /* MakeSureSipperSipperFileExists */




void  ExtractionManager::Initialize (bool&  _successful)
{
  log.Level (10) << "ExtractionManager::Initialize" << endl;
  _successful = true;

  completionStatus = "Starting";

  startTime    = osGetLocalDateTime ();
  startCPUsecs = osGetSystemTimeUsed ();

  if  (parms.UpdateDataBase ())
  {
    dbConn = new DataBase (parms.DataBaseServer (), log);
    if  (!dbConn->Valid ())
    {
      log.Level (-1) << endl 
        << "ExtractionManager::Initialize    *** Could not connect to Database ***" << endl 
        << "       DataBase[" << dbConn->ServerDescription () << "]" << endl
        << endl;
      _successful = false;
      parms.StatusMessage ("Could not connect to Database   *** " + dbConn->ServerDescription () + " ***");
      completionStatus = "Failed to connect to DataBase";
      crashed = true;
      return;
    }
  }

  if  (cancelFlag)
  {
    _successful = false;
    return;
  }

  sipperRootName = osGetRootName (parms.SipperFileName ());

  DateTime  now = osGetLocalDateTime ();
  KKStr  validationInfoDir = SipperVariables::ValidationInfoDir ();
  osCreateDirectoryPath (validationInfoDir);
  validationInfoFileName        = osAddSlash (validationInfoDir) + sipperRootName + "_ValidationInfo.txt";
  validationInfoFileNameHistory = osAddSlash (validationInfoDir) + sipperRootName + "_ValidationInfo_" + now.YYYYMMDDHHMMSS () + ".txt";

  sipperFileRec = MakeSureSipperSipperFileExists (parms.SipperFileName ());

  if  (cancelFlag)
  {
    _successful = false;
    return;
  }

  if  (dbConn)
  {
    // Flag Siper file as Extraction Started.
    dbConn->SipperFilesUpdateExtractionStatus (sipperRootName, '2');
  }

  if  ((!parms.RefreshDataBaseImages ())  &&  parms.UpdateDataBase ())
  {
    // We do not erase database when we are ReFreshingDataBaseImages.

    // Since we are re-extracting the SIPPER file we need to:
    // 1) Save existing Validation Info.
    // 2) Load validation info from a prebvious run that might have crashed.
    // 3) Append to this list Validation from the Images table.
    // 4) remove all data from Images table for this sipper file.

    //dbConn->ImagesEraseSipperFile (parms.SipperFileName ());

    validationInfo = new DataBaseImageValidatedEntryList (validationInfoFileName, log);

    DataBaseImageValidatedEntryListPtr  dbValidationInfo = dbConn->ImagesRetrieveValidatedClass (sipperRootName);
    if  (dbValidationInfo  &&  (dbValidationInfo->QueueSize () > 0))
    {
      validationInfo->AddQueue (*dbValidationInfo);
      validationInfo->Save (validationInfoFileName, log);
      dbValidationInfo->Owner (false);
    }
    delete  dbValidationInfo;
    dbValidationInfo = NULL;

    DateType compilationDate (__DATE__);
    TimeType compilationTime (__TIME__);
    DateTime compilationDateTime (compilationDate, compilationTime);

    logEntry = dbConn->LogEntriesProcessStart ("IE",
                                               applicationName,
                                               compilationDateTime,
                                               parms.CmdLine (),
                                               sipperRootName
                                              );
    logEntryId = logEntry->LogEntryId ();
    dbConn->ImagesEraseSipperFileSizeRange (parms.SipperFileName (), parms.MinImageSize (), parms.MaxImageSize ());
  }

  if  (cancelFlag)
  {
    _successful = false;
    return;
  }


  // kurt kramer  2014-04-25
  // The 4bit file format from KKLine Scanner utilities does not contain instrument data.
  // For purposes of seeeding up debuging we will not update database from it.
  if  (parms.FileFormat () != sfSipper4Bit)
  {
    InstrumentDataListPtr   instrumentData = InstrumentDataFileManager::GetInstrumentDataForSipperFile 
                                             (parms.SipperFileName (), 
                                              sipperFileRec, 
                                              true,   // true = Fource re-extraction of data from SipperFiles.
                                              cancelFlag, 
                                              log
                                             );

    if  ((instrumentData)  &&  (!parms.RefreshDataBaseImages ())  &&  parms.UpdateDataBase ())
      dbConn->InstrumentDataSaveListForOneSipperFile (parms.SipperFileName (), *instrumentData, cancelFlag);
  }

  if  (cancelFlag)
  {
    _successful = false;
    return;
  }

  // Make sure Output Dircectory Exists
  osCreateDirectoryPath (parms.OutputRootDir ());
  
  KKStr  logFileName = osAddSlash (parms.OutputRootDir ()) + osGetRootName (parms.SipperFileName ()) + ".log";
  log.AttachFile (logFileName);

  reportFile.open (parms.ReportFileName ().Str ());

  reportFile << "Command Line [" << parms.CmdLine () << "]" << endl << endl << endl;
  log.Level (10) << endl << "Command Line[" << parms.CmdLine () << "]" << endl << endl;

  {
    KKStr  instrumentReportDirectory = osAddSlash (parms.OutputRootDir ()) + "InstumentData";

    InstrumentDataManagerPtr  instrumentDataManager = 
      new InstrumentDataManager (parms.SipperFileName (), 
                                 sipperFileRec, 
                                 instrumentReportDirectory, 
                                 log
                                );

    SipperBuffPtr  sipperBuff = SipperBuff::CreateSipperBuff 
                                   (parms.FileFormat (), 
                                    parms.SipperFileName (), 
                                    parms.CameraNum (),
                                    instrumentDataManager,
                                    log
                                   );

    if  (sipperBuff)
    {
      if  (!sipperBuff->Opened ())
      {
        delete  sipperBuff;
        sipperBuff = NULL;
      }
    }

    if  (sipperBuff == NULL)
    {
      log.Level (-1) << endl << "ExtractionManager::Initialize    *** Could not open SipperFile[" << parms.SipperFileName () << "] ***" << endl << endl;
      parms.StatusMessage ("Could not open SIPPER File[" + parms.SipperFileName () + "]");
      _successful = false;
      crashed = true;
      completionStatus = "Failed to open Sipper File";
      return;
    }

    frameWidth = sipperBuff->LineWidth ();

    delete  sipperBuff;
    sipperBuff = NULL;

    delete  instrumentDataManager;
    instrumentDataManager = NULL;
  }

  imageManager = new SipperExtractionImageManager (fileDesc, 
                                                   parms.OutputRootDir (),
                                                   parms.ExtractFeatureData (),
                                                   veryLargeImageSize,
                                                   (!parms.ConfigFileName ().Empty ()),  // We are going to clasify images,
                                                   parms.CountOnly (),
                                                   parms.ImagesPerDirectory (),
                                                   log
                                                  );

  if  (cancelFlag)
  {
    _successful = false;
    return;
  }

  {
    // This makes sure that the Classifier stored on disk is up-2-date.
    KKStr  trainerStatusMsg;
    TrainingProcess2Ptr  trainer 
         = new TrainingProcess2 (parms.ConfigFileName (),
                                 NULL,
                                 fileDesc, 
                                 log, 
                                 NULL, 
                                 false,             // false = DON'T force model rebuild.
                                 true,              // true = Check for duplicate images in training data.
                                 cancelFlag,
                                 trainerStatusMsg
                                );

    if  (trainer->Abort ())
    {
      completionStatus = "Abort building Classifier";
      _successful = false;
    }

    delete  trainer;
    trainer = NULL;
  }

  if  (!_successful)
    return;

  msgQueue = new MsgQueue ("ExtractionManager");

  uint32  maxNumOfFrames = KKU::Max ((uint32)3, (uint32)(maxNumThreads + 1));

  framePool = new LogicalFrameQueue (parms, 
                                     maxNumOfFrames,    // Maximum number of frames
                                     2 * frameWidth,    // Maximum Frame Height.
                                     frameWidth,
                                     cancelFlag
                                    );

  imagesAwaitingUpdate = new ExtractedImageQueue ();

  log.Level (10) << "ExtractionManager::Initialize   Exiting" << endl;
}  /* Initialize */




void  ExtractionManager::StartFrameProcessor (bool&  threadStartedSuccessfully)
{
  uint32 x = frameProcessors->size ();

  KKStr  threadName = "FrameProcessor_" + StrFormatInt (x, "00");

  log.Level (10) << "ExtractionManager::StartFrameProcessor  Starting '" << threadName << "'." << endl;

  FrameProcessorThreadPtr  fp 
    = new FrameProcessorThread (parms,
                                this,
                                threadName,
                                msgQueue,
                                framePool,
                                imagesAwaitingUpdate,
                                threadStartedSuccessfully
                               );

  if  (!threadStartedSuccessfully)
  {
    KKStr  msg (128);
    msg << "StartFrameProcessor   ***ERROR***    Failed to create Thread Instance[" << threadName << "].";
    msgQueue->AddMsg (msg);
    log.Level (-1) << endl << "ExtractionManager::" << msg << endl << endl;
    delete  fp;
    fp = NULL;
    completionStatus = "Failed to start FrameProcessor";
    crashed = true;
  }
  else if  (CancelFlag ())
  {
    delete  fp;
    fp = NULL;
  }
  else
  {
    frameProcessors->PushOnBack (fp);
    allThreads->PushOnBack (fp);
    StartThread (fp);
  }
}  /* StartFrameProcessor */




void  ExtractionManager::StartFrameExtractor (bool&  threadStartedSuccessfully)
{
  log.Level (10) << "ExtractionManager::StartFrameExtractor" << endl;
  frameExtractorThread 
      = new FrameExtractorThread (parms,
                                  this,
                                  "FrameExtractor",
                                  msgQueue,
                                  framePool,
                                  sipperFileRec,
                                  imageManager,
                                  threadStartedSuccessfully
                                 );
  if  (!threadStartedSuccessfully)
  {
    KKStr  msg = "StartFrameExtractor   ***ERROR***    Failed to create Thread Instance[FrameExtractorThread].";
    log.Level (-1) << endl << "ExtractionManager::" << msg << endl << endl;
    msgQueue->AddMsg (msg);
    delete  frameExtractorThread;
    frameExtractorThread = NULL;
    completionStatus = "Failed to start FrameExtractor";
    crashed = true;
  }
  else
  {
    allThreads->PushOnBack (frameExtractorThread);
    StartThread (frameExtractorThread);
  }
}  /* StartFrameExtractor */




void  ExtractionManager::StartThreads (bool&  threadsStartedSuccessfully)
{
  log.Level (10) << "ExtractionManager::StartThreads" << endl;

  threadsStartedSuccessfully = true;

  delete  allThreads;
  allThreads = new ImageExtractionThreadList (true);
    
  dataBaseUpdaterThread = new DataBaseUpdateThread 
                  (parms, 
                   this,
                   "DataBaseUpdater",
                   msgQueue,
                   imagesAwaitingUpdate,
                   imageManager,
                   fileDesc
                  );

  allThreads->PushOnBack (dataBaseUpdaterThread);
  StartThread (dataBaseUpdaterThread);

  delete  frameProcessors;
  frameProcessors = new ImageExtractionThreadList (false);
  uint32  numFrameProcessors = Max ((uint32)1, (uint32)(maxNumThreads - 1));

  if  (!CancelFlag ())
  {
    // Get the first frame processor running;  then start the frame extractor then 
    // get therest of the frame proecssors running
    StartFrameProcessor (threadsStartedSuccessfully);
    if  ((!threadsStartedSuccessfully)  &&  (!CancelFlag ()))
    {
      crashed = true;
      return;
    }

    if  (!CancelFlag ())
    {
      StartFrameExtractor (threadsStartedSuccessfully);
      if  ((!threadsStartedSuccessfully)  &&  (!CancelFlag ()))
      {
        crashed = true;
        return;
      }

      // Lets get the rest o the FrameProcessor threads running.
      for  (uint32 x = 1;  ((x < numFrameProcessors)  &&  threadsStartedSuccessfully &&  (!CancelFlag ()));  ++x)
        StartFrameProcessor (threadsStartedSuccessfully);
    }
  }
}  /* StartThreads */




void  ExtractionManager::StartThread (ImageExtractionThreadPtr  threadInstance)
{
  threadInstance->Start ();
}  /* StartThread */




void  ExtractionManager::GetRunTimeStats (ExtractionManagerStats&  stats)
{
  if  (frameExtractorThread)
    frameExtractorThread->GetRunTimeStats (stats.bytesRead, stats.framesRead);

  if  (frameProcessors)
  {
    uint32  framesProcessed     = 0;
    uint32  scanLinesProcessed  = 0;
    uint32  imagesFound         = 0;
    uint32  imagesClassified    = 0;

    ImageExtractionThreadList::iterator  idx;
    for  (idx = frameProcessors->begin ();  idx != frameProcessors->end ();  ++idx)
    {
      FrameProcessorThreadPtr  fp = dynamic_cast<FrameProcessorThreadPtr>(*idx);
      fp->GetRunTimeStats (framesProcessed, scanLinesProcessed, imagesFound, imagesClassified);
      stats.framesProcessed    += framesProcessed;
      stats.scanLinesProcessed += scanLinesProcessed;
      stats.imagesFound        += imagesFound;
      stats.imagesClassified   += imagesClassified;
    }
  }

  if  (dataBaseUpdaterThread)
    dataBaseUpdaterThread->GetRunTimeStats (stats.imagesUpdated, stats.duplicatesDetected, stats.updateFailures);

  if  (framePool)
    stats.framesOnQueue = framePool->FramesOnQueue ();

  if  (imagesAwaitingUpdate)
    stats.imagesOnQueue = imagesAwaitingUpdate->ImagesOnQueue ();
}  /* GetRunTimeStats */




void  ExtractionManager::GetRunTimeStats (uint64&  _bytesRead,
                                          uint32&  _framesRead,
                                          uint32&  _framesProcessed,
                                          uint32&  _scanLinesProcessed,
                                          uint32&  _imagesFound,
                                          uint32&  _imagesClassified,
                                          uint32&  _imagesUpdated,
                                          uint32&  _duplicatesDetected,
                                          uint32&  _updateFailures,
                                          uint32&  _framesOnQueue,
                                          uint32&  _imagesOnQueue
                                         )
{
  _bytesRead           = 0;
  _framesRead          = 0;
  _framesProcessed     = 0;
  _scanLinesProcessed  = 0;
  _imagesFound         = 0;
  _imagesClassified    = 0;
  _imagesUpdated       = 0;
  _duplicatesDetected  = 0;
  _updateFailures      = 0;
  _framesOnQueue       = 0;
  _imagesOnQueue       = 0;

  if  (frameExtractorThread)
    frameExtractorThread->GetRunTimeStats (_bytesRead, _framesRead);

  if  (frameProcessors)
  {
    uint32  framesProcessed     = 0;
    uint32  scanLinesProcessed  = 0;
    uint32  imagesFound         = 0;
    uint32  imagesClassified    = 0;

    ImageExtractionThreadList::iterator  idx;
    for  (idx = frameProcessors->begin ();  idx != frameProcessors->end ();  ++idx)
    {
      FrameProcessorThreadPtr  fp = dynamic_cast<FrameProcessorThreadPtr>(*idx);
      fp->GetRunTimeStats (framesProcessed, scanLinesProcessed, imagesFound, imagesClassified);
      _framesProcessed     += framesProcessed;
      _scanLinesProcessed  += scanLinesProcessed;
      _imagesFound         += imagesFound;
      _imagesClassified    += imagesClassified;
    }
  }

  if  (dataBaseUpdaterThread)
    dataBaseUpdaterThread->GetRunTimeStats (_imagesUpdated, _duplicatesDetected, _updateFailures);


  if  (framePool)
    _framesOnQueue = framePool->FramesOnQueue ();

  if  (imagesAwaitingUpdate)
    _imagesOnQueue = imagesAwaitingUpdate->ImagesOnQueue ();

}  /* GetRunTimeStats */




ClassStatisticListPtr  ExtractionManager::GetClassStatistics ()  const
{
  if  (!imageManager)
    return NULL;

  return imageManager->ClassStats ();
}  /* GetClassStatistics */




bool  ExtractionManager::AnyProcessorsCrashed ()
{
  bool  anyProcessorsCrashed = false;

  if  (allThreads)
  {
    ImageExtractionThreadList::iterator  idx;
    for  (idx = allThreads->begin ();  idx != allThreads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  t = *idx;
      if  (t->Crashed ())
      {
        anyProcessorsCrashed = true;
        completionStatus = "Thread \"" + t->ThreadName () + "\" Crashed";
        break;
      }
    }
  }

  return  anyProcessorsCrashed;
}  /* AnyProcessorsCrash */



bool  ExtractionManager::AllProcessorsTerminated (ImageExtractionThreadListPtr  threads)
{
  bool  allProcessorsTerminated = true;

  if  (threads)
  {
    ImageExtractionThreadList::iterator  idx;
    for  (idx = threads->begin ();  idx != threads->end ();  ++idx)
    {
      ImageExtractionThreadPtr  t = *idx;
      if  (t->Status () != tsStopped)
      {
        allProcessorsTerminated = false;
        break;
      }
    }
  }

  return  allProcessorsTerminated;
}  /* AllProcessorsTerminated */




void  ExtractionManager::MonitorUntilDone (ImageExtractionThreadListPtr  threadsToMonitor,
                                           uint32&                       loopCount,
                                           bool&                         successful
                                          )
{
  TerminateProcessing (threadsToMonitor);  // Tells the threads that we are monitoring to run until their tasks are completed. 
  while  (!cancelFlag)
  {
    KKStrPtr  msg = msgQueue->GetNextMsg ();
    while  (msg != NULL)
    {
      log.Level (10) << (*msg) << endl;
      cout << (*msg) << endl;
      delete  msg;
      msg = NULL;
      msg = msgQueue->GetNextMsg ();
    }

    if  ((loopCount % 20) == 0)
    {
      // Report Current results.
      DisplayRunTimeStats (loopCount);

      if  (AnyProcessorsCrashed ())
      {
        KKStr  msg = "One or more Threads crashed and as a result the extraction did not complete corectly.";
        reportFile << endl << msg << endl << endl;

        log.Level (10) << endl  << "ExtractionManager::MonitorUntilDone  " << msg << endl << endl;

        CancelProcessing (2000);
        successful = false;
        crashed = true;
        break;
      }

      if  (AllProcessorsTerminated (threadsToMonitor))
      {
        successful = true;
        break;
      }
    }

    osSleep (0.1f);
    ++loopCount;
  }
}  /* MonitorUntilDone */



void  ExtractionManager::ManageTheExtraction (bool&  successful)
{
  Initialize (successful);
  if  (!successful)
  {
    doneExecuting = true;
    return;
  }

  if  (CancelFlag ())
    return;

  StartThreads (successful);
  if  (!successful)
  {
    CancelProcessing (5000);
    doneExecuting = true;
    return;
  }

  if  (!CancelFlag ())
  {
    // Image Extraction can not be done until all frames have been extracted; so for this reason we only
    // monitir the 'frameExtractorThread' thread. After the 'frameExtractorThread' has processed all avaialble
    // sipper data we will then no longer need to monitorthis thread.
    uint32  loopCount = 0;
    ImageExtractionThreadList  frameExtractionThreads (false);
    frameExtractionThreads.PushOnBack (frameExtractorThread);
    MonitorUntilDone (&frameExtractionThreads, loopCount, successful);
    if  (!successful)
    {
      doneExecuting = true;
      return;
    }

    if  (!CancelFlag ())
    {
      // Even though there is no more frames being produced by 'frameExtractorThread' we still have to make
      // sure that all 'frames' that are on queue to be proessed are processed.
      MonitorUntilDone (frameProcessors, loopCount, successful);
      if  (!successful)
      {
        doneExecuting = true;
        return;
      }

      if  (!CancelFlag ())
      {
        // At this point all data has been extracted and all frames processed;  we only need to make sure that all 
        // the images that have been queued for insertion into the database have been processed.
        ImageExtractionThreadList  dataBaseUpdateThreads (false);
        dataBaseUpdateThreads.PushOnBack (dataBaseUpdaterThread);
        MonitorUntilDone (&dataBaseUpdateThreads, loopCount, successful);
        if  (!successful)
        {
          doneExecuting = true;
          return;
        }
      }
    }
  }

  if  (AnyProcessorsCrashed ())
  {
    reportFile << endl 
      << endl
      << "One or more Threads crashed and as a result the extraction did not complete corectly." << endl
      << endl;

    log.Level (10) << endl
      << "ExtractionManager::ManageTheExtraction   One or more threads crashed extraction is not complete." << endl
      << endl;
    CancelProcessing (10000);
    doneExecuting = true;
    crashed = true;
    return;
  }

  if  ((!cancelFlag) &&  (imageManager != NULL))
    imageManager->Flush ();

  endTime     = osGetLocalDateTime  ();
  endCPUsecs  = osGetSystemTimeUsed ();

  GenerateReport ();

  if  (cancelFlag)
    completionStatus = "Extraction Canceled";

  else if  (!crashed)
    completionStatus = "Done";

  double  cpuTimeUsed = endCPUsecs - startCPUsecs;

  ReUpdateValidationData ();

  if  ((dbConn != NULL)  &&  (logEntry != NULL))
  {
    dbConn->LogEntriesProcessEnd (logEntry, (float)cpuTimeUsed, completionStatus);
    delete  logEntry;
    logEntry = NULL;
  }

  if  (dbConn)
  {
    if  ((!cancelFlag)   &&  (!crashed))
      dbConn->SipperFilesUpdateExtractionStatus (sipperRootName, '3');
  }

  doneExecuting = true;
}  /* ManageTheExtraction */



void  ExtractionManager::DisplayRunTimeStats (uint32  loopCount)
{
    // Report Current results.
  uint64  bytesRead          = 0;
  uint32  framesRead         = 0,  framesProcessed  = 0,  scanLinesProcessed = 0;
  uint32  imagesFound        = 0,  imagesClassified = 0,  imagesUpdated      = 0;
  uint32  duplicatesDetected = 0,  updateFailures   = 0,  framesOnQueue      = 0;
  uint32  imagesOnQueue      = 0;

  GetRunTimeStats (bytesRead,        framesRead,    
                   framesProcessed,  scanLinesProcessed, imagesFound,    imagesClassified,
                   imagesUpdated,    duplicatesDetected, updateFailures,
                   framesOnQueue,    imagesOnQueue
                  );

  log.Level (10) << "BytesRead"  << "\t" << StrFormatInt64 (bytesRead,        "zz,zzz,zzz,zz0")  << "\t"
                 << "FramesOnQ"  << "\t" << StrFormatInt   (framesOnQueue,    "zz,zz0")          << "\t"
                 << "Classified" << "\t" << StrFormatInt   (imagesClassified, "zz,zzz,zz0")      << "\t"
                 << "ImagesOnQ"  << "\t" << StrFormatInt   (imagesOnQueue,    "zz,zz0")          << "\t"
                 << "ImagesUpd"  << "\t" << StrFormatInt   (imagesUpdated,    "zz,zz0")          << "\t"
                 << "Failures"   << "\t" << StrFormatInt   (updateFailures,   "z,zzz,zz0")
                 << endl;
 
  if  ((loopCount % 200) == 0)
  {
    //       0         1         2         3         4         5         6       
    //       0123456789012345678901234567890123456789012345678901234567890123456789
    cout << endl;
    cout << "                 Frames   Images   Images   Images   Update"  << endl;
    cout << "     BytesRead     On-Q    Class     On-Q    Updtd    Fails"  << endl;
  }

  cout << StrFormatInt64 (bytesRead,        "##,###,###,##0")
       << StrFormatInt   (framesOnQueue,    "#,###,##0")
       << StrFormatInt   (imagesClassified, "#,###,##0")
       << StrFormatInt   (imagesOnQueue,    "#,###,##0")
       << StrFormatInt   (imagesUpdated,    "#,###,##0")
       << StrFormatInt   (updateFailures,   "#,###,##0")
       << endl;
}  /* DisplayRunTimeStats */




void  ExtractionManager::ReportHeader (ostream&  r)
{
  if  (cancelFlag)
  {
    r << endl << endl 
      << "   This extraction process was  **** C A N C E L E D ****" << endl
      << "                                **** C A N C E L E D ****" << endl
      << "                                **** C A N C E L E D ****" << endl
      << "                                **** C A N C E L E D ****" << endl
      << "                                **** C A N C E L E D ****" << endl
      << endl << endl;
  }

  else if  (crashed)
  {
    r << endl << endl 
      << "   This extraction process    **** C R A S H E D ****" << endl
      << "                              **** C R A S H E D ****" << endl
      << "                              **** C R A S H E D ****" << endl
      << "                              **** C R A S H E D ****" << endl
      << "                              **** C R A S H E D ****" << endl
      << endl << endl;
  }

  KKStr  sipperFileFormatStr = SipperFileFormatToStr (parms.FileFormat ());
  KKStr  sipperBuffFileName = "";
  if  (frameExtractorThread)
    sipperBuffFileName = frameExtractorThread->SipperBuffFileName ();

  r << endl;

  KKStr  mySqlServer = "";
  if  (dbConn)
    mySqlServer = dbConn->ServerDescription ();

  r << "Version               [" << "4.01" << "]."  << endl << endl;
  r << "MySQL Server          [" << mySqlServer                  << "]." << endl;
  r << "Source Sipper File    [" << sipperBuffFileName           << "]." << endl;
  r << "File Format           [" << sipperFileFormatStr          << "]." << endl;
  r << "Camera Number         [" << (int)parms.CameraNum     ()  << "]." << endl;
  r << "Destination Directory [" << parms.OutputRootDir      ()  << "]." << endl;
  r << "Minimum Image Size    [" << parms.MinImageSize       ()  << "]." << endl;
  r << "Maximum Image Size    [" << parms.MaxImageSize       ()  << "]." << endl;
  r << "Start Scan Row        [" << parms.ScanLineStart      ()  << "]." << endl;
  r << "End   Scan Row        [" << parms.ScanLineEnd        ()  << "]." << endl;
  r << "MorphOperations       [" << parms.MorphOperations    ()  << "]." << endl;
  r << "Connected Pixel Dist  [" << parms.ConnectedPixelDist ()  << "]." << endl;
  r << "CountOnly             [" << (parms.CountOnly () ?"Yes":"No") << "]." << endl;
  
  r << endl;

  double  totalCpuSecs = endCPUsecs - startCPUsecs;
  double  cpuSecs = totalCpuSecs;

  uint32  cpuHours =  (uint32)floor (cpuSecs / 3600.0);
  cpuSecs = cpuSecs - (cpuHours * 3600.0);
  uint32  cpuMins = (uint32)floor (cpuSecs / 60.0);
  cpuSecs = cpuSecs - (cpuMins * 60.0);

  DateTime  elapsedTime = endTime - startTime;

  r << "Start Time      : " << startTime   << endl;
  r << "End Time        : " << endTime     << endl;
  r << "Elapsed Time    : " << StrFormatInt64 (elapsedTime.Seconds (), "###,##0") << " Secs" << "\t" 
                            << elapsedTime.HH_MM_SS ()  << endl;
  r << "Processing Time : " << StrFormatDouble (totalCpuSecs, "###,##0.0") << "\t"
                            << StrFormatInt    (cpuHours, "#00") << ":" 
                            << StrFormatInt    (cpuMins, "00")   << ":" 
                            << StrFormatDouble (cpuSecs, "00.0") << endl;

  r << endl;

  if  (cancelFlag)
  {
    r << endl
      << endl
      << endl;
    r << "****************************************************************************"  << endl;
    r << "*                                                                          *"  << endl;
    r << "*                    CANCELING EXTRACTION AT USER  REQUEST                 *"  << endl;
    r << "*                                                                          *"  << endl;
    r << "****************************************************************************"  << endl;
    r << endl
      << endl
      << endl;
  }
}  /* ReportHeader */
  



void  ExtractionManager::GenerateReport ()
{
  ReportHeader (reportFile);

  if  (parms.ScanLineStart () > 0)
  {
    reportFile << endl
               << "Skipped to Line[" << parms.ScanLineStart () << "] in Sipper File." << endl
               << endl;
  }

  if  (parms.ScanLineEnd () > 0)
  {
    reportFile << endl
               << "End Scanline Specified[" << parms.ScanLineEnd () << "] in Sipper File." << endl
               << endl;
  }

  uint64  bytesRead          = 0;
  uint32  framesRead         = 0,  framesProcessed  = 0,  scanLinesProcessed = 0;
  uint32  imagesFound        = 0,  imagesClassified = 0,  imagesUpdated      = 0;
  uint32  duplicatesDetected = 0,  updateFailures   = 0,  framesOnQueue      = 0;
  uint32  imagesOnQueue      = 0;

  GetRunTimeStats (bytesRead,        framesRead,    
                   framesProcessed,  scanLinesProcessed, imagesFound,    imagesClassified,
                   imagesUpdated,    duplicatesDetected, updateFailures,
                   framesOnQueue,    imagesOnQueue
                  );

  reportFile << "Totals" << endl << endl;

  uint  imagesNotWritten = imagesFound - imagesClassified;

  reportFile << "Total Bytes         [" << StrFormatInt64 (bytesRead,           "zzz,zzz,zzz,zz0")  << "]" << endl
             << "Total Lines         [" << StrFormatInt   (scanLinesProcessed,  "zzz,zzz,zz0")      << "]" << endl
             << "Total Images        [" << StrFormatInt   (imagesFound,         "zzz,zzz,zz0")      << "]" << endl
             << "Images Extracted    [" << StrFormatInt   (imagesClassified,    "zzz,zzz,zz0")      << "]" << endl
             << "Images Updated      [" << StrFormatInt   (imagesUpdated,       "zzz,zzz,zz0")      << "]" << endl
             << "Duplicates Detected [" << StrFormatInt   (duplicatesDetected,  "zzz,zzz,zz0")      << "]" << endl
             << "Update Failures     [" << StrFormatInt   (updateFailures,      "zzz,zzz,zz0")      << "]" << endl
             << endl;

  reportFile << "Very Large Image Threshold[" << StrFormatInt (veryLargeImageSize, "zzz,zz@") << "]  "
             << endl
             << endl;

  // reportFile << "Bytes Dropped[" << StrFormatInt (sipperBuff->BytesDropped (), "zz,zzz,zz0") << "]." << endl << endl;

  reportFile << endl << endl 
             << "Count By Size:" << endl;
  imageManager->ReportStatistics (reportFile);

  if  (dbConn != NULL)
  {
    long long  sizeInBytes = (long long)osGetFileSize (parms.SipperFileName ());
    dbConn->SipperFileUpdateFileSizeStats (osGetRootName (parms.SipperFileName ()), sizeInBytes, scanLinesProcessed);
  }

  if  ((!parms.FramesOnly ())  &&  (frameExtractorThread != NULL))
  {
    const InstrumentDataManagerPtr  dataManager = frameExtractorThread->DataManager ();

    VectorUlong*  scanLinesPerMeterDepth =  dataManager->ScanLinesPerMeterDepth ();
    KKStr  depthReportFileName = osRemoveExtension (parms.ReportFileName ()) + "_Depth.txt";
    ofstream depthReport (depthReportFileName.Str ());
    depthReport << "Command Line [" << parms.CmdLine      () << "]" << endl;
    depthReport << "Date         [" << osGetLocalDateTime () << "]" << endl << endl;
    imageManager->ReportStatsByDepth (depthReport, scanLinesPerMeterDepth);
    depthReport.close ();
    delete  scanLinesPerMeterDepth;  scanLinesPerMeterDepth = NULL;
  }

}  /* GenerateReport */




void  ExtractionManager::ReUpdateValidationData ()
{
  if  ((!validationInfo)  ||  (!dbConn))
    return;

  log.Level (10) << "ReUpdateValidationData  Re-Initializing Validation Info    NumImages[" << validationInfo->QueueSize () << "]." << endl;

  reportFile << endl << endl
    << "Validation Info ReInserted." << endl
    << endl
    << "ImageFileName" << "\t" << "ValidatedClass" << "\t" << "ImageFound" << "\t" << "Updated" << endl;

  DataBaseImageValidatedEntryList::const_iterator  idx;
  for  (idx = validationInfo->begin ();  idx != validationInfo->end ();  ++idx)
  {
    DataBaseImageValidatedEntryPtr  ve = *idx;
    DataBaseImagePtr  existingImage = dbConn->ImageLoad (ve->ImageFileName ());
    bool  found = (existingImage != NULL);
    if  (!found)
      log.Level (10) << "ReUpdateValidationData   Image[" << ve->ImageFileName () << "] Not Found." << endl;

    bool  updated = false;
    if  (existingImage != NULL)
    {
      if  (ve->ValidatedClass () != NULL)
        dbConn->ImagesUpdateValidatedClass (ve->ImageFileName (), ve->ValidatedClass ());

      if  (ve->SizeCoordinates () != NULL)
        dbConn->ImagesUpdateImageSize (ve->ImageFileName (), ve->SizeCoordinates (), ve->ImageSize (1.0f, 1.0f));

      updated = dbConn->Valid ();
      delete  existingImage;
      existingImage = NULL;
    }

    reportFile 
           << ve->ImageFileName () 
           << "\t" << ve->ValidatedClassName ()
           << "\t" << (found   ? "Yes" : "No")
           << "\t" << (updated ? "Yes" : "No")
           << endl;
  }

  osRenameFile (validationInfoFileName, validationInfoFileNameHistory);
}  /* ReUpdateValidationData */

