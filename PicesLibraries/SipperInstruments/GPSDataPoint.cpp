#include "FirstIncludes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include  "MemoryDebug.h"
using namespace std;



#include "BasicTypes.h"
using namespace KKU;


#include "GPSDataPoint.h"
using  namespace  SipperHardware;


GPSDataPoint::GPSDataPoint ():
    ctdDateTime  (),
    gpsStartTime (),
    sipperFileId (-1),
    avgScanLine  (0),
    avgLatitude  (0.0),
    avgLongitude (0.0),
    avgFlowRate  (0.0f)
{
}


GPSDataPoint::GPSDataPoint (const GPSDataPoint&  _dataPoint):
    ctdDateTime  (_dataPoint.ctdDateTime),
    gpsStartTime (_dataPoint.gpsStartTime),
    sipperFileId (_dataPoint.sipperFileId),
    avgScanLine  (_dataPoint.avgScanLine),
    avgLatitude  (_dataPoint.avgLatitude),
    avgLongitude (_dataPoint.avgLongitude),
    avgFlowRate  (_dataPoint.avgFlowRate)
{
}


GPSDataPoint::GPSDataPoint (DateTime _ctdDateTime,
                            TimeType _gpsStartTime,
                            int32    _sipperFileId,
                            uint32   _avgScanLine,
                            double   _avgLatitude,
                            double   _avgLongitude,
                            float    _avgFlowRate
                           ):
    ctdDateTime  (_ctdDateTime),
    gpsStartTime (_gpsStartTime),
    sipperFileId (_sipperFileId),
    avgScanLine  (_avgScanLine),
    avgLatitude  (_avgLatitude),
    avgLongitude (_avgLongitude),
    avgFlowRate  (_avgFlowRate)
{
}



GPSDataPoint::~GPSDataPoint ()
{
}




GPSDataPointList::GPSDataPointList (bool _owner):
    KKQueue<GPSDataPoint> (_owner)
{
}

  
GPSDataPointList::~GPSDataPointList ()
{
}
