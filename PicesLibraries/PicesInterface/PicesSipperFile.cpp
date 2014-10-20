#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;

#include  "SipperFile.h"
using namespace  SipperHardware;

#include  "PicesKKStr.h"
#include  "PicesMethods.h"
#include  "PicesSipperFile.h"

#include  "Instrument.h"
using namespace  PicesInterface;

using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

namespace  PicesInterface
{
  PicesSipperFile::PicesSipperFile (String^  _sipperFileName)
  {
    sipperFile = new SipperFile (PicesKKStr::SystemStringToKKStr (_sipperFileName));
  }


  PicesSipperFile::PicesSipperFile (SipperFilePtr  _sipperFile)
  {
    sipperFile = _sipperFile;
  }



  PicesSipperFile::~PicesSipperFile ()
  {
    this->!PicesSipperFile ();
  }



  PicesSipperFile::!PicesSipperFile ()
  {
    delete  sipperFile;
    sipperFile = NULL;
  }



  String^  PicesSipperFile::GetInsrumentShortName (InstrumentPtr  instrument)
  {
    if  (instrument)
      return  PicesKKStr::KKStrToSystenStr (instrument->ShortName ());
    else
      return  "";
  }



  InstrumentPtr  PicesSipperFile::InstrumentLookUp (String^  instrumentShortName)
  {
    return  Instrument::LookUpByShortName (PicesKKStr::SystemStringToKKStr (instrumentShortName));
  }



  String^ PicesSipperFile::SipperFileName::get          () {return  PicesKKStr::KKStrToSystenStr (sipperFile->SipperFileName ());}
  String^ PicesSipperFile::CruiseName::get              () {return  PicesKKStr::KKStrToSystenStr (sipperFile->CruiseName     ());}
  String^ PicesSipperFile::StationName::get             () {return  PicesKKStr::KKStrToSystenStr (sipperFile->StationName    ());}
  String^ PicesSipperFile::DeploymentNum::get           () {return  PicesKKStr::KKStrToSystenStr (sipperFile->DeploymentNum  ());}
  String^ PicesSipperFile::Description::get             () {return  PicesKKStr::KKStrToSystenStr (sipperFile->Description    ());}
  double  PicesSipperFile::Latitude::get                () {return                                sipperFile->Latitude       ();}
  double  PicesSipperFile::Longitude::get               () {return                                sipperFile->Longitude      ();}
  System::DateTime  PicesSipperFile::DateTimeStart::get () {return  PicesMethods::DateTimeKKUtoSystem (sipperFile->DateTimeStart ());}
  String^ PicesSipperFile::Sp0::get                     () {return  GetInsrumentShortName (sipperFile->Sp0 ());}
  String^ PicesSipperFile::Sp1::get                     () {return  GetInsrumentShortName (sipperFile->Sp1 ());}
  String^ PicesSipperFile::Sp2::get                     () {return  GetInsrumentShortName (sipperFile->Sp2 ());}
  String^ PicesSipperFile::Sp3::get                     () {return  GetInsrumentShortName (sipperFile->Sp3 ());}
  String^ PicesSipperFile::CtdExt0::get                 () {return  PicesKKStr::KKStrToSystenStr (sipperFile->CtdExt0 ());}
  String^ PicesSipperFile::CtdExt1::get                 () {return  PicesKKStr::KKStrToSystenStr (sipperFile->CtdExt1 ());}
  String^ PicesSipperFile::CtdExt2::get                 () {return  PicesKKStr::KKStrToSystenStr (sipperFile->CtdExt2 ());}
  String^ PicesSipperFile::CtdExt3::get                 () {return  PicesKKStr::KKStrToSystenStr (sipperFile->CtdExt3 ());}
  ulong   PicesSipperFile::SizeInBytes::get             () {return  (ulong)(sipperFile->SizeInBytes ());}
  uint    PicesSipperFile::NumScanLines::get            () {return  sipperFile->NumScanLines ();}
  float   PicesSipperFile::ScanRate::get                () {return                                sipperFile->ScanRate       ();}
  float   PicesSipperFile::Depth::get                   () {return                                sipperFile->Depth          ();}
  char    PicesSipperFile::ExtractionStatus::get        () {return                                sipperFile->ExtractionStatus        ();}
  uint    PicesSipperFile::ExtractionScanLineStart::get () {return                                sipperFile->ExtractionScanLineStart ();}
  uint    PicesSipperFile::ExtractionScanLineEnd::get   () {return                                sipperFile->ExtractionScanLineEnd   ();}


