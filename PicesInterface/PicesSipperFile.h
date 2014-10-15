#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


//using namespace PicesCSharpClasses;

#include  "SipperFile.h"
#include  "ImageFeatures.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesSipperFile  
  {
  public:
    typedef  SipperHardware::SipperFilePtr      SipperFilePtr;
    typedef  SipperHardware::InstrumentPtr      InstrumentPtr;
   
    PicesSipperFile (String^  _sipperFileName);

    PicesSipperFile (SipperFilePtr  _sipperFile);

  private:
    ~PicesSipperFile ();

  protected:
    !PicesSipperFile ();

  public:

    property  String^     SipperFileName          {String^ get ();  void  set (String^   _sipperFileName); }

    property  String^     CruiseName              {String^ get ();  void  set (String^   _cruiseName);     }
    property  String^     StationName             {String^ get ();  void  set (String^   _stationName);    }
    property  String^     DeploymentNum           {String^ get ();  void  set (String^   _deploymentNum);  }
    property  String^     Description             {String^ get ();  void  set (String^   _description);    }
    property  double      Latitude                {double  get ();  void  set (double    _latitude);       }
    property  double      Longitude               {double  get ();  void  set (double    _longitde);       }
    property  float       ScanRate                {float   get ();  void  set (float     _scanRate);       }
    property  float       Depth                   {float   get ();  void  set (float     _depth);          }
    property  char        ExtractionStatus        {char    get ();  void  set (char      _extractionStatus);}
    property  uint        ExtractionScanLineStart {uint    get ();  void  set (uint      _extractionScanLineStart);}
    property  uint        ExtractionScanLineEnd   {uint    get ();  void  set (uint      _extractionScanLineEnd);}

    property  System::DateTime   DateTimeStart  {System::DateTime    get ();  void  set (System::DateTime _dateTimeStart);}
  
    property  String^     Sp0            {String^      get ();  void  set (String^   _sp0);}
    property  String^     Sp1            {String^      get ();  void  set (String^   _sp1);}
    property  String^     Sp2            {String^      get ();  void  set (String^   _sp2);}
    property  String^     Sp3            {String^      get ();  void  set (String^   _sp3);}

    property  String^     CtdExt0        {String^      get ();  void  set (String^   _ctdExt0);}
    property  String^     CtdExt1        {String^      get ();  void  set (String^   _ctdExt1);}
    property  String^     CtdExt2        {String^      get ();  void  set (String^   _ctdExt2);}
    property  String^     CtdExt3        {String^      get ();  void  set (String^   _ctdExt3);}
   
    property  ulong       SizeInBytes    {ulong        get ();  void  set (ulong     _sizeInBytes);}
    property  uint        NumScanLines   {uint         get ();  void  set (uint      _numScanLines);}

    property  String^     GPSString  {String^  get ();}

    SipperFilePtr  UnmanagedClass ()  {return  sipperFile;}

    void  ReFresh (PicesSipperFile^  _right);

  private:
    InstrumentPtr  InstrumentLookUp (String^  instrumentShortName);
    String^        GetInsrumentShortName (InstrumentPtr  instrument);

    SipperFilePtr  sipperFile;

  };  /* PicesSipperFile */



  public ref class PicesSipperFileList:  List<PicesSipperFile^>
  {
  public:
    typedef  SipperHardware::SipperFileListPtr  SipperFileListPtr;


    PicesSipperFileList ();

    PicesSipperFileList (SipperFileListPtr  _sipperFileList);



  private:
    ~PicesSipperFileList ();

  protected:
    !PicesSipperFileList ();

  public:
    static
      PicesSipperFileList^  GetListOfSipperFilesWeCanFind (PicesRunLog^  runLog);

    /**  Returns the earliest CTD DateTime in the list that is greater than 1999 */
    property  System::DateTime   DateTimeStart  {System::DateTime    get ();}

  };

}

