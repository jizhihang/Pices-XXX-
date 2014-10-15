/* SipperThread.cpp -- Manages the threads that perform the image extraction process.
 * Copyright (C) 2012 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in ImageExtractionManager.h
 */
#include "FirstIncludes.h"
#include <string>
#include <iostream>
#include <vector>
#include "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
#include "ImageIO.h"
#include "MsgQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;


#include "InstrumentData.h"
#include "InstrumentDataManager.h"
#include "InstrumentDataFileManager.h"
#include "SipperBuff.h"
#include "SipperBuffBinary.h"
#include "SipperBuff3Bit.h"
#include "Sipper3Buff.h"
#include "Sipper3RevBuff.h"
using namespace  SipperHardware;


#include "Classifier2.h"
#include "DataBase.h"
#include "FileDesc.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "SipperExtractionImageManager.h"
#include "TrainingProcess2.h"
using namespace  MLL;


#include "FrameProcessorThread.h"
#include "ExtractedImage.h"
#include "ExtractionManager.h"
#include "LogicalFrame.h"
#include "LogicalFrameQueue.h"
using namespace  ImageExtractionManager;


#include "FrameProcessorThread.h"


FrameProcessorThread::FrameProcessorThread (ExtractionParms&        _parms,
                                            ExtractionManagerPtr    _extractionManager,
                                            const KKStr&            _threadName,
                                            MsgQueuePtr             _msgQueue,
                                            LogicalFrameQueuePtr    _framePool,
                                            ExtractedImageQueuePtr  _imagesAwaitingUpdate,
                                            bool&                   _successful
                                           ):

  ImageExtractionThread (_parms, _extractionManager, _threadName, _msgQueue),
  framePool                (_framePool),
  imagesAwaitingUpdate     (_imagesAwaitingUpdate),

  fileDesc                 (NULL),
  trainer                  (NULL),
  classifier               (NULL),
  sipperRootName           (),
  unKnownMLClass        (NULL),
  framesProcessed          (0),
  scanLinesProcessed       (0),
  imagesFound              (0),
  imagesClassified         (0),
  log                      ()

{
  AddMsg ("FrameProcessorThread");

  _successful = true;

  sipperRootName = osGetRootName (parms.SipperFileName ());

  fileDesc = FeatureFileIOPices::NewPlanktonFile (log);

  InstrumentDataFileManager::InitializePush ();
 
  KKStr  siperFileRootName = osGetRootName (parms.SipperFileName ());

  unKnownMLClass = MLClass::CreateNewMLClass ("UnKnown");

  LoadClassifier (_successful);

  AddMsg ("FrameProcessorThread  Exiting");
}



FrameProcessorThread::~FrameProcessorThread ()
{
  // uint x;
  AddMsg ("~FrameProcessorThread");

  InstrumentDataFileManager::InitializePop ();

  delete  trainer;     trainer     = NULL;
  delete  classifier;  classifier  = NULL;
}



void  FrameProcessorThread::GetRunTimeStats (uint32&  _framesProcessed,
                                             uint32&  _scanLinesProcessed,
                                             uint32&  _imagesFound,
                                             uint32&  _imagesClassified
                                            )
{
  _framesProcessed     = framesProcessed;
  _scanLinesProcessed  = scanLinesProcessed;
  _imagesFound         = imagesFound;
  _imagesClassified    = imagesClassified;
}




KKStr  FrameProcessorThread::SipperFileFormatStr ()
{
  return  SipperFileFormatToStr (parms.FileFormat ());
}  /* SipperFileFormatStr */



