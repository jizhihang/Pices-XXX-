#include "StdAfx.h"

#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace  System::Diagnostics;


#include  "OSservices.h"
#include  "PicesKKStr.h"
#include  "PicesSipperVariables.h"

#include  "PicesRunLog.h"



using namespace PicesInterface;


PicesRunLog::PicesRunLog ():
  msgQueue (nullptr),
  runLog   (NULL)
{
  runLog = new RunLog ();
}



PicesRunLog::PicesRunLog (PicesMsgQueue^  _msgQueue):
  msgQueue (_msgQueue),
  runLog   (NULL)
{
  if  (msgQueue == nullptr)
  {
    msgQueue = gcnew PicesMsgQueue (L"NoNameProvided");
  }
  runLog = new RunLog ();
  runLog->AttachMsgQueue (msgQueue->MsgQueue ());
}




PicesRunLog::PicesRunLog (String^  logFileName):
  msgQueue (nullptr),
  runLog   (NULL)
{
  KKStr s = PicesKKStr::SystemStringToKKStr (logFileName);
  runLog = new RunLog (s.Str ());
}




PicesRunLog::!PicesRunLog ()
{
  Close ();
}


PicesRunLog::~PicesRunLog ()
{
  this->!PicesRunLog ();
}


// Will create a RunLog with a file in the 'PicesTempDirectory' Starting 
// with 'rootName' with the Date and Time appended to the end.
PicesRunLog^  PicesRunLog::CreateRunLog (String^  rootName)
{
  String^  dir = OSservices::AddSlash (PicesSipperVariables::PicesTempDirectory ()) + "RunLogs";
  OSservices::CreateDirectoryPath (dir);

  String^  fileName = OSservices::AddSlash (dir) + rootName + "_RunLog_" + System::DateTime::Now.ToString ("yyyyMMdd-HHmmss") + ".txt";
  return  gcnew  PicesRunLog (fileName);
}  /* CreateRunLog */





void  PicesRunLog::Close ()
{
  delete runLog;  runLog   = NULL;
  msgQueue = nullptr;
};



void  PicesRunLog::AttachFile (String^  runLogFileName)
{
  KKStr  kkStr = PicesKKStr::SystemStringToKKStr (runLogFileName);
  runLog->AttachFile (kkStr);
}



void  PicesRunLog::AttachFile (const KKStr&  runLogFileName)
{
  runLog->AttachFile (runLogFileName);
}


void  PicesRunLog::DetachFile ()
{
  runLog->DetachFile ();
}




String^  PicesRunLog::FileName ()
{
  if  (runLog == NULL)
    return  "";

  return  PicesKKStr::KKStrToSystenStr (runLog->FileName ());
}  /* FileName */



String^  PicesRunLog::LastLine ()
{
  if  (runLog == NULL)
    return  System::String::Empty;

  return  PicesKKStr::KKStrToSystenStr (runLog->LastLine ());
}  /* LastLine */



String^  PicesRunLog::RunLogFileName ()
{
  return  PicesKKStr::KKStrToSystenStr (runLog->FileName ());
}  /* RunLogFileName */



void  PicesRunLog::SetLoggingLevel (int _loggingLevel)
{
 if  (runLog)
   runLog->SetLoggingLevel (_loggingLevel);
}  /* SetLevel */



int  PicesRunLog::LineCount ()
{
  if  (runLog == NULL)
    return 0;
  return  runLog->LineCount ();
}



void  PicesRunLog::Write (String^  s)
{
  if  (runLog)
    runLog->Append (PicesKKStr::SystemStringToKKStr (s).Str ());
}



void  PicesRunLog::Writeln (String^  s)
{
  if  (runLog)
  {
    runLog->Append (PicesKKStr::SystemStringToKKStr (s).Str ());
    runLog->Append ("\n");
  }
}



void  PicesRunLog::Write (int      level,
                          String^  s
                         )
{
  if  (runLog)
  {
    runLog->SetLevel (level);
    runLog->Append (PicesKKStr::SystemStringToKKStr (s).Str ());
  }
}


void  PicesRunLog::Writeln (int      level,
                            String^  s
                           )
{
  if  (runLog)
  {
    runLog->SetLevel (level);
    runLog->Append (PicesKKStr::SystemStringToKKStr (s).Str ());
    runLog->Append ("\n");
  }
}



void  PicesRunLog::DisplayWithNoteBook ()
{
  if  (!runLog)
    throw gcnew Exception ("PicesRunLog::DisplayWithNoteBook    *** runLog not defined ***");

  KKStr  curFileName = runLog->FileName ();
  if  (curFileName.Empty ())
  {
    // Logging was to memory and not DISK
    throw gcnew Exception ("PicesRunLog::DisplayWithNoteBook    *** Logging was not to a Disk File can't display contents ***");
  }

  runLog->DetachFile ();

  Process^  proc = gcnew Process();
  proc->StartInfo->WorkingDirectory = "";
  proc->StartInfo->FileName = "notepad.exe";
  proc->StartInfo->Arguments = PicesKKStr::KKStrToSystenStr (curFileName);
  proc->StartInfo->UseShellExecute = false;
  proc->StartInfo->RedirectStandardOutput = false;
  proc->StartInfo->RedirectStandardError = true;
  proc->Start();
  proc->WaitForExit();
  proc->Close();

  runLog->AttachFileAppend (curFileName);
}  /* DisplayWithNoteBook */
