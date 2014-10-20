#include  "FirstIncludes.h"
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>


#include  <iostream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
using  namespace  KKU;


#include  "SipperHeaderRec.h"
using  namespace  SipperHardware;





PortAssignment::PortAssignment (InstrumentPtr  _instrument,
                                const KKStr&  _desc
                               ):
     instrument (_instrument),
     desc       (_desc)
{
}


PortAssignment::PortAssignment (const KKStr&  _shortName,
                                const KKStr&  _desc
                               ):
     instrument (NULL),
     desc       (_desc)

{
  instrument = Instrument::LookUpByShortName (_shortName);
}



PortAssignment::PortAssignment (const PortAssignment&  _portAssignment)
{
  instrument = _portAssignment.instrument;
  desc       = _portAssignment.desc;
}



     
KKStr  PortAssignment::HeaderStr ()  const
{
  KKStr  s (50);
  
  s << instrument->ShortName ();
  if  (instrument->Other ())
    s << "," + desc;
  return  s;
}  /* HeaderStr */



const KKStr&  PortAssignment::ShortName ()  const
{
  if  (instrument)
    return instrument->ShortName ();
  else
    return KKStr::EmptyStr ();
}




SipperHeaderRec::SipperHeaderRec ():
    sipperFileName    (),
    cruiseLocation    (),
    objective         (),
    principal         (),
    researchOrg       (),
    cruiseDateStart   (),
    cruiseDateEnd     (),
    deploymentVehicle (),
    seaConditions     (),
    waveHeight        (),
    weatherConditions (),
    airTemp           (),
    latitude          (0.0),
    longitude         (0.0),
    scanRate          (0),

    portAssignments   ()

{
  InitFields ();
}






SipperHeaderRec::SipperHeaderRec (const SipperHeaderRec&  _headerRec):
    sipperFileName    (_headerRec.sipperFileName),
    objective         (_headerRec.objective),
    principal         (_headerRec.principal),
    researchOrg       (_headerRec.researchOrg),
    cruiseDateStart   (_headerRec.cruiseDateStart),
    cruiseDateEnd     (_headerRec.cruiseDateEnd),
    deploymentVehicle (_headerRec.deploymentVehicle),
    seaConditions     (_headerRec.seaConditions),
    waveHeight        (_headerRec.waveHeight),
    weatherConditions (_headerRec.weatherConditions),
    airTemp           (_headerRec.airTemp),
    latitude          (_headerRec.latitude),
    longitude         (_headerRec.longitude),
    scanRate          (_headerRec.scanRate),

    ctdExt0Code       (_headerRec.ctdExt0Code),
    ctdExt1Code       (_headerRec.ctdExt1Code),
    ctdExt2Code       (_headerRec.ctdExt2Code),
    ctdExt3Code       (_headerRec.ctdExt3Code),

    portAssignments   ()

{
  uint32 x;
  for  (x = 0;  x < _headerRec.portAssignments.size ();  x++)
  {
    if  (_headerRec.portAssignments[x])
      portAssignments.push_back (new PortAssignment (*_headerRec.portAssignments[x]));
  }
}




SipperHeaderRec::~SipperHeaderRec ()
{
  ClearOutExistingPortAssignments ();
}




void  SipperHeaderRec::ClearOutExistingPortAssignments ()
{
  while  (portAssignments.size () > 0)
  {
    PortAssignmentPtr  a = portAssignments.back ();
    delete  a;  a = NULL;
    portAssignments.pop_back ();
  }
}  /* ClearOutExistingPortAssignments */




void  SipperHeaderRec::InitFields ()
{
  sipperFileName    = "";
  cruiseLocation    = "";
  objective         = "";
  principal         = "";
  researchOrg       = "";
  cruiseDateStart   = osGetLocalDateTime ().Date ();
  cruiseDateEnd     = osGetLocalDateTime ().Date ();
  deploymentVehicle = "";
  seaConditions     = "";
  waveHeight        = "";
  weatherConditions = "";
  airTemp           = "";
  scanRate          = 0;
  latitude          = 0.0;
  longitude         = 0.0;

  ctdExt0Code       = "";
  ctdExt1Code       = "";
  ctdExt2Code       = "";
  ctdExt3Code       = "";

  ClearOutExistingPortAssignments ();
}   /* InitFields */
    



