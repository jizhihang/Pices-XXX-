#include "FirstIncludes.h"
#include <stdio.h>
#include <ctype.h>

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "MemoryDebug.h"
using namespace  std;


#include "BasicTypes.h"
#include "Str.h"
using namespace  KKU;

#include "LogicalFrame.h"
#include "LogicalFrameQueue.h"
using namespace  ImageExtractionManager;



LogicalFrameQueue::LogicalFrameQueue (const ExtractionParms&  _parms,
                                      uint32                  _maxSize,
                                      uint32                  _frameHeightMax,
                                      uint32                  _frameWidth,
                                      const bool&             _cancelFlag
                                     ):
    parms                (_parms),
    maxSize              (_maxSize),
    frameHeightMax       (_frameHeightMax),
    frameWidth           (_frameWidth),
    totalFramesAllocated (0),
    goalie               (NULL),
    cancelFlag           (_cancelFlag)
{
  goalie = new GoalKeeper ("LogicalFrameQueue");
}



LogicalFrameQueue::~LogicalFrameQueue ()
{
  while  (allFrames.size () > 0)
  {
    LogicalFramePtr  frame = allFrames.back ();
    allFrames.pop_back ();
    delete  frame;
    frame = NULL;
  }

  delete  goalie;
  goalie = NULL;
}



int32  LogicalFrameQueue::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (*this);

  vector<LogicalFramePtr>::const_iterator  idx;
  for  (idx = allFrames.begin ();  idx != allFrames.end ();  ++idx)
  {
    LogicalFramePtr f = *idx;
    memoryConsumedEstimated += f->MemoryConsumedEstimated ();
  }
  
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */



uint32  LogicalFrameQueue::FramesOnQueue ()
{
  uint32  framesOnQueue = 0;
  goalie->StartBlock ();
  framesOnQueue = framesAwaitingProcessing.size ();
  goalie->EndBlock ();
  return  framesOnQueue;
}



void  LogicalFrameQueue::RemoveFromFramesBeingProcessed (LogicalFramePtr  frame)
{
  goalie->StartBlock ();

  vector<LogicalFramePtr>::iterator  idx;
  for  (idx = framesBeingProcessed.begin ();  idx != framesBeingProcessed.end ();  ++idx)
  {
    if  (*idx == frame)
    {
      framesBeingProcessed.erase (idx);
      break;
    }
  }

  goalie->EndBlock ();

}  /* RemoveFromFramesBeingProcessed */




void  LogicalFrameQueue::FrameProcessed (LogicalFramePtr  processedFrame)
{
  goalie->StartBlock ();

  RemoveFromFramesBeingProcessed (processedFrame);
  availableFrames.push (processedFrame);

  goalie->EndBlock ();
}  /* FrameProcessed */



LogicalFramePtr  LogicalFrameQueue::GetNextAvailaleFrame ()
{
  LogicalFramePtr  availableFrame = NULL;
  goalie->StartBlock ();

  if  (availableFrames.size () > 0)
  {
    availableFrame = availableFrames.front ();
    availableFrames.pop ();
  }
  else
  {
    if  (totalFramesAllocated < maxSize)
    {
      availableFrame = new LogicalFrame (parms, frameHeightMax, frameWidth, cancelFlag);
      allFrames.push_back (availableFrame);

      ++totalFramesAllocated;
    }
  }

  goalie->EndBlock ();

  return  availableFrame;
}  /* GetNextAvailaleFrame */



LogicalFramePtr  LogicalFrameQueue::GetNextFrameToProcess ()
{
  LogicalFramePtr  frameToProcess = NULL;
  goalie->StartBlock ();

  if  (framesAwaitingProcessing.size () > 0)
  {
    frameToProcess = framesAwaitingProcessing.front ();
    framesAwaitingProcessing.pop ();
    framesBeingProcessed.push_back (frameToProcess);
  }

  goalie->EndBlock ();
  return  frameToProcess;
}  /* GetNextFrameToProcess */



void  LogicalFrameQueue::QueueFrameToProcess (LogicalFramePtr   processedFrame)
{
  goalie->StartBlock ();
  framesAwaitingProcessing.push (processedFrame);
  goalie->EndBlock ();
}  /* QueueFrameToProcess */



void  LogicalFrameQueue::AddBackToAvailableFrames (LogicalFramePtr   availableFrame)
{
  goalie->StartBlock ();
  availableFrames.push (availableFrame);
  goalie->EndBlock ();
}  /* AddBackToAvailableFrames */



void  LogicalFrameQueue::AddBackToFramesAwaitingProcessing (LogicalFramePtr   frameAwaitingProcessing)
{
  goalie->StartBlock ();
  framesAwaitingProcessing.push (frameAwaitingProcessing);
  RemoveFromFramesBeingProcessed (frameAwaitingProcessing);

  goalie->EndBlock ();
}  /* AddBackToFramesAwaitingProcessing */
