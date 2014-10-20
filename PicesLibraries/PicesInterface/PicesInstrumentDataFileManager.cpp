#include  "StdAfx.h"
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


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;



#include "FileDesc.h"
using namespace MLL;

#include "PicesKKStr.h"
#include "PicesSipperFile.h"
#include "PicesRunLog.h"


#include "PicesInstrumentDataFileManager.h"
using namespace  SipperHardware;
using namespace  PicesInterface;



PicesInstrumentDataFileManager::PicesInstrumentDataFileManager ()
{
  cancelFlag = new bool;
  *cancelFlag = false;
}


PicesInstrumentDataFileManager::~PicesInstrumentDataFileManager ()
{
  this->!PicesInstrumentDataFileManager ();
}


PicesInstrumentDataFileManager::!PicesInstrumentDataFileManager ()
{
  delete  cancelFlag;
}


bool   PicesInstrumentDataFileManager::Cancel::get ()
{
  if  (cancelFlag)
    return *cancelFlag;
  else
    return false;
}


void   PicesInstrumentDataFileManager::Cancel::set (bool _cancel)
{
  if  (!cancelFlag)
    cancelFlag = new bool;
  *cancelFlag = _cancel;
}




PicesInstrumentDataList^   PicesInstrumentDataFileManager::ReExtractInstrumentDataForSipperFile 
              (String^           _sipperFileName,
               PicesSipperFile^  _sipperFile,
               PicesRunLog^      _log
              )
{
  InstrumentDataFileManager::InitializePush ();

  KKStr sipperFileName = PicesKKStr::SystemStringToKKStr (_sipperFileName);
  RunLog&  log = _log->Log ();

  PicesInstrumentDataList^  results = nullptr;

  SipperFilePtr  sipperFile = _sipperFile->UnmanagedClass ();

  InstrumentDataListPtr  data = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile (sipperFileName, sipperFile, *cancelFlag, log);
  if  (data)
    results = gcnew PicesInstrumentDataList (data);
  else
    results = nullptr;

  InstrumentDataFileManager::InitializePop ();

  return  results;
}  /* ReExtractInstrumentDataForSipperFile */