// Builds Header string that will be placed at beginning of SIPPER file
// with the sfd command.
KKStr  SipperHeaderRec::HeaderStr ()
{
  uint32  x = 0;
  KKStr  s (500);

  s << "fn" << "\t" << sipperFileName                   + "\t" 
    << "sr" << "\t" << StrFormatInt (scanRate, "zzzz0") + "\t";

  for  (x = 0;  x < portAssignments.size ();  x++)
  {
    s << "sp" << "," << portAssignments[x]->HeaderStr () << "\t";
  }

  s << "cl"   <<  "\t"  << cruiseLocation     << "\t"
    << "ob"   <<  "\t"  << objective          << "\t"
    << "pr"   <<  "\t"  << principal          << "\t"
    << "ro"   <<  "\t"  << researchOrg        << "\t"
    << "cd"   <<  "\t"  << cruiseDateStart    << "," 
                        << cruiseDateEnd      << "\t"
    << "dv"   <<  "\t"  << deploymentVehicle  << "\t"
    << "sc"   <<  "\t"  << seaConditions      << "\t"
    << "wh"   <<  "\t"  << waveHeight         << "\t"
    << "wc"   <<  "\t"  << weatherConditions  << "\t"
    << "at"   <<  "\t"  << airTemp            << "\t"
    << "lat"  <<  "\t"  << StrFormatDouble (latitude,  "zz0.0000000")  << "\t"
    << "log"  <<  "\t"  << StrFormatDouble (longitude, "zz0.0000000")  << "\t"
    << "ext0" <<  "\t"  << ctdExt0Code        << "\t"
    << "ext1" <<  "\t"  << ctdExt1Code        << "\t"
    << "ext2" <<  "\t"  << ctdExt2Code        << "\t"
    << "ext3" <<  "\t"  << ctdExt3Code;

  return  s;
}  /* HeaderStr */




void  SipperHeaderRec::ParseHeaderStr (const KKStr&  s)
{
  InitFields ();

  VectorKKStr  fields = s.Split ('\t');

  uint32  x = 0;
  bool  found = true;

  while  (x < fields.size ())
  {
    KKStr  fieldName = fields[x].ToLower (); 
    x++;

    KKStr  fieldValue = "";

    int32 commaIdx = fieldName.LocateCharacter (',');
    if  (commaIdx < 0)
    {
      if  (x < fields.size ())
      {
        fieldValue = fields[x]; 
        x++;
      }
    }
    else
    {
      fieldValue = fieldName.SubStrPart (commaIdx + 1);
      fieldName  = fieldName.SubStrPart (0, commaIdx - 1);
    }

    ProcessNameAndDataStrings (fieldName, fieldValue, found);
  }
}  /* ParseDataFileLine */




void  SipperHeaderRec::ProcessNameAndDataStrings (const KKStr&   _fieldName,
                                                  const KKStr&   fieldValue,
                                                  bool&           found
                                                 )
{
  found = true;
  KKStr fieldName = _fieldName.ToLower ();

  if  ((fieldName == "at")  ||  (fieldName == "airtemp"))
    airTemp = fieldValue;

  else if  ((fieldName == "cd")  ||  (fieldName == "cruisedates"))
  {
     VectorKKStr  subFields = fieldValue.Split (',');
     
     cruiseDateStart = DateType (subFields[0]);
     if  (subFields.size () < 2)
       cruiseDateEnd = cruiseDateStart;
     else
       cruiseDateEnd = DateType (subFields[1]);
  }

  else if  ((fieldName == "cl")  ||  (fieldName == "cruiselocation"))
    cruiseLocation = fieldValue;

  else if  ((fieldName == "dv")  ||  (fieldName == "deploymentvehicle"))
    deploymentVehicle = fieldValue;

  else if  ((fieldName == "fn")  ||  (fieldName == "sipperfilename"))
    sipperFileName = fieldValue;
      
  else if  ((fieldName == "lat")  ||  (fieldName == "latitude"))
    latitude = fieldValue.ToDouble ();

  else if ((fieldName == "log") || (fieldName == "longitude"))
    longitude = fieldValue.ToDouble ();

  else if ((fieldName == "ob") || (fieldName == "objective"))
    objective = fieldValue;

  else if  ((fieldName == "pr")  ||  (fieldName == "principal"))
    principal = fieldValue;

  else if  ((fieldName == "ro")  ||  (fieldName == "researchorganization"))
    researchOrg = fieldValue;

  else if  ((fieldName == "sc")  ||  (fieldName == "seaconditions"))
    seaConditions = fieldValue;

  else if  ((fieldName == "sr")  ||  (fieldName == "scanrate"))
    scanRate = fieldValue.ToInt ();

  else if  ((fieldName == "sp")  ||  (fieldName == "serialport"))
  {
    // We are looking at a serial port definition

    ClearOutExistingPortAssignments ();
    VectorKKStr  subFields = fieldValue.Split (',');
    
    int32  serialPortNum = subFields[0].ToInt ();
    if  ((serialPortNum >= 0) && (serialPortNum < 6))
    {
      PortAssignmentPtr  portAssignment = NULL;

      if  (subFields.size () < 2)
         portAssignment = new PortAssignment ("", "");

      else if  (subFields.size () < 3)
         portAssignment = new PortAssignment (subFields[1], "");

      else
         portAssignment = new PortAssignment (subFields[1], subFields[2]);

      portAssignments.push_back (portAssignment);
    }
  }
 
  else if  ((fieldName == "ext0") || (fieldName == "ctdext0code"))
    ctdExt0Code = fieldValue;

  else if  ((fieldName == "ext1") || (fieldName == "ctdext1code"))
    ctdExt1Code = fieldValue;

  else if  ((fieldName == "ext2") || (fieldName == "ctdext2code"))
    ctdExt2Code = fieldValue;

  else if  ((fieldName == "ext3") || (fieldName == "ctdext3code"))
    ctdExt3Code = fieldValue;

  else if  ((fieldName == "wc")  ||  (fieldName == "weatherconditions"))
    weatherConditions = fieldValue;

  else if  ((fieldName == "wh")  ||  (fieldName == "waveheight"))
    waveHeight = fieldValue;

  else
    found = false;
}  /* ParseDataFileLine */



