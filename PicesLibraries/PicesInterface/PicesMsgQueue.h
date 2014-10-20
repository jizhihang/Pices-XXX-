#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;

#include "MsgQueue.h"
using namespace  KKU;


namespace PicesInterface 
{

  public  ref class PicesMsgQueue
  {
  public:
    PicesMsgQueue (String^  name);

    !PicesMsgQueue ();
    ~PicesMsgQueue ();

    void          AddMsg (String^  msg);

    void          Close ();

    /** 
     *@brief Returns a copy of the string last added to the message queue.
     *@details This will not effect the current copy of the message queue.  he returned string
     * will be owned by the caller who will be responsible for deleting it.
     */
    String^       GetCopyOfLastMsg ();

    /**@brief  Removes from the queue the oldest message added to the queue that has not been removed. */
    String^       GetNextMsg ();

    MsgQueuePtr      MsgQueue ()  {return msgQueue;}


  private:
      MsgQueuePtr  msgQueue;
  };

}  /* namespace  PicesInterface */

