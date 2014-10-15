#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;

#include  "DateTime.h"
#include  "PicesRunLog.h"


namespace PicesInterface {

  public ref class PicesMethods
  {
  public:
    PicesMethods ();

    static  array<String^>^    GetListOfSipperFiles (PicesRunLog^  log);

    static  String^            SipperFileNameFromImageFileName (String^  imageFileName);

    static  System::DateTime   DateKKUtoSystem (const  KKU::DateType&  date);

    static  KKU::DateType      DateSystemToKKU (System::DateTime   dt);

    static  System::DateTime   DateTimeKKUtoSystem (const  KKU::DateTime&  date);

    static  KKU::DateTime      DateTimeSystemToKKU (System::DateTime   dt);

    static  System::TimeSpan   TimeTypeKKUtoSystem (const KKU::TimeType&  t);

    static  System::String^    LatitudeToString (double latitude,
                                                 int    minDecimals
                                                );

    static  System::String^    LongitudeToString (double longitude,
                                                  int    minDecimals
                                                 );

    static  System::String^    LatitudeLongitudeToString (double latitude,
                                                          double longitude
                                                         );

    static  void               StartMemoryLeakDetection ();

  };  /* PicesMethods */


}  /* PicesInterface */