void  SipperHeaderRec::Save (ostream&  w)
{
   w << "AirTemp              = " << airTemp             << endl;
   w << "CruiseDates          = " << cruiseDateStart     << "," << cruiseDateEnd << endl;
   w << "CruiseLocation       = " << cruiseLocation      << endl;
   w << "deploymentVehicle    = " << deploymentVehicle   << endl;
   w << "SipperFilename       = " << sipperFileName      << endl;
   w << "Latitude             = " << StrFormatDouble (latitude,  "zz0.0000000") << endl; 
   w << "Longitude            = " << StrFormatDouble (longitude, "zz0.0000000") << endl;
   w << "Objective            = " << objective           << endl;
   w << "Principal            = " << principal           << endl;
   w << "ResearchOrganization = " << researchOrg         << endl;
   w << "SeaConditions        = " << seaConditions       << endl;
   w << "ScanRate             = " << StrFormatInt (scanRate, "zzzzzz0") << endl;

   for  (uint32 x = 0;  x < portAssignments.size ();  x++)
   {
     PortAssignmentPtr  portAssignment = portAssignments[x];

     if  (portAssignment)
     {
       w << "SerialPort           = " << x <<  ","  << portAssignment->HeaderStr ();
     }
   }

   if  (!ctdExt0Code.Empty ())
     w << "CtdExt0Code          = " << ctdExt0Code << endl;

   if  (!ctdExt1Code.Empty ())
     w << "CtdExt1Code          = " << ctdExt1Code << endl;

   if  (!ctdExt2Code.Empty ())
     w << "CtdExt2Code          = " << ctdExt2Code << endl;

   if  (!ctdExt3Code.Empty ())
     w << "CtdExt3Code          = " << ctdExt3Code << endl;

   w << "WeatherConditions    = " << weatherConditions << endl;
   w << "WaveHeight           = " << waveHeight        << endl;
 }  /* Save */



const KKStr&  SipperHeaderRec::SerialPortShortName (uint32  serialPortNum)  const
 {
   if  ((serialPortNum < 0)  ||  (serialPortNum >= portAssignments.size ()))
     return KKStr::EmptyStr ();

   if  (portAssignments[serialPortNum] == NULL)
     return  KKStr::EmptyStr ();

   return  portAssignments[serialPortNum]->ShortName ();
 }




const KKStr&  SipperHeaderRec::SerialPortDesc (uint32  serialPortNum)  const
{
   if  ((serialPortNum < 0)  ||  (serialPortNum >= portAssignments.size ()))
     return KKStr::EmptyStr ();

   if  (portAssignments[serialPortNum] == NULL)
     return KKStr::EmptyStr ();

   return  portAssignments[serialPortNum]->Desc ();
}



InstrumentPtr  SipperHeaderRec::SerialPortAssignment (uint32 serialPort)  const
{
  if  (serialPort >= portAssignments.size ())
    return NULL;

  if  (portAssignments[serialPort] == NULL)
    return  NULL;

  return  portAssignments[serialPort]->Instrument ();
}





InstrumentPtr  SipperHeaderRec::InclinationMeter (uint32  inclinationSerailPort)  const
{
  inclinationSerailPort = 0;
  uint32  x;
  for  (x = 0;  x < portAssignments.size ();  x++)
  {
    if  (portAssignments[x] != NULL)
    {
      if  (portAssignments[x]->ShortName () == "Inclination")
      {
        inclinationSerailPort = x;
        return  portAssignments[x]->Instrument ();
      }
    }
  }
      
  return  NULL;
}  /* InclinationMeter */




SipperHeaderRec&  SipperHeaderRec::operator= (const SipperHeaderRec&  right)
{
    sipperFileName    = right.sipperFileName;
    objective         = right.objective;
    principal         = right.principal;
    researchOrg       = right.researchOrg;
    cruiseDateStart   = right.cruiseDateStart;
    cruiseDateEnd     = right.cruiseDateEnd;
    deploymentVehicle = right.deploymentVehicle;
    seaConditions     = right.seaConditions;
    waveHeight        = right.waveHeight;
    weatherConditions = right.weatherConditions;
    airTemp           = right.airTemp;
    latitude          = right.latitude;
    longitude         = right.longitude;
    scanRate          = right.scanRate;
    portAssignments   = right.portAssignments;

   return  *this;
}  /* operator= */
