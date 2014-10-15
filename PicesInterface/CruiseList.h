#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::IO;



#include  "Cruise.h"

namespace PicesInterface {
  public  ref  class  CruiseList:  List<Cruise^>
  {
  public:

    CruiseList (String^  baseDir);


    Cruise^  LookUpByName (String^  name);

  private:
    void  Load (String^ baseDir);
  };  /* CruiseList */
}
