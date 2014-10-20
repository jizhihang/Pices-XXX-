/* SipperThread.cpp -- Manages the threads that perform the image extraction process.
 * Copyright (C) 2012 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in ImageExtractionManager.h
 */
#include  "FirstIncludes.h"
#include  <string>
#include  <iostream>
#include  <vector>
#include  "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
#include "ImageIO.h"
#include "MsgQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;


#include "InstrumentData.h"
#include "InstrumentDataManager.h"
#include "InstrumentDataFileManager.h"
#include "SipperBuff.h"
#include "SipperBuffBinary.h"
#include "SipperBuff3Bit.h"
#include "Sipper3Buff.h"
#include "Sipper3RevBuff.h"
using namespace SipperHardware;


#include "FrameExtractorThread.h"
#include "ExtractionManager.h"
#include "LogicalFrame.h"
#include "LogicalFrameQueue.h"
using namespace ImageExtractionManager;



FrameExtractorThread::FrameExtractorThread (ExtractionParms&                 _parms,
                                            ExtractionManagerPtr             _extractionManager,
                                            const KKStr&                     _threadName,
                                            MsgQueuePtr                      _msgQueue,
                                            LogicalFrameQueuePtr             _framePool,
                                            SipperFilePtr                    _sipperFileRec,
                                            SipperExtractionImageManagerPtr  _imageManager,
                                            bool&                            _successful
                                           ):
  ImageExtractionThread (_parms, _extractionManager, _threadName, _msgQueue),
  framePool (_framePool),

  frame                      (NULL),
  frameArea                  (NULL),
  frameRowByteOffset         (NULL),
  totPixels                  (0),
  frameNum                   (0),
  frameSipperRow             (0),
  endOfFileReachedInLastCall (false),

  lastRowInFrame             (0),
  lastRowUsed                (0),
  frameHeightMax             (0),
  frameWidth                 (0),
  nextFrameStartRow          (0),
  pixelsPerRow               (NULL),
  colCount                   (NULL),
  scanLineEnd                (0),
  
  sipperBuff                 (NULL),
  sipperBuffFileName         (),
  sipperRootName             (),
  instrumentDataManager      (NULL),
  imageManager               (_imageManager),
  sipperFileRec              (_sipperFileRec),

  bytesRead                  (0),
  framesRead                 (0),
  log                        ()

{
  AddMsg ("FrameExtractorThread");

  _successful = true;

  InstrumentDataFileManager::InitializePush ();

  KKStr  siperFileRootName = osGetRootName (parms.SipperFileName ());

  if  (sipperFileRec == NULL)
  {
    KKStr msg = "FrameExtractorThread   ***ERROR***     (sipperFileRec == NULL)";
    AddMsg (msg);
    _successful = false;
    return;
  }

  KKStr  instrumentReportDirectory = osAddSlash (parms.OutputRootDir ()) + "InstumentData";

  instrumentDataManager = new InstrumentDataManager (_parms.SipperFileName (), sipperFileRec, instrumentReportDirectory, log);

  sipperBuff = SipperBuff::CreateSipperBuff (parms.FileFormat (), 
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
    KKStr msg (100);
    msg << "FrameExtractorThread  ***ERROR***      *** Could not open SipperFile[" << parms.SipperFileName () << "] ***" << endl << endl;
    _successful = false;
    AddMsg (msg);
    cerr << endl << msg << endl << endl;
    return;
  }

  sipperBuffFileName = sipperBuff->FileName ();

  frameWidth = sipperBuff->LineWidth ();

  frameNum          = 0;
  lastRowInFrame    = 0;
  lastRowUsed       = 0;
  frameHeightMax    = 0;
  nextFrameStartRow = 0;

  AllocateFrame ();

  if  (parms.ScanLineStart () > 0)
  {
    sipperBuff->SkipToScanLine (parms.ScanLineStart ());
    KKStr  msg(100);
    msg << "FrameExtractorThread    Skipped to scan line[" <<  parms.ScanLineStart () << "]";
    AddMsg (msg);
  }

  if  (parms.ScanLineEnd () > 0)
  {
    scanLineEnd = parms.ScanLineEnd ();
  }
  else
  {
    // We want to read all scan lines in the file.
    scanLineEnd = uint32_max;
  }


  AddMsg ("FrameExtractorThread    Exiting");
}




