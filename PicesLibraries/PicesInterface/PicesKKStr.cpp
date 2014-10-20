#include "StdAfx.h"
#include "FirstIncludes.h"

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

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace System;
using namespace KKU;


#include "OSservices.h"
#include "PicesKKStr.h"

using namespace  PicesInterface;


PicesKKStr::PicesKKStr ()
{
}



KKStr  PicesKKStr::SystemStringToKKStr (System::String^  s)
{
  if  (s == nullptr)
    return KKStr::EmptyStr ();

  KKU::KKStr  kkStr (s->Length);
  for  (int x = 0;  x < s->Length;  x++)
    kkStr.Append ((char)s[x]);
  return  kkStr;
}


KKStrPtr  PicesKKStr::SystemStringToKKStrPtr (System::String^  s)
{
  if  (s == nullptr)
    return new KKStr ();

  KKU::KKStrPtr  kkStr = new KKStr (s->Length);
  for  (int x = 0;  x < s->Length;  x++)
    kkStr->Append ((char)s[x]);
  return  kkStr;
}




KKU::KKStrListPtr  PicesKKStr::SystemStringArrayToKKStrListPtr (array<String^>^  list)
{
  KKStrListPtr  kkStrList = new KKStrList (true);
  for each (String^ s in list)
    kkStrList->PushOnBack (SystemStringToKKStrPtr (s));
  return  kkStrList;
}



KKU::KKStrListPtr  PicesKKStr::SystemStringListToKKStrListPtr (List<String^>^  list)
{
  KKStrListPtr  kkStrList = new KKStrList (true);
  for each (String^ s in list)
    kkStrList->PushOnBack (SystemStringToKKStrPtr (s));
  return  kkStrList;
}




System::String^  PicesKKStr::KKStrToSystenStr (const KKStr&  s)
{
  System::String^  sysStr = gcnew System::String (s.Str ());
  return  sysStr;
}


byte  PicesKKStr::StrToByte (String^  s)
{
  byte i = 0;
  try  {i = System::Byte::Parse (s);}
  catch (Exception^){i = 0;}
  return  i;
}  /* StrToByte */



char*  PicesKKStr::StrToCharStar (String^ s)
{
  if  (s == nullptr)
    return _strdup ("");

  int len = s->Length;
  char*  result = new char[len + 1];
  for  (int x = 0;  x < len;  x++)
    result[x] = (char)s[x];
  result[len] = 0;
  return  result;
}  /* StrToCharStar*/



String^  StripOutCommas (String^ s)
{
  if  (s == nullptr)
    return  String::Empty;

  int  zed = s->IndexOf (',');
  while  (zed >= 0)
  {
    s = s->Remove (zed, 1);
    zed = s->IndexOf (',');
  }
  s = s->Trim (nullptr);
  return  s;
}  /* StripOutCommas */



uint  PicesKKStr::StrToUint (String^  s)
{
  uint i = 0;
  try {i = (uint)System::UInt32::Parse (StripOutCommas (s));}
  catch  (Exception^) {i = 0;}
  return  i;
}  /* StrToUint */



int   PicesKKStr::StrToInt (String^  s)
{
  int i = 0;

  try {i = int::Parse (StripOutCommas (s));}
  catch  (Exception^) {i = 0;}
  return  i;
}  /* StrToInt */



double  PicesKKStr::StrToLatitude  (String^ s)
{
  KKStr kkStr = PicesKKStr::SystemStringToKKStr (s);
  return  kkStr.ToLatitude ();
}


double  PicesKKStr::StrToLongitude (String^ s)
{
  KKStr kkStr = PicesKKStr::SystemStringToKKStr (s);
  return  kkStr.ToLongitude ();
}



long  PicesKKStr::StrToLong (String^ s)
{
  long i = 0;
  try  {i = long::Parse(StripOutCommas (s)); }
  catch (Exception^) { i = 0; }
  return i;
}  /* StrToInt */



