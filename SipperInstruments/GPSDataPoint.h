#if  !defined(_GPSDATAPOINT_)
#define _GPSDATAPOINT_


/**
 *@class MLL::GPSDataPoint
 *@details
 *  Keeps track of one GPS datapoint consisting of the DateTime,  Average Latitude and Longitude, of a given time interval.
 *  binDepth, and volumeSampled.
 *  
 *@author  Kurt Kramer
 */
#include <map>

#include "BasicTypes.h"
#include "DateTime.h"
#include "KKQueue.h"
using namespace KKU;


namespace  SipperHardware
{
  class GPSDataPoint
  {
  public:
    typedef  KKU::int32     int32;
    typedef  KKU::uint32    uint32;
    typedef  KKU::DateTime  DateTime;
    typedef  KKU::TimeType  TimeType;
    typedef  GPSDataPoint*  GPSDataPointPtr;

    GPSDataPoint ();

    GPSDataPoint::GPSDataPoint (const GPSDataPoint&  _dataPoint);


    GPSDataPoint::GPSDataPoint (DateTime _ctdDateTime,
                                TimeType _gpsStartTime,
                                int32    _sipperFileId,
                                uint32   _avgScanLine,
                                double   _avgLatitude,
                                double   _avgLongitude,
                                float    _avgFlowRate
                               );

    ~GPSDataPoint ();

    DateTime  CtdDateTime  () const {return ctdDateTime;}
    TimeType  GPSStartTime () const {return gpsStartTime;}
    int32     SipperFileId () const {return sipperFileId;}
    uint32    AvgScanLine  () const {return avgScanLine;}
    double    AvgLatitude  () const {return avgLatitude;}
    double    AvgLongitude () const {return avgLongitude;}
    float     AvgFlowRate  () const {return avgFlowRate;}

  private:
    DateTime  ctdDateTime;
    TimeType  gpsStartTime;   /**<  Time that this data point start covering from.  */
    int32     sipperFileId;
    uint32    avgScanLine;
    double    avgLatitude;
    double    avgLongitude;
    float     avgFlowRate;
  };
  typedef  GPSDataPoint::GPSDataPointPtr  GPSDataPointPtr;





  class GPSDataPointList:  public KKQueue<GPSDataPoint>  
  {
  public:
    typedef  KKU::int32   int32;
    typedef  GPSDataPointList*  GPSDataPointListPtr;

    GPSDataPointList (bool _owner);
  
    ~GPSDataPointList ();

 private:
  };
  
  typedef  GPSDataPointList::GPSDataPointListPtr  GPSDataPointListPtr;
}  /* SipperHardware */

#endif


