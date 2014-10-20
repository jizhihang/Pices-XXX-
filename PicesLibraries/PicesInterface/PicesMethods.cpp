#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


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

using namespace std;
using namespace KKU;

#include  "InstrumentDataFileManager.h"
using  namespace SipperHardware;

#include  "PicesMethods.h"

#include  "PicesKKStr.h"



using namespace PicesInterface;


PicesMethods::PicesMethods ()
{
}


/**
 @brief Returns a list of all known SIPPER files on the local PC; each entry includes the file path.  
 */
array<String^>^    PicesMethods::GetListOfSipperFiles (PicesRunLog^  log)
{
  VectorKKStr sipperFileNames = InstrumentDataFileManager::GetListOfSipperFiles (log->Log ());

  array<String^>^ results = gcnew array<String^> (sipperFileNames.size ());

  VectorKKStr::iterator  idx;
  int  idx2 = 0;
  for  (idx = sipperFileNames.begin ();  idx != sipperFileNames.end ();  idx++)
  {
    results[idx2] = PicesKKStr::KKStrToSystenStr (*idx);
    idx2++;
  }
  
  return  results;
}  /* GetListOfSipperFiles */




String^  PicesMethods::SipperFileNameFromImageFileName (String^  imageFileName)
{
  KKStr  sipperFileName = InstrumentDataFileManager::SipperFileRootNameFromSipperImageFileName (PicesKKStr::SystemStringToKKStr (imageFileName));
  return  PicesKKStr::KKStrToSystenStr (sipperFileName);
}




System::DateTime   PicesMethods::DateKKUtoSystem (const  KKU::DateType&  date)
{
  return  System::DateTime (date.Year (), date.Month (),  date.Day ());
}


KKU::DateType   PicesMethods::DateSystemToKKU (System::DateTime   dt)
{
  
  return  KKU::DateType (dt.Year, dt.Month, dt.Day);
}





System::DateTime   PicesMethods::DateTimeKKUtoSystem (const  KKU::DateTime&  date)
{
  int  year  = date.Date ().Year  ();
  int  month = date.Date ().Month ();
  int  day   = date.Date ().Day   ();

  int  hours   = date.Time ().Hour   ();
  int  minutes = date.Time ().Minute ();
  int  seconds = date.Time ().Second ();

  bool valid = true;

  year    = Max (1, year);
  month   = Min (12, Max (1, month));
  day     = Min (31, Max (1, day));
  hours   = Min (24, Max (0, hours));
  minutes = Min (60, Max (0, minutes));

  System::DateTime  dt;
  try
  {
    dt = System::DateTime (year, month, day, hours, minutes, seconds);
  }
  catch (Exception^)
  {
    dt = System::DateTime (1900, 1, 1, 0, 0, 0);
  }

  return  dt;
}  /* DateTimeKKUtoSystem */



KKU::DateTime   PicesMethods::DateTimeSystemToKKU (System::DateTime   dt)
{
  
  return  KKU::DateTime (dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);
}


System::TimeSpan   PicesMethods::TimeTypeKKUtoSystem (const KKU::TimeType&  t)
{
  System::TimeSpan  td ((int)t.Hour (), (int)t.Minute (), (int)t.Second ());
  return  td;
}



System::String^   PicesMethods::LatitudeToString (double latitude,
                                                  int    minDecimals
                                                 )
{
  String^  minFormatStr = "00";
  switch  (minDecimals)
  {
  case 0: minFormatStr = "00";          break;
  case 1: minFormatStr = "00.0";        break;
  case 2: minFormatStr = "00.00";       break;
  case 3: minFormatStr = "00.000";      break;
  case 4: minFormatStr = "00.0000";     break;
  case 5: minFormatStr = "00.00000";    break;
  case 6: minFormatStr = "00.000000";   break;
  case 7: minFormatStr = "00.0000000";  break;
  }

  bool  north = (latitude >= 0.0);
  latitude = fabs (latitude);

  double  latitudeDegrees = 0.0;
  double  latitudeMinutes = modf (latitude, &latitudeDegrees) * 60.0;

  System::String^  latStr  = latitudeDegrees.ToString  ("00") + ":" + latitudeMinutes.ToString  (minFormatStr) + (north ? "N" : "S");

  return latStr;
}


System::String^   PicesMethods::LongitudeToString (double longitude,
                                                   int    minDecimals
                                                  )
{
  String^  minFormatStr = "00";
  switch  (minDecimals)
  {
  case 0: minFormatStr = "00";          break;
  case 1: minFormatStr = "00.0";        break;
  case 2: minFormatStr = "00.00";       break;
  case 3: minFormatStr = "00.000";      break;
  case 4: minFormatStr = "00.0000";     break;
  case 5: minFormatStr = "00.00000";    break;
  case 6: minFormatStr = "00.000000";   break;
  case 7: minFormatStr = "00.0000000";  break;
  }

  while  (longitude > 180.0)
    longitude = longitude - 360.0;

  while  (longitude <= -180.0)
    longitude = longitude + 360;
  bool  west  = (longitude < 0.0);

  longitude = fabs (longitude);

  double  longitudeDegrees = 0.0;
  double  longitudeMinutes = modf (longitude, &longitudeDegrees) * 60.0;

  System::String^  longStr = longitudeDegrees.ToString ("00") + ":" + longitudeMinutes.ToString (minFormatStr) + (west  ? "W" : "E");

  return longStr;
}






System::String^   PicesMethods::LatitudeLongitudeToString (double latitude,
                                                           double longitude
                                                          )
{
  // It s assumed that both lastitude and longtitude are expressed in degrees

  bool  north = (latitude >= 0.0);
  latitude = fabs (latitude);

  while  (longitude > 180.0)
    longitude = longitude - 360.0;

  while  (longitude <= -180.0)
    longitude = longitude + 360;
  bool  west  = (longitude < 0.0);

  longitude = fabs (longitude);

  double  longitudeDegrees = 0.0;
  double  longitudeMinutes = modf (longitude, &longitudeDegrees) * 60.0;

  double  latitudeDegrees = 0.0;
  double  latitudeMinutes = modf (latitude, &latitudeDegrees) * 60.0;

  System::String^  latStr  = latitudeDegrees.ToString  ("00") + ":" + latitudeMinutes.ToString  ("00.0000") + (north ? "N" : "S");
  System::String^  longStr = longitudeDegrees.ToString ("00") + ":" + longitudeMinutes.ToString ("00.0000") + (west  ? "W" : "E");

  return latStr + " " + longStr;
}  /* LatitudeLongitudeToString */



void  PicesMethods::StartMemoryLeakDetection ()
{
  _CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}