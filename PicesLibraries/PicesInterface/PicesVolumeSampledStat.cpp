#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <istream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Collections;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include "MemoryDebug.h"
#include "BasicTypes.h"
#include "Point.h"
using namespace KKU;


#include "PicesKKStr.h"
#include "PicesVolumeSampledStat.h"


using namespace  PicesInterface;

namespace  PicesInterface
{


  PicesVolumeSampledStat::PicesVolumeSampledStat (PicesVolumeSampledStat^  stat):
     binId         (stat->BinId),
     binDepth      (stat->BinDepth),
     volumeSampled (stat->VolumeSampled)
  {
  }

  PicesVolumeSampledStat::PicesVolumeSampledStat (const VolumeSampledStat&  stat):
     binId         (stat.BinId         ()),
     binDepth      (stat.BinDepth      ()),
     volumeSampled (stat.VolumeSampled ())
  {
  }



  PicesVolumeSampledStat::!PicesVolumeSampledStat ()
  {
  }


  PicesVolumeSampledStat::~PicesVolumeSampledStat ()
  {
    this->!PicesVolumeSampledStat ();
  }



  PicesVolumeSampledStatList::PicesVolumeSampledStatList ()
  {
  }

  
  PicesVolumeSampledStatList::PicesVolumeSampledStatList (const VolumeSampledStatList&  stats)
  {
    VolumeSampledStatList::const_iterator  idx;
    for  (idx = stats.begin ();  idx != stats.end ();  ++idx)
      Add (gcnew PicesVolumeSampledStat (*(idx->second)));
  }


  PicesVolumeSampledStatList::~PicesVolumeSampledStatList ()
  {
    this->!PicesVolumeSampledStatList ();
  }



  PicesVolumeSampledStatList::!PicesVolumeSampledStatList ()
  {
    CleanUpMemory ();
  }



  void  PicesVolumeSampledStatList::AddList (PicesVolumeSampledStatList^ list)
  {
    for each (PicesVolumeSampledStat^ stat in list)
    {
      Add (gcnew PicesVolumeSampledStat (stat));
    }
  }


  void  PicesVolumeSampledStatList::AddList (const VolumeSampledStatList&  stats)
  {
    VolumeSampledStatList::const_iterator  idx;
    for  (idx = stats.begin ();  idx != stats.end ();  ++idx)
      Add (gcnew PicesVolumeSampledStat (*(idx->second)));
  }


  void  PicesVolumeSampledStatList::CleanUpMemory ()
  {
  }


  PicesVolumeSampledStat^  PicesVolumeSampledStatList::Locate (int binId)
  {
    for each (PicesVolumeSampledStat^ stat in *this)
    {
      if  (stat->BinId == binId)
        return stat;
    }
    return  nullptr;
  }


  int  PicesVolumeSampledStatList::LargestBinId ()
  {
    int  largestBinId = -1;
    for each (PicesVolumeSampledStat^  s in *this)
    {
      if  (s->BinId > largestBinId)
        largestBinId = s->BinId;
    }
    return  largestBinId;
  }



  array<float>^  PicesVolumeSampledStatList::ToArray ()
  {
    int numBins = LargestBinId () + 1;
    array<float>^  result = gcnew array<float>(numBins);

    for each (PicesVolumeSampledStat^  s in *this)
    {
      result[s->BinId] = s->VolumeSampled;
    }
    return  result;
  }



  void   PicesVolumeSampledStatList::MergeIn (PicesVolumeSampledStatList^ right)
  {
    for each  (PicesVolumeSampledStat^ stat in right)
    {
      PicesVolumeSampledStat^ existingStat = Locate (stat->BinId);
      if  (existingStat == nullptr)
        Add (gcnew PicesVolumeSampledStat (stat));
      else
        stat->VolumeSampled = stat->VolumeSampled + existingStat->VolumeSampled;
    }
  }



}  /* PicesInterface */