System::Int64   PicesKKStr::StrToInt64 (String^ s)
{
  Int64 i = 0;
  try  {i = Int64::Parse(StripOutCommas (s)); }
  catch (Exception^) { i = 0; }
  return i;
}


System::UInt64   PicesKKStr::StrToUInt64 (String^ s)
{
  UInt64 i = 0;
  try  {i = UInt64::Parse(StripOutCommas (s)); }
  catch (Exception^) { i = 0; }
  return i;
}



bool  PicesKKStr::StrToBool (String^ s)
{
  String^ u = s->ToUpper ();
  if  ((u == "T")  ||  (u == "TRUE")  ||  (u == "Y")  ||  (u == "YES")  ||  (u =="ON") ||  (u == "1"))
    return true;
  else
    return false;
}  /* StrToBool */



float  PicesKKStr::StrToFloat (String^ s)
{
  bool  percentage = false;
  int  perectIndex = s->LastIndexOf ("%");
  if  (perectIndex >= 0)
  {
    percentage = true;
    s = s->Remove (perectIndex, 1);
  }  

  float  f = 0.0f;
  try  {f = float::Parse (StripOutCommas (s)); }
  catch (Exception^) {f = 0.0f;}

  if  (percentage)
    f = f / 100.0f;
  return f ;
}  /* StrToFloat */



double  PicesKKStr::StrToDouble (String^ s)
{
  double v = 0.0;
  try {v = double::Parse (StripOutCommas (s));}
  catch (Exception^){v = 0.0;}
  return v;
}  /* StrToDouble */




System::DateTime  PicesKKStr::StrToDateTime (String^ s)
{
  System::DateTime t = System::DateTime::Now;
  try {t = System::DateTime::Parse (s);}
  catch (Exception^)
  {
    KKStr  ds = PicesKKStr::SystemStringToKKStr (s);
    KKU::DateTime  dt (ds);
    t = PicesInterface::OSservices::KKuDateTimeToSystemDateTime (dt);
  }
  return t;
}  /* StrToDateTime */





KKStr&  PicesInterface::operator<< (KKStr&           left,
                                    System::String^  right
                                   )
{
  if  (right != nullptr)
  {
    for  (int x = 0;  x < right->Length;  x++)
      left.Append ((char)right[x]);
  }

  return  left;
}  /* operator<< */





String^ PicesInterface::operator<< (String^  left,
                                    KKStr&   right
                                   )
{
  left = left + PicesKKStr::KKStrToSystenStr (right);
  return left;
}




String^  PicesKKStr::LatitudeToStr  (double latitude)
{
  bool  north = (latitude >= 0.0);
  latitude = fabs (latitude);

  while  (latitude > 90)
  {
    latitude = 180 - latitude;
    if  (latitude < 0.0)
    {
      latitude = fabs (latitude);
      north = !north;
    }
  }

  double  latitudeDegrees = 0.0;
  double  latitudeMinutes = modf (latitude, &latitudeDegrees) * 60.0;

  System::String^  latStr  = latitudeDegrees.ToString  ("#0")      +
                             ":"                                   +
                             latitudeMinutes.ToString  ("00.0000") +
                             (north ? "N" : "S");
  return  latStr;
}  /* LatitudeToStr */



String^  PicesKKStr::LongitudeToStr  (double longitude)
{
  bool  east = longitude >= 0.0;
  longitude = fabs (longitude);
  while  (longitude > 180.0)
  {
    longitude = longitude - 360.0;
    if  (longitude < 0.0)
    {
      longitude = fabs (longitude);
      east = !east;
    }
  }

  double  longitudeDegrees = 0.0;
  double  longitudeMinutes = modf (longitude, &longitudeDegrees) * 60.0;

  System::String^  longStr = longitudeDegrees.ToString ("##0")     + 
                             ":"                                   + 
                             longitudeMinutes.ToString ("00.0000") + 
                             (east  ? "E" : "W");
  return  longStr;
}  /* LongitudeToStr */
