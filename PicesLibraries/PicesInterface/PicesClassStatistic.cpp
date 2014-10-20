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

#include  "..\BaseLibrary\GoalKeeper.h"
using namespace KKU;

#include  "PicesClass.h"
#include  "PicesClassList.h"
#include  "PicesClassStatistic.h"
using namespace PicesInterface;

using namespace System;
using namespace System;
using namespace System::Windows::Forms;



namespace  PicesInterface
{
  PicesClassStatistic::PicesClassStatistic (const ClassStatistic&  classStat)
  {
    mlClass = PicesClassList::GetUniqueClass (classStat.MLClass ());
    count      = classStat.Count ();
  }



  PicesClassStatistic::PicesClassStatistic (PicesClass^  _mlClass,
                                            uint         _count
                                           )
  {
    mlClass = _mlClass;
    count      = _count;
  }



  String^  PicesClassStatistic::ClassName::get ()
  {
    if  (mlClass == nullptr)
      return String::Empty;

    return  mlClass->Name;
  }



  PicesClassStatisticList::PicesClassStatisticList ()
  {
  }



  PicesClassStatisticList::PicesClassStatisticList (const ClassStatisticList&  stats)
  {
    ClassStatisticList::const_iterator  idx;
    for  (idx = stats.begin ();  idx != stats.end ();  idx++)
    {
      Add (gcnew PicesClassStatistic (**idx));
    }
  }



  int  ComparePicesClassStatistic (PicesClassStatistic^ left,
                                   PicesClassStatistic^ right
                                  )
  {
    if  (left == nullptr)
    {
      if  (right == nullptr)
        return 0;
      else
        return 1;
    }

    else if  (right == nullptr)
      return -1;

    else
      return  String::Compare(left->ClassName, right->ClassName);
  }  /* ComparePicesClassStatistic*/




  void  PicesClassStatisticList::SortByClassName ()
  {
    Sort(gcnew Comparison<PicesClassStatistic^>(ComparePicesClassStatistic));
  }  /* SortByClassName */




  void  PicesClassStatisticList::Add (PicesClass^  mlClass,
                                      uint         count
                                     )
  {
    for each  (PicesClassStatistic^  stat  in  (*this))
    {
      if  (stat->MLClass == mlClass)
      {
        stat->Count = stat->Count + count;
        return;
      }
    }

    this->Add (gcnew PicesClassStatistic (mlClass, count));
  }  /* AddInAClass */




  void  PicesClassStatisticList::Add (PicesClassStatisticList^  stats)
  {
    for each  (PicesClassStatistic^  stat  in  stats)
    {
      Add (stat->MLClass, stat->Count);
    }
  }



  KKU::uint  PicesClassStatisticList::ImageCountTotal::get ()  
  {
    uint  imageCountTotal = 0;
    for each  (PicesClassStatistic^ stat in (*this))
      imageCountTotal += stat->Count;
    return  imageCountTotal;
  }



  KKU::uint  PicesClassStatisticList::CountTotal::get ()  
  {
    uint  countTotal = 0;
    for each  (PicesClassStatistic^ stat in (*this))
      countTotal += stat->Count;
    return  countTotal;
  }


  
  PicesClassStatisticList^  PicesClassStatisticList::Summarize (int hierarchyLevel)
  {
    SortByClassName ();

    PicesClassStatisticList^  summary = gcnew PicesClassStatisticList ();

    PicesClassStatistic^  lastSumClassStat = nullptr;

    for each (PicesClassStatistic^  classStat in *this)
    {
      PicesClass^  sumClass = classStat->MLClass->MLClassForGivenHierarchialLevel (hierarchyLevel);
      if  ((lastSumClassStat == nullptr)  ||  (sumClass != lastSumClassStat->MLClass))
      {
        lastSumClassStat = gcnew PicesClassStatistic (sumClass, 0);
        summary->Add (lastSumClassStat);
      }

      lastSumClassStat->Count = lastSumClassStat->Count + classStat->Count;
    }

    return  summary;
  }  /* Summarize */



  PicesClassStatisticList^  PicesClassStatisticList::RemoveNoiseClasses ()
  {
    PicesClassStatisticList^  result = gcnew PicesClassStatisticList ();

    for each (PicesClassStatistic^  classStat in *this)
    {
      String^ cn = classStat->ClassName;
      if  ((cn->Length < 5) ||
           (cn->Substring (0, 5)->ToLower () != "noise")
          )
        result->Add (classStat);
    }

    return  result;
  }




  ClassStatisticListPtr  PicesClassStatisticList::ToUnmanagedClassStatisticList ()
  {
    ClassStatisticListPtr  results = new ClassStatisticList (true);
    for each  (PicesClassStatistic^  pcs in *this)
    {
      PicesClass^ pc = pcs->MLClass;
      uint32 count = pcs->Count;
      results->PushOnBack (new ClassStatistic (pc->UnmanagedMLClass (), count));
    }
    return  results;
  }  /* ToUnmanagedClassStatisticList */



  PicesClassList^  PicesClassStatisticList::ToPicesClassList ()
  {
    PicesClassList^  results = gcnew PicesClassList ();
    for each  (PicesClassStatistic^  pcs in *this)
    {
      PicesClass^ pc = pcs->MLClass;
      results->Add (pc);
    }
    return  results;
  }



}  /* PicesInterface */



