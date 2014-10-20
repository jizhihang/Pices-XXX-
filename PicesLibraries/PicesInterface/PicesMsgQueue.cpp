#include "StdAfx.h"

#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

#include "MemoryDebug.h"

#include "BasicTypes.h"
#include "MsgQueue.h"
using namespace  KKU;

using namespace  System::Diagnostics;


#include  "PicesKKStr.h"

#include  "PicesMsgQueue.h"



using namespace PicesInterface;


PicesMsgQueue::PicesMsgQueue (String^  name)
{
  msgQueue = new KKU::MsgQueue (PicesKKStr::SystemStringToKKStr (name));
}



PicesMsgQueue::!PicesMsgQueue ()
{
  Close ();
}


PicesMsgQueue::~PicesMsgQueue ()
{
  this->!PicesMsgQueue ();
}




void  PicesMsgQueue::Close ()
{
  delete msgQueue;  msgQueue = NULL;
};




String^  PicesMsgQueue::GetCopyOfLastMsg ()
{
  KKStrPtr  lastMsg = msgQueue->GetCopyOfLastMsg ();
  if  (lastMsg == NULL)
    return nullptr;

  String^ s = PicesKKStr::KKStrToSystenStr (*lastMsg);
  delete  lastMsg;
  lastMsg = NULL;

  return  s;
}  /* GetCopyOfLastMsg */



String^  PicesMsgQueue::GetNextMsg ()
{
  KKStrPtr  nextMsg = msgQueue->GetNextMsg ();
  if  (nextMsg == NULL)
    return nullptr;

  String^ s = PicesKKStr::KKStrToSystenStr (*nextMsg);
  delete  nextMsg;
  nextMsg = NULL;

  return  s;
}  /* GetNextMsg */




void  PicesMsgQueue::AddMsg (String^  msg)
{
  if  (msgQueue)
    msgQueue->AddMsg (PicesKKStr::SystemStringToKKStr (msg));
}



