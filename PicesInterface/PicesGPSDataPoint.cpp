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
#include "PicesGPSDataPoint.h"
#include "PicesMethods.h"


using namespace  PicesInterface;

namespace  PicesInterface
{
  PicesGPSDataPoint::PicesGPSDataPoint (PicesGPSDataPoint^  stat):
      ctdDateTime  (stat->ctdDateTime),
      gpsStartTime (stat->gpsStartTime),
      sipperFileId (stat->sipperFileId),
      avgScanLine  (stat->avgScanLine),
      avgLatitude  (stat->avgLatitude),
      avgLongitude (stat->avgLongitude),
      avgFlowRate  (stat->avgFlowRate)
  {
  }


  PicesGPSDataPoint::PicesGPSDataPoint (const GPSDataPoint&  stat):
      ctdDateTime  (PicesMethods::DateTimeKKUtoSystem (stat.CtdDateTime  ())),
      gpsStartTime (PicesMethods::TimeTypeKKUtoSystem (stat.GPSStartTime ())),
      sipperFileId (stat.SipperFileId ()),
      avgScanLine  (stat.AvgScanLine  ()),
      avgLatitude  (stat.AvgLatitude  ()),
      avgLongitude (stat.AvgLongitude ()),
      avgFlowRate  (stat.AvgFlowRate  ())
  {
  }


  PicesGPSDataPoint::!PicesGPSDataPoint ()
  {
  }


  PicesGPSDataPoint::~PicesGPSDataPoint ()
  {
    this->!PicesGPSDataPoint ();
  }



  PicesGPSDataPointList::PicesGPSDataPointList ()
  {
  }



  PicesGPSDataPointList::PicesGPSDataPointList (PicesGPSDataPointList^  stats)
  {
    for each (PicesGPSDataPoint^ stat in stats)
    {
      Add (stat);
    }  
  }



  PicesGPSDataPointList::PicesGPSDataPointList (const GPSDataPointList&  stats)
  {
    GPSDataPointList::const_iterator  idx;
    for  (idx = stats.begin ();  idx != stats.end ();  ++idx)
      Add (gcnew PicesGPSDataPoint (*(*idx)));
  }


  PicesGPSDataPointList::~PicesGPSDataPointList ()
  {
    this->!PicesGPSDataPointList ();
  }



  PicesGPSDataPointList::!PicesGPSDataPointList ()
  {
    CleanUpMemory ();
  }



  void  PicesGPSDataPointList::AddList (PicesGPSDataPointList^ list)
  {
    for each (PicesGPSDataPoint^ stat in list)
    {
      Add (stat);
    }
  }



  void  PicesGPSDataPointList::AddList (const GPSDataPointList&  stats)
  {
    GPSDataPointList::const_iterator  idx;
    for  (idx = stats.begin ();  idx != stats.end ();  ++idx)
      Add (gcnew PicesGPSDataPoint (*(*idx)));
  }



  void  PicesGPSDataPointList::CleanUpMemory ()
  {
  }
}  /* PicesInterface */