FrameExtractorThread::~FrameExtractorThread ()
{
  AddMsg ("~FrameExtractorThread");

  delete  instrumentDataManager;  instrumentDataManager = NULL;
  delete  sipperBuff;             sipperBuff            = NULL;
  delete  frameArea;              frameArea             = NULL;
  delete  frame;                  frame                 = NULL;
  delete  frameRowByteOffset;     frameRowByteOffset    = NULL;
  delete  pixelsPerRow;           pixelsPerRow          = NULL;
  delete  colCount;               colCount              = NULL;
  InstrumentDataFileManager::InitializePop ();
}



void  FrameExtractorThread::GetRunTimeStats (uint64&  _bytesRead,
                                             uint32&  _framesRead
                                            )
{
  _bytesRead  = bytesRead;
  _framesRead = framesRead;
}



void  FrameExtractorThread::AllocateFrame ()
{
  frameHeightMax = frameWidth;

  lastRowUsed    = 0;

  totPixels = frameHeightMax * frameWidth;

  frameArea          = new uchar [totPixels];

  frame              = new uchar* [frameHeightMax];
  frameRowByteOffset = new uint64 [frameHeightMax];
  pixelsPerRow       = new uint32 [frameHeightMax];
  colCount           = new uint32 [frameWidth];

  nextFrameStartRow = frameHeightMax;

  int  curPixel = 0;

  for  (uint32  x = 0;  x < frameHeightMax;  x++)
  {
    frame[x] = &(frameArea[curPixel]);
    memset (frame[x], 0, frameWidth);
    frameRowByteOffset[x] = 0;
    pixelsPerRow[x] = 0;
    curPixel += frameWidth;
  }
}  /* AllocateFrame */





void  FrameExtractorThread::GetNextFrame (bool&  moreFrames)
{
  bool    flow     = false;
  uint32  lineSize = 0;
  uint32  rowCount = 0;

  for  (uint32 x = 0;  x < frameWidth;  ++x)
    colCount[x] = 0;

  frameSipperRow = sipperBuff->CurRow ();

  if  (nextFrameStartRow >= lastRowUsed)
  {
    // There are no lines left over from a previous call to this function.
    if  (endOfFileReachedInLastCall) 
    {
      moreFrames = false;
      return;
    }
  }

  else
  {
    // We have left over lines from previous GetNextFrame
    uint32  tempPixsInRow     = 0;
    uint64  tempRowByteOffset = 0;

    for  (uint32 x = nextFrameStartRow;  x <= lastRowUsed;  ++x)
    {
      uchar*  destRow = frame[rowCount];

      memcpy (destRow, frame[x], frameWidth);
      frameRowByteOffset [rowCount] = frameRowByteOffset [x];
      pixelsPerRow       [rowCount] = pixelsPerRow       [x];

      for  (uint32 col = 0;  col < frameWidth;  ++col)
      {
        if  (destRow[col] > 0)
          colCount[col]++;
      }

      frameSipperRow--;  // Since we are using lines that have already
                         // been read from sipper buff we need to subtract
                         // these lines from prev frame from our starting 
                         // frame starting position.
      rowCount++;
    }

    lastRowUsed       = rowCount - 1;
    lastRowInFrame    = lastRowUsed;
    nextFrameStartRow = frameHeightMax;
  }

  bool  moreLines = false;

  if  (!endOfFileReachedInLastCall) 
  {
    moreLines = true;

    while  (moreLines  &&  (rowCount < frameHeightMax))
    {
      frameRowByteOffset[rowCount] = sipperBuff->ByteOffset ();
      sipperBuff->GetNextLine (frame[rowCount], 
                               frameWidth, 
                               lineSize, 
                               colCount, 
                               pixelsPerRow[rowCount], 
                               flow
                              );
      if  ((lineSize <= 0)  ||  (sipperBuff->CurRow () > scanLineEnd))
      {
        endOfFileReachedInLastCall = true;
        moreLines = false;
      }
      else
      {
        rowCount++;
        parms.IncrementLinesExtracted ();
      }
      bytesRead = sipperBuff->ByteOffset ();
    }
  }

  if  (!moreLines)
  {
    endOfFileReachedInLastCall = true;
    if  (rowCount < 10)
    {
      moreFrames = false;
      return;
    }
  }
  
  lastRowUsed    = rowCount - 1;
  lastRowInFrame = lastRowUsed;

  while  (rowCount < frameHeightMax)
  {
    memset (frame[rowCount], 0, frameWidth);
    pixelsPerRow[rowCount] = 0;
    rowCount++;
  }

  if  (!parms.FramesOnly ())
    SearchForCleanBreak ();

  if  (parms.SaveFrames ())
    SaveFrame ("Before");

  if  (parms.PreProcess ())
    EliminatePosibleLines (colCount);

  if  (parms.SaveFramesAfter ())     // kk 2005-03-03,  as per andrew only before frames.
    SaveFrame ("After");
}  /* GetNextFrame */



