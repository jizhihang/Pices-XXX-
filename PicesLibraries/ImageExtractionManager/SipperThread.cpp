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


#include  "MsgQueue.h"
using namespace KKU;


#include "SipperThread.h"
#include "ExtractionManager.h"
using namespace ImageExtractionManager;


SipperThread::SipperThread (ExtractionManagerPtr  _extractionManager,
                            const KKStr&          _threadName,
                            MsgQueuePtr           _msgQueue,
                            const bool&           _cancelFlag,
                            const bool&           _terminateFlag
                           ):
   cancelFlag        (_cancelFlag),
   extractionManager (_extractionManager),
   msgQueue          (_msgQueue),
   status            (tsNotStarted),
   terminateFlag     (_terminateFlag),
   threadName        (threadName)
{
}



SipperThread::~SipperThread ()
{
}



void  SipperThread::AddMsg (KKStrPtr  msg)
{
  if  (msgQueue == NULL)
  {
    cerr << endl 
         << "SipperThread::AddMsg   ***ERROR***    msgQuue is not defined."  << endl
         << "                       Msg[" << *msg << "]." << endl
         << endl;
    delete  msg;
    msg = NULL;
  }
  else
  {
    msgQueue->AddMsg (msg);
  }
}  /* AddMsg */




void  SipperThread::AddMsg (const KKStr&  msg)
{
  if  (msgQueue == NULL)
  {
    cerr << endl 
         << "SipperThread::AddMsg   ***ERROR***    msgQuue is not defined."  << endl
         << "                       Msg[" << msg << "]." << endl
         << endl;
  }
  else
  {
    msgQueue->AddMsg (msg);
  }
}  /* AddMsg */


