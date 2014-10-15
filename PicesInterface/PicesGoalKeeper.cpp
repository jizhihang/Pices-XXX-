#include "StdAfx.h"



#include  <msclr\lock.h>



using namespace System;
using namespace msclr;



#include "PicesGoalKeeper.h"

using namespace  PicesInterface;

PicesGoalKeeper::PicesGoalKeeper (String^  _name):
  blocked           (false),
  blockerDepth      (0),
  blockerThreadId   (-1),
  name              (_name),
  numBlockedThreads (0)
{
}




void  PicesGoalKeeper::StartBlock ()
{
  int  curProcessorId = System::Threading::Thread::CurrentThread->ManagedThreadId;
  
  bool           firstPassThruLoop = true;
  bool           weAreBlocked      = true;
  unsigned long  milliSecsBlocked  = 0;

  while  (weAreBlocked)
  {
    try
    {
      lock  lockVariable (this);

      if  (firstPassThruLoop)
        numBlockedThreads++;

      if  (blocked)
      {
        if  (curProcessorId == blockerThreadId)
        {
          // We are the thread that already holds the block;  so ok for us 
          // to procees.
          blockerDepth++;
          weAreBlocked = false;
          numBlockedThreads--;
        }
        else
        {
          weAreBlocked = true;
        }
      }
      else
      {
        // No one holds the block;  so we can take it.
        blocked = true;
        blockerDepth = 1;
        blockerThreadId = curProcessorId;
        weAreBlocked = false;
        numBlockedThreads--;
      }
    }

    catch  (Exception^ e)
    {
      //  Don't know why we can not establish a lock;  but will assume
      // that we are blocked.
      if  (milliSecsBlocked > 10000)
        throw gcnew Exception ("PicesGoalKeeper::StartBlock    Name[" + name + "]  Could not establish a lock\n\n" + e->ToString ());
      weAreBlocked = true;
    }

    if  (weAreBlocked)
    {
      if  (milliSecsBlocked < 10)
      {
        System::Threading::Thread::Sleep (1);
        milliSecsBlocked++;
      }

      else if  (milliSecsBlocked < 200)
      {
        System::Threading::Thread::Sleep (10);
        milliSecsBlocked += 10;
      }

      else if  (milliSecsBlocked < 10000)
      {
        System::Threading::Thread::Sleep (100);
        milliSecsBlocked += 100;
      }

      else
      {
        System::Threading::Thread::Sleep (400);
        milliSecsBlocked += 400;
      }
    }

    firstPassThruLoop = false;
  }

  return;
}   /* StartBlock */




void   PicesGoalKeeper::EndBlock ()
{
  int  curProcessorId = System::Threading::Thread::CurrentThread->ManagedThreadId;

  int   errorCode = 0;   // 0=No Error;  
                         // 1=There is no Block
                         // 2=Different thread holds the block
                         // 3=Failure to get a lock

  try
  {
    lock  lockVariable (this);

    if  (!blocked)
    {
      errorCode = 1;
    }
    
    else if  (curProcessorId != blockerThreadId)
    {
      errorCode = 2;
    }

    else
    {
      blockerDepth--;
      if  (blockerDepth < 1)
      {
        blocked = false;
        blockerThreadId = -1;
        blockerDepth = 0;
      }
    }
  }
  catch  (Exception^ e)
  {
    throw gcnew Exception ("PicesGoalKeeper::EndBlock    Name[" + name + "]  Exception trying to establish lock.\n\n" + e->ToString ());
  }

  if  (errorCode == 0)
    return;

  else if  (errorCode == 1)
    throw gcnew Exception ("PicesGoalKeeper::EndBlock    Name[" + name + "]  There was no block established.");

  else if  (errorCode == 2)
    throw gcnew Exception ("PicesGoalKeeper::EndBlock    Name[" + name + "]  ThreadId[" + curProcessorId + "] Curently holds Block;  our ThreadId[" + curProcessorId + "]");

  return;
}  /* EndBlock */
