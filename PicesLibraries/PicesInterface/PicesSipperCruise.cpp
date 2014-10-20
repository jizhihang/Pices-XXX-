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
//using namespace KKU;

#include  "PicesSipperCruise.h"
#include  "PicesKKStr.h"
using namespace PicesInterface;

using namespace System;
using namespace System::Windows::Forms;


PicesSipperCruise::PicesSipperCruise ():
   sipperCruise ()
{
  sipperCruise = new SipperCruise ();
}



PicesSipperCruise::PicesSipperCruise (SipperCruisePtr _cruise):
   sipperCruise (_cruise)

{
}


   
PicesSipperCruise::!PicesSipperCruise  ()
{
  delete sipperCruise;
  sipperCruise = NULL;
}



PicesSipperCruise::~PicesSipperCruise  ()
{
  this->!PicesSipperCruise  ();
}


	
System::DateTime   ToSystemDateTime (const  KKU::DateType&  date)
{
  System::DateTime  dt;

  try {dt = System::DateTime (date.Year (), date.Month (),  date.Day ());}
  catch (Exception^)
  {dt = System::DateTime (1990, 1, 1);}
  return  dt;
}


KKU::DateType   ToKkuDateType (System::DateTime   dt)
{
  
  return  KKU::DateType (dt.Year, dt.Month, dt.Day);
}




String^   PicesSipperCruise::CruiseName::get  ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->CruiseName  ());}
String^   PicesSipperCruise::ShipName::get    ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->ShipName    ());}
String^   PicesSipperCruise::Description::get ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->Description ());}

System::DateTime PicesSipperCruise::DateStart::get   ()  {return  ToSystemDateTime (sipperCruise->DateStart ());}
System::DateTime PicesSipperCruise::DateEnd::get     ()  {return  ToSystemDateTime (sipperCruise->DateEnd ());}

String^   PicesSipperCruise::Location::get    ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->Location    ());}
String^   PicesSipperCruise::Objective::get   ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->Objective   ());}
String^   PicesSipperCruise::Principal::get   ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->Principal   ());}
String^   PicesSipperCruise::ResearchOrg::get ()  {return  PicesKKStr::KKStrToSystenStr (sipperCruise->ResearchOrg ());}


void  PicesSipperCruise::CruiseName::set  (String^   _cruiseName)   {sipperCruise->CruiseName  (PicesKKStr::SystemStringToKKStr (_cruiseName  ));}
void  PicesSipperCruise::ShipName::set    (String^   _shipName)     {sipperCruise->ShipName    (PicesKKStr::SystemStringToKKStr (_shipName    ));}
void  PicesSipperCruise::Description::set (String^   _description)  {sipperCruise->Description (PicesKKStr::SystemStringToKKStr (_description ));}

void  PicesSipperCruise::DateStart::set   (System::DateTime _dateStart)    {sipperCruise->DateStart   (ToKkuDateType (_dateStart));}
void  PicesSipperCruise::DateEnd::set     (System::DateTime _dateEnd)      {sipperCruise->DateEnd     (ToKkuDateType (_dateEnd));}

void  PicesSipperCruise::Location::set    (String^   _location)     {sipperCruise->Location    (PicesKKStr::SystemStringToKKStr (_location    ));}
void  PicesSipperCruise::Objective::set   (String^   _objective)    {sipperCruise->Objective   (PicesKKStr::SystemStringToKKStr (_objective   ));}
void  PicesSipperCruise::Principal::set   (String^   _principal)    {sipperCruise->Principal   (PicesKKStr::SystemStringToKKStr (_principal   ));}
void  PicesSipperCruise::ResearchOrg::set (String^   _researchOrg)  {sipperCruise->ResearchOrg (PicesKKStr::SystemStringToKKStr (_researchOrg ));}




String^  PicesSipperCruise::ValidateCruiseName (String^  _cruiseName)
{
  KKStr  errorDesc = SipperCruise::ValidateCruiseName (PicesKKStr::SystemStringToKKStr (_cruiseName));
  if  (errorDesc.Empty ())
    return nullptr;
  else
    return PicesKKStr::KKStrToSystenStr (errorDesc);
}


String^  PicesSipperCruise::ToString ()
{
  return  CruiseName + " - " + DateStart.ToShortDateString ();
}



PicesSipperCruiseList::PicesSipperCruiseList (SipperCruiseListPtr  sipperCruises)
{
  if  (sipperCruises)
  {
    SipperCruiseList::iterator  idx;
    for  (idx = sipperCruises->begin ();  idx != sipperCruises->end ();  idx++)
    {
      SipperCruisePtr  cruise = *idx;
      Add (gcnew PicesSipperCruise (cruise));
    }

    sipperCruises->Owner (false);
  }
}



PicesSipperCruiseList::!PicesSipperCruiseList ()
{
}



PicesSipperCruiseList::~PicesSipperCruiseList ()
{
  this->!PicesSipperCruiseList ();
  Clear ();
}



PicesSipperCruise^  PicesSipperCruiseList::LookUpByName (String^  cruiseName)
{
  String^  cruiseNameUpper = cruiseName->ToUpper ();

  for each  (PicesSipperCruise^  c in (*this))
  {
    if  (c->CruiseName->ToUpper () == cruiseNameUpper)
      return  c;
  }

  return  nullptr;
}  /* LookUpByName */




