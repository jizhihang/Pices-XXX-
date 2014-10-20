#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

#include  "PicesClass.h"

namespace PicesInterface
{
  public  ref  class  PicesPrediction
  {
  public:
    PicesPrediction  (PicesClass^  _mlClass,
                      int          _votes,
                      double       _probability
                     );

    property PicesClass^  MLClass   {PicesClass^ get ()  {return  mlClass;}   void  set (PicesClass^  _mlClass)  {mlClass  = _mlClass;}}
    property double       Probability  {double      get ()  {return  probability;}  void  set (double       _probability) {probability = _probability;}}
    property String^      ClassName    {String^     get ();}
    property int          Votes        {int         get ()  {return  votes;}        void  set (int          _votes)       {votes       = _votes;}}
   
    void  Refresh (PicesPrediction^  p);


  private:
    PicesClass^  mlClass;
    double       probability;
    int          votes;

  };
}
