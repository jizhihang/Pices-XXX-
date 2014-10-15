/* ImageExtractionThread.cpp -- Manages the threads that perform the image extraction process.
 * Copyright (C) 2012 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in ImageExtractionManager.h
 */
#include "FirstIncludes.h"
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
#include "MemoryDebug.h"
using namespace std;

#include <windows.h>

#include  "MsgQueue.h"
using namespace KKU;


#include "ImageExtractionThread.h"
#include "ExtractionManager.h"
using namespace ImageExtractionManager;




//
// Usage: SetThreadName (-1, "MainThread");
//
#define MS_VC_EXCEPTION 0x406D1388

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName (DWORD        dwThreadID, 
                    const char*  threadName
                   )
{
   Sleep(10);
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = threadName;
   info.dwThreadID = dwThreadID;
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
}  /* SetThreadName */






ImageExtractionThread::ImageExtractionThread (ExtractionParms&      _parms,
                                              ExtractionManagerPtr  _extractionManager,
                                              const KKStr&          _threadName,
                                              MsgQueuePtr           _msgQueue
                                             ):
   cancelFlag        (false),
   crashed           (false),
   extractionManager (_extractionManager),
   msgQueue          (_msgQueue),
   parms             (_parms),
   status            (tsNotStarted),
   terminateFlag     (false),
   threadName        (_threadName),
   windowsThread     (NULL),
   windowsThreadId   (0)
{
}



ImageExtractionThread::~ImageExtractionThread ()
{
}



void  ImageExtractionThread::AddMsg (KKStrPtr  msg)
{
  if  (msgQueue == NULL)
  {
    cerr << endl 
         << "ImageExtractionThread::AddMsg   ***ERROR***    msgQuue is not defined."  << endl
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




void  ImageExtractionThread::AddMsg (const KKStr&  msg)
{
  if  (msgQueue == NULL)
  {
    cerr << endl 
         << "ImageExtractionThread::AddMsg   ***ERROR***    msgQuue is not defined."  << endl
         << "                       Msg[" << msg << "]." << endl
         << endl;
  }
  else
  {
    msgQueue->AddMsg (threadName + "::" + msg);
  }
}  /* AddMsg */


void  ImageExtractionThread::CancelThread ()
{
  cancelFlag = true;
}



void  ImageExtractionThread::TerminateThread ()
{
  terminateFlag = true;
}




extern "C" 
{
  unsigned int ThreadStartCallBack (void* param) 
  {
	  ImageExtractionThreadPtr  tp = (ImageExtractionThreadPtr)param;
    DataBase::ThreadInit ();
    tp->Status (tsStarting);
	  tp->Run ();
    tp->Status (tsStopped);
    DataBase::ThreadEnd ();
	  return 0;
  }
}


void  ImageExtractionThread::Start () 
{
	DWORD windowsThreadId = 0;

	windowsThread = (unsigned long*)CreateThread (NULL,
                                                0,
                                                (LPTHREAD_START_ROUTINE)ThreadStartCallBack,
                                                (ImageExtractionThreadPtr)this,
                                                0,
                                                &windowsThreadId
                                               );
	if  (windowsThread == NULL) 
  {
		throw KKStrException ("Failed to create thread");
	}
  else 
  {
	  SetPriority (THREAD_PRIORITY_NORMAL);
    SetThreadName (windowsThreadId, threadName.Str ());
	}
}  /* Start */



/** stop()
  * stops the running thread and frees the thread handle
**/ 
void  ImageExtractionThread::Stop ()
{
	if  (windowsThread == NULL)
    return;	

	WaitForSingleObject (windowsThread, INFINITE);
	CloseHandle (windowsThread);
	windowsThread = NULL;
}  /* Stop */




// http://www.codeproject.com/Articles/5218/Synchronized-multi-threading-in-C-No-MFC
void  ImageExtractionThread::SetPriority (int tp) 
{
	if  (windowsThread == NULL) 
  {
		throw KKStrException ("Thread object is null");
	}
  else 
  {
		if  (SetThreadPriority (windowsThread, tp) == 0) 
    {
			throw KKStrException ("Failed to set priority");
		}
  }
}  /* SetPriority */





