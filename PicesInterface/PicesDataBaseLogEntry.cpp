#include "StdAfx.h"
#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

#include "MemoryDebug.h"
#include "BasicTypes.h"
using namespace KKU;


#include "DataBaseLogEntry.h"
using namespace MLL;

#include "PicesKKStr.h"
#include "PicesMethods.h"
#include "PicesDataBaseLogEntry.h"
using namespace  PicesInterface;

using namespace System;


namespace  PicesInterface
{
  PicesDataBaseLogEntry::PicesDataBaseLogEntry ()
  {
    dataBaseLogEntry = new DataBaseLogEntry ();
  }


  PicesDataBaseLogEntry::PicesDataBaseLogEntry (DataBaseLogEntryPtr  _dataBaseLogEntry)
  {
    dataBaseLogEntry = _dataBaseLogEntry;
  }



  PicesDataBaseLogEntry::PicesDataBaseLogEntry (String^           _progCode,
                                                String^           _progName,
                                                System::DateTime  _dateTimeCompiled,
                                                String^           _cmdLine,
                                                String^           _compName,
                                                String^           _compUserName,
                                                String^           _sipperFileName  /// Root name of Sipper File if Sipper File Specific.
                                               ):
    dataBaseLogEntry (NULL)
  {
    KKU::DateTime dateTimeCompiled = PicesMethods::DateTimeSystemToKKU (_dateTimeCompiled);
    dataBaseLogEntry = 
      new DataBaseLogEntry (PicesKKStr::SystemStringToKKStr   (_progCode),
                            PicesKKStr::SystemStringToKKStr   (_progName),
                            PicesMethods::DateTimeSystemToKKU (_dateTimeCompiled),
                            PicesKKStr::SystemStringToKKStr   (_cmdLine),
                            PicesKKStr::SystemStringToKKStr   (_compName),
                            PicesKKStr::SystemStringToKKStr   (_compUserName),
                            PicesKKStr::SystemStringToKKStr   (_sipperFileName)
                           );
  }




  PicesDataBaseLogEntry::~PicesDataBaseLogEntry ()
  {
    this->!PicesDataBaseLogEntry ();
  }



  PicesDataBaseLogEntry::!PicesDataBaseLogEntry ()
  {
    delete  dataBaseLogEntry;
    dataBaseLogEntry = NULL;
  }