void  FrameProcessorThread::ProcessFrame (LogicalFramePtr  frame)
{
  if  (CancelOrTerminateRequested ())
    return;

  ExtractedImageListPtr  extractedImages = frame->ProcessFrame ();
  if  (!extractedImages)
    return;

  imagesFound += frame->ImagesInFrame ();

  while  ((extractedImages->QueueSize () > 0)  &&  (!CancelFlag ()))
  {
    ExtractedImagePtr  extractedImage = extractedImages->PopFromBack ();

    uint32  sipperTopRow = extractedImage->SipperFileScanRow ();
    uint32  sipperTopCol = extractedImage->SipperFileScanCol ();

    KKStr  imageFileName (64);
    imageFileName << sipperRootName
                  << "_"
                  << StrFormatInt (sipperTopRow, "ZZ00000000")
                  << "_"
                  << StrFormatInt (sipperTopCol, "0000")
                  << ".bmp";
  
    ImageFeaturesPtr  featureVector = NULL;
    RasterSipperPtr raster = extractedImage->Image ();
    raster->FileName (imageFileName);
    
    float  depth = 0.0f;
    MLClassConstPtr  predClass1       = unKnownMLClass;
    MLClassConstPtr  predClass2       = NULL;
    int32               predClass1Votes  = 0;
    int32               predClass2Votes  = 0;
    double              knownClassProb   = 0.0;
    double              predClass1Prob   = 0.0;
    double              predClass2Prob   = 0.0;
    int32               numOfWinners     = 0;
    double              breakTie         = 0.0;
    double              compact          = 0.0;
    
    bool  imageIsDuplicate = false;
    bool  cf = false;


    /**@todo  When a propper implementation for SIPPER 4 is made i will need to get instrument data for it.  */

    InstrumentDataPtr  id = NULL;
    if  (this->parms.FileFormat () != sfSipper4Bit)
      id = InstrumentDataFileManager::GetClosestInstrumentData (imageFileName, cf, log);

    if  (id)
      depth = id->Depth ();

    if  (parms.ExtractFeatureData ())
    {
      raster->FileName (imageFileName);

      featureVector = new ImageFeatures (*raster, unKnownMLClass);

      featureVector->ImageFileName (imageFileName);

      if  (id)
      {
        featureVector->FlowRate1    (id->FlowRate1    ());
        featureVector->FlowRate2    (id->FlowRate2    ());
        featureVector->Latitude     (id->Latitude     ());
        featureVector->Longitude    (id->Longitude    ());
        featureVector->Depth        (id->Depth        ());
        featureVector->Salinity     (id->Salinity     ());
        featureVector->Oxygen       (id->Oxygen       ());
        featureVector->Fluorescence (id->Fluorescence ());
      }

      featureVector->SfCentroidCol (featureVector->CentroidCol () + float  (sipperTopCol));
      featureVector->SfCentroidRow (featureVector->CentroidRow () + double (sipperTopRow));
 
      // Duplicate Image Detection was moved over to the 'DataBaseUpdateThread".

      if  (classifier)
      {
        int  numOfWinners;
        bool knownClassOneOfTheWinners  = false;
        double  probability = 0.0f;
        double  breakTie    = 0.0f;

        ImageFeaturesPtr  dupFV = new ImageFeatures (*featureVector);
        classifier->ClassifyAImage (*dupFV,
                                     predClass1,       predClass2,
                                     predClass1Votes,  predClass2Votes,
                                     knownClassProb,   
                                     predClass1Prob,   predClass2Prob,
                                     numOfWinners,
                                     breakTie,
                                     compact
                                    );

        featureVector->MLClass     (predClass1);
        featureVector->Probability    ((float)predClass1Prob);
        featureVector->BreakTie       ((float)breakTie);
        featureVector->PredictedClass (predClass1);
        delete  dupFV;
        dupFV = NULL;
      }
    }

    extractedImage->Depth         (depth);
    extractedImage->FeatureVector (featureVector);
    
    extractedImage->Prediction (predClass1, (float)predClass1Prob, predClass2, (float)predClass2Prob);

    ++imagesClassified;

    imagesAwaitingUpdate->AddExtractedImage (extractedImage);
  }

  delete  extractedImages;
  extractedImages = NULL;

  ++framesProcessed;
  scanLinesProcessed += frame->FrameHeight ();
} /* ProcessFrame */



void  FrameProcessorThread::LoadClassifier (bool&  _successful)
{
  _successful = true;
  delete  trainer;     trainer    = NULL;
  delete  classifier;  classifier = NULL;

  if  ((parms.ExtractFeatureData ())  &&  (!parms.ConfigFileName ().Empty ()))
  {
    AddMsg ("LoadClassifier    Building Training Model");
    KKStr  trainerStatusMsg = "";

    trainer = new TrainingProcess2 (parms.ConfigFileName (),
                                    fileDesc,
                                    log,
                                    false,          // false = Feature are not already normalized.
                                    CancelFlag (),
                                    trainerStatusMsg
                                   );


    if  (CancelOrTerminateRequested ())
    {
      AddMsg ("LoadClassifier    Image Extraction Canceled.");
    }

    else if  (trainer->Abort ())
    {
      _successful = false;
      KKStr  msg (100);
      msg << "LoadClassifier    ***ERROR***  Building Classifier for ConfigFileName[" << parms.ConfigFileName () << "]  Reason[" << trainerStatusMsg << "].";
      AddMsg (msg);
    }

    else
    {
      AddMsg ("LoadClassifier   Classifier2 Created Sucessfully.");
      classifier = new Classifier2 (trainer, log);
    }
  }
}  /* LoadClassifier */



LogicalFramePtr  FrameProcessorThread::GetNextFrameToProcess ()
{
  LogicalFramePtr  logicalFrame = framePool->GetNextFrameToProcess ();
  while  ((logicalFrame == NULL)  &&  (!CancelFlag ()))
  {
    osSleep (0.01f);
    logicalFrame = framePool->GetNextFrameToProcess ();
    if  ((logicalFrame == NULL)  &&  TerminateFlag ())
    {
      // There are no more frames needing processing and the Extractionmanager wants to stop when convienient.
      break;
    }
  }

  if  (CancelFlag ()  &&  (logicalFrame != NULL))
  {
    framePool->AddBackToFramesAwaitingProcessing (logicalFrame);
    logicalFrame = NULL;
  }

  return  logicalFrame;
}  /* GetNextFrameToProcess */




void  FrameProcessorThread::Run ()
{
  Status (tsStarting);

  bool  successful = false;

  Status (tsRunning);

  LogicalFramePtr  logicalFrame = GetNextFrameToProcess ();
  while  (!CancelFlag ())
  {
    if  (logicalFrame)
    {
      ProcessFrame (logicalFrame);
      framePool->FrameProcessed (logicalFrame);
    }
    
    else if (TerminateFlag ())
      break;

    logicalFrame = GetNextFrameToProcess ();
  }

  Status (tsStopping);
  AddMsg ("Run " + ThreadName () + " done.");

  Status (tsStopped);
}  /* Run */