void  FrameExtractorThread::SearchForCleanBreak ()
{
  // A Clean Break is defined by having three rows in a row that do not 
  // have any pixels occupied.

  lastRowInFrame = lastRowUsed;

  int     blankRowsInARow = 0;

  if  ((pixelsPerRow[lastRowInFrame]     == 0)  &&
       (pixelsPerRow[lastRowInFrame - 1] == 0))
  {
    // The Frame ends with a clean break so lets set next Frame to 
    // frameHeightMax indicating that there is nothing to carry over
    // and back lastRowInFrame and lastRowUsed to first non blank 
    // Row.

    lastRowInFrame--;
    while  ((lastRowInFrame > 100) && (pixelsPerRow[lastRowInFrame] == 0))
      lastRowInFrame--;

    nextFrameStartRow = frameHeightMax;
    lastRowUsed = lastRowInFrame;
  }

  else
  {
    // Since the last row has some pixels we need to search back for 
    // a clean break in the frame.

    while  ((lastRowInFrame > 1000)  &&  (blankRowsInARow < 3))
    {
      if  (pixelsPerRow[lastRowInFrame] == 0)
      {
        blankRowsInARow++;
      }
      else
      {
        blankRowsInARow = 0;
        nextFrameStartRow = lastRowInFrame;
      }

      lastRowInFrame--;
    }

    if  ((lastRowInFrame <= 1000)  &&  (blankRowsInARow < 3))
    {
      // Since we could not find a seperation between images lets look
      // for were the row with least pixels.
      uint32  rowWithLeastPixels = lastRowInFrame;
      uint32  leastNumOfPixels   = 999999;

      for  (uint32  x = lastRowInFrame;  x < lastRowUsed;  ++x)
      {
        if  (pixelsPerRow[x] < leastNumOfPixels )
        {
          rowWithLeastPixels = x;
          leastNumOfPixels = pixelsPerRow[x];
        }
      }

      lastRowInFrame = rowWithLeastPixels;
      nextFrameStartRow = lastRowInFrame + 1;
    }
    else
    {
      nextFrameStartRow = lastRowInFrame + 1;

      // We now have a Good Starting Position for the next Frame.
      // So now lets look for the ending position of the prev frame.
      while  ((lastRowInFrame > 100) && (pixelsPerRow[lastRowInFrame] == 0))
        lastRowInFrame--;
    }
  }
}  /* SearchForCleanBreak */



void  FrameExtractorThread::BuildColCount (uint32  colCount[])
{
  uint32  col;
  uint32  row;

  for  (col = 0;  col < frameWidth;  ++col)
  {
    colCount[col] = 0;
  }

  uchar*  rowContent = NULL;
  for  (row = 0; row <= lastRowInFrame; row++)
  {
    rowContent = frame[row];

    for  (col = 0;  col < frameWidth;  ++col)
    {
      if  (rowContent[col] > 0)
        colCount[col]++;
    }
  }
}  /* BuildColCount */




