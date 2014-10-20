#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

#include  "GPSDataPoint.h"
using namespace  SipperHardware;

#include  "PicesKKStr.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesGPSDataPoint
  {
  public:
    PicesGPSDataPoint (PicesGPSDataPoint^  stat);

    PicesGPSDataPoint (const GPSDataPoint&  stat);

    !PicesGPSDataPoint ();
    ~PicesGPSDataPoint ();

    property System::DateTime  CtdDateTime         {System::DateTime  get ()  {return  ctdDateTime;}}
    property System::TimeSpan  GPSStartTime        {System::TimeSpan  get ()  {return  gpsStartTime;}}
    property int32             SipperFileId        {int32             get ()  {return  sipperFileId;}}
    property uint32            AvgScanLine         {uint32            get ()  {return  avgScanLine;}}
    property double            AvgLatitude         {double            get ()  {return  avgLatitude;}}
    property double            AvgLongitude        {double            get ()  {return  avgLongitude;}}
    property float             AvgFlowRate         {float             get ()  {return  avgFlowRate;}}
       
  private:
    System::DateTime  ctdDateTime;
    System::TimeSpan  gpsStartTime;   /**<  Time that this data point start covering from.  */
    int32             sipperFileId;
    uint32            avgScanLine;
    double            avgLatitude;
    double            avgLongitude;
    float             avgFlowRate;
  };  /* PicesGPSDataPoint */


  public  ref class  PicesGPSDataPointList:  List<PicesGPSDataPoint^>
  {
  public:
    PicesGPSDataPointList ();

    PicesGPSDataPointList (PicesGPSDataPointList^  stats);

    /**
     *@breief  Construct a list of managed PicesGPSDataPoint instances from the contents of the unmanaged list 'unmanagedImages'.
     *@details  Will NOT take ownership of the instances of 'GPSDataPoint' in 'stats but will create managed copies.
     *@param[in]  stats  List of unmanaged 'GPSDataPoint' objects; new Managed instances of 'GPSDataPoint' will be created and added to list.
     */
    PicesGPSDataPointList (const GPSDataPointList&  stats);

    ~PicesGPSDataPointList ();

    !PicesGPSDataPointList ();

    /**
     *@brief Duplicates the contsts of 'list' and adds to this instance.
     */
    void  AddList (PicesGPSDataPointList^ list);

    void  AddList (const GPSDataPointList&  stats);

    void  CleanUpMemory ();


  private:
  };  /* PicesGPSDataPointList */



}  /* PicesInterface */

