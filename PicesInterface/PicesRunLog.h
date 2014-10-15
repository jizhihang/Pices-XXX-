#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;

#include "RunLog.h"
using namespace  KKU;

#include "PicesMsgQueue.h"

namespace PicesInterface 
{

  public  ref class PicesRunLog
  {
  public:
    PicesRunLog ();
    PicesRunLog (PicesMsgQueue^  msgQueue);
    PicesRunLog (String^  logFileName);

    !PicesRunLog ();
    ~PicesRunLog ();

    void  AttachFile (String^       runLogFileName);
    void  AttachFile (const KKU::KKStr&  runLogFileName);

    void          Close ();

    void          DetachFile ();

    void          DisplayWithNoteBook ();

    String^       FileName ();

    String^       LastLine ();

    int           LineCount ();

    RunLog&       Log ()  {return *runLog;}

    void          SetLoggingLevel (int _loggingLevel);

    String^       RunLogFileName ();

    void          Write   (String^  s);
     
    void          Writeln (String^  s);

    void          Write   (int      level,
                           String^  s
                          );

    void          Writeln (int      level,
                           String^  s
                          );


    static  PicesRunLog^  CreateRunLog (String^  rootName);    /**< Will create a RunLog with a file in the 'PicesTempDirectory' Starting 
                                                                * with 'rootName' with the Date and Time appended to end.
                                                                */

  private:
      RunLogPtr       runLog;
      PicesMsgQueue^  msgQueue;

  };

}  /* namespace  PicesInterface */

//	public ref class TrainingModel :  public System::ComponentModel::Component