void  FrameExtractorThread::EliminatePosibleLines (uint32  colCount[])
{
  uint32  endCol;
  bool    endColFound;
  uint32  col;
  uint32  row;
  uint32  startCol;
  uint32  threshold = lastRowInFrame  / 6;
  if  (threshold < 300)
    threshold = 300;

  uint32  x = 0;

  BuildColCount (colCount);


  for  (col = 1;  col < (frameWidth - 1);  ++col)
  {
    if  (colCount[col] > threshold)
    {
      startCol = col;
      endCol   = col;
      endColFound = false;
      col++;

      while  ((col < (frameWidth - 1))  &&  (!endColFound))
      {
        if  (colCount[col] > threshold)
        {
          endCol = col;
          col++;
        }
        else
        {
          endColFound = true;
        }
      }

      int  width = 1 + endCol - startCol;

      if  (width > MaxArtifactLineWidth)
        continue;

      // Now that we defined a Possible Collumn lets get rid of the stupid Line.

      uchar*  rowContent = NULL;
      for  (row = 0; row <= lastRowUsed; row++)
      {
        rowContent = frame[row];

        if  (((rowContent[startCol - 1] == 0) ||  (startCol <= 1))  &&  
             ((rowContent[endCol   + 1] == 0) ||  (endCol   >= (frameWidth - 2))))
        {
          for  (x = startCol; x <= endCol; x++)
          {
            if  (rowContent[x] > 0)
            {
              rowContent[x] = 0;
              pixelsPerRow[row]--;
            }
          }
        }
      }
    }
  }  /* end of for col */
}  /* EliminatePosibleLines */




void  FrameExtractorThread::SaveFrame (const KKStr& suffix)
{
  uint32  col = 0;
  uint32  row = 0;

  RasterSipperPtr  frameImage = new RasterSipper (lastRowInFrame + 1, 4096, false);
  uchar**  grayArea = frameImage->Green ();

  for  (row = 0; row < lastRowInFrame; row++)
  {
    for  (col = 0;  col < frameWidth;  ++col)
    {
      grayArea[row][col] = frame[row][col];
    }
  }
  
  KKStr  frameName = "Frame_" + StrFormatInt (frameNum, "0000") + "_" +
                      StrFormatInt (frameSipperRow, "00000000")  + "_" +
                      suffix +
                      ".bmp";

  imageManager->SaveFrame (frameName, frameImage, parms.Colorize ());

  delete  frameImage;
}  /* SaveFrame */





void  FrameExtractorThread::Run ()
{
  Status (tsStarting);

  AddMsg ("Run  Starting");

  startTime = osGetLocalDateTime ();
  endTime   = startTime;

  double  startCPUsecs = osGetSystemTimeUsed ();
  
  frameNum = 0;
  bool  moreFrames = true;

  Status (tsRunning);

  GetNextFrame (moreFrames);
  while  (moreFrames  &&  (!CancelFlag ()))
  {
    ++framesRead;

    parms.BytesExtracted (sipperBuff->ByteOffset ());
 
    if  (!parms.FramesOnly ())
    {
      ProcessFrame ();
    }

    if  (CancelFlag ())
    {
      AddMsg ("Run   Cancel or Terminate  requested.");
      break;
    }

    frameNum++;
    GetNextFrame (moreFrames);
  }

  Status (tsStopping);

  AddMsg ("Run   Done Extracting Frames.");
  
  double  doneCPUsecs    = osGetSystemTimeUsed ();
  systemTimeUsed = doneCPUsecs - startCPUsecs;

  endTime =  osGetLocalDateTime ();
  elaspedTime = endTime - startTime;

  Status (tsStopped);
}  /* Run */



void  FrameExtractorThread::ProcessFrame ()
{
  LogicalFramePtr  logicalFrame = framePool->GetNextAvailaleFrame ();
  while  ((logicalFrame == NULL)  &&  (!CancelFlag ()))
  {
    osSleep (0.002f);
    logicalFrame = framePool->GetNextAvailaleFrame ();
  }

  if  (CancelFlag ())
  {
    if  (logicalFrame)
    {
      framePool->AddBackToAvailableFrames (logicalFrame);
      logicalFrame = NULL;
    }
    return;
  }

  logicalFrame->PopulateFrame (frameNum, 
                               lastRowInFrame, 
                               frameArea, 
                               frameSipperRow,
                               frameRowByteOffset,
                               pixelsPerRow
                              );
  framePool->QueueFrameToProcess (logicalFrame);
} /* ProcessFrame */




