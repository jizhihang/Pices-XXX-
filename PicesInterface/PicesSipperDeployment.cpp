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
using namespace KKU;

#include  "OSservices.h"
#include  "PicesKKStr.h"
#include  "PicesMethods.h"
#include  "PicesSipperDeployment.h"
using namespace  PicesInterface;

using namespace System;
using namespace System::Windows::Forms;


PicesSipperDeployment::PicesSipperDeployment (String^  _cruiseName,
                                              String^  _stationName
                                              ):
  deployment (new SipperDeployment ())

{
  deployment->CruiseName  (PicesKKStr::SystemStringToKKStr (_cruiseName));
  deployment->StationName (PicesKKStr::SystemStringToKKStr (_stationName));
}




PicesSipperDeployment::PicesSipperDeployment (SipperDeploymentPtr  _deployment):
   deployment (_deployment)
{
}


PicesSipperDeployment::~PicesSipperDeployment ()
{
  this->!PicesSipperDeployment ();
}


PicesSipperDeployment::!PicesSipperDeployment ()
{
  delete  deployment;
  deployment = NULL;
}


String^  PicesSipperDeployment::CruiseName::get ()  {return  PicesKKStr::KKStrToSystenStr (deployment->CruiseName ());}
void     PicesSipperDeployment::CruiseName::set (String^ _cruiseName)  {deployment->CruiseName (PicesKKStr::SystemStringToKKStr (_cruiseName));}


String^  PicesSipperDeployment::DeploymentNum::get ()  {return  PicesKKStr::KKStrToSystenStr (deployment->DeploymentNum ());}  
void  PicesSipperDeployment::DeploymentNum::set (String^ _deploymentNum) {deployment->DeploymentNum (PicesKKStr::SystemStringToKKStr (_deploymentNum));}


String^  PicesSipperDeployment::StationName::get   ()  {return  PicesKKStr::KKStrToSystenStr (deployment->StationName   ()); } 
void  PicesSipperDeployment::StationName::set (String^ _stationName)  {deployment->StationName (PicesKKStr::SystemStringToKKStr (_stationName));}


String^  PicesSipperDeployment::Description::get ()  {return  PicesKKStr::KKStrToSystenStr (deployment->Description ());}
void PicesSipperDeployment::Description::set (String^  _description)  {deployment->Description (PicesKKStr::SystemStringToKKStr (_description));}


System::DateTime  PicesSipperDeployment::DateTimeStart::get ()  
{
  return  PicesMethods::DateTimeKKUtoSystem (deployment->DateTimeStart ());
}


void   PicesSipperDeployment::DateTimeStart::set (System::DateTime  _dateTimeStart)  
{
  deployment->DateTimeStart (PicesMethods::DateTimeSystemToKKU (_dateTimeStart));
}



System::DateTime  PicesSipperDeployment::DateTimeEnd::get ()  
{
  return  PicesMethods::DateTimeKKUtoSystem (deployment->DateTimeEnd ());
}


void   PicesSipperDeployment::DateTimeEnd::set (System::DateTime  _dateTimeEnd)  
{
  deployment->DateTimeEnd (PicesMethods::DateTimeSystemToKKU (_dateTimeEnd));
}



System::DateTime   PicesSipperDeployment::SyncTimeStampActual::get () 
{
  return  PicesMethods::DateTimeKKUtoSystem (deployment->SyncTimeStampActual ());
}


System::DateTime   PicesSipperDeployment::SyncTimeStampCTD::get ()
{
  return  PicesMethods::DateTimeKKUtoSystem (deployment->SyncTimeStampCTD ());
}


System::DateTime   PicesSipperDeployment::SyncTimeStampGPS::get ()
{
  return  PicesMethods::DateTimeKKUtoSystem (deployment->SyncTimeStampGPS ());
}



void  PicesSipperDeployment::SyncTimeStampActual::set (System::DateTime _syncTimeStampActual)
{
  deployment->SyncTimeStampActual (PicesMethods::DateTimeSystemToKKU (_syncTimeStampActual));
}


void  PicesSipperDeployment::SyncTimeStampCTD::set (System::DateTime _syncTimeStampCTD)
{
  deployment->SyncTimeStampCTD (PicesMethods::DateTimeSystemToKKU (_syncTimeStampCTD));
}



void  PicesSipperDeployment::SyncTimeStampGPS::set (System::DateTime _syncTimeStampGPS)
{
  deployment->SyncTimeStampGPS (PicesMethods::DateTimeSystemToKKU (_syncTimeStampGPS));
}



String^  PicesSipperDeployment::GPSString::get () 
{
  return  PicesMethods::LatitudeLongitudeToString (deployment->Latitude (), deployment->Longitude ());
}


PicesSipperDeploymentList::PicesSipperDeploymentList ()
{
}


PicesSipperDeploymentList::PicesSipperDeploymentList (SipperDeploymentListPtr  deployments)
{
  if  (deployments)
  {
    SipperDeploymentList::iterator  idx;
    for  (idx = deployments->begin ();  idx != deployments->end ();  idx++)
      Add (gcnew PicesSipperDeployment (*idx));

    // We now own the contents of 'stations'  so set the owner flag to 'false'.
    // this way when the caller deletes 'stations' they will not delete the cntents
    // which we now own and depend on.
    deployments->Owner (false);
  }
}
 



PicesSipperDeploymentList::~PicesSipperDeploymentList ()
{
  Clear ();
  this->!PicesSipperDeploymentList ();
}


PicesSipperDeploymentList::!PicesSipperDeploymentList ()
{
}


