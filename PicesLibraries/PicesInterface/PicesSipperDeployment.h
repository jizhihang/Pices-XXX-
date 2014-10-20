#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

#include  "DateTime.h"
using namespace KKU;

#include  "SipperDeployment.h"
using namespace SipperHardware;

#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesSipperDeployment
  {
  public:
    PicesSipperDeployment (String^  _cruiseName,
                           String^  _stationName
                          );

    PicesSipperDeployment (SipperDeploymentPtr  _deployment);
    
  private:
    ~PicesSipperDeployment ();

  protected:
    !PicesSipperDeployment ();

  public:
    property  String^  CruiseName     {String^  get ();  void  set (String^ _cruiseName);}
    property  String^  DeploymentNum  {String^  get ();  void  set (String^ _deploymentNum);}
    property  String^  StationName    {String^  get ();  void  set (String^ _stationName);}
    property  String^  Description    {String^  get ();  void  set (String^  _description);}

    property  System::DateTime  DateTimeStart {System::DateTime  get ();  void  set (System::DateTime  _dateTimeStart);}
    property  System::DateTime  DateTimeEnd   {System::DateTime  get ();  void  set (System::DateTime  _dateTimeEnd);}


    property  double  Latitude    {double  get ()  {return  deployment->Latitude  ();}    void  set (double _latitude)    {deployment->Latitude  (_latitude);}}
    property  double  Longitude   {double  get ()  {return  deployment->Longitude ();}    void  set (double _longitude)   {deployment->Longitude (_longitude);}}
    property  uint32  CropLeft    {uint32  get ()  {return  deployment->CropLeft  ();}    void  set (uint32 _cropLeft)    {deployment->CropLeft  (_cropLeft);}}      
    property  uint32  CropRight   {uint32  get ()  {return  deployment->CropRight ();}    void  set (uint32 _cropRight)   {deployment->CropRight (_cropRight);}}      

    property  System::DateTime   SyncTimeStampActual {System::DateTime   get ();   void  set (System::DateTime _syncTimeStampActual);}
    property  System::DateTime   SyncTimeStampCTD    {System::DateTime   get ();   void  set (System::DateTime _syncTimeStampCTD);}
    property  System::DateTime   SyncTimeStampGPS    {System::DateTime   get ();   void  set (System::DateTime _syncTimeStampGPS);}


    property  String^  GPSString  {String^  get ();}

    SipperDeploymentPtr  UnManagedClass () {return deployment;}

  private:
    SipperDeploymentPtr  deployment;
  };  /* PicesSipperDeployment */




  public ref class  PicesSipperDeploymentList:  public  List<PicesSipperDeployment^>
  {
  public:
    PicesSipperDeploymentList ();

    // Will take ownership of contents of 'stations'  but not the list itself.
    // the caler will still be responsable for deleting the list('stations').
    PicesSipperDeploymentList (SipperDeploymentListPtr  stations);

  private:
    ~PicesSipperDeploymentList ();

  protected:
    !PicesSipperDeploymentList ();


  private:

  };


};