  void  PicesSipperFile::SipperFileName::set          (String^  _sipperFileName)          {sipperFile->SipperFileName          (PicesKKStr::SystemStringToKKStr (_sipperFileName));}
  void  PicesSipperFile::CruiseName::set              (String^  _cruiseName)              {sipperFile->CruiseName              (PicesKKStr::SystemStringToKKStr (_cruiseName));}
  void  PicesSipperFile::StationName::set             (String^  _stationName)             {sipperFile->StationName             (PicesKKStr::SystemStringToKKStr (_stationName));}
  void  PicesSipperFile::DeploymentNum::set           (String^  _deploymentNum)           {sipperFile->DeploymentNum           (PicesKKStr::SystemStringToKKStr (_deploymentNum));}
  void  PicesSipperFile::Description::set             (String^  _description)             {sipperFile->Description             (PicesKKStr::SystemStringToKKStr (_description));}
  void  PicesSipperFile::Latitude::set                (double   _latitude)                {sipperFile->Latitude                (_latitude);}
  void  PicesSipperFile::Longitude::set               (double   _longitde)                {sipperFile->Longitude               (_longitde);}
  void  PicesSipperFile::DateTimeStart::set           (System::DateTime _dateTimeStart)   {sipperFile->DateTimeStart           (PicesMethods::DateTimeSystemToKKU (_dateTimeStart));}
  void  PicesSipperFile::Sp0::set                     (String^  _sp0)                     {sipperFile->Sp0                     (InstrumentLookUp (_sp0));}
  void  PicesSipperFile::Sp1::set                     (String^  _sp1)                     {sipperFile->Sp1                     (InstrumentLookUp (_sp1));}
  void  PicesSipperFile::Sp2::set                     (String^  _sp2)                     {sipperFile->Sp2                     (InstrumentLookUp (_sp2));}
  void  PicesSipperFile::Sp3::set                     (String^  _sp3)                     {sipperFile->Sp3                     (InstrumentLookUp (_sp3));}
  void  PicesSipperFile::CtdExt0::set                 (String^ _ctdExt0)                  {sipperFile->CtdExt0                 (PicesKKStr::SystemStringToKKStr (_ctdExt0));}
  void  PicesSipperFile::CtdExt1::set                 (String^ _ctdExt1)                  {sipperFile->CtdExt1                 (PicesKKStr::SystemStringToKKStr (_ctdExt1));}
  void  PicesSipperFile::CtdExt2::set                 (String^ _ctdExt2)                  {sipperFile->CtdExt2                 (PicesKKStr::SystemStringToKKStr (_ctdExt2));}
  void  PicesSipperFile::CtdExt3::set                 (String^ _ctdExt3)                  {sipperFile->CtdExt3                 (PicesKKStr::SystemStringToKKStr (_ctdExt3));}
  void  PicesSipperFile::SizeInBytes::set             (ulong   _sizeInBytes)              {sipperFile->SizeInBytes             (_sizeInBytes);}
  void  PicesSipperFile::NumScanLines::set            (uint    _numScanLines)             {sipperFile->NumScanLines            (_numScanLines);}
  void  PicesSipperFile::ScanRate::set                (float   _scanRate)                 {sipperFile->ScanRate                (_scanRate);}
  void  PicesSipperFile::Depth::set                   (float   _depth)                    {sipperFile->Depth                   (_depth);}
  void  PicesSipperFile::ExtractionStatus::set        (char    _extractionStatus)         {sipperFile->ExtractionStatus        (_extractionStatus);}
  void  PicesSipperFile::ExtractionScanLineStart::set (uint    _extractionScanLineStart)  {sipperFile->ExtractionScanLineStart (_extractionScanLineStart);}
  void  PicesSipperFile::ExtractionScanLineEnd::set   (uint    _extractionScanLineEnd)    {sipperFile->ExtractionScanLineEnd   (_extractionScanLineEnd);}



  String^  PicesSipperFile::GPSString::get () 
  {
    return  PicesMethods::LatitudeLongitudeToString (sipperFile->Latitude (), sipperFile->Longitude ());
  }


  void  PicesSipperFile::ReFresh (PicesSipperFile^  _right)
  {
    if  (_right == nullptr)
      return;

    if  ((sipperFile != NULL)  &&  (_right->UnmanagedClass () != NULL))
      sipperFile->ReFresh (*(_right->UnmanagedClass ()));
  }


  PicesSipperFileList::PicesSipperFileList ()
  {
  }


  


  //**************************************************************************************
  //* Will take ownership of the contents of '_sipperFileList' but not the list it self. *
  //**************************************************************************************
  PicesSipperFileList::PicesSipperFileList (SipperFileListPtr  _sipperFileList)
  {
    SipperFileList::iterator  idx;
    for  (idx = _sipperFileList->begin ();  idx != _sipperFileList->end ();  idx++)
    {
      SipperFilePtr  sf = *idx;
      Add (gcnew PicesSipperFile (sf));
    }

    _sipperFileList->Owner (false);
  }




  PicesSipperFileList::~PicesSipperFileList ()
  {
    Clear ();
    this->!PicesSipperFileList ();
  }


  PicesSipperFileList::!PicesSipperFileList ()
  {
  }



  PicesSipperFileList^  PicesSipperFileList::GetListOfSipperFilesWeCanFind (PicesRunLog^  runLog)
  {
    SipperFileListPtr  sipperFiles = SipperFileList::GetListOfSipperFilesWeCanFind (runLog->Log ());

    PicesSipperFileList^  picesSipperFiles = gcnew PicesSipperFileList (sipperFiles);

    sipperFiles->Owner (false);
    delete  sipperFiles;

    return  picesSipperFiles;
  }  /* GetListOfSipperFilesWeCanFind */



  System::DateTime   PicesSipperFileList::DateTimeStart::get ()
  {
    System::DateTime  startTime (2199, 1, 1, 0, 0, 0);

    for each  (PicesSipperFile^ sf in *this)
    {
      if  (sf->DateTimeStart.Year < 1999)
        continue;

      if  (sf->DateTimeStart < startTime)
        startTime = sf->DateTimeStart;
    }

    return  startTime;
  }




}  /* PicesInterface */







