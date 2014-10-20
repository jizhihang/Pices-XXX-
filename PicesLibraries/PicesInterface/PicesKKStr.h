#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;

#include  "DateTime.h"
#include  "Str.h"


namespace PicesInterface 
{
  public  ref class PicesKKStr
  {
  public:
    PicesKKStr ();

    static
      KKU::KKStr  SystemStringToKKStr (System::String^  s);

    static
      KKU::KKStrPtr  SystemStringToKKStrPtr (System::String^  s);

    static
      KKU::KKStrListPtr  SystemStringArrayToKKStrListPtr (array<String^>^  list);

    static
      KKU::KKStrListPtr  SystemStringListToKKStrListPtr (List<String^>^  list);

    static
      System::String^  KKStrToSystenStr (const KKU::KKStr&  s);

    static System::Byte      StrToByte      (String^ s);
    static bool              StrToBool      (String^ s);
    static char*             StrToCharStar  (String^ s);
    static System::DateTime  StrToDateTime  (String^ s);
    static double            StrToDouble    (String^ s);
    static float             StrToFloat     (String^ s);
    static int               StrToInt       (String^ s);
    static double            StrToLatitude  (String^ s);
    static long              StrToLong      (String^ s);
    static double            StrToLongitude (String^ s);
    static KKU::uint         StrToUint      (String^ s);

    static System::Int64     StrToInt64     (String^ s);
    static System::UInt64    StrToUInt64    (String^ s);

    static String^           LatitudeToStr  (double latitude);
    static String^           LongitudeToStr (double longitude);
  };


  KKU::KKStr&  operator<< (KKU::KKStr&      left,
                           System::String^  right
                          );

  String^ operator<< (String^      left,
                      KKU::KKStr&  right
                     );
}  /* PicesInterface */
