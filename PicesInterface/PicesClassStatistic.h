#ifndef  _PICESCLASSLIST_
#define  _PICESCLASSLIST_

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

#include  "ClassStatistic.h"
#include  "MLClass.h"

#include  "PicesClass.h"


namespace PicesInterface
{
  /*!
   \class  PicesClassStatistic
   \brief  Used as a vehicle to return statistics for a goven class.
   */

  public ref  class  PicesClassStatistic
  {
  public:
    PicesClassStatistic (const ClassStatistic&  classStat);

    PicesClassStatistic (PicesClass^ _mlClass,
                         uint        _count
                        );

    property  String^      ClassName   {String^      get ();}
    property  uint         Count       {uint         get ()  {return  count;}        void  set (uint         _count)       {count      = _count;}}
    property  PicesClass^  MLClass  {PicesClass^  get ()  {return  mlClass;}   void  set (PicesClass^  _mlClass)  {mlClass = _mlClass;}}

  private:
    uint         count;
    PicesClass^  mlClass;
  };




  public  ref  class  PicesClassStatisticList:  public  List<PicesClassStatistic^>
  {
  public:
    typedef  KKU::uint  uint;

    PicesClassStatisticList ();

    PicesClassStatisticList (const ClassStatisticList&  stats);

    void  Add (PicesClass^  mlClass,
               uint         count
              );

    void  Add (PicesClassStatisticList^  stats);

    property  uint  CountTotal  {uint get ();}

    property  uint  ImageCountTotal  {uint  get ();}

    void  SortByClassName ();

    PicesClassStatisticList^  RemoveNoiseClasses ();

    PicesClassStatisticList^  Summarize (int hierarchyLevel);

    PicesClassList^        ToPicesClassList ();

    ClassStatisticListPtr  ToUnmanagedClassStatisticList ();

  };
}
#endif
