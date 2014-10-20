#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

#include  "DataBaseLogEntry.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesDataBaseLogEntry  
  {
  public:
    PicesDataBaseLogEntry ();
    
    PicesDataBaseLogEntry (String^           _progCode,
                           String^           _progName,
                           System::DateTime  _dateTimeCompiled,
                           String^           _cmdLine,
                           String^           _compName,
                           String^           _compUserName,
                           String^           _sipperFileName
                          );

    /** Gives ownersip of UnManaged '_dataBaseLogEntry'  to managed newly created 'PicesDataBaseLogEntry' instance. */
    PicesDataBaseLogEntry (DataBaseLogEntryPtr  _dataBaseLogEntry);

  private:
    ~PicesDataBaseLogEntry ();

  protected:
    !PicesDataBaseLogEntry ();

  public:
    property  uint              LogEntryId        {uint             get ();   void  set (uint              _logEntryId);      }
    property  String^           ProgCode          {String^          get ();   void  set (String^           _progCode);        }
    property  String^           ProgName          {String^          get ();   void  set (String^           _progName);        }
    property  System::DateTime  DateTimeCompiled  {System::DateTime get ();   void  set (System::DateTime  _dateTimeCompiled);}
    property  String^           CmdLine           {String^          get ();   void  set (String^           _cmdLine);         }
    property  String^           CompName          {String^          get ();   void  set (String^           _compName);        }
    property  String^           DataBaseUserName  {String^          get ();   void  set (String^           _dataBaseUserName);}
    property  String^           CompUserName      {String^          get ();   void  set (String^           _compUserName);    }
    property  System::DateTime  DateTimeStart     {System::DateTime get ();   void  set (System::DateTime  _dateTimeStart);   }
    property  System::DateTime  DateTimeStartUtc  {System::DateTime get ();   void  set (System::DateTime  _dateTimeStartUtc);}
    property  System::DateTime  DateTimeEnd       {System::DateTime get ();   void  set (System::DateTime  _dateTimeEnd);     }
    property  float             CpuTimeUsed       {float            get ();   void  set (float             _cpuTimeUsed);     }
    property  String^           CompletionStatus  {String^          get ();   void  set (String^           _completionStatus);}
    property  String^           SipperFileName    {String^          get ();   void  set (String^           _sipperFileName);  }

    MLL::DataBaseLogEntryPtr  UnmanagedClass ()  {return  dataBaseLogEntry;}

  private:
    MLL::DataBaseLogEntryPtr  dataBaseLogEntry;
  };  /* PicesDataBaseLogEntry */


  public ref class PicesDataBaseLogEntryList:  List<PicesDataBaseLogEntry^>
  {
  public:
    typedef  MLL::DataBaseLogEntryPtr DataBaseLogEntryPtr;


    PicesDataBaseLogEntryList ();

    PicesDataBaseLogEntryList (DataBaseLogEntryListPtr&  _dataBaseLogEntryList);

  private:
    ~PicesDataBaseLogEntryList ();

  protected:
    !PicesDataBaseLogEntryList ();
  };

}