  uint    PicesDataBaseLogEntry::LogEntryId::get                 () {return  dataBaseLogEntry->LogEntryId                                          ();}
  String^ PicesDataBaseLogEntry::ProgCode::get                   () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->ProgCode         ());}
  String^ PicesDataBaseLogEntry::ProgName::get                   () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->ProgName         ());}
  System::DateTime  PicesDataBaseLogEntry::DateTimeCompiled::get () {return  PicesMethods::DateTimeKKUtoSystem (dataBaseLogEntry->DateTimeCompiled ());}
  String^ PicesDataBaseLogEntry::CmdLine::get                    () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->CmdLine          ());}
  String^ PicesDataBaseLogEntry::CompName::get                   () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->CompName         ());}
  String^ PicesDataBaseLogEntry::DataBaseUserName::get           () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->DataBaseUserName ());}
  String^ PicesDataBaseLogEntry::CompUserName::get               () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->CompUserName     ());}
  System::DateTime  PicesDataBaseLogEntry::DateTimeStart::get    () {return  PicesMethods::DateTimeKKUtoSystem (dataBaseLogEntry->DateTimeStart    ());}
  System::DateTime  PicesDataBaseLogEntry::DateTimeStartUtc::get () {return  PicesMethods::DateTimeKKUtoSystem (dataBaseLogEntry->DateTimeStartUtc ());}
  System::DateTime  PicesDataBaseLogEntry::DateTimeEnd::get      () {return  PicesMethods::DateTimeKKUtoSystem (dataBaseLogEntry->DateTimeEnd      ());}
  float   PicesDataBaseLogEntry::CpuTimeUsed::get                () {return                                     dataBaseLogEntry->CpuTimeUsed      ();}
  String^ PicesDataBaseLogEntry::CompletionStatus::get           () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->CompletionStatus ());}
  String^ PicesDataBaseLogEntry::SipperFileName::get             () {return  PicesKKStr::KKStrToSystenStr      (dataBaseLogEntry->SipperFileName   ());}

  void  PicesDataBaseLogEntry::LogEntryId::set        (uint             _logEntryId)       {dataBaseLogEntry->LogEntryId       (_logEntryId);}
  void  PicesDataBaseLogEntry::ProgCode::set          (String^          _progCode)         {dataBaseLogEntry->ProgCode         (PicesKKStr::SystemStringToKKStr   (_progCode));}
  void  PicesDataBaseLogEntry::ProgName::set          (String^          _progName)         {dataBaseLogEntry->ProgName         (PicesKKStr::SystemStringToKKStr   (_progName));}
  void  PicesDataBaseLogEntry::DateTimeCompiled::set  (System::DateTime _dateTimeCompiled) {dataBaseLogEntry->DateTimeCompiled (PicesMethods::DateTimeSystemToKKU (_dateTimeCompiled));}
  void  PicesDataBaseLogEntry::CmdLine::set           (String^          _cmdLine)          {dataBaseLogEntry->CmdLine          (PicesKKStr::SystemStringToKKStr   (_cmdLine));}
  void  PicesDataBaseLogEntry::CompName::set          (String^          _compName)         {dataBaseLogEntry->CompName         (PicesKKStr::SystemStringToKKStr   (_compName));}
  void  PicesDataBaseLogEntry::DataBaseUserName::set  (String^          _dataBaseUserName) {dataBaseLogEntry->DataBaseUserName (PicesKKStr::SystemStringToKKStr   (_dataBaseUserName));}
  void  PicesDataBaseLogEntry::CompUserName::set      (String^          _compUserName)     {dataBaseLogEntry->CompUserName     (PicesKKStr::SystemStringToKKStr   (_compUserName));}
  void  PicesDataBaseLogEntry::DateTimeStart::set     (System::DateTime _dateTimeStart)    {dataBaseLogEntry->DateTimeStart    (PicesMethods::DateTimeSystemToKKU (_dateTimeStart));}
  void  PicesDataBaseLogEntry::DateTimeStartUtc::set  (System::DateTime _dateTimeStartUtc) {dataBaseLogEntry->DateTimeStartUtc (PicesMethods::DateTimeSystemToKKU (_dateTimeStartUtc));}
  void  PicesDataBaseLogEntry::DateTimeEnd::set       (System::DateTime _dateTimeEnd)      {dataBaseLogEntry->DateTimeEnd      (PicesMethods::DateTimeSystemToKKU (_dateTimeEnd));}
  void  PicesDataBaseLogEntry::CpuTimeUsed::set       (float            _cpuTimeUsed)      {dataBaseLogEntry->CpuTimeUsed      (_cpuTimeUsed);}
  void  PicesDataBaseLogEntry::CompletionStatus::set  (String^          _completionStatus) {dataBaseLogEntry->CompletionStatus (PicesKKStr::SystemStringToKKStr   (_completionStatus));}
  void  PicesDataBaseLogEntry::SipperFileName::set    (String^          _sipperFileName)   {dataBaseLogEntry->SipperFileName   (PicesKKStr::SystemStringToKKStr   (_sipperFileName));}


  PicesDataBaseLogEntryList::PicesDataBaseLogEntryList ()
  {
  }


  //****************************************************************************
  //* Will take ownership of the contents of '_list' and delete '_list'        *
  //****************************************************************************
  PicesDataBaseLogEntryList::PicesDataBaseLogEntryList (DataBaseLogEntryListPtr&  _list)
  {
    DataBaseLogEntryList::iterator  idx;

    while  (_list->QueueSize () > 0)
    {
      DataBaseLogEntryPtr  logEntry = _list->PopFromFront ();
      Add (gcnew PicesDataBaseLogEntry (logEntry));
    }

    delete  _list;
    _list = NULL;
  }





  PicesDataBaseLogEntryList::~PicesDataBaseLogEntryList ()
  {
    Clear ();
    this->!PicesDataBaseLogEntryList ();
  }


  PicesDataBaseLogEntryList::!PicesDataBaseLogEntryList ()
  {
  }

}  /* PicesInterface */
