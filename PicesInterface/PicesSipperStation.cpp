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
#include  "OSservices.h"
using namespace KKU;

#include  "SipperCruise.h"
using namespace SipperHardware;

#include  "PicesSipperStation.h"
#include  "PicesKKStr.h"
#include  "PicesMethods.h"
#include  "PicesSipperCruise.h"
using namespace  PicesInterface;


using namespace System;
using namespace System::Windows::Forms;



PicesSipperStation::PicesSipperStation (SipperStationPtr  _station):
   station (_station)
{
}



 PicesSipperStation::PicesSipperStation (PicesSipperCruise^  _cruise)  /// Creatuing a new station  that will belong to '_cruise'
 {
   SipperCruisePtr  cruise = _cruise->UnmanagedSipperCruise ();
   station = new SipperStation ();
   station->CruiseName (cruise->CruiseName ());
 }



PicesSipperStation::~PicesSipperStation ()
{
  this->!PicesSipperStation ();
}


PicesSipperStation::!PicesSipperStation ()
{
  delete  station;
  station = NULL;
}



System::DateTime  PicesSipperStation::DateTimeStart::get ()
{
  return  PicesMethods::DateTimeKKUtoSystem (station->DateTimeStart ());
}


void  PicesSipperStation::DateTimeStart::set (System::DateTime  _dateTimeStart)
{
  station->DateTimeStart (PicesMethods::DateTimeSystemToKKU (_dateTimeStart));
}



String^  PicesSipperStation::GPSString::get () 
{
  return  PicesMethods::LatitudeLongitudeToString (station->Latitude (), station->Longitude ());
}




PicesSipperStationList::PicesSipperStationList (SipperStationListPtr  stations)
{
  if  (stations)
  {
    SipperStationList::iterator  idx;
    for  (idx = stations->begin ();  idx != stations->end ();  idx++)
      Add (gcnew PicesSipperStation (*idx));

    // We now own the contents of 'stations'  so set the owner flag to 'false'.
    // this way when the caller deletes 'stations' they will not delete the cntents
    // which we now own and depend on.
    stations->Owner (false);
  }
}
 



PicesSipperStationList::~PicesSipperStationList ()
{
  Clear ();
  this->!PicesSipperStationList ();
}


PicesSipperStationList::!PicesSipperStationList ()
{